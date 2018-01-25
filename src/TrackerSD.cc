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
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::TrackerSD(G4String name)
: G4VSensitiveDetector(name) {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackerSD::~TrackerSD() {

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::Initialize(G4HCofThisEvent* /*hce*/) {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool TrackerSD::ProcessHits(G4Step* aStep,
        G4TouchableHistory*) {
    G4double edep = aStep->GetTotalEnergyDeposit();
    if (edep == 0.) return false;
    if (aStep->GetTrack()->GetDynamicParticle()->GetCharge()==0) return false;
    G4int photons = 0;
    G4SteppingManager* fpSteppingManager = G4EventManager::GetEventManager()
            ->GetTrackingManager()->GetSteppingManager();
    G4StepStatus stepStatus = fpSteppingManager->GetfStepStatus();
    if (stepStatus != fAtRestDoItProc) {
        G4ProcessVector* procPost = fpSteppingManager->GetfPostStepDoItVector();
        size_t MAXofPostStepLoops = fpSteppingManager->GetMAXofPostStepLoops();
        for (size_t i3 = 0; i3 < MAXofPostStepLoops; i3++) {
            /*
            if ((*procPost)[i3]->GetProcessName() == "Cerenkov") {
                G4Cerenkov* proc =(G4Cerenkov*) (*procPost)[i3];
                photons+=proc->GetNumPhotons();
            }
             */
            if ((*procPost)[i3]->GetProcessName() == "Scintillation") {
                G4Scintillation* proc1 = (G4Scintillation*) (*procPost)[i3];
                photons += proc1->GetNumPhotons();
            }
        }
    }
    ConfigurationManager* cfMgr = ConfigurationManager::getInstance();
    if (cfMgr->GetdoAnalysis()) {
        // get analysis manager
        auto analysisManager = G4AnalysisManager::Instance();
        // fill ntuple
        analysisManager->FillNtupleDColumn(0,0, edep / MeV);
        analysisManager->FillNtupleDColumn(0,1, aStep->GetTrack()->GetPosition().x() / cm);
        analysisManager->FillNtupleDColumn(0,2, aStep->GetTrack()->GetPosition().y() / cm);
        analysisManager->FillNtupleDColumn(0,3, aStep->GetTrack()->GetPosition().z() / cm);
        analysisManager->FillNtupleDColumn(0,4, aStep->GetTrack()->GetGlobalTime() / ns);
        analysisManager->FillNtupleDColumn(0,5, aStep->GetStepLength() / cm);
        analysisManager->FillNtupleIColumn(0,6, photons);
        analysisManager->FillNtupleIColumn(0,7, G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
        analysisManager->AddNtupleRow(0);
    }
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackerSD::EndOfEvent(G4HCofThisEvent*) {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
