#!/bin/bash -e

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

export PULPRT_HOME=$scriptDir