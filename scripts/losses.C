#include <iostream>
#include <fstream>
//#include <math>
#include <TNtuple.h>
#include <TFile.h>
#include <cstdio>	
void losses()
{
  FILE *fp = fopen("losses.txt","r"); 
  FILE *fout = fopen("mars.txt","w+"); 
  Float_t x,xangle,y,yangle,z,momentum,energy,zangle;
  Int_t ncols;
  Int_t nlines = 0;
  const double pmass = 0.9382720813; // proton mass 
  const int protoni = 1;             // mars index of proton  
  const float  weight=1.;            // mars weight
  TNtuple *ntuple = new TNtuple("ntuple","synerga losses data from ascii file","x:xangle:y:yangle:z:zangle:momentum:energy");

  while(1) {
    ncols = fscanf(fp,"%f %f %f %f %f %f",&x, &xangle, &y, &yangle, &z, &momentum);
    if (ncols < 0) break;
    energy=sqrt(momentum*momentum+ pmass*pmass)- pmass;
    zangle = sqrt(1.-xangle*xangle-yangle*yangle);
    ntuple->Fill(x,xangle,y,yangle,z,zangle,momentum,energy);
    nlines++;
    fprintf(fout,"%11d%7d%13.5f%13.5f%15.7f%15.7f%19.11f%17.9f%17.9f%17.9f\n", nlines,protoni,energy,weight,x*100.,y*100.,z*100.,xangle,yangle,zangle);
  }
  std::cout << " found " << nlines << " points" << std::endl ;
  
  fclose(fp);
  fclose(fout);
  
  ntuple->Print();
  
  ntuple->Draw("z : sqrt(x*x + y*y)");
  ntuple->Draw("momentum");
  //ntuple->Draw("sqrt(x*x + y*y)");
  ntuple->Draw("x:y");
  //   ntuple->Draw("z");
  //ntuple->Draw("yangle");
  //ntuple->Draw("y:yangle");
  //ntuple->Draw("x:xangle");
  //ntuple->Draw("xangle:yangle");
  //ntuple->Draw("energy");
  ntuple->Draw("zangle");

}
