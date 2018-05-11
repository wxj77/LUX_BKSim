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
#    mkdir -p ${newfolder}/${S1}/${S2}
    mkdir -p ${newfolder}/${S1}/${S2}/rootfiles
    for f in `ls ${s2}/rootfiles/*`; do 
      ff=$(basename ${f})
      newff=R4Bkt_${ff:6:${#ff}-6}
      newf=${newfolder}/${S1}/${S2}/rootfiles/${newff}
#      echo ${newf}
#      echo ${#f}  ${f} 
#      echo ${#newff} ${newff} 
#      root -q -l -b "../BackgroundSim/trimtreemore.C(\"${f}\", \"${newf}\")"
      bsub -o /dev/null -W 22:30 -q long "bash trimtreemore_single.sh ${f} ${newf}"
    done
  done
done
