#!/bin/bash

newfolder=new
folder=$1
if [[ "$#" -gt 1 ]]; then
  newfolder=$2
fi

mkdir -p ${newfolder}

for s1 in `ls -d ${folder}/*`; do
#  echo ${s1}
  S1=$(basename ${s1})
  echo ${S1}
  for s2 in `ls -d ${s1}`/*; do 
    S2=$(basename ${s2})
    echo ${S2}
#    echo `ls ${s2}/rootfiles`
    mkdir -p ${newfolder}/${S1}/${S2}
    mv ${s2}/rootfiles ${newfolder}/${S1}/${S2}
  done
done
