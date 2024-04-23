#!/bin/bash -e

export PULPRT_TARGET=pulpissimo
export PULPRUN_TARGET=pulpissimo
export USE_CV32E40X=1

if [  -n "${ZSH_VERSION:-}" ]; then
        DIR="$(readlink -f -- "${(%):-%x}")"
        scriptDir="$(dirname $DIR)"
else

    scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

fi

source $scriptDir/common.sh
