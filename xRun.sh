#!/bin/bash

# run ./350A2.sh (after reading that one's comments) instead

# IGNORE BELOW

# example command given 350 assignment 2
# ./xRun.sh 3 ./simul 700 700 1 0 10 1 1 1 1 >> output.txt

# make sure output.txt is different for every run of this script
# as >> appends to end of file.

# DO NOT DO THIS IN THE SAME DIRECTORY AS .C AND .H FILES
# DO NOT DO THIS IN THE SAME DIRECTORY AS .C AND .H FILES
# DO NOT DO THIS IN THE SAME DIRECTORY AS .C AND .H FILES
# DO NOT DO THIS IN THE SAME DIRECTORY AS .C AND .H FILES
# nothing really happens unless you set the output file as one of
# the .C or .H .... but still

# MAKE A NEW DIRECTORY, COPY THIS SCRIPT AND SIMUL EXECUTABLE OVER

echo "Script xRun"
echo "This script runs a command n times. First argument = n"

n=$1 # this is your first command line argument

echo "Warning, this script does not check if a file already exists"
echo "Please do not overwrite your own files, use at your own risk"

#read -n1 -p  "Press the spacebar to continue running script" key
#echo ''

#if [ "$key" = '' ]; then
#  echo "Continued "
#else
#  exit
#fi

# parallel? concurrent? idk the word but yea.... 
for (( i=0; i<$n; ++i)); 
do
  ${@:2} & # does command n times
done 

