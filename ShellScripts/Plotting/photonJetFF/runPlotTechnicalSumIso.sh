#!/bin/bash

set -x

g++ ./Plotting/photonJetFF/plotTechnicalSumIso.C $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o ./Plotting/photonJetFF/plotTechnicalSumIso.exe || exit 1

inputFile="./Data/photonJetFF/photonSumIso_Pythia8_Hydjet_MB.root"
./Plotting/photonJetFF/plotTechnicalSumIso.exe $inputFile
