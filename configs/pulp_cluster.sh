#!/bin/bash -e

export PULPRT_TARGET=pulp_cluster
export PULPRUN_TARGET=pulp_cluster
export CONFIG_NO_FC=1
# export ARCHI_HMR=1

if [  -n "${ZSH_VERSION:-}" ]; then
        DIR="$(readlink -f -- "${(%):-%x}")"
        scriptDir="$(dirname $DIR)"
else

    scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

fi

source $scriptDir/common.sh
