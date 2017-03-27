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
#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
//class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger : public G4UImessenger {
public:

    DetectorMessenger(DetectorConstruction*);
    virtual ~DetectorMessenger();

    void SetNewValue(G4UIcommand*, G4String);

private:

    DetectorConstruction* Detector;

    G4UIdirectory* testDir;
    G4UIcmdWithAString* matCmd;
    G4UIcmdWithAString* mat1Cmd;
    G4UIcmdWithADoubleAndUnit* xCmd;
    G4UIcmdWithADoubleAndUnit* yCmd;
    G4UIcmdWithADoubleAndUnit* zCmd;
//    G4UIcmdWithADoubleAndUnit* stepCmd;
    G4UIcmdWithoutParameter* updateCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

