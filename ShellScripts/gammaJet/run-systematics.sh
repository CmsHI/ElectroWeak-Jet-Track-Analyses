#!/bin/bash

if [ $# -ne 2 ]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-systematics.sh [input dir] [output dir]"
    echo -e "[input dir] should contain *_gammaJetSkim.root files"
    exit 1
fi

set -x

INDIR=$1
OUTDIR=$2
mkdir -p $OUTDIR

SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
CONFSUFFIXES=("" _mc _pp _pp_mc)

SYSTEMATIC=(_JER _PES _PURITY_UP _PURITY_DOWN _ELE_REJ)
for ITERSYS in 0 1 2 3 4
do
    for ITERSAM in 0 1 2 3
    do
        CONFSUFFIX=${CONFSUFFIXES[ITERSAM]}${SYSTEMATIC[ITERSYS]}
        ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $INDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root
        ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root
    done

    ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet${SYSTEMATIC[ITERSYS]}.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/gammaJetHistograms${SYSTEMATIC[ITERSYS]}.root
done

# photon isolation systematics
for ITERSAM in 1 3
do
    CONFSUFFIX=${CONFSUFFIXES[ITERSAM]}_ISO
    ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $INDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram_ISO.root
    ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram_ISO.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic_ISO.root
done

./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic_ISO.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic_ISO.root ${OUTDIR}/gammaJetHistograms_ISO.root

# calculate systematics
