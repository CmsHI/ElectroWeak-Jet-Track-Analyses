#!/bin/bash

set -x
g++ Plotting/vJet/vJetPlotProjection.C -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -o Plotting/vJet/vJetPlotProjection.exe || exit 1

progPath="./Plotting/vJet/vJetPlotProjection.exe"
inputFiles=(
"./Data/zBosons/zJet_Data_pbpb_cent_030_0100.root"
"./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/data_60_30_gxi1_defnFF1-final-and-systematics.root"
);

figureIndices=(0 1 2 3 4 5 6)

arrayIndices=${!figureIndices[*]}
for i1 in $arrayIndices
do
    figureIndex=${figureIndices[i1]}
    inputFile=${inputFiles[i1]}
    $progPath $figureIndex $inputFile 0
    $progPath $figureIndex $inputFile -1
    $progPath $figureIndex $inputFile 0.3
    $progPath $figureIndex $inputFile 0.5
    $progPath $figureIndex $inputFile 0.7
    wait
done

