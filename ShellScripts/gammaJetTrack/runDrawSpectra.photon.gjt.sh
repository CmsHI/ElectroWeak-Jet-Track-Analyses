#!/bin/bash

#g++ -Wall -Werror -Wextra -O2 -Wno-narrowing `root-config --cflags --libs` -lTMVA -lRooFitCore -lRooFit -MMD -MF ./build/Performance/drawSpectra.d Performance/drawSpectra.C -o Performance/drawSpectra.exe

runCmd="./ShellScripts/myRun.sh"
progPath="./Performance/drawSpectra.exe"
inputFile="/export/d00/scratch/biran/photon-jet-track/PbPb-Data-skim-170712.root"

configFiles=(
"Configurations/gammaJetTrack/spectra.photon.gjt.conf"
"Configurations/gammaJetTrack/spectra.photon.gjt.pho60.conf"
"Configurations/gammaJetTrack/spectra.photon.gjt.pho60.phoBkg.conf"
"Configurations/gammaJetTrack/spectra.photon.gjt.pho60.phoSig.conf"
"Configurations/gammaJetTrack/spectra.photon.gjt.pho80.conf"
"Configurations/gammaJetTrack/spectra2D.photon.etaphi.gjt.conf"
);

outDirBase="/export/d00/scratch/"$USER"/GJT-out"
outputSuffix="PbPb_Data_skim_170712"
outList=(
$outDirBase"/spectra_photon_gjt_"$outputSuffix".root"
$outDirBase"/spectra_photon_gjt_pho60_"$outputSuffix".root"
$outDirBase"/spectra_photon_gjt_pho60_phoBkg_"$outputSuffix".root"
$outDirBase"/spectra_photon_gjt_pho60_phoSig_"$outputSuffix".root"
$outDirBase"/spectra_photon_gjt_pho80_"$outputSuffix".root"
$outDirBase"/spectra2D_photon_etaphi_gjt_"$outputSuffix".root"
);

arrayIndices=${!outList[*]}
for i1 in $arrayIndices
do
    configFile=${configFiles[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir
    $runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &
    echo "$runCmd $progPath $configFile $inputFile $outputFile &> $outputFileLOG &"
done


