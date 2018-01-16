#!/bin/bash

set -x

g++ Plotting/photonJetFF/plotXiPhaseSpace.C $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -O2 -o Plotting/photonJetFF/plotXiPhaseSpace.exe || exit 1

cd Plotting/photonJetFF/
./plotXiPhaseSpace.exe
