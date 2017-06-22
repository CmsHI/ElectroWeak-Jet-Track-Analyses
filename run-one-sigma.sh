#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Usage: ./run-one-sigma.sh [nominal file] [input dir] [output dir]"
    exit 1;
fi

NOMINAL=$1
INPUT=$2
OUTPUT=$3

SYSTYPE=(JER JES_UP JES_DOWN)

for ISYS in 0 1 2; do
    OUTPUT_FINAL=$OUTPUT/${SYSTYPE[ISYS]}
    mkdir -p $OUTPUT_FINAL

    HIST_LIST=$OUTPUT_FINAL/histogram-names.list
    ./emit-histogram-names.sh $HIST_LIST

    VAR_FILE_LIST=$OUTPUT_FINAL/variations_${SYSTYPE[ISYS]}.list
    ls $INPUT/PbPb_Data_${SYSTYPE[ISYS]}/gammaJetHistograms_*.root > $VAR_FILE_LIST

    ./one_sigma_systematics $NOMINAL $VAR_FILE_LIST $HIST_LIST $OUTPUT_FINAL/gammaJetHistograms_${SYSTYPE[ISYS]}
done
