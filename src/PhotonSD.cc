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

#include "PhotonSD.hh"
#include "G4VProcess.hh"
#include "G4OpticalPhoton.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonSD::PhotonSD(const G4String& name,
        const G4String& /* hitsCollectionName */)
: G4VSensitiveDetector(name) {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhotonSD::Initialize(G4HCofThisEvent* HCE) {
 
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhotonSD::~PhotonSD() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool PhotonSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {

    G4Track* theTrack = aStep->GetTrack();
                 
    // we only deal with optical Photons:
    if (theTrack->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
        return false;
    }
    G4double theEdep = theTrack->GetTotalEnergy();
    const G4VProcess * thisProcess = theTrack->GetCreatorProcess();
    
    G4String processname;
    if(thisProcess !=NULL)
         processname = thisProcess->GetProcessName();
    else
        processname = "No Process";
    G4int theCreationProcessid;
    if (processname == "Cerenkov") {
        theCreationProcessid = 0;
    } else if (processname == "Scintillation") {
        theCreationProcessid = 1;
    } else {
        theCreationProcessid = -1;
    }
    /*
    PhotonHit* newHit = new PhotonHit();
    newHit->SetProcessID(theCreationProcessid);
    newHit->SetEdep(theEdep);
    newHit->SetPos(aStep->GetPostStepPoint()->GetPosition());
    newHit->SetTime(theTrack->GetGlobalTime());
    photonCollection->insert(newHit);
*/
    theTrack->SetTrackStatus(fStopAndKill);
    return true;
}
void PhotonSD::EndOfEvent(G4HCofThisEvent*) {
}
