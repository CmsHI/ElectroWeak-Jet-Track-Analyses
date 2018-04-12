#!/bin/bash

progPath="./Performance/readWriteTObject.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -O2 -o $progPath || exit 1

runCmd="./ShellScripts/myRun.sh"

inputFile="./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
outputFile="./Data/photonJetFF/readWriteTObject_xijet.root"

inputObjs=(
"hff_final_pbpbdata_recoreco_100_200"
"hff_final_pbpbdata_recoreco_60_100"
"hff_final_pbpbdata_recoreco_20_60"
"hff_final_pbpbdata_recoreco_0_20"
"hff_final_ppdata_srecoreco_100_200"
"hff_final_ppdata_srecoreco_60_100"
"hff_final_ppdata_srecoreco_20_60"
"hff_final_ppdata_srecoreco_0_20"
"hff_final_ratio_100_200"
"hff_final_ratio_60_100"
"hff_final_ratio_20_60"
"hff_final_ratio_0_20"
"hff_final_pbpbdata_recoreco_100_200_systematics"
"hff_final_pbpbdata_recoreco_60_100_systematics"
"hff_final_pbpbdata_recoreco_20_60_systematics"
"hff_final_pbpbdata_recoreco_0_20_systematics"
"hff_final_ppdata_srecoreco_100_200_systematics"
"hff_final_ppdata_srecoreco_60_100_systematics"
"hff_final_ppdata_srecoreco_20_60_systematics"
"hff_final_ppdata_srecoreco_0_20_systematics"
"hff_final_ratio_100_200_systematics"
"hff_final_ratio_60_100_systematics"
"hff_final_ratio_20_60_systematics"
"hff_final_ratio_0_20_systematics"
);

outputObjs=(
"ff_pbpb_Cent_50_100"
"ff_pbpb_Cent_30_50"
"ff_pbpb_Cent_10_30"
"ff_pbpb_Cent_0_10"
"ff_pp_smear_Cent_50_100"
"ff_pp_smear_Cent_30_50"
"ff_pp_smear_Cent_10_30"
"ff_pp_smear_Cent_0_10"
"ff_pbpb_pp_ratio_Cent_50_100"
"ff_pbpb_pp_ratio_Cent_30_50"
"ff_pbpb_pp_ratio_Cent_10_30"
"ff_pbpb_pp_ratio_Cent_0_10"
"ff_pbpb_Cent_50_100_sys"
"ff_pbpb_Cent_30_50_sys"
"ff_pbpb_Cent_10_30_sys"
"ff_pbpb_Cent_0_10_sys"
"ff_pp_smear_Cent_50_100_sys"
"ff_pp_smear_Cent_30_50_sys"
"ff_pp_smear_Cent_10_30_sys"
"ff_pp_smear_Cent_0_10_sys"
"ff_pbpb_pp_ratio_Cent_50_100_sys"
"ff_pbpb_pp_ratio_Cent_30_50_sys"
"ff_pbpb_pp_ratio_Cent_10_30_sys"
"ff_pbpb_pp_ratio_Cent_0_10_sys"
);

writeMode="UPDATE"

arrayIndices=${!outputObjs[*]}
for i1 in $arrayIndices
do
    inputObj=${inputObjs[i1]}
    outputObj=${outputObjs[i1]}
    $progPath $inputFile $outputFile $inputObj $outputObj $writeMode
    echo "$progPath $inputFile $outputFile $inputObj $outputObj $writeMode"
done

