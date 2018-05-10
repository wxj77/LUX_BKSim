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


bool inCenter (tree & e, double rmin=0, double zmin=25, double rmax=0, double zmax=50.){return 1;}

int LUXSimForDiffBetaDecayInXenon(TString txtFileList = "test.txt", TString fOutName=""){
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
double mass0 = pi*18*18*16*2*rho;
double mass1 = pi*20.5*20.5*(48.6-8.54)*rho;
double mass2 = pi*23*23*(48.72)*rho - pi*20.5*20.5*(48.6-8.54)*rho;




  string sOutName = fOutName.Data();
  TStopwatch* clock = new TStopwatch();
  clock->Start();
  
  TChain* ttree = new TChain("tree", "tree");
  load_chain(txtFileList, ttree);

//  TChain* theader = new TChain("header", "header");
 // load_chain(txtFileList, theader);
  std::cout<<ttree->GetEntries()<<std::endl;

  
  tree tree_e(ttree); // to be mapped to tree tree;
//  header header_e(theader); // to be mapped to coin tree;

  std::cout<<"success load trees. "<<std::endl;

  TH1F* h0              = new TH1F("he_center", ";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* h1               = new TH1F("he_fd", ";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);
  TH1F* h2               = new TH1F("he_skin", ";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]", 100, 0, 100);

double numOfSim= double (ttree->GetEntries());
for (int ii=0; ii<numOfSim; ii++){
  ttree->GetEntry(ii);
  if ( inCenter(tree_e) ) {h0->Fill(tree_e.fTotEDep);}

}

//tree->Draw("fTotEDep>>h0(100,0,100)", "fTotEDep<100"&& z0 && r0);
//tree->Draw("fTotEDep>>h1(100,0,100)", "fTotEDep<100"&& z1 && r1);
//tree->Draw("fTotEDep>>h2(100,0,100)", "fTotEDep<100"&& z2 && r2);


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

h0->SetTitle(";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
h0->Scale(massTot/mass0/numOfSim*fraction*daymBq);
h0->SetName("center");
h0->SetLineColor(kBlue);
h1->SetTitle(";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
h1->Scale(massTot/mass1/numOfSim*fraction*daymBq);
h1->SetName("activeVolume");
h1->SetLineColor(kRed);
h2->SetTitle(";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
h2->Scale(massTot/mass2/numOfSim*fraction*daymBq);
h2->SetName("skin");
h2->SetLineColor(kBlack);

h0->Rebin(4);
h0->Scale(1./4);
h1->Rebin(4);
h1->Scale(1./4);
h2->Rebin(4);
h2->Scale(1./4);


h0->Fit("pol2");
TF1* f0 = h0->GetFunction("pol2");
f0->SetLineColor(kBlue);
if (fitAct){
h1->Fit("pol1");
TF1* f1 = h1->GetFunction("pol1");
f1->SetLineColor(kRed);
}




//-----start drawing


TCanvas* c1 = new TCanvas("c1", ";Energy [keV];event rate per activity mass [(mBq/ kg)^{-1} kg^{-1} day^{-1} keV^{-1}]");
c1->Draw();

double skinScale=2.; //--Wei Need to change

TLegend* tl = new TLegend(0.7,0.8,0.9,0.9);
tl->AddEntry(h0, "center");
tl->AddEntry(h1, "active volume");
TString label= TString::Format("skin * %d",int(skinScale));
if (drawSkin) {tl->AddEntry(h2, label.Data());}
h0->Draw();
h1->Draw("same");
if (drawSkin) {
h2->Scale(1./skinScale); //further scale down to plot on the same graph.
h2->Draw("same");
}
tl->Draw("same");

TString tcontent;
tcontent= TString::Format("#splitline{p0: %.3e }{#splitline{p1: %.3e }{p2: %.3e}}",f0->GetParameter(0), f0->GetParameter(1), f0->GetParameter(2));
TLatex* tt = new TLatex(100*0.6, 0.15*(h0->GetMaximum() - h0->GetMinimum() )+ h0->GetMinimum(), tcontent.Data());
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
