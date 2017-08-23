#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-skims.sh [hadoop dir] [output dir] [0/1]"
    echo -e "arg 3: [0: skim, 1: merge]"
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

SYS_SUFFIX=("" _PES _PURITY_UP _PURITY_DOWN _ELE_REJ _ISO _JER _JES_UP _JES_DOWN _QUARK _GLUON)
SAM_SUFFIX=("" _mc _pp _pp_mc)

if [[ $3 -eq 0 ]]; then
    for ISAM in ${!SAM_SUFFIX[@]}
    do
        ./ShellScripts/gammaJet/gamma-jet-condor.sh ./CutConfigurations/gammaJet${SAM_SUFFIX[ISAM]}.conf ${INFILE[ISAM]} $HADOOPDIR/${SAMPLE[ISAM]}_unmerged/ ${NJOBS[ISAM]} ${MIXINGSAMPLE[ISAM]}
    done
fi

if [[ $3 -eq 1 ]]; then
    for ISAM in ${!SAM_SUFFIX[@]}
    do
        hadd $OUTDIR/${SAMPLE[ISAM]}_gammaJetSkim.root $HADOOPDIR/${SAMPLE[ISAM]}_unmerged/gammaJetSkim*.root &
    done
    wait

    for ISYS in ${!SYS_SUFFIX[@]}
    do
        for ISAM in ${!SAM_SUFFIX[@]}
        do
            CONFSUFFIX=${SAM_SUFFIX[ISAM]}${SYS_SUFFIX[ISYS]}
            ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet${CONFSUFFIX}.conf $OUTDIR/${SAMPLE[ISAM]}_gammaJetHistogram${SYS_SUFFIX[ISYS]}.root $OUTDIR/${SAMPLE[ISAM]}_gammaJetHistogramArithmetic${SYS_SUFFIX[ISYS]}.root &> $OUTDIR/${SAMPLE[ISAM]}_gammaJetHistogramArithmetic${SYS_SUFFIX[ISYS]}.log &
        done
        wait

        ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic${SYS_SUFFIX[ISYS]}.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic${SYS_SUFFIX[ISYS]}.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic${SYS_SUFFIX[ISYS]}.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic${SYS_SUFFIX[ISYS]}.root ${OUTDIR}/gammaJetHistograms${SYS_SUFFIX[ISYS]}.root &> ${OUTDIR}/gammaJetHistograms${SYS_SUFFIX[ISYS]}.log
    done
fi
