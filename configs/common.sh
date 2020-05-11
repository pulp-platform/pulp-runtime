#!/bin/bash -e

if [  -n "${ZSH_VERSION:-}" ]; then 
        DIR="$(readlink -f -- "${(%):-%x}")"
        DIRNAME="$(dirname $DIR)"
        scriptDir="$(dirname $DIRNAME)"
else

scriptDir="$(dirname "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")")"

fi

export PULPRT_HOME=$scriptDir
export PULP_SDK_HOME=$PULPRT_HOME