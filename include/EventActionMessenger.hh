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
#ifndef EventActionMessenger_h
#define EventActionMessenger_h 1

#include "G4UImessenger.hh"

class EventAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class G4UIdirectory;
class G4UIcmdWithABool;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventActionMessenger: public G4UImessenger
{
public:

  EventActionMessenger(EventAction*);
  virtual ~EventActionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:

  EventAction*          eventAction;   
  G4UIcmdWithAString*   drawCmd;
  G4UIcmdWithAnInteger* printCmd;    
  G4UIcmdWithAnInteger* dCmd;    

  G4UIdirectory*        profDirectory;
  G4UIcmdWithABool*     profCmd;    

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
