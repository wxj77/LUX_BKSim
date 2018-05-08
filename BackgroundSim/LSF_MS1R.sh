#!/bin/bash

# This script is what runs on the cluster. It runs once for each set of 25000 events simulated on the cluster.
# The ulimit line is cluster specific. 
# Basically if you're running a version of LUXSim that segfaults, there may be a core dump and this line prevents it on the Yale cluster. 
# The next block tells the script where important stuff is (where root and LUXSim are and where to output the simultaions and where to store the macros, etc.)
# The final block does the following steps:
# 1. Sources EDepMacGenerator_R.sh which creates the specific LUXSim macro for this set of simulations.
# 2. Executes LUXSim while calling this macro as the arguement
# 3. Uses LUXRootReader to convert the binary output of LUXSim into a root file
# 4. Runs the root script MakeEventsFile_WS.c to pare events into the WIMP search region of interest, confine them to the main Xe volume, and break them into managable size lists of events. 
# 5. Removes the original binary and root files in order to save disk space
path=$BKSimOutput
dirpath=$path/BackgroundSim/
ulimit -c 0

ROOTLOCATION=/gpfs/home/fas/mckinsey/ko267/Software/root/bin
SCRIPTLOCATION=$dirpath
MACROLOCATION=$dirpath/macros
mkdir $MACROLOCATION
MSLOCATION=$dirpath
#LUXSIMLOCATION=$path/LUXSim
ROOTREADERLOCATION=${LUXSIMLOCATION}/tools
OUTPUTFOLDER=${BKSimOutputResult}
OUTPUTFOLDERTEMP=/scratch/wxj/
DATE=10072017

mkdir -p $OUTPUTFOLDER/$2/$3/
mkdir -p $OUTPUTFOLDERTEMP/$2/$3/

#module load Geant4/4.9.4.p04
#:module load ROOT/5.34.10
cd $MSLOCATION
source $MSLOCATION/EDepMacGenerator_R.sh $1 $2 $3 $MACROLOCATION $OUTPUTFOLDERTEMP $DATE
cd $LUXSIMLOCATION
#echo ./LUXSimExecutable $MACROLOCATION/R4Bkg_$2_$3_$1.mac

echo "start LUXSIM"
./LUXSimExecutable $MACROLOCATION/R4Bkg_$2_$3_$1.mac

echo "success LUXSIM"
#source $ROOTLOCATION/thisroot.sh
#echo ${ROOTREADERLOCATION}/LUXRootReader $OUTPUTFOLDER/$2/$3/R4Bkg_$2_$3_$1.bin
${ROOTREADERLOCATION}/LUXRootReader $OUTPUTFOLDERTEMP/$2/$3/R4Bkg_$2_$3_$1.bin
echo "success ROOTreader"


cd $OUTPUTFOLDERTEMP/$2/$3/
#root -l -q -b "$MSLOCATION/MakeEventsFile.c(\"R4Bkg_$2_$3_$1.root\",\"R4Bkg_$2_$3_$1\")"
#root -l -q -b "$MSLOCATION/MakeEventsFile_WS.c(\"R4Bkg_$2_$3_$1.root\",\"R4Bkg_$2_$3_$1\")"
root -l -q -b "${MSLOCATION}/MakeEventsFile_WS_new.c(\"R4Bkg_$2_$3_$1.root\",\"R4Bkg_$2_$3_$1\")"
root -q -l -b "${MSLOCATION}/trimtree.C(\"R4Bkg_$2_$3_$1.root\", \"R4BkT_$2_$3_$1.root\")"
cp R4Bkg_$2_$3_$1*.data $OUTPUTFOLDER/$2/$3/
mkdir -p $OUTPUTFOLDER/$2/$3/rootfiles/
cp R4BkT_$2_$3_$1.root $OUTPUTFOLDER/$2/$3/
rm -f $OUTPUTFOLDERTEMP/$2/$3/R4Bkg_$2_$3_$1.root ##Wei, keep results for .root to check the correctness of simulations.
rm -f $OUTPUTFOLDERTEMP/$2/$3/R4Bkg_$2_$3_$1.bin

exit
