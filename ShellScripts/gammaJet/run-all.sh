#!/bin/bash

if [[ $# -ne 3 ]]; then
    echo -e "Usage: ./ShellScripts/gammaJet/run-all.sh [input dir] [output dir] [0/1/2]"
    echo -e "arg 3: [0: histogram, 1: arithmetic, 2: final]"
    echo -e "[input dir] should contain input files"
    echo -e "[output dir] will contain output files up to final output"
    exit 1
fi

INDIR=$1
OUTDIR=$2
mkdir -p $OUTDIR

./ShellScripts/gammaJet/run-nominal.sh $@

ISODIR=$([[ $3 -eq 2 ]] && echo "$INDIR" || echo "$OUTDIR")
cp ${ISODIR}/*_Data_gammaJetHistogramArithmetic.root ${OUTDIR}/

./ShellScripts/gammaJet/run-systematics.sh $@ 1

# write systematics config file
SYSLIST=${OUTDIR}/gammaJetSystematics.list
echo -e "# nominal" > $SYSLIST
echo -e "${OUTDIR}/gammaJetHistograms.root" >> $SYSLIST
echo -e "# systematics" >> $SYSLIST
echo -e "${OUTDIR}/gammaJetHistograms_JES_UP.root" >> $SYSLIST
echo -e "${OUTDIR}/gammaJetHistograms_JES_DOWN.root" >> $SYSLIST
echo -e "${OUTDIR}/gammaJetHistograms_PURITY_UP.root" >> $SYSLIST
echo -e "${OUTDIR}/gammaJetHistograms_PURITY_DOWN.root" >> $SYSLIST
echo -e "${OUTDIR}/gammaJetHistograms_JER.root" >> $SYSLIST
echo -e "${OUTDIR}/gammaJetHistograms_ELE_REJ.root" >> $SYSLIST
echo -e "${OUTDIR}/gammaJetHistograms_PES.root" >> $SYSLIST
echo -e "${OUTDIR}/gammaJetHistograms_ISO.root" >> $SYSLIST

./Systematics/gammaJet/gammaJetSystematics.exe CutConfigurations/gammaJet.conf ${OUTDIR}/gammaJetSystematics.list ${OUTDIR}/gammaJetSystematics.root > ${OUTDIR}/gammaJetSystematics.log
