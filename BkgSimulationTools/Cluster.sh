#! /bin/bash

# Runs the root script Cluster.c over this set of files.
# Cluster.c groups together all the energy deposits into individual energy-weighted deposits (very conservatively, 0.4 cm radius grouping) and writes this back out to a new file

if test "$#" -ne 2; then
	echo "Illegal number of parameters ./Cluster.sh BkgSrc BkgComp"

else
	SIMDIR=/nfs/farm/g/superb/u01/lz/R4BG_WJ/

	SIMFILES=$SIMDIR/$1/$2/*.data
	
	CombinedFile=$SIMDIR/$1/$2/$1_$2.comb
	IFS='.' read -r -a CombShort <<< "$CombinedFile"

	echo "Combining data files into " $CombinedFile " " $CombShort  
	cat $SIMDIR/$1/$2/*.data > $CombinedFile

	root -b -q -l "Cluster.C(\"$CombinedFile\",\"${CombShort[0]}\")"
#root -b -q -l 'Cluster.C("/nfs/farm/g/superb/u01/lz/R4BG_WJ//PMTT/Co60/PMTT_Co60.comb","/nfs/farm/g/superb/u01/lz/R4BG_WJ//PMTT/Co60/PMTT_Co60")'
	

#	rm $CombinedFile

fi
