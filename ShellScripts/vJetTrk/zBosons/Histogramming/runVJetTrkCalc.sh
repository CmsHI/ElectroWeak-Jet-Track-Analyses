#!/bin/bash

source ./ShellScripts/bashUtils.sh
source ./ShellScripts/vJetTrk/zBosons/bashUtils.sh

runCmd="./ShellScripts/myRun.sh"
progPath="./Histogramming/vJetTrk/vJetTrkCalc.exe"
progCode="${progPath/.exe/.C}"
#g++ $progCode $(root-config --cflags --libs) -Werror -Wall -Wextra -Wno-narrowing -lTMVA -lRooFitCore -lRooFit -O2 -o $progPath || exit 1

### USER INPUT START
doSCALEBINW=1
doBKGSUB=1
doNORMV=1
doMERGE=0
vRG="r" # Options are "r" (reco) or "g" (gen)
trkRG="r" # Options are "r" (reco) or "g" (gen)
trkRBS="raw" # Options are "raw" (v+jet event), "bkg" (mix event) or "sig" (sube==0 particles from v+jet event)
dirSpecial=""  # name of the directory where special/non-nominal output (if any) would be written
sampleFlags=(
## pp 2017
0 # pp_2017_mc_zmm
0 # pp_2017_mc_zee
0 # pp_2017_data_zmm
0 # pp_2017_data_zee
0 # pp_2017_mc_zll
0 # pp_2017_data_zll
## pbpb 2018
1 # pbpb_2018_mc_zmm
0 # pbpb_2018_mc_zee
0 # pbpb_2018_data_zmm
0 # pbpb_2018_data_zee
0 # pbpb_2018_mc_zll
0 # pbpb_2018_data_zll
## pp 2015
0 # pp_2015_mc_zmm
0 # pp_2015_mc_zee
0 # pp_2015_data_zmm
0 # pp_2015_data_zee
0 # pp_2015_mc_zll
0 # pp_2015_data_zll
## pbpb 2015
0 # pbpb_2015_mc_zmm
0 # pbpb_2015_mc_zee
0 # pbpb_2015_data_zmm
0 # pbpb_2015_data_zee
0 # pbpb_2015_mc_zll
0 # pbpb_2015_data_zll
);

labels_obs=(
dphi
dphi_rebin
deta
trkPt
trkPt_rebin
trkEta
zh
zh_T
xivh
dphi_vs_trkPt
dphi_vs_deta
dphi_vs_trkEta
trkPhi_vs_trkEta
);

labels_vPt=(
10_20
20_40
20_0
30_0
40_0
);

labels_trkPt=(
1_0
1_2
2_3
3_4
4_8
1_20
);

labels_cent=(
0_100
0_30
30_100
0_10
10_30
30_50
50_90
50_100
);
### USER INPUT END

inputObjList="vJetTrkCalc_inputObjs.list"
rm -rf ${inputObjList}

## create list of input objects
for lbl_obs in "${labels_obs[@]}"
do
  for lbl_vPt in "${labels_vPt[@]}"
  do
    for lbl_trkPt in "${labels_trkPt[@]}"
    do
      for lbl_cent in "${labels_cent[@]}"
      do

        hPath="h_"${lbl_obs}"_vPt"${lbl_vPt}"_trkPt"${lbl_trkPt}"_cent"${lbl_cent}
        ## Ex. hPath = h_dphi_vPt20_40_trkPt1_2_cent0_30
        if [[ ${lbl_obs} == trkPt || ${lbl_obs} == trkPt_rebin ]]; then
          hPath="h_"${lbl_obs}"_vPt"${lbl_vPt}"_cent"${lbl_cent}
        elif [[ ${lbl_obs} == *_vs_* ]]; then
          hPath="h2_"${lbl_obs}"_vPt"${lbl_vPt}"_trkPt"${lbl_trkPt}"_cent"${lbl_cent}
          if [[ ${lbl_obs} == *trkPt* ]]; then
            hPath="h2_"${lbl_obs}"_vPt"${lbl_vPt}"_cent"${lbl_cent}
          fi
        fi

        if [ ${doBKGSUB} == 1 ]; then

          echo ${hPath} >> ${inputObjList}   ## add raw obj
          echo ${hPath} >> ${inputObjList}   ## add bkg obj

          if [[ ${hPath} == h2_* ]]; then
            ## calc 1D projections of TH2D
            echo ${hPath}"_projX" >> ${inputObjList}   ## add raw obj
            echo ${hPath}"_projX" >> ${inputObjList}   ## add bkg obj

            echo ${hPath}"_projY" >> ${inputObjList}   ## add raw obj
            echo ${hPath}"_projY" >> ${inputObjList}   ## add bkg obj
          fi
        elif [ ${doNORMV} == 1 ]; then

          echo ${hPath} >> ${inputObjList}

          if [[ ${hPath} == h2_* ]]; then
            ## calc 1D projections of TH2D
            echo ${hPath}"_projX" >> ${inputObjList}
            echo ${hPath}"_projY" >> ${inputObjList}
          fi
        fi

      done
    done
  done
done


outputDirBase=$EWJTAOUT
if [ -z "$outputDirBase" ]; then
  outputDirBase="/export/d00/scratch/"$USER"/EWJTA-out"
fi

arrayIndices=${!sampleFlags[*]}
for i1 in $arrayIndices
do
  if [ ${sampleFlags[i1]} == 1 ]; then
 
    operation=""
    if [[ ${doSCALEBINW} == 1 ]]; then
      operation=${operation}",SCALEBINW"
    fi
    if [[ ${doBKGSUB} == 1 ]]; then
      operation=${operation}",BKGSUB"
    fi
    if [[ ${doNORMV} == 1 ]]; then
      operation=${operation}",NORMV"
    fi

    if [[ ${doMERGE} == 1 ]]; then
      operation="MERGE"
      # MERGE cannot be combined with other operations
      doSCALEBINW=0
      doBKGSUB=0
      doNORMV=0
    fi

    inputFileList="vJetTrkCalc_inputFiles.list"
    rm -rf ${inputFileList}

    inputSuffix=${anaSuffixList[i1]}
    outputSuffix=${calcSuffixList[i1]}
    outputFile=""
    if [ ${doBKGSUB} == 1 ]; then

#      inputFile_trkRAW=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix}"_trk_"${trkRG}"_raw.root"
#      inputFile_trkBKG=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix}"_trk_"${trkRG}"_bkg.root"
      inputFile_trkRAW=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix}"_v"${vRG}"_trk_"${trkRG}"_raw.root"
      inputFile_trkBKG=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix}"_v"${vRG}"_trk_"${trkRG}"_bkg.root"
      echo ${inputFile_trkRAW} >> ${inputFileList}
      echo ${inputFile_trkBKG} >> ${inputFileList}

#      outputFile=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${outputSuffix}"_trk_"${trkRG}"_bkgsub.root"
      outputFile=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${outputSuffix}"_v"${vRG}"_trk_"${trkRG}"_bkgsub.root"
    elif [ ${doNORMV} == 1 ]; then

#      inputFile=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix}"_trk_"${trkRG}"_"${trkRBS}".root"
      inputFile=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix}"_v"${vRG}"_trk_"${trkRG}"_"${trkRBS}".root"
      echo ${inputFile} >> ${inputFileList}

#      outputFile=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${outputSuffix}"_trk_"${trkRG}"_"${trkRBS}".root"
      outputFile=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${outputSuffix}"_v"${vRG}"_trk_"${trkRG}"_"${trkRBS}".root"
    elif [ ${doMERGE} == 1 ]; then

      outputSuffix=${mergeSuffixList[i1]}
      rm -rf ${inputObjList}
      touch ${inputObjList}

      inputSuffix_zmm="${inputSuffix/vJetTrkCalc_/vJetTrkAna_}"
      inputSuffix_zmm="${inputSuffix_zmm/_zll_merge/_zmm}"
#      inputFile_zmm=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix_zmm}"_trk_"${trkRG}"_"${trkRBS}".root"
      inputFile_zmm=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix_zmm}"_v"${vRG}"_trk_"${trkRG}"_"${trkRBS}".root"
      inputSuffix_zee="${inputSuffix_zmm/_zmm/_zee}"
#      inputFile_zee=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix_zee}"_trk_"${trkRG}"_"${trkRBS}".root"
      inputFile_zee=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${inputSuffix_zee}"_v"${vRG}"_trk_"${trkRG}"_"${trkRBS}".root"
      echo ${inputFile_zmm} >> ${inputFileList}
      echo ${inputFile_zee} >> ${inputFileList}

#      outputFile=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${outputSuffix}"_trk_"${trkRG}"_"${trkRBS}".root"
      outputFile=${outputDirBase}"/"${relDirHist}"/"${dirSpecial}"/"${outputSuffix}"_v"${vRG}"_trk_"${trkRG}"_"${trkRBS}".root"
    fi

    writeMode="RECREATE"

    outputFileLOG="${outputFile/.root/.log}"
    outDir=$(dirname "${outputFile}")
    mkdir -p $outDir

    $runCmd $progPath ${inputFileList} ${inputObjList} $outputFile ${writeMode} ${operation} &> $outputFileLOG
    echo "$runCmd $progPath ${inputFileList} ${inputObjList} $outputFile ${writeMode} ${operation} &> $outputFileLOG"
  fi
done

rm -rf ${inputObjList}
