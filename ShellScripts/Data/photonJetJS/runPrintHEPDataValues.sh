#!/bin/bash

progPath="./Data/printHEPData.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -O2 -o $progPath || exit 1

runCmd="./ShellScripts/myRun.sh"

outputBase="./Data/photonJetJS/hepdata"

hPaths=(
"hjs_final_ppdata_corrjsrecoreco_100_200"
"hjs_final_pbpbdata_corrjsrecoreco_100_200"
"hjs_final_pbpbdata_corrjsrecoreco_60_100"
"hjs_final_pbpbdata_corrjsrecoreco_20_60"
"hjs_final_pbpbdata_corrjsrecoreco_0_20"
"hjs_final_ratio_100_200"
"hjs_final_ratio_60_100"
"hjs_final_ratio_20_60"
"hjs_final_ratio_0_20"
);

xMin=0
xMax=0.3

## rm all output files from previous
rm $outputBase"/data"?".yaml"
rm $outputBase"/data"??".yaml"

arrayIndices=${!hPaths[*]}
for i1 in $arrayIndices
do
    hPath=${hPaths[i1]}
    hSysPath=$hPath"_systematics"

    inputFile="./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"

    iTable=$((i1 + 1))
    outputFile=$outputBase"/data"$iTable".yaml"

    doIndepVar=1

    $progPath $inputFile $outputFile $hPath $hSysPath $xMin $xMax $doIndepVar
    echo "$progPath $inputFile $outputFile $hPath $hSysPath $xMin $xMax $doIndepVar"
done

