// To compile:
// g++ generateFastBkg.cc `root-config --cflags --libs` -o generateFastBkg	 
//
// To run:
// ./generateFastBkg <time bin> <inputfilename.root> [<outputfilename>]
//	 
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <functional>	 
#include <numeric>			
#include <cmath>
#include <cassert>
#include <sstream>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "Riostream.h"

#include <TTree.h>
#include <TString.h>
#include <TFile.h>

#include "TROOT.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TSystemDirectory.h"
#include "TList.h"
#include "TSystemFile.h"
//#include "../../LibNest/libNEST/NEST.cxx"
#include "NEST.h"
#include "modelBinDef.h"

using namespace std;

double delta_z(vector<double> z);
TTree * Cluster(TString fName);

int main(int argc, char** argv)
{
	
	//cout<<outputFile<<endl;
	int TimeBinNum=1;
	cout<<argv[0]<<" "<<argv[1]<<" "<<argv[2]<<endl;
	string inputFile=argv[1];
	//TFile* out_file = new TFile( outputFile.c_str(), "RECREATE");
	//cout<<inputFile<<endl;
	//cout<<inputFile.substr(0,inputFile.length()-3)<<endl;
	string outputFile = inputFile.substr(0,inputFile.length()-3);
	outputFile = outputFile+"root";
	//cout<<outputFile<<endl;
	
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


TTree * Cluster(TString fName) {

	// This code groups energy depositions that are spatially close into a single energy deposition. 
	//There is a reason for this that is related to how NEST is non-linear in certain important regiems. 
	//But it solves some problem.

    char name[100];
    sprintf(name, "%s", fName.Data());
    TTree *temp = new TTree("temp","Temp TTree");
    TTree *temp2 = new TTree("temp2","Temp TTree Two");
    ifstream in;
    float x[100], y[100], z[100], energy[100];
    int evtn, partid, iRecordSize;
    int j = 0;
    int evtn_temp = 0;
    float x_temp;
    float y_temp;
    float z_temp;
    float energy_temp;
    
    temp->Branch("evtn",&evtn_temp,"evtn/I");
    temp->Branch("iRecordSize",&iRecordSize,"iRecordSize/I");
    temp->Branch("partid",&partid,"partid/I");
    temp->Branch("x",&x,"x[iRecordSize]/F");
    temp->Branch("y",&y,"y[iRecordSize]/F");
    temp->Branch("z",&z,"z[iRecordSize]/F");
    temp->Branch("energy",&energy,"energy[iRecordSize]/F");
    
    in.open(name);

    int p = 0;
    while (1) {
        in >> evtn >> partid >> energy_temp >> x_temp >> y_temp >> z_temp;
        if (!in.good()) {temp->Fill();break;}
        if (evtn_temp==0) {
            evtn_temp=evtn;
        }
        if (evtn == evtn_temp){
            x[p] = x_temp;
            y[p] = y_temp;
            z[p] = z_temp;
            energy[p] = energy_temp;
            p++;
            iRecordSize = p;
        } else {
            temp->Fill();
            p = 0;
            evtn_temp=evtn;
            x[p] = x_temp;
            y[p] = y_temp;
            z[p] = z_temp;
            energy[p] = energy_temp;
            p++;
            iRecordSize = p;
        }
    }

    temp->SetBranchAddress("x",&x);
    temp->SetBranchAddress("y",&y);
    temp->SetBranchAddress("z",&z);
    temp->SetBranchAddress("partid",&partid);
    temp->SetBranchAddress("iRecordSize",&iRecordSize);
    temp->SetBranchAddress("evtn",&evtn);
    temp->SetBranchAddress("energy",&energy);
    
    float x2[100], y2[100], z2[100], energy2[100];
    int evtn2, partid2, iRecordSize2;
    
    temp2->Branch("evtn2",&evtn2,"evtn2/I");
    temp2->Branch("iRecordSize2",&iRecordSize2,"iRecordSize2/I");
    temp2->Branch("partid2",&partid2,"partid2/I");
    temp2->Branch("x2",&x2,"x2[iRecordSize2]/F");
    temp2->Branch("y2",&y2,"y2[iRecordSize2]/F");
    temp2->Branch("z2",&z2,"z2[iRecordSize2]/F");
    temp2->Branch("energy2",&energy2,"energy2[iRecordSize2]/F");

    int k;
    int n;
    int numEntries = temp->GetEntries();
    for(int i = 0; i < numEntries; i++) {
        temp->GetEntry(i);
        k = 0;
        for(int j = 0; j < iRecordSize; j++) {
            if(j==0){
              x2[k] = x[j];
              y2[k] = y[j];
              z2[k] = z[j];
              energy2[k] = 0.;
              iRecordSize2 = k + 1;
            }
            for(int m = 0; m < k + 1; m++){
                if((x[j]-x2[m]) * (x[j]-x2[m]) + (y[j]-y2[m]) * (y[j]-y2[m]) + (z[j]-z2[m]) * (z[j]-z2[m]) < (0.04) * (0.04)){
                    energy2[m] = energy2[m] + energy[j];
                    break;
                }
                else if (m == k){
                    k++;
                    x2[k] = x[j];
                    y2[k] = y[j];
                    z2[k] = z[j];
                    energy2[k] = energy[j];
                    iRecordSize2 = k + 1;
                    break;
                }
            }
        }
        evtn2 = evtn;
        partid2 = partid;
        temp2->Fill();
    }
    temp2->SetBranchAddress("x2",&x2);
    temp2->SetBranchAddress("y2",&y2);
    temp2->SetBranchAddress("z2",&z2);
    temp2->SetBranchAddress("partid2",&partid2);
    temp2->SetBranchAddress("iRecordSize2",&iRecordSize2);
    temp2->SetBranchAddress("evtn2",&evtn2);
    temp2->SetBranchAddress("energy2",&energy2);
    
    int numEntries2 = temp2->GetEntries();
    /*char outName[100];
    sprintf(outName, "%s.dat", fOutName.Data());
        
    FILE* outFile;
    outFile = fopen(outName, "w");
    for(int i = 0; i < numEntries2; i++) {
        temp2->GetEntry(i);
        for(int j = 0; j < iRecordSize2; j++) {
            fprintf(outFile, "%d   %d   %f   %f   %f   %f\n", evtn2, partid2, energy2[j], x2[j], y2[j], z2[j]);
        }
    }
    fclose(outFile);*/
}
