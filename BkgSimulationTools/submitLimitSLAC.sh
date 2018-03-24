#!/bin/bash
#DataFile=0
DataFile='final_Run4_WS_unsalted_dwall3cm.root'
mWimp=$1
fileNum=$2
nTrials=$3
useXSec=$4
useNuisParam=$5
nPoints=$6
queue=$7
walltime=$8
#echo bsub -q $queue -W $walltime -o /dev/null "./runMass $DataFile $mWimp $fileNum $nTrials $useXSec $useNuisParam $nPoints"
#bsub -q $queue -W $walltime -o /dev/null "./runMass $DataFile $mWimp $fileNum $nTrials $useXSec $useNuisParam $nPoints"


echo bsub -q $queue -W $walltime -o /dev/null "./SIRun4 $mWimp $fileNum $nTrials $DataFile $useNuisParam $useXSec $nPoints 0 0"
bsub -q $queue -W $walltime -o /dev/null "./SIRun4 $mWimp $fileNum $nTrials $DataFile $useNuisParam $useXSec $nPoints 0 0"
