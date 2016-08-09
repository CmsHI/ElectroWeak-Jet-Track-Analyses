if [[ $# -ne 4 ]]
then
  echo "Usage: ./gammaJetHistogram_condor.sh <cutconfig> <inputFile> <outputDir> <nJobs>"
  exit 1
fi

now="$(basename $2)_$(date +"%Y-%m-%d_%H_%M_%S")"
mkdir $now
echo "Working directory: $now"

mkdir -p $3
cp Histogramming/gammaJetHistogram.exe $now/
mkdir -p $now/CutConfigurations/
cp $1 $now/CutConfigurations/
cp ./CutConfigurations/gammaJet.conf $now/CutConfigurations/

cat > $now/pmerge.condor <<EOF
Universe     = vanilla
Initialdir   = $PWD/$now
Notification = Error
Executable   = $PWD/$now/merge.sh
Arguments    = $1 $2 $3 \$(Process) 
GetEnv       = True
Output       = \$(Process).out
Error        = \$(Process).err
Log          = \$(Process).log
Rank         = Mips
+AccountingGroup = "group_cmshi.$(whoami)"
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = gammaJetHistogram.exe,$1,./CutConfigurations/gammaJet.conf

Queue $4

EOF

cat > $now/merge.sh <<EOF
mkdir CutConfigurations/
mv gammaJet*.conf CutConfigurations/
./gammaJetHistogram.exe \$1 \$2 gammaJetHistogram_\${4}.root $4 \$4
mv gammaJetHistogram_\${4}.root \$3
rm *.root
EOF

#cat $now/pmerge.condor
#cat $now/merge.sh
#echo condor_submit $now/pmerge.condor
condor_submit $now/pmerge.condor
