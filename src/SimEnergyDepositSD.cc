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
// Geant4 headers
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4SteppingManager.hh"
#include "G4Track.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
// project headers
#include "SimEnergyDeposit.hh"
#include "SimEnergyDepositSD.hh"
#include "Analysis.hh"
#include "ConfigurationManager.hh"
//#include "SimEnergyDeposit.hh"
//#include "SimTrajectory.hh"
#include "RootIO.hh"
#define UNUSED(expr) do { (void)(expr); } while (0)
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SimEnergyDepositSD::SimEnergyDepositSD(G4String name)
: G4VSensitiveDetector(name) {
    //   std::vector<SimEnergyDeposit*> stVector;
    std::cout << "SimEnergyDepositSD: constructor" << std::endl;
    sedvec = new std::vector<SimEnergyDeposit*>();
    //        sedvec = new std::map<int, SimTrajectory*>();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SimEnergyDepositSD::~SimEnergyDepositSD() {
    delete sedvec;
    //RootIO::GetInstance()->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimEnergyDepositSD::Initialize(G4HCofThisEvent*) {
    //  UNUSED(hce); // avoiding unused parameter ‘HCE’ compiler message 
    //std::cout << "SimEnergyDepositSD: Initialize" << std::endl;
    //stVector.clear();
    sedvec->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool SimEnergyDepositSD::ProcessHits(G4Step* aStep,
        G4TouchableHistory*) {
    G4double edep = aStep->GetTotalEnergyDeposit();

    if (edep == 0.) return false;

    /*    G4Track* aTrack = aStep->GetTrack();
        std::cout << aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName()
                << " Track ID: " << aTrack->GetTrackID() << "  Parent ID:  " << aTrack->GetParentID()
                << " step number:  " << aTrack->GetCurrentStepNumber() << " Edep:  " << edep * MeV
                << " First:  " << aStep->IsFirstStepInVolume()
                << " Last:  " << aStep->IsLastStepInVolume()
                << " pos:  " << aStep->GetPostStepPoint()->GetPosition().getX() << ","
                << aStep->GetPostStepPoint()->GetPosition().getY() << ","
                << aStep->GetPostStepPoint()->GetPosition().getZ()
                << std::endl;
     */
    /*
        float xpos = (float) aStep->GetPostStepPoint()->GetPosition().getX();
        float ypos = (float) aStep->GetPostStepPoint()->GetPosition().getY();
        float zpos = (float) aStep->GetPostStepPoint()->GetPosition().getZ();
     */
    SimEnergyDeposit* sdep = new SimEnergyDeposit(0, 0, 0,
            (float) aStep->GetPreStepPoint()->GetPosition().getX() / CLHEP::cm,
            (float) aStep->GetPreStepPoint()->GetPosition().getY() / CLHEP::cm,
            (float) aStep->GetPreStepPoint()->GetPosition().getZ() / CLHEP::cm,
            (float) aStep->GetPostStepPoint()->GetPosition().getX() / CLHEP::cm,
            (float) aStep->GetPostStepPoint()->GetPosition().getY() / CLHEP::cm,
            (float) aStep->GetPostStepPoint()->GetPosition().getZ() / CLHEP::cm,
            (float) aStep->GetPreStepPoint()->GetGlobalTime() / CLHEP::ns,
            (float) aStep->GetPostStepPoint()->GetGlobalTime() / CLHEP::ns,
            (float) aStep->GetTotalEnergyDeposit() / CLHEP::MeV);
    sedvec->push_back(sdep);

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SimEnergyDepositSD::EndOfEvent(G4HCofThisEvent*) {
    RootIO::GetInstance()->Write(sedvec);
    
        for (auto itr = sedvec->begin(); itr != sedvec->end(); itr++) {
        {delete *itr;
        }
    }
    sedvec->clear();
    
    
    //RootIO::GetInstance()->Write(&stVector);
    //std::cout<<"****************:   "<<stVector.size()<<std::endl;
    //  for (auto itr = sedvec->begin(); itr != sedvec->end(); itr++)
    //      cout << "ID:  " << itr->first << "  steps:  " << (itr->second)->GetTrajectory()->size() << '\n';
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
