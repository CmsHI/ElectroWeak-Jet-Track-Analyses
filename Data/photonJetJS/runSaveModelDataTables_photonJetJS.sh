#!/bin/bash

set -x
#g++ Data/photonJetJS/saveModelDataTables_photonJetJS.C -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -o Data/photonJetJS/saveModelDataTables_photonJetJS.exe || exit 1
./Data/photonJetJS/saveModelDataTables_photonJetJS.exe ./Data/photonJetJS/model_photonJetJS_5TeV.root

