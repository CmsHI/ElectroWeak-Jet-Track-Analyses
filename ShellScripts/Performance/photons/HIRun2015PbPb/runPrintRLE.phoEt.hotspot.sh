#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/printRunLumiEvent.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputFiles="/mnt/hadoop/cms/store/user/richard/2015-Data-promptRECO-photonSkims/HIPhoton40AndZ/PbPb-photonHLTFilter-v3/160202_145715/0000/HiForestAOD_*.root"
nFiles=10

inputFile="tmp.list"
ls -1 $inputFiles | head -${nFiles} > $inputFile
sed -i "s,/mnt/hadoop/cms/store,root://xrootd.cmsaf.mit.edu//store,g" $inputFile

configFiles=(
"Configurations/photons/HIRun2015PbPb/printRLE.phoEt.hotspot.conf"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outList=(
$outDirBase"/Configurations/photons/HIRun2015PbPb/printRLE_phoEt_hotspot_HIPhoton40AndZ_HIRun2015.txt"
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


