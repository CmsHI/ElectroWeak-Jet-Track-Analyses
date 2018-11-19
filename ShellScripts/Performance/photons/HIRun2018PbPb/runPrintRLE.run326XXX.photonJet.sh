#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/printRunLumiEvent.exe"

inputList=(
"Configurations/filelists/HIRun2018PbPb/ExpressForests_326XXX.list"
"Configurations/filelists/HIRun2018PbPb/HIHardProbes_skim_HLT_Photon40_StreamerForests_326XXX.list"
);

configFiles=(
"Configurations/photons/HIRun2018PbPb/printRLE.run326XXX.photonJet.conf"
"Configurations/photons/HIRun2018PbPb/printRLE.run326XXX.photonJet.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffixExpress="ExpressForests"
outputSuffixSkim="HIHardProbes_skim_HLT_Photon40_StreamerForests"
outList=(
$outDirBase"/Configurations/Performance/photons/HIRun2018PbPb/printRLE_photonJet_run326XXX_"$outputSuffixExpress".txt"
$outDirBase"/Configurations/Performance/photons/HIRun2018PbPb/printRLE_photonJet_run326XXX_"$outputSuffixSkim".txt"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    inputFile=${inputList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.txt/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


