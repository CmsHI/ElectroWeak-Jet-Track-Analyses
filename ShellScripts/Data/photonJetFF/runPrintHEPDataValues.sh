#!/bin/bash

progPath="./Data/photonJetFF/printHEPData.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -O2 -o $progPath || exit 1

runCmd="./ShellScripts/myRun.sh"

outputBase="./Data/photonJetFF/hepdata"

hPaths=(
"hff_final_ppdata_srecoreco_100_200"
"hff_final_pbpbdata_recoreco_100_200"
"hff_final_ppdata_srecoreco_60_100"
"hff_final_pbpbdata_recoreco_60_100"
"hff_final_ppdata_srecoreco_20_60"
"hff_final_pbpbdata_recoreco_20_60"
"hff_final_ppdata_srecoreco_0_20"
"hff_final_pbpbdata_recoreco_0_20"
"hff_final_ratio_100_200"
"hff_final_ratio_60_100"
"hff_final_ratio_20_60"
"hff_final_ratio_0_20"
"hff_final_ppdata_srecoreco_100_200"
"hff_final_pbpbdata_recoreco_100_200"
"hff_final_ppdata_srecoreco_60_100"
"hff_final_pbpbdata_recoreco_60_100"
"hff_final_ppdata_srecoreco_20_60"
"hff_final_pbpbdata_recoreco_20_60"
"hff_final_ppdata_srecoreco_0_20"
"hff_final_pbpbdata_recoreco_0_20"
"hff_final_ratio_100_200"
"hff_final_ratio_60_100"
"hff_final_ratio_20_60"
"hff_final_ratio_0_20"
);

xMin=0.5
xMax=4.5

## rm all output files from previous
rm $outputBase"/data"?".yaml"
rm $outputBase"/data"??".yaml"

arrayIndices=${!hPaths[*]}
for i1 in $arrayIndices
do
    hPath=${hPaths[i1]}
    hSysPath=$hPath"_systematics"

    inputFile="./Data/photonJetFF/ffdata_data_60_30_gxi0_obs1_ffjs_final-and-systematics.root"
    if [ $i1 -ge 12 ]; then
      inputFile="./Data/photonJetFF/ffdata_data_60_30_gxi1_obs1_ffjs_final-and-systematics.root"
    fi

    iTable=$((i1 % 12))
    if [ $iTable -lt 8 ]; then
      iTable=$((iTable / 2 + 1))
    else
      iTable=$((iTable - 4 + 1))
    fi
    if [ $i1 -ge 12 ]; then
      iTable=$((iTable + 8))
    fi
    outputFile=$outputBase"/data"$iTable".yaml"

    doIndepVar=1
    if [[ $hPath = *pbpb* ]]; then
      doIndepVar=0
    fi

    $progPath $inputFile $outputFile $hPath $hSysPath $xMin $xMax $doIndepVar
    echo "$progPath $inputFile $outputFile $hPath $hSysPath $xMin $xMax $doIndepVar"
done

