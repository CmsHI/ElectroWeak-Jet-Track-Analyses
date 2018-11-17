#!/bin/bash

set -x

g++ Plotting/photonJetFF/plotPhotonJetFF.C $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o Plotting/photonJetFF/plotPhotonJetFF.exe || exit 1

progPath="./Plotting/photonJetFF/plotPhotonJetFF.exe"
inputFiles=(
"./Data/photonJetFF/ffdata_data_60_30_gxi0_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi1_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi0_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi1_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi0_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi1_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi0_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi1_obs1_ffjs_final-and-systematics.root"
);

histLists=(
"Configurations/photonJetFF/ff.list"
"Configurations/photonJetFF/ff.list"
"Configurations/photonJetFF/ff-cent030.list"
"Configurations/photonJetFF/ff-cent030.list"
"Configurations/photonJetFF/ff-cent030-ratio.list"
"Configurations/photonJetFF/ff-cent030-ratio.list"
"Configurations/photonJetFF/ff-cent1030-cent010-ratio.list"
"Configurations/photonJetFF/ff-cent1030-cent010-ratio.list"
);

configList=(
"Configurations/photonJetFF/ff-60-30-gxi0.conf"
"Configurations/photonJetFF/ff-60-30-gxi1.conf"
"Configurations/photonJetFF/ff-60-30-gxi0-cent030.conf"
"Configurations/photonJetFF/ff-60-30-gxi1-cent030.conf"
"Configurations/photonJetFF/ff-60-30-gxi0-cent030-ratio.conf"
"Configurations/photonJetFF/ff-60-30-gxi1-cent030-ratio.conf"
"Configurations/photonJetFF/ff-60-30-gxi0-cent1030-cent010-ratio.conf"
"Configurations/photonJetFF/ff-60-30-gxi1-cent1030-cent010-ratio.conf"
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

