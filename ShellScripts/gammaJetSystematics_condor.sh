if [[ $# -ne 5 ]]
then
  echo "Usage: ./gammaJetHistogram_condor.sh <cutconfig> <inputFile> <outputDir> <nJobs> <inputMixing>"
  exit 1
fi

now="$(basename $1)_$(date +"%Y-%m-%d_%H_%M_%S")"
mkdir $now
echo "Working directory: $now"

mkdir -p $3
cp ForestSkimmers/photons/gammaJetSkim.exe $now/
cp Corrections/gammaJetCorrections.exe $now/
cp Histogramming/gammaJetHistogram.exe $now/

cp ShellScripts/*.list $now/

CONFNAME="$(basename $1)"
cp ./CutConfigurations/gammaJet*.conf $now/

cp Corrections/jets/merged_Pythia8_Photon50_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1_0_20160801_pthat_50_RESIDUALCORR.root Corrections/jets/L2L3/L2L3VsPtEtaBinned_alphacut_high2_ak3PF_etacut3_dopPb0.root Corrections/photonEnergyCorrections.root Corrections/sumIsoCorrections_Data.root Corrections/sumIsoCorrections_MC.root Corrections/PbPb_MC_weights.root Corrections/pp_MC_weights.root $now/

BINARIES="gammaJetSkim.exe,gammaJetCorrections.exe,gammaJetHistogram.exe"
FILESTOCOPY="merged_Pythia8_Photon50_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1_0_20160801_pthat_50_RESIDUALCORR.root,L2L3VsPtEtaBinned_alphacut_high2_ak3PF_etacut3_dopPb0.root,photonEnergyCorrections.root,sumIsoCorrections_Data.root,sumIsoCorrections_MC.root,PbPb_MC_weights.root,pp_MC_weights.root"
BASECONFS="gammaJet.conf,gammaJet_JER.conf,gammaJet_JES_DOWN.conf,gammaJet_JES_UP.conf,gammaJet_RESIDUAL.conf"
BASELISTS="PbPb_Data_HiForest.list,PbPb_Data_MB_HiForest.list,PbPb_MC_HiForest.list,pp_MC_HiForest.list"

cat > $now/gjSystematics.condor <<EOF
Universe     = vanilla
Initialdir   = $PWD/$now
Request_memory = 4096
Notification = Error
Executable   = $PWD/$now/runGammaJetAna.sh
Arguments    = $1 $2 $3 \$(Process) $5
GetEnv       = True
Output       = \$(Process).out
Error        = \$(Process).err
Log          = \$(Process).log
Rank         = Mips
+AccountingGroup = "group_cmshi.$(whoami)"
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = $BINARIES,$CONFNAME,$BASECONFS,$FILESTOCOPY,$BASELISTS

Queue $4

EOF

cat > $now/runGammaJetAna.sh <<EOF
#!/bin/bash
set -x
# setup local folders with correct directory structure
mkdir CutConfigurations/
mv gammaJet*.conf CutConfigurations/
mkdir ShellScripts/
mv *.list ShellScripts/
mkdir -p Corrections/jets/L2L3
mv *.root Corrections/
mv Corrections/merged_Pythia8_Photon50_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1_0_20160801_pthat_50_RESIDUALCORR.root Corrections/jets/
mv Corrections/L2L3VsPtEtaBinned_alphacut_high2_ak3PF_etacut3_dopPb0.root Corrections/jets/L2L3/

#run Skim
./gammaJetSkim.exe \$1 \$2 gammaJetSkim_\${4}.root \${5} $4 \${4}

#run Corrections
./gammaJetCorrections.exe \$1 gammaJetSkim_\${4}.root gammaJetSkim_Corrected_\${4}.root

#run Histogram
./gammaJetHistogram.exe \$1 gammaJetSkim_Corrected_\${4}.root gammaJetHistogram_\${4}.root

#copy out histogram to hadoop
mv gammaJetHistogram_\${4}.root \$3
EOF

#cat $now/gjSystematics.condor
#cat $now/runGammaJetAna.sh
#echo condor_submit $now/gjSystematics.condor
condor_submit $now/gjSystematics.condor
