#ifndef SteppingAction_H
#define SteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class G4Event;

class SteppingAction : public G4UserSteppingAction {

public:

  SteppingAction();
  virtual ~SteppingAction();

public:

  virtual void UserSteppingAction( const G4Step* );

  G4int GetNSteps(G4int kType) { return theNSteps[kType] ; };
  G4int GetNTracks(G4int kType) { return theNTracks[kType] ; };

  enum PType { kGamma=0, kEMinus, kEPlus, kPiPlus, kPiMinus, 
	       kProton, kNeutron, kOther, kNPType};

  void Reset();
  void Print(const G4Event* evt);

private:

  G4int theNSteps[kNPType];
  G4int theNTracks[kNPType];
 
};
#endif
