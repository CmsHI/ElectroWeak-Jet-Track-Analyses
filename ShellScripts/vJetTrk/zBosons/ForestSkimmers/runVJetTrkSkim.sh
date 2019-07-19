#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./ForestSkimmers/vJetTrk/vJetTrkSkim.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

### USER INPUT START
dirSpecial=""  # name of the directory where special/non-nominal output (if any) would be written
sampleFlags=(
1 # pp_2017_mc_zmm
0 # pp_2017_mc_zee
0 # pp_2017_data_zmm
0 # pp_2017_data_zee
##
0 # pbpb_2018_mc_zmm
0 # pbpb_2018_mc_zee
0 # pbpb_2018_data_zmm
0 # pbpb_2018_data_zee
##
0 # pp_2015_mc_zmm
0 # pp_2015_mc_zee
0 # pp_2015_data_zmm
0 # pp_2015_data_zee
##
0 # pbpb_2015_mc_zmm
0 # pbpb_2015_mc_zee
0 # pbpb_2015_data_zmm
0 # pbpb_2015_data_zee
## 
0 # pbpb_2018_mc_zmm_noPU
);
### USER INPUT END

inputList=(
${forest_pp_2017_mc_zmm}
${forest_pp_2017_mc_zee}
${forest_pp_2017_data_zmm}
${forest_pp_2017_data_zee}
##
${forest_pbpb_2018_mc_zmm}
${forest_pbpb_2018_mc_zee}
${forest_pbpb_2018_data_zmm}
${forest_pbpb_2018_data_zee}
##
${forest_pp_2015_mc_zmm}
${forest_pp_2015_mc_zee}
${forest_pp_2015_data_zmm}
${forest_pp_2015_data_zee}
##
${forest_pbpb_2015_mc_zmm}
${forest_pbpb_2015_mc_zee}
${forest_pbpb_2015_data_zmm}
${forest_pbpb_2015_data_zee}
##
${forest_pbpb_2018_mc_zmm_noPU}
);

mixFileList=(
${forest_pp_2017_mc_mix}
${forest_pp_2017_mc_mix}
${forest_pp_2017_data_mix}
${forest_pp_2017_data_mix}
##
${forest_pbpb_2018_mc_mix}
${forest_pbpb_2018_mc_mix}
${forest_pbpb_2018_data_mix_1}
${forest_pbpb_2018_data_mix_2}
##
${forest_pp_2015_mc_mix}
${forest_pp_2015_mc_mix}
${forest_pp_2015_data_mix}
${forest_pp_2015_data_mix}
##
${forest_pbpb_2015_mc_mix}
${forest_pbpb_2015_mc_mix}
${forest_pbpb_2015_data_mix_1}
${forest_pbpb_2015_data_mix_2}
##
"DUMMY.root"
);

configList=(
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pp.2017.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pp.2017.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pp.2017.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pp.2017.data.conf"
##
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pbpb.2018.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pbpb.2018.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pbpb.2018.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pbpb.2018.data.conf"
##
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pp.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pp.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pp.2015.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pp.2015.data.conf"
##
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pbpb.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pbpb.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pbpb.2015.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pbpb.2015.data.conf"
##
"Configurations/vJetTrk/zBoson/vJetTrkSkim.pbpb.2018.mc.noPU.conf"
);

#optionBase="--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
optionList=(
"--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zee --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zee --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
##
"--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zee --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zee --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
##
"--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zee --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zee --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
##
"--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zee --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
"--vType=zee --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
##
"--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

outList=(
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pp_2017_mc_zmm.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pp_2017_mc_zee.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pp_2017_data_zmm.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pp_2017_data_zee.root"
##
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pbpb_2018_mc_zmm.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pbpb_2018_mc_zee.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pbpb_2018_data_zmm.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pbpb_2018_data_zee.root"
##
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pp_2015_mc_zmm.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pp_2015_mc_zee.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pp_2015_data_zmm.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pp_2015_data_zee.root"
##
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pbpb_2015_mc_zmm.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pbpb_2015_mc_zee.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pbpb_2015_data_zmm.root"
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pbpb_2015_data_zee.root"
##
$outDirBase"/"${relDirSkim}"/"${dirSpecial}"/vJetTrkSkim_pbpb_2018_mc_zmm_noPU.root"
);

arrayIndices=${!sampleFlags[*]}
for i1 in $arrayIndices
do
  if [ ${sampleFlags[i1]} == 1 ]; then

    configFile=${configList[i1]}
    inputFile=${inputList[i1]}
    mixFile=${mixFileList[i1]}
    options=${optionList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir

    $runCmd $progPath $configFile $inputFile $outputFile $mixFile ${options} &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile $mixFile ${options} &> $outputFileLOG &"

  fi
done

