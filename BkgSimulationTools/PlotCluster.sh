#! /bin/bash

# Runs the root script Cluster.c over this set of files.
# Cluster.c groups together all the energy deposits into individual energy-weighted deposits (very conservatively, 0.4 cm radius grouping) and writes this back out to a new file


#	echo "Illegal number of parameters ./Cluster.sh BkgSrc BkgComp"

source  /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh
source /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh
mkdir $1

#queue='short'
queue='long'
walltime='22:00'
SIMDIR=/nfs/farm/g/superb/u01/lz/R4BG_WJ2/

for BkgSrc in $SIMDIR/[ABCDEFGHIJKLMNOPQRSTUVWXYZ]*; do
#for BkgSrc in $SIMDIR/[OPQRSTUVWXYZ]*; do
#    echo $BkgSrc 
    bkgsrc="$(basename $BkgSrc)"
    for BkgComp in $BkgSrc/*; do
        if [ -d "${BkgComp}" ] ; then
        echo $BkgComp
        bkgcomp="$(basename $BkgComp)"
#	SIMFILES=$BkgComp/*.data	

        /nfs/farm/g/superb/u01/lz/wxj/WeiCode/Cpp_Tutorial/bin/PlotAfterCluster ${BkgComp} $1/${bkgsrc}_${bkgcomp}.root $1/${bkgsrc}_${bkgcomp}.png ${bkgsrc} ${bkgcomp}
echo /nfs/farm/g/superb/u01/lz/wxj/WeiCode/Cpp_Tutorial/bin/PlotAfterCluster ${BkgComp} $1/${bkgsrc}_${bkgcomp}.root $1/${bkgsrc}_${bkgcomp}.png ${bkgsrc} ${bkgcomp}
#        bsub -q $queue -W $walltime -o /dev/null "/nfs/farm/g/superb/u01/lz/wxj/WeiCode/Cpp_Tutorial/bin/PlotAfterCluster ${BkgComp} ${BkgComp}_Cluster_WJ.root $1/${bkgsrc}_${bkgcomp}.png ${bkgsrc} ${bkgcomp}"
        fi
    done
done
