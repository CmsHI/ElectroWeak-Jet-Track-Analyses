#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
#inputFile="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"
inputFile="/mnt/hadoop/cms/store/user/tatar/official/Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"
#inputFile="/mnt/hadoop/cms/store/user/tatar/EGamma/SingleGammaFlatPt10To100_pythia8_Hydjet/0.root"

configFiles=(
"Configurations/photons/spectra2D.photon.SCnBCnHits.eScale.conf"
"Configurations/photons/spectra2D.photon.SCnBCnHits.eScale.GED.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
#outputSuffix="Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB"
outputSuffix="Pythia8_AllQCDPhoton30Flt30_Hydjet_Cymbal_MB"
#outputSuffix="SingleGammaFlatPt10To100_pythia8_Hydjet"
outList=(
$outDirBase"/Configurations/photons/spectra2D_photon_SCnBCnHits_eScale_"$outputSuffix".root"
$outDirBase"/Configurations/photons/spectra2D_photon_SCnBCnHits_eScale_GED_"$outputSuffix".root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


