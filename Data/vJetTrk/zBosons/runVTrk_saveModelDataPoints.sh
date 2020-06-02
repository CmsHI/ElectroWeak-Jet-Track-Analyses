#!/bin/bash

progPath="./Data/vJetTrk/zBosons/vTrk_saveModelDataPoints.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

${progPath} ./Data/vJetTrk/zBosons/models_vJetTrk_5TeV.root

