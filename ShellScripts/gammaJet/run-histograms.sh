#!/bin/bash

if [ $# -ne 2 ]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-histograms.sh [input dir] [output dir]"
    echo -e "[input dir] should contain *_gammaJetSkim.root files"
    exit 1
fi

set -x

INDIR=$1
OUTDIR=$2
mkdir -p $OUTDIR

SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
SAM_SUFFIX=("" _mc _pp _pp_mc)

for ITERSAM in 0 1 2 3
do
    CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}
    ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $INDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.root
    ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic.root
done

./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/gammaJetHistograms.root
