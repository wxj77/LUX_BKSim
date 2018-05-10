#!/bin/bash

FolderName=$1
NewFolderName=$2
mkdir -p ${NewFolderName}

for X in `ls ${FolderName}`; do
	for Y in `ls ${FolderName}/${X}`; do
		Num=`ls ${FolderName}/${X}/${Y}/*.root|wc -l`
		if [ "${Num}" -gt 0 ]; then
			echo $Num
			cat -f ${NewFolderName}/${X}_${Y}_COMB.dat ${FolderName}/${X}/${Y}/*.data
			rm ${FolderName}/${X}/${Y}/*.data
			#rm ${FolderName}/${X}/${Y}/*.bin
		fi;
	done
done
