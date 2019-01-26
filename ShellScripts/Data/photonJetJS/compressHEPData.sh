#!/bin/bash

baseDir="./Data/photonJetJS/hepdata"

inputFiles="./*.yaml"
outputFile="./hepdata-test.zip"

cd $baseDir
rm -rf $outputFile
zip $outputFile $inputFiles

