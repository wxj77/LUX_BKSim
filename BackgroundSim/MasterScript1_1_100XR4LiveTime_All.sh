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

source MS1Loop_R.sh 1920 HDPE Th232E
source MS1Loop_R.sh 5120 HDPE Th232L
source MS1Loop_R.sh 600 PTFE Th232E
source MS1Loop_R.sh 1600 PTFE Th232L
source MS1Loop_R.sh 480 REFLECTOR Th232E
source MS1Loop_R.sh 1280 REFLECTOR Th232L
source MS1Loop_R.sh 6600 CRYOSTAT Th232E
source MS1Loop_R.sh 17600 CRYOSTAT Th232L
source MS1Loop_R.sh 120 FIELDGRID Th232E
source MS1Loop_R.sh 320 FIELDGRID Th232L
source MS1Loop_R.sh 840 FIELDRING Th232E
source MS1Loop_R.sh 2240 FIELDRING Th232L
source MS1Loop_R.sh 17280 PMTMOUNT Th232E
source MS1Loop_R.sh 46080 PMTMOUNT Th232L
source MS1Loop_R.sh 30000 FCSHIELD Th232E
source MS1Loop_R.sh 80000 FCSHIELD Th232L
source MS1Loop_R.sh 12480 TOPSHIELD Th232E
source MS1Loop_R.sh 33280 TOPSHIELD Th232L

source MS1Loop_R.sh 140760 PMT Ra226
source MS1Loop_R.sh 11520 HDPE Ra226
source MS1Loop_R.sh 5040 PTFE Ra226
source MS1Loop_R.sh 5040 REFLECTOR Ra226
source MS1Loop_R.sh 9360 CRYOSTAT Ra226
source MS1Loop_R.sh 720 FIELDGRID Ra226
source MS1Loop_R.sh 1800 FIELDRING Ra226
source MS1Loop_R.sh 39600 PMTMOUNT Ra226
source MS1Loop_R.sh 68400 FCSHIELD Ra226
source MS1Loop_R.sh 28440 TOPSHIELD Ra226
source MS1Loop_R.sh 96360 PMT K40
source MS1Loop_R.sh 4360 CRYOSTAT K40
source MS1Loop_R.sh 40 FIELDGRID K40
source MS1Loop_R.sh 3800 PMT Co60
source MS1Loop_R.sh 80 FIELDGRID Co60
source MS1Loop_R.sh 120 FIELDRING Co60
source MS1Loop_R.sh 3400 PMTMOUNT Co60
source MS1Loop_R.sh 5880 FCSHIELD Co60
source  MS1Loop_R.sh 2440 TOPSHIELD Co60
source  MS1Loop_R.sh 12960 CRYOSTAT Sc46
source  MS1Loop_R.sh 200 XENON Rn222
source  MS1Loop_R.sh 40 XENON Rn220
source  MS1Loop_R.sh 2040 XENON Xe127
source  MS1Loop_R.sh 40 XENON Kr85

source  MS1Loop_R.sh 1200 MYLAR Th232E
source  MS1Loop_R.sh 3200 MYLAR Th232L

source MS1Loop_R.sh 17280 MYLAR Ra226
source MS1Loop_R.sh 16660 MYLAR K40

source MS1Loop_R.sh 12640 THERMALSHIELD Th232E
source MS1Loop_R.sh 31040 THERMALSHIELD Th232L

source MS1Loop_R.sh 82800 THERMALSHIELD Ra226
source MS1Loop_R.sh 7480 THERMALSHIELD K40

source  MS1Loop_R.sh 7200 FCSHIELDPLANE Co60
source  MS1Loop_R.sh 1600 FCSHIELDPLANE Pb212
source  MS1Loop_R.sh 1600 FCSHIELDPLANE Pb214
source  MS1Loop_R.sh 1600 FCSHIELDPLANE Bi210
source  MS1Loop_R.sh 1600 FCSHIELDPLANE Bi214
source  MS1Loop_R.sh 1600 FCSHIELDPLANE Tl208
