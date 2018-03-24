#! /bin/bash

# Need to run in BkgSimulationTools folder.
#
# Change /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg to your path to generateFastBkg
# Change /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh to where your root lib is.
# Change /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh to where your libnest lib is.

#cd /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools

# Change SIMDIR to the folder that has all R4BG files.
# Run "./FilterAll.sh"


source /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh

queue='long'
walltime='22:00'
#SIMDIR=/nfs/farm/g/superb/u01/lz/R4BG_WJ2/

declare -a SourceArray=("Ex_All" "Ex_Th232" "FCS_Co60" "FCS_Pb212" "FCS_Ra226" "FCS_Tl208" "PMTB_All" "PMTB_Th232" "PMTT_All" "PMTT_Th232" "Xe_Kr85" "Xe_Rn220" "Xe_Rn222" "Xe_Xe127")
for TimeBin in 1 2 3 4; do
#for TimeBin in 1; do
#    for SourceType in "${SourceArray[@]}" ; do 
    for SourceType in "Xe_Xe127"; do
    echo $SourceType    
    bsub -q $queue -W $walltime -o /dev/null "./FilterSingle.sh ${SIMDIR} ${SourceType} ${TimeBin}"
    done
done
