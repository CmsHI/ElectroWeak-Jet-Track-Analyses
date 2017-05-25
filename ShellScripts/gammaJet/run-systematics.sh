#!/bin/bash

if [ $# -ne 1 ]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-systematics.sh [label]"
    exit 1
fi

set -x

LABEL=$1

OUTDIR=/export/d00/scratch/biran/gamma-jet/${LABEL}
mkdir -p $OUTDIR

SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
CONFSUFFIXES=("" _mc _pp _pp_mc)

SYSTEMATIC=(_JER _PES _PURITY_UP _PURITY_DOWN _ELE_REJ)

for ITERSYS in 0 1 2 3 4
do
    for ITERSAM in 0 1 2 3
    do
        ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root
        CONFSUFFIX=${CONFSUFFIXES[ITERSAM]}${SYSTEMATIC[ITERSYS]}
        ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root
    done

    ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet${SYSTEMATIC[ITERSYS]}.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/gammaJetHistograms${SYSTEMATIC[ITERSYS]}.root
done

ITERSYS=5
for ITERSAM in 1 3
do
    ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root
    CONFSUFFIX=${CONFSUFFIXES[ITERSAM]}${SYSTEMATIC[ITERSYS]}
    ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root
done

./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/gammaJetHistograms${SYSTEMATIC[ITERSYS]}.root

# calculate systematics
