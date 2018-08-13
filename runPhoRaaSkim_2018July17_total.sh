#!/bin/bash
set -x

#DATE=$(date +"%Y-%m-%d_%H_%M_%S")
DATE=2018-08-05-nominal

OUTDIR=/u/user/goyeonju/scratch/files/photons2016/GAMMAJETFILES/${DATE}

# ############ SKIMMING SECTION ##############
echo "## Skimming"

mkdir -p $OUTDIR
#int photonRaaSkim(const TString configFile, const TString inputFile, const TString outputFile = "photonRaaSkim.root", const int isEmEnr=0, const int isHighPhotonPD=false, const int nJobs=-1, const int jobNum=-1) 
#
echo "### pp DATA ###" #HighPtLowerPhotons for low pt photons
INFILE=./ShellScripts/photonRaa/filelist/pp_Data_HighPtLowerPhotons.list
OUTMCSKIM=pp_Data_photonRaaSkim_HighPtLowerPhotons.root
CONFIG=./CutConfigurations/photonRaa_pp.conf
time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#echo "### PbPb DATA ###" # HIHardProbes 
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbes_run262620to263035.list
#OUTMCSKIM=PbPb_Data_photonRaaSkim_HIHardProbes.root
#CONFIG=./CutConfigurations/photonRaa.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#echo "### PbPb DATA ###" # HIHardProbesPhotones run263406to263614 first 
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbesPhotons_run262233to263400.list
#OUTMCSKIM=PbPb_Data_photonRaaSkim_HIHardProbesPhotons_first.root
#CONFIG=./CutConfigurations/photonRaa.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#echo "### PbPb DATA ###" # HIHardProbesPhotones run263406to263614 first not finished 
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbesPhotons_run263233to263400_notFinished.list
#OUTMCSKIM=PbPb_Data_photonRaaSkim_HIHardProbesPhotons_first_notFinished.root
#CONFIG=./CutConfigurations/photonRaa.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#echo "### PbPb DATA ###" # HIHardProbesPhotones run263406to263614 second 
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbesPhotons_run263406to263614.list
#OUTMCSKIM=PbPb_Data_photonRaaSkim_HIHardProbesPhotons_second.root
#CONFIG=./CutConfigurations/photonRaa.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#echo "### PbPb DATA ###" # HIHardProbesPhotones total 
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbesPhotons_run263233to263614.list
#OUTMCSKIM=PbPb_Data_photonRaaSkim_HIHardProbesPhotons_total.root
#CONFIG=./CutConfigurations/photonRaa.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#echo "### PbPb DATA ###" # TOTAL : HIHardProbes + HIHardProbesPhotons 
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_Data_HiHardProbes_run262620to263035_HiHardProbesPhotons_run263233to263614_notFinished.list
#OUTMCSKIM=PbPb_Data_photonRaaSkim_HIHardProbes_HIHardProbesPhotons_trig15or20.root
#CONFIG=./CutConfigurations/photonRaa.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#echo "### pp DATA ###" #HighPtPhoton30AndZ for high pt photons
#INFILE=./ShellScripts/photonRaa/filelist/pp_Data_HighPtPhoton30AndZ.list
#OUTMCSKIM=pp_Data_photonRaaSkim_HighPtPhoton30AndZ.root
#CONFIG=./CutConfigurations/photonRaa_pp.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 0 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#echo "### PbPb DATA ###" # TOTAL : HIPhoton40AndZ 
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_Data_HIPhoton40AndZ.list
#OUTMCSKIM=PbPb_Data_photonRaaSkim_HIPhoton40AndZ.root
#CONFIG=./CutConfigurations/photonRaa.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 0 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#echo "### PbPb MC AllQCDPhotons Cymbal noFlt30 ###"
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_MC_Cymbal.list
#OUTMCSKIM=PbPb_MC_photonRaaSkim_Cymbal.root
#CONFIG=./CutConfigurations/photonRaa_mc.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
##
#echo "### pp MC AllQCDPhotons ###"
#INFILE=./ShellScripts/photonRaa/filelist/pp_MC_v1.list
#OUTMCSKIM=pp_MC_photonRaaSkim.root
#CONFIG=./CutConfigurations/photonRaa_pp_mc.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#echo "### pp MC EmEnrichedDijet ###"
#INFILE=./ShellScripts/photonRaa/filelist/pp_EmEnrMC_v1.list
#OUTMCSKIM=pp_EmEnrMC_photonRaaSkim.root
#CONFIG=./CutConfigurations/photonRaa_pp_mc_EmEnr.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"
#
#echo "### PbPb MC EmEnriched Cymbal ###"
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_EmEnrMC_Cymbal.list
#OUTMCSKIM=PbPb_EmEnrMC_photonRaaSkim.root
#CONFIG=./CutConfigurations/photonRaa_mc_EmEnr.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} 1 > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#echo "### PbPb MC AllQCDPhotons GAUS energy correction Cymbal noFlt30 ###"
#INFILE=./ShellScripts/photonRaa/filelist/PbPb_MC_Cymbal.list
#OUTMCSKIM=PbPb_MC_photonRaaSkim_Cymbal_gausECorr.root
#CONFIG=./CutConfigurations/photonRaa_mc_gausECorr.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#echo "### pp MC AllQCDPhotons GAUS energy correction ###"
#INFILE=./ShellScripts/photonRaa/filelist/pp_MC_v1.list
#OUTMCSKIM=pp_MC_photonRaaSkim_gausECorr.root
#CONFIG=./CutConfigurations/photonRaa_pp_mc_gausECorr.conf
#time ./ForestSkimmers/photons/photonRaaSkim.exe ${CONFIG} ${INFILE} ${OUTDIR}/${OUTMCSKIM} > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"


