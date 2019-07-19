#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Utilities/lhadd.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

dirGRID="/mnt/hadoop/cms/store/user/katatar/EWJTA-out"
dirRelative="vJetTrk/zBoson/ForestSkimmers"
dirBase=${dirGRID}"/"${dirRelative}

inputDirsRel=(
#"vJetTrkSkim_pbpb_2018_data_zmm/condor_20190703_105056"
#"vJetTrkSkim_pbpb_2018_data_zee/condor_20190703_105059"
#"vJetTrkSkim_pbpb_2018_mc_zmm/condor_20190703_212912"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_hiBin/condor_20190706_024144"
#"vJetTrkSkim_pbpb_2018_mc_zmm/condor_20190707_003835"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_hiBin_nVzBins5_nEvtPlaneBins4/condor_20190708_024549"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_hiHF_nVzBins5_nEvtPlaneBins8_nCentBins800_hiHF_minus_20/condor_20190710_173200"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_hiHF_nVzBins5_nEvtPlaneBins8_nCentBins800_hiHF_minus_30/condor_20190709_223644"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_hiHF_nVzBins5_nEvtPlaneBins8_nCentBins800_hiHF_minus_40/condor_20190710_173431"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_hiHF_nVzBins5_nEvtPlaneBins8_nCentBins400_hiHF_minus_30/condor_20190709_222824"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minus1p26/condor_20190714_015847"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minus1p32/condor_20190714_015618"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minus1p45/condor_20190714_014433"
#"vJetTrkSkim_pbpb_2018_mc_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minusRhoDiff/condor_20190714_175457"
##
#"vJetTrkSkim_pbpb_2018_data_zmm_match_hiHF_nCentBins800_nVzBins15_nEvtPlaneBins8_hiHF_minus_25/condor_20190715_034425"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_hiHF_nCentBins800_nVzBins15_nEvtPlaneBins8_hiHF_minus_30/condor_20190715_034127"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_hiHF_nCentBins800_nVzBins15_nEvtPlaneBins8_hiHF_minus_35/condor_20190715_034532"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_hiHF_nCentBins800_nVzBins15_nEvtPlaneBins8_hiHF_minus_40/condor_20190715_034651"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minus0p5/condor_20190715_042220"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minus1p26/condor_20190714_020950"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minus1p32/condor_20190714_021201"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minus1p45/condor_20190714_021617"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minus2p0/condor_20190715_042448"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_rho_nCentBins800_nVzBins15_nEvtPlaneBins8_minusRhoDiff/condor_20190714_184951"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_hiHF_nCentBins800_nVzBins15_nEvtPlaneBins8_hiHF_minus_50/condor_20190715_165459"
#"vJetTrkSkim_pbpb_2018_data_zmm_match_hiHF_nCentBins800_nVzBins15_nEvtPlaneBins8_hiHF_minus_60/condor_20190715_165648"
"vJetTrkSkim_pbpb_2018_data_zmm_match_hiHF_nCentBins800_nVzBins15_nEvtPlaneBins8_hiHF_minus_20/condor_20190715_202129"
);

logDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  logDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

arrayIndices=${!inputDirsRel[*]}
for i1 in $arrayIndices
do
    listTmp="condorSubmit_lhadd_vJetTrk.list"
    inputDirRel=${inputDirsRel[i1]}
    inputDir=${dirBase}"/"${inputDirRel}
    ls -1 ${inputDir}/*.root > $listTmp

    outDir=${inputDir}"/merged"
    # nJobs="1"
    # nFilesPerJob="-1"

    logFile=${logDirBase}"/"${relDirCondorCreateLog}"/lhadd_"${inputDirRel}".log"
    logFileDir=$(dirname "${logFile}")
    mkdir -p $logFileDir

    $runCmd ./ShellScripts/Condor/condorSubmit_lhadd.sh $progPath $listTmp ${outDir} 1 -1 &> ${logFile}
    echo "$runCmd ./ShellScripts/Condor/condorSubmit_lhadd.sh $progPath $listTmp ${outDir} 1 -1 &> ${logFile}"

    rm -rf $listTmp
done

