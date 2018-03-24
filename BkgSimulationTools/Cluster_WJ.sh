#! /bin/bash

# Runs the root script Cluster.c over this set of files.
# Cluster.c groups together all the energy deposits into individual energy-weighted deposits (very conservatively, 0.4 cm radius grouping) and writes this back out to a new file


#	echo "Illegal number of parameters ./Cluster.sh BkgSrc BkgComp"


	SIMDIR=/nfs/farm/g/superb/u01/lz/R4BG_WJ/
for BkgSrc in $SIMDIR/[ABCDEFGHIJKLMNOPQRSTUVWXYZ]*; do
#    echo $BkgSrc 
    bkgsrc="$(basename $BkgSrc)"
    for BkgComp in $BkgSrc/*; do
#        echo $BkgComp
        bkgcomp="$(basename $BkgComp)"
	SIMFILES=$BkgComp/*.data	
	CombinedFile=$BkgComp/${bkgsrc}_${bkgcomp}.comb
#        echo $CombinedFile
	IFS='.' read -r -a CombShort <<< "$CombinedFile"
	echo "Combining data files into " $CombinedFile "  

	cat $BkgComp/*.data > $CombinedFile

	root -b -q -l "Cluster.C(\"$CombinedFile\",\"${CombShort[0]}\")"
#root -b -q -l 'Cluster.C("/nfs/farm/g/superb/u01/lz/R4BG_WJ//PMTT/Co60/PMTT_Co60.comb","/nfs/farm/g/superb/u01/lz/R4BG_WJ//PMTT/Co60/PMTT_Co60")'	
	rm $CombinedFile
    done
done
