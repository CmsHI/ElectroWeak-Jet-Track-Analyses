#!/bin/bash

set -x

progSrcPath="./Plotting/photonJetFF/compact/plotCompact.C"
progPath="${progSrcPath/.C/.exe}"

g++ $progSrcPath $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o $progPath

inputFiles=(
"./Data/photonJetFF/ffdata_data_60_30_gxi0_obs1_ffjs_final-and-systematics.root"
"./Data/photonJetFF/ffdata_data_60_30_gxi1_obs1_ffjs_final-and-systematics.root"
);

figureIndices=(1 2);
isJS=0

arrayIndices=${!inputFiles[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    figureIndex=${figureIndices[i1]}
    $progPath $inputFile $figureIndex $isJS
done
