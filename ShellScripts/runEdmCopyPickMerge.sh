#!/bin/bash

runCmd="./ShellScripts/myRun.sh"

outputFile="edmCPM_Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_AODSIM.root"
events="1:121658727,1:200648128,1:284997"
inputFiles="/store/himc/HINPbPbWinter16DR/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/AODSIM/75X_mcRun2_HeavyIon_v14-v1/130000/00502EC0-0BFA-E611-A43B-FA163E16E92B.root,/store/himc/HINPbPbWinter16DR/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/AODSIM/75X_mcRun2_HeavyIon_v14-v1/130000/08C36E45-93FA-E611-B388-FA163EB2B3CB.root,/store/himc/HINPbPbWinter16DR/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/AODSIM/75X_mcRun2_HeavyIon_v14-v1/130000/96992181-8BFA-E611-B2DD-FA163E4349F6.root"

logFile="${outputFile/.root/.log}"

$runCmd edmCopyPickMerge outputFile=$outputFile eventsToProcess=$events inputFiles=$inputFiles &> $logFile &
echo "$runCmd edmCopyPickMerge outputFile=$outputFile eventsToProcess=$events inputFiles=$inputFiles &> $logFile &"

