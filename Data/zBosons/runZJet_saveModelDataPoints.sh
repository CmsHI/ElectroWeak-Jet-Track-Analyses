#!/bin/bash

set -x
#g++ Data/zBosons/zJet_saveModelDataPoints.C -Wall -Werror -Wextra -O2 `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -o Data/zBosons/zJet_saveModelDataPoints.exe || exit 1
./Data/zBosons/zJet_saveModelDataPoints.exe ./Data/zBosons/model_zJet_5TeV.root

