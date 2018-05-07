#!/bin/bash

set -x

progSrcPath="./Plotting/photonJetFF/compact/plotCompact.C"
progPath="${progSrcPath/.C/.exe}"

g++ $progSrcPath $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o $progPath

fileData="./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
filePPMC="./Data/photonJetJS/jsclosure_ppmc_60_30_gxi0_obs2_ffjs_final.root"
fileDataMCRatio="./Data/photonJetJS/jsdata_pp_data_mc_ratio_60_30_gxi0_obs2.root"
fileAll="./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics-PPMC-TMP.root"
hadd -f $fileAll $fileData $filePPMC $fileDataMCRatio

inputFiles=(
$fileData
$fileAll
);

figureIndices=(1 2);
isJS=1

arrayIndices=${!inputFiles[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    figureIndex=${figureIndices[i1]}
    $progPath $inputFile $figureIndex $isJS
done

rm $fileAll
