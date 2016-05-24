#!/bin/bash
set -x

DATE=$(date +"%Y-%m-%d_%H:%M:%S")

OUTDIR=/export/d00/scratch/luck/GAMMAJETFILES/${DATE}/
mkdir -p OUTDIR

echo "### PbPb Data/MC ###"

echo "## Skimming"
INFILE=PbPb_Data_HiForest.list
OUTFILE=PbPb_Data_gammaJetSkim.root
../ForestSkimmers/photons/gammaJetSkim.exe ../CutConfigurations/gammaJet.conf ${INFILE} ${OUTDIR}/${OUTFILE} /export/d00/scratch/luck/GAMMAJETFILES/minBiasJetSkim/minbiasJetSkim.root || echo "ERROR"

INFILE=PbPb_MC_HiForest.list
OUTMCSKIM=PbPb_MC_gammaJetSkim.root
../ForestSkimmers/photons/gammaJetSkim.exe ../CutConfigurations/gammaJet_mc.conf ${INFILE} ${OUTDIR}/${OUTMCSKIM}  /export/d00/scratch/luck/GAMMAJETFILES/PbPb_MC_minbiasJetSkim.root || echo "ERROR"

echo "## Histogramming"
INFILE=${OUTDIR}/${OUTFILE}
INMC=${OUTDIR}/${OUTMCSKIM}
OUTFILE=PbPb_Data_gammaJetHistogram.root
../Histogramming/gammaJetHistogram.exe ../CutConfigurations/gammaJet.conf ${INFILE} ${INMC} ${OUTDIR}/${OUTFILE} || echo "ERROR"

INFILE=${OUTDIR}/${OUTMCSKIM}
INMC=${INFILE}
OUTFILE=PbPb_MC_gammaJetHistogram.root
../Histogramming/gammaJetHistogram.exe ../CutConfigurations/gammaJet_mc.conf ${INFILE} ${INMC} ${OUTDIR}/${OUTFILE} || echo "ERROR"

echo "### pp Data/MC ###"

echo "## Skimming"
OUTFILE=pp_Data_gammaJetSkim.root
../ForestSkimmers/photons/gammaJetSkim.exe ../CutConfigurations/gammaJet_pp.conf /mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root ${OUTDIR}/${OUTFILE} || echo "ERROR"

INFILE=pp_MC_HiForest.list
OUTMCSKIM=pp_MC_gammaJetSkim.root
../ForestSkimmers/photons/gammaJetSkim.exe ../CutConfigurations/gammaJet_pp.conf $INFILE ${OUTDIR}/${OUTFILE} || echo "ERROR"

echo "## Histogramming"
INFILE=${OUTDIR}/${OUTFILE}
INMC=${OUTDIR}/${OUTMCSKIM}
OUTFILE=pp_Data_gammaJetHistogram.root
../Histogramming/gammaJetHistogram.exe ../CutConfigurations/gammaJet_pp.conf ${INFILE} ${INMC} ${OUTDIR}/${OUTFILE} || echo "ERROR"

INFILE=${OUTDIR}/${OUTMCSKIM}
INMC=${INFILE}
OUTFILE=pp_MC_gammaJetHistogram.root
../Histogramming/gammaJetHistogram.exe ../CutConfigurations/gammaJet_pp.conf ${INFILE} ${INMC} ${OUTDIR}/${OUTFILE} || echo "ERROR"
