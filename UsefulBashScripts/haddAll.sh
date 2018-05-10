#!/bin/bash

FolderName=$1
NewFolderName=$2
mkdir -p ${NewFolderName}

for X in `ls ${FolderName}`; do
	for Y in `ls ${FolderName}/${X}`; do
		Num=`ls ${FolderName}/${X}/${Y}/*.root|wc -l`
		if [ "${Num}" -gt 0 ]; then
			echo $Num
			hadd -f ${NewFolderName}/${X}_${Y}_COMB.root ${FolderName}/${X}/${Y}/*.root
			rm ${FolderName}/${X}/${Y}/*.root
			rm ${FolderName}/${X}/${Y}/*.bin
		fi;
	done
done

for f in *.root; do
	echo ${f:0:${#f}-5}_Trim.root 
	root -q -b "/nfs/farm/g/superb/u01/lz/wxj//LUX_BKSim/BackgroundSim/trimtree.C(\"$f\", \"${f:0:${#f}-5}_Trim.root\")"	
done

