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

#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithABool;
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

//    G4UIcmdWithADoubleAndUnit* stepCmd;
    G4UIcmdWithABool*   hitsCmd;    
    G4UIcmdWithABool*   analCmd;    
//    G4UIcmdWithABool*   gdmlCmd;    
    G4UIcmdWithoutParameter* updateCmd;

    //control for performance profiling sevice
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

