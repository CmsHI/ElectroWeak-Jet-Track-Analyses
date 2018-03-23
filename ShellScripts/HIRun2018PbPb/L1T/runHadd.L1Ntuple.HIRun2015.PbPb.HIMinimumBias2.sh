#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
runScript="./ShellScripts/runHadd.sh"

set -x
outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2/180318_055016/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2/180318_055016/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassEGVetos1/180318_055100/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassEGVetos1/180318_055100/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassEGVetos1.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassExtHOverE1/180318_055457/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassExtHOverE1/180318_055457/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassExtHOverE1.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB0_EE0/180318_055147/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB0_EE0/180318_055147/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB0_EE0.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB1_EE1/180318_055230/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB1_EE1/180318_055230/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB1_EE1.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE1/180318_055311/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE1/180318_055311/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE1.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE2/180318_055357/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE2/180318_055357/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egHOverEcut_EB2_EE2.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1/180320_164859/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1/180320_164859/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassShapeBit1/180320_145130/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassShapeBit1/180320_145130/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassShapeBit1.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1/180320_145107/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1/180320_145107/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB1_EE1/180320_145157/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB1_EE1/180320_145157/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB1_EE1.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

outputFile="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB2_EE2/180320_145221/L1Ntuple_merged.root"
inputFiles="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB2_EE2/180320_145221/0000/L1Ntuple_*.root"
logFile=$EWJTAOUT/ShellScripts/runHadd_L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1ShapeBit1_egHOverEcut_EB2_EE2.log
$runCmd $runScript $outputFile $inputFiles &> $logFile

