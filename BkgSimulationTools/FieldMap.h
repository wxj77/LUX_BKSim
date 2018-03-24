#ifndef FIELDMAP_H
#define FIELDMAP_H
#include <TString.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH2.h>
#include <TRandom3.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;

// struct to store field map data points
struct gridPoint {
   int Idx;
   double x,y,z,E,S2x,S2y,dt,evtClass;
   double params[4]; // filled with {S2x, S2y, dt, E} for convenience
   double paramsS2[4]; // filled with {x, y, z, E} for convenience
   bool hasNaN;
   double dist;
   double distS2;
};

// sorting methods
struct byDist {
   bool operator()(const gridPoint& p1,const gridPoint& p2) {
      return p1.dist < p2.dist;
   }
};
struct byDistS2 {
   bool operator()(const gridPoint& p1,const gridPoint& p2) {
      return p1.distS2 < p2.distS2;
   }
};

// Class definition
class FieldMap {
private:
   // variable pertaining to imported field map
   vector<double> x,y,z,E,S2x,S2y,dt,evtClass; // obsolete
   vector<bool> hasNaN; // obsolete
   int nGridPoints;
   vector<gridPoint> pp;
   int gridVersion;
   // variables pertaining to specified point of interest (for interpolation)
   double xx,yy,zz;
   double S2xx,S2yy,dtt;
   vector<gridPoint> pp_sorted;
public:
   // Class methods
   FieldMap(){;}
   FieldMap(const char *map_fname) { LoadFieldMap(map_fname); }
   virtual	  ~FieldMap() {;}
   virtual void	  LoadFieldMap(const char *);
   double	  GetX(int Idx)	       {return pp[Idx].x;}
   double	  GetY(int Idx)	       {return pp[Idx].y;}
   double	  GetZ(int Idx)	       {return pp[Idx].z;}
   double	  GetS2x(int Idx)      {return pp[Idx].S2x;}
   double	  GetS2y(int Idx)      {return pp[Idx].S2y;}
   double	  GetDt(int Idx)       {return pp[Idx].dt;}
   double	  GetEvtClass(int Idx) {return pp[Idx].evtClass;}
   void		  SetCurrentPoint(double, double, double);
   void		  SetCurrentPointS2(double, double, double);
   int		  FindP000Idx();
//   int		  FindPxxxIdx(int, int, int);
   int		  FindPxxxIdx(int xside=0, int yside=0, int zside=0);
   void		  InterpolateTrilinear(double *);
   void		  InterpolateGridless(double *);
   void		  InterpolateGridlessS2(double *);
   int		  SortByDist(double rSphere=2);
   int		  SortByDistS2(double rSphere=2);
   double	  DistFromIdx(int);
   double	  DistS2FromIdx(int);
   void		  PlotMap(int numPts=1000);
};
//***********************************************//
// LoadFieldMap:
// Read field map txt file and store data in
//    member variables.
//***********************************************//
void FieldMap::LoadFieldMap(const char *map_fname) {
   // Open up the field map text file
   cout << "Using field map: " << map_fname << endl;
   ifstream infile(map_fname);
   // Read in header row (don't need it)
   char buffer[1000];
   infile.getline(buffer,1000);
   // Read in datapoints
   string xi_str,yi_str,zi_str,Ei_str,S2xi_str,S2yi_str,dti_str,evtClassi_str;
   int lineNum = 1;
   double nanCode = -999;
   while (infile >> xi_str >> yi_str >> zi_str >> Ei_str >> S2xi_str >> S2yi_str >> dti_str >> evtClassi_str) {
      //if (lineNum > 1000) break; // for testing
      lineNum++;
      //cout << "reading line number " << lineNum << endl;
      char xi[1024]; strcpy(xi,xi_str.c_str());
      char yi[1024]; strcpy(yi,yi_str.c_str());
      char zi[1024]; strcpy(zi,zi_str.c_str());
      char Ei[1024]; strcpy(Ei,Ei_str.c_str());
      char S2xi[1024]; strcpy(S2xi,S2xi_str.c_str());
      char S2yi[1024]; strcpy(S2yi,S2yi_str.c_str());
      char dti[1024]; strcpy(dti,dti_str.c_str());
      char evtClassi[1024]; strcpy(evtClassi,evtClassi_str.c_str());
      // Store data points in member variable vectors (this is now obsolete)
      /*x.push_back(stod(xi));
      y.push_back(stod(yi));
      z.push_back(stod(zi));
      (Ei.compare("nan")==0)	    ? E.push_back(nanCode)	  : E.push_back(stod(Ei));
      (S2xi.compare("nan")==0)	    ? S2x.push_back(nanCode)	  : S2x.push_back(stod(S2xi));
      (S2yi.compare("nan")==0)	    ? S2y.push_back(nanCode)	  : S2y.push_back(stod(S2yi));
      (dti.compare("nan")==0)	    ? dt.push_back(nanCode)	  : dt.push_back(stod(dti));
      (evtClassi.compare("nan")==0) ? evtClass.push_back(nanCode) : evtClass.push_back(stod(evtClassi));
      hasNaN.push_back( (dti.compare("nan")==0) );
      */
      // end of obsolete data vector filling

      // Store data in vector of gridPoint structs
      // For NaN, fill structure fields with a default code and flag with boolean (hasNaN)
      gridPoint pi;
      /*pi.x = stod(xi); pi.y = stod(yi); pi.z = stod(zi);
      (Ei.compare("nan")==0)	    ? pi.E = nanCode	    : pi.E = stod(Ei);
      (S2xi.compare("nan")==0)	    ? pi.S2x = nanCode	    : pi.S2x = stod(S2xi);
      (S2yi.compare("nan")==0)	    ? pi.S2y = nanCode	    : pi.S2y = stod(S2yi);
      (dti.compare("nan")==0)	    ? pi.dt = nanCode	    : pi.dt = stod(dti);
      (evtClassi.compare("nan")==0) ? pi.evtClass = nanCode : pi.evtClass = stod(evtClassi);*/
      pi.x = atof(xi); pi.y = atof(yi); pi.z = atof(zi);
      char nan[] = "nan";
      (strcmp(Ei,nan)==0)	 ? pi.E = nanCode	 : pi.E = atof(Ei);
      (strcmp(S2xi,nan)==0)	 ? pi.S2x = nanCode	 : pi.S2x = atof(S2xi);
      (strcmp(S2yi,nan)==0)	 ? pi.S2y = nanCode	 : pi.S2y = atof(S2yi);
      (strcmp(dti,nan)==0)	 ? pi.dt = nanCode	 : pi.dt = atof(dti);
      (strcmp(evtClassi,nan)==0) ? pi.evtClass = nanCode : pi.evtClass = atof(evtClassi);

      pi.params[0] = pi.S2x; pi.params[1] =  pi.S2y; pi.params[2] = pi.dt; pi.params[3] =  pi.E;
      pi.paramsS2[0] = pi.x; pi.paramsS2[1] =  pi.y; pi.paramsS2[2] = pi.z; pi.paramsS2[3] =  pi.E;
      pi.Idx = lineNum-2;
      //pi.hasNaN =  dti.compare("nan")==0; // shouldn't have NaN in other fields and not drift time...
      pi.hasNaN =  (strcmp(dti,nan)==0) ; // shouldn't have NaN in other fields and not drift time...
      pp.push_back(pi);
      //cout  << xi <<"\t"<< yi <<"\t"<< zi <<"\t"<< Ei <<"\t"<< S2xi <<"\t"<< S2yi <<"\t"<< dti <<"\t"<< evtClassi << endl;
      //cout  << pi.x <<"\t"<< pi.y <<"\t"<< pi.z <<"\t"<< pi.E <<"\t"<< pi.S2x <<"\t"<< pi.S2y <<"\t"<< pi.dt <<"\t"<< pi.evtClass << endl;

   }
   //pp_sorted = pp;
   nGridPoints = lineNum - 1;
   if (	       pp[0].x == -24	 && pp[0].y == -24    && pp[0].z == 5.8) {
      gridVersion = 1;
   } else if ( pp[0].x == -23.5	 && pp[0].y == -23.5  && pp[0].z == -33.975) {
      gridVersion = 0;
   } else {
      gridVersion = -1;
   }
   //printf("Read %d lines\n",lineNum-1);
   //printf("Size of x ,E ,S2x : %d, %d, %d\n",x.size(),E.size(),S2x.size());
   //printf("x[3] , E[3], S2x[3]:  %f, %f, %f\n",x[3] , E[3], S2x[3]);

}

//+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++
void FieldMap::SetCurrentPoint(double xthis, double ythis, double zthis) {
// Define the reference point (in real space) used for interpolation
   xx = xthis; yy = ythis; zz = zthis;
}
//+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++
void FieldMap::SetCurrentPointS2(double xthis, double ythis, double dtthis) {
// Define the reference point (in S2 space) used for interpolation
   S2xx = xthis; S2yy = ythis; dtt = dtthis;
}

//***********************************************//
// FindP000Idx:
// Find lattice point closest to input coordinates
// with x0,y0,z0 < x,y,z
//***********************************************//
int FieldMap::FindP000Idx() {
   int xIdx = floor(xx + 23.5);
   int yIdx = floor(yy + 23.5);
   int zIdx = floor(zz + 33.975);
   int rIdx = xIdx + 48*yIdx + 48*48*zIdx;
   return rIdx;
}
//***********************************************//
// FindPxxxIdx:
// Find lattice point closest to input coordinates
// Use the  3 arguments to choose upper or lower
// edges for x,y,z
// Now can accommodate two versions of field map
// coordinate system.
//***********************************************//
int FieldMap::FindPxxxIdx(int xside, int yside, int zside) {
   double xmin, xmax, ymin, ymax, zmin, zmax;
   if (gridVersion == 0) {
      xmin = ymin = -23.5;
      xmax = ymax = 23.5;
      zmin = -33.975;
      zmax = 14.025;
   } else if (gridVersion == 1) {
      xmin = ymin = -24.;
      xmax = ymax = 24;
      zmin = 5.8;
      zmax = 53.8;
   }
   // Throw an error if the point is outside the grid
   if (xx > xmax || xx < xmin || yy > ymax || yy < ymin || zz > zmax || zz < zmin ) {
      return -1; 
   }
   int xIdx = (xside ==0 ) ? floor(xx - xmin)	: ceil(xx - xmin);
   int yIdx = (yside ==0 ) ? floor(yy - ymin)	: ceil(yy - ymin);
   int zIdx = (zside ==0 ) ? floor(zz - zmin)	: ceil(zz - zmin);
   // find the linearized index
   int nXpoints = (int)(xmax - xmin + 1);
   int nYpoints = (int)(ymax - ymin + 1);
   int rIdx = xIdx + nXpoints*yIdx + nXpoints*nYpoints*zIdx;
   //printf("xIdx,yIdx,zIdx,rIdx = %d,%d,%d,%d, P%d%d%d = (%.1f,%.1f,%.3f)\n",xIdx,yIdx,zIdx,rIdx,xside,yside,zside,pp[rIdx].x,pp[rIdx].y,pp[rIdx].z);
   return rIdx;
}

//||**********************************************************||
//    InterpolateTrilinear:
//    Fill the supplied array with {S2x, S2y, dt, E}
//    as interpolated from field map for the "current"
//    point in real space. This relies on a regular
//    grid in real space. If any of the 8 vertices
//    required for this algorithm have undefined values,
//    this function calls the algorithm used for "gridless"
//    data.
//    https://en.wikipedia.org/wiki/Trilinear_interpolation
//||**********************************************************||
void FieldMap::InterpolateTrilinear(double params[4]) {
   // Abandon trilinear algorithm if lookup table won't work
   if (gridVersion == -1) {
      cout << "WARNING: Trilinear interpolation failed because we don't recognize the grid coordinate system." << endl;
      cout << "Using position weighted interpolation... " << endl;
      InterpolateGridless(params);
      return;
   }
   // Find 8 vertices of cube surrounding point of interest
   int Idx000 = FindPxxxIdx(0,0,0);
   int Idx001 = FindPxxxIdx(0,0,1);
   int Idx010 = FindPxxxIdx(0,1,0);
   int Idx011 = FindPxxxIdx(0,1,1);
   int Idx100 = FindPxxxIdx(1,0,0);
   int Idx101 = FindPxxxIdx(1,0,1);
   int Idx110 = FindPxxxIdx(1,1,0);
   int Idx111 = FindPxxxIdx(1,1,1);
   // Guard against reference points outside field map grid
   if (Idx000 == -1) {
      //cout << "WARNING: Trilinear interpolation failed because reference point is outside field map grid!" << endl;
      //cout << "Using position weighted interpolation... " << endl;
      InterpolateGridless(params);
      return;
   }
   // If any vertex has any NaN entries, stop and use the slower nearest-neighbor method
   if (	 pp[Idx000].hasNaN || pp[Idx001].hasNaN || pp[Idx010].hasNaN || pp[Idx011].hasNaN ||
	 pp[Idx100].hasNaN || pp[Idx101].hasNaN || pp[Idx110].hasNaN || pp[Idx111].hasNaN ) {
      //cout << "WARNING: Trilinear interpolation failed because not all cube vertices had non-NAN values." << endl;
      //cout << "Using position weighted interpolation... " << endl;
      InterpolateGridless(params);
      return;
   }

   // Find x,y,z fractional distances to those points (these are the weights in the averaging)
   double xd = (xx - pp[Idx000].x) / (pp[Idx111].x - pp[Idx000].x);
   double yd = (yy - pp[Idx000].y) / (pp[Idx111].y - pp[Idx000].y);
   double zd = (zz - pp[Idx000].z) / (pp[Idx111].z - pp[Idx000].z);
   // Loop over functions to interpolate (S2x, S2y, drift time, E-field)
   for (int ff=0; ff<4; ff++) {
      switch (ff) { // this conditional is a holdover from an older method, still good for debugging
	 case 0:
	    /*printf("pp[Idx000].x,pp[Idx001].x,pp[Idx010].x,pp[Idx011].x,pp[Idx100].x,pp[Idx101].x,pp[Idx110].x,pp[Idx111].x:\n%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
		  pp[Idx000].x,pp[Idx001].x,pp[Idx010].x,pp[Idx011].x,pp[Idx100].x,pp[Idx101].x,pp[Idx110].x,pp[Idx111].x);
	    printf("pp[Idx000].y,pp[Idx001].y,pp[Idx010].y,pp[Idx011].y,pp[Idx100].y,pp[Idx101].y,pp[Idx110].y,pp[Idx111].y:\n%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
		  pp[Idx000].y,pp[Idx001].y,pp[Idx010].y,pp[Idx011].y,pp[Idx100].y,pp[Idx101].y,pp[Idx110].y,pp[Idx111].y);
	    printf("pp[Idx000].z,pp[Idx001].z,pp[Idx010].z,pp[Idx011].z,pp[Idx100].z,pp[Idx101].z,pp[Idx110].z,pp[Idx111].z:\n%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
		  pp[Idx000].z,pp[Idx001].z,pp[Idx010].z,pp[Idx011].z,pp[Idx100].z,pp[Idx101].z,pp[Idx110].z,pp[Idx111].z);
	    printf("pp[Idx000].hasNaN,pp[Idx001].hasNaN,pp[Idx010].hasNaN,pp[Idx011].hasNaN,pp[Idx100].hasNaN,pp[Idx101].hasNaN,pp[Idx110].hasNaN,pp[Idx111].hasNaN:\n%d %d %d %d %d %d %d %d\n",
		  (int)pp[Idx000].hasNaN,(int)pp[Idx001].hasNaN,(int)pp[Idx010].hasNaN,(int)pp[Idx011].hasNaN,(int)pp[Idx100].hasNaN,(int)pp[Idx101].hasNaN,(int)pp[Idx110].hasNaN,(int)pp[Idx111].hasNaN);*/
	    //cout << "S2x" << endl;
	    break;
	 case 1:
	    //cout << "S2y" << endl;
	    break;
	 case 2:
	    //cout << "dt" << endl;
	    break;
	 case 3:
	    //cout << "E" << endl;
	    break;
      }
      // interpolate V (generic name for any of the 4 interpolated values)
      double V00 = pp[Idx000].params[ff]*(1-xd) + pp[Idx100].params[ff]*xd;
      double V01 = pp[Idx001].params[ff]*(1-xd) + pp[Idx101].params[ff]*xd;
      double V10 = pp[Idx010].params[ff]*(1-xd) + pp[Idx110].params[ff]*xd;
      double V11 = pp[Idx011].params[ff]*(1-xd) + pp[Idx111].params[ff]*xd;
      double V0 = V00*(1-yd) + V10*yd;
      double V1 = V01*(1-yd) + V11*yd;
      double VInterp = V0*(1-zd) + V1*zd;
      if (VInterp < -50) {
	 //cout<< "nan" << endl;
	 //InverseDistInterp(xx,yy,zz);
      }
      //printf("pp[Idx000].params[ff],pp[Idx001].params[ff],pp[Idx010].params[ff],pp[Idx011].params[ff],pp[Idx100].params[ff],pp[Idx101].params[ff],pp[Idx110].params[ff],pp[Idx111].params[ff]:\n%.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",pp[Idx000].params[ff],pp[Idx001].params[ff],pp[Idx010].params[ff],pp[Idx011].params[ff],pp[Idx100].params[ff],pp[Idx101].params[ff],pp[Idx110].params[ff],pp[Idx111].params[ff]);
      //printf("V interpolated: %.2f\n",VInterp);
      params[ff] = VInterp;
   }
}

//+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++
void FieldMap::InterpolateGridless(double params[4]) {
// Find the weighted average of {S2x, S2y, dt, E} for nearby points in real space.
// Weights are defined as 1/r.

   // Find and sort nearby points (within 5 cm radius)
   int numWithinSphere = SortByDist(5);
   for (int ii=0; ii<4; ii++) { // loop over value types
      double invRnorm = 0;
      double sum = 0;
      // Go through 8 nearest neighbors, but cut off if farther than 5 cm away
      for (int jj=0; jj<numWithinSphere && jj<8; jj++) {
	 sum += pp_sorted[jj].params[ii] / pp_sorted[jj].dist;
	 invRnorm += 1. / pp_sorted[jj].dist;
      }
      // Fill array with interpolated value (error code if no nearby neighbors found)
      params[ii] = (numWithinSphere > 0) ?  sum/invRnorm : -9999;
   }
}
//+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++
void FieldMap::InterpolateGridlessS2(double params[4]) {
// Find the weighted average of {x, y, z, E} for nearby points in S2 space.
// Weights are defined as 1/r.

   // Find and sort nearby points (within 5 cm radius)
   int numWithinSphere = SortByDistS2(5);
   for (int ii=0; ii<4; ii++) {
      // Go through 8 nearest neighbors, but cut off if farther than 5 cm away
      double invRnorm = 0;
      double sum = 0;
      for (int jj=0; jj<numWithinSphere && jj<8; jj++) {
	 sum += pp_sorted[jj].paramsS2[ii] / pp_sorted[jj].distS2;
      // Fill array with interpolated value (error code if no nearby neighbors found)
	 invRnorm += 1. / pp_sorted[jj].distS2;
      }
      params[ii] = (numWithinSphere > 0) ?  sum/invRnorm : -9999;
   }

}
//+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++
int FieldMap::SortByDist(double rSphere) {
// Find and sort map points close to real-space point of interest.
// Loops through all grid points, but only stores points within rSphere [cm]

   pp_sorted.clear();
   int numDefinedInSphere=0;
   for (int ii=0; ii<nGridPoints; ii++) {
      pp[ii].dist = DistFromIdx(pp[ii].Idx);
      if (pp[ii].dist < rSphere && !pp[ii].hasNaN) {
	 pp_sorted.push_back(pp[ii]);
	 numDefinedInSphere++;
      }
   }
   sort(pp_sorted.begin(), pp_sorted.end(), byDist());
   /*for (int ii=0; ii<10; ii++) {
      cout << "x\ty\tz\t|r|" << endl;
      cout << pp_sorted[ii].x << "\t" << pp_sorted[ii].y << "\t" << pp_sorted[ii].z << "\t" << pp_sorted[ii].dist << "\t" << endl;
   }*/
   return numDefinedInSphere;
}

//+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++
int FieldMap::SortByDistS2(double rSphere) {
// Find and sort map points close to S2-space point of interest.
// Loops through all grid points and calculates distance, but only stores
// points within rSphere [cm]

   pp_sorted.clear();
   int numDefinedInSphere=0;
   for (int ii=0; ii<nGridPoints; ii++) {
      pp[ii].distS2 = DistS2FromIdx(pp[ii].Idx);
      if (pp[ii].distS2 < rSphere && !pp[ii].hasNaN) {
	 pp_sorted.push_back(pp[ii]);
	 numDefinedInSphere++;
      }
   }
   sort(pp_sorted.begin(), pp_sorted.end(), byDistS2());
   /*for (int ii=0; ii<10; ii++) {
      cout << "x\ty\tz\t|r|" << endl;
      cout << pp_sorted[ii].x << "\t" << pp_sorted[ii].y << "\t" << pp_sorted[ii].z << "\t" << pp_sorted[ii].dist << "\t" << endl;
   }*/
   return numDefinedInSphere;
}

//+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++
double FieldMap::DistFromIdx(int Idx) {
// Find distance between current real-space coordinate and indexed grid point
   //return sqrt(pow(xx-x[Idx],2) + pow(yy-y[Idx],2) + pow(zz-z[Idx],2));
   return sqrt(pow(xx-pp[Idx].x,2) + pow(yy-pp[Idx].y,2) + pow(zz-pp[Idx].z,2));
}

//+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++
double FieldMap::DistS2FromIdx(int Idx) {
// Find distance between current S2-space coordinate and indexed grid point
   // Scale the dt coordinate by .135 to roughly translate from us to cm
   return sqrt(pow(S2xx-pp[Idx].S2x,2) + pow(S2yy-pp[Idx].S2y,2) + pow(0.135*(dtt-pp[Idx].dt),2));
}

//+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++---+++
//void FieldMap::PlotMap(int numPts=1000) {
void FieldMap::PlotMap(int numPts) {
// Test out the code by generating random positions in real space,
// mapping to S2 space, and then mapping back again.

   double gridZmin, gridZmax;
   if (gridVersion == 0) {gridZmin = -33.975; gridZmax = 14.025;} 
   else if (gridVersion == 1) {gridZmin = 5.8; gridZmax = 53.8;}
   else {return;}
   // Declare histos
   TH2F* h_R2vsZ_real = new TH2F("h_R2vsZ_real","h_R2vsZ_real",100,0,500,100,gridZmin,gridZmax);
   TH2F* h_R2vsDT_S2 = new TH2F("h_R2vsDT_S2","h_R2vsDT_S2",100,0,500,100,-350,0);
   TH2F* h_R2vsZ_realGuess = new TH2F("h_R2vsZ_realGuess","h_R2vsZ_realGuess",100,0,500,100,gridZmin,gridZmax);
   TH2F* h_R2vsZ_realGuessDisp = new TH2F("h_R2vsZ_realGuessDisp","h_R2vsZ_realGuessDisp",100,0,500,100,gridZmin,gridZmax);
   TH2F* h_XvsY_real = new TH2F("h_XvsY_real","h_XvsY_real",100,-23,23,100,-23,23);
   TH2F* h_XvsY_S2 = new TH2F("h_XvsY_S2","h_XvsY_S2",100,-23,23,100,-23,23);
   TH2F* h_XvsY_realGuess = new TH2F("h_XvsY_realGuess","h_XvsY_realGuess",100,-23,23,100,-23,23);
   TH2F* h_XvsY_realGuessDisp = new TH2F("h_XvsY_realGuessDisp","h_XvsY_realGuessDisp",100,-23,23,100,-23,23);
   TH2F* h_ZvsE_real = new TH2F("h_ZvsE_real","h_ZvsE_real",100,gridZmin,gridZmax,100,0,600);
   TH2F* h_DTvsE_S2 = new TH2F("h_DTvsE_S2","h_DTvsE_S2",100,-350,0,100,0,600);
   TRandom3 rando;
   // Hastily defined fiducial volume
   double xmin = -20;
   double xmax = 20;
   double ymin = -20;
   double ymax = 20;
   double zmin = gridZmin + 2.;
   double zmax = gridZmax -2.;
   //int numPts=10000;
   for (int ii=0; ii<numPts; ii++) {
      // pick random points in a box
      double xthis = xmin + (xmax - xmin)*rando.Rndm();
      double ythis = ymin + (ymax - ymin)*rando.Rndm();
      double zthis = zmin + (zmax - zmin)*rando.Rndm();
      // reject points outside fiducial cylinder
      if (xthis*xthis+ythis*ythis > 400) {ii--; continue;}
      if (ii % 1000 == 0) cout << ii  << " points plotted..." << endl;
      // Interpolate from field map to get S2x, S2y, dt, E
      SetCurrentPoint(xthis,ythis,zthis);
      double params[4];
      double paramsS2[4];
      InterpolateTrilinear(params); // params = {S2x, S2y, dt, E}
      // Fill histos
      h_R2vsZ_real->Fill(xx*xx+yy*yy,zz);
      h_R2vsDT_S2->Fill(params[0]*params[0]+params[1]*params[1],-params[2]);
      h_XvsY_real->Fill(xx,yy);
      h_XvsY_S2->Fill(params[0],params[1]);
      h_ZvsE_real->Fill(zz,params[3]/100.);
      // Now use reverse mapping to go from S2 space to real space again
      SetCurrentPointS2(params[0],params[1],params[2]);
      InterpolateGridlessS2(paramsS2); // paramsS2 = {x, y, z, E}
      h_R2vsZ_realGuess->Fill(paramsS2[0]*paramsS2[0]+paramsS2[1]*paramsS2[1],paramsS2[2]);
      h_XvsY_realGuess->Fill(paramsS2[0],paramsS2[1]);
      h_DTvsE_S2->Fill(-params[2],paramsS2[3]/100.);
      // how far away is the double-mapped point from the original?
      double displacement = sqrt( pow(xx - paramsS2[0], 2) + pow(yy - paramsS2[1], 2) + pow(zz - paramsS2[2], 2) );
      h_R2vsZ_realGuessDisp->Fill(xx*xx+yy*yy,zz, displacement);
      h_XvsY_realGuessDisp->Fill(xx,yy,displacement);
   }
   // Normalize displacement histos
   h_R2vsZ_realGuessDisp->Divide(h_R2vsZ_real);
   h_XvsY_realGuessDisp->Divide(h_XvsY_real);
   // Plot the position mapping histos
   TCanvas *c1 = new TCanvas("c1","c1");
   c1->Divide(4,2);
   c1->cd(1);
   h_R2vsZ_real->SetTitle("random positions in true space...");
   h_R2vsZ_real->SetXTitle("R^2 [cm^2]");
   h_R2vsZ_real->SetYTitle("Z [cm]");
   h_R2vsZ_real->Draw("colz");
   c1->cd(2);
   h_R2vsDT_S2->SetTitle("...mapped into S2 space...");
   h_R2vsDT_S2->SetXTitle("R_{S2}^2 [cm^2]");
   h_R2vsDT_S2->SetYTitle("Drift time [#mus]");
   h_R2vsDT_S2->Draw("colz");
   c1->cd(3);
   h_R2vsZ_realGuess->SetTitle("...mapped back into true space");
   h_R2vsZ_realGuess->SetXTitle("R^2 [cm^2]");
   h_R2vsZ_realGuess->SetYTitle("Z [cm]");
   h_R2vsZ_realGuess->Draw("colz");
   c1->cd(4);
   h_R2vsZ_realGuessDisp->SetTitle("avg displacement from original point");
   h_R2vsZ_realGuessDisp->SetXTitle("R^2 [cm^2]");
   h_R2vsZ_realGuessDisp->SetYTitle("Z [cm]");
   h_R2vsZ_realGuessDisp->SetZTitle("disp. [cm]");
   h_R2vsZ_realGuessDisp->Draw("colz");
   c1->cd(5);
   h_XvsY_real->SetTitle("");
   h_XvsY_real->SetXTitle("X [cm]");
   h_XvsY_real->SetYTitle("Y [cm]");
   h_XvsY_real->Draw("colz");
   c1->cd(6);
   h_XvsY_S2->SetTitle("");
   h_XvsY_S2->SetXTitle("X_{S2} [cm]");
   h_XvsY_S2->SetYTitle("Y_{S2} [cm]");
   h_XvsY_S2->Draw("colz");
   c1->cd(7);
   h_XvsY_realGuess->SetTitle("");
   h_XvsY_realGuess->SetXTitle("X [cm]");
   h_XvsY_realGuess->SetYTitle("Y [cm]");
   h_XvsY_realGuess->Draw("colz");
   c1->cd(8);
   h_XvsY_realGuessDisp->SetTitle("");
   h_XvsY_realGuessDisp->SetXTitle("X [cm]");
   h_XvsY_realGuessDisp->SetYTitle("Y [cm]");
   h_XvsY_realGuessDisp->SetZTitle("disp. [cm]");
   h_XvsY_realGuessDisp->Draw("colz");
   // Plot the E-field vs. Z histos
   TCanvas *c2 = new TCanvas("c2","c2");
   c2->Divide(2,1);
   c2->cd(1);
   gStyle->SetOptStat(0);
   h_ZvsE_real->SetTitle("Interpolated E field at random positions");
   h_ZvsE_real->SetXTitle("Z [cm]");
   h_ZvsE_real->SetYTitle("|E| [V/cm]");
   h_ZvsE_real->Draw("colz");
   c2->cd(2);
   gStyle->SetOptStat(0);
   h_DTvsE_S2->SetTitle("Interpolated E field at mapped random positions");
   h_DTvsE_S2->SetXTitle("Drift time [#mus]");
   h_DTvsE_S2->SetYTitle("|E| [V/cm]");
   h_DTvsE_S2->Draw("colz");

}

#endif

