#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/Plotting/photonJetFF/cp2pasHIN16014.sh [PAS trunk dir]"
  echo "Example: ./ShellScripts/Plotting/photonJetFF/cp2pasHIN16014.sh /path/to/myDocuments/HIN-16-014/notes/HIN-16-014/trunk/"
  exit 1
fi

destinationDir=$1
sourceDir="./"

sourceFiles=(
"ff_60_30_gxi0.pdf"
"ff_60_30_gxi1.pdf"
"ff_80_40_gxi0.pdf"
"ff_80_40_gxi1.pdf"
);

destinationFiles=(
"figures/xijet_phoet60_jetpt30.pdf"
"figures/xigamma_phoet60_jetpt30.pdf"
"figures/xijet_phoet80_jetpt40.pdf"
"figures/xigamma_phoet80_jetpt40.pdf"
);

arrayIndices=${!sourceFiles[*]}
for i1 in $arrayIndices
do
    srcFile=${sourceFiles[i1]}
    destFile=${destinationFiles[i1]}
    cp $sourceDir$srcFile $destinationDir$destFile
    echo "cp $sourceDir$srcFile $destinationDir$destFile"
done

