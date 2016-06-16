#!/bin/bash
set -x

#DATE=$(date +"%Y-%m-%d_%H_%M_%S")
DATE=2016-06-15_14_17_17

OUTDIR=/export/d00/scratch/luck/GAMMAJETFILES/${DATE}
H_INDIR=/mnt/hadoop/cms/store/user/luck/GAMMAJETFILES/${DATE}/

######### SKIMMING SECTION ##############
# echo "## Skimming"

#mkdir -p $OUTDIR

#echo "### PbPb Data/MC ###"

# INFILE=./ShellScripts/PbPb_Data_HiForest.list
# OUTFILE=PbPb_Data_gammaJetSkim.root
# time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet.conf ${INFILE} ${OUTDIR}/${OUTFILE} /export/d00/scratch/luck/GAMMAJETFILES/minBiasJetSkim/minbiasJetSkim.root > ${OUTDIR}/${OUTFILE}.log || echo "ERROR"

# INFILE=./ShellScripts/PbPb_MC_HiForest.list
# OUTMCSKIM=PbPb_MC_gammaJetSkim.root
# time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet_mc.conf ${INFILE} ${OUTDIR}/${OUTMCSKIM}  /export/d00/scratch/luck/GAMMAJETFILES/PbPb_MC_minbiasJetSkim.root > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

# echo "### pp Data/MC ###"

# INFILE=/mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root
# OUTFILE=pp_Data_gammaJetSkim.root
# time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet_pp.conf ${INFILE} ${OUTDIR}/${OUTFILE}  > ${OUTDIR}/${OUTFILE}.log || echo "ERROR"

# INFILE=./ShellScripts/pp_MC_HiForest.list
# OUTMCSKIM=pp_MC_gammaJetSkim.root
# time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet_pp_mc.conf $INFILE ${OUTDIR}/${OUTMCSKIM}  > ${OUTDIR}/${OUTMCSKIM}.log || echo "ERROR"

#mkdir -p ${H_INDIR}
#cp ${OUTDIR}/*.root ${H_INDIR}


########### PURITY SECTION ##################
# Optionally print out purity values to be pasted into Histogram macro
echo "## Purity Calculations"
./Performance/printPurity.exe CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetSkim.root ${OUTDIR}/PbPb_MC_gammaJetSkim.root > PbPb_Data_purity.txt

./Performance/printPurity.exe CutConfigurations/gammaJet_mc.conf ${OUTDIR}/PbPb_MC_gammaJetSkim.root ${OUTDIR}/PbPb_MC_gammaJetSkim.root > PbPb_MC_purity.txt

./Performance/printPurity.exe CutConfigurations/gammaJet_pp.conf ${OUTDIR}/pp_Data_gammaJetSkim.root ${OUTDIR}/pp_MC_gammaJetSkim.root > pp_Data_purity.txt

./Performance/printPurity.exe CutConfigurations/gammaJet_pp_mc.conf ${OUTDIR}/pp_MC_gammaJetSkim.root ${OUTDIR}/pp_MC_gammaJetSkim.root > pp_MC_purity.txt

# ########### PARALLEL HISTOGRAMMING SECTION ##########
# echo "## Histogramming Condor Jobs"
# ./ShellScripts/gammaJetHistogram_condor.sh ./CutConfigurations/gammaJet.conf ${H_INDIR}/PbPb_Data_gammaJetSkim.root ${H_INDIR}/PbPb_Data_gammaJetHistogram_unmerged/ 10

# ./ShellScripts/gammaJetHistogram_condor.sh ./CutConfigurations/gammaJet_mc.conf ${H_INDIR}/PbPb_MC_gammaJetSkim.root ${H_INDIR}/PbPb_MC_gammaJetHistogram_unmerged/ 2

# ./ShellScripts/gammaJetHistogram_condor.sh ./CutConfigurations/gammaJet_pp.conf ${H_INDIR}/pp_Data_gammaJetSkim.root ${H_INDIR}/pp_Data_gammaJetHistogram_unmerged/ 122

# ./ShellScripts/gammaJetHistogram_condor.sh ./CutConfigurations/gammaJet_pp_mc.conf ${H_INDIR}/pp_MC_gammaJetSkim.root ${H_INDIR}/pp_MC_gammaJetHistogram_unmerged/ 78

# ############ HADD/ARITHMETIC SECTION ###################
# echo "## Summing Histograms"
# hadd PbPb_Data_gammaJetHistogram.root ${H_INDIR}/PbPb_Data_gammaJetHistogram_unmerged/*.root
# cp PbPb_Data_gammaJetHistogram.root ${OUTDIR}/
# rm ${H_INDIR}/PbPb_Data_gammaJetHistogram_unmerged/*.root

# hadd PbPb_MC_gammaJetHistogram.root ${H_INDIR}/PbPb_MC_gammaJetHistogram_unmerged/*.root
# cp PbPb_MC_gammaJetHistogram.root ${OUTDIR}/
# rm ${H_INDIR}/PbPb_MC_gammaJetHistogram_unmerged/*.root

# hadd pp_Data_gammaJetHistogram.root ${H_INDIR}/pp_Data_gammaJetHistogram_unmerged/*.root
# cp pp_Data_gammaJetHistogram.root ${OUTDIR}/
# rm ${H_INDIR}/pp_Data_gammaJetHistogram_unmerged/*.root

# hadd pp_MC_gammaJetHistogram.root ${H_INDIR}/pp_MC_gammaJetHistogram_unmerged/*.root
# cp pp_MC_gammaJetHistogram.root ${OUTDIR}/
# rm ${H_INDIR}/pp_MC_gammaJetHistogram_unmerged/*.root

# ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogram.root ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root

# ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_MC_gammaJetHistogram.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root

# ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/pp_Data_gammaJetHistogram.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root

# ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/pp_MC_gammaJetHistogram.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root
