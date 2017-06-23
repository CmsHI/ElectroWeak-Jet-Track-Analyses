#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-nominal.sh [input dir] [output dir] [0/1/2]"
    echo -e "arg 3: [0: histogram, 1: arithmetic, 2: final]"
    echo -e "[input dir] should contain input files"
    echo -e "[output dir] will contain output files up to final output"
    exit 1
fi

set -x

INDIR=$1
OUTDIR=$2
mkdir -p $OUTDIR

ARITHINDIR=$([[ $3 -eq 0 ]] && echo "$OUTDIR" || echo "$INDIR")
FINALINDIR=$([[ $3 -eq 2 ]] && echo "$INDIR" || echo "$OUTDIR")

SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)

SAM_SUFFIX=("" _mc _pp _pp_mc)

if [[ $3 -eq 0 ]]; then
    for ITERSAM in ${!SAM_SUFFIX[@]}
    do
        CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}
        ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $INDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.root &> $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.log &
    done
    wait
fi

if [[ $3 -ne 2 ]]; then
    for ITERSAM in ${!SAM_SUFFIX[@]}
    do
        CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}
        ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $ARITHINDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic.root &> $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic.log &
    done
    wait
fi

./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${FINALINDIR}/PbPb_Data_gammaJetHistogramArithmetic.root ${FINALINDIR}/PbPb_MC_gammaJetHistogramArithmetic.root ${FINALINDIR}/pp_Data_gammaJetHistogramArithmetic.root ${FINALINDIR}/pp_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/gammaJetHistograms.root &> ${OUTDIR}/gammaJetHistograms.log
