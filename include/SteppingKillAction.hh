#ifndef SteppingKillAction_H
#define SteppingKillAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class G4Event;

class SteppingKillAction : public G4UserSteppingAction {

public:

  SteppingKillAction();
  virtual ~SteppingKillAction();

  virtual void UserSteppingAction( const G4Step* );

 
};
#endif
