#!/bin/bash

progPath="./Plotting/trigger/triggerPlotPublic.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

inputList=(
${EWJTAOUT}"/Performance/photons/HIRun2018PbPb/HLT/objTriggerAna_HIGEDPhoton_EB_HIMinimumBias012_04Apr2019_v1_Cert_326381_327564_filterPhoPt15_matchHLTandL1obj.root"
${EWJTAOUT}"/Performance/photons/HIRun2018PbPb/HLT/objTriggerAna_HIGEDPhoton_EB_HIMinimumBias012_04Apr2019_v1_Cert_326381_327564_filterPhoPt15_matchHLTandL1obj.root"
${EWJTAOUT}"/Performance/photons/HIRun2018PbPb/HLT/objTriggerAna_HIGEDPhoton_EB_HIMinimumBias012_04Apr2019_v1_Cert_326381_327564_filterPhoPt15_denomL1_matchHLTobj.root"
${EWJTAOUT}"/Performance/photons/HIRun2018PbPb/HLT/objTriggerAna_HIGEDPhoton_EB_HIMinimumBias012_04Apr2019_v1_Cert_326381_327564_filterPhoPt15_denomL1_matchHLTobj.root"
);

figureIndices=(0 1 2 3)

arrayIndices=${!figureIndices[*]}
for i1 in $arrayIndices
do
    figureIndex=${figureIndices[i1]}
    inputFile=${inputList[i1]}
    $progPath $figureIndex $inputFile
    wait
done

set -x
mkdir -p triggerPlots/
mv triggerPlot_*.pdf triggerPlots/
