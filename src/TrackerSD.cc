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
//#include "Analysis.hh"
//#include "ConfigurationManager.hh"
#include "SimStep.hh"
#include "SimTrajectory.hh"
#include "SimCluster.hh"
#include "RootIO.hh"
#define UNUSED(expr) do { (void)(expr); } while (0)
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::TrackerSD(G4String name)
: G4VSensitiveDetector(name) {
    //   std::vector<SimStep*> stVector;
    std::cout << "TrackerSD: constructor" << std::endl;
    scVector = std::vector<SimCluster*>();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::~TrackerSD() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::Initialize(G4HCofThisEvent*) {
    //  UNUSED(hce); // avoiding unused parameter ‘HCE’ compiler message 

    for (auto itr = scVector.begin(); itr != scVector.end(); itr++) {
        delete (*itr);
    }
    scVector.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TrackerSD::ProcessHits(G4Step* aStep,
        G4TouchableHistory*) {



    G4double edep = aStep->GetTotalEnergyDeposit();

    if (edep == 0.) return false;
    G4Track* aTrack = aStep->GetTrack();
    float xpos = (float) aStep->GetPostStepPoint()->GetPosition().getX();
    float ypos = (float) aStep->GetPostStepPoint()->GetPosition().getY();
    float zpos = (float) aStep->GetPostStepPoint()->GetPosition().getZ();
    std::cout << "size of vector:  " << scVector.size() << std::endl;
    if (scVector.size() == 0) {
        SimTrajectory* simtr = new SimTrajectory(aTrack->GetTrackID());
        std::vector<SimStep*>* simsteps = simtr->GetTrajectory();
        SimStep* firstst = new SimStep((float) aStep->GetPreStepPoint()->GetPosition().getX() / CLHEP::cm,
                (float) aStep->GetPreStepPoint()->GetPosition().getY() / CLHEP::cm,
                (float) aStep->GetPreStepPoint()->GetPosition().getZ() / CLHEP::cm,
                0.0,
                (float) aStep->GetPreStepPoint()->GetGlobalTime(),
                0.0);
        simsteps->push_back(firstst);
        SimStep* stpre = new SimStep(xpos, ypos, zpos, (float) aStep->GetStepLength(), (float) aStep->GetPostStepPoint()->GetGlobalTime(), (float) aStep->GetTotalEnergyDeposit());
        simsteps->push_back(stpre);
        SimStep* st = new SimStep(xpos, ypos, zpos, (float) aStep->GetStepLength(), (float) aStep->GetPostStepPoint()->GetGlobalTime(), (float) aStep->GetTotalEnergyDeposit());
        simsteps->push_back(st);
        std::vector<SimTrajectory*>* trajvec = new std::vector<SimTrajectory*>();
        trajvec->push_back(simtr);
        SimCluster* simclus = new SimCluster(aTrack->GetTrackID(), trajvec);
        scVector.push_back(simclus);
    } else {
         std::cout <<  " sear"<<std::endl;
        for (auto it = std::begin(scVector); it != std::end(scVector); ++it) {
            std::cout << *it << "\n";
            std::cout << "size of map: " << (*it)->GetCluster().size() << std::endl;
            std::map<int, std::vector<SimTrajectory*>*> tm = (*it)->GetCluster();
            for (auto itr3 = tm.find(aTrack->GetTrackID()); itr3 != tm.end(); itr3++)
            {
               std::cout <<  " Found:  " <<aTrack->GetTrackID()<<std::endl;
            }
            
            
        }
    }
            //auto itr = tm.
            //auto itr = (*it)->GetCluster().find(aTrack->GetTrackID());
            //   (*it)->
//            auto itr = (*it)->GetCluster().find(aTrack->GetTrackID());
//            if (itr == scVector->end()) // new track
//            {
//                std::cout << "track Id:  " << aTrack->GetTrackID() << "  Parent ID:  " << aTrack->GetParentID() << std::endl;
//            }
 //       }
        //        scVector.insert(std::make_pair(aTrack->GetTrackID(), simclus));
        /*); 
        //        scVector.insert(std::make_pair(aTrack->GetTrackID(), trajvec*));
        //*
                    SimCluster* simclus = new SimCluster(aTrack->GetTrackID()); 
                          SimCluster(aTrack->GetTrackID(),std::vector<SimTrajectory*>);
                    //scVector.insert(std::make_pair(aTrack->GetTrackID(), simtr));
         //*/

        /*
        for (auto it = std::begin(scVector); it != std::end(scVector); ++it) {
            std::cout << *it << "\n";
            std::cout << "size of map: " << (*it)->GetCluster().size() << std::endl;
            auto itr = (*it)->GetCluster().find(aTrack->GetTrackID());
    
    
            auto itr = scVector->find(aTrack->GetTrackID());
            if (itr == scVector->end()) // new track
            {
                std::cout << "track Id:  " << aTrack->GetTrackID() << "  Parent ID:  " << aTrack->GetParentID() << std::endl;
                auto itr2 = scVector->find(aTrack->GetParentID());
                if (itr2 != scVector->end()) // new track
                {
                    std::cout << "Parent in list " << std::endl;
                }
                std::cout << " new trajectory is first in Volume:  " << aStep->IsFirstStepInVolume() << "  edep: " << edep * MeV << std::endl;
                //if (edep==0.0) {
                //   std::cout << " : " <<aStep->GetTrack()->GetDynamicParticle()->GetParticleDefinition()->GetParticleName()<<std::endl;
                //} else
                //{
                //   std::cout <<  std::endl;
                //} 
                SimTrajectory* simtr = new SimTrajectory(aTrack->GetTrackID());
                std::cout << "Volume name (prestep: " << aStep->GetPreStepPoint()->GetTouchable()->GetVolume()->GetName() << std::endl;
                std::vector<SimStep*>* simsteps = simtr->GetTrajectory();
                SimStep* firstst = new SimStep((float) aStep->GetPreStepPoint()->GetPosition().getX() / CLHEP::cm,
                        (float) aStep->GetPreStepPoint()->GetPosition().getY() / CLHEP::cm,
                        (float) aStep->GetPreStepPoint()->GetPosition().getZ() / CLHEP::cm,
                        0.0,
                        (float) aStep->GetPreStepPoint()->GetGlobalTime(),
                        0.0);
                simsteps->push_back(firstst);
                SimStep* st = new SimStep(xpos, ypos, zpos, (float) aStep->GetStepLength(), (float) aStep->GetPostStepPoint()->GetGlobalTime(), (float) aStep->GetTotalEnergyDeposit());
                simsteps->push_back(st);
                scVector->insert(std::make_pair(aTrack->GetTrackID(), simtr));
            } else {
                SimTrajectory* simtr = itr->second;
                std::vector<SimStep*>* simsteps = simtr->GetTrajectory();
                SimStep* st = new SimStep(xpos, ypos, zpos, (float) aStep->GetStepLength(), (float) aStep->GetPostStepPoint()->GetGlobalTime(), (float) aStep->GetTotalEnergyDeposit());
                simsteps->push_back(st);
            }
         */
        return true;
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

    void TrackerSD::EndOfEvent(G4HCofThisEvent*) {
        /*
        RootIO::GetInstance()->Write(scVector);
        //RootIO::GetInstance()->Write(&stVector);
        //std::cout<<"****************:   "<<stVector.size()<<std::endl;
        for (auto itr = scVector->begin(); itr != scVector->end(); itr++) {
            for (auto itr2 = (itr->second)->GetTrajectory()->begin(); itr2 != (itr->second)->GetTrajectory()->end(); itr2++) {
                delete *itr2;
            }
            itr->second->GetTrajectory()->clear();
            delete (itr->second);
        }
        scVector->clear();
        //    for (auto itr = scVector->begin(); itr != scVector->end(); itr++)
        //        cout << "ID:  " << itr->first << "  steps:  " << (itr->second)->GetTrajectory()->size() << '\n';
         */
    }

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
