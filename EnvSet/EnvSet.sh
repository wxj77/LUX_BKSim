#!/bin/bash

# This code is for set up environment variables in SLAC cluster. If you want to use your code in other cluster, you may need to change the location of these variables.

#root
source /afs/slac.stanford.edu/u/rd/wto/Downloads/root/bin/thisroot.sh
#source /afs/slac.stanford.edu/package/cernroot/53426/bin/thisroot.sh

#nest
#nest source code: git@github.com:luxdarkmatter/LibNEST.git
#v1 version 
source /nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/thislibNEST.sh

#geant4/4.9.4.p04
source /afs/slac.stanford.edu/package/geant4/9.4.p04/env-rhel6-64.sh	

#LUXSim, need to compile this before using the other simulation code
#LUXSim is using #geant4/4.9.4.p04 and root/5.34.20
#LUXSim source code : git@github.com:luxdarkmatter/LUXSim.git
export LUXSIMLOCATION="/nfs/farm/g/superb/u01/lz/wxj/background/LUXSim/"

#Simulation file location
#export bk="/nfs/farm/g/superb/u01/lz/wxj/BackgroundModel//BkgSimulationTools" 
#export BK="/nfs/farm/g/superb/u01/lz/R4BG_WJ2/"

# where the sim output is stored and going to be used.
export SIMDIR="/nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100/"

# where the BackgroundSim/ folder is locate. 
# in this case, it should be /my-path-to/LUX_BKSim
#export BKSimOutput="/nfs/farm/g/superb/u01/lz/wxj/background/"
export BKSimOutput="/nfs/farm/g/superb/u01/lz/wxj/LUX_BKSim/"

# where the BkgSimulationTool/ folder is locate. 
# in this case, it should be /my-path-to/LUX_BKSim
export BKSimTools="/nfs/farm/g/superb/u01/lz/wxj/LUX_BKSim/"

# where the sim output is going to be stored. 
export BKSimOutputResult="/nfs/farm/g/superb/u01/lz/wxj/background/LUXR4Background100_newtrim/"

if [ "${HOSTNAME:0:4}" = "pdsf" ];then
	module unload ROOT
	module load ROOT/5.34.20
	module unload gcc
	module load gcc/4.8.2
	export LUXSIMLOCATION="/project/projectdirs/lux/users/weiji/LUXSim/"
	export SIMDIR="/project/projectdirs/lux/users/weiji/wei/data/"
	export BKSimOutput="/project/projectdirs/lux/users/weiji/wei/LUX_BKSim/"
	export BKSimTools="/project/projectdirs/lux/users/weiji/wei/LUX_BKSim/"
	export BKSimOutputResult="/project/projectdirs/lux/users/weiji/wei/data/"
fi
