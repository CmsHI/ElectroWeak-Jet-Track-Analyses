#!/bin/bash

if [ $# -ne 5 ]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-skims.sh [0/1] [0/1] [hadoop dir] [output dir] [mixing dir]"
    echo -e "arg 1: [0: skim + histogram, 1: merge + produce final results]"
    echo -e "arg 2: [0: results, 1: systematics]"
    echo -e "[hadoop dir] contains condor output, [output dir] will contain merged skims/histograms"
    exit 1
fi

set -x

HADOOPDIR=$3
mkdir -p $HADOOPDIR
OUTDIR=$4
mkdir -p $OUTDIR

SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
INFILE=(./ShellScripts/gammaJet/PbPb_Data_HiForest.list ./ShellScripts/gammaJet/PbPb_MC_Flt30_HiForest.list /mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root ./ShellScripts/gammaJet/pp_MC_HiForest.list)
NJOBS=(200 100 200 5)
MIXINGSAMPLE=(${4}/PbPb_Data_minbiasJetSkim.root ${4}/PbPb_MC_minbiasJetSkim.root DUMMY.root DUMMY.root)
CONFSUFFIXES=("" _mc _pp _pp_mc)

if [ $2 -eq 0 ]; then
    NSYS=0
    SYSTEMATIC=("")
elif [ $2 -eq 1 ]; then
    NSYS=1
    SYSTEMATIC=(_JES_UP _JES_DOWN)
fi

if [ $1 -eq 0 ]; then
    for ITERSAM in 0 1 2 3
    do
        for ITERSYS in $(seq 0 $NSYS)
        do
            CONFSUFFIX=${CONFSUFFIXES[ITERSAM]}${SYSTEMATIC[ITERSYS]}
            mkdir -p $HADOOPDIR/${SAMPLE[ITERSAM]}${SYSTEMATIC[ITERSYS]}_unmerged/
            ./ShellScripts/gammaJet/gamma-jet-condor.sh ./CutConfigurations/gammaJet${CONFSUFFIX}.conf ${INFILE[ITERSAM]} $HADOOPDIR/${SAMPLE[ITERSAM]}${SYSTEMATIC[ITERSYS]}_unmerged/ ${NJOBS[ITERSAM]} ${MIXINGSAMPLE[ITERSAM]}
        done
    done
fi

if [ $1 -eq 1 ]; then
    for ITERSAM in 0 1 2 3
    do
        hadd $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $HADOOPDIR/${SAMPLE[ITERSAM]}_unmerged/gammaJetSkim*.root
    done

    for ITERSYS in $(seq 0 $NSYS)
    do
        for ITERSAM in 0 1 2 3
        do
            hadd $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root $HADOOPDIR/${SAMPLE[ITERSAM]}${SYSTEMATIC[ITERSYS]}_unmerged/gammaJetHistogram*.root
            CONFSUFFIX=${CONFSUFFIXES[ITERSAM]}${SYSTEMATIC[ITERSYS]}
            ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYSTEMATIC[ITERSYS]}.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root
        done

        ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet${SYSTEMATIC[ITERSYS]}.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic${SYSTEMATIC[ITERSYS]}.root ${OUTDIR}/gammaJetHistograms${SYSTEMATIC[ITERSYS]}.root
    done
fi
