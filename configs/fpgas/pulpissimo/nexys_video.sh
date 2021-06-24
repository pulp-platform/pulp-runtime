#!/bin/bash -e

export PULPRT_TARGET=pulpissimo
export PULPRUN_TARGET=pulpissimo

if [  -n "${ZSH_VERSION:-}" ]; then 
    DIR="$(readlink -f -- "${(%):-%x}")"
    scriptDir="$(dirname $DIR)"
else

    scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

fi

source $scriptDir/../../common.sh

export PULPRUN_PLATFORM=fpga

#Nexys Video uses 5MHz SoC Frequency
export PULPRT_CONFIG_CFLAGS='-DARCHI_FPGA_PER_FREQUENCY=5000000 -DARCHI_FPGA_SOC_FREQUENCY=10000000'

export io=uart
