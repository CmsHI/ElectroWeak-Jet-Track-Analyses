#!/bin/bash

if [ $# -lt 1 ]; then
    echo -e "Usage: ./ShellScripts/gammaJet/skim-minbias.sh [label]"
    exit 1
fi

set -x

LABEL=$1

OUTDIR=/export/d00/scratch/biran/gamma-jet/${LABEL}
mkdir -p $OUTDIR
H_INDIR=/mnt/hadoop/cms/store/user/biran/gamma-jet/${LABEL}/
mkdir -p $H_INDIR

INFILE=./ShellScripts/gammaJet/PbPb_Data_MB_HiForest.list
OUTFILE=PbPb_Data_minbiasJetSkim.root
./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/PbPb_Data_minbiasJetSkim.log &

#INFILE=/mnt/hadoop/cms/store/user/rbi/merged/Hydjet_Quenched_MinBias_5020GeV_750-HINPbPbWinter16DR-NoPU_75X_mcRun2_HeavyIon_forest_v2/0.root
INFILE=/mnt/hadoop/cms/store/user/biran/photon-jet-track/PbPb-MB-Hydjet-Cymbal-170331.root
OUTFILE=PbPb_MC_minbiasJetSkim.root
./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet_mc.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/PbPb_MC_minbiasJetSkim.log &
wait

cp ${OUTDIR}/*_minbiasJetSkim.root $H_INDIR/
