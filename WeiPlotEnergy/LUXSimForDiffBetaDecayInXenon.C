//how to run
//root -q -b -l "LUXSimForDiffBetaDecayInXenon.C(\"test.txt\", \"test.root\", \"test2\", \"^{214}Pb decay\")"

#ifndef LUXSimForDiffBetaDecayInXenon_C
#define LUXSimForDiffBetaDecayInXenon_C 1

int debug=-1;
int lograte=0;
double maxscaledown=1.e-5;
int drawfunction=1;

//-1: use all events.
//0: use maximum 500000 events.
//1: use maximum 2000 events.
//2: use maximum 200000 events.

//c lib
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <cstdlib>
#include <string.h>

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
#include "TDirectory.h"
#include "TStopwatch.h"
#include "TImage.h"
#include "TSystem.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TColor.h"
#include "TF1.h"
#include "TLatex.h"
#include "TGaxis.h"
#include "TAxis.h"
#include "TAttAxis.h"


//custom header
#ifndef tree_cxx
#include "tree.C"
#endif
#ifndef header_cxx
#include "header.C"
#endif

using namespace std;

double load_chain(TString txtFileList, TChain* chain)
{
  double numOfFiles=0;
  std::cout << "Loading file names from "<<txtFileList << std::endl;
  
  ifstream fileList(txtFileList);
  fileList.seekg(0, ios::beg);

  string file;
  if (fileList.is_open()) {
    while ( getline(fileList, file) ) {
      chain->AddFile(file.c_str());
      numOfFiles+=1;
      //cout<<numOfFiles<<endl;
      //std::cout << "Added to TChain: "<< file << std::endl;
    }
    fileList.close();
  }
  return numOfFiles;
}



bool inVolume (double r, double z, double rmin=0, double zmin=25, double rmax=0, double zmax=50.){
  bool rin=( (rmin<=r) &&(r<rmax) ) ;
  bool zin=( (zmin<=z) &&(z<zmax) ) ;
  return (rin && zin);
}

int LUXSimForDiffBetaDecayInXenon(TString txtFileList = "test.txt", TString fOutName="test.root", TString fOutfigName="test", TString Particle="", double numOfSpecies=1.){ 

  
  int WhichStyle =4;
  TStyle *lzStyle = new TStyle("lzStyle","LZ Style");

  Int_t FontStyle = 42;
  Float_t FontSizeLabel = 0.04;
  Float_t FontSizeTitle = 0.035;
  Float_t YOffsetTitle = 1.6;
 
  switch(WhichStyle) {
  case 1:
    FontStyle = 42;
    FontSizeLabel = 0.05;
    FontSizeTitle = 0.065;
    YOffsetTitle = 1.19;
    break;
  case 2:
    FontStyle = 42;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  case 3:
    FontStyle = 132;
    FontSizeLabel = 0.035;
    FontSizeTitle = 0.05;
    YOffsetTitle = 1.6;
    break;
  }

  lzStyle->SetFrameBorderMode(0);
  lzStyle->SetCanvasBorderMode(0);
  lzStyle->SetPadBorderMode(0);
  lzStyle->SetCanvasBorderSize(0);
  lzStyle->SetFrameBorderSize(0);
  lzStyle->SetDrawBorder(0);
  lzStyle->SetTitleBorderSize(0);

  lzStyle->SetPadColor(0);
  lzStyle->SetCanvasColor(0);
  lzStyle->SetStatColor(0);
  lzStyle->SetFillColor(0);

  lzStyle->SetEndErrorSize(4);
  lzStyle->SetStripDecimals(kFALSE);

  lzStyle->SetLegendBorderSize(0);
  lzStyle->SetLegendFont(FontStyle);

  // set the paper & margin sizes
  lzStyle->SetPaperSize(20, 26);
  lzStyle->SetPadTopMargin(0.2);
  lzStyle->SetPadBottomMargin(0.15);
  lzStyle->SetPadRightMargin(0.13); // 0.075 -> 0.13 for colz option
  lzStyle->SetPadLeftMargin(0.16);//to include both large/small font options
  
  // Fonts, sizes, offsets
  lzStyle->SetTextFont(FontStyle);
  lzStyle->SetTextSize(0.08);

  lzStyle->SetLabelFont(FontStyle, "x");
  lzStyle->SetLabelFont(FontStyle, "y");
  lzStyle->SetLabelFont(FontStyle, "z");
  lzStyle->SetLabelFont(FontStyle, "t");
  lzStyle->SetLabelSize(FontSizeLabel, "x");
  lzStyle->SetLabelSize(FontSizeLabel, "y");
  lzStyle->SetLabelSize(FontSizeLabel, "z");
  lzStyle->SetLabelOffset(0.015, "x");
  lzStyle->SetLabelOffset(0.015, "y");
  lzStyle->SetLabelOffset(0.015, "z");

  lzStyle->SetTitleFont(FontStyle, "x");
  lzStyle->SetTitleFont(FontStyle, "y");
  lzStyle->SetTitleFont(FontStyle, "z");
  lzStyle->SetTitleFont(FontStyle, "t");
  lzStyle->SetTitleSize(FontSizeTitle, "y");
  lzStyle->SetTitleSize(FontSizeTitle, "x");
  lzStyle->SetTitleSize(FontSizeTitle, "z");
  lzStyle->SetTitleOffset(1.14, "x");
  lzStyle->SetTitleOffset(YOffsetTitle, "y");
  lzStyle->SetTitleOffset(1.2, "z");

  lzStyle->SetTitleStyle(0);
  lzStyle->SetTitleFontSize(0.06);//0.08
  lzStyle->SetTitleFont(FontStyle, "pad");
  lzStyle->SetTitleBorderSize(0);
  lzStyle->SetTitleX(0.1f);
  lzStyle->SetTitleW(0.8f);

  // use bold lines and markers
  lzStyle->SetMarkerStyle(1);
  lzStyle->SetHistLineWidth( Width_t(2.5) );
  lzStyle->SetLineStyleString(2, "[12 12]"); // postscript dashes
  
  // get rid of X error bars and y error bar caps
  lzStyle->SetErrorX(0.001);
  
  // do not display any of the standard histogram decorations
  lzStyle->SetOptTitle(0);
  lzStyle->SetOptStat(0);
  lzStyle->SetOptFit(0);
  
  // put tick marks on top and RHS of plots
  lzStyle->SetPadTickX(1);
  lzStyle->SetPadTickY(1);
  
  // -- color --
  // functions blue
  lzStyle->SetFuncColor(600-4);

//  lzStyle->SetFillColor(1); // make color fillings (not white)
  // - color setup for 2D -
  // - "cold"/ blue-ish -
//  double red[]   = { 0.00, 0.09, 0.18, 0.09, 0.00 };
//  double green[] = { 0.01, 0.02, 0.39, 0.68, 0.97 };
//  double blue[]  = { 0.17, 0.39, 0.62, 0.79, 0.97 };
//  double stops[] = { 0.00, 0.02, 0.20, 0.60, 1.00 };
//  const Int_t NRGBs = 5;
  // - "warm" red-ish colors -
  //  double red[]   = {1.00, 1.00, 0.25 };
  //  double green[] = {1.00, 0.00, 0.00 };
  //  double blue[]  = {0.00, 0.00, 0.00 };
  //-grey inverted
    double red[]   = {1.00, 0.00 };
    double green[] = {1.00, 0.00};
    double blue[]  = {1.00, 0.00};
    double stops[]  = {0.00, 1.00};
const Int_t NRGBs = 2;
  

  const Int_t NCont = 255;

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  lzStyle->SetNumberContours(NCont);

/*
const int ncol=10;
double red[ncol];
double green[ncol];
double blue[ncol];
double stops[ncol];

double dcol = -.1/double(ncol);
double gray = 1;
for (int j = 0; j < ncol; j++) {
//   ...... Define color with RGB equal to : gray, gray, gray .......

   red[j]=gray;
   blue[j]=gray;
   green[j]=gray;
   stops[j]=double(j)/double(ncol-1);
   gray += dcol;
}
const int totcol=50;

TColor::CreateGradientColorTable(ncol, red,green,blue,stops,totcol);
lzStyle->SetNumberContours(totcol);*/

  // - Rainbow -
  //  lzStyle->SetPalette(1);  // use the rainbow color set
  //   lzStyle->SetPalette(52); //grey
  //https://root.cern.ch/doc/master/classTColor.html#TColor:SetPalette High quality predefined palettes


  // -- axis --
  lzStyle->SetStripDecimals(kFALSE); // don't do 1.0 -> 1
  //  TGaxis::SetMaxDigits(3); // doesn't have an effect 
  // no supressed zeroes!
  lzStyle->SetHistMinimumZero(kTRUE);
  


 
  gROOT->SetStyle("lzStyle");
  gROOT->ForceStyle(kTRUE);

  //---Finish setting plot style
  //---start do calculation of energy spectrum.

  bool drawSkin=true;
  bool fitAct=true;
  //double skinScale;

  double skinScale=1.; //--Wei Need to change

  //X0:center; X1; active volume; X2->Skin 

  TCut x0="fPositionX_cm < 10 && fPositionX_cm > -10";
  TCut y0="fPositionY_cm < 10 && fPositionY_cm > -10";
  TCut z0="fPositionZ_cm < 28+16 && fPositionZ_cm > 28-16";
  TCut r0="fPositionX_cm * fPositionX_cm + fPositionY_cm * fPositionY_cm<18*18"; //cm
  // volume in r=18cm dz=16*2cm; is 3.257e4 cm^3, rho=2.88 kg/cm^3 at 170K m= 93.81 kg

  TCut z1="fPositionZ_cm < 48.6 && fPositionZ_cm > 8.54";
  TCut r1="fPositionX_cm * fPositionX_cm + fPositionY_cm * fPositionY_cm<20.5*20.5"; //cm 
  // volume in r=20.5cm dz=[8.54, 48.6 ]cm; is 5.289e4 cm^3, rho=2.88 kg/cm^3 at 170K m=152.3kg
  //http://www.pd.infn.it/~conti/images/LXe/LXEdensity.jpg

  TCut z2="fPositionZ_cm < 5.6+48.72 && fPositionZ_cm > 5.6";
  TCut r2="fPositionX_cm * fPositionX_cm + fPositionY_cm * fPositionY_cm<25*25 && fPositionX_cm * fPositionX_cm + fPositionY_cm * fPositionY_cm>20*20"; //cm 
  // volume in r=20.5cm dz=[8.54, 48.6 ]cm; is 5.289e4 cm^3, rho=2.88 kg/cm^3 at 170K m=152.3kg
  //http://www.pd.infn.it/~conti/images/LXe/LXEdensity.jpg

  //simulate in r=25cm dz=[28-28, 28+28 ]cm; source volume = 3.167 e5 cm^3
  double rho = 2.88e-3; //kg/cm^3
  double pi=3.1415927;
  double massTot = pi*25*25*28*2*rho;

  double r0min=0, r0max=10, z0min=28-10, z0max=28+10;
  double mass0 = pi*(r0max*r0max - r0min*r0min)*(z0max-z0min)*rho;

  double r1min=0, r1max=20.5, z1min=8.54, z1max=48.6;
  double mass1 = pi*(r1max*r1max - r1min*r1min)*(z1max-z1min)*rho;

  double r2min=20.5, r2max=23, z2min=5.6, z2max=5.6+48.72;
  double mass2 = pi*(r2max*r2max - r2min*r2min)*(z2max-z2min)*rho;

  string sOutName = fOutName.Data();
  TStopwatch* clock = new TStopwatch();
  clock->Start();
  

/*
  TChain* theader = new TChain("header", "header");
  load_chain(txtFileList, theader);
  double nSimP=0;
  header h(theader);
  int hentries=theader->GetEntries();
  for (int ll=0; ll<hentries; ll++){
     theader->GetEntry(ii);
     nSimp+=h.iNumRecords;
}
 */

  TChain* ttree = new TChain("tree", "tree");
  double nFiles = load_chain(txtFileList, ttree); //number of simulation is 250000* nFiles, each file has a beamon of 250000
  double nSims =  250000* nFiles;
  cout<<"number of files simulated: "<< nFiles<<endl;
  cout<<"number of counts simulated: "<< nSims<<endl;
  cout<<"number of counts (per equilibruim activities) simulated: "<< nSims/numOfSpecies <<endl;
  //  TChain* theader = new TChain("header", "header");
  // load_chain(txtFileList, theader);


  
  tree e(ttree); // to be mapped to tree tree;
  //  header header_e(theader); // to be mapped to coin tree;

  std::cout<<"success load trees. "<<std::endl;

//-----Wei important, long run change number of sims
  double numOfSim= double (ttree->GetEntries());
  cout<<"fraction of recorded/simulated: " << numOfSim/nSims<<endl;
  if (debug==0) {if (numOfSim>500000.) {numOfSim = 500000.;}}
  if (debug==1) {if (numOfSim>2000.) {numOfSim = 2000.;}}
  if (debug==2) {if (numOfSim>200000.) {numOfSim = 200000.;}}
  std::cout<<"number of entry used: "<<numOfSim<<std::endl;




  TFile * outFile = new TFile(fOutName.Data(), "recreate");
  outFile->cd();


  TH1F* he_center              = new TH1F("he_center", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* he_fd               = new TH1F("he_fd", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* he_skin               = new TH1F("he_skin", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);

  TH1F* hsc_center              = new TH1F("hsc_center", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* hsc_fd               = new TH1F("hsc_fd", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* hsc_skin               = new TH1F("hsc_skin", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);

  TH1F* hgxsc_center              = new TH1F("hgxsc_center", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* hgxsc_fd               = new TH1F("hgxsc_fd", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* hgxsc_skin               = new TH1F("hgxsc_skin", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);

  TH1F* hall_center              = new TH1F("hall_center", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* hall_fd               = new TH1F("hall_fd", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* hall_skin               = new TH1F("hall_skin", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);


  TH2F* he_center_xy               = new TH2F("he_center_xy ", "; x [cm]; y [cm]", 100, -30, 30, 100, -30, 30);
  TH2F* hsc_center_xy               = new TH2F("hsc_center_xy ", "; x [cm]; y [cm]", 100, -30, 30, 100, 30, 30);
  TH2F* hgxsc_center_xy               = new TH2F("hgxsc_center_xy ", "; x [cm]; y [cm]", 100, -30, 30, 100, -30, 30);

  TH1F* hr100sc               = new TH1F("hr100sc", ";r^{2} [cm^{2}]; event rate at z center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 600, 0, 600);
  TH1F* hr50sc               = new TH1F("hr50sc", ";r^{2} [cm^{2}]; event rate at z center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 600, 0, 600);
  TH1F* hr20sc               = new TH1F("hr20sc", ";r^{2} [cm^{2}]; event rate at z center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 600, 0, 600);

  TH1F* hr100gxsc               = new TH1F("hr100gxsc", ";z [cm]; event rate at z center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 600, 0, 600);
  TH1F* hr50gxsc               = new TH1F("hr50gxsc", ";z [cm]; event rate at z center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 600, 0, 600);
  TH1F* hr20gxsc               = new TH1F("hr20gxsc", ";z [cm]; event rate at z center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 600, 0, 600);

  TH1F* hz100sc               = new TH1F("hz100sc", ";z [cm]; event rate at r center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 60, 0, 60);
  TH1F* hz50sc               = new TH1F("hz50sc", ";z [cm]; event rate at r center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 60, 0, 60);
  TH1F* hz20sc               = new TH1F("hz20sc", ";z [cm]; event rate at r center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 60, 0, 60);

  TH1F* hz100gxsc               = new TH1F("hz100gxsc", ";z [cm]; event rate at r center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 60, 0, 60);
  TH1F* hz50gxsc               = new TH1F("hz50gxsc", ";z [cm]; event rate at r center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 60, 0, 60);
  TH1F* hz20gxsc               = new TH1F("hz20gxsc", ";r^{2} [cm^{2}]; event rate at r center [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]", 60, 0, 60);

  TH2F* hrz50sc               = new TH2F("hrz50sc", ";r^{2} [cm^{2}];z [cm]; event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 600, 0, 600, 70, 0, 70);
  TH2F* hrz50gxsc               = new TH2F("hrz50gxsc", ";r^{2} [cm^{2}];z [cm]; event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 600, 0, 600, 70, 0, 70);


 for (int ii=0; ii<numOfSim; ii++){
    
    ttree->GetEntry(ii);
    if( e.iRecordSize <=0) {
    //  cout<<"no record in xenon"<<endl;
      continue;
    }
/*    bool wrongParticleName1=0; 
    if (Particle[2] != e.cPrimaryParName[2]) {wrongParticleName1=1;}
    if (Particle[3] != e.cPrimaryParName[3]) {wrongParticleName1=1;}
    if (Particle[4] != e.cPrimaryParName[4]) {wrongParticleName1=1;}
    if (Particle[6] != e.cPrimaryParName[0]) {wrongParticleName1=1;}
    if (Particle[7] != e.cPrimaryParName[1]) {wrongParticleName1=1;}
    bool wrongParticleName2=0; 
    if (Particle[2] != e.cPrimaryParName[2]) {wrongParticleName2=1;}
    if (Particle[3] != e.cPrimaryParName[3]) {wrongParticleName2=1;}
    if (Particle[5] != e.cPrimaryParName[0]) {wrongParticleName2=1;}
    if (Particle[6] != e.cPrimaryParName[1]) {wrongParticleName2=1;}
    bool wrongParticleName3=0; 
    if (Particle[2] != e.cPrimaryParName[1]) {wrongParticleName3=1;}
    if (Particle[4] != e.cPrimaryParName[0]) {wrongParticleName3=1;}

    if (wrongParticleName1 && wrongParticleName2 && wrongParticleName3) {cout<<"error on primary particle: you assign: "<<Particle.Data() <<"    simulation has: "<<e.cPrimaryParName<<endl;}
*/    // find single scatter above cathode. and double scatter with one energy deposit site above cathode
    //
    //clear value from previous loop.
    //1 for above cathode, below gate, 2 for below cathode.
    double ZMin = 999999999.;
    double ZMax = -999999999.;
    double ZMinAboveCat = 999999999.;
    double ZMaxAboveCat = -999999999.;
    double EAboveCat=0;
    double delZAboveCat=0;
    double ZMinBelowCat = 999999999.;
    double ZMaxBelowCat = -999999999.;
    double EBelowCat=0;
    double delZBelowCat=0;
    bool gX=0, gX1=0, gX2=0; //gamma X, there is energy deposition outside active region. gX1: there is event above cathode below gate, gX2: there is event below cathode
    bool sC=0; //single scatter, the delta Z of energy deposition is smaller than 0.6 cm;
    double XAboveCat=0;//average x, y above cathode by energy to get s2 location
    double XBelowCat=0;
    double YAboveCat=0;
    double YBelowCat=0;
    double ZAboveCat=0;
    double ZBelowCat=0;
    double Z2AboveCat=0;
    double Z2BelowCat=0; //average z^2

    double ETotal=0;

    for(int j = 0; j < e.iRecordSize; j++) { 
//      cout<<e.fPositionZ_cm[j] <<"pid"<<e.iParticleID[j] <<endl;
      if((e.iParticleID[j] == 22 || e.iParticleID[j] == 11 || e.iParticleID[j] == -11)){
	if (e.fPositionZ_cm[j] < ZMin) {ZMin=e.fPositionZ_cm[j];}
	if (e.fPositionZ_cm[j] > ZMax) {ZMax=e.fPositionZ_cm[j];}
	if (e.fPositionZ_cm[j] > 5.6 && e.fPositionZ_cm[j] < 53.92 ){
	  EAboveCat += e.fEnergyDep_keV[j]; ETotal += e.fEnergyDep_keV[j];
          gX1=1;
	  if (e.fPositionZ_cm[j] < ZMinAboveCat) {ZMinAboveCat=e.fPositionZ_cm[j];}
	  if (e.fPositionZ_cm[j] > ZMaxAboveCat) {ZMaxAboveCat=e.fPositionZ_cm[j];}
          XAboveCat+= e.fEnergyDep_keV[j]* e.fPositionX_cm[j]; //
          YAboveCat+= e.fEnergyDep_keV[j]* e.fPositionY_cm[j]; //
          ZAboveCat+= e.fEnergyDep_keV[j]* e.fPositionZ_cm[j]; //
          Z2AboveCat+= e.fEnergyDep_keV[j]* pow(e.fPositionZ_cm[j] , 2); //
	}
	else if (e.fPositionZ_cm[j] > 0 && e.fPositionZ_cm[j] <= 5.6 ){
//    cout<<endl<<endl<<"set gX flag 1 here."<<endl<<endl<<endl;
	  EBelowCat += e.fEnergyDep_keV[j]; ETotal += e.fEnergyDep_keV[j]; 
          gX2=1;
	  if (e.fPositionZ_cm[j] < ZMinBelowCat) {ZMinBelowCat=e.fPositionZ_cm[j];}
	  if (e.fPositionZ_cm[j] > ZMaxBelowCat) {ZMaxBelowCat=e.fPositionZ_cm[j];}
          XBelowCat+= e.fEnergyDep_keV[j]* e.fPositionX_cm[j]; //
          YBelowCat+= e.fEnergyDep_keV[j]* e.fPositionY_cm[j]; //
          ZBelowCat+= e.fEnergyDep_keV[j]* e.fPositionZ_cm[j]; //
          Z2BelowCat+= e.fEnergyDep_keV[j]* pow(e.fPositionZ_cm[j] , 2); //
	}  
      }
    }
    delZAboveCat =ZMaxAboveCat-ZMinAboveCat; 
    delZBelowCat =ZMaxBelowCat-ZMinBelowCat; 
    XAboveCat = XAboveCat/EAboveCat;
    XBelowCat = XBelowCat/EBelowCat;
    YAboveCat = YAboveCat/EAboveCat;
    YBelowCat = YBelowCat/EBelowCat;
    ZAboveCat = ZAboveCat/EAboveCat;
    ZBelowCat = ZBelowCat/EBelowCat;
    Z2AboveCat = Z2AboveCat/EAboveCat;
    Z2BelowCat = Z2BelowCat/EBelowCat;

    double rAboveCat = pow( pow(XAboveCat ,2)+  pow(YAboveCat ,2)  ,0.5) ;
    double s2width  = pow( Z2AboveCat - pow(ZAboveCat ,2)  ,0.5) ;
    sC = (s2width<0.6) || (delZAboveCat<0.6); //0.6 cm to be able to separate s1 s2 //single scatter at between gate grid and anode grid.
    gX = gX1 && gX2;
/*
    cout<<endl<<endl<<endl<<endl<<endl;
    cout<<rAboveCat<<endl;
    cout<<XAboveCat<<endl;
    cout<<YAboveCat<<endl;
    cout<<ZAboveCat<<endl;
    cout<< s2width<<endl;
    cout<<ZMinAboveCat<<endl;
    cout<<ZMaxAboveCat<<endl;
    cout<<ZMinBelowCat<<endl;
    cout<<ZMaxBelowCat<<endl;
    cout<<ETotal<<endl;
    cout<<e.fTotEDep<<endl;
    cout<<EAboveCat<<endl;
    cout<<gX<<"  "<<sC<<endl;
    cout<<endl<<endl<<endl<<endl<<endl;
    cout<<inV0<<endl;*/

    bool smalle100 = (EAboveCat<100.) && (EAboveCat>0);
    bool smalle50 = (EAboveCat<50.) && (EAboveCat>0);
    bool smalle20=(EAboveCat<20.) && (EAboveCat>0);
    bool inV0 = inVolume(rAboveCat, ZAboveCat, r0min, z0min, r0max, z0max);
    bool inV1 =inVolume(rAboveCat, ZAboveCat, r1min, z1min, r1max, z1max);
    bool inV2 = inVolume(rAboveCat, ZAboveCat, r2min, z2min, r2max, z2max);
    bool inVzcenter=inVolume(rAboveCat, ZAboveCat, 0., z0min, 25., z0max);
    bool inVrcenter=inVolume(rAboveCat, ZAboveCat, r0min, 0, r0max, 60);


    if ( inV0 && (!gX) && (!gX2) && (sC) && smalle100) {he_center->Fill(EAboveCat); he_center_xy->Fill(XAboveCat, YAboveCat);}
    if ( inV1 && (!gX) && (!gX2) && (sC) && smalle100) {he_fd->Fill(EAboveCat);}
    if ( inV2 && (!gX) && (!gX2) && (sC) && smalle100) {he_skin->Fill(EAboveCat);}

    if ( inV0 && (!gX2) && (sC) && smalle100) {hsc_center->Fill(EAboveCat);hsc_center_xy->Fill(XAboveCat, YAboveCat);}
    if ( inV1 && (!gX2) && (sC) && smalle100) {hsc_fd->Fill(EAboveCat);}
    if ( inV2 && (!gX2) && (sC) && smalle100) {hsc_skin->Fill(EAboveCat);}

    if ( inV0 && (sC) && smalle100) {hgxsc_center->Fill(EAboveCat);hgxsc_center_xy->Fill(XAboveCat, YAboveCat);}
    if ( inV1 && (sC) && smalle100) {hgxsc_fd->Fill(EAboveCat);}
    if ( inV2 && (sC) && smalle100) {hgxsc_skin->Fill(EAboveCat);}


    if ( inV0  ) {hall_center->Fill(EAboveCat);}
    if ( inV0  ) {hall_fd->Fill(EAboveCat);}
    if ( inV0  ) {hall_skin->Fill(EAboveCat);}
 

    if ( inVzcenter && (!gX2) && (sC) && smalle100 ) {hr100sc->Fill(XAboveCat*XAboveCat + YAboveCat* YAboveCat);}
    if ( inVzcenter && (!gX2) && (sC) && smalle50) {hr50sc->Fill(XAboveCat*XAboveCat + YAboveCat* YAboveCat);}
    if ( inVzcenter && (!gX2) && (sC) && smalle20) {hr20sc->Fill(XAboveCat*XAboveCat + YAboveCat* YAboveCat);}

    if ( inVzcenter && (sC) && smalle100 ) {hr100gxsc->Fill(XAboveCat*XAboveCat + YAboveCat* YAboveCat);}
    if ( inVzcenter && (sC) && smalle50) {hr50gxsc->Fill(XAboveCat*XAboveCat + YAboveCat* YAboveCat);}
    if ( inVzcenter && (sC) && smalle20) {hr20gxsc->Fill(XAboveCat*XAboveCat + YAboveCat* YAboveCat);}


    if ( inVrcenter && (!gX2) && (sC) && smalle100 ) {hz100sc->Fill(ZAboveCat);}
    if ( inVrcenter && (!gX2) && (sC) && smalle50) {hz50sc->Fill(ZAboveCat);}
    if ( inVrcenter && (!gX2) && (sC) && smalle20) {hz20sc->Fill(ZAboveCat);}

    if ( inVrcenter && (sC) && smalle100 ) {hz100gxsc->Fill(ZAboveCat);}
    if ( inVrcenter && (sC) && smalle50) {hz50gxsc->Fill(ZAboveCat);}
    if ( inVrcenter && (sC) && smalle20) {hz20gxsc->Fill(ZAboveCat);}
 
   if ((!gX2) && (sC) && smalle50) {hrz50sc->Fill(XAboveCat*XAboveCat + YAboveCat* YAboveCat, ZAboveCat);}
   if ((sC) && smalle50) {hrz50gxsc->Fill(XAboveCat*XAboveCat + YAboveCat* YAboveCat, ZAboveCat);}


  }

  //tree->Draw("fTotEDep>>he_center(100,0,100)", "fTotEDep<100"&& z0 && r0);
  //tree->Draw("fTotEDep>>he_fd(100,0,100)", "fTotEDep<100"&& z1 && r1);
  //tree->Draw("fTotEDep>>he_skin(100,0,100)", "fTotEDep<100"&& z2 && r2);


  double dayLength=24*60*60; //second
  double daymBq = dayLength *1.e-3;
  //double mDRU = 1.e-3;
  double fraction; //since the source is confined in xenon space so numOfSims are not uniformly distribute in massTot
  TCut fz1="fPrimaryParPosZ_mm< (28+16)*10 && fPrimaryParPosZ_mm > (28-16)*10";
  TCut fr1="fPrimaryParPosX_mm * fPrimaryParPosX_mm + fPrimaryParPosY_mm * fPrimaryParPosY_mm<100*100"; //cm
  double fr1min=0, fr1max=10, fz1min=(28-16), fz1max=(28+16);
  double fmass1 = pi*(fr1max*fr1max - fr1min*fr1min)*(fz1max-fz1min)*rho;

  TCut fz0="fPrimaryParPosZ_mm< (28+20)*10 && fPrimaryParPosZ_mm > (28-16)*10";
  TCut fr0="fPrimaryParPosX_mm * fPrimaryParPosX_mm + fPrimaryParPosY_mm * fPrimaryParPosY_mm<50*50"; //cm
  double fmass0 = pi*5.0*5*(16+20)*rho;
//  double k=ttree->GetEntries(fz0 && fr0)/fmass0;

  double counts1=0;
  for (int ii=0; ii<numOfSim; ii++){
    ttree->GetEntry(ii);
    if ( inVolume( pow(e.fPrimaryParPosX_mm[0] * e.fPrimaryParPosX_mm[0] + e.fPrimaryParPosY_mm[0] * e.fPrimaryParPosY_mm[0], 0.5), e.fPrimaryParPosZ_mm[0], fr1min*10., fz1min*10., fr1max*10., fz1max*10.) ) {counts1+=1;}
//    cout<< pow(e.fPrimaryParPosX_mm[0] * e.fPrimaryParPosX_mm[0] + e.fPrimaryParPosY_mm[0] * e.fPrimaryParPosY_mm[0], 0.5)<<endl;
//    cout<< e.fPrimaryParPosZ_mm[0]<<endl;
//    cout<<fr1min*10.<<"  "<< fz1min*10.<<"  "<< fr1max*10.<<"  "<< fz1max*10. <<endl;
  }
  fraction = 1./(counts1/fmass1);

//fraction=1./nSims;
//fraction=fraction*numOfSpecies;

/*  cout<<fraction<<endl;
  cout<<counts1<<endl;
  cout<<fmass1<<endl;*/


//  he_center->SetTitle(";Energy deposit between gate grid and cathode grid [keV];event rate per activity mass [(mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
  he_center->Scale(1./mass0*fraction*daymBq);
//  he_center->SetName("he_center");
  he_center->SetLineColor(kBlue+2);
//  he_fd->SetTitle(";Energy deposit between gate grid and cathode grid [keV];event rate per activity mass [(mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
  he_fd->Scale(1./mass1*fraction*daymBq);
//  he_fd->SetName("he_fd);
  he_fd->SetLineColor(kRed+2);
//  he_skin->SetTitle(";Energy deposit between gate grid and cathode grid [keV];event rate per activity mass [(mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
  he_skin->Scale(1./mass2*fraction*daymBq);
//  he_skin->SetName("he_skin");
  he_skin->SetLineColor(kGreen+2);
/*
  cout<<hsc_center->GetEntries()<<endl;
  cout<<hsc_fd->GetEntries()<<endl;
  cout<<hsc_skin->GetEntries()<<endl;
  cout<<1./mass0*fraction*daymBq<<endl;
  cout<<1./mass1*fraction*daymBq<<endl;
  cout<<1./mass2*fraction*daymBq<<endl;
*/
  hsc_center->Scale(1./mass0*fraction*daymBq);
  hsc_center->SetLineColor(kBlue+2);
  hsc_fd->Scale(1./mass1*fraction*daymBq);
  hsc_fd->SetLineColor(kRed+2);
  hsc_skin->Scale(1./mass2*fraction*daymBq);
  hsc_skin->SetLineColor(kGreen+2);

  hgxsc_center->Scale(1./mass0*fraction*daymBq);
  hgxsc_center->SetLineColor(kBlue+2-4);
  hgxsc_fd->Scale(1./mass1*fraction*daymBq);
  hgxsc_fd->SetLineColor(kRed+2-4);
  hgxsc_skin->Scale(1./mass2*fraction*daymBq);
  hgxsc_skin->SetLineColor(kGreen+2-4);

  hgxsc_center->SetLineWidth(1);
  hgxsc_fd->SetLineWidth(1);
  hgxsc_skin->SetLineWidth(1);


  hall_center->Scale(1./mass0*fraction*daymBq);
  hall_center->SetLineColor(kBlue+2);
  hall_fd->Scale(1./mass1*fraction*daymBq);
  hall_fd->SetLineColor(kRed+2);
  hall_skin->Scale(1./mass2*fraction*daymBq);
  hall_skin->SetLineColor(kGreen+2);


  hsc_center->Rebin(2);
  hsc_center->Scale(1./2);
  hsc_fd->Rebin(2);
  hsc_fd->Scale(1./2);
  hsc_skin->Rebin(2);
  hsc_skin->Scale(1./2);

  hgxsc_center->Rebin(2);
  hgxsc_center->Scale(1./2);
  hgxsc_fd->Rebin(2);
  hgxsc_fd->Scale(1./2);
  hgxsc_skin->Rebin(2);
  hgxsc_skin->Scale(1./2);

  hr100sc->Scale(1./(pi*(25.*25. - 0.*0.)*(z2max-z2min)*rho)*fraction*daymBq*600);//
  hr100sc->SetLineColor(kGreen+2);
  hr50sc->Scale(1./(pi*(25.*25. - 0.*0.)*(z2max-z2min)*rho)*fraction*daymBq*600);//
  hr50sc->SetLineColor(kBlue+2);
  hr20sc->Scale(1./(pi*(25.*25. - 0.*0.)*(z2max-z2min)*rho)*fraction*daymBq*600);//
  hr20sc->SetLineColor(kRed+2);

  hr100gxsc->Scale(1./(pi*(25.*25. - 0.*0.)*(z2max-z2min)*rho)*fraction*daymBq*600);//
  hr100gxsc->SetLineColor(kGreen+2-4);
  hr50gxsc->Scale(1./(pi*(25.*25. - 0.*0.)*(z2max-z2min)*rho)*fraction*daymBq*600);//
  hr50gxsc->SetLineColor(kBlue+2-4);
  hr20gxsc->Scale(1./(pi*(25.*25. - 0.*0.)*(z2max-z2min)*rho)*fraction*daymBq*600);//
  hr20gxsc->SetLineColor(kRed+2-4);

  hr100gxsc->SetLineWidth(1);
  hr50gxsc->SetLineWidth(1);
  hr20gxsc->SetLineWidth(1);


  hz100sc->Scale(1./(pi*(r0max*r0max - r0min*r0min)*(60.-0.)*rho)*fraction*daymBq*60);//
  hz100sc->SetLineColor(kGreen+2);
  hz50sc->Scale(1./(pi*(r0max*r0max - r0min*r0min)*(60.-0.)*rho)*fraction*daymBq*60);//
  hz50sc->SetLineColor(kBlue+2);
  hz20sc->Scale(1./(pi*(r0max*r0max - r0min*r0min)*(60.-0.)*rho)*fraction*daymBq*60);//
  hz20sc->SetLineColor(kRed+2);

  hz100gxsc->Scale(1./(pi*(r0max*r0max - r0min*r0min)*(60.-0.)*rho)*fraction*daymBq*60);//
  hz100gxsc->SetLineColor(kGreen+2-4);
  hz50gxsc->Scale(1./(pi*(r0max*r0max - r0min*r0min)*(60.-0.)*rho)*fraction*daymBq*60);//
  hz50gxsc->SetLineColor(kBlue+2-4);
  hz20gxsc->Scale(1./(pi*(r0max*r0max - r0min*r0min)*(60.-0.)*rho)*fraction*daymBq*60);//
  hz20gxsc->SetLineColor(kRed+2-4);

  hz100gxsc->SetLineWidth(1);
  hz50gxsc->SetLineWidth(1);
  hz20gxsc->SetLineWidth(1);

  hrz50sc->Scale(1./(pi*600*(70.-0.)*rho)*fraction*daymBq*600*70);//
  hrz50gxsc->Scale(1./(pi*600*(70.-0.)*rho)*fraction*daymBq*600*70);//

if (drawfunction){
  hsc_center->Fit("pol1");
  TF1* f0 = hsc_center->GetFunction("pol1");
  f0->SetLineColor(kBlue+2);
  TF1* f1 = new TF1();
  if (fitAct){
    hsc_fd->Fit("pol1");
    f1 = hsc_fd->GetFunction("pol1");
    f1->SetLineColor(kRed+2);
//    hsc_skin->Fit("pol2");
//    TF1* f2 = hsc_skin->GetFunction("pol2");
//    f2->SetLineColor(kRed+2);
  }
}



  //-----start drawing
  double hmean;
  double hmin;
  double hmax;
  double scalemax=1.25;
  const Int_t nq = 1;
        Double_t xq[nq];  // position where to compute the quantiles in [0,1]
        Double_t yq[nq];  // array to contain the quantiles
  xq[0]=0.05;
  TCanvas* c1 = new TCanvas("c1", ";Energy deposit between gate grid and cathode grid [keV];event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
  c1->Draw();
  if (lograte) {c1->SetLogy();}

  if (lograte) scalemax=10.;
  hmin=0;
  if (lograte) {
    hmin = 0.095*hsc_center->GetMinimum();   
    hmean = hsc_center->GetBinContent(1);
    //hmean = yq[0];
    if (hmin< hmean*0.01) {
         hmin=hmean*0.01;
        }

  }

//  hmin=-0.2*(hsc_center->GetMaximum() - hsc_center->GetMinimum() )+ hsc_center->GetMinimum();
  hmax= 1.25*hgxsc_skin->GetMaximum(); //1.3*(hsc_skin->GetMaximum() - hsc_center->GetMinimum() )+ hsc_center->GetMinimum();
   if (lograte)  {if (hmin<=maxscaledown*hmax) {
         hmin=maxscaledown*hmax;
        }}

  if (lograte) {hmax= scalemax*hmax;}
  hsc_center->GetYaxis()->SetRangeUser(hmin, hmax);

  hsc_center->Draw();
  
  hsc_fd->Draw("same");
  if (drawSkin) {
    hsc_skin->Scale(1./skinScale); //further scale down to plot on the same graph.
    hsc_skin->Draw("same");
  }

  hgxsc_center->Draw("same");
  
  hgxsc_fd->Draw("same");
  if (drawSkin) {
    hgxsc_skin->Scale(1./skinScale); //further scale down to plot on the same graph.
    hgxsc_skin->Draw("same");
  }




  TLegend* tl = new TLegend(0.15,0.8,0.85,0.9);
  tl->SetNColumns(3);
  tl->AddEntry(hsc_center, "center (SC)");
  tl->AddEntry(hsc_fd, "active volume (SC)");
  TString label= TString::Format("skin (SC)");// * %d",int(skinScale));
  if (drawSkin) {tl->AddEntry(hsc_skin, label.Data());}
  tl->AddEntry(hgxsc_center, "center (SC+GXSC)");
  tl->AddEntry(hgxsc_fd, "active volume (SC+GXSC)");
  label= TString::Format("skin (SC+GXSC)");// * %d",int(skinScale));
  if (drawSkin) {tl->AddEntry(hgxsc_skin, label.Data());}

  tl->SetTextSize(0.023);
  tl->SetFillColor(kWhite);
  tl->SetLineColor(kBlack);
  tl->SetTextColor(kBlack);
  tl->Draw("same");

  TString tcontent;
  if (drawfunction)
{
  tcontent= TString::Format("#splitline{p0: %.3e }{#splitline{p1: %.3e }{}}",f0->GetParameter(0), f0->GetParameter(1));
//  TLatex* tt = new TLatex(100*0.6, 0.05*(hmax - hmin )+ hmin, tcontent.Data());
  TLatex* tt = new TLatex(0.586, 0.185, tcontent.Data());
  tt->SetNDC(kTRUE);
  tt->SetTextColor(kBlue+2);   
  tt->SetTextFont(43);
  tt->SetTextSize(20);
  tt->Draw("same");
  if (fitAct){
  tcontent= TString::Format("#splitline{p0: %.3e }{#splitline{p1: %.3e }{}}",f1->GetParameter(0), f1->GetParameter(1));
//  TLatex* tt1 = new TLatex(100*0.2, 0.05*(hmax - hmin )+ hmin, tcontent.Data());
  TLatex* tt1 = new TLatex(0.302, 0.185, tcontent.Data());
  tt1->SetNDC(kTRUE);
  tt1->SetTextColor(kRed+2);   
  tt1->SetTextFont(43);
  tt1->SetTextSize(20);
  tt1->Draw("same");
  }
}
  tcontent= TString::Format("%s", Particle.Data());
//  TLatex* ttp = new TLatex(100*0.05, 0.85*(hmax - hmin )+ hmin, tcontent.Data());
  TLatex* ttp = new TLatex(0.1955, 0.7025, tcontent.Data());
  ttp->SetNDC(kTRUE);
  ttp->SetTextColor(kBlack);   
  ttp->SetTextFont(43);
  ttp->SetTextSize(30);
  ttp->Draw("same");

  TImage *img = TImage::Create();
  TString figname;
  figname = TString::Format("%srate.png", fOutfigName.Data()) ;
  c1->Update();  
  img->FromPad(c1);
  img->WriteImage(figname.Data());
  std::cout << figname.Data()<<std::endl;


//----rate vs r^2

  TCanvas* c2 = new TCanvas("c2", ";r^{2} [cm^{2}]; event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]");
  c2->Draw();
  c2->cd();


  hr100sc->Rebin(10);
  hr100sc->Scale(1./10);
  hr50sc->Rebin(10);
  hr50sc->Scale(1./10);
  hr20sc->Rebin(10);
  hr20sc->Scale(1./10);

  hr100gxsc->Rebin(10);
  hr100gxsc->Scale(1./10);
  hr50gxsc->Rebin(10);
  hr50gxsc->Scale(1./10);
  hr20gxsc->Rebin(10);
  hr20gxsc->Scale(1./10);

  if (lograte) {c2->SetLogy();}

  hmin=0;
  if (lograte) {
    hmin = 0.095*hr20sc->GetMinimum();   
    hmean = hr20sc->GetBinContent(1);
    //hmean = yq[0];
    if (hmin< hmean*0.01) {hmin=hmean*0.01;}
  }
//  hmin=-0.2*(hsc_center->GetMaximum() - hsc_center->GetMinimum() )+ hsc_center->GetMinimum();
  hmax=1.25*hr100gxsc->GetMaximum();//1.3*(hr100sc->GetMaximum() - hr100sc->GetMinimum() )+ hr100sc->GetMinimum();
   if (lograte)  {if (hmin<=maxscaledown*hmax) {
         hmin=maxscaledown*hmax;
        }}

  if (lograte) {hmax= scalemax*hmax;}
  hr100sc->GetYaxis()->SetRangeUser(hmin, hmax);
 
  hr100sc->Draw();
  hr50sc->Draw("same");
  hr20sc->Draw("same");
  hr100gxsc->Draw("same");
  hr50gxsc->Draw("same");
  hr20gxsc->Draw("same");

  tcontent= TString::Format("%s", Particle.Data());
//  TLatex* ttp2 = new TLatex(600*0.05, 0.85*(hmax - hmin )+ hmin, tcontent.Data());
  TLatex* ttp2 = new TLatex(0.1955, 0.7025, tcontent.Data());
  ttp2->SetNDC(kTRUE);
  ttp2->SetTextColor(kBlack);   
  ttp2->SetTextFont(43);
  ttp2->SetTextSize(30);
  ttp2->Draw("same");

  TLegend* tl2 = new TLegend(0.15,0.8,0.85,0.9);
  tl2->SetNColumns(3);
  tl2->AddEntry(hr100sc, "below 100 keV (SC)");
  tl2->AddEntry(hr50sc, "below 50 keV (SC)");
  tl2->AddEntry(hr20sc, "below 20 keV (SC)");
  tl2->AddEntry(hr100gxsc, "below 100 keV (SC+GXSC)");
  tl2->AddEntry(hr50gxsc, "below 50 keV (SC+GXSC)");
  tl2->AddEntry(hr20gxsc, "below 20 keV (SC+GXSC)");

  tl2->SetTextSize(0.023);
  tl2->SetFillColor(kWhite);
  tl2->SetLineColor(kBlack);
  tl2->SetTextColor(kBlack);
  tl2->Draw("same");

  TImage *img2 = TImage::Create();
  TString figname2;
  figname2 = TString::Format("%shr.png", fOutfigName.Data()) ;
  c2->Update();  
  img2->FromPad(c2);
  img2->WriteImage(figname2.Data());
  std::cout << figname2.Data()<<std::endl;





//----rate vs z

  TCanvas* c3 = new TCanvas("c3", ";z [cm]; event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]");
  c3->Draw();
  c3->cd();
 /*
  hz100sc->Rebin(2);
  hz100sc->Scale(1./2);
  hz50sc->Rebin(2);
  hz50sc->Scale(1./2);
  hz20sc->Rebin(2);
  hz20sc->Scale(1./2);

  hz100gxsc->Rebin(2);
  hz100gxsc->Scale(1./2);
  hz50gxsc->Rebin(2);
  hz50gxsc->Scale(1./2);
  hz20gxsc->Rebin(2);
  hz20gxsc->Scale(1./2);
*/
  if (lograte) {c3->SetLogy();}

  hmin=0;
  if (lograte) {
    hmin = 0.095*hz20sc->GetMinimum();   
    hmean = hz20sc->GetBinContent(30);
    //hmean = yq[0];
    if (hmin< hmean*0.01) {hmin=hmean*0.01;}
  }

//  hmin=-0.2*(hsc_center->GetMaximum() - hsc_center->GetMinimum() )+ hsc_center->GetMinimum();
  hmax=1.25*hz100gxsc->GetMaximum();//1.3*(hr100sc->GetMaximum() - hr100sc->GetMinimum() )+ hr100sc->GetMinimum();
   if (lograte)  {if (hmin<=maxscaledown*hmax) {
         hmin=maxscaledown*hmax;
        }}

  cout<<"hmin"<<hmin<<endl;
  cout<<"hmax"<<hmax<<endl;

  if (lograte) {hmax= scalemax*hmax;}
  hz100sc->GetYaxis()->SetRangeUser(hmin, hmax);
 
  hz100sc->Draw();
  hz50sc->Draw("same");
  hz20sc->Draw("same");
  hz100gxsc->Draw("same");
  hz50gxsc->Draw("same");
  hz20gxsc->Draw("same");

  tcontent= TString::Format("%s", Particle.Data());
//  TLatex* ttp3 = new TLatex(60*0.05, 0.85*(hmax - hmin )+ hmin, tcontent.Data());
  TLatex* ttp3 = new TLatex(0.1955, 0.7025, tcontent.Data());
  ttp3->SetNDC(kTRUE);
  ttp3->SetTextColor(kBlack);   
  ttp3->SetTextFont(43);
  ttp3->SetTextSize(30);
  ttp3->Draw("same");

  TLegend* tl3 = new TLegend(0.15,0.8,0.85,0.9);
  tl3->SetNColumns(3);
  tl3->AddEntry(hz100sc, "below 100 keV (SC)");
  tl3->AddEntry(hz50sc, "below 50 keV (SC)");
  tl3->AddEntry(hz20sc, "below 20 keV (SC)");
  tl3->AddEntry(hz100gxsc, "below 100 keV (SC+GXSC)");
  tl3->AddEntry(hz50gxsc, "below 50 keV (SC+GXSC)");
  tl3->AddEntry(hz20gxsc, "below 20 keV (SC+GXSC)");

  tl3->SetTextSize(0.023);
  tl3->SetFillColor(kWhite);
  tl3->SetLineColor(kBlack);
  tl3->SetTextColor(kBlack);
  tl3->Draw("same");

  TImage *img3 = TImage::Create();
  TString figname3;
  figname3 = TString::Format("%shz.png", fOutfigName.Data()) ;
  c3->Update();  
  img3->FromPad(c3);
  img3->WriteImage(figname3.Data());
  std::cout << figname3.Data()<<std::endl;


//----rate vs r^2 and z


  TBox* bcenter = new TBox(r0min*r0min, z0min, r0max*r0max, z0max);
  bcenter->SetLineColor(kBlue+2);
  bcenter->SetFillStyle(0); //transparent
  bcenter->SetLineWidth(3);
  bcenter->SetLineStyle(1);
  TBox* bfd = new TBox(r1min*r1min, z1min, r1max*r1max, z1max);
  bfd->SetLineColor(kRed+2);
  bfd->SetFillStyle(0); //transparent
  bfd->SetLineWidth(3);
  bfd->SetLineStyle(1);
  TBox* bskin = new TBox(r2min*r2min, z2min, r2max*r2max, z2max);
  bskin->SetLineColor(kGreen+2);
  bskin->SetFillStyle(0); //transparent
  bskin->SetLineWidth(3);
  bskin->SetLineStyle(1);





  double hrzmax = hrz50gxsc->GetBinContent(480,30)*1.3;
  double hrzmin = hrz50sc->GetBinContent(1,30)*0.7;
 //---single scatter
  TCanvas* c4 = new TCanvas("c4", "");
  c4->Draw();
  c4->cd();
  if (lograte) {c4->SetLogz();}
  if (lograte) {hrzmax=hrzmax*10; if (hrzmin<=hrzmax*maxscaledown) {hrzmin=hrzmax*maxscaledown;}}

  hrz50sc->Rebin2D(10, 1);
  hrz50sc->Scale(1./10/1);

  hrz50sc->GetZaxis()->SetRangeUser(hrzmin, hrzmax);
  hrz50sc->Draw("colz");

  bcenter->Draw("same");
  bfd->Draw("same");
  bskin->Draw("same");  


  tcontent= TString::Format("%s", Particle.Data());
//  TLatex* ttp4 = new TLatex(600*0.05, 0.85*(70 - 0 )+ 0, tcontent.Data());
  TLatex* ttp4 = new TLatex(0.1955, 0.7025, tcontent.Data());
  ttp4->SetNDC(kTRUE);
  ttp4->SetTextColor(kBlack);   
  ttp4->SetTextFont(43);
  ttp4->SetTextSize(30);
  ttp4->Draw("same");

  tcontent= TString::Format("event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]");
  TLatex* ttpn4 = new TLatex(0.6, 0.92, tcontent.Data());
  ttpn4->SetNDC(kTRUE);
  ttpn4->SetTextColor(kBlack);   
  ttpn4->SetTextFont(43);
  ttpn4->SetTextSize(15);
//  ttpn4->Draw("same");


  TLegend* tl4 = new TLegend(0.15,0.8,0.85,0.9);
  tl4->SetNColumns(3);
  tl4->AddEntry(bcenter, "#splitline{center (SC)}{below 50 keV}");
  tl4->AddEntry(bfd, "#splitline{active volume (SC)}{below 50 keV}");
  TString label4= TString::Format("#splitline{skin (SC)}{below 50 keV}");// * %d",int(skinScale));      
  if (drawSkin) {tl4->AddEntry(bskin, label4.Data());}

  tl4->SetTextSize(0.023);
  tl4->SetFillColor(kWhite);
  tl4->SetLineColor(kBlack);
  tl4->SetTextColor(kBlack);
  tl4->Draw("same");

  TImage *img4 = TImage::Create();
  TString figname4;
  figname4 = TString::Format("%shrz.png", fOutfigName.Data()) ;
  c4->Update();  
  img4->FromPad(c4);
  img4->WriteImage(figname4.Data());
  std::cout << figname4.Data()<<std::endl;

//---single scatter +gammax

  TCanvas* c5 = new TCanvas("c5", "");
  c5->Draw();
  c5->cd();
  if (lograte) {c5->SetLogz();}
  if (lograte) {hrzmax=hrzmax*10; if (hrzmin<=hrzmax*maxscaledown) {hrzmin=hrzmax*maxscaledown;}}


  hrz50gxsc->Rebin2D(10, 1);
  hrz50gxsc->Scale(1./10/1);

  hrz50gxsc->GetZaxis()->SetRangeUser(hrzmin, hrzmax);
  hrz50gxsc->Draw("colz");

  bcenter->Draw("same");
  bfd->Draw("same");
  bskin->Draw("same");  


  tcontent= TString::Format("%s", Particle.Data());
//  TLatex* ttp5 = new TLatex(600*0.05, 0.85*(70 - 0 )+ 0, tcontent.Data());
  TLatex* ttp5 = new TLatex(0.1955, 0.7025, tcontent.Data());
  ttp5->SetNDC(kTRUE);
  ttp5->SetTextColor(kBlack);   
  ttp5->SetTextFont(43);
  ttp5->SetTextSize(30);
  ttp5->Draw("same");

  tcontent= TString::Format("event rate [cts (mBq /kg)^{-1} kg^{-1} day^{-1}]");
  TLatex* ttpn5 = new TLatex(0.6, 0.92, tcontent.Data());
  ttpn5->SetNDC(kTRUE);
  ttpn5->SetTextColor(kBlack);   
  ttpn5->SetTextFont(43);
  ttpn5->SetTextSize(15);
//  ttpn5->Draw("same");

  TLegend* tl5 = new TLegend(0.15,0.8,0.85,0.9);
  tl5->SetNColumns(3);
  tl5->AddEntry(bcenter, "#splitline{center (SC+GXSC)}{below 50 keV}");
  tl5->AddEntry(bfd, "#splitline{active volume (SC+GXSC)}{below 50 keV}");
  TString label5= TString::Format("#splitline{skin (SC+GXSC)}{below 50 keV}");// * %d",int(skinScale));      
  if (drawSkin) {tl5->AddEntry(bskin, label5.Data());}

  tl5->SetTextSize(0.023);
  tl5->SetFillColor(kWhite);
  tl5->SetLineColor(kBlack);
  tl5->SetTextColor(kBlack);
  tl5->Draw("same");

  TImage *img5 = TImage::Create();
  TString figname5;
  figname5 = TString::Format("%shrzgxsc.png", fOutfigName.Data()) ;
  c5->Update();  
  img5->FromPad(c5);
  img5->WriteImage(figname5.Data());
  std::cout << figname5.Data()<<std::endl;


//------write output.
  outFile->Write();
//  hsc_center->Write();
//  hsc_fd->Write();
//  hsc_skin->Write();

  return 1;
}

#endif
