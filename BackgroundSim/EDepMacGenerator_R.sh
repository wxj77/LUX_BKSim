#!/bin/bash

# This script generates macro files givin six inputs:
# 1. Loop number -- This is used to ensure that a different random seed is used for each of the sets of events.
# 2. Source -- The region of the detector where the activity originates. See the backgrounds paper for more details. 
# 3. Isotope -- The desired isotope or decay chain.
# 4. Macro location -- Disk location where the macro should be saved.
# 5. Output folder -- Where LUXSim should save the resultant binary file.
# 6. File Date -- Should be self explanitory.
# The first block of code sets up the macro name and file location.
# The next portion of the code is a set of nested if statements that defines the specific geometry that will be used for each source/isotope combination.
# After the if statments is a block of code where the MACSTR variables are defined. This is a set of strings that defines the body of the macro to be generated.
# Finally, the echo statement writes the text of the macro to the specified file. 

LOOPNUMBER=$1 
SOURCE=$2
ISOTOPE=$3
MACROLOCATION=$4
OUTPUTFOLDER=$5
FILEDATE=$6

MACFILELOC=$MACROLOCATION
MACFILEBASE=R4Bkg$'_'$SOURCE$'_'$ISOTOPE$'_'$LOOPNUMBER
MACFILEEXT=.mac

OUTPUTLOCATION=$OUTPUTFOLDER/$2/$3

MACSTRB=$'/LUXSim/source/set '

if  [ "$SOURCE" = "FCSHIELDPLANE" ]; then
	if [ "$ISOTOPE" = "Co60" ]; then
		MACSTRB=$''
		SIR=$'/gps/particle ion\n/gps/ion 27 60 0 0\n/gps/ang/type iso\n/gps/energy 0. keV\n/gps/pos/type Volume\n/gps/pos/shape Cylinder\n/gps/pos/halfz .01 cm\n/gps/pos/radius 27.2 cm\n/gps/pos/centre 0. 0. -23.2 cm\n/gps/pos/confine RadDome\n/grdm/nucleusLimits 60 60 27 27'
	fi
	if [ "$ISOTOPE" = "Tl208" ]; then
		MACSTRB=$''
		SIR=$'/gps/particle ion\n/gps/ion 81 208 0 0\n/gps/ang/type iso\n/gps/energy 0. keV\n/gps/pos/type Volume\n/gps/pos/shape Cylinder\n/gps/pos/halfz .01 cm\n/gps/pos/radius 27.2 cm\n/gps/pos/centre 0. 0. -23.2 cm\n/gps/pos/confine RadDome\n/grdm/nucleusLimits 208 208 81 81'
	fi
	if [ "$ISOTOPE" = "Bi214" ]; then
		MACSTRB=$''
		SIR=$'/gps/particle ion\n/gps/ion 83 214 0 0\n/gps/ang/type iso\n/gps/energy 0. keV\n/gps/pos/type Volume\n/gps/pos/shape Cylinder\n/gps/pos/halfz .01 cm\n/gps/pos/radius 27.2 cm\n/gps/pos/centre 0. 0. -23.2 cm\n/gps/pos/confine RadDome\n/grdm/nucleusLimits 214 214 83 83'
	fi
	if [ "$ISOTOPE" = "Pb212" ]; then
		MACSTRB=$''
		SIR=$'/gps/particle ion\n/gps/ion 82 212 0 0\n/gps/ang/type iso\n/gps/energy 0. keV\n/gps/pos/type Volume\n/gps/pos/shape Cylinder\n/gps/pos/halfz .01 cm\n/gps/pos/radius 27.2 cm\n/gps/pos/centre 0. 0. -23.2 cm\n/gps/pos/confine RadDome\n/grdm/nucleusLimits 212 212 82 82'
	fi
	if [ "$ISOTOPE" = "Bi210" ]; then
		MACSTRB=$''
		SIR=$'/gps/particle ion\n/gps/ion 83 210 0 0\n/gps/ang/type iso\n/gps/energy 0. keV\n/gps/pos/type Volume\n/gps/pos/shape Cylinder\n/gps/pos/halfz .01 cm\n/gps/pos/radius 27.2 cm\n/gps/pos/centre 0. 0. -23.2 cm\n/gps/pos/confine RadDome\n/grdm/nucleusLimits 210 210 83 83'
	fi
	if [ "$ISOTOPE" = "Pb214" ]; then
		MACSTRB=$''
		SIR=$'/gps/particle ion\n/gps/ion 82 214 0 0\n/gps/ang/type iso\n/gps/energy 0. keV\n/gps/pos/type Volume\n/gps/pos/shape Cylinder\n/gps/pos/halfz .01 cm\n/gps/pos/radius 27.2 cm\n/gps/pos/centre 0. 0. -23.2 cm\n/gps/pos/confine RadDome\n/grdm/nucleusLimits 214 214 82 82'
	fi
fi

if [ "$SOURCE" = "PMTT" ]; then
	if [ "$ISOTOPE" = "U238" ]; then
		SIR=$'Top_PMT_Window U238 1.4 Bq'
	fi
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'Top_PMT_Window Th232 175. mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'Top_PMT_Window Ra226 665. mBq'
	fi
	if [ "$ISOTOPE" = "K40" ]; then
		SIR=$'Top_PMT_Window SingleDecay_40_19 4.1 Bq'
	fi
	if [ "$ISOTOPE" = "Co60" ]; then
		SIR=$'Top_PMT_Window SingleDecay_60_27 160. mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'Top_PMT_Window SingleDecay_232_90 1. mBq\n/LUXSim/source/set Top_PMT_Window SingleDecay_228_88 1. mBq\n/LUXSim/source/set Top_PMT_Window SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'Top_PMT_Window SingleDecay_228_90 1. mBq\n/LUXSim/source/set Top_PMT_Window SingleDecay_224_88 1. mBq\n/LUXSim/source/set Top_PMT_Window SingleDecay_220_86 1. mBq\n/LUXSim/source/set Top_PMT_Window SingleDecay_216_84 1. mBq\n/LUXSim/source/set Top_PMT_Window SingleDecay_212_82 1. mBq\n/LUXSim/source/set Top_PMT_Window SingleDecay_212_83 1. mBq\n/LUXSim/source/set Top_PMT_Window SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set Top_PMT_Window SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "PMTB" ]; then
	if [ "$ISOTOPE" = "U238" ]; then
		SIR=$'Bottom_PMT_Window U238 1.4 Bq'
	fi
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'Bottom_PMT_Window Th232 175. mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'Bottom_PMT_Window Ra226 665. mBq'
	fi
	if [ "$ISOTOPE" = "K40" ]; then
		SIR=$'Bottom_PMT_Window SingleDecay_40_19 4.1 Bq'
	fi
	if [ "$ISOTOPE" = "Co60" ]; then
		SIR=$'Bottom_PMT_Window SingleDecay_60_27 160. mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'Bottom_PMT_Window SingleDecay_232_90 1. mBq\n/LUXSim/source/set Bottom_PMT_Window SingleDecay_228_88 1. mBq\n/LUXSim/source/set Bottom_PMT_Window SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'Bottom_PMT_Window SingleDecay_228_90 1. mBq\n/LUXSim/source/set Bottom_PMT_Window SingleDecay_224_88 1. mBq\n/LUXSim/source/set Bottom_PMT_Window SingleDecay_220_86 1. mBq\n/LUXSim/source/set Bottom_PMT_Window SingleDecay_216_84 1. mBq\n/LUXSim/source/set Bottom_PMT_Window SingleDecay_212_82 1. mBq\n/LUXSim/source/set Bottom_PMT_Window SingleDecay_212_83 1. mBq\n/LUXSim/source/set Bottom_PMT_Window SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set Bottom_PMT_Window SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "HDPE" ]; then
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'HDPEHolder Th232 48.1 mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'HDPEHolder Ra226 97.7 mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'HDPEHolder SingleDecay_232_90 1. mBq\n/LUXSim/source/set HDPEHolder SingleDecay_228_88 1. mBq\n/LUXSim/source/set HDPEHolder SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'HDPEHolder SingleDecay_228_90 1. mBq\n/LUXSim/source/set HDPEHolder SingleDecay_224_88 1. mBq\n/LUXSim/source/set HDPEHolder SingleDecay_220_86 1. mBq\n/LUXSim/source/set HDPEHolder SingleDecay_216_84 1. mBq\n/LUXSim/source/set HDPEHolder SingleDecay_212_82 1. mBq\n/LUXSim/source/set HDPEHolder SingleDecay_212_83 1. mBq\n/LUXSim/source/set HDPEHolder SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set HDPEHolder SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "PTFE" ]; then
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'PTFESheets Th232 15.5 mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'PTFESheets Ra226 46.5 mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'PTFESheets SingleDecay_232_90 1. mBq\n/LUXSim/source/set PTFESheets SingleDecay_228_88 1. mBq\n/LUXSim/source/set PTFESheets SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'PTFESheets SingleDecay_228_90 1. mBq\n/LUXSim/source/set PTFESheets SingleDecay_224_88 1. mBq\n/LUXSim/source/set PTFESheets SingleDecay_220_86 1. mBq\n/LUXSim/source/set PTFESheets SingleDecay_216_84 1. mBq\n/LUXSim/source/set PTFESheets SingleDecay_212_82 1. mBq\n/LUXSim/source/set PTFESheets SingleDecay_212_83 1. mBq\n/LUXSim/source/set PTFESheets SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set PTFESheets SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "REFLECTOR" ]; then
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'TopReflector Th232 7.5 mBq\n/LUXSim/source/set BottomReflector Th232 7.5 mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'TopReflector Ra226 23. mBq\n/LUXSim/source/set BottomReflector Ra226 23. mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'TopReflector SingleDecay_232_90 1. mBq\n/LUXSim/source/set TopReflector SingleDecay_228_88 1. mBq\n/LUXSim/source/set TopReflector SingleDecay_228_89 1. mBq\n/LUXSim/source/set BottomReflector SingleDecay_232_90 1. mBq\n/LUXSim/source/set BottomReflector SingleDecay_228_88 1. mBq\n/LUXSim/source/set BottomReflector SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'TopReflector SingleDecay_228_90 1. mBq\n/LUXSim/source/set TopReflector SingleDecay_224_88 1. mBq\n/LUXSim/source/set TopReflector SingleDecay_220_86 1. mBq\n/LUXSim/source/set TopReflector SingleDecay_216_84 1. mBq\n/LUXSim/source/set TopReflector SingleDecay_212_82 1. mBq\n/LUXSim/source/set TopReflector SingleDecay_212_83 1. mBq\n/LUXSim/source/set TopReflector SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set TopReflector SingleDecay_208_81 .6406 mBq\n/LUXSim/source/set BottomReflector SingleDecay_228_90 1. mBq\n/LUXSim/source/set BottomReflector SingleDecay_224_88 1. mBq\n/LUXSim/source/set BottomReflector SingleDecay_220_86 1. mBq\n/LUXSim/source/set BottomReflector SingleDecay_216_84 1. mBq\n/LUXSim/source/set BottomReflector SingleDecay_212_82 1. mBq\n/LUXSim/source/set BottomReflector SingleDecay_212_83 1. mBq\n/LUXSim/source/set BottomReflector SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set BottomReflector SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "CRYOSTAT" ]; then
	if [ "$ISOTOPE" = "U238" ]; then
		SIR=$'XenonVessel U238 1.724 Bq'
	fi
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'XenonVessel Th232 546. mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'XenonVessel Ra226 1. Bq'
	fi
	if [ "$ISOTOPE" = "K40" ]; then
		SIR=$'XenonVessel SingleDecay_40_19 2.38 Bq'
	fi
	if [ "$ISOTOPE" = "Sc46" ]; then
		SIR=$'XenonVessel SingleDecay_46_21 1. Bq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'XenonVessel SingleDecay_232_90 1. mBq\n/LUXSim/source/set XenonVessel SingleDecay_228_88 1. mBq\n/LUXSim/source/set XenonVessel SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'XenonVessel SingleDecay_228_90 1. mBq\n/LUXSim/source/set XenonVessel SingleDecay_224_88 1. mBq\n/LUXSim/source/set XenonVessel SingleDecay_220_86 1. mBq\n/LUXSim/source/set XenonVessel SingleDecay_216_84 1. mBq\n/LUXSim/source/set XenonVessel SingleDecay_212_82 1. mBq\n/LUXSim/source/set XenonVessel SingleDecay_212_83 1. mBq\n/LUXSim/source/set XenonVessel SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set XenonVessel SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "FIELDGRID" ]; then
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'GridFrame Th232 1.04 mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'GridFrame Ra226 6.3 mBq'
	fi
	if [ "$ISOTOPE" = "K40" ]; then
		SIR=$'GridFrame SingleDecay_40_19 1.8 mBq'
	fi
	if [ "$ISOTOPE" = "Co60" ]; then
		SIR=$'GridFrame SingleDecay_60_27 6.3 mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'GridFrame SingleDecay_232_90 1. mBq\n/LUXSim/source/set GridFrame SingleDecay_228_88 1. mBq\n/LUXSim/source/set GridFrame SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'GridFrame SingleDecay_228_90 1. mBq\n/LUXSim/source/set GridFrame SingleDecay_224_88 1. mBq\n/LUXSim/source/set GridFrame SingleDecay_220_86 1. mBq\n/LUXSim/source/set GridFrame SingleDecay_216_84 1. mBq\n/LUXSim/source/set GridFrame SingleDecay_212_82 1. mBq\n/LUXSim/source/set GridFrame SingleDecay_212_83 1. mBq\n/LUXSim/source/set GridFrame SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set GridFrame SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "FIELDRING" ]; then
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'FieldRing Th232 23. mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'FieldRing Ra226 15.2 mBq'
	fi
	if [ "$ISOTOPE" = "Co60" ]; then
		SIR=$'FieldRing SingleDecay_60_27 8.9 mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'FieldRing SingleDecay_232_90 1. mBq\n/LUXSim/source/set FieldRing SingleDecay_228_88 1. mBq\n/LUXSim/source/set FieldRing SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'FieldRing SingleDecay_228_90 1. mBq\n/LUXSim/source/set FieldRing SingleDecay_224_88 1. mBq\n/LUXSim/source/set FieldRing SingleDecay_220_86 1. mBq\n/LUXSim/source/set FieldRing SingleDecay_216_84 1. mBq\n/LUXSim/source/set FieldRing SingleDecay_212_82 1. mBq\n/LUXSim/source/set FieldRing SingleDecay_212_83 1. mBq\n/LUXSim/source/set FieldRing SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set FieldRing SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "PMTMOUNT" ]; then
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'TopPMTHolder Th232 490. mBq\n/LUXSim/source/set BottomPMTHolder Th232 490. mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'TopPMTHolder Ra226 372. mBq\n/LUXSim/source/set BottomPMTHolder Ra226 372. mBq'
	fi
	if [ "$ISOTOPE" = "Co60" ]; then
		SIR=$'TopPMTHolder SingleDecay_60_27 287. mBq\n/LUXSim/source/set BottomPMTHolder SingleDecay_60_27 287. mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'TopPMTHolder SingleDecay_232_90 1. mBq\n/LUXSim/source/set TopPMTHolder SingleDecay_228_88 1. mBq\n/LUXSim/source/set TopPMTHolder SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'TopPMTHolder SingleDecay_228_90 1. mBq\n/LUXSim/source/set TopPMTHolder SingleDecay_224_88 1. mBq\n/LUXSim/source/set TopPMTHolder SingleDecay_220_86 1. mBq\n/LUXSim/source/set TopPMTHolder SingleDecay_216_84 1. mBq\n/LUXSim/source/set TopPMTHolder SingleDecay_212_82 1. mBq\n/LUXSim/source/set TopPMTHolder SingleDecay_212_83 1. mBq\n/LUXSim/source/set TopPMTHolder SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set TopPMTHolder SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "FCSHIELD" ]; then
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'RadDome Th232 490. mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'RadDome Ra226 372. mBq'
	fi
	if [ "$ISOTOPE" = "Co60" ]; then
		SIR=$'RadDome SingleDecay_60_27 287. mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'RadDome SingleDecay_232_90 1. mBq\n/LUXSim/source/set RadDome SingleDecay_228_88 1. mBq\n/LUXSim/source/set RadDome SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'RadDome SingleDecay_228_90 1. mBq\n/LUXSim/source/set RadDome SingleDecay_224_88 1. mBq\n/LUXSim/source/set RadDome SingleDecay_220_86 1. mBq\n/LUXSim/source/set RadDome SingleDecay_216_84 1. mBq\n/LUXSim/source/set RadDome SingleDecay_212_82 1. mBq\n/LUXSim/source/set RadDome SingleDecay_212_83 1. mBq\n/LUXSim/source/set RadDome SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set RadDome SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "TOPSHIELD" ]; then
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'RadShield Th232 490. mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'RadShield Ra226 372. mBq'
	fi
	if [ "$ISOTOPE" = "Co60" ]; then
		SIR=$'RadShield SingleDecay_60_27 287. mBq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'RadShield SingleDecay_232_90 1. mBq\n/LUXSim/source/set RadShield SingleDecay_228_88 1. mBq\n/LUXSim/source/set RadShield SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'RadShield SingleDecay_228_90 1. mBq\n/LUXSim/source/set RadShield SingleDecay_224_88 1. mBq\n/LUXSim/source/set RadShield SingleDecay_220_86 1. mBq\n/LUXSim/source/set RadShield SingleDecay_216_84 1. mBq\n/LUXSim/source/set RadShield SingleDecay_212_82 1. mBq\n/LUXSim/source/set RadShield SingleDecay_212_83 1. mBq\n/LUXSim/source/set RadShield SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set RadShield SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "XENON" ]; then
	if [ "$ISOTOPE" = "Rn220" ]; then
		MACSTRB=$''
		SIR=$'/gps/particle ion \n/gps/ion 82 212 0 0\n/gps/ang/type iso\n/gps/energy 0. keV\n/gps/pos/type Volume\n/gps/pos/shape Cylinder\n/gps/pos/halfz 28. cm\n/gps/pos/radius 25. cm\n/gps/pos/centre 0. 0. 28. cm\n/gps/pos/confine LiquidXenon\n/grdm/nucleusLimits 212 212 82 82\n'
	fi
	if [ "$ISOTOPE" = "Rn222" ]; then
		MACSTRB=$''
		SIR=$'/gps/particle ion \n/gps/ion 82 214 0 0\n/gps/ang/type iso\n/gps/energy 0. keV\n/gps/pos/type Volume\n/gps/pos/shape Cylinder\n/gps/pos/halfz 28. cm\n/gps/pos/radius 25. cm\n/gps/pos/centre 0. 0. 28. cm\n/gps/pos/confine LiquidXenon\n/grdm/nucleusLimits 214 214 82 82\n'
	fi
	if [ "$ISOTOPE" = "Xe127" ]; then
		SIR=$'LiquidXenon SingleDecay_127_54 171. mBq'
	fi
	if [ "$ISOTOPE" = "Kr85" ]; then
		SIR=$'LiquidXenon SingleDecay_85_36 0.35 mBq'
	fi
fi

if [ "$SOURCE" = "MYLAR" ]; then
	if [ "$ISOTOPE" = "U238" ]; then
		SIR=$'MylarInsulation U238 1.724 Bq'
	fi
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'MylarInsulation Th232 546. mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'MylarInsulation Ra226 1. Bq'
	fi
	if [ "$ISOTOPE" = "K40" ]; then
		SIR=$'MylarInsulation SingleDecay_40_19 2.38 Bq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'MylarInsulation SingleDecay_232_90 1. mBq\n/LUXSim/source/set MylarInsulation SingleDecay_228_88 1. mBq\n/LUXSim/source/set MylarInsulation SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'MylarInsulation SingleDecay_228_90 1. mBq\n/LUXSim/source/set MylarInsulation SingleDecay_224_88 1. mBq\n/LUXSim/source/set MylarInsulation SingleDecay_220_86 1. mBq\n/LUXSim/source/set MylarInsulation SingleDecay_216_84 1. mBq\n/LUXSim/source/set MylarInsulation SingleDecay_212_82 1. mBq\n/LUXSim/source/set MylarInsulation SingleDecay_212_83 1. mBq\n/LUXSim/source/set MylarInsulation SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set MylarInsulation SingleDecay_208_81 .6406 mBq'
	fi
fi

if [ "$SOURCE" = "THERMALSHIELD" ]; then
	if [ "$ISOTOPE" = "U238" ]; then
		SIR=$'ThermalShield U238 1.724 Bq'
	fi
	if [ "$ISOTOPE" = "Th232" ]; then
		SIR=$'ThermalShield Th232 546. mBq'
	fi
	if [ "$ISOTOPE" = "Ra226" ]; then
		SIR=$'ThermalShield Ra226 1. Bq'
	fi
	if [ "$ISOTOPE" = "K40" ]; then
		SIR=$'ThermalShield SingleDecay_40_19 2.38 Bq'
	fi
	if [ "$ISOTOPE" = "Th232E" ]; then
		SIR=$'ThermalShield SingleDecay_232_90 1. mBq\n/LUXSim/source/set ThermalShield SingleDecay_228_88 1. mBq\n/LUXSim/source/set ThermalShield SingleDecay_228_89 1. mBq'
	fi
	if [ "$ISOTOPE" = "Th232L" ]; then
		SIR=$'ThermalShield SingleDecay_228_90 1. mBq\n/LUXSim/source/set ThermalShield SingleDecay_224_88 1. mBq\n/LUXSim/source/set ThermalShield SingleDecay_220_86 1. mBq\n/LUXSim/source/set ThermalShield SingleDecay_216_84 1. mBq\n/LUXSim/source/set ThermalShield SingleDecay_212_82 1. mBq\n/LUXSim/source/set ThermalShield SingleDecay_212_83 1. mBq\n/LUXSim/source/set ThermalShield SingleDecay_212_84 .3594 mBq\n/LUXSim/source/set ThermalShield SingleDecay_208_81 .6406 mBq'
	fi
fi


MACSTR1=$'/run/verbose 0\n/control/verbose 0\n/tracking/verbose 0\n/grdm/verbose 0\n/process/verbose 0\n\n/run/initialize\n/LUXSim/io/updateFrequency 1000 \n/LUXSim/io/outputDir '
MACSTR2=$OUTPUTLOCATION
MACSTR3=$'\n/LUXSim/io/outputName R4Bkg_'
MACSTR4=$SOURCE
MACSTR5=$'_'
MACSTR6=$ISOTOPE
MACSTR7=$'_\n/LUXSim/randomSeed '
MACSTR8=$LOOPNUMBER
MACSTR9=$'\n/LUXSim/detector/select 1_0Detector \n/LUXSim/detector/gridWires on \n/LUXSim/detector/cryoStand on \n/LUXSim/detector/muonVeto on \n\n/LUXSim/detector/topGridVoltage -1 kV\n/LUXSim/detector/anodeGridVoltage 3.5 kV\n/LUXSim/detector/gateGridVoltage -1.5 kV\n/LUXSim/detector/cathodeGridVoltage -10 kV\n/LUXSim/detector/bottomGridVoltage -2 kV\n/LUXSim/detector/printEFields\n\n/LUXSim/detector/update\n\n/LUXSim/detector/recordLevel LiquidXenon 2\n\n'
MACSTR0=$SIR
MACSTRA=$'\n/LUXSim/physicsList/useOpticalProcesses false\n\n/LUXSim/beamOn 250000\n\nexit'

MACFILENAME="$MACFILEBASE$MACFILEEXT"

echo "$MACSTR1$MACSTR2$MACSTR3$MACSTR4$MACSTR5$MACSTR6$MACSTR7$MACSTR8$MACSTR9$MACSTRB$MACSTR0$MACSTRA">$MACFILELOC/$MACFILENAME

