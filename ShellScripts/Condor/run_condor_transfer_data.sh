#!/bin/bash

# simple script to transfer stdout and stderr files for condor jobs
if [ $# -ne 2 ];
then
  echo "Usage: ./run_condor_transfer_data.sh <clusterID> <nJobs>"
  exit 1
fi

clusterID=$1
nJobs=$2

echo "running condor_transfer_data..."
for (( iJob=0; iJob<nJobs; iJob++ ))
do  
   echo "transfer data for clusterID.job = "${clusterID}.${iJob}
   condor_transfer_data -pool submit.mit.edu:9615 -name submit.mit.edu ${clusterID}.${iJob}
done
echo "running condor_transfer_data - END"

