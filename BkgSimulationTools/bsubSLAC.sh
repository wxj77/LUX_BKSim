#!/bin/bash

: '
#QUEUE: short
#  -- Maximum 21 minutes CPU time [SLAC units].  This is one of the default queues.
fileNumStart=101
fileNumEnd=103
useXSec=1
nTrials=50
useNuisParam=0
nPoints=10
queue='short'
walltime='00:20'
'

#: '
#QUEUE: medium
#  -- Maximum 2.8 hours CPU time [SLAC units].  This is one of the default queues.
fileNumStart=101
fileNumEnd=103
useXSec=1
nTrials=10
useNuisParam=0
nPoints=10
queue='medium'
walltime='02:30'
#'

: '
#QUEUE: long
#  -- Maximum 22.3 hours CPU time [SLAC units]  This is one of the default queues.
fileNumStart=11
fileNumEnd=100
useXSec=1
nTrials=50
useNuisParam=1
nPoints=9
queue='long'
walltime='22:00'
'

for m in 7 10 12 14 17 21 33 50 100 4000 
do
   for i in `seq $fileNumStart $fileNumEnd`
   do
	   ./submitLimitSLAC.sh $m $i $nTrials $useXSec $useNuisParam $nPoints $queue $walltime
   done
done


