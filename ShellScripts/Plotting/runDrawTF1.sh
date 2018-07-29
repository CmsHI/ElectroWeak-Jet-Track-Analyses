#!/bin/bash

progPath="./Plotting/drawTF1.exe"
progCode="${progPath/.exe/.C}"
g++ $progCode $(root-config --cflags --libs) -Werror -Wall -O2 -o $progPath || exit 1

runCmd="./ShellScripts/myRun.sh"

PT0=30
formulas=(
"gaus"
"[0]*TMath::Power(60/x,[1])"
"[0]*TMath::Power("$PT0"/x,[1]+[2]*TMath::Log(x/"$PT0"))"
);

# parameter values are separated by ';'
parValuesList=(
"100;1;0.2"
"0.0593128;4.51425"
"0.0100805;2.94486;0.514389"
);

xMins=(
"0"
"30"
"30"
);

xMaxs=(
"2"
"130"
"200"
);

prefix="drawTF1"
outList=(
$prefix"_gaus.root"
$prefix"_ptSpectrum1.root"
$prefix"_ptSpectrum2.root"
);

arrayIndices=${!formulas[*]}
for i1 in $arrayIndices
do
    formula=${formulas[i1]}
    parValues=${parValuesList[i1]}
    xMin=${xMins[i1]}
    xMax=${xMaxs[i1]}
    outputFile=${outList[i1]}
    outputFileLOG="${outputFile/.root/.log}"
    pictureFileName="cnv_${outputFile/.root/}"
    $runCmd $progPath $formula $parValues $xMin $xMax $outputFile $pictureFileName &> $outputFileLOG
    echo "$runCmd $progPath $formula $parValues $xMin $xMax $outputFile $pictureFileName &> $outputFileLOG"
done

