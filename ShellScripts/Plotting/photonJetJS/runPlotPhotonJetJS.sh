#!/bin/bash

set -x

g++ Plotting/photonJetFF/plotPhotonJetFF.C $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o Plotting/photonJetFF/plotPhotonJetFF.exe || exit 1

fileData="./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
filePPMC="./Data/photonJetJS/jsclosure_ppmc_60_30_gxi0_obs2_ffjs_final.root"
fileDataMCRatio="./Data/photonJetJS/jsdata_pp_data_mc_ratio_60_30_gxi0_obs2.root"
fileAll="./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics-PPMC-TMP.root"
hadd -f $fileAll $fileData $filePPMC $fileDataMCRatio

progPath="./Plotting/photonJetFF/plotPhotonJetFF.exe"
inputFiles=(
"./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
$fileAll
"./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
"./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
#"./Data/photonJetJS/jsdata_data_normJet_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
#"./Data/photonJetJS/jsdata_data_normJet_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
);

histLists=(
"Configurations/photonJetJS/js.list"
"Configurations/photonJetJS/js-ppmc.list"
"Configurations/photonJetJS/js-cent030.list"
"Configurations/photonJetJS/js-cent010-ratio.list"
"Configurations/photonJetJS/js-cent030-ratio.list"
"Configurations/photonJetJS/js-cent1030-cent010-ratio.list"
#"Configurations/photonJetJS/js-normJet.list"
#"Configurations/photonJetJS/js-normJet-cent030.list"
);

configList=(
"Configurations/photonJetJS/js-60-30-gxi0.conf"
"Configurations/photonJetJS/js-60-30-gxi0-ppmc.conf"
"Configurations/photonJetJS/js-60-30-gxi0-cent030.conf"
"Configurations/photonJetJS/js-60-30-gxi0-cent010-ratio.conf"
"Configurations/photonJetJS/js-60-30-gxi0-cent030-ratio.conf"
"Configurations/photonJetJS/js-60-30-gxi0-cent1030-cent010-ratio.conf"
#"Configurations/photonJetJS/js-normJet-60-30-gxi0.conf"
#"Configurations/photonJetJS/js-normJet-60-30-gxi0-cent030.conf"
);

arrayIndices=${!configList[*]}
for i1 in $arrayIndices
do
    configFile=${configList[i1]}
    outputFileLOG="${configFile/.conf/.log}"
    inputFile=${inputFiles[i1]}
    histList=${histLists[i1]}
    $progPath $inputFile $histList $configFile &> $outputFileLOG
done

rm $fileAll
