#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo "Usage: ./run-one-sigma.sh [nominal file] [input dir] [output dir]"
    exit 1;
fi

SYSTYPE=(JER JES_UP JES_DOWN)

for ISYS in 0 1 2; do
    OUTPUTDIR=$3/${SYSTYPE[ISYS]}

    HIST_LIST=$OUTPUTDIR/histogram-names.list
    ./emit-histogram-names.sh $HIST_LIST

    VAR_FILE_LIST=variations_${SYSTYPE[ISYS]}.list
    ls $2/PbPb_Data_${SYSTYPE[ISYS]}/gammaJetHistograms_*.root > $VAR_FILE_LIST

    ./one_sigma_systematics $1 $VAR_FILE_LIST $HIST_LIST ${SYSTYPE[ISYS]}
done
