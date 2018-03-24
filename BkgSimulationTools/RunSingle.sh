#! /bin/bash

# Need to run in BkgSimulationTools folder.
#
# Change /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg to your path to generateFastBkg
# Change /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh to where your root lib is.
# Change /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh to where your libnest lib is.

#cd /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools

##########################################
#####use lines below if needed.
source /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh
source /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh

SIMFILES=$1/$2/$3/*.data
CombinedFile=$1/$2/$3/$2_$3.comb
IFS='.' read -r -a CombShort <<< "$CombinedFile"

echo "Combining data files into " $CombinedFile " " $CombShort  
cat $1/$2/$3/*.data > $CombinedFile

root -b -q -l "Cluster.C(\"$CombinedFile\",\"${CombShort[0]}\")"

root -b -q -l "mapDatFile.C+(\"$1\",\"$2\", \"$3\", 1)"
root -b -q -l "mapDatFile.C+(\"$1\",\"$2\", \"$3\", 2)"
root -b -q -l "mapDatFile.C+(\"$1\",\"$2\", \"$3\", 3)"
root -b -q -l "mapDatFile.C+(\"$1\",\"$2\", \"$3\", 4)"


/nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg $1 $2 $3 1
/nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg $1 $2 $3 2
/nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg $1 $2 $3 3
/nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg $1 $2 $3 4

