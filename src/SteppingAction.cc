#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"

#include "G4Event.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction()  
{
  Reset();
}


SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction( const G4Step* aStep ) 
{
  G4Track* aTrack = aStep->GetTrack();
  G4int pid = aTrack->GetDefinition()->GetPDGEncoding();
  G4int pType = -1;

  switch (pid) {
  case   11 : pType = kEMinus ; break;
  case   22 : pType = kGamma  ; break;
  case  -11 : pType = kEPlus  ; break;
  case -211 : pType = kPiMinus; break;
  case  211 : pType = kPiPlus ; break;
  case 2212 : pType = kProton ; break;
  case 2112 : pType = kNeutron; break;
  default   : pType = kOther  ; break;
  }

  theNSteps[pType]++;
  if(aTrack->GetCurrentStepNumber() == 1) theNTracks[pType]++;
 
}

void SteppingAction::Reset() 
{
  for (G4int i = 0 ; i < kNPType ; i++) {
    theNSteps[i] = 0;
    theNTracks[i] = 0;
  }
}

void SteppingAction::Print(const G4Event* evt) 
{
  std::cout << "NStepping> " << evt->GetEventID()+1 << " " 
	    << G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID()+1;
  for(G4int i = 0 ; i <  SteppingAction::kNPType ; i++)
    std::cout << " " << GetNSteps(i);
  for(G4int i = 0 ; i <  SteppingAction::kNPType ; i++)
    std::cout << " " << GetNTracks(i);
  std::cout << std::endl;
}
