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
MIXDIR=$5

SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
INFILE=(./ShellScripts/gammaJet/PbPb_Data_HiForest.list ./ShellScripts/gammaJet/PbPb_MC_Flt30_HiForest.list /mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root ./ShellScripts/gammaJet/pp_MC_HiForest.list)
MIXINGSAMPLE=(${MIXDIR}/PbPb_Data_minbiasJetSkim.root ${MIXDIR}/PbPb_MC_minbiasJetSkim.root DUMMY.root DUMMY.root)
NJOBS=(200 100 200 5)

SAM_SUFFIX=("" _mc _pp _pp_mc)

if [ $2 -eq 0 ]; then
    NSYS=0
    SYS_SUFFIX=("")
elif [ $2 -eq 1 ]; then
    NSYS=1
    SYS_SUFFIX=(_JES_UP _JES_DOWN)
fi

if [ $1 -eq 0 ]; then
    for ITERSAM in 0 1 2 3
    do
        for ITERSYS in $(seq 0 $NSYS)
        do
            CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}${SYS_SUFFIX[ITERSYS]}
            mkdir -p $HADOOPDIR/${SAMPLE[ITERSAM]}${SYS_SUFFIX[ITERSYS]}_unmerged/
            ./ShellScripts/gammaJet/gamma-jet-condor.sh ./CutConfigurations/gammaJet${CONFSUFFIX}.conf ${INFILE[ITERSAM]} $HADOOPDIR/${SAMPLE[ITERSAM]}${SYS_SUFFIX[ITERSYS]}_unmerged/ ${NJOBS[ITERSAM]} ${MIXINGSAMPLE[ITERSAM]}
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
            hadd $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYS_SUFFIX[ITERSYS]}.root $HADOOPDIR/${SAMPLE[ITERSAM]}${SYS_SUFFIX[ITERSYS]}_unmerged/gammaJetHistogram*.root
            CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}${SYS_SUFFIX[ITERSYS]}
            ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram${SYS_SUFFIX[ITERSYS]}.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root
        done

        ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet${SYS_SUFFIX[ITERSYS]}.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic${SYS_SUFFIX[ITERSYS]}.root ${OUTDIR}/gammaJetHistograms${SYS_SUFFIX[ITERSYS]}.root
    done
fi
