#!/bin/bash

# print run lumi event for events with fake photons, see config files below for fake criteria

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/printRunLumiEvent.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputFileTmp="/mnt/hadoop/cms/store/user/cmcginn/Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8/ikucher-crab_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8_5020GeV_RECO_103X_upgrade2018_realistic_v4_20181015/181015_074519/merged/HiForestAOD_ikucher-crab_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8_5020GeV_RECO_103X_upgrade2018_realistic_v4_20181015_181015_074519_121_OutOf121_MERGED.root"

inputFile=$inputFileTmp

## use xrootd
#inputFile=$(echo ${inputFileTmp} | sed "s,/mnt/hadoop/cms/store,root://xrootd.cmsaf.mit.edu//store,g")
inputFile=$(ShellScripts/replaceMntWithXrootd.sh ${inputFileTmp})

configFiles=(
"Configurations/photons/printRLE.photon.fake.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Configurations/photons/printRLE_photon_fake_Pythia8_AllQCDPhoton30_Hydjet_Quenched_Cymbal5Ev8_5020GeV_RECO_103X_upgrade2018_realistic_v4.txt"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.txt/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


