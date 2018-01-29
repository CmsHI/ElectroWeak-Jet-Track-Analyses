#!/bin/bash

set -x
#g++ Data/photonJetFF/saveModelDataTables_photonJetFF.C -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -o Data/photonJetFF/saveModelDataTables_photonJetFF.exe || exit 1
./Data/photonJetFF/saveModelDataTables_photonJetFF.exe ./Data/photonJetFF/model_photonJetFF_5TeV.root

