#!/bin/bash

runCmd="./ShellScripts/myRun.sh"

nEvents="50"
dataset="/Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB/HINPbPbWinter16DR-75X_mcRun2_HeavyIon_v14-v1/AODSIM"
runLumiEvent=(
# run:lumi:event
"1:7:284997"
"1:4619:200648128"
"1:2801:121658727"
);
logFile="edmPE_Pythia8_AllQCDPhoton30_Hydjet_Cymbal_MB_AODSIM.log"

timeNow=$(date +"%Y%m%d_%H%M%S")
eventsFileTMP="events_edmPE_"$timeNow".txt"

rm -f $eventsFileTMP
arrayIndices=${!runLumiEvent[*]}
for i1 in $arrayIndices
do
    line=${runLumiEvent[i1]}
    echo $line >> $eventsFileTMP
done

echo "$runCmd edmPickEvents.py --maxEventsInteractive=$nEvents $dataset $eventsFileTMP &> $logFile"
$runCmd edmPickEvents.py --maxEventsInteractive=$nEvents $dataset $eventsFileTMP &> $logFile
#sleep 5s # wait for input file to be read
rm -f $eventsFileTMP

echo "options for edmCopyPickMerge"
optEventsToProcess=$(grep "eventsToProcess=" $logFile)
optEventsToProcess="${optEventsToProcess/ \\/}" # replace " \" with ""
optInputFile=$(grep "inputFiles=" $logFile)
echo $optEventsToProcess
echo $optInputFile

