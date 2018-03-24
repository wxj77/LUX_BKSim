#include "modelBinDef.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"
#include "FieldMap.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

template <typename T>
string NumberToString ( T Number )
{
   ostringstream ss;
   ss << Number;
   return ss.str();
}

//void mapDatFile(TString inDatPath, TString outDatPath, int timeBin = 1) {
void mapDatFile(TString BkgFileDir, TString BkgSrc, TString BkgComp, int timeBin = 1) {
   TString fieldMapName = timeBinDef(timeBin);
   TString TimeBinStr=NumberToString(timeBin);
   fieldMapName = "Run4FieldMaps/" + fieldMapName;
   // Declare a FieldMap object that reads in the desired Lucie field map
   FieldMap m(fieldMapName.Data());
   // Txt-file parsing vars
   unsigned int filler1, filler2, filler7(99);
   float dat_x, dat_y, dat_z, filler3, filler8(99.0);
   
   TString InputFile=BkgFileDir+"/"+BkgSrc+"/"+BkgComp+"/"+BkgSrc+"_"+BkgComp+".dat";
   TString OutputFile=BkgFileDir+"/"+BkgSrc+"/"+BkgComp+"/TB"+TimeBinStr+"/"+BkgSrc+"_"+BkgComp+".C3PO";
   
   
   ofstream new_coords(OutputFile.Data(), ofstream::out);
   size_t counter = 0;
   cout << "--> Reading in .dat file " << InputFile.Data() << "..." << endl;
   // Open .dat file list of coordinates
   
   ifstream coords(InputFile.Data());
   if(coords.is_open()) {
	//////////////////////////////////////////////
	// Loop over all .dat file positions and map them
	/////////////////////////////////////////////
      for(string line; getline(coords, line); ) {
         istringstream is(line);
         counter++;
         //cerr << "On event " << counter << "\r";
         if ((counter+1) % 10000 == 0) cout << "On event " << counter+1 << endl;
         if (is >> filler1 >> filler2 >> filler3 >> dat_x >> dat_y >> dat_z) {

            m.SetCurrentPoint(dat_x,dat_y,dat_z);
            double params[4];
            m.InterpolateTrilinear(params); // params = {S2x, S2y, dt, E}
            double S2x     = params[0]; // cm
            double S2y     = params[1]; // cm
            double S2dt    = params[2]; // us

            new_coords << filler1 << "\t" << filler2 << "\t" << filler3 << "\t" << dat_x << "\t" <<
            dat_y << "\t" << dat_z <<  "\t" << S2x << "\t" << S2y << "\t" << S2dt << "\t" << filler7 << "\t" << filler8  <<  endl;
         }
         else {
            cerr << "*** Problem reading in lines of .dat file. ***" << endl;
            exit(1);
         }
      }
   }
   
}
