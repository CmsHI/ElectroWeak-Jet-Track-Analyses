#!/bin/bash

progPath="./Performance/readWriteTObject.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -O2 -o $progPath || exit 1

runCmd="./ShellScripts/myRun.sh"

inputFiles=(
"./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsclosure_ppmc_60_30_gxi0_obs2_ffjs_final.root"
"./Data/photonJetJS/jsdata_pp_data_mc_ratio_60_30_gxi0_obs2.root"
"./Data/photonJetJS/jsdata_pp_data_mc_ratio_60_30_gxi0_obs2.root"
"./Data/photonJetJS/jsdata_pp_data_mc_ratio_60_30_gxi0_obs2.root"
"./Data/photonJetJS/jsdata_pp_data_mc_ratio_60_30_gxi0_obs2.root"
);
outputFiles=(
"./Data/photonJetJS/jsdata_ppdata_mc_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_ppdata_mc_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_ppdata_mc_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_ppdata_mc_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_ppdata_mc_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_ppdata_mc_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_ppdata_mc_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
);

inputObjs=(
"hjs_final_ppdata_corrjsrecoreco_100_200"
"hjs_final_ppdata_corrjsrecoreco_100_200_systematics"
"hjs_final_ppmc_ref0gen0_100_200"
"hjs_final_ratio_ppmc_ppdata_100_200"
"hjs_final_ratio_ppmc_ppdata_100_200_systematics"
"hjs_final_ratio_ppdata_ppmc_100_200"
"hjs_final_ratio_ppdata_ppmc_100_200_systematics"
);
outputObjs=(
"hjs_final_ppdata_corrjsrecoreco_100_200"
"hjs_final_ppdata_corrjsrecoreco_100_200_systematics"
"hjs_final_ppmc_ref0gen0_100_200"
"hjs_final_ratio_ppmc_ppdata_100_200"
"hjs_final_ratio_ppmc_ppdata_100_200_systematics"
"hjs_final_ratio_ppdata_ppmc_100_200"
"hjs_final_ratio_ppdata_ppmc_100_200_systematics"
);

writeMode="UPDATE"

arrayIndices=${!outputObjs[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    outputFile=${outputFiles[i1]}
    inputObj=${inputObjs[i1]}
    outputObj=${outputObjs[i1]}
    $progPath $inputFile $outputFile $inputObj $outputObj $writeMode
    echo "$progPath $inputFile $outputFile $inputObj $outputObj $writeMode"
done

