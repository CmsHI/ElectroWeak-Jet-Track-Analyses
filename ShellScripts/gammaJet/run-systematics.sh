#!/bin/bash

if [ $# -ne 4 ]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-systematics.sh [input dir] [output dir] [0/1/2] [0/1]"
    echo -e "arg 3: [0: histogram, 1: arithmetic, 2: final]"
    echo -e "arg 4: [0: exclude jes, 1: all]"
    echo -e "[input dir] should contain input files"
    echo -e "[output dir] will contain output files up to final output"
    echo -e "note: [output dir] should also contain nominal data arithmetic files (for isolation systematics)"
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
SYS_SUFFIX=(_JER _PES _PURITY_UP _PURITY_DOWN _ELE_REJ _JES_UP _JES_DOWN)

NSYS=$(( $4 == 0 ? 4 : 6))

for ITERSYS in $(seq 0 $NSYS)
do
    if [[ $3 -eq 0 ]]; then
        for ITERSAM in 0 1 2 3
        do
            CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}${SYS_SUFFIX[ITERSYS]}
            ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $INDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYS_SUFFIX[ITERSYS]}.root &> $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYS_SUFFIX[ITERSYS]}.log &
        done
        wait
    fi

    if [[ $3 -ne 2 ]]; then
        for ITERSAM in 0 1 2 3
        do
            CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}${SYS_SUFFIX[ITERSYS]}
            ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $ARITHINDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYS_SUFFIX[ITERSYS]}.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root &> $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.log &
        done
        wait
    fi

    ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet${SYS_SUFFIX[ITERSYS]}.conf ${FINALINDIR}/PbPb_Data_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root ${FINALINDIR}/PbPb_MC_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root ${FINALINDIR}/pp_Data_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root ${FINALINDIR}/pp_MC_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root ${OUTDIR}/gammaJetHistograms${SYS_SUFFIX[ITERSYS]}.root &> ${OUTDIR}/gammaJetHistograms${SYS_SUFFIX[ITERSYS]}.log
done

# photon isolation systematics
if [[ $3 -eq 0 ]]; then
    for ITERSAM in 1 3
    do
        CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}_ISO
        ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $INDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram_ISO.root &> $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram_ISO.log &
    done
    wait
fi

if [[ $3 -ne 2 ]]; then
    for ITERSAM in 1 3
    do
        CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}_ISO
        ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $ARITHINDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram_ISO.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic_ISO.root &> $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic_ISO.log &
    done
    wait
fi

./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root ${FINALINDIR}/PbPb_MC_gammaJetHistogramArithmetic_ISO.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root ${FINALINDIR}/pp_MC_gammaJetHistogramArithmetic_ISO.root ${OUTDIR}/gammaJetHistograms_ISO.root &> ${OUTDIR}/gammaJetHistograms_ISO.log
