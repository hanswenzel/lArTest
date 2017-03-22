void Ana()
{
  TFile *f = new TFile("ntuple_0007.root");
  TTree *evt = (TTree*)f->Get("Event");
  Double_t edep;
  Double_t x;
  Double_t y;
  Double_t z;
  Double_t StepLength;
  Int_t    NrofPhotons;
  Int_t eventID;
  evt->SetBranchAddress("Edep", &edep);
  evt->SetBranchAddress("x", &x);
  evt->SetBranchAddress("y", &y);
  evt->SetBranchAddress("z", &z);
  evt->SetBranchAddress("StepLength", &StepLength);
  evt->SetBranchAddress("Photons", &NrofPhotons);
  evt->SetBranchAddress("eventID", &eventID);
  Int_t sumphotons = 0;
  Float_t sumEdep=0.0;
  Int_t evtid =0;
  TH1F *nphot   = new TH1F("nphot","Nr of Photons",100,0,300000000);
  TH1F *hedp   = new TH1F("edep","Edep [MeV]",100,0,4000);
  Int_t nentries = (Int_t)evt->GetEntries();
  for (Int_t i=0;i<nentries;i++) {
    evt->GetEntry(i);
    sumEdep= sumEdep+edep;
    sumphotons=sumphotons+NrofPhotons;
    if (eventID!=evtid) 
      {
	cout << evtid<< "    "<<sumphotons<<"   "<< sumEdep <<endl;
	nphot-> Fill(sumphotons);
	hedp-> Fill(sumEdep);
	evtid=eventID;
	sumphotons=0;
	sumEdep=0.0;
      }
  }
  cout << evtid<< "    "<<sumphotons<<"   "<< sumEdep << endl;
  nphot-> Fill(sumphotons);
  hedp-> Fill(sumEdep);
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
}
