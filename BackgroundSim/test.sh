#!/bin/bash

#This script is the "master list" for setting the numbers of events to simulate for each isotope in each detector component.
#Each line has the number of sets of 250000 events to simulate, followed by the detector component and the isotope/chain.

#Values for each set correspond to more than 10X Run04 Live Time (min of 340 live days)
path=$BKSimOutput
#cd $path/Run4Background/Tools_100X_100keVee
cd $path/BackgroundSim/ 

source  MS1Loop_R.sh 20 XENON Rn222
source  MS1Loop_R.sh 4 XENON Rn220

