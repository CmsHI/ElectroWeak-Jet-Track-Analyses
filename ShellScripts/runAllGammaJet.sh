#!/bin/bash
set -x

#DATE=$(date +"%Y-%m-%d_%H_%M_%S")
DATE=2016-07-20-noEvtPlaneBins

OUTDIR=/export/d00/scratch/luck/GAMMAJETFILES/${DATE}
mkdir -p $OUTDIR

# ################ Minbias Skimming Section ####################
echo "## minbias skimming"

INFILE=./ShellScripts/PbPb_Data_MB_HiForest.list
OUTFILE=PbPb_Data_minbiasJetSkim.root
time ./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/$OUTFILE.log &

INFILE=/mnt/hadoop/cms/store/user/rbi/merged/Hydjet_Quenched_MinBias_5020GeV_750-HINPbPbWinter16DR-NoPU_75X_mcRun2_HeavyIon_forest_v2/0.root 
OUTFILE=PbPb_MC_minbiasJetSkim.root
time ./ForestSkimmers/jets/minBiasJetSkim.exe ./CutConfigurations/gammaJet_mc.conf $INFILE ${OUTDIR}/$OUTFILE > ${OUTDIR}/$OUTFILE.log &

wait

# # ############ SKIMMING SECTION ##############
echo "## Skimming"

echo "### PbPb Data/MC ###"

INFILE=./ShellScripts/PbPb_Data_HiForest.list
OUTFILE=PbPb_Data_gammaJetSkim.root
time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet.conf ${INFILE} ${OUTDIR}/${OUTFILE} ${OUTDIR}/PbPb_Data_minbiasJetSkim.root > ${OUTDIR}/${OUTFILE}.log &

INFILE=./ShellScripts/PbPb_MC_HiForest.list
OUTMCSKIM=PbPb_MC_gammaJetSkim.root
time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet_mc.conf ${INFILE} ${OUTDIR}/${OUTMCSKIM} ${OUTDIR}/PbPb_MC_minbiasJetSkim.root  > ${OUTDIR}/${OUTMCSKIM}.log &

echo "### pp Data/MC ###"

INFILE=/mnt/hadoop/cms/store/user/luck/2015-Data-promptRECO-photonSkims/pp-photonHLTFilter-v0-HiForest/0.root
OUTFILE=pp_Data_gammaJetSkim.root
time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet_pp.conf ${INFILE} ${OUTDIR}/${OUTFILE}  > ${OUTDIR}/${OUTFILE}.log &

INFILE=./ShellScripts/pp_MC_HiForest.list
OUTMCSKIM=pp_MC_gammaJetSkim.root
time ./ForestSkimmers/photons/gammaJetSkim.exe ./CutConfigurations/gammaJet_pp_mc.conf $INFILE ${OUTDIR}/${OUTMCSKIM}  > ${OUTDIR}/${OUTMCSKIM}.log &

wait

########### Photon corrections ###################
echo "## Corrections"

time ./Corrections/gammaJetCorrections.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetSkim.root ${OUTDIR}/PbPb_Data_gammaJetSkim_Corrected.root > ${OUTDIR}/PbPb_Data_gammaJetSkim_Corrected.root.log &

time ./Corrections/gammaJetCorrections.exe ./CutConfigurations/gammaJet_mc.conf ${OUTDIR}/PbPb_MC_gammaJetSkim.root ${OUTDIR}/PbPb_MC_gammaJetSkim_Corrected.root > ${OUTDIR}/PbPb_MC_gammaJetSkim_Corrected.root.log &

time ./Corrections/gammaJetCorrections.exe ./CutConfigurations/gammaJet_pp.conf ${OUTDIR}/pp_Data_gammaJetSkim.root ${OUTDIR}/pp_Data_gammaJetSkim_Corrected.root > ${OUTDIR}/pp_Data_gammaJetSkim_Corrected.root.log &

time ./Corrections/gammaJetCorrections.exe ./CutConfigurations/gammaJet_pp_mc.conf ${OUTDIR}/pp_MC_gammaJetSkim.root ${OUTDIR}/pp_MC_gammaJetSkim_Corrected.root > ${OUTDIR}/pp_MC_gammaJetSkim_Corrected.root.log &

wait

# # ########### PURITY SECTION ##################
##Optionally print out purity values to be pasted into Histogram macro
echo "## Purity Calculations"
./Performance/printPurity.exe CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetSkim_Corrected.root ${OUTDIR}/PbPb_MC_gammaJetSkim_Corrected.root 1 &> PbPb_Data_purity.txt &

./Performance/printPurity.exe CutConfigurations/gammaJet_mc.conf ${OUTDIR}/PbPb_MC_gammaJetSkim_Corrected.root ${OUTDIR}/PbPb_MC_gammaJetSkim_Corrected.root 1 &> PbPb_MC_purity.txt &

./Performance/printPurity.exe CutConfigurations/gammaJet_pp.conf ${OUTDIR}/pp_Data_gammaJetSkim_Corrected.root ${OUTDIR}/pp_MC_gammaJetSkim_Corrected.root 1 &> pp_Data_purity.txt &

./Performance/printPurity.exe CutConfigurations/gammaJet_pp_mc.conf ${OUTDIR}/pp_MC_gammaJetSkim_Corrected.root ${OUTDIR}/pp_MC_gammaJetSkim_Corrected.root 1 &> pp_MC_purity.txt &

wait

# # # # ########### PARALLEL HISTOGRAMMING SECTION ##########
# echo "## Histogramming Condor Jobs"
# H_INDIR=/mnt/hadoop/cms/store/user/luck/GAMMAJETFILES/${DATE}/
# mkdir -p ${H_INDIR}
# cp ${OUTDIR}/*_Corrected.root ${H_INDIR}

# ./ShellScripts/gammaJetHistogram_condor.sh ./CutConfigurations/gammaJet.conf ${H_INDIR}/PbPb_Data_gammaJetSkim_Corrected.root ${H_INDIR}/PbPb_Data_gammaJetHistogram_unmerged/ 20

# ./ShellScripts/gammaJetHistogram_condor.sh ./CutConfigurations/gammaJet_mc.conf ${H_INDIR}/PbPb_MC_gammaJetSkim_Corrected.root ${H_INDIR}/PbPb_MC_gammaJetHistogram_unmerged/ 4

# ./ShellScripts/gammaJetHistogram_condor.sh ./CutConfigurations/gammaJet_pp.conf ${H_INDIR}/pp_Data_gammaJetSkim_Corrected.root ${H_INDIR}/pp_Data_gammaJetHistogram_unmerged/ 244

# ./ShellScripts/gammaJetHistogram_condor.sh ./CutConfigurations/gammaJet_pp_mc.conf ${H_INDIR}/pp_MC_gammaJetSkim_Corrected.root ${H_INDIR}/pp_MC_gammaJetHistogram_unmerged/ 160

############ HADD SECTION ###################
# echo "## Summing Histograms"
# hadd -f ${OUTDIR}/PbPb_Data_gammaJetHistogram.root ${H_INDIR}/PbPb_Data_gammaJetHistogram_unmerged/*.root
# #rm -r ${H_INDIR}/PbPb_Data_gammaJetHistogram_unmerged

# hadd -f ${OUTDIR}/PbPb_MC_gammaJetHistogram.root ${H_INDIR}/PbPb_MC_gammaJetHistogram_unmerged/*.root
# #rm -r ${H_INDIR}/PbPb_MC_gammaJetHistogram_unmerged

# hadd -f ${OUTDIR}/pp_Data_gammaJetHistogram.root ${H_INDIR}/pp_Data_gammaJetHistogram_unmerged/*.root
# #rm -r ${H_INDIR}/pp_Data_gammaJetHistogram_unmerged

# hadd -f ${OUTDIR}/pp_MC_gammaJetHistogram.root ${H_INDIR}/pp_MC_gammaJetHistogram_unmerged/*.root
# #rm -r ${H_INDIR}/pp_MC_gammaJetHistogram_unmerged

######## Serial Histogramming Section ############## Somehow we got faster!?!!?
echo "## Histogramming"
time ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetSkim_Corrected.root ${OUTDIR}/PbPb_Data_gammaJetHistogram.root > ${OUTDIR}/PbPb_Data_gammaJetHistogram.root.log &

time ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet_mc.conf ${OUTDIR}/PbPb_MC_gammaJetSkim_Corrected.root ${OUTDIR}/PbPb_MC_gammaJetHistogram.root > ${OUTDIR}/PbPb_MC_gammaJetHistogram.root.log &

time ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet_pp.conf ${OUTDIR}/pp_Data_gammaJetSkim_Corrected.root ${OUTDIR}/pp_Data_gammaJetHistogram.root > ${OUTDIR}/pp_Data_gammaJetHistogram.root.log &

time ./Histogramming/gammaJetHistogram.exe ./CutConfigurations/gammaJet_pp_mc.conf ${OUTDIR}/pp_MC_gammaJetSkim_Corrected.root ${OUTDIR}/pp_MC_gammaJetHistogram.root > ${OUTDIR}/pp_MC_gammaJetHistogram.root.log &

wait

######## Histogram Arithmetic Section ##############
echo "## Arithmetic"
time ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogram.root ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root > ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root.log &

time ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet_mc.conf ${OUTDIR}/PbPb_MC_gammaJetHistogram.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root > ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root.log &

time ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet_pp.conf ${OUTDIR}/pp_Data_gammaJetHistogram.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root > ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root.log &

time ./Histogramming/gammaJetHistogramArithmetic.exe ./CutConfigurations/gammaJet_pp_mc.conf ${OUTDIR}/pp_MC_gammaJetHistogram.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root > ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root.log &

wait

######## IAA Section ##############
time ./Histogramming/gammaJetFinalHistograms.exe ./CutConfigurations/gammaJet.conf ${OUTDIR}/PbPb_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/PbPb_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/pp_MC_gammaJetHistogramArithmetic.root ${OUTDIR}/gammaJetHistograms.root > ${OUTDIR}/gammaJetHistograms.root.log &

wait
