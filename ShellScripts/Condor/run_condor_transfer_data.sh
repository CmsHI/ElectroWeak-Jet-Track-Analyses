#!/bin/bash

# simple script to transfer stdout and stderr files for condor jobs
if [ $# -ne 1 ] && [ $# -ne 2 ];
then
  echo "Usage 1: ./run_condor_transfer_data.sh <clusterID>"
  echo "Usage 2: ./run_condor_transfer_data.sh <clusterID> <nJobs>"
  exit 1
fi

clusterID=$1

echo "running condor_transfer_data..."
if [ $# == 1 ];
then
  echo "transfer data for clusterID = "${clusterID}
  condor_transfer_data -name submit.mit.edu ${clusterID}
elif [ $# == 2 ];
then
  nJobs=$2
  for (( iJob=0; iJob<nJobs; iJob++ ))
  do
    echo "transfer data for clusterID.job = "${clusterID}.${iJob}
    condor_transfer_data -name submit.mit.edu ${clusterID}.${iJob}
  done
fi
echo "running condor_transfer_data - END"

