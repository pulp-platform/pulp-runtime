#!/usr/bin/env python3

# Copyright 2020 ETH Zurich and University of Bologna
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Run shell commands listed in a file separated by newlines in a parallel
# fashion. If requested the results (tuples consisting of command, stdout,
# stderr and returncode) will be gathered in a junit.xml file. There a few
# knobs to tune the number of spawned processes and the junit.xml formatting.

# Author: Robert Balas (balasr@iis.ee.ethz.ch)

import argparse
from subprocess import (Popen, TimeoutExpired,
                        CalledProcessError, PIPE)
import sys
import signal
import os
import multiprocessing
import errno
import pprint


runtest = argparse.ArgumentParser(prog='bwruntests',
                                  description="""Run PULP tests in parallel""")

runtest.version = '0.1'

runtest.add_argument('test_file', type=str,
                     help='file defining tests to be run')
runtest.add_argument('-p', '--max-procs', type=int,
                     default=multiprocessing.cpu_count(),
                     help="""Number of parallel
                     processes used to run test.
                     Default is number of cpu cores.""")
runtest.add_argument('-t', '--timeout', type=float,
                     default=None,
                     help="""Timeout for all processes in seconds""")
runtest.add_argument('-v', '--verbose', action='store_true',
                     help="""Enable verbose output""")
runtest.add_argument('--report-junit', action='store_true',
                     help="""Generate a junit report""")
runtest.add_argument('--disable-junit-pp', action='store_false',
                     help="""Disable pretty print of junit report""")
runtest.add_argument('-o,', '--output', type=str,
                     help="""Write to file instead of stdout""")


class FinishedProcess(object):
    """A process that has finished running.
    """
    def __init__(self, args, returncode, stdout=None, stderr=None):
        self.args = args
        self.returncode = returncode
        self.stdout = stdout
        self.stderr = stderr

    def __repr__(self):
        args = ['args={!r}'.format(self.args),
                'returncode={!r}'.format(self.returncode)]
        if self.stdout is not None:
            args.append('stdout={!r}'.format(self.stdout))
        if self.stderr is not None:
            args.append('stderr={!r}'.format(self.stderr))
        return "{}({})".format(type(self).__name__, ', '.join(args))


def fork(*popenargs, check=False, shell=True,
         **kwargs):
    """Run subprocess and return process args, error code, stdout and stderr
    """

    kwargs['stdout'] = PIPE
    kwargs['stderr'] = PIPE

    with Popen(*popenargs, preexec_fn=os.setpgrp, **kwargs) as process:
        try:
            # Child and parent are racing for setting/using the pgid so we have
            # to set it in both processes. See glib manual.
            try:
                os.setpgid(process.pid, process.pid)
            except OSError as e:
                if e.errno != errno.EACCES:
                    raise

            stdout, stderr = process.communicate(input, timeout=args.timeout)
        except TimeoutExpired:
            pgid = os.getpgid(process.pid)
            os.killpg(pgid, signal.SIGKILL)
            # process.kill() will only kill the immediate child but not its
            # forks. This won't work since our commands will create a few forks
            # (make -> vsim -> etc). We need to make a process group and kill
            # that
            stdout, stderr = process.communicate()
            return FinishedProcess(process.args, 1,
                                   stdout.decode('utf-8'),
                                   'TIMEOUT after {:f}s\n'.format(args.timeout)
                                   + stderr.decode('utf-8'))
        # Including KeyboardInterrupt, communicate handled that.
        except:  # noqa: E722
            pgid = os.getpgid(process.pid)
            os.killpg(pgid, signal.SIGKILL)
            # We don't call process.wait() as .__exit__ does that for us.
            raise
        retcode = process.poll()
        if check and retcode:
            raise CalledProcessError(retcode, process.args,
                                     output=stdout, stderr=stderr)
    return FinishedProcess(process.args, retcode,
                           stdout.decode('utf-8'),
                           stderr.decode('utf-8'))


if __name__ == '__main__':
    args = runtest.parse_args()
    pp = pprint.PrettyPrinter(indent=4)

    # lazy importing so that we can work without junit_xml
    if args.report_junit:
        try:
            from junit_xml import TestSuite, TestCase
        except ImportError:
            print("""The --report-junit option requires
the junit_xml module which is not installed.""",
                  file=sys.stderr)
            exit(1)

    # load command list
    tests = []
    with open(args.test_file) as f:
        tests = list(map(str.rstrip, f))

    if args.verbose:
        print('Tests which we are running:')
        pp.pprint(tests)

    # list of commands to be run
    shellcmds = [['make', '-C', e, 'clean', 'all', 'run'] for e in tests]
    if args.verbose:
        print('Generated shell commands:')
        pp.pprint(shellcmds)

    # by default we use the number of available cores to limit the number of
    # concurrently spawned process
    pool = multiprocessing.Pool(processes=args.max_procs)
    procresults = pool.map(fork, shellcmds)
    pp.pprint(procresults)

    # Generate junit.xml file. Junit.xml differentiates between failure and
    # errors but we treat everything as errors.
    if args.report_junit:
        testcases = []
        for p in procresults:
            testcase = TestCase(' '.join(p.args), stdout=p.stdout,
                                stderr=p.stderr)
            if p.returncode != 0:
                testcase.add_error_info(p.stderr)
            testcases.append(testcase)

        testsuite = TestSuite("bwruntests", testcases)
        if args.output:
            with open(args.output, 'w') as f:
                TestSuite.to_file(f, [testsuite],
                                  prettyprint=args.disable_junit_pp)
        else:
            print(TestSuite.to_xml_string([testsuite],
                                          prettyprint=args.disable_junit_pp))
