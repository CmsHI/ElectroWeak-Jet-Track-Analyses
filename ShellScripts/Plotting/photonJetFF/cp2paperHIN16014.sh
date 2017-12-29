#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/Plotting/photonJetFF/cp2paperHIN16014.sh [PAPER trunk dir]"
  echo "Example: ./ShellScripts/Plotting/photonJetFF/cp2paperHIN16014.sh /path/to/myDocuments/HIN-16-014/papers/HIN-16-014/trunk/"
  exit 1
fi

destinationDir=$1
sourceDir="./"

sourceFiles=(
"ff_60_30_gxi0.pdf"
"ff_60_30_gxi1.pdf"
"photonjetFF_compact_fig1.pdf"
"photonjetFF_compact_fig2.pdf"
);

destinationFiles=(
"figures/xijet_phoet60_jetpt30.pdf"
"figures/xigamma_phoet60_jetpt30.pdf"
"figures/xijet_phoet60_jetpt30_PRL.pdf"
"figures/xigamma_phoet60_jetpt30_PRL.pdf"
);

arrayIndices=${!sourceFiles[*]}
for i1 in $arrayIndices
do
    srcFile=${sourceFiles[i1]}
    destFile=${destinationFiles[i1]}
    cp $sourceDir$srcFile $destinationDir$destFile
    echo "cp $sourceDir$srcFile $destinationDir$destFile"
done

