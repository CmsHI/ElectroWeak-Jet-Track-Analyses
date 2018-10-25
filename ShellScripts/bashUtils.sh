#!/bin/bash
# collection of bash utilities

## global variables
IFS=$'\n' # very important. force to use only '\n' to split the elements of the array
## global variables - END

## recursively find all the configs imported from the given file, write the file paths to given list file
## $1 : config file for which all imported configs is to be found
## $2 : file where the paths to found configs is to be written. The entries are not necessarily unique, but this is not a concern, what matters is all the imported ones are found.
function findImportedConfigs() {

local configFile=$1
local importedList=$2

echo "findImportedConfigs() ${configFile} ${importedList}"

local lines1=$(grep "import" ${configFile})
#IFS=$'\n'
local lines=($lines1) # list of lines with "import" statement

local arrayIndices=${!lines[*]}
for i1 in $arrayIndices
do
  local line=${lines[i1]}

  local importedConf=${line}
  local importedConf="${importedConf//import.cut =/}"  # remove "import.cut =" if any
  local importedConf="${importedConf//import.cut=/}"  # remove "import.cut=" if any
  local importedConf="${importedConf//import.input =/}"  # remove "import.input =" if any
  local importedConf="${importedConf//import.input=/}"  # remove "import.input=" if any
  local importedConf="${importedConf// /}"  # remove " " if any
  #echo "line $i1 is $line"
  #echo "importedConf $i1 is$importedConf"
  echo "found imported config : $importedConf"
  if [ ! -z "$importedConf" ]; then
    echo ${importedConf} >> $importedList
    findImportedConfigs $importedConf $importedList
  fi
done
}

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
    echo "runGeneratedConf()"
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
      $progPath $temporaryConfig $inputFile $outputFile &> $logFile & #      ./ShellScripts/myRun.sh
      ## possibility to run as a ROOT macro
      ## progPath2="${progPath/.exe/.C}"
      ## root -l ''$progPath2'("'$temporaryConfig'", "'$inputFile'", "'$outputFile'")' &> $logFile &
    else
      echo "$progPath $temporaryConfig $inputFile $outputFile &"
      $progPath $temporaryConfig $inputFile $outputFile & # ./ShellScripts/myRun.sh
      ## possibility to run as a ROOT macro
      ## progPath2="${progPath/.exe/.C}"
      ## root -l ''$progPath2'("'$temporaryConfig'", "'$inputFile'", "'$outputFile'")'
    fi

    # remove the config file
    sleep 1.0	# !! do not delete the temp config immediately. make sure that the file is removed after the job reads it.
    rm $temporaryConfig

    echo "runGeneratedConf() - END"
}


## function to run a program with an "on the fly generated" configuration.
## this function is the same "runGeneratedConf()", but with one additional argument.
## $1 : path to the program to be run
## $2 : configuration to be imported and modified
## $3 : string that contains the cut    configuration to be applied after importing $2
## $4 : string that contains the import configuration to be applied after importing $2
## $5 : path to the input  file of program
## $6 : path to the output file of program
## $7 : path to the additional argument
## $8 : path to the log file of program (optional)
function runGeneratedConf2() {

    progPath=$1
    importedConfig=$2
    cutConfig=$3
    inputConfig=$4
    inputFile=$5
    outputFile=$6
    ioFile2=$7
    logFile=$8

    ## verbose ##
    echo "runGeneratedConf2()"
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
      echo "$progPath $temporaryConfig $inputFile $outputFile $ioFile2 &> $logFile &"
      $progPath $temporaryConfig $inputFile $outputFile $ioFile2 &> $logFile & #      ./ShellScripts/myRun.sh
      ## possibility to run as a ROOT macro
      ## progPath2="${progPath/.exe/.C}"
      ## root -l ''$progPath2'("'$temporaryConfig'", "'$inputFile'", "'$outputFile'", "'$ioFile2'")' &> $logFile &
    else
      echo "$progPath $temporaryConfig $inputFile $outputFile $ioFile2 &"
      $progPath $temporaryConfig $inputFile $outputFile $ioFile2 & # ./ShellScripts/myRun.sh
      ## possibility to run as a ROOT macro
      ## progPath2="${progPath/.exe/.C}"
      ## root -l ''$progPath2'("'$temporaryConfig'", "'$inputFile'", "'$outputFile'", "'$ioFile2'")'
    fi

    # remove the config file
    sleep 1.0	# !! do not delete the temp config immediately. make sure that the file is removed after the job reads it.
    rm $temporaryConfig

    echo "runGeneratedConf2() - END"
}

