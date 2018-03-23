#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
runScript="./ShellScripts/runHadd.sh"

set -x

outputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2/180318_055016/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassEGVetos1/180318_055100/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassExtHOverE1/180318_055457/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB0_EE0/180318_055147/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB1_EE1/180318_055230/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE1/180318_055311/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE2/180318_055357/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1/180320_164859/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassShapeBit1/180320_145130/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1/180320_145107/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB1_EE1/180320_145157/L1Ntuple_merged.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB2_EE2/180320_145221/L1Ntuple_merged.root"
);

inputList=(
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2/180318_055016/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassEGVetos1/180318_055100/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassExtHOverE1/180318_055457/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB0_EE0/180318_055147/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB1_EE1/180318_055230/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE1/180318_055311/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE2/180318_055357/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1/180320_164859/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassShapeBit1/180320_145130/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1/180320_145107/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB1_EE1/180320_145157/0000/L1Ntuple_*.root"
"/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB2_EE2/180320_145221/0000/L1Ntuple_*.root"
);

logFileList=(
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassEGVetos1.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassExtHOverE1.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB0_EE0.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB1_EE1.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE1.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE2.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassShapeBit1.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB1_EE1.log"
$EWJTAOUT"/ShellScripts/HIRun2018PbPb/L1T/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB2_EE2.log"
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

