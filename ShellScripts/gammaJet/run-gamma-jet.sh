#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo -e "Usage: ./ShellScipts/gammaJet/run-skims.sh [hadoop dir]"
    echo -e "[hadoop dir] will contain condor output"
    exit 1
fi

set -x

HADOOPDIR=$1
MIXDIR=/mnt/hadoop/cms/store/user/biran/gamma-jet/MB_skims

SAMPLE=(PbPb_Data PbPb_MC pp_Data pp_MC)
SAM_SUFFIX=("" _mc _pp _pp_mc)
INFILE=(./ShellScripts/gammaJet/PbPb_Data_HiForest.list ./ShellScripts/gammaJet/PbPb_MC_Flt30_HiForest.list /mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root ./ShellScripts/gammaJet/pp_MC_HiForest.list)
MIXINGSAMPLE=(${MIXDIR}/PbPb_Data_minbiasJetSkim.root ${MIXDIR}/PbPb_MC_minbiasJetSkim.root DUMMY.root DUMMY.root)
NJOBS=(400 100 200 5)

if [[ $3 -eq 0 ]]; then
    for ISAM in ${!SAM_SUFFIX[@]}
    do
        ./ShellScripts/gammaJet/gamma-jet-condor.sh ./CutConfigurations/gammaJet${SAM_SUFFIX[ISAM]}.conf ${INFILE[ISAM]} $HADOOPDIR/${SAMPLE[ISAM]}_unmerged/ ${NJOBS[ISAM]} ${MIXINGSAMPLE[ISAM]}
    done
fi
