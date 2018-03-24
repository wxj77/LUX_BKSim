#! /bin/bash
source /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh
source /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh
root -b -q -l "mapDatFile.C+(\"$1\",\"$2\", \"$3\", 1)"
root -b -q -l "mapDatFile.C+(\"$1\",\"$2\", \"$3\", 2)"
root -b -q -l "mapDatFile.C+(\"$1\",\"$2\", \"$3\", 3)"
root -b -q -l "mapDatFile.C+(\"$1\",\"$2\", \"$3\", 4)"
