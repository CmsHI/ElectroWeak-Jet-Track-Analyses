#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: ./ShellScripts/Condor/convert2CondorSubmit.sh <inputScript> [<outputScript>]"
  echo "<inputScript>  = script that runs code interactively"
  echo "<outputScript> = path for the output script that will create condor jobs for the same code"
  echo "Example : ./ShellScripts/Condor/convert2CondorSubmit.sh ShellScripts/Performance/photons/runPhotonRecoAna.eScaleRawE.AllQCD.sh"
  echo "This will create ShellScripts/Performance/photons/runPhotonRecoAna.eScaleRawE.AllQCD.CONDOR.sh"
  exit 1
fi

inputScript=$1
outputScript=$2
if [ ! -f $inputScript ]; then
  echo "File not found : $inputScript"
  exit 1
fi
if [ -z "$outputScript" ]; then
  echo "No path for output script is given. Setting the output path the same as input path with \"CONDOR\" as suffix"
  outputScript="${inputScript/.sh/.CONDOR.sh}"
fi

if [ -f $outputScript ]; then
   echo "Script $outputScript exists already. Exiting."
   exit 1
fi

cp $inputScript $outputScript

lineToReplace="outDirBase=\$EWJTAOUT"
newLine="outDirBase=\$EWJTAOUTGRID"
sed -i "s/$lineToReplace/$newLine/g" $outputScript

# need to put "\" in front of each special characters and delimiters such as $ ' " ( ) { } [ ] 
lineToReplace="if \[ -z \"\$outDirBase\" \]; then"
newLine1="  USERGRID=\$(voms-proxy-info -issuer \| awk \'\{split\(\$0,myLine,\"\/CN=\"\); print myLine\[2\]\}\'\)"
newLine2="  if \[ -z \"\$USERGRID\" \]; then"
newLine3="    USERGRID=\$USER"
newLine4="  fi"
newLine=$lineToReplace"\n"$newLine1"\n"$newLine2"\n"$newLine3"\n"$newLine4
sed -i "s/$lineToReplace/$newLine/g" $outputScript

lineToReplace="outDirBase=\"\/export\/d00\/scratch\/\"\$USER\"\/EWJTA-out\""
newLine="outDirBase=\"\/mnt\/hadoop\/cms\/store\/user\/\"\$USERGRID\"\/EWJTA-out\""
sed -i "s/$lineToReplace/$newLine/g" $outputScript

lineToReplace="outputFile=\${outList\[i1\]}"
newLine1="outputFileTmp=\${outList\[i1\]}"
newLine2="    outputFile=\"\$\{outputFileTmp\/.root\/\/DUMMY.root}\""
#newLine2="    outputFile=\"\$\{outputFileTmp\/.root\/\\\\\/DUMMY.root}\""  # need to type "\\\\" to insert "\"
newLine=$newLine1"\n"$newLine2
sed -i "s/$lineToReplace/$newLine/g" $outputScript

lineToReplace="outputFileLOG="
newLine="#outputFileLOG="
sed -i "s/$lineToReplace/$newLine/g" $outputScript

lineToReplace="mkdir -p \$outDir"
newLine="#mkdir -p \$outDir"
sed -i "s/$lineToReplace/$newLine/g" $outputScript

lineToReplace="\$progPath \$configFile \$inputFile \$outputFile &> \$outputFileLOG &"
newLine=".\/ShellScripts\/Condor\/condorSubmit.sh \$progPath \$configFile \$inputFile \$outDir 1 0"
sed -i "s/$lineToReplace/$newLine/g" $outputScript

echo "Script has been created at $outputScript"
echo "run the script via :"
outputScriptLOG="${outputScript/.sh/.log}"
echo "$outputScript &> $EWJTAOUT/$outputScriptLOG &"

