#! /bin/bash

# Need to run in BkgSimulationTools folder.
#
# Change /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools/generateFastBkg to your path to generateFastBkg
# Change /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh to where your root lib is.
# Change /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh to where your libnest lib is.

#cd /nfs/farm/g/superb/u01/lz/wxj/BackgroundModel/BkgSimulationTools

# Change SIMDIR to the folder that has all R4BG files.

source /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh
source /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh

root -l -b -q "filter.C(\"$1\",\"$2\",$3)"


