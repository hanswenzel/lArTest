#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> 
#include <cmath>
//#include <math.h>
#include <TCanvas.h> 
#include <TGraph.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TFrame.h>
#include <TFile.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
using namespace std;

//-------------------------------------------------------------------------------
//
// see formula 33.46 in Review of particle physics 2018
//  
//-------------------------------------------------------------------------------

Double_t ceren(Double_t *x, Double_t *par) {
    double lambda=x[0];
    double z = par[0];
    double beta = par[1];
    double n = par[2];
    const double alpha = 7.2973525664E-3;
    return (1 - 1./(beta * beta * n * n)) *(2. * M_PI * alpha * z * z) / (lambda*lambda);
}


void Cerenkov() {
    TF1 *f1 = new TF1("ceren", ceren, 100,500, 3);
    f1->SetParameters(1,1, 1.5);
    f1->SetParNames("z", "beta", "n");
    f1->Draw();
}
