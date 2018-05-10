//how to run
//root -q -b -l "LUXSimForDiffBetaDecayInXenon.C(\"test.txt\")"


#ifndef LUXSimForDiffBetaDecayInXenon_C
#define LUXSimForDiffBetaDecayInXenon_C 1

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
#include "The_fdF.h"
#include "The_skinF.h"
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

void load_chain(TString txtFileList, TChain* chain)
{
  std::cout << "Loading file names from "<<txtFileList << std::endl;
  
  ifstream fileList(txtFileList);
  fileList.seekg(0, ios::beg);

  string file;
  if (fileList.is_open()) {
    while ( getline(fileList, file) ) {
      chain->AddFile(file.c_str());
      std::cout << "Added to TChain: "<< file << std::endl;
    }
    fileList.close();
  }
}


bool inVolume (double r, double z, double rmin=0, double zmin=25, double rmax=0, double zmax=50.){
  bool rin=( (rmin<=r) &&(r<rmax) ) ;
  bool zin=( (zmin<=z) &&(z<zmax) ) ;
  return (rin && zin);
}

int LUXSimForDiffBetaDecayInXenon(TString txtFileList = "test.txt", TString fOutName="test.root", TString fOutfigName="test"){
  int WhichStyle =1;
  TStyle *lzStyle = new TStyle("lzStyle","LZ Style");

  Int_t FontStyle = 22;
  Float_t FontSizeLabel = 0.035;
  Float_t FontSizeTitle = 0.05;
  Float_t YOffsetTitle = 1.3;
 
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
  lzStyle->SetPadTopMargin(0.1);
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
  lzStyle->SetMarkerStyle(6);
  lzStyle->SetHistLineWidth( Width_t(1.5) );
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

  lzStyle->SetFillColor(1); // make color fillings (not white)
  // - color setup for 2D -
  // - "cold"/ blue-ish -
  double red[]   = { 0.00, 0.09, 0.18, 0.09, 0.00 };
  double green[] = { 0.01, 0.02, 0.39, 0.68, 0.97 };
  double blue[]  = { 0.17, 0.39, 0.62, 0.79, 0.97 };
  // - "warm" red-ish colors -
  //  double red[]   = {1.00, 1.00, 0.25 };
  //  double green[] = {1.00, 0.00, 0.00 };
  //  double blue[]  = {0.00, 0.00, 0.00 };

  double stops[] = { 0.00, 0.02, 0.20, 0.60, 1.00 };
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  lzStyle->SetNumberContours(NCont);

  // - Rainbow -
  //  lzStyle->SetPalette(1);  // use the rainbow color set

  // -- axis --
  lzStyle->SetStripDecimals(kFALSE); // don't do 1.0 -> 1
  //  TGaxis::SetMaxDigits(3); // doesn't have an effect 
  // no supressed zeroes!
  lzStyle->SetHistMinimumZero(kTRUE);
  
  
  gROOT->SetStyle("lzStyle");
  gROOT->ForceStyle(kTRUE);

  //---Finish setting plot style
  //---start do calculation of energy spectrum.

  bool drawSkin=false;
  bool fitAct=false;
  //double skinScale;


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

  double r0min=0, r0max=18, z0min=28-16, z0max=28+16;
  double mass0 = pi*(r0max*r0max - r0min*r0min)*(z0max-z0min)*rho;

  double r1min=0, r1max=20.5, z1min=8.54, z1max=48.6;
  double mass1 = pi*(r1max*r1max - r1min*r1min)*(z1max-z1min)*rho;

  double r2min=20.5, r2max=23, z2min=5.6, z2max=5.6+48.72;
  double mass2 = pi*(r2max*r2max - r2min*r2min)*(z2max-z2min)*rho;

  string sOutName = fOutName.Data();
  TStopwatch* clock = new TStopwatch();
  clock->Start();
  
  TChain* ttree = new TChain("tree", "tree");
  load_chain(txtFileList, ttree);

  //  TChain* theader = new TChain("header", "header");
  // load_chain(txtFileList, theader);
  std::cout<<ttree->GetEntries()<<std::endl;

  
  tree e(ttree); // to be mapped to tree tree;
  //  header header_e(theader); // to be mapped to coin tree;

  std::cout<<"success load trees. "<<std::endl;

  The_fdF* he_center              = new The_fdF("he_center", ";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  The_fdF* he_fd               = new The_fdF("he_fd", ";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  The_fdF* he_skin               = new The_fdF("he_skin", ";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);



  double numOfSim= double (ttree->GetEntries());
  for (int ii=0; ii<numOfSim; ii++){
    ttree->GetEntry(ii);
    // find single scatter above cathode. and double scatter with one energy deposit site above cathode
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
    bool gX=0; //gamma X, there is energy deposition outside active region.
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
	  if (e.fPositionZ_cm[j] < ZMinAboveCat) {ZMinAboveCat=e.fPositionZ_cm[j];}
	  if (e.fPositionZ_cm[j] > ZMaxAboveCat) {ZMaxAboveCat=e.fPositionZ_cm[j];}
          XAboveCat+= e.fEnergyDep_keV[j]* e.fPositionX_cm[j]; //
          YAboveCat+= e.fEnergyDep_keV[j]* e.fPositionY_cm[j]; //
          ZAboveCat+= e.fEnergyDep_keV[j]* e.fPositionZ_cm[j]; //
          Z2AboveCat+= e.fEnergyDep_keV[j]* pow(e.fPositionZ_cm[j] , 2); //
	}
	else if (e.fPositionZ_cm[j] > 0 && e.fPositionZ_cm[j] <= 5.6 ){
	  EBelowCat += e.fEnergyDep_keV[j]; ETotal += e.fEnergyDep_keV[j]; 
          gX=1;
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
    sC = (s2width<0.6) || (delZAboveCat<0.6); //0.6 cm to be able to separate s1 s2
/*
    cout<<rAboveCat<<endl;
    cout<< s2width<<endl;
    cout<<ZMinAboveCat<<endl;
    cout<<ZMaxAboveCat<<endl;
    cout<<ETotal<<endl;
    cout<<e.fTotEDep<<endl;
    cout<<EAboveCat<<endl;
    cout<<endl<<endl<<endl<<endl<<endl;
*/
    if ( inVolume(rAboveCat, ZAboveCat, r0min, z0min, r0max, z0max) && (!gX) && (sC) ) {he_center->Fill(EAboveCat);}
    if ( inVolume(rAboveCat, ZAboveCat, r1min, z1min, r1max, z1max) && (!gX) && (sC) ) {he_fd->Fill(EAboveCat);}
    if ( inVolume(rAboveCat, ZAboveCat, r2min, z2min, r2max, z2max) && (!gX) && (sC) ) {he_skin->Fill(EAboveCat);}

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
  double fmass1 = pi*10.0*10.0*16*2*rho;

  TCut fz0="fPrimaryParPosZ_mm< (28+20)*10 && fPrimaryParPosZ_mm > (28-16)*10";
  TCut fr0="fPrimaryParPosX_mm * fPrimaryParPosX_mm + fPrimaryParPosY_mm * fPrimaryParPosY_mm<50*50"; //cm
  double fmass0 = pi*5.0*5*(16+20)*rho;
  double k=ttree->GetEntries(fz0 && fr0)/fmass0;

  fraction = 1./(ttree->GetEntries(fr1&&fz1)/fmass1)*(numOfSim/massTot);

  he_center->SetTitle(";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
  he_center->Scale(massTot/mass0/numOfSim*fraction*daymBq);
  he_center->SetName("center");
  he_center->SetLineColor(kBlue);
  he_fd->SetTitle(";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
  he_fd->Scale(massTot/mass1/numOfSim*fraction*daymBq);
  he_fd->SetName("activeVolume");
  he_fd->SetLineColor(kRed);
  he_skin->SetTitle(";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
  he_skin->Scale(massTot/mass2/numOfSim*fraction*daymBq);
  he_skin->SetName("skin");
  he_skin->SetLineColor(kBlack);

  he_center->Rebin(4);
  he_center->Scale(1./4);
  he_fd->Rebin(4);
  he_fd->Scale(1./4);
  he_skin->Rebin(4);
  he_skin->Scale(1./4);


  he_center->Fit("pol2");
  TF1* f0 = he_center->GetFunction("pol2");
  f0->SetLineColor(kBlue);
  if (fitAct){
    he_fd->Fit("pol1");
    TF1* f1 = he_fd->GetFunction("pol1");
    f1->SetLineColor(kRed);
  }




  //-----start drawing


  TCanvas* c1 = new TCanvas("c1", ";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
  c1->Draw();

  double skinScale=2.; //--Wei Need to change

  TLegend* tl = new TLegend(0.7,0.8,0.9,0.9);
  tl->AddEntry(he_center, "center");
  tl->AddEntry(he_fd, "active volume");
  TString label= TString::Format("skin * %d",int(skinScale));
  if (drawSkin) {tl->AddEntry(he_skin, label.Data());}
  he_center->Draw();
  he_fd->Draw("same");
  if (drawSkin) {
    he_skin->Scale(1./skinScale); //further scale down to plot on the same graph.
    he_skin->Draw("same");
  }
  tl->Draw("same");

  TString tcontent;
  tcontent= TString::Format("#splitline{p0: %.3e }{#splitline{p1: %.3e }{p2: %.3e}}",f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2));
  TLatex* tt = new TLatex(100*0.6, 0.15*(he_center->GetMaximum() - he_center->GetMinimum() )+ he_center->GetMinimum(), tcontent.Data());
  tt->SetTextColor(kBlue);   
  tt->SetTextFont(43);
  tt->SetTextSize(25);
  tt->Draw("same");


  TImage *img = TImage::Create();
  TString figname;
  figname = TString::Format("test.png") ;
  c1->Update();  
  img->FromPad(c1);
  img->WriteImage(figname.Data());
  std::cout << figname.Data()<<std::endl;


  return 1;
}

#endif
