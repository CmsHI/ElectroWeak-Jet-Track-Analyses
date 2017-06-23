#!/bin/bash

if [[ $# -ne 4 ]]; then
    echo "Usage: ./ShellScripts/gammaJet/run-small-variations.sh [input skim] [output dir] [nsteps] [arithmetic file dir]"
    exit 1
fi

INPUTFILE=$1
OUTPUTDIR=$2
NSTEPS=$3
ARITHFILESDIR=$4

set -x

SYS_SUFFIX=(_JER _JES_UP _JES_DOWN)

for ISYS in ${!SYS_SUFFIX[@]}; do
    CONFSUFFIX=${SYS_SUFFIX[ISYS]}
    ./ShellScripts/gammaJet/small-variations-condor.sh ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $INPUTFILE $OUTPUTDIR/PbPb_Data$CONFSUFFIX $NSTEPS ${SYS_SUFFIX[ISYS]} $ARITHFILESDIR
done
