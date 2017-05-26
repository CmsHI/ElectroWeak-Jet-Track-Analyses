#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo -e "Usage: ./ShellScripts/gammaJet/skim-minbias.sh [hadoop dir] [output dir]"
    echo -e "[hadoop dir] will contain copies of the output skims"
    echo -e "[output dir] will contain output skims and logs"
    exit 1
fi

set -x

HADOOPDIR=$1
OUTDIR=$2
mkdir -p $HADOOPDIR
mkdir -p $OUTDIR

INFILE=./ShellScripts/gammaJet/PbPb_Data_MB_HiForest.list
OUTFILE=PbPb_Data_minbiasJetSkim.root
./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/PbPb_Data_minbiasJetSkim.log &

INFILE=/mnt/hadoop/cms/store/user/biran/photon-jet-track/PbPb-MB-Hydjet-Cymbal-170331.root
OUTFILE=PbPb_MC_minbiasJetSkim.root
./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet_mc.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/PbPb_MC_minbiasJetSkim.log &
wait

cp ${OUTDIR}/*_minbiasJetSkim.root $HADOOPDIR/
