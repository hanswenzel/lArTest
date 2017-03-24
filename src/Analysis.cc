// -----------------------------------------------------
//  _    _        _____         _   
// | |  / \   _ _|_   _|__  ___| |_ 
// | | / _ \ | '__|| |/ _ \/ __| __|
// | |/ ___ \| |   | |  __/\__ \ |_ 
// |_/_/   \_\_|   |_|\___||___/\__|
//                                  
// lArTest: A Geant4 application to study and profile  
//          simulation of physics processes relevant 
//          to liquid Ar TPCs
//
// Author: Hans Wenzel, Fermilab
// -----------------------------------------------------
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
//Root 
#include <TFile.h>          
#include <TTree.h>
//GEANT4 
#include "globals.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryContainer.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4Run.hh"
#include "G4Proton.hh"
#include "G4HadronicProcessStore.hh"
#include "G4NistManager.hh"
// project headers
#include "Analysis.hh"

using namespace std;

Analysis* Analysis::instance = 0;

//------------------------------------------------------------------------------------

Analysis::Analysis() {
#ifdef G4ANALYSIS_USE
#endif
}
//------------------------------------------------------------------------------------

Analysis::~Analysis() {


#ifdef G4ANALYSIS_USE
#endif
}
//------------------------------------------------------------------------------------

Analysis* Analysis::getInstance() {
    if (instance == 0) instance = new Analysis;
    return instance;
}
//------------------------------------------------------------------------------------

void Analysis::book(G4long id0, G4long id1) {
    sprintf(NtupleFileName, "ntuple_%04d.root", int(id0));

    G4cout << "Random Seeds: " << id0 << " " << id1 << G4endl;

    FileNtuple = new TFile(NtupleFileName, "RECREATE", "hadronic interactions hadron-Target");

    //Get and store general information about this run: 
    HeaderTree = new TTree("RunInfo", "Run Info");
    HeaderTree->Branch("targetx", &targetx, "targetx/D");
    HeaderTree->Branch("targety", &targety, "targety/D");
    HeaderTree->Branch("targetz", &targetz, "targetz/D");
    HeaderTree->Branch("density", &rho, "density/D");
    HeaderTree->Branch("aweight", &aweight, "aweight/D");
    HeaderTree->Branch("material", &material);
    HeaderTree->Branch("enerPrimGen", &enerPrimGen, "enerPrimGen/D");
    HeaderTree->Branch("PartName", &PartName);
    //
    EventTree = new TTree("Event", "EventInfo");
    EventTree->Branch("Edep", &edep, "Edep/D");
    EventTree->Branch("x", &x, "x/D");
    EventTree->Branch("y", &y, "y/D");
    EventTree->Branch("z", &z, "z/D");
    EventTree->Branch("StepLength", &StepLength, "StepLength/D");
    EventTree->Branch("Photons", &NrofPhotons, "Photons/I");
    EventTree->Branch("eventID", &eventID, "eventID/I");
}


//------------------------------------------------------------------------------------

void Analysis::finish() {


    FileNtuple->cd();
    WriteHeader();
    WriteEvent();
    FileNtuple->Close();
}

void Analysis::WriteNtuple() {

    //    ProdTree->Fill();

}

void Analysis::SetDetConstInfo(std::vector<Double_t> DCinfo, G4Material* mat) {
    targetx = DCinfo[0];
    targety = DCinfo[1];
    targetz = DCinfo[2];
    rho = DCinfo[3];
    mate = mat;
    material = mate->GetName();
}

void Analysis::SetPrimGenInfo(Double_t enerPrim, G4ParticleDefinition* Part) {
    enerPrimGen = enerPrim;
    particle = Part;
    PartName = particle->GetParticleName();
}

void Analysis::SetRunActInfo(Int_t nevt) {
    numberEvts = nevt;
}

void Analysis::WriteHeader() {
    HeaderTree->Fill();
    HeaderTree->Write();
}

void Analysis::FillEvent(Double_t edep1,
        Double_t x1,
        Double_t y1,
        Double_t z1,
        Double_t StepLength1,
        G4int NrofPhotons1,
        G4int NEvent1) {
    edep = edep1;
    x = x1;
    y = y1;
    z = z1;
    StepLength = StepLength1;
    NrofPhotons = NrofPhotons1;
    eventID = NEvent1;
    EventTree->Fill();
}

void Analysis::WriteEvent() {
    EventTree->Write();
}
