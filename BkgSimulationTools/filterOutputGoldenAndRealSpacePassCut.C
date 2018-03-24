#include <stdlib.h>
#include <TROOT.h>
#include <TChain.h>
#include <TTree.h>
#include <TFile.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TMath.h>
#include <iostream>
#include <fstream>
#include "Parameters.h"
#include <TString.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

#include <cstdlib>
#include <cstdio>
#include <math.h>
//#include <boost/foreach.hpp>

using namespace std;
/*
TString generateFileName(TString BkgFileDir, TString bkgsrc, TString bkgcomp, int TimeBin){
   TString TimeBinStr = NumberToString(TimeBin);
   TString filename = BkgFileDir+"/"+"golden"+bkgsrc+"_"+bkgcomp+"_tb"+TimeBinStr+"_fidCut.root";
   return(filename);
}
*/
void filterOutputGoldenAndRealSpacePassCut(std::string filename, std::string outputfigurename) {
//  TString TimeBinStr = NumberToString(TimeBin);
//   TString filename = BkgFileDir+"/"+"golden"+bkgsrc+"_"+bkgcomp+"_tb"+TimeBinStr+"_fidCut.root";
   std::cout<<filename.c_str()<<std::endl;
   TFile* f= TFile::Open(filename.c_str());
   TCanvas *c1 = new TCanvas("c1","multipads",900,700);

   gStyle->SetOptStat(0);
   c1->Divide(2,2);

   c1->cd(1);
   gPad->SetTickx(2);
   h_R2vsZ_real_cut->Draw();

   c1->cd(2);
   h_R2vsDT_S2_cut->Draw();
   
   c1->cd(3);
 //  h_XvsY_real_cut->Draw();

   c1->cd(4);
//   h_XvsY_S2_cut->Draw();

   TImage *img = TImage::Create();
   img->FromPad(c1, 10, 10, 300, 200);
   img->FromPad(c1);
   img->WriteImage(outputfigurename.c_str());
   delete img;
}      
