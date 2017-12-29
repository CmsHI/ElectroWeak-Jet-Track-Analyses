#!/bin/bash

set -x

progSrcPath="./Plotting/photonJetFF/compact/plotCompact.C"
progPath="${progSrcPath/.C/.exe}"

g++ $progSrcPath $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o $progPath

inputFiles=(
"./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi1_defnFF1-final-and-systematics.root"
);

figureIndices=(1 2);

arrayIndices=${!inputFiles[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    figureIndex=${figureIndices[i1]}
    $progPath $inputFile $figureIndex
done