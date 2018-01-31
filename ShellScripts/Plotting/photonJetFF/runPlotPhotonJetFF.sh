#!/bin/bash

set -x

g++ Plotting/photonJetFF/plotPhotonJetFF.C $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o Plotting/photonJetFF/plotPhotonJetFF.exe || exit 1

progPath="./Plotting/photonJetFF/plotPhotonJetFF.exe"
inputFiles=(
"./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_80_40_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_80_40_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi1_defnFF1-final-and-systematics.root"
);

histLists=(
"Configurations/photonJetFF/ff.list"
"Configurations/photonJetFF/ff.list"
"Configurations/photonJetFF/ff.list"
"Configurations/photonJetFF/ff.list"
"Configurations/photonJetFF/ff-cent030.list"
"Configurations/photonJetFF/ff-cent030.list"
"Configurations/photonJetFF/ff-reweight.list"
"Configurations/photonJetFF/ff-reweight.list"
"Configurations/photonJetFF/ff-cent030-reweight.list"
"Configurations/photonJetFF/ff-cent030-reweight.list"
);

configList=(
"Configurations/photonJetFF/ff-60-30-gxi0.conf"
"Configurations/photonJetFF/ff-60-30-gxi1.conf"
"Configurations/photonJetFF/ff-80-40-gxi0.conf"
"Configurations/photonJetFF/ff-80-40-gxi1.conf"
"Configurations/photonJetFF/ff-60-30-gxi0-cent030.conf"
"Configurations/photonJetFF/ff-60-30-gxi1-cent030.conf"
"Configurations/photonJetFF/ff-60-30-gxi0-reweight.conf"
"Configurations/photonJetFF/ff-60-30-gxi1-reweight.conf"
"Configurations/photonJetFF/ff-60-30-gxi0-cent030-reweight.conf"
"Configurations/photonJetFF/ff-60-30-gxi1-cent030-reweight.conf"
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

