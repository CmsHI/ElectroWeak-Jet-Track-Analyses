#!/bin/bash
# script to create a plotHistogram.C configuration file at the given path, creates necessary directories

if [ $# -ne 1 ]; then
  echo "Usage: ./ShellScripts/Plotting/copyPlotHistogram.sh <PathtoConfigFile>"
  echo "Example: ./ShellScripts/Plotting/copyPlotHistogram.sh ./plotsDir/plot.pt.conf"
  exit 1
fi

output=$1
# extract the directory
outputDir=$(dirname "${output}")"/"
outputBase=$(basename "${output}")
     
if [ -f $output ]; then
   echo "File $output exists already. Exiting."
   exit 1
fi

# (re)create the output directory
mkdir -p $outputDir
cp Configurations/plotHistogram.template.conf $output

# put a generic command to run the configuration
outputGraphics="${output/.conf/.png}"
outputLog="${output/.conf/.log}"
line="## ./Plotting/plotHistogram.exe $output $output $outputGraphics &> $outputLog &"
## delete the 1st and 2nd lines remaining from the template file
sed -i "2d" $output
sed -i "1d" $output
## insert the sample run command at the beginning of the file
sed -i "1i $line" $output

echo "File $outputBase has been placed into $outputDir"
echo "output = $output"


