#!/bin/bash

set -x
g++ Plotting/photonJetJS/plotPhotonJetJSdataMC.C -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -o Plotting/photonJetJS/plotPhotonJetJSdataMC.exe || exit 1

progPath="./Plotting/photonJetJS/plotPhotonJetJSdataMC.exe"
inputFiles=(
"./Data/photonJetJS/jsdata_ppdata_mc_60_30_gxi0_obs2_ffjs_final-and-systematics.root"
);

figureIndices=(0)

arrayIndices=${!figureIndices[*]}
for i1 in $arrayIndices
do
    figureIndex=${figureIndices[i1]}
    inputFile=${inputFiles[i1]}
    $progPath $figureIndex $inputFile
    wait
done


