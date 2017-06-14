if [[ $# -ne 5 ]]; then
  echo "Usage: ./ShellScripts/gammaJet/gamma-jet-condor.sh [config] [input] [output dir] [njobs] [mixing file]"
  exit 1
fi

PROXYFILE=$(ls /tmp/ -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}')

now="$(basename $1 .conf)_$(date +"%Y-%m-%d_%H_%M_%S")"
mkdir $now
echo "Working directory: $now"

mkdir -p $3
cp ForestSkimmers/photons/gammaJetSkim.exe $now/
cp Histogramming/gammaJetHistogram.exe $now/

cp ShellScripts/gammaJet/*.list $now/

CONFNAME="$(basename $1)"
cp ./CutConfigurations/gammaJet*.conf $now/

cp Corrections/jets/merged_Pythia8_Photon50_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1_0_20160801_pthat_50_RESIDUALCORR.root Corrections/jets/L2L3/L2L3VsPtEtaBinned_alphacut_high2_ak3PF_etacut3_dopPb0.root Corrections/photonEnergyCorrections.root Corrections/photonEnergyCorrections_pp.root Corrections/sumIsoCorrections_Data.root Corrections/sumIsoCorrections_MC.root Corrections/PbPb_MC_weights.root Corrections/pp_MC_weights.root $now/

BINARIES="gammaJetSkim.exe,gammaJetHistogram.exe"
FILESTOCOPY="merged_Pythia8_Photon50_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1_0_20160801_pthat_50_RESIDUALCORR.root,L2L3VsPtEtaBinned_alphacut_high2_ak3PF_etacut3_dopPb0.root,photonEnergyCorrections.root,photonEnergyCorrections_pp.root,sumIsoCorrections_Data.root,sumIsoCorrections_MC.root,PbPb_MC_weights.root,pp_MC_weights.root"
BASECONFS="gammaJet.conf,gammaJet_JES_DOWN.conf,gammaJet_JES_UP.conf"
BASELISTS="PbPb_Data_HiForest.list,PbPb_MC_Flt30_HiForest.list,pp_MC_HiForest.list"

cat > $now/skim-gamma-jet.condor <<EOF
Universe     = vanilla
Initialdir   = $PWD/$now
# Request_memory = 4096
Notification = Error
Executable   = $PWD/$now/skim-gamma-jet.sh
Arguments    = $1 $2 $3 \$(Process) $5
GetEnv       = True
Output       = \$(Process).out
Error        = \$(Process).err
Log          = \$(Process).log
Rank         = Mips
+AccountingGroup = "group_cmshi.$(whoami)"
requirements = GLIDEIN_Site == "MIT_CampusFactory" && BOSCOGroup == "bosco_cmshi" && HAS_CVMFS_cms_cern_ch && BOSCOCluster == "ce03.cmsaf.mit.edu"
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = /tmp/$PROXYFILE,$BINARIES,$CONFNAME,$BASECONFS,$FILESTOCOPY,$BASELISTS

Queue $4

EOF

cat > $now/skim-gamma-jet.sh <<EOF
#!/bin/bash

echo \$HOSTNAME

# setup grid proxy
export X509_USER_PROXY=\${PWD}/$PROXYFILE

# set hadoop directory path for xrdcp
XRDCP_PREFIX="/mnt/hadoop/cms/"
XRDCP_PATH=\${3#\${XRDCP_PREFIX}}
SRM_PREFIX="/mnt/hadoop/"
SRM_PATH=\${3#\${SRM_PREFIX}}

# setup local folders with correct directory structure
mkdir CutConfigurations/
mv gammaJet*.conf CutConfigurations/
mkdir -p ShellScripts/gammaJet/
mv *.list ShellScripts/gammaJet/
mkdir -p Corrections/jets/L2L3
mv *.root Corrections/
mv Corrections/merged_Pythia8_Photon50_Hydjet_MB-HINPbPbWinter16DR-75X_mcRun2_HeavyIon_forest_v1_0_20160801_pthat_50_RESIDUALCORR.root Corrections/jets/
mv Corrections/L2L3VsPtEtaBinned_alphacut_high2_ak3PF_etacut3_dopPb0.root Corrections/jets/L2L3/

set -x

# run gammaJetSkim
./gammaJetSkim.exe \$1 \$2 gammaJetSkim_\${4}.root \${5} $4 \${4}

# run gammaJetHistogram
./gammaJetHistogram.exe \$1 gammaJetSkim_\${4}.root gammaJetHistogram_\${4}.root

if [[ \$? -eq 0 ]]; then
  mv gammaJetSkim_\${4}.root \$3
  mv gammaJetHistogram_\${4}.root \$3

  if [[ \$? -ne 0 ]]; then
    gfal-copy file://\${PWD}/gammaJetSkim_\${4}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetSkim_\${4}.root
    gfal-copy file://\${PWD}/gammaJetHistogram_\${4}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetHistogram_\${4}.root

    if [[ \$? -ne 0 ]]; then
      xrdcp gammaJetSkim_\${4}.root root://xrootd.cmsaf.mit.edu//\${XRDCP_PATH}
      xrdcp gammaJetHistogram_\${4}.root root://xrootd.cmsaf.mit.edu//\${XRDCP_PATH}

      if [[ \$? -ne 0 ]]; then
        lcg-cp -v -D srmv2 -b file://\${PWD}/gammaJetSkim_\${4}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetSkim_\${4}.root
        lcg-cp -v -D srmv2 -b file://\${PWD}/gammaJetHistogram_\${4}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetHistogram_\${4}.root
      fi
    fi
  fi
fi

rm -r CutConfigurations ShellScripts Corrections
rm *.exe *.root
EOF

condor_submit $now/skim-gamma-jet.condor -pool submit.mit.edu:9615 -name submit.mit.edu -spool
