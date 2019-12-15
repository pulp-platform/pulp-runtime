#!/bin/bash -e

export PULPRT_TARGET=pulpissimo
export PULPRUN_TARGET=pulpissimo

scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

if [ -e ${scriptDir}/../init.sh ]; then
    source ${scriptDir}/../init.sh
fi
