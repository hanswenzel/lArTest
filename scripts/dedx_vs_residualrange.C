/*****************************************
 ****contact person: Ajib Paudel***********
 ****ajibpaudel@phys.ksu.edu***************
 ******************************************
 ******************************************/
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TStyle.h>
#include <iostream>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <math.h>
#include <string>
#include <TImage.h>
#include <iomanip>
#include <TSpline.h>
#include <TText.h>
#include <TFrame.h>
#include "TMinuit.h"


//*******************These values needs to be changed as required********************************//
double pitchvalue=0.50;//similar to Thickness of the medium for calculating MPV 
bool is_muon=1;
bool is_proton=1;
////************************************************************//////////



float parA=8;
float parb=-0.37;
///////////////////// Constants and sub routines defined for Landau-Vavilov function /////////////////
const int Z=18; //Atomic number of Argon
const double A=39.948; // g/mol Atomic mass of Argon
const double I=188.0e-6; // ev
const double K=0.307; // Mev.cm^2 / mol
const double Me=0.51; // Mev for electron
const double rho=1.396;//g/cm^3

const double Mmu=105.658; // Mev for Mu
const double Mpro=938.27; //proton mass in Mev


double beta(double gamma){
  double value=TMath::Sqrt(1-(1.0/(gamma*gamma)));
  return value;
}

double gamma(double KE_value,double mass){
  double value=(double(KE_value)/mass)+1;
  return value;
}

//double KE_mu=266.; //Minimum ionizing energy for muons
//double KE_pro=2000; //Minimum ionizing energy for protons
//double g_mu=gamma(KE_mu,Mmu);
//double g_pro=gamma(KE_pro,Mpro);

//double b_mu=beta(g_mu);
//double b_pro=beta(g_pro);


const double C=-5.2146;
const double X0=0.2;
const double X1=3.0;
const double a=0.19559;
const double m=3.0;
const double N=2*TMath::Log(10);

double density(double bg){
  double value;
  double x = TMath::Log10(bg);
  if(x<X0) return 0;
  if(x>X1) return N*x + C;
  value=a*(TMath::Power((X1-x),m));
  return N*x + C + value;
}

double dpdx(double KE_value,double x,double mass){
  double g=gamma(KE_value,mass);
  double b=beta(g);
  double epsilon=(double(K)/2)*(double(Z)/A)*(double(x*rho)/(b*b));
  double A0=double(2*Me*(TMath::Power((b*g),2)))/I;
  double A1=double(epsilon)/I;
  double value=(1.0/x)*epsilon*((TMath::Log(A0)) + TMath::Log(A1) + 0.2 - b*b - density(b*g));
  return value;
}

void dedx_vs_residualrange(){
  TFile *file1=new TFile("dedx_rr_plots.root","RECREATE");
  std::vector<double> rr_mu,rr_pro,dedx_mu,dedx_pro;

  if(is_muon){
    //These values are from NIST tables
    double KE_mu[13]; //KE values are in MeV 
    double Range_mu[13]; //residual range values are in cm
    KE_mu[0] = 10;  Range_mu[0] = 0.70437;
    KE_mu[1] = 14;  Range_mu[1] = 1.27937;
    KE_mu[2] = 20;  Range_mu[2] = 2.37894;
    KE_mu[3] = 30;  Range_mu[3] = 4.72636;
    KE_mu[4] = 40;  Range_mu[4] = 7.5788;
    KE_mu[5] = 80;  Range_mu[5] = 22.0917;
    KE_mu[6] = 100;  Range_mu[6] = 30.4441;
    KE_mu[7] = 140;  Range_mu[7] = 48.2235;
    KE_mu[8] = 200;  Range_mu[8] = 76.1461;
    KE_mu[9] = 300;  Range_mu[9] = 123.567;
    KE_mu[10] = 400;  Range_mu[10] = 170.845;
    KE_mu[11] = 800;  Range_mu[11] = 353.438;
    KE_mu[12] = 1000;  Range_mu[12] = 441.476;
    for(int i=0;i<13;i++){
      Range_mu[i]=Range_mu[i]/1.396;
    }
    TSpline3 *sp = new TSpline3("Cubic Spline",Range_mu,KE_mu,13,"b2e2",0,0);
    ofstream myfile1;
    myfile1.open("muon_mpv_dedx_vs_range_0.50.txt");
    for (int i=0; i<4000; i++){
      myfile1<<0.5+i*0.1<<"  "<<dpdx(sp->Eval(0.5+i*0.1),pitchvalue,Mmu)<<endl;
      dedx_mu.push_back(dpdx(sp->Eval(0.5+i*0.1),pitchvalue,Mmu));
      rr_mu.push_back(0.5+i*0.1);
    }
    TGraph *mu_dedx_rr=new TGraph(rr_mu.size(),&rr_mu[0],&dedx_mu[0]);
    mu_dedx_rr->SetTitle("MPV dE/dx vs residual range for muons;residual range[cm];dE/dx[MeV/cm]");
    mu_dedx_rr->Write("muon_dedx_rr");
    myfile1.close();
  }


  if(is_proton){
    //These values are from NIST tables
    double KE_pro[66]; double Range_pro[66];
    KE_pro[0] = 1;  Range_pro[0] = 0.00424;                                                                         
    KE_pro[1] = 1.25;  Range_pro[1] = 0.005979;                                                                     
    KE_pro[2] = 1.5;  Range_pro[2] = 0.00796;                                                                       
    KE_pro[3] = 1.75;  Range_pro[3] = 0.01017;                                                                      
    KE_pro[4] = 2;  Range_pro[4] = 0.0126;                                                                          
    KE_pro[5] = 2.25;  Range_pro[5] = 0.01524;                                                                      
    KE_pro[6] = 2.5;  Range_pro[6] = 0.01809;                                                                       
    KE_pro[7] = 2.75;  Range_pro[7] = 0.02115;                                                                      
    KE_pro[8] = 3;  Range_pro[8] = 0.0244;                                                                          
    KE_pro[9] = 3.5;  Range_pro[9] = 0.03149;                                                                       
    KE_pro[10] = 4;  Range_pro[10] = 0.03934;                                                                       
    KE_pro[11] = 4.5;  Range_pro[11] = 0.04793;                                                                     
    KE_pro[12] = 5;  Range_pro[12] = 0.05724;                                                                       
    KE_pro[13] = 5.5;  Range_pro[13] = 0.06726;                                                                     
    KE_pro[14] = 6;  Range_pro[14] = 0.07798;                                                                       
    KE_pro[15] = 6.5;  Range_pro[15] = 0.08937;                                                                     
    KE_pro[16] = 7;  Range_pro[16] = 0.1014;                                                                        
    KE_pro[17] = 7.5;  Range_pro[17] = 0.1142;                                                                      
    KE_pro[18] = 8;  Range_pro[18] = 0.1276;                                                                        
    KE_pro[19] = 8.5;  Range_pro[19] = 0.1416;                                                                      
    KE_pro[20] = 9;  Range_pro[20] = 0.1563;                                                                        
    KE_pro[21] = 9.5;  Range_pro[21] = 0.1716;                                                                      
    KE_pro[22] = 10;  Range_pro[22] = 0.1875;                                                                       
    KE_pro[23] = 12.5;  Range_pro[23] = 0.2764;                                                                     
    KE_pro[24] = 15;  Range_pro[24] = 0.3801;                                                                       
    KE_pro[25] = 17.5;  Range_pro[25] = 0.4981;                                                                     
    KE_pro[26] = 20;  Range_pro[26] = 0.6301;                                                                       
    KE_pro[27] = 25;  Range_pro[27] = 0.9342;                                                                       
    KE_pro[28] = 27.5;  Range_pro[28] = 1.106;                                                                      
    KE_pro[29] = 30;  Range_pro[29] = 1.29;                                                                         
    KE_pro[30] = 35;  Range_pro[30] = 1.695;                                                                        
    KE_pro[31] = 40;  Range_pro[31] = 2.149;                                                                        
    KE_pro[32] = 45;  Range_pro[32] = 2.649;                                                                        
    KE_pro[33] = 50;  Range_pro[33] = 3.193;                                                                        
    KE_pro[34] = 55;  Range_pro[34] = 3.782;                                                                        
    KE_pro[35] = 60;  Range_pro[35] = 4.413;                                                                        
    KE_pro[36] = 65;  Range_pro[36] = 5.086;                                           
    KE_pro[37] = 70;  Range_pro[37] = 5.799;
    KE_pro[38] = 75;  Range_pro[38] = 6.552;
    KE_pro[39] = 80;  Range_pro[39] = 7.344;
    KE_pro[40] = 85;  Range_pro[40] = 8.173;
    KE_pro[41] = 90;  Range_pro[41] = 9.04;
    KE_pro[42] = 95;  Range_pro[42] = 9.942;
    KE_pro[43] = 100;  Range_pro[43] = 10.88;
    KE_pro[44] = 125;  Range_pro[44] = 16.08;
    KE_pro[45] = 150;  Range_pro[45] = 22.07;
    KE_pro[46] = 175;  Range_pro[46] = 28.77;
    KE_pro[47] = 200;  Range_pro[47] = 36.14;
    KE_pro[48] = 225;  Range_pro[48] = 44.11;
    KE_pro[49] = 250;  Range_pro[49] = 52.64;
    KE_pro[50] = 275;  Range_pro[50] = 61.68;
    KE_pro[51] = 300;  Range_pro[51] = 71.2;
    KE_pro[52] = 350;  Range_pro[52] = 91.54;
    KE_pro[53] = 400;  Range_pro[53] = 113.4;
    KE_pro[54] = 450;  Range_pro[54] = 136.6;
    KE_pro[55] = 500;  Range_pro[55] = 161;
    KE_pro[56] = 550;  Range_pro[56] = 186.3;
    KE_pro[57] = 600;  Range_pro[57] = 212.6;
    KE_pro[58] = 650;  Range_pro[58] = 239.6;
    KE_pro[59] = 700;  Range_pro[59] = 267.4;
    KE_pro[60] = 750;  Range_pro[60] = 295.7;
    KE_pro[61] = 800;  Range_pro[61] = 324.6;
    KE_pro[62] = 850;  Range_pro[62] = 353.9;
    KE_pro[63] = 900;  Range_pro[63] = 383.7;
    KE_pro[64] = 950;  Range_pro[64] = 413.8;
    KE_pro[65] = 1000;  Range_pro[65] = 444.3;

    for(int i=0;i<66;i++){
      Range_pro[i]=Range_pro[i]/1.396;
    }
    TSpline3 *sp1 = new TSpline3("Cubic Spline",Range_pro,KE_pro,66,"b2e2",0,0);
    ofstream myfile2;
    myfile2.open("proton_mpv_dedx_vs_range_0.50.txt");
    for (int i=0; i<4000; i++){
      myfile2<<0.5+i*0.1<<"  "<<dpdx(sp1->Eval(0.5+i*0.1),pitchvalue,Mpro)<<endl;
      dedx_pro.push_back(dpdx(sp1->Eval(0.5+i*0.1),pitchvalue,Mpro));
      rr_pro.push_back(0.5+i*0.1);
    }
    TGraph *pro_dedx_rr=new TGraph(rr_pro.size(),&rr_pro[0],&dedx_pro[0]);
    pro_dedx_rr->SetTitle("MPV dE/dx vs residual range for protons;residual range[cm];dE/dx[MeV/cm]");
    pro_dedx_rr->Write("proton_dedx_rr");
    myfile2.close();
  }
  file1->Close();
  return 0;
 
}

