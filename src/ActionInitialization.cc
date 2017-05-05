#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

//ActionInitialization::ActionInitialization
//                      (DetectorConstruction* detConstruction)
// : G4VUserActionInitialization(),
//   fDetConstruction(detConstruction)
//{}
ActionInitialization::ActionInitialization(): G4VUserActionInitialization(){}
ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);
  EventAction* eventAction = new EventAction;
  SetUserAction(eventAction);
  SetUserAction(new SteppingAction);
}  
