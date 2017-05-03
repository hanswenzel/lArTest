void Ana()
{
  TFile *f = new TFile("lArTest.root");
  TTree *evt = (TTree*)f->Get("ntuple/lArTest");
  Double_t edep;
  Double_t x;
  Double_t y;
  Double_t z;
  Double_t t;
  Double_t StepLength;
  Int_t    NrofPhotons;
  Int_t eventID;
  evt->SetBranchAddress("Edep", &edep);
  evt->SetBranchAddress("x", &x);
  evt->SetBranchAddress("y", &y);
  evt->SetBranchAddress("z", &z);
  evt->SetBranchAddress("t", &t);
  evt->SetBranchAddress("steplength", &StepLength);
  evt->SetBranchAddress("NPhotons", &NrofPhotons);
  evt->SetBranchAddress("Evt", &eventID);
  Int_t sumphotons = 0;
  Float_t sumEdep=0.0;
  Float_t sumEdepcm=0.0;
  Int_t evtid =0;
  TH1F *nphot   = new TH1F("nphot","Nr of Photons",100,0,300000000);
  TH1F *landau  = new TH1F("dEdx/cm","dEdx [MeV]",100,0.,10.);
  TH1F *hedp    = new TH1F("edep","Edep [MeV]",100,0,4000);
  TH1F *dEdx    = new TH1F("dEdx","dEdx [MeV]",900,-4500,4500);
  TH1F *dEdxPh  = new TH1F("dEdxPh","Nphotons ",900,-4500,4500);
  TH1F *time  = new TH1F("time","time ",100,0,100);
  TH1F *steplength  = new TH1F("steplength","steplength",100,0,0.02);
  Int_t nentries = (Int_t)evt->GetEntries();
  for (Int_t i=0;i<nentries;i++) {
    evt->GetEntry(i);
    sumEdep= sumEdep+edep;
    dEdx->Fill(z,edep);
    time->Fill(t);
    if (z>0&&z<1.){
      sumEdepcm=sumEdepcm+edep;
      //     cout <<z<<endl;
    }
    steplength->Fill(StepLength);
    dEdxPh->Fill(z,NrofPhotons);
    sumphotons=sumphotons+NrofPhotons;
    if (eventID!=evtid)                                  // new event
      {
	//	cout << evtid<< "    "<<sumphotons<<"   "<< sumEdep <<endl;
	nphot-> Fill(sumphotons);
	hedp-> Fill(sumEdep);
	landau->Fill(sumEdepcm);
	evtid=eventID;
	sumphotons=0;
	sumEdep=0.0;
	sumEdepcm=0.0;
      }
  }
  //  cout << evtid<< "    "<<sumphotons<<"   "<< sumEdep << "  "  << sumEdepcm <<endl;
  nphot-> Fill(sumphotons);
  hedp-> Fill(sumEdep);
landau->Fill(sumEdepcm);

  TCanvas *c1 = new TCanvas("c1","Edep",200,10,1400,1000);
  c1->SetGrid();
  //gPad->SetLogx();
  //gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  hedp->Draw();
  
  TCanvas *c2 = new TCanvas("c2","NPhotons",200,10,1400,1000);
  c2->SetGrid();
  //  gPad->SetLogx();
  // gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  nphot->Draw();
  TCanvas *c3 = new TCanvas("c3","dEdx",200,10,1400,1000);
  c3->SetGrid();
  //  gPad->SetLogx();
  // gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  dEdx->Draw();
  TCanvas *c4 = new TCanvas("c4","dEdxPh",200,10,1400,1000);
  c4->SetGrid();
  //  gPad->SetLogx();
  // gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  dEdxPh->Draw();
TCanvas *c5 = new TCanvas("c5","dEdxcm",200,10,1400,1000);
  c5->SetGrid();
  //  gPad->SetLogx();
  // gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  landau->Draw();
TCanvas *c6 = new TCanvas("c6","steplength",200,10,1400,1000);
  c6->SetGrid();
  //  gPad->SetLogx();
  // gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  steplength->Draw();
  TCanvas *c7 = new TCanvas("c7","time",200,10,1400,1000);
  c7->SetGrid();
  //  gPad->SetLogx();
  // gPad->SetLogy();
  gPad->SetGridx();
  gPad->SetGridy();
  time->Draw();
}
