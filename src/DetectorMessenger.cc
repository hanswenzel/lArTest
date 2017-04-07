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
// Geant4 headers
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
// project headers
#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction * Det)
: Detector(Det) {
    testDir = new G4UIdirectory("/Detector/");
    testDir->SetGuidance("G4 Detector stores all steps within the TrackerSD. The test allows to apply step limits.");


    updateCmd = new G4UIcmdWithoutParameter("/Detector/Update", this);
    updateCmd->SetGuidance("Update geometry.");
    updateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
    updateCmd->SetGuidance("if you changed geometrical value(s)");
    updateCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    //UI for performance profiling
    hitsCmd = new G4UIcmdWithABool("/Detector/WriteHits",this);
    hitsCmd->SetGuidance("Enable/disable tracker sensitive detector");
    hitsCmd->SetParameterName("WriteHits",true);
    hitsCmd->SetDefaultValue(true);
    hitsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    analCmd = new G4UIcmdWithABool("/Detector/DoAnalysis",this);
    analCmd->SetGuidance("Enable/disable analysis");
    analCmd->SetParameterName("DoAnalysis",true);
    analCmd->SetDefaultValue(true);
    analCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger() {

    delete updateCmd;
    delete testDir;
   // delete stepCmd;
    delete hitsCmd;
    delete analCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {

    if(command == hitsCmd)
        Detector->SetWriteHits(hitsCmd->GetNewBoolValue(newValue));
    else if(command == analCmd)
        Detector->SetDoAnalysis(analCmd->GetNewBoolValue(newValue));
    else if (command == updateCmd)
        Detector->UpdateGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

