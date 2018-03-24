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
#	SIMFILES=$BkgComp/*.data	

        mkdir $BkgComp/TB1
        mkdir $BkgComp/TB2
        mkdir $BkgComp/TB3
        mkdir $BkgComp/TB4
#        echo $CombinedFile

        root -b -q -l "mapDatFile.C+(\"${SIMDIR}\",\"${bkgsrc}\", \"${bkgcomp}\", 1)"
        root -b -q -l "mapDatFile.C+(\"${SIMDIR}\",\"${bkgsrc}\", \"${bkgcomp}\", 2)"
        root -b -q -l "mapDatFile.C+(\"${SIMDIR}\",\"${bkgsrc}\", \"${bkgcomp}\", 3)"
        root -b -q -l "mapDatFile.C+(\"${SIMDIR}\",\"${bkgsrc}\", \"${bkgcomp}\", 4)"


#	root -b -q -l "Cluster.C(\"$CombinedFile\",\"${CombShort[0]}\")"
    done
done
