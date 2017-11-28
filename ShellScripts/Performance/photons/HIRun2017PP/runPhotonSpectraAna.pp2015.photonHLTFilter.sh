#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/photons/photonSpectraAna.exe"
inputFile="/mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root"

outDirBase="/export/d00/scratch/"$USER"/EWJTA-out/"
outputSuffix="2015_promptRECO_pp_photonHLTFilter"
outList=(
$outDirBase"Performance/photons/HIRun2017PP/photonSpectraAna_"$outputSuffix".root"
);

configList=(
"Configurations/Performance/photons/HIRun2017PP/photonSpectraAna.pp2015.photonHLTFilter.conf"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done

