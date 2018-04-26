#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
runScript="./ShellScripts/runHadd.sh"

set -x

outputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_hiFlag0/180422_071258/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_hiFlag1/180422_071317/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_None/180422_071336/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_ChunkyDonut/180422_071239/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingHIRegion/180422_071355/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingHITower/180422_071413/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPP/180422_071435/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPExclude/180422_071454/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPTower/180422_071514/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPTowerMask/180422_071534/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPTowerMedian/180422_071553/L1Ntuple_merged.root"
);

inputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_hiFlag0/180422_071258/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_hiFlag1/180422_071317/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_None/180422_071336/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_ChunkyDonut/180422_071239/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingHIRegion/180422_071355/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingHITower/180422_071413/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPP/180422_071435/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPExclude/180422_071454/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPTower/180422_071514/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPTowerMask/180422_071534/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPTowerMedian/180422_071553/0000/L1Ntuple_*.root"
);

logFileList=(
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_hiFlag0.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_hiFlag1.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_None.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_ChunkyDonut.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingHIRegion.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingHITower.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPP.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPExclude.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPTower.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPTowerMask.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_XeXeRun2017_HIMinimumBias7_PhiRingPPTowerMedian.log"
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

