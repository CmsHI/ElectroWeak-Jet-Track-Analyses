#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Histogramming/vJetTrk/vJetTrkAna.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

### USER INPUT START
trkRG="g" # Options are "r" (reco) or "g" (gen)
trkRBS="bkg" # Options are "raw" (v+jet event), "bkg" (mix event) or "sig" (sube==0 particles from v+jet event)
dirSpecial=""  # name of the directory where special/non-nominal output (if any) would be written
sampleFlags=(
0 # pp_2017_mc_zmm
0 # pp_2017_mc_zee
0 # pp_2017_data_zmm
0 # pp_2017_data_zee
0 # DUMMY
0 # DUMMY
##
1 # pbpb_2018_mc_zmm
0 # pbpb_2018_mc_zee
0 # pbpb_2018_data_zmm
0 # pbpb_2018_data_zee
0 # DUMMY
0 # DUMMY
##
0 # pp_2015_mc_zmm
0 # pp_2015_mc_zee
0 # pp_2015_data_zmm
0 # pp_2015_data_zee
0 # DUMMY
0 # DUMMY
##
0 # pbpb_2015_mc_zmm
0 # pbpb_2015_mc_zee
0 # pbpb_2015_data_zmm
0 # pbpb_2015_data_zee
0 # DUMMY
0 # DUMMY
## 
0 # pbpb_2018_mc_zmm_noPU
);
### USER INPUT END

inputList=(
## pp 2017
${skim_pp_2017_mc_zmm}
${skim_pp_2017_mc_zee}
${skim_pp_2017_data_zmm}
${skim_pp_2017_data_zee}
"DUMMY"
"DUMMY"
## pbpb 2018
${skim_pbpb_2018_mc_zmm}
${skim_pbpb_2018_mc_zee}
${skim_pbpb_2018_data_zmm}
${skim_pbpb_2018_data_zee}
"DUMMY"
"DUMMY"
## pp 2015
${skim_pp_2015_mc_zmm}
${skim_pp_2015_mc_zee}
${skim_pp_2015_data_zmm}
${skim_pp_2015_data_zee}
"DUMMY"
"DUMMY"
## pbpb 2015
${skim_pbpb_2015_mc_zmm}
${skim_pbpb_2015_mc_zee}
${skim_pbpb_2015_data_zmm}
${skim_pbpb_2015_data_zee}
"DUMMY"
"DUMMY"
##
${skim_pbpb_2018_mc_zmm_noPU}
);

#"Configurations/vJetTrk/zBoson/vJetTrkAna.conf"
configList=(
## pp 2017
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2017.data.conf"
"DUMMY"
"DUMMY"
## pbpb 2018
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.data.conf"
"DUMMY"
"DUMMY"
## pp 2015
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pp.2015.data.conf"
"DUMMY"
"DUMMY"
## pbpb 2015
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.mc.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.data.conf"
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2015.data.conf"
"DUMMY"
"DUMMY"
## pbpb 2018 NoPU
"Configurations/vJetTrk/zBoson/vJetTrkAna.pbpb.2018.mc.conf"
);

#optionBase="--vType=zmm --vPtMin=10 --jetPtMin=20 --jetEtaMax=1.6 --trkPtMin=0.7 --trkEtaMax=2.4"
optionList=(
## pp 2017
"--vType=zmm --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:-1"
"--vType=zee --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:-1"
"--vType=zmm --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:-1"
"--vType=zee --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:-1"
"DUMMY"
"DUMMY"
## pbpb 2018
"--vType=zmm --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:30,30:100,0:10,10:30,30:50,50:90"
"--vType=zee --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:30,30:100,0:10,10:30,30:50,50:90"
"--vType=zmm --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:30,30:100,0:10,10:30,30:50,50:90"
"--vType=zee --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:30,30:100,0:10,10:30,30:50,50:90"
"DUMMY"
"DUMMY"
### pp 2015
"--vType=zmm --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:-1"
"--vType=zee --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:-1"
"--vType=zmm --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:-1"
"--vType=zee --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:-1"
"DUMMY"
"DUMMY"
## pbpb 2015
"--vType=zmm --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:30,30:100,0:10,10:30,30:50,50:90"
"--vType=zee --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:30,30:100,0:10,10:30,30:50,50:90"
"--vType=zmm --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:30,30:100,0:10,10:30,30:50,50:90"
"--vType=zee --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:30,30:100,0:10,10:30,30:50,50:90"
"DUMMY"
"DUMMY"
## pbpb 2018 noPU
"--vType=zmm --vPts=10:20,20:40,20:0,30:0,40:0 --vEtaMax=2.4 --trkPts=1:0,1:2,2:3,3:4,4:8 --trkEtaMax=2.4 --vTrkDphiMin=0.875 --cents=0:-1"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

arrayIndices=${!sampleFlags[*]}
for i1 in $arrayIndices
do
  if [ ${sampleFlags[i1]} == 1 ]; then
  
    configFile=${configList[i1]}
    inputFile=${inputList[i1]}
    mixFile=${mixFileList[i1]}
    options=${optionList[i1]}

    ## append specific options
    options=${options}" --trkRG"=${trkRG}
    options=${options}" --trkRBS"=${trkRBS}

    outputSuffix=${anaSuffixList[i1]}
    outputFile=${outDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${outputSuffix}".root"
    ## modify output name based on options
    outputFile="${outputFile/.root/_trk_${trkRG}_${trkRBS}.root}"

    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir

    $runCmd $progPath $configFile $inputFile $outputFile ${options} &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile ${options} &> $outputFileLOG &"
  fi
done

