#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo -e "Usage: ./ShellScripts/gammaJet/run-purity.sh [input dir] [output dir]"
    echo -e "[input dir] should contain skim files"
    echo -e "[output dir] will contain log files"
    exit 1;
fi

set -x

INDIR=$1
OUTDIR=$2

DATA_SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
MC_SAMPLE=(PbPb_MC PbPb_MC pp_MC pp_MC)

SAM_SUFFIX=("" _mc _pp _pp_mc)
SYS_SUFFIX=("" _PURITY_UP _PURITY_DOWN)

for ITERSYS in 0 1 2
do
    for ITERSAM in 0 1 2 3
    do
        CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}${SYS_SUFFIX[ITERSYS]}
        ./Performance/printPurity.exe CutConfigurations/gammaJet${CONFSUFFIX}.conf ${INDIR}/${DATA_SAMPLE[ITERSAM]}_gammaJetSkim.root ${INDIR}/${MC_SAMPLE[ITERSAM]}_gammaJetSkim.root 1 &> ${OUTDIR}/${DATA_SAMPLE[ITERSAM]}_purity${SYS_SUFFIX[ITERSYS]}.log &
    done
    wait

    for ITERSAM in 0 1 2 3
    do
        CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}${SYS_SUFFIX[ITERSYS]}
        PURITY=$(tail -n1 ${OUTDIR}/${DATA_SAMPLE[ITERSAM]}_purity${SYS_SUFFIX[ITERSYS]}.log)

        sed -i "s/histogram.photon.purity = .*/${PURITY}/g" CutConfigurations/gammaJet${CONFSUFFIX}.conf
    done
done
