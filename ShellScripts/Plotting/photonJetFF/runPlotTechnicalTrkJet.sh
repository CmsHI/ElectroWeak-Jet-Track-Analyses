#!/bin/bash

set -x

g++ ./Plotting/photonJetFF/plotTechnicalTrkJet.C $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o ./Plotting/photonJetFF/plotTechnicalTrkJet.exe || exit 1

inputFile="./Data/photonJetFF/data_60_30_gxi0_defnFF1-raw-and-final.root"
./Plotting/photonJetFF/plotTechnicalTrkJet.exe $inputFile
