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
#include "TrackerSD.hh"
#include "Analysis.hh"
#include "ConfigurationManager.hh"
//#include "SimStep.hh"
//#include "SimTrajectory.hh"
#include "RootIO.hh"
#define UNUSED(expr) do { (void)(expr); } while (0)
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::TrackerSD(G4String name)
: G4VSensitiveDetector(name) {
    //   std::vector<SimStep*> stVector;
    std::cout << "TrackerSD: constructor" << std::endl;
    tmap = new std::map<int, SimTrajectory*>();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::~TrackerSD() {
    //RootIO::GetInstance()->Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::Initialize(G4HCofThisEvent*) {
    //  UNUSED(hce); // avoiding unused parameter ‘HCE’ compiler message 
    //std::cout << "TrackerSD: Initialize" << std::endl;
    //stVector.clear();
    //tmap->clear();
    for (auto itr = tmap->begin(); itr != tmap->end(); itr++) {
        delete (itr->second);
    }
    tmap->clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TrackerSD::ProcessHits(G4Step* aStep,
        G4TouchableHistory*) {
    G4double edep = aStep->GetTotalEnergyDeposit();

    if (edep == 0.) return false;

    //  ConfigurationManager* cfMgr = ConfigurationManager::getInstance();
    /* 
        std::map<G4String, int> *mapOfntIDs = cfMgr->getMapOfntIDs();
        //const G4String name = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName() + "_Tracker";
        const G4String name = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName() + "_Tracker";
        //G4int ID = 0;
        std::map<G4String, int>::iterator iter = mapOfntIDs->find(name);
        G4int ID = (*mapOfntIDs)[name];
#include "SimTrajectory.hh"
     */
    G4Track* aTrack = aStep->GetTrack();
    /*
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
    float xpos = (float) aStep->GetPostStepPoint()->GetPosition().getX();
    float ypos = (float) aStep->GetPostStepPoint()->GetPosition().getY();
    float zpos = (float) aStep->GetPostStepPoint()->GetPosition().getZ();
    ///SimStep* st = new SimStep(xpos, ypos, zpos, (float) aStep->GetStepLength(), (float) aStep->GetPostStepPoint()->GetGlobalTime(), (float) aStep->GetTotalEnergyDeposit());
    ///stVector.push_back(st);
    //G4Track* aTrack = aStep->GetTrack();

    auto itr = tmap->find(aTrack->GetTrackID());
    if (itr == tmap->end()) // new track
    {
        SimTrajectory* simtr = new SimTrajectory(aTrack->GetTrackID());
        std::vector<SimStep*>* simsteps = simtr->GetTrajectory();
        SimStep* st = new SimStep(xpos, ypos, zpos, (float) aStep->GetStepLength(), (float) aStep->GetPostStepPoint()->GetGlobalTime(), (float) aStep->GetTotalEnergyDeposit());
        simsteps->push_back(st);
        tmap->insert(std::make_pair(aTrack->GetTrackID(), simtr));
    } else {
        SimTrajectory* simtr = itr->second;
        std::vector<SimStep*>* simsteps = simtr->GetTrajectory();
        SimStep* st = new SimStep(xpos, ypos, zpos, (float) aStep->GetStepLength(), (float) aStep->GetPostStepPoint()->GetGlobalTime(), (float) aStep->GetTotalEnergyDeposit());
        simsteps->push_back(st);
    }

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::EndOfEvent(G4HCofThisEvent*) {
    RootIO::GetInstance()->Write(tmap);
    //RootIO::GetInstance()->Write(&stVector);
    //std::cout<<"****************:   "<<stVector.size()<<std::endl;
    for (auto itr = tmap->begin(); itr != tmap->end(); itr++) {
        for (auto itr2=(itr->second)->GetTrajectory()->begin();itr2!=(itr->second)->GetTrajectory()->end();itr2++)
        {delete *itr2;
        }
        itr->second->GetTrajectory()->clear();
        delete (itr->second);
    }
    tmap->clear();
    //    for (auto itr = tmap->begin(); itr != tmap->end(); itr++)
    //        cout << "ID:  " << itr->first << "  steps:  " << (itr->second)->GetTrajectory()->size() << '\n';
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
