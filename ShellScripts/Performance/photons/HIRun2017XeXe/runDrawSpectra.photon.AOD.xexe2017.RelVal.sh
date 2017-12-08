#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputFile="Configurations/filelists/HIRun2017XeXe/HIMinimumBias1_dataRun2_Rereco17_ONLYXeXeBS_v1_RelVal_xexe2017_AOD.list"

configFiles=(
"Configurations/photons/HIRun2017XeXe/spectra.photon.AOD.conf"
"Configurations/photons/HIRun2017XeXe/spectra.photon.AOD.nonGED.conf"
);

#outDirBase="/export/d00/scratch/"$USER"/EWJTA-out/"
outDirBase="Configurations/photons/HIRun2017XeXe/"
outputSuffix="HIMinimumBias1_dataRun2_Rereco17_ONLYXeXeBS_v1_RelVal_xexe2017"
outputFiles=(
$outDirBase"spectra_photon_AOD_"$outputSuffix".root"
$outDirBase"spectra_photon_AOD_nonGED_"$outputSuffix".root"
);

arrayIndices=${!outputFiles[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    outputFile=${outputFiles[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


