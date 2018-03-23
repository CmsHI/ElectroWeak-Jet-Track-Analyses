#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
runScript="./ShellScripts/runHadd.sh"

set -x

outputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias1/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias2/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias3/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias4/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias5/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias6/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias7/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias8/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias9/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias10/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias11/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias12/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias13/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias14/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias15/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias16/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias17/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias18/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias19/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias20/XeXeRun2017-13Dec2017-v1-photonFOREST/HiForestAOD_photon_merged.root"
);

inputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181900/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias1/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_153946/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias2/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181535/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias3/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181554/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias4/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181618/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias5/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181642/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias6/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181707/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias7/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181731/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias8/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181754/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias9/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181822/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias10/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_181842/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias11/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_220837/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias12/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_220855/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias13/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_220914/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias14/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_220936/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias15/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_220956/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias16/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_221014/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias17/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_221032/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias18/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_221050/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias19/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_221107/0000/HiForestAOD_photon_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2017XeXe/HIMinimumBias20/XeXeRun2017-13Dec2017-v1-photonFOREST/180321_221126/0000/HiForestAOD_photon_*.root"
);

logFileList=(
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias1_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias2_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias3_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias4_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias5_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias6_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias7_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias8_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias9_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias10_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias11_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias12_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias13_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias14_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias15_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias16_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias17_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias18_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias19_photonFOREST.log"
$EWJTAOUT"/ShellScripts/HIRun2017XeXe/runHadd_XeXeRun2017_13Dec2017_HIMinimumBias20_photonFOREST.log"
);

arrayIndices=${!outputList[*]}
for i1 in $arrayIndices
do
    inputFiles=${inputList[i1]}
    outputFile=${outputList[i1]}
    logFile=${logFileList[i1]}
    outDir=$(dirname "${logFile}")
    mkdir -p $outDir
    $runCmd $runScript $outputFile $inputFiles &> $logFile
    echo "$runCmd $runScript $outputFile $inputFiles &> $logFile"
done

