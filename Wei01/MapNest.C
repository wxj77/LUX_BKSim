//root -b -q -l "./Wei01/MapNest.C+(\"test3.root\", \"test4\",1)"

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
#include "WeiLookForGammaX.C"
#include "/nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/NEST.h"
#include "/nfs/farm/g/superb/u01/lz/wxj/LibNEST/libNEST/NEST.cxx"

using namespace std;


//functions in this file.
void eventLoop2(TTree* newtree, TTree* newtree2, int timeBin);
void MyMap(MyClusterEvent & cl, MyMapNestEvent & mn, FieldMap m);
void MyNest(MyClusterEvent & cl, MyMapNestEvent & mn, NEST nest, TH3F* h_binDef, TH3F* h_binFrDef);

//int MapNest(TString fOutName3, TString fOutName4);
int MapNest(TString fOutName3, TString fOutName4, int timeBin);


//map cluster to s2x, s2y, drift time.
void MyMap(MyClusterEvent & cl, MyMapNestEvent & mn, FieldMap m) {
   mn.iClusterNum2 = cl.iClusterNum;
   for (int j=0; j< cl.iClusterNum; j++){
            m.SetCurrentPoint(cl.fPositionX_cm[j],cl.fPositionY_cm[j],cl.fPositionZ_cm[j]);
            double params[4];
            m.InterpolateTrilinear(params); // params = {S2x, S2y, dt, E}
            mn.S2X_cm[j]    = params[0]; // cm
            mn.S2Y_cm[j]     = params[1]; // cm
            mn.Drift_us[j]    = params[2]; // us
     if (isnan(mn.S2X_cm[j])){
       std::cout << mn.S2X_cm[j]<<" "<<mn.Drift_us[j]<<" "<<std::endl;
     }
   }
}//MyMap()

//map cluster to s2x, s2y, drift time.
void MyNest(MyClusterEvent & cl, MyMapNestEvent & mn, NEST nest, TH3F* h_binDef, TH3F* h_binFrDef) {
//   mn.iClusterNum2 = cl.iClusterNum;
      mn.s1_raw    = 0; // 
      mn.s1c     = 0; // 
      mn.s2_raw    = 0; // 
      mn.s2c   =	0;
   for (int j=0; j< cl.iClusterNum; j++){
     if (isnan(mn.s1c)){
       std::cout << mn.S2X_cm[j]<<" "<<mn.Drift_us[j]<<" "<<std::endl;
      mn.S1_raw[j]    = -99999999.; // 
      mn.S1c[j]     = -99999999.; // 
      mn.S2_raw[j]    = -99999999.; // 
      mn.S2c[j]    =	-99999999.;
     }

      unsigned int globalBin = h_binDef->FindBin( mn.S2X_cm[j], mn.S2Y_cm[j], mn.Drift_us[j] );
      double electricField = h_binDef->GetBinContent( globalBin );
      double Fr = h_binFrDef->GetBinContent( globalBin );
						
      if( cl.iParticleID[j] == 11 ) nest.SetParticleType(1);
      else nest.SetParticleType(0);
      nest.SetEnergy( cl.fEnergyDep_keV[j] );
      nest.SetDriftLocation( mn.Drift_us[j] );
      nest.SetElectricField( electricField );
      nest.SetFr( Fr );
      nest.DetectorResponse();
      mn.S1_raw[j]    = nest.GetS1(); // 
      mn.S1c[j]     = nest.GetS1c(); // 
      mn.S2_raw[j]    = nest.GetS2(); // 
      mn.S2c[j]    =	nest.GetS2c();
      mn.s1_raw    += nest.GetS1(); // 
      mn.s1c     += nest.GetS1c(); // 
      mn.s2_raw    += nest.GetS2(); // 
      mn.s2c   +=	nest.GetS2c();
   }
}//MyNest()





//------------------------------------------------------------------------------
// Main event loop is contained here.
void eventLoop2(TTree* newtree, TTree* newtree2, int timeBin){
//a newtree2 to recorde mapped and nested results. 
  int TimeBin = timeBin;
//define field map;
  TString fieldMapName = timeBinDef(timeBin);
  TString TimeBinStr=TString::Format("%d",timeBin);
  char* env_p = std::getenv("BKSimTools");
  fieldMapName = TString::Format("%s/BkgSimulationTools/Run4FieldMaps/%s", env_p,fieldMapName.Data());
  std::cout << "field map name:" << fieldMapName.Data() <<endl;
  // Declare a FieldMap object that reads in the desired Lucie field map
  FieldMap m(fieldMapName.Data());
  MyClusterEvent cl;
  MyMapNestEvent mn;


// define nest
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
	
	//Get binning definition for model bins
	TH3F* h_binDef = modelBinDef(TimeBin);
	TH3F* h_binFrDef = modelBinFrDef(TimeBin);

  load_newtree(newtree, cl);

  set_newtree2(newtree2, mn);

  int numEntries = newtree->GetEntries();//2000;//
  for(int i = 0; i < numEntries; i++) {
    if (i%5000==0){
      std::cout << "Working on eventLoop, event"<<" "<<i<<" ."<<std::endl;
    }
    newtree->GetEntry(i);
    MyMap(cl, mn, m); //time bin mapping
    MyNest(cl, mn, nest, h_binDef, h_binFrDef);//time bin nest
    newtree2->Fill();
  }
}


//////////////////////////////////////////
////////////////////////////////////Main Function//////////////////////////////////////////

int MapNest(TString fOutName3, TString fOutName4, int timeBin) {

// This root script pares down the output of the energy deposition only sims so that the optical sims don't have to run as many unneccessary simulations.
// fName: File name for a txt that contains the directory of the root results for this simulation.
// fOutName1: A File contain a TChain that has the simulation results.
// fOutName2: A File that contain the counts and the file name for the simulation results. 
// fOutName3: A File that contain the clustered, mapped, nested result for the simulation results.
// fOutName4: A File that contain the clustered, mapped, nested result for the simulation results, for different timeBin.

  TStopwatch* clock = new TStopwatch();
  clock->Start();

// write newtree info, has output for cluster events.
  TFile* infile = new TFile(fOutName3.Data(), "read");
  TTree* newtree = (TTree*) infile->Get("tree");
  std::cout << "Num of events. " << newtree->GetEntries() <<std::endl;
  std::cout << "Done loading newtree!"<<" "<<clock->RealTime()<<" s."<<std::endl;


  
  if(1)//for (int timeBin=1; timeBin<=4; timeBin++)
  {
    TString fOutName4_tb = TString::Format("%s_tb%d.root", fOutName4.Data(), timeBin);
    TFile* outfile2 = new TFile(fOutName4_tb.Data(), "recreate");
    TTree* newtree2 = new TTree("tree2","LUXSim Map Nest result.");
    std::cout << "Done open newtree2!"<<" "<<clock->RealTime()<<" s."<<std::endl;
    eventLoop2( newtree, newtree2, timeBin);
    outfile2->cd();
    newtree2->Write();
    outfile2->Close();
  }
  
  std::cout << "Done write newtree2!"<<" "<<clock->RealTime()<<" s."<<std::endl;

  std::cout << "Done!"<<" "<<clock->RealTime()<<" s."<<std::endl;
  
//  newtree->Delete();
//  delete clock;
  return 1;




  

}//WeiLookForGammaX
