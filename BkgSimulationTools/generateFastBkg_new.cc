// To compile:
// g++ generateFastBkg.cc `root-config --cflags --libs` -o generateFastBkg	 
//
// To run:
// ./generateFastBkg <time bin> <inputfilename.root> [<outputfilename>]
//	
// adapt to new structurce libnest
 
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>	 
#include <numeric>			
#include <cmath>
#include <cassert>
#include <sstream>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TSystemDirectory.h"
#include "TList.h"
#include "TSystemFile.h"
//#include "NEST.h"
#include "modelBinDef.h"
#include "NEST.hh"
#include "detector.hh"

bool newLibNEST=1;

using namespace std;

double delta_z(vector<double> z);

int main(int argc, char** argv)
{
	if(argc != 5){
		cout<<"Invalid Input Parameters: ./generateFastBkg BkgFileDir BkgSrc BkgComp TB\n";
	 	return 0;
	}
	string BkgFileDir=argv[1];
	string BkgSrc=argv[2];
	string BkgComp=argv[3];
	string TimeBinStr=argv[4];
	int TimeBin=atoi(argv[4]);
	
	string inputFile = BkgFileDir+"/"+BkgSrc+"/"+BkgComp+"/TB"+TimeBinStr+"/"+BkgSrc+"_"+BkgComp+".C3PO";
	cout<<"InputFile = "<<inputFile<<endl;
	
	string outputFile = BkgFileDir+"/"+BkgSrc+"/"+BkgComp+"/TB"+TimeBinStr+"/"+BkgSrc+"_"+BkgComp+"_Nested.root";
	cout<<"OutputFile = "<<outputFile<<endl;
	TFile* out_file = new TFile( outputFile.c_str(), "RECREATE");
	TTree* out_tree = new TTree("events", "LUX Fast Bkg Events Tree");
	
	//Final ROOT File Branches----------------
	
	int br_iEvtN = -1; out_tree->Branch("iEvtN", &br_iEvtN, "iEvtN/I");
	int br_iParticleID = -1; out_tree->Branch("iParticleID", &br_iParticleID, "iParticleID/I");
	//real space positions
	float br_x_cm = -999.; out_tree->Branch("x_cm", &br_x_cm, "x_cm/F");
	float br_y_cm = -999.; out_tree->Branch("y_cm", &br_y_cm, "y_cm/F");
	float br_r_cm = -999.; out_tree->Branch("r_cm", &br_r_cm, "r_cm/F");
	float br_z_cm = -999.; out_tree->Branch("z_cm", &br_z_cm, "z_cm/F");
	float br_phi = -999.; out_tree->Branch("phi", &br_phi, "phi/F");
	//S2 space positions
	float br_x_s2_cm = -999.; out_tree->Branch("x_s2_cm", &br_x_s2_cm, "x_s2_cm/F");
	float br_y_s2_cm = -999.; out_tree->Branch("y_s2_cm", &br_y_s2_cm, "y_s2_cm/F");
	float br_r_s2_cm = -999.; out_tree->Branch("r_s2_cm", &br_r_s2_cm, "r_s2_cm/F");
	float br_dt_s2 = -999.; out_tree->Branch("dt_s2", &br_dt_s2, "dt_s2/F");
	float br_phi_s2 = -999.; out_tree->Branch("phi_s2", &br_phi_s2, "phi_s2/F");
	//Event energy
	float br_energy_keV = -999.; out_tree->Branch("energy_keV", &br_energy_keV, "energy_keV/F");
	//S1 info
	float br_s1_raw = -999.; out_tree->Branch("s1_raw", &br_s1_raw, "s1_raw/F");
	float br_s1c = -999.; out_tree->Branch("s1c", &br_s1c, "s1c/F");
	//S2 info
	float br_s2_raw = -999.; out_tree->Branch("s2_raw", &br_s2_raw, "s2_raw/F");
	float br_s2c = -999.; out_tree->Branch("s2c", &br_s2c, "s2c/F");
	float br_log10S2 = -999.; out_tree->Branch("log10S2", &br_log10S2, "log10S2/F");
	
	//Internal Variables---------------------
	int iEvtN=0, iParticleID=0;
	int previousEvtN=0;
	int counter = 0;
	double fEnergyDep_keV, fPositionX_cm, fPositionY_cm, fPositionZ_cm;
	double fPositionX_S2_cm, fPositionY_S2_cm, fDrift_S2_us;
	double last_column; //dummy var
	int nPoints; //dummy var
 // double x_cm, y_cm, r_cm, z_cm;
 // int particleType;
	
	vector<double> event_s1_raw, event_s1c, event_s2_raw, event_s2c, event_energy_keV;
	vector<double> event_x_cm, event_y_cm, event_z_cm, event_r_cm, event_phi;
	vector<double> event_x_s2_cm, event_y_s2_cm, event_dt_us;
	vector<int> event_particle_id;
	
	//Create NEST object and load in LUX detector parameters
	//inputs: particle type (ER==1, NR==0), energy (keV), electric field (V/cm), xenon density (g/cm^3), dift time (us)
if (!newLibNEST){
	const float density_lxe = 2.88; // g/cm3
	double electricField = 200.; // default
	double Fr = 0.01;
	//const float drift_velocity = 0.1501; // cm/us, run 3 value, hard coded in libNEST
	NEST nest(1, 2.0, electricField, density_lxe, -1);
	Detector luxDetector;
	//Load detector based on time bin input
	switch (TimeBin){
	
		 case 0: luxDetector.LUXSettings();
						 break;
		 case 1: luxDetector.LUXRun04PlanBSettingsTime1();
						 break;
		 case 2: luxDetector.LUXRun04PlanBSettingsTime2();
						 break;
		 case 3: luxDetector.LUXRun04PlanBSettingsTime3();
						 break;
		 case 4: luxDetector.LUXRun04PlanBSettingsTime4();
						 break;
		 default: cout<<"Not a valid time bin number!!! Running "
		 <<"with time bin 1 settings anyways..."<<endl;
							luxDetector.LUXRun04PlanBSettingsTime1();
	}
	nest.SetDetectorParameters(luxDetector);
}
else{

  NEST::NESTcalc n;
  double pos_z, driftTime, field;
  
  unsigned long int numEvts = atoi(argv[1]);

  string type ;
  INTERACTION_TYPE type_num;
  WIMP_spectrum_prep wimp_spectrum_prep; //used only in WIMP case
  if (type == "NR") type_num = NR;
  else if (type == "WIMP")
  {
    type_num = WIMP;
    wimp_spectrum_prep= WIMP_prep_spectrum(atof(argv[3]));
    numEvts = n.poisson_draw(wimp_spectrum_prep.integral * atof(argv[1]) * atof(argv[4]) / 1e-36);
  } else if (type == "B8")
  {
    type_num = B8;
    numEvts = n.poisson_draw(0.0026 * atof(argv[1]));
  } else if (type == "DD") type_num = DD;
  else if (type == "AmBe")type_num = AmBe;
  else if (type == "Cf") type_num = Cf;
  else if (type == "ion") type_num = ion;
  else if (type == "gamma")type_num = gammaRay;
  else if (type == "Kr83m")type_num=Kr83m;
  else if (type == "CH3T")type_num = CH3T;
  else type_num = beta;

  double eMin ;
  double eMax ;
  double rho = SetDensity(T_Kelvin);
  
  if ( type_num == Kr83m && eMin == 9.4 && eMax == 9.4 )
    fprintf(stdout, "t [ns]\t\tE [keV]\t\tfield [V/cm]\ttDrift [us]\tvert pos [mm]\tNph\tNe-\tS1_raw [PE]\tS1_Zcorr\tS1c_spike\tNe-X\tS2_rawArea\tS2_Zcorr [phd]\n");
  else
    fprintf(stdout, "E [keV]\t\tfield [V/cm]\ttDrift [us]\tvert pos [mm]\tNph\tNe-\tS1_raw [PE]\tS1_Zcorr\tS1c_spike\tNe-X\tS2_rawArea\tS2_Zcorr [phd]\n");
    
    double keV = -999;
    for (unsigned long int j = 0; j < numEvts; j++) {
      if (eMin == eMax) {
	keV = eMin;
      } else {
	switch (type_num) {
	case CH3T:
	  keV = CH3T_spectrum(eMin, eMax, n);
	  break;
	case B8: //normalize this to ~3500 / 10-ton / year, for E-threshold of 0.5 keVnr, OR 180 evts/t/yr/keV at 1 keV
	  keV = B8_spectrum(eMin, eMax, n);
	  break;
	case AmBe: //for ZEPLIN-III FSR from HA (Pal '98)
	  keV = AmBe_spectrum(eMin, eMax, n);
	  break;
	case Cf:
	  keV = Cf_spectrum(eMin, eMax, n);
	  break;
	case DD:
	  keV = DD_spectrum(eMin, eMax, n);
	  break;
	case WIMP:
          {          
          keV = WIMP_spectrum(wimp_spectrum_prep, atof(argv[3]),n);
          }
          break;
	default:
	  keV = eMin + (eMax - eMin) * n.rand_uniform();
	  break;
	}
      }
      
      if ( type_num != WIMP ) {
	if (keV > eMax) keV = eMax;
	if (keV < eMin) keV = eMin;
      }

    Z_NEW:
      if ( atof(argv[6]) == -1. ) // -1 means default, random location mode
	pos_z = 0. + ( liquidBorder - 0. ) * n.rand_uniform(); // initial guess
      else pos_z = atof(argv[6]);
      
      if ( atof(argv[5]) == -1. ) { // -1 means use poly position dependence
        field = efpoly[0] + efpoly[1] * pos_z +
          efpoly[2] * pow(pos_z,2.)+
          efpoly[3] * pow(pos_z,3.)+
          efpoly[4] * pow(pos_z,4.)+
	  efpoly[5] * pow(pos_z,5.); // note sixth term: this one is quintic
      }
      else field = atof(argv[5]);
      
      if ( field <= 0. ) cout << "\nWARNING: A LITERAL ZERO FIELD MAY YIELD WEIRD RESULTS. USE A SMALL VALUE INSTEAD.\n";
      
      vD = SetDriftVelocity(T_Kelvin,field);
      driftTime = ( liquidBorder - pos_z ) / vD; // (mm - mm) / (mm / us) = us
      if ( (driftTime > dt_max || driftTime < dt_min) && atof(argv[6]) == -1. )
	goto Z_NEW;
      
      NEST::YieldResult yields = n.GetYields(type_num,keV,rho,field);
      NEST::QuantaResult quanta = n.GetQuanta(yields,rho);
      vector<double> scint = GetS1(quanta.photons,n,pos_z);
      printf("%.6f\t%.6f\t%.6f\t%.6f\t%d\t%d\t",keV,field,driftTime,pos_z,quanta.photons,quanta.electrons);
      printf("%.6f\t%.6f\t%.6f\t", scint[2], scint[5], scint[7]);
      scint = GetS2(quanta.electrons,n,driftTime);
      printf("%i\t%.6f\t%.6f\n", (int)scint[0], scint[4], scint[7]);
    }
    
}	
	//Get binning definition for model bins
	TH3F* h_binDef = modelBinDef(TimeBin);
	TH3F* h_binFrDef = modelBinFrDef(TimeBin);
	
	//Open up the .dat file
	//loop over events in the file and for each event get: S1c,S2c,r,z
	//save those in a .root file
	std::string line;
	ifstream infile(inputFile.c_str());
	int iLine=0,iEvent=0;
	if(!infile){
		cout<<"Invalid Input File!!!!!\n";
	}
	while(!infile.eof()){
		if( iLine % 10000 == 0 ) cout<<"Reading line "<<iLine<<endl;
		infile	>>iEvtN>>iParticleID>>fEnergyDep_keV>>fPositionX_cm>>fPositionY_cm>>fPositionZ_cm
				>>fPositionX_S2_cm>>fPositionY_S2_cm>>fDrift_S2_us>>nPoints>>last_column;
		if(iLine == 0) previousEvtN = iEvtN; // Make previousEvtN the current event for the first line
		//getline(infile,line,'\n');
		
		//cout<<iEvtN<<" "<<iParticleID<<" "<<fEnergyDep_keV<<" "<<fPositionX_cm<<" "<<fPositionY_cm<<" "<<fPositionZ_cm
		//	  <<" "<<fPositionX_S2_cm<<" "<<fPositionY_S2_cm<<" "<<fDrift_S2_us<<" "<<nPoints<<" "<<last_column<<endl;
		//cout<<line<<endl;
		
		//else previousEvtN=0;
		
		//When event # switches, record previous event info
		if( iEvtN != previousEvtN){//|| i == numLines-1 ){
			if( delta_z(event_z_cm) < 0.65 ){ //single scatter cut
				br_iEvtN = previousEvtN;
				br_iParticleID = event_particle_id[0];
				//Real space coords
				br_x_cm = event_x_cm[0];
				br_y_cm = event_y_cm[0];
				br_r_cm = event_r_cm[0];
				br_z_cm = event_z_cm[0];
				br_phi = event_phi[0];
			
				//S2 space coords
				br_x_s2_cm = event_x_s2_cm[0];
				br_y_s2_cm = event_y_s2_cm[0];
				br_r_s2_cm = sqrt( pow( event_x_s2_cm[0], 2 ) + pow( event_y_s2_cm[0], 2 ) );
				br_phi_s2 = atan2( event_y_s2_cm[0], event_x_s2_cm[0] );
				br_dt_s2 = event_dt_us[0];
			
				//Event energy
				br_energy_keV = accumulate(event_energy_keV.begin(),event_energy_keV.end(),0.0);
			
				//Run NEST over the energy deps--------
			
				// find what model bin the cluster is in
				// to get electric field and Fr values
				unsigned int globalBin = h_binDef->FindBin( br_r_s2_cm, br_phi_s2, br_dt_s2 );
				electricField = h_binDef->GetBinContent( globalBin );
				Fr = h_binFrDef->GetBinContent( globalBin );
				
				for( unsigned int j=0; j<event_energy_keV.size(); j++ ){
			
					if( event_particle_id[j] == 11 ) nest.SetParticleType(1);
					else nest.SetParticleType(0);
				
					nest.SetEnergy( event_energy_keV[j] );
					nest.SetDriftLocation( br_dt_s2 );
					nest.SetElectricField( electricField );
					nest.SetFr( Fr );
				
					nest.DetectorResponse();

					event_s1_raw.push_back( nest.GetS1() );
					event_s1c.push_back( nest.GetS1c() );
			
					event_s2_raw.push_back( nest.GetS2() );
					event_s2c.push_back( nest.GetS2c() );
			
				}
			
				br_s1_raw = accumulate(event_s1_raw.begin(),event_s1_raw.end(),0.0);
				br_s1c = accumulate(event_s1c.begin(),event_s1c.end(),0.0);
			
				br_s2_raw = accumulate(event_s2_raw.begin(),event_s2_raw.end(),0.0);
				br_s2c = accumulate(event_s2c.begin(),event_s2c.end(),0.0);
				br_log10S2 = log10( br_s2c );
			
				//cout<<"Event #: "<< br_iEvtN <<endl;
				//cout<<"br_x_cm: "<< br_x_cm <<endl;
				//cout<<"br_y_cm: "<< br_y_cm <<endl;
				//cout<<"br_r_cm: "<< br_r_cm <<endl;
				//cout<<"br_z_cm: "<< br_z_cm <<endl;
				//cout<<"br_s1c: "<< br_s1c <<endl;
				//cout<<"br_s2c: "<< br_s2c <<endl;
				//cout<<endl;
			
				out_tree->Fill(); //Fill new tree with info if single scatter
				counter++;
			} // if single scatter
					 
			//clear vectors for next event
			event_s1_raw.clear(); event_s1c.clear();
			event_s2_raw.clear(); event_s2c.clear();
			event_energy_keV.clear();
			event_x_cm.clear(); event_y_cm.clear(); event_r_cm.clear(); event_z_cm.clear(); event_phi.clear();
			event_x_s2_cm.clear(); event_y_s2_cm.clear(); event_dt_us.clear();
			event_particle_id.clear();
			iEvent++;
		} // if event number switched
				
		//Assign Event Info
		event_x_cm.push_back( fPositionX_cm );
		event_y_cm.push_back( fPositionY_cm );
		event_r_cm.push_back(sqrt( pow(fPositionX_cm, 2) + pow(fPositionY_cm, 2) ));
		event_z_cm.push_back( fPositionZ_cm );
		
		event_x_s2_cm.push_back( fPositionX_S2_cm );
		event_y_s2_cm.push_back( fPositionY_S2_cm );
		event_dt_us.push_back( fDrift_S2_us );
		
		event_phi.push_back( atan2( fPositionY_cm , fPositionX_cm ) );
		event_energy_keV.push_back( fEnergyDep_keV );
		event_particle_id.push_back( iParticleID );
		
		previousEvtN = iEvtN;
		iLine++;		
	}//looping over lines in file
				
	//numLines = 0;
	infile.close();
	
	
	out_file->cd();
	out_tree->Write();
	out_file->Close();
	
	cout<< counter << " single scatters saved"<<endl;
	cout<<"Finished!"<<endl;
	
	return 0;
	
}

double delta_z(vector<double> z) {
	double min_z = 9999999.;
	double max_z = -9999999.;
	for (vector<double>::size_type i = 0; i != z.size(); i++) {
		if (z[i] > max_z) {
			max_z = z[i];
		}
		if (z[i] < min_z) {
			min_z = z[i];
		}
	}
	return (max_z - min_z);
}
