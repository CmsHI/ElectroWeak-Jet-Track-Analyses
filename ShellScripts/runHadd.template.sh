#!/bin/bash

set -x

mergedFinal="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias/180205_205303/L1Ntuple_merged.root"
mergeInput="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_XeXeRun2017_HIMinimumBias/180205_205303/0000/L1Ntuple_*.root"

timeNow=$(date +"%Y%m%d_%H%M%S")
mergedTMP="/export/d00/scratch/"$USER"/mergedTMP_"$timeNow".root"
hadd -f $mergedTMP $mergeInput

mv $mergedTMP $mergedFinal
if [ $? -ne 0 ]; then
  srmPrefix="/mnt/hadoop/"
  if [ ${mergedFinal:0:12} = $srmPrefix ]; then
    outputSRM=${mergedFinal#${srmPrefix}}
    gfal-copy file://${mergedTMP} gsiftp://se01.cmsaf.mit.edu:2811/${outputSRM}
  fi
  rm $mergedTMP
fi

