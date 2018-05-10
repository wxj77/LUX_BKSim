#!/bin/bash

#This script is the "master list" for setting the numbers of events to simulate for each isotope in each detector component.
#Each line has the number of sets of 250000 events to simulate, followed by the detector component and the isotope/chain.

#Values for each set correspond to more than 10X Run04 Live Time (min of 340 live days)
path=$BKSimOutput
#cd $path/Run4Background/Tools_100X_100keVee
cd $path/BackgroundSim/

source  MS1Loop_R.sh 48200 PMTT K40
source  MS1Loop_R.sh 1840 PMTT Co60
source MS1Loop_R.sh 70400 PMTT Ra226
source MS1Loop_R.sh 6120 PMTT Th232E
source MS1Loop_R.sh 16320 PMTT Th232L
source MS1Loop_R.sh 48200 PMTB K40
source MS1Loop_R.sh 1840 PMTB Co60
source MS1Loop_R.sh 70400 PMTB Ra226
source MS1Loop_R.sh 6120 PMTB Th232E
source MS1Loop_R.sh 16320 PMTB Th232L
