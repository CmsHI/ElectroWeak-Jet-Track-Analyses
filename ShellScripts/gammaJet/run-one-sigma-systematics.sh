#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Usage: ./ShellScripts/gammaJet/run-one-sigma-systematics.sh [nominal file] [input dir] [output dir]"
    exit 1;
fi

NOMINAL=$1
INPUT=$2
OUTPUT=$3

SYSTYPE=(JER JES)

for ISYS in ${!SYSTYPE[@]}; do
    OUTPUT_FINAL=$OUTPUT/${SYSTYPE[ISYS]}
    mkdir -p $OUTPUT_FINAL

    HIST_LIST=$OUTPUT_FINAL/histogram-names.list
    ./ShellScripts/gammaJet/emit-histogram-names.sh $HIST_LIST

    VAR_FILE_LIST=$OUTPUT_FINAL/variations_${SYSTYPE[ISYS]}.list
    ls $INPUT/PbPb_Data_${SYSTYPE[ISYS]}*/gammaJetHistograms_*.root > $VAR_FILE_LIST

    ./Systematics/gammaJet/one_sigma_systematics.exe $NOMINAL $VAR_FILE_LIST $HIST_LIST $OUTPUT_FINAL/gammaJetHistograms_${SYSTYPE[ISYS]}
done
