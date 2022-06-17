#!/bin/bash -e

export PULPRT_TARGET=control-pulp
export PULPRUN_TARGET=control-pulp

if [  -n "${ZSH_VERSION:-}" ]; then
    DIR="$(readlink -f -- "${(%):-%x}")"
    scriptDir="$(dirname $DIR)"
else
    scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"
fi

source $scriptDir/../../common.sh

export PULPRUN_PLATFORM=fpga

export PULPRT_CONFIG_CFLAGS='-DARCHI_FPGA_PER_FREQUENCY=10000000 \
       -DARCHI_FPGA_FC_FREQUENCY=20000000 \
       -DARCHI_FPGA_CL_FREQUENCY=20000000'

export io=uart
