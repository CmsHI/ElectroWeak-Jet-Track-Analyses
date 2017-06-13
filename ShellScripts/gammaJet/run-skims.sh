#!/bin/bash

if [[ $# -ne 5 ]]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-skims.sh [hadoop dir] [output dir] [0/1]"
    echo -e "arg 4: [0: skim, 1: merge + arithmetic]"
    echo -e "[hadoop dir] will contain condor output"
    echo -e "[output dir] will contain merged skims/histograms"
    exit 1
fi

set -x

HADOOPDIR=$1
OUTDIR=$2
mkdir -p $OUTDIR

MIXDIR=/mnt/hadoop/cms/store/user/biran/gamma-jet/MB_skims

SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
INFILE=(./ShellScripts/gammaJet/PbPb_Data_HiForest.list ./ShellScripts/gammaJet/PbPb_MC_Flt30_HiForest.list /mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root ./ShellScripts/gammaJet/pp_MC_HiForest.list)
MIXINGSAMPLE=(${MIXDIR}/PbPb_Data_minbiasJetSkim.root ${MIXDIR}/PbPb_MC_minbiasJetSkim.root DUMMY.root DUMMY.root)
NJOBS=(400 100 200 5)

SAM_SUFFIX=("" _mc _pp _pp_mc)

if [[ $4 -eq 0 ]]; then
    for ITERSAM in 0 1 2 3
    do
        if [[ $5 -ne 2 ]]; then
            CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}
            ./ShellScripts/gammaJet/gamma-jet-condor.sh ./CutConfigurations/gammaJet${CONFSUFFIX}.conf ${INFILE[ITERSAM]} $HADOOPDIR/${SAMPLE[ITERSAM]}_unmerged/ ${NJOBS[ITERSAM]} ${MIXINGSAMPLE[ITERSAM]}
        fi
    done
fi

if [[ $4 -eq 1 ]]; then
    for ITERSAM in 0 1 2 3
    do
        hadd $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetSkim.root $HADOOPDIR/${SAMPLE[ITERSAM]}_unmerged/gammaJetSkim*.root &
        hadd $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.root $HADOOPDIR/${SAMPLE[ITERSAM]}_unmerged/gammaJetHistogram*.root &
    done
    wait
fi

if [[ $4 -ne 0 ]]; then
    for ITERSAM in 0 1 2 3
    do
        CONFSUFFIX=${SAM_SUFFIX[ITERSAM]}
        ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogram.root $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic.root &> $OUTDIR/${SAMPLE[ITERSAM]}_gammaJetHistogramArithmetic.log &
    done
    wait

    ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/gammaJetHistograms.root &> ${OUTDIR}/gammaJetHistograms.log
fi
