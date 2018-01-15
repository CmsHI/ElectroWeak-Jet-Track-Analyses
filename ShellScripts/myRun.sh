#!/bin/bash
############################################################
## print the command
echo "command called =" $*
## start time of the script
date1=$(date +"%s")         # %s     seconds since 1970-01-01 00:00:00 UTC
#date1Human=$(date -d @$date1)
date1Human=$(date -d @$date1 '+%d/%m/%Y %H:%M:%S')
echo "started on     =" $date1Human
echo "############################################################"

# run the whole argument as if it is a bash command or bash script
# So, it is not making something different or new. But it gives extra information about the command run before and after the run of the command.
$*

echo "############################################################"
echo "##### content of the SCRIPT =" $1
# check if it really is a bash script (check if it ends with ".sh")
# https://viewsby.wordpress.com/2013/09/06/bash-string-ends-with/
# http://tldp.org/LDP/abs/html/string-manipulation.html
prefix="./";
if [[ $1 == *.sh ]]
then
    # remove "./" from "./myScript.sh" so that "myScript.sh" is printed.
    cat ${1#$prefix}
fi
echo "############################################################"
echo "##### log of the SCRIPT =" $1      # the second argument is actually the command that is run.

## finish time and run time of the script
date2=$(date +"%s")         # %s     seconds since 1970-01-01 00:00:00 UTC
#date2Human=$(date -d @$date2)
date2Human=$(date -d @$date2 '+%d/%m/%Y %H:%M:%S')
diff=$(($date2-$date1))
diffHuman=$(date -d @$diff)
#echo $date1
echo "started  on    =" $date1Human
echo "finished on    =" $date2Human
#echo "$(($diff / 60)) minutes and $(($diff % 60)) seconds elapsed."
echo "$(($diff / 3600)) hours, $((($diff / 60) % 60)) minutes and $(($diff % 60)) seconds elapsed."
echo $diffHuman

echo "##### END #####"
