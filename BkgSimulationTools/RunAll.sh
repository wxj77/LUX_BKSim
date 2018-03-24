#! /bin/bash

# Need to run in BkgSimulationTools folder.
#
# Change /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg to your path to generateFastBkg
# Change /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh to where your root lib is.
# Change /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh to where your libnest lib is.

#cd /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools

# Change SIMDIR to the folder that has all R4BG files.
# Run "./RunAll.sh"


source  /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh
source /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh

#queue='short'
queue='long'
walltime='100:00'
#SIMDIR=/nfs/farm/g/superb/u01/lz/R4BG_WJ3/

for BkgSrc in $SIMDIR/[ABCDEFGHIJKLMNOPQRSTUVWXYZ]*; do
#for BkgSrc in $SIMDIR/[OPQRSTUVWXYZ]*; do
#    echo $BkgSrc 
    bkgsrc="$(basename $BkgSrc)"
    for BkgComp in $BkgSrc/*; do
#        echo $BkgComp
        bkgcomp="$(basename $BkgComp)"
#	SIMFILES=$BkgComp/*.data	

        mkdir $BkgComp/TB1
        mkdir $BkgComp/TB2
        mkdir $BkgComp/TB3
        mkdir $BkgComp/TB4
#        echo -q $queue -W $walltime -o /dev/null "./Map_WJ_Single.sh ${SIMDIR} ${bkgsrc} ${bkgcomp}"
        bsub -q $queue -W $walltime -M 4000 -o /dev/null "./RunSingle.sh ${SIMDIR} ${bkgsrc} ${bkgcomp}"
    done
done
