#!/bin/bash
set -x

DATE=$(date +"%Y-%m-%d_%H_%M_%S")
#DATE=2016-05-30_16_31_06

OUTDIR=/export/d00/scratch/luck/GAMMAJETFILES/${DATE}
mkdir -p $OUTDIR

echo "### PbPb Data/MC ###"

echo "## Skimming"
INFILE=./ShellScripts/PbPb_Data_HiForest.list
OUTFILE=PbPb_Data_gammaJetSkim.root
time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet.conf ${INFILE} ${OUTDIR}/${OUTFILE} /export/d00/scratch/luck/GAMMAJETFILES/minBiasJetSkim/minbiasJetSkim.root > ${OUTDIR}/${OUTFILE}.log || echo "ERROR"

INFILE=./ShellScripts/PbPb_MC_HiForest.list
OUTMCSKIM=PbPb_MC_gammaJetSkim.root
time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet_mc.conf ${INFILE} ${OUTDIR}/${OUTMCSKIM}  /export/d00/scratch/luck/GAMMAJETFILES/PbPb_MC_minbiasJetSkim.root > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

echo "## Histogramming"
INFILE=${OUTDIR}/${OUTFILE}
INMC=${OUTDIR}/${OUTMCSKIM}
OUTFILE=PbPb_Data_gammaJetHistogram.root
time ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet.conf ${INFILE} ${INMC} ${OUTDIR}/${OUTFILE} > ${OUTDIR}/${OUTFILE}.log || echo "ERROR"

INFILE=${OUTDIR}/${OUTMCSKIM}
INMC=${INFILE}
OUTFILE=PbPb_MC_gammaJetHistogram.root
time ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet_mc.conf ${INFILE} ${INMC} ${OUTDIR}/${OUTFILE} > ${OUTDIR}/${OUTFILE}.log || echo "ERROR"

echo "### pp Data/MC ###"

echo "## Skimming"
OUTFILE=pp_Data_gammaJetSkim.root
time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet_pp.conf /mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root ${OUTDIR}/${OUTFILE}  > ${OUTDIR}/${OUTFILE}.log || echo "ERROR"

INFILE=./ShellScripts/pp_MC_HiForest.list
OUTMCSKIM=pp_MC_gammaJetSkim.root
time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet_pp_mc.conf $INFILE ${OUTDIR}/${OUTMCSKIM}  > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

echo "## Histogramming"
INFILE=${OUTDIR}/${OUTFILE}
INMC=${OUTDIR}/${OUTMCSKIM}
OUTFILE=pp_Data_gammaJetHistogram.root
time ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet_pp.conf ${INFILE} ${INMC} ${OUTDIR}/${OUTFILE}  > ${OUTDIR}/${OUTFILE}.log || echo "ERROR"

INFILE=${OUTDIR}/${OUTMCSKIM}
INMC=${INFILE}
OUTFILE=pp_MC_gammaJetHistogram.root
time ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet_pp_mc.conf ${INFILE} ${INMC} ${OUTDIR}/${OUTFILE}  > ${OUTDIR}/${OUTFILE}.log || echo "ERROR"
