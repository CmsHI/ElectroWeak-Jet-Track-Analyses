#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo -e "Usage: ./ShellScripts/gammaJet/run-all.sh [input dir] [output dir] [0/1]"
    echo -e "arg 3: [0: all, 1: arithmetic only]"
    echo -e "[input dir] should contain skim files"
    echo -e "[output dir] will contain histogram/arithmetic files"
    echo -e "[output dir] should contain histogram files (if running arithmetic only)"
    exit 1
fi

INDIR=$1
OUTDIR=$2
mkdir -p $OUTDIR

./ShellScripts/gammaJet/run-nominal.sh $@
./ShellScripts/gammaJet/run-systematics.sh $@

cp ${OUTDIR}/gammaJetHistograms*.root ./Data/photons/

./Systematics/gammaJet/gammaJetSystematics.exe CutConfigurations/gammaJet.conf Configurations/gammaJet/gammaJetSystematics.list Data/photons/gammaJetSystematics.root
