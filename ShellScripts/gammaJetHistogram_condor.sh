if [[ $# -ne 4 ]]
then
  echo "Usage: ./gammaJetHistogram_condor.sh <cutconfig> <inputFile> <outputDir> <nJobs>"
  exit 1
fi

now="submit_$(date +"%Y-%m-%d__%H_%M_%S")"
mkdir $now
echo "Working directory: $now"

mkdir -p $2

cat > $now/pmerge.condor <<EOF
Universe     = vanilla
Initialdir   = $PWD/$now
Notification = Error
Executable   = $PWD/$now/merge.sh
Arguments    = $1 $2 $3 \$(Process) 
GetEnv       = True
#Output       = \$(Process).out
#Error        = \$(Process).err
#Log          = \$(Process).log
Rank         = Mips
+AccountingGroup = "group_cmshi.$(whoami)"
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = Histogramming/gammaJetHistogram.exe,$1

Queue $4

EOF

cat > $now/merge.sh <<EOF
./gammaJetHistogram.exe \$1 \$2 gammaJetHistogram_\${4}.root $4 \$4
mv gammaJetHistogram_\${1}.root \$3
EOF

#cat $now/pmerge.condor
#cat $now/merge.sh
#echo condor_submit $now/pmerge.condor
condor_submit $now/pmerge.condor
