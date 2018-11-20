#!/bin/bash

baseDir="./Data/photonJetFF/hepdata"

inputFiles="./*.yaml"
outputFile="./hepdata-test.zip"

cd $baseDir
#rm -rf $outputFile
zip $outputFile $inputFiles

