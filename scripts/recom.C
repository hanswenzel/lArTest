
Double_t boxrecomfun(double x) {
  double fGeVToElectrons = 4.237e+07;
  double fRecombA        = 0.800;
  double fRecombk        = 0.0486;
  double fModBoxA        = 0.930;
  double fModBoxB        = 0.212;
  double EFieldStep      = 0.5; 
  double dEdx=x; // alpha Ekin 5MeV
  double Xi = fModBoxB * dEdx / EFieldStep;
  double recomb = log(fModBoxA + Xi) / Xi;
  return recomb;
  }

void recom()
{
TCanvas* c = new TCanvas("dEdx canvas", "dE/dx", 200, 10, 700, 500);
    c->SetGrid();
    c->GetFrame()->SetBorderSize(12);
    //    c->SetLogx();
    c->SetLogy();
    TF1 *fa3= new TF1("fa3","boxrecomfun(x)",0.,1500);
    fa3->SetMinimum(0.01);
    fa3->SetMaximum(1.);
    fa3->Draw();

}
