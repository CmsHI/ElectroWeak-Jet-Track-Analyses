#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputFiles=(
"Configurations/filelists/Pythia8_AllQCDPhoton80Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list"
"Configurations/filelists/Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV.list"
);

configFiles=(
"Configurations/event/spectra2D.ngen.hiBin.conf"
"Configurations/event/spectra2D.ngen.hiBin.conf"
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
$outDirBase"/Configurations/event/spectra2D_ngen_hiBin_AllQCDPhoton80Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR/dummy.root"
$outDirBase"/Configurations/event/spectra2D_ngen_hiBin_Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV/dummy.root"
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


