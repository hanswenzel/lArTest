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

#ifndef ANALYSIS_HH
#define ANALYSIS_HH

//#include "globals.hh"

//root
#include "RtypesCore.h"
//#include "TSystem.h"
#include <vector>


class G4ParticleDefinition;
class TFile;
class TTree;
class G4Material;

class Analysis {
public:

    Analysis();
    ~Analysis();

    void book(G4long, G4long);

    void FillEvent(Double_t edep,
            Double_t x,
            Double_t y,
            Double_t z,
            Double_t StepLength,
            G4int NrofPhotons,
            G4int eventID);
    void WriteEvent();
    void WriteNtuple();
    void WriteHeader();
    void SetDetConstInfo(std::vector<Double_t> DCinfo, G4Material* mat);
    void SetPrimGenInfo(Double_t enerPrim, G4ParticleDefinition* Part);
    void SetRunActInfo(Int_t nevt);
    static Analysis* getInstance();
    void finish();


private:
    static Analysis* instance;

    char NtupleFileName[50];
    TFile* FileNtuple;
    TTree* HeaderTree;
    TTree* EventTree;

    //info for header record:
    Double_t targetx;
    Double_t targety;
    Double_t targetz;
    Double_t rho;
    Double_t aweight;
    Double_t edep;
    Double_t x;
    Double_t y;
    Double_t z;
    Double_t enerPrimGen;
    std::string PartName;

    
    // info  for Event record
    Double_t StepLength;
    G4int NrofPhotons;
    G4int eventID;
    G4Material* mate;
    std::string material;
    std::string namePrim;
    G4ParticleDefinition* particle;
    Int_t numberEvts;
};

#endif 
