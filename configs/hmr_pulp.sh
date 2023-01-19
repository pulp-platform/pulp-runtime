#!/bin/bash -e

export PULPRT_TARGET=pulp
export PULPRUN_TARGET=pulp
export USE_CV32E40P=1
export ARCHI_HMR=1

if [  -n "${ZSH_VERSION:-}" ]; then 
        DIR="$(readlink -f -- "${(%):-%x}")"
        scriptDir="$(dirname $DIR)"
else

    scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

fi

source $scriptDir/common.sh
