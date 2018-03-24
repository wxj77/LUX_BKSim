#!/bin/bash

#This loops through once for each set of 250000 events to simulate. 
#For each one it starts a different instance in the cluster of LSF_MS1R.sh.
#This script is written to use LSF for queuing. 
#Job name is [Source][Isotope][Instance].
#Error and output to /dev/null.
#Run on one node with a walltime of 4:30 hours.
#Queued in the "shared" queue.

path=$BKSimOutput
dirpath=$path/BackgroundSim/
#echo $1 $2 $3
COUNTER=1000000
CI=$(expr "$COUNTER" + "$1")
cd $dirpath
pwd
i=0
while [  $COUNTER -lt $CI ]; do
#        sbatch "LSF_MS1R.sh" $COUNTER $2 $3 -q shared -t 05:00:00
        bsub -J $2$3$COUNTER -o /dev/null -W 22:30 -q long "sh LSF_MS1R.sh $COUNTER $2 $3"
#        bsub -J $2$3$COUNTER -e ~/err.err -o ~/log.log -n 1 -W 04:30 -q shared "sh LSF_MS1R.sh $COUNTER $2 $3"
#	bsub -J $2$3$COUNTER -e /dev/null -o /dev/null -n 1 -W 02:00 -q shared "sh LSF_MS1R.sh $COUNTER $2 $3"
	let COUNTER=COUNTER+1
done
