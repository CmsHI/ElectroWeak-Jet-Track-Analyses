#!/bin/bash

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/nTupleGetEntries.exe"
inputFiles=(
"Configurations/filelists/Pythia8_EmEnrichedDijet80_Hydjet_Cymbal_MB.list"
);

#make $progPath || exit 1

outDirBase=$EWJTAOUT
if [ -z "$outDirBase" ]; then
  outDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi
outputFiles=(
$outDirBase"/Performance/photons/nTupleGetEntries_mcCalIso_hOverE_EmEnrichedDijet80_Hydjet_Cymbal_MB.log"
);

argFileTmp="./Performance/nTupleGetEntries.args.txt"
rm -f $argFileTmp
echo "==selections==" >> $argFileTmp
evtSel="pcollisionEventSelection > 0 && abs(vz) < 15 && hiBin < 60"
spikeReject="abs(pho_seedTime)<3 && pho_swissCrx <0.9 && phoSigmaIEtaIEta_2012 > 0.002"
sumIso="(pho_ecalClusterIsoR4 + pho_hcalRechitIsoR4 + pho_trackIsoR4PtCut20) < 1"
sieie="phoSigmaIEtaIEta_2012 < 0.01"
sieieSB="phoSigmaIEtaIEta_2012 > 0.011 && phoSigmaIEtaIEta_2012 < 0.017"
genMatching="pho_genMatchedIndex >=0 && mcPID[pho_genMatchedIndex] == 22 && mcMomPID[pho_genMatchedIndex] == 111"
genPi02Pho="mcPID == 22 && abs(mcEta) <= 1.44 && mcMomPID == 111 && mcMomPt >= 40"
echo "$evtSel && $genPi02Pho" >> $argFileTmp
echo "$evtSel && $genPi02Pho && mcCalIsoDR04 < 5" >> $argFileTmp
echo "$evtSel && Sum\$($genPi02Pho) > 0 && phoEt >= 40 && abs(phoEta) <= 1.44 && $spikeReject && phoHoverE < 0.4" >> $argFileTmp
echo "$evtSel && Sum\$($genPi02Pho) > 0 && phoEt >= 40 && abs(phoEta) <= 1.44 && $spikeReject && phoHoverE < 0.1" >> $argFileTmp
echo "$evtSel && Sum\$($genPi02Pho && mcCalIsoDR04 < 5) > 0 && phoEt >= 40 && abs(phoEta) <= 1.44 && $spikeReject && phoHoverE < 0.4" >> $argFileTmp
echo "$evtSel && Sum\$($genPi02Pho && mcCalIsoDR04 < 5) > 0 && phoEt >= 40 && abs(phoEta) <= 1.44 && $spikeReject && phoHoverE < 0.1" >> $argFileTmp
#echo "$evtSel && phoEt >= 40 && abs(phoEta) <= 1.44 && $spikeReject && $genMatching && mcCalIsoDR04[pho_genMatchedIndex] < 5 && phoHoverE < 0.1" >> $argFileTmp
#echo "$evtSel && phoEt >= 40 && abs(phoEta) <= 1.44 && $spikeReject && $genMatching && phoHoverE < 0.1" >> $argFileTmp
#echo "$evtSel && phoEt >= 40 && abs(phoEta) <= 1.44 && $spikeReject && $genMatching && mcCalIsoDR04[pho_genMatchedIndex] < 5 && phoHoverE < 0.4" >> $argFileTmp
#echo "$evtSel && phoEt >= 40 && abs(phoEta) <= 1.44 && $spikeReject && $genMatching && phoHoverE < 0.4" >> $argFileTmp
#echo "phoEt >= 40 && phoEt < 60 && abs(phoEta) <= 1.44 && $spikeReject && $genMatching && phoHoverE < 0.1" >> $argFileTmp
#echo "phoEt >= 60 && abs(phoEta) <= 1.44 && $spikeReject && $genMatching && phoHoverE < 0.1" >> $argFileTmp
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


