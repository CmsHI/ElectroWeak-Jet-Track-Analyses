#!/bin/bash

mkdir plots

for CONF in CutConfigurations/gammaJetPlotConfigs/*.conf
do
    BASECONF=$(basename $CONF)
    ./Plotting/plotHistogram.exe $CONF $CONF "plots/$BASECONF.pdf"
done
