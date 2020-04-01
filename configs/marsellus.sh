#!/bin/bash -e

export PULPRT_TARGET=marsellus
export PULPRUN_TARGET=marsellus

if [  -n "${ZSH_VERSION:-}" ]; then 
        DIR="$(readlink -f -- "${(%):-%x}")"
        scriptDir="$(dirname $DIR)"
else

    scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

fi

source $scriptDir/common.sh
