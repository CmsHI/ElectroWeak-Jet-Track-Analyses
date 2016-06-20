#!/bin/bash
# collection of bash utilities

## global variables
IFS=$'\n' # very important. force to use only '\n' to split the elements of the array
## global variables - END

## function to run a program with an "on the fly generated" configuration.
## $1 : path to the program to be run
## $2 : configuration to be imported and modified
## $3 : string that contains the cut    configuration to be applied after importing $2
## $4 : string that contains the import configuration to be applied after importing $2
## $5 : path to the input  file of program
## $6 : path to the output file of program
## $7 : path to the log file of program (optional)
function runGeneratedConf() {

    progPath=$1
    importedConfig=$2
    cutConfig=$3
    inputConfig=$4
    inputFile=$5
    outputFile=$6
    logFile=$7

    ## verbose ##
    echo "imported configuration : $importedConfig" 
    echo "cut configuration      : $cutConfig"
    echo "input configuration    : $inputConfig"
    ## verbose - END ##
    # write the temporary configuration file
    temporaryConfig=$importedConfig".temp"
    printf "##### Cut Configuration #####\n" > $temporaryConfig
    printf "import.cut = %s\n" "$importedConfig" >> $temporaryConfig
    # write the cut configuration
    printf "%b\n" "$cutConfig" >> $temporaryConfig     # be careful of possible replacements of backslash characters, Ex. might need to use "\\\eta" instead of "\eta"
    printf "#CUTS-END#\n" >> $temporaryConfig
    printf "##### input Configuration #####\n" >> $temporaryConfig
    printf "import.input = %s\n" "$importedConfig" >> $temporaryConfig
    # write the input configuration
    printf "%b\n" "$inputConfig" >> $temporaryConfig     # be careful of possible replacements of backslash characters, Ex. might need to use "\\\eta" instead of "\eta"
    printf "#INPUT-END#\n" >> $temporaryConfig
    
    if (( ${#logFile} > 0 )); then
      echo "$progPath $temporaryConfig $inputFile $outputFile &> $logFile &"
      $progPath $temporaryConfig $inputFile $outputFile &> $logFile &
    else
      echo "$progPath $temporaryConfig $inputFile $outputFile &"
      $progPath $temporaryConfig $inputFile $outputFile &
    fi

    # remove the config file
    sleep 1.0	# !! do not delete the temp config immediately. make sure that the file is removed after the job reads it.
    rm $temporaryConfig
}


