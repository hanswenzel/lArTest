  Double_t boxrecomfun(double x) {
    // Nucl.Instrum.Meth.A523:275-286,2004
    double fGeVToElectrons = 4.237e+07;
    double fRecombA        = 0.800;
    double fRecombk        = 0.0486;
    double fModBoxA        = 0.930;
    double fModBoxB        = 0.212;
    double EFieldStep      = 0.5; 
    double dEdx=x; 
    double Xi = fModBoxB * dEdx / EFieldStep;
    double recomb = log(fModBoxA + Xi) / Xi;
    return recomb;
  }
void recom()
{
  TCanvas *c = new TCanvas("c","electron recombination",800,600);
  //gPad->SetMaximum(1.);
  //gPad->SetMinimum(0.01);
  gPad->SetLogy();
  //gPad->SetLogx();
  TF1 *fa3 = new TF1("fa3","boxrecomfun(x)",0,15);
  fa3->SetTitle(";stopping power -dE/dx[MeVg^{-1}cm^{2}]; electron recombination");
  fa3->SetMaximum(1.);
  fa3->SetMinimum(0.01);
  fa3->Draw();
}
