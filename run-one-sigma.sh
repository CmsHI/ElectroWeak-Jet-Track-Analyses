#!/bin/bash

HIST_LIST=histograms.list
./emit-histogram-names.sh $HIST_LIST

SYSTYPE=(JER JES_UP JES_DOWN)

for ISYS in 0 1 2; do
    VAR_FILE_LIST=variations_${SYSTYPE[ISYS]}.list
    ls /export/d00/scratch/biran/gamma-jet/2017-06-20-incremental/PbPb_Data_${SYSTYPE[ISYS]}/gammaJetHistogramArithmetic_*.root > $VAR_FILE_LIST

    ./one_sigma_systematics /export/d00/scratch/biran/gamma-jet/2017-06-20-incremental/PbPb_Data_gammaJetHistogramArithmetic.root $VAR_FILE_LIST $HIST_LIST ${SYSTYPE[ISYS]}
done
