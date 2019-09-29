#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Histogramming/vJetTrk/trkSpectra.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

dirSpecial=""  # name of the directory where special/non-nominal output (if any) would be written
inputList=(
${forest_pbpb_2018_mc_mix_unmerged}
${forest_pbpb_2018_mc_zmm}
${forest_pbpb_2018_mc_zee}
#${forest_pbpb_2018_data_mix}
);

optionList=(
"--sampleType=pbpb_2018_mc"
"--sampleType=pbpb_2018_mc"
"--sampleType=pbpb_2018_mc"
#"--sampleType=pbpb_2018_data"
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
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_mix.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_zmm.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_zee.root"
#$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_data_mix.root"
);

outDirBaseSCRATCH=$EWJTAOUT
outDirBaseSCRATCH="/mnt/submit-hi2/d00/scratch/tatar/EWJTA-out"
if [ -z "$outDirBaseSCRATCH" ]; then
  outDirBaseSCRATCH="/export/d00/scratch/"$USER"/EWJTA-out"
fi

nJobs=0
#nFilesPerJob=1
nFilesPerJob=20

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile="DUMMY"
    rm -rf ${configFile}
    touch ${configFile}

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


