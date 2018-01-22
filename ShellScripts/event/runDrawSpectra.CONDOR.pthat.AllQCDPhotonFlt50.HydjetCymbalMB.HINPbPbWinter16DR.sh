#!/bin/bash

## how to run this script and submit the condor jobs :
# $> ./ShellScripts/event/runDrawSpectra.CONDOR.pthat.AllQCDPhotonFlt50.HydjetCymbalMB.HINPbPbWinter16DR.sh &> tempLog.txt
# $> grep "condor_submit /" tempLog.txt # execute the lines that are printed

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputFiles=(
"Configurations/filelists/Pythia8_AllQCDPhoton30Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_AllQCDPhoton50Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_AllQCDPhoton80Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_AllQCDPhoton120Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
"Configurations/filelists/Pythia8_AllQCDPhotonXXFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
);

configFiles=(
"Configurations/event/Pythia8_AllQCDPhotonFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/spectra.pthat.conf"
"Configurations/event/Pythia8_AllQCDPhotonFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/spectra.pthat.conf"
"Configurations/event/Pythia8_AllQCDPhotonFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/spectra.pthat.conf"
"Configurations/event/Pythia8_AllQCDPhotonFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/spectra.pthat.conf"
"Configurations/event/Pythia8_AllQCDPhotonFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/spectra.pthat.conf"
);

outDirBase=$EWJTAOUTGRID
if [ -z "$outDirBase" ]; then
  USERGRID=$(voms-proxy-info -issuer | awk '{split($0,myLine,"/CN="); print myLine[2]}')
  if [ -z "$USERGRID" ]; then
    USERGRID=$USER
  fi
  outDirBase="/mnt/hadoop/cms/store/user/"$USERGRID"/EWJTA-out"
fi
outputFiles=(
$outDirBase"/Configurations/event/spectra_pthat_AllQCDPhoton30Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/dummy.root"
$outDirBase"/Configurations/event/spectra_pthat_AllQCDPhoton50Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/dummy.root"
$outDirBase"/Configurations/event/spectra_pthat_AllQCDPhoton80Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/dummy.root"
$outDirBase"/Configurations/event/spectra_pthat_AllQCDPhoton120Flt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/dummy.root"
$outDirBase"/Configurations/event/spectra_pthat_AllQCDPhotonXXFlt50_Hydjet_Cymbal_MB_HINPbPbWinter16DR/dummy.root"
);

arrayIndices=${!outputFiles[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    configFile=${configFiles[i1]}
    outputFile=${outputFiles[i1]}
    #outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    #mkdir -p $outDir
    $runCmd "./ShellScripts/Condor/condorSubmit.sh" $progPath $configFile $inputFile $outDir 1 0
    echo "$runCmd ./ShellScripts/Condor/condorSubmit.sh $progPath $configFile $inputFile $outDir 1 0"
done


