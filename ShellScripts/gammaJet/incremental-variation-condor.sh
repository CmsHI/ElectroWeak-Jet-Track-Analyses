#!/bin/bash

if [[ $# -ne 6 ]]; then
  echo "Usage: ./ShellScripts/gammaJet/incremental-variation-condor.sh [config] [input] [output dir] [njobs] [suffix] [arithmetic files dir]"
  exit 1
fi

PROXYFILE=$(ls /tmp/ -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}')

now="incremental_variations_$(date +"%Y-%m-%d_%H_%M_%S")"
mkdir $now
echo "Working directory: $now"

SRM_PREFIX="/mnt/hadoop/"
SRM_PATH=${3#${SRM_PREFIX}}

gfal-mkdir -p gsiftp://se01.cmsaf.mit.edu:2811/${SRM_PATH}
cp Histogramming/gammaJetHistogram.exe $now/
cp Histogramming/gammaJetHistogramArithmetic.exe $now/
cp Histogramming/gammaJetFinalHistograms.exe $now/
cp $6/PbPb_MC_gammaJetHistogramArithmetic${5}.root $now/
cp $6/pp_Data_gammaJetHistogramArithmetic${5}.root $now/
cp $6/pp_MC_gammaJetHistogramArithmetic${5}.root $now/
cp $1 $now/

BINARIES="gammaJetHistogram.exe,gammaJetHistogramArithmetic.exe,gammaJetFinalHistograms.exe"
ARITHFILES="PbPb_MC_gammaJetHistogramArithmetic${5}.root,pp_Data_gammaJetHistogramArithmetic${5}.root,pp_MC_gammaJetHistogramArithmetic${5}.root"
CONF="$(basename $1)"

cat > $now/incremental-variation.condor <<EOF
Universe     = vanilla
Initialdir   = $PWD/$now
# Request_memory = 4096
Notification = Error
Executable   = $PWD/$now/incremental-variation.sh
Arguments    = $1 $2 $3 $4 \$(Process)
GetEnv       = True
Output       = \$(Process).out
Error        = \$(Process).err
Log          = \$(Process).log
Rank         = Mips
+AccountingGroup = "group_cmshi.$(whoami)"
requirements = GLIDEIN_Site == "MIT_CampusFactory" && BOSCOGroup == "bosco_cmshi" && HAS_CVMFS_cms_cern_ch && BOSCOCluster == "ce03.cmsaf.mit.edu"
job_lease_duration = 480
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = /tmp/$PROXYFILE,$BINARIES,$ARITHFILES,$CONF

Queue $4
EOF

cat > $now/incremental-variation.sh <<EOF
#!/bin/bash

ls
echo \$(whoami)
echo \$HOSTNAME

# setup grid proxy
export X509_USER_PROXY=\${PWD}/$PROXYFILE

# set hadoop directory path for xrdcp
SRM_PREFIX="/mnt/hadoop/"
SRM_PATH=\${3#\${SRM_PREFIX}}

NVARIATION=\$((\$5+1))
VARIATION="\$(bc -l <<< "scale=4; \${NVARIATION}/\${4}")"

# set up correct directory structure
mkdir CutConfigurations
mv gammaJet_*.conf CutConfigurations/

set -x

# run gammaJetHistogram
./gammaJetHistogram.exe \$1 \$2 gammaJetHistogram_\${5}.root -1 -1 \$VARIATION
./gammaJetHistogramArithmetic.exe \$1 gammaJetHistogram_\${5}.root gammaJetHistogramArithmetic_\${5}.root
./gammaJetFinalHistograms.exe \$1 gammaJetHistogramArithmetic_\${5}.root PbPb_MC_gammaJetHistogramArithmetic${5}.root pp_Data_gammaJetHistogramArithmetic${5}.root pp_MC_gammaJetHistogramArithmetic${5}.root gammaJetHistograms_\${5}.root

if [[ \$? -eq 0 ]]; then
  mv gammaJetHistogram_\${5}.root \$3
  mv gammaJetHistogramArithmetic_\${5}.root \$3
  mv gammaJetHistograms_\${5}.root \$3

  if [[ \$? -ne 0 ]]; then
    gfal-copy file://\${PWD}/gammaJetHistogram_\${5}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetHistogram_\${5}.root
    gfal-copy file://\${PWD}/gammaJetHistogramArithmetic_\${5}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetHistogramArithmetic_\${5}.root
    gfal-copy file://\${PWD}/gammaJetHistograms_\${5}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetHistograms_\${5}.root

    if [[ \$? -ne 0 ]]; then
      srmcp -2 file:///\${PWD}/gammaJetHistogram_\${5}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetHistogram_\${5}.root
      srmcp -2 file:///\${PWD}/gammaJetHistogramArithmetic_\${5}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetHistogramArithmetic_\${5}.root
      srmcp -2 file:///\${PWD}/gammaJetHistograms_\${5}.root gsiftp://se01.cmsaf.mit.edu:2811/\${SRM_PATH}/gammaJetHistograms_\${5}.root
    fi
  fi
fi

rm *.exe *.root
EOF

condor_submit $now/incremental-variation.condor -pool submit.mit.edu:9615 -name submit.mit.edu -spool
