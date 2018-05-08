#!/bin/bash

#This script is the "master list" for setting the numbers of events to simulate for each isotope in each detector component.
#Each line has the number of sets of 250000 events to simulate, followed by the detector component and the isotope/chain.

#Values for each set correspond to more than 10X Run04 Live Time (min of 340 live days)
path=$BKSimOutput
#cd $path/Run4Background/Tools_100X_100keVee
cd $path/BackgroundSim/ 

source MS1Loop_R.sh 12 FIELDGRID Th232E
source MS1Loop_R.sh 32 FIELDGRID Th232L
source MS1Loop_R.sh 84 FIELDRING Th232E
source MS1Loop_R.sh 224 FIELDRING Th232L

source MS1Loop_R.sh 8 FIELDGRID Co60
source MS1Loop_R.sh 12 FIELDRING Co60

source MS1Loop_R.sh 72 FIELDGRID Ra226
source MS1Loop_R.sh 180 FIELDRING Ra226

source MS1Loop_R.sh 4 FIELDGRID K40


source  MS1Loop_R.sh 720 FCSHIELDPLANE Co60
source MS1Loop_R.sh 3000 FCSHIELD Th232E
source MS1Loop_R.sh 8000 FCSHIELD Th232L

