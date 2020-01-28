#include "HadStackingAction.hh"
//#include "HadAnalysis.hh"
#include "G4Track.hh"
#include "G4HadronicProcessStore.hh"
#include "G4NistManager.hh"
//#include "HadEventAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadStackingAction::HadStackingAction() {
    killSecondary = false;
    pname = "";
    elm = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadStackingAction::~HadStackingAction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
HadStackingAction::ClassifyNewTrack(const G4Track* aTrack) {
    G4ClassificationOfNewTrack status = fUrgent;
    //G4EventManager EvtManager = G4EventManager::GetEventManager();
    //    HadEvtAct = (HadEventAction*) (EvtManager -> GetUserEventAction());
    bool procNP = false;
    bool procPi = false;
    bool procK = false;

    if ((aTrack->GetTrackStatus() == fAlive) && (aTrack->GetParentID() == 1)) {

        //if( aTrack->GetCreatorProcess()->GetProcessName() != "hIoni" ) {
        //std::cout << "particle " << aTrack->GetDefinition()->GetParticleName() << " process " << aTrack->GetCreatorProcess()->GetProcessName() << std::endl;
        //}
        if (aTrack->GetCreatorProcess()->GetProcessName() == "muonNuclear") {
            G4cout << "muonNuclear:  " << aTrack->GetParticleDefinition()->GetParticleName() << "  Ekin:  " << aTrack->GetKineticEnergy() << G4endl;
            return status;
        }

        procNP = (aTrack->GetCreatorProcess()->GetProcessName() == "protonInelastic") || (aTrack->GetCreatorProcess()->GetProcessName() == "neutronInelastic");

        procPi = (aTrack->GetCreatorProcess()->GetProcessName() == "pi+Inelastic") || (aTrack->GetCreatorProcess()->GetProcessName() == "pi-Inelastic");

        if (procPi) {
            G4cout << "pi Inelastic:" << aTrack->GetParticleDefinition()->GetParticleName() << "  " << aTrack->GetTrackStatus() 
                            //<< aTrack->GetStep()->IsLastStepInVolume()
                            << G4endl;
            if (aTrack->GetParticleDefinition()->GetParticleName() == "pi0") {
                G4cout << aTrack->GetParticleDefinition()->GetParticleName() << G4endl;
            }
        }

        procK = (aTrack->GetCreatorProcess()->GetProcessName() == "kaon+Inelastic") || (aTrack->GetCreatorProcess()->GetProcessName() == "kaon-Inelastic");

        //  if (procNP || procPi || procK) HadEvtAct->AddTrack(aTrack, 1);

        //   if (aTrack->GetCreatorProcess()->GetProcessName() == "hadElastic") HadEvtAct->AddTrack(aTrack, 2);

    }
    //stack or delete secondaries
    if (killSecondary) status = fKill;

    return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......