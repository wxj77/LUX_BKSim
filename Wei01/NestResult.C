#ifndef NESTRESULT_C
#define NESTRESULT_C 1

//c lib
#include <iostream>
#include <fstream>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <string.h>
#include <cstring>

//root lib
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TFile.h"
#include "TStopwatch.h"

//custom header
#include "../BkgSimulationTools/FieldMap.h"
#include "../BkgSimulationTools/modelBinDef.h"
//#include "WeiLookForGammaX.C"
#include "MapNest.C"
//#include "/nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/NEST.h"
//#include "/nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/NEST.cxx"
int NestResult(TString fOutName3, TString fOutName4, TString outputFile, int timeBin);

int NestResult(TString fOutName3, TString fOutName4, TString outputFile, int timeBin){
	TFile* out_file = new TFile( outputFile.Data(), "RECREATE");
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

	float br_Fr = -999.; out_tree->Branch("Fr", &br_Fr, "Fr/F");
	float br_Efield = -999.; out_tree->Branch("Efield", &br_Efield, "Efield/F");
	float br_Energy2 = -999.; out_tree->Branch("Energy2", &br_Energy2, "Energy2/F");

// fOutName3: A File that contain the clustered, mapped, nested result for the simulation results.
// fOutName4: A File that contain the clustered, mapped, nested result for the simulation results, for different timeBin.


  TStopwatch* clock = new TStopwatch();
  clock->Start();

// load file.
  TFile* infile = new TFile(fOutName3.Data(), "read");
  TTree* newtree = (TTree*) infile->Get("tree");
  std::cout << "Num of events. " << newtree->GetEntries() <<std::endl;
  std::cout << "Done loading newtree!"<<" "<<clock->RealTime()<<" s."<<std::endl;

  TString fOutName4_tb = TString::Format("%s_tb%d.root", fOutName4.Data(), timeBin);
  TFile* infile2 = new TFile(fOutName4_tb.Data(), "read");
  newtree->AddFriend("tree2", infile2);

  MyClusterEvent cl; MyMapNestEvent mn;

  load_newtree(newtree, cl);

  load_newtree2(newtree, mn);
///////////////
// main event loop.
	newtree->Print();

  int numEntries = newtree->GetEntries();//2000;//
  for(int i = 0; i < numEntries; i++) {
//	cout<< " single scatters saved" <<cl.Energy2 <<endl; 
    if (i%50000==0){
      std::cout << "Working on eventLoop, event"<<" "<<i<<" ."<<std::endl;
    }
    newtree->GetEntry(i);
    if ((cl.deltaZ > -.5) && (cl.deltaZ<.65) ){ //single scatter above cathode.
       
	br_iEvtN = i; 
	br_iParticleID = cl.iParticleID[0]; //Wei, need to change this 
	//real space positions
	br_x_cm = cl.posX;
	br_y_cm = cl.posY; 
	br_r_cm = sqrt(cl.posX*cl.posX+cl.posY*cl.posY); 
	br_z_cm = cl.posZ; 
	br_phi = atan2(cl.posY, cl.posX); 
	//S2 space positions
	br_x_s2_cm = mn.S2X_cm[0]; 
	br_y_s2_cm = mn.S2Y_cm[0]; 
	br_r_s2_cm = sqrt(mn.S2X_cm[0]*mn.S2X_cm[0]+mn.S2Y_cm[0]*mn.S2Y_cm[0]); 
	br_dt_s2 = mn.Drift_us[0]; 
	br_phi_s2 = atan2(mn.S2Y_cm[0], mn.S2X_cm[0]); 
	//Event energy
	br_energy_keV = cl.fTotEDep; 
	//S1 info
	br_s1_raw = mn.s1_raw; 
	br_s1c = mn.s1c; 
	//S2 info
	br_s2_raw = mn.s2_raw; 
	br_s2c = mn.s2c; 
	br_log10S2 = log10(mn.s2c); 

	br_Fr = -999.; //Wei, need to change this 
	br_Efield = -999.; //Wei, need to change this 

	br_Energy2=cl.Energy2;
        out_tree->Fill();
    }
  }


	out_file->cd();
	
	cout<< out_tree->GetEntries() << " single scatters (total)saved"<<endl;
        cout<< out_tree->GetEntries("Energy2>1.e-4") << " single scatters (with energy deposition below cathode) saved"<<endl;

	out_tree->Write();
	out_file->Close();

	cout<<"Finished!"<<endl;
 return 1; 
}

#endif
