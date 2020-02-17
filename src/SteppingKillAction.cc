#include "SteppingKillAction.hh"

#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

SteppingKillAction::SteppingKillAction() {
}

SteppingKillAction::~SteppingKillAction() {
}

void SteppingKillAction::UserSteppingAction(const G4Step* aStep) {

    G4Track* aTrack = aStep->GetTrack();
    G4cout << "Track ID: " << aTrack->GetTrackID() << G4endl;
    if (aTrack->GetTrackID() == 1) // primary track
    {
        bool kill = true;
        if (kill) {
            aTrack->SetTrackStatus(fStopAndKill);
        }
    }
}



