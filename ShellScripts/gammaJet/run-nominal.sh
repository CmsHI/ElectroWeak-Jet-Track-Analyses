#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-histograms.sh [input dir] [output dir] [0/1]"
    echo -e "arg 3: [0: all, 1: arithmetic only]"
    echo -e "[input dir] should contain skim files"
    echo -e "[output dir] will contain histogram and arithmetic files"
    echo -e "[output dir] should contain histogram files (if running arithmetic only)"
    exit 1
fi

set -x

INDIR=$1
OUTDIR=$2
mkdir -p $OUTDIR

SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)

SAM_SUFFIX=("" _mc _pp _pp_mc)

if [[ $3 -ne 1 ]]; then
    for ITERSAM in 0 1 2 3
    do
        CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}
        ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $INDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.root &> $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.log &
    done
    wait
fi

for ITERSAM in 0 1 2 3
do
    CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}
    ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic.root &> $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic.log &
done
wait

./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/gammaJetHistograms.root &> ${OUTDIR}/gammaJetHistograms.log
