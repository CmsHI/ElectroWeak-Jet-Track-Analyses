#!/bin/bash

if [ $# -lt 2 ]; then
  echo "Usage: ./ShellScripts/runHadd.sh <mergedFinal> <mergeInput>"
  exit 1
fi

mergedFinal=$1
mergeInput=${@:2}
### Examples for setting parameters in the script
## Ex. 1
# mergedFinal="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIPhoton40AndZ_egBypassEGVetos1/180218_063831/L1Ntuple_merged_v2.root"
# mergeInput="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIPhoton40AndZ_egBypassEGVetos1/180218_063831/0000/L1Ntuple_??.root"
## Ex. 2
# mergedFinal="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassShapeBit1/180309_221156/L1Ntuple_merged.root"
# mergeInput="$(ls -1 /mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassShapeBit1/180309_221156/0000/L1Ntuple_*.root | head -46)"
## Ex. 3
# mergedFinal="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1/180309_221027/L1Ntuple_merged.root"
# mergeInput="/mnt/hadoop/cms/store/user/katatar/HIRun2018PbPb/L1T/L1Ntuple_HIRun2015_HIMinimumBias2_egBypassFGBit1/180309_221027/0000/L1Ntuple_*.root"

echo "mergedFinal = $mergedFinal"
echo "mergeInput = $mergeInput"

set -x

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

