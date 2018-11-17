#!/bin/bash

progPath="./Data/photonJetFF/printHEPData.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -O2 -o $progPath || exit 1

runCmd="./ShellScripts/myRun.sh"

inputFiles=(
"./Data/photonJetFF/ffdata_data_60_30_gxi0_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi0_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi1_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi1_obs1_ffjs_final-and-systematics.root"
);

outputBase="./Data/photonJetFF/hepdata"
outputFiles=(
$outputBase"/data1.yaml"
$outputBase"/data2.yaml"
$outputBase"/data3.yaml"
$outputBase"/data4.yaml"
);

hPaths=(
"hff_final_pbpbdata_recoreco_100_200"
"hff_final_ppdata_srecoreco_100_200"
"hff_final_pbpbdata_recoreco_100_200"
"hff_final_ppdata_srecoreco_100_200"
);

hSysPaths=(
"hff_final_pbpbdata_recoreco_100_200_systematics"
"hff_final_ppdata_srecoreco_100_200_systematics"
"hff_final_pbpbdata_recoreco_100_200_systematics"
"hff_final_ppdata_srecoreco_100_200_systematics"
);

xMin=0.5
xMax=4.5

arrayIndices=${!hPaths[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    outputFile=${outputFiles[i1]}
    hPath=${hPaths[i1]}
    hSysPath=${hSysPaths[i1]}
    $progPath $inputFile $outputFile $hPath $hSysPath $xMin $xMax
    echo "$progPath $inputFile $outputFile $hPath $hSysPath $xMin $xMax"
done

