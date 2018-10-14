#!/bin/bash

set -x
progPath="./Plotting/vJet/vJetPlotProjection.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -o $progPath || exit 1


progPath="./Plotting/vJet/vJetPlotProjection.exe"
inputFiles=(
"./Data/zBosons/zJet_Data_pbpb_cent_030_0100.root"
"./Data/zBosons/zJet_Data_pbpb_cent_030_0100.root"
"./Data/zBosons/zJet_Data_pbpb_cent_030_0100.root"
"./Data/zBosons/zJet_ResultData.root"
"./Data/zBosons/zJet_projection_rebin.root"
"./Data/zBosons/zJet_projection_rebin.root"
"./Data/photons/gammaJet_projection.root"
"./Data/photonJetFF/PAS/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/PAS/data_60_30_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/PAS/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/PAS/data_60_30_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/PAS/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/PAS/data_60_30_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/PAS/data_60_30_gxi0_defnFF1-final-and-systematics.root"
"./Data/photonJetFF/PAS/data_60_30_gxi1_defnFF1-final-and-systematics.root"
"./Data/photonJetJS/jsdata_data_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
);

figureIndices=(0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15)

arrayIndices=${!figureIndices[*]}
for i1 in $arrayIndices
do
    figureIndex=${figureIndices[i1]}
    inputFile=${inputFiles[i1]}
    $progPath $figureIndex $inputFile 0
    $progPath $figureIndex $inputFile -1
    $progPath $figureIndex $inputFile 0.5
    wait
done


