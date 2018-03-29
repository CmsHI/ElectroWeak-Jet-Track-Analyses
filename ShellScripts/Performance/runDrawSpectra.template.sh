#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputFiles=(
"/mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"
"/mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"
);

configFiles=(
"Configurations/photons/spectra.photon.conf"
"Configurations/photons/spectra.phoEt.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB"
outputFiles=(
$outDirBase"/Configurations/photons/spectra_photon_"$outputSuffix".root"
$outDirBase"/Configurations/photons/spectra_phoEt_"$outputSuffix".root"
);

arrayIndices=${!outputFiles[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    inputFile=${inputFiles[i1]}
    outputFile=${outputFiles[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


