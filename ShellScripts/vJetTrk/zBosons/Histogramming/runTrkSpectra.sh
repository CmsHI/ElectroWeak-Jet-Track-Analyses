#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Histogramming/vJetTrk/trkSpectra.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

### USER INPUT START
dirSpecial=""  # name of the directory where special/non-nominal output (if any) would be written
sampleFlags=(
0 # pp_2017_mc_mix
0 # pp_2017_mc_zmm
0 # pp_2017_mc_zee
##
1 # pbpb_2018_mc_mix
1 # pbpb_2018_mc_zmm
1 # pbpb_2018_mc_zee
0 # pbpb_2018_data_mix_1
0 # pbpb_2018_data_mix
);
### USER INPUT END

inputList=(
${forest_pp_2017_mc_mix}
${forest_pp_2017_mc_zmm}
${forest_pp_2017_mc_zee}
##
${forest_pbpb_2018_mc_mix}
${forest_pbpb_2018_mc_zmm}
${forest_pbpb_2018_mc_zee}
${forest_pbpb_2018_data_mix_1}
${forest_pbpb_2018_data_mix}
);

optionList=(
"--sampleType=pp_2017_mc --skipMu=0 --skipEle=0 --th1Names=NULL --cents=0:100 --trkEtas=0:2.4,0:1.6,0:1.0 --trkPts=1:0,1:2,2:4,4:8,8:20,20:0" # --maxNVtx=1
"--sampleType=pp_2017_mc --skipMu=1 --skipEle=1 --th1Names=NULL --cents=0:100 --trkEtas=0:2.4,0:1.6,0:1.0 --trkPts=1:0,1:2,2:4,4:8,8:20,20:0"
"--sampleType=pp_2017_mc --skipMu=1 --skipEle=1 --th1Names=NULL --cents=0:100 --trkEtas=0:2.4,0:1.6,0:1.0 --trkPts=1:0,1:2,2:4,4:8,8:20,20:0"
##
"--sampleType=pbpb_2018_mc --skipMu=1 --skipEle=1 --anajets=1 --jetptMin=30"
"--sampleType=pbpb_2018_mc --skipMu=1 --skipEle=1 --anajets=1 --jetptMin=30"
"--sampleType=pbpb_2018_mc --skipMu=1 --skipEle=1"
"--sampleType=pbpb_2018_data"
"--sampleType=pbpb_2018_data"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

outList=(
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pp_2017_mc_mix_trkPt_trkEta.root" # maxNVtx1
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pp_2017_mc_zmm_trkPt_trkEta.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pp_2017_mc_zee_trkPt_trkEta.root"
##
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_mix.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_zmm.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_mc_zee.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_data_mix_1.root"
$outDirBase"/"${relDirHist}"/"${dirSpecial}"/trkSpectra_pbpb_2018_data_mix.root"
);

arrayIndices=${!sampleFlags[*]}
for i1 in $arrayIndices
do
  if [ ${sampleFlags[i1]} == 1 ]; then

    configFile="DUMMY"
    inputFile=${inputList[i1]}
    options=${optionList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir

    $runCmd $progPath $configFile $inputFile $outputFile ${options} &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile ${options} &> $outputFileLOG &"

  fi
done
