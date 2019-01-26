#!/bin/bash

progPath="./Data/checkHEPData.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -O2 -o $progPath || exit 1

runCmd="./ShellScripts/myRun.sh"

histPaths1=(
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

histPaths2=(
"Table 1/Hist1D_y1"
"Table 1/Hist1D_y2"
"Table 2/Hist1D_y1"
"Table 2/Hist1D_y2"
"Table 3/Hist1D_y1"
"Table 3/Hist1D_y2"
"Table 4/Hist1D_y1"
"Table 4/Hist1D_y2"
"Table 5/Hist1D_y1"
"Table 6/Hist1D_y1"
"Table 7/Hist1D_y1"
"Table 8/Hist1D_y1"
"Table 9/Hist1D_y1"
"Table 9/Hist1D_y2"
"Table 10/Hist1D_y1"
"Table 10/Hist1D_y2"
"Table 11/Hist1D_y1"
"Table 11/Hist1D_y2"
"Table 12/Hist1D_y1"
"Table 12/Hist1D_y2"
"Table 13/Hist1D_y1"
"Table 14/Hist1D_y1"
"Table 15/Hist1D_y1"
"Table 16/Hist1D_y1"
);

xMin=0.5
xMax=4.5

arrayIndices=${!histPaths1[*]}
for i1 in $arrayIndices
do
    inputFile1="./Data/photonJetFF/ffdata_data_60_30_gxi0_obs1_ffjs_final-and-systematics.root"
    xi=0
    if [ $i1 -ge 12 ]; then
      inputFile1="./Data/photonJetFF/ffdata_data_60_30_gxi1_obs1_ffjs_final-and-systematics.root"
      xi=1
    fi

    inputFile2="./Data/photonJetFF/hepdata/HEPData-ins1648162-v1-root.root"

    histPath1=${histPaths1[i1]}
    histPath2=${histPaths2[i1]}
    logFile="./Data/photonJetFF/checkHEPData_"$histPath1"_xi"$xi".log"
    $progPath $inputFile1 $histPath1 $inputFile2 "$histPath2" $xMin $xMax &> $logFile
    echo "$progPath $inputFile1 $histPath1 $inputFile2 "$histPath2" $xMin $xMax &> $logFile"
done

