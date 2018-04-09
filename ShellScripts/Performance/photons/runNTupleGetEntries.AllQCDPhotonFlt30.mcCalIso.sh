#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"

cat Configurations/filelists/Pythia8_AllQCDPhoton50Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST.list | head -200 > Configurations/filelists/Pythia8_AllQCDPhoton50Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST_N200.list

inputFiles=(
"Configurations/filelists/Pythia8_AllQCDPhoton50Flt30_Hydjet_Cymbal_MB_HINPbPbWinter16DR_FOREST_N200.list"
);

#make $progPath || exit 1

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputFiles=(
$outDirBase"/Performance/photons/nTupleGetEntries_mcCalIso_AllQCDPhoton50Flt30_Hydjet_Cymbal_MB.log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
evtSel="pcollisionEventSelection > 0 && abs(vz) < 15 && hiBin < 60"
spikeReject="abs(pho_seedTime)<3 && pho_swissCrx <0.9 && phoSigmaIEtaIEta_2012 > 0.002"
sumIso="(pho_ecalClusterIsoR4 + pho_hcalRechitIsoR4 + pho_trackIsoR4PtCut20) < 1"
sieie="phoSigmaIEtaIEta_2012 < 0.01"
sieieSB="phoSigmaIEtaIEta_2012 > 0.011 && phoSigmaIEtaIEta_2012 < 0.017"
genPho="mcPID == 22 && mcPt >= 35 && abs(mcEta) <= 1.44"
echo "$genPho" >> $argFileTmp
echo "$genPho && mcCalIsoDR04 < 5" >> $argFileTmp
echo "==trees==" >> $argFileTmp
echo "ggHiNtuplizer/EventTree" >> $argFileTmp
echo "hiEvtAnalyzer/HiTree" >> $argFileTmp
echo "skimanalysis/HltTree" >> $argFileTmp

argFiles=(
$argFileTmp
);

arrayIndices=${!outputFiles[*]}
for i1 in $arrayIndices
do
    inputFile=${inputFiles[i1]}
    outputFile=${outputFiles[i1]}
    argFile=${argFiles[i1]}
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $inputFile $argFile &> $outputFile &
    echo "$runCmd $progPath $inputFile $argFile &> $outputFile &"
done


