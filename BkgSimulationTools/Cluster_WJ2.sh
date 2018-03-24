#! /bin/bash

# Runs the root script Cluster.c over this set of files.
# Cluster.c groups together all the energy deposits into individual energy-weighted deposits (very conservatively, 0.4 cm radius grouping) and writes this back out to a new file


#	echo "Illegal number of parameters ./Cluster.sh BkgSrc BkgComp"

queue='long'
walltime='22:00'
SIMDIR=/nfs/farm/g/superb/u01/lz/R4BG_WJ/
for BkgSrc in $SIMDIR/[ABCDEFGHIJKLMNOPQRSTUVWXYZ]*; do
#    echo $BkgSrc 
    bkgsrc="$(basename $BkgSrc)"
    for BkgComp in $BkgSrc/*; do
#        echo $BkgComp
        bkgcomp="$(basename $BkgComp)"
        echo bsub -q $queue -W $walltime -o /dev/null "./Cluster.sh ${bkgsrc} ${bkgcomp}"
        bsub -q $queue -W $walltime -o /dev/null "./Cluster.sh ${bkgsrc} ${bkgcomp}"
    done
done
