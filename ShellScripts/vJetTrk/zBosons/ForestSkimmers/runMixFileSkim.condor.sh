#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./ForestSkimmers/vJetTrk/mixFileSkim.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

dirSpecial=""  # name of the directory where special/non-nominal output (if any) would be written
inputList=(
${forest_pbpb_2018_mc_mix}
${forest_pbpb_2018_data_mix_1}
${forest_pbpb_2018_data_mix_2}
);

configList=(
"Configurations/vJetTrk/zBoson/mixFileSkim.pbpb.2018.mc.conf"
"Configurations/vJetTrk/zBoson/mixFileSkim.pbpb.2018.data.conf"
"Configurations/vJetTrk/zBoson/mixFileSkim.pbpb.2018.data.conf"
);

optionList=(
## pbpb 18 mc
"--jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
## pbpb 18 data
"--jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
);

outDirBase=$EWJTAOUTGRID
if [ -z "$outDirBase" ]; then
  USERGRID=$(voms-proxy-info -issuer | awk '{split($0,myLine,"/CN="); print myLine[2]}')
  if [ -z "$USERGRID" ]; then
    USERGRID=$USER
  fi
  outDirBase="/mnt/hadoop/cms/store/user/"$USERGRID"/EWJTA-out"
fi

outList=(
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/mixFileSkim_pbpb_2018_mc.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/mixFileSkim_pbpb_2018_data_1.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/mixFileSkim_pbpb_2018_data_2.root"
);

outDirBaseSCRATCH=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBaseSCRATCH="/export/d00/scratch/"$USER"/EWJTA-out"
fi

nJobs=0
nFilesPerJob=1

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    inputFile=${inputList[i1]}
    mixFile="NOFILE"
    options=${optionList[i1]}
    optsFile="Configurations/vJetTrk/zBoson/optsTmp.txt"
    echo ${options} > ${optsFile}
    outputFileTmp=${outList[i1]}
    timeNow=$(date +"%Y%m%d_%H%M%S")
    outputFile="${outputFileTmp/.root//condor_${timeNow}/DUMMY.root}"
    #outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    #mkdir -p $outDir

    outputFileLOG="${outputFile/\/DUMMY.root/.log}"
    outputFileLOG="${outputFileLOG/${outDirBase}/${outDirBaseSCRATCH}}"
    outDirLOG=$(dirname "${outputFileLOG}")
    mkdir -p $outDirLOG

    $runCmd ./ShellScripts/vJetTrk/Condor/condorSubmit.skim.sh $progPath $configFile $inputFile $outDir $mixFile ${optsFile} ${nJobs} ${nFilesPerJob} &> ${outputFileLOG}
    echo "$runCmd ./ShellScripts/vJetTrk/Condor/condorSubmit.skim.sh $progPath $configFile $inputFile $outDir $mixFile ${optsFile} ${nJobs} ${nFilesPerJob} &> ${outputFileLOG}"
done


