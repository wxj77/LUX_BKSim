#!/bin/bash

#This script is the "master list" for setting the numbers of events to simulate for each isotope in each detector component.
#Each line has the number of sets of 250000 events to simulate, followed by the detector component and the isotope/chain.

#Values for each set correspond to more than 10X Run04 Live Time (min of 340 live days)
path=$BKSimOutput
#cd $path/Run4Background/Tools_100X_100keVee
cd $path/BackgroundSim/ 

source MS1Loop_R.sh 4820 PMTT K40
source MS1Loop_R.sh 184 PMTT Co60
source MS1Loop_R.sh 7040 PMTT Ra226
source MS1Loop_R.sh 612 PMTT Th232E
source MS1Loop_R.sh 1632 PMTT Th232L
source MS1Loop_R.sh 4820 PMTB K40
source MS1Loop_R.sh 184 PMTB Co60
source MS1Loop_R.sh 7040 PMTB Ra226
source MS1Loop_R.sh 612 PMTB Th232E
source MS1Loop_R.sh 1632 PMTB Th232L

source MS1Loop_R.sh 192 HDPE Th232E
source MS1Loop_R.sh 512 HDPE Th232L
source MS1Loop_R.sh 60 PTFE Th232E
source MS1Loop_R.sh 160 PTFE Th232L
source MS1Loop_R.sh 48 REFLECTOR Th232E
source MS1Loop_R.sh 128 REFLECTOR Th232L
source MS1Loop_R.sh 660 CRYOSTAT Th232E
source MS1Loop_R.sh 1760 CRYOSTAT Th232L
source MS1Loop_R.sh 12 FIELDGRID Th232E
source MS1Loop_R.sh 32 FIELDGRID Th232L
source MS1Loop_R.sh 84 FIELDRING Th232E
source MS1Loop_R.sh 224 FIELDRING Th232L
source MS1Loop_R.sh 1728 PMTMOUNT Th232E
source MS1Loop_R.sh 4608 PMTMOUNT Th232L
source MS1Loop_R.sh 3000 FCSHIELD Th232E
source MS1Loop_R.sh 8000 FCSHIELD Th232L
source MS1Loop_R.sh 1248 TOPSHIELD Th232E
source MS1Loop_R.sh 3328 TOPSHIELD Th232L

source MS1Loop_R.sh 14076 PMT Ra226
source MS1Loop_R.sh 1152 HDPE Ra226
source MS1Loop_R.sh 504 PTFE Ra226
source MS1Loop_R.sh 504 REFLECTOR Ra226
source MS1Loop_R.sh 936 CRYOSTAT Ra226
source MS1Loop_R.sh 72 FIELDGRID Ra226
source MS1Loop_R.sh 180 FIELDRING Ra226
source MS1Loop_R.sh 3960 PMTMOUNT Ra226
source MS1Loop_R.sh 6840 FCSHIELD Ra226
source MS1Loop_R.sh 2844 TOPSHIELD Ra226
source MS1Loop_R.sh 9636 PMT K40
source MS1Loop_R.sh 436 CRYOSTAT K40
source MS1Loop_R.sh 4 FIELDGRID K40
source MS1Loop_R.sh 380 PMT Co60
source MS1Loop_R.sh 8 FIELDGRID Co60
source MS1Loop_R.sh 12 FIELDRING Co60
source MS1Loop_R.sh 340 PMTMOUNT Co60
source MS1Loop_R.sh 588 FCSHIELD Co60
source  MS1Loop_R.sh 244 TOPSHIELD Co60
source  MS1Loop_R.sh 1296 CRYOSTAT Sc46
source  MS1Loop_R.sh 20 XENON Rn222
source  MS1Loop_R.sh 4 XENON Rn220
source  MS1Loop_R.sh 204 XENON Xe127
source  MS1Loop_R.sh 4 XENON Kr85

source  MS1Loop_R.sh 120 MYLAR Th232E
source  MS1Loop_R.sh 320 MYLAR Th232L

source MS1Loop_R.sh 1728 MYLAR Ra226
source MS1Loop_R.sh 1666 MYLAR K40

source MS1Loop_R.sh 1264 THERMALSHIELD Th232E
source MS1Loop_R.sh 3104 THERMALSHIELD Th232L

source MS1Loop_R.sh 8280 THERMALSHIELD Ra226
source MS1Loop_R.sh 748 THERMALSHIELD K40

source  MS1Loop_R.sh 720 FCSHIELDPLANE Co60
source  MS1Loop_R.sh 160 FCSHIELDPLANE Pb212
source  MS1Loop_R.sh 160 FCSHIELDPLANE Pb214
source  MS1Loop_R.sh 160 FCSHIELDPLANE Bi210
source  MS1Loop_R.sh 160 FCSHIELDPLANE Bi214
source  MS1Loop_R.sh 160 FCSHIELDPLANE Tl208
