#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/readWriteTTree.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -O2 -o $progPath || exit 1

inputList=(
"Configurations/filelists/Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST.list"
);

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputSuffix="Pythia8_Photon30_pp502_TuneCUETP8M1_HINppWinter16DR_FOREST"
outList=(
$outDirBase"/Performance/readWriteTTree_"$outputSuffix".root"
);

argFileTmp="./Performance/readWriteTTree.args.txt"
rm -f $argFileTmp
## list of TTrees to read and write
echo "==trees==" >> $argFileTmp
echo "ggHiNtuplizerGED/EventTree" >> $argFileTmp
echo "ak4PFJetAnalyzer/t" >> $argFileTmp
## list of branches to read and write
echo "==branches==" >> $argFileTmp
echo "nPho,pho*,nMC,mc*" >> $argFileTmp
echo "nref,jtpt,jteta,jtphi,subid,rawpt,ref*" >> $argFileTmp

argFiles=(
$argFileTmp
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
  inputFile=${inputList[i1]}
  outputFile=${outList[i1]}
  outputFileLOG="${outputFile/.root/.log}"
  argFile=${argFiles[i1]}
  outDir=$(dirname "${outputFile}")
  mkdir -p $outDir
  $runCmd $progPath $inputFile $outputFile $argFile &> $outputFileLOG &
  echo "$runCmd $progPath $inputFile $outputFile $argFile &> $outputFileLOG &"
done


