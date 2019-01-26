#!/bin/bash

progPath="./Data/photonJetFF/checkHEPData.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -O2 -o $progPath || exit 1

runCmd="./ShellScripts/myRun.sh"

histPaths1=(
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

histPaths2=(
"Table 1/Hist1D_y1"
"Table 2/Hist1D_y1"
"Table 3/Hist1D_y1"
"Table 4/Hist1D_y1"
"Table 5/Hist1D_y1"
"Table 6/Hist1D_y1"
"Table 7/Hist1D_y1"
"Table 8/Hist1D_y1"
"Table 9/Hist1D_y1"
);

xMin=0
xMax=0.3

arrayIndices=${!histPaths1[*]}
for i1 in $arrayIndices
do
    inputFile1="./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
    inputFile2="./Data/photonJetJS/hepdata/HEPData-1548463189-v1-root.root"

    histPath1=${histPaths1[i1]}
    histPath2=${histPaths2[i1]}
    logFile="./Data/photonJetJS/checkHEPData_"$histPath1".log"
    $progPath $inputFile1 $histPath1 $inputFile2 "$histPath2" $xMin $xMax &> $logFile
    echo "$progPath $inputFile1 $histPath1 $inputFile2 "$histPath2" $xMin $xMax &> $logFile"
done

