#!/bin/bash

set -x
# g++ Plotting/zJet/zJetPlotPublic.C -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -o Plotting/zJet/zJetPlotPublic.exe || exit 1

progPath="./Plotting/zJet/zJetPlotPublic.exe"
inputFile="./Data/zBosons/zJet_ResultData.root"

figureIndices=(0 1 2 3 4 5 6 7 8 9 10 11 12 13 14)

arrayIndices=${!figureIndices[*]}
for i1 in $arrayIndices
do
    figureIndex=${figureIndices[i1]}
    $progPath $figureIndex $inputFile
    wait
done


