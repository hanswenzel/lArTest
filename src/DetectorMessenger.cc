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

    matCmd = new G4UIcmdWithAString("/Detector/TargetMat", this);
    matCmd->SetGuidance("Select Material for the target");
    matCmd->SetParameterName("tMaterial", false);
    matCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    mat1Cmd = new G4UIcmdWithAString("/Detector/WorldMat", this);
    mat1Cmd->SetGuidance("Select Material for world");
    mat1Cmd->SetParameterName("wMaterial", false);
    mat1Cmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    xCmd = new G4UIcmdWithADoubleAndUnit("/Detector/TargetX", this);
    xCmd->SetGuidance("Set x half length of the target");
    xCmd->SetParameterName("lengthX", false);
    xCmd->SetUnitCategory("Length");
    xCmd->SetRange("lengthX>0");
    xCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    yCmd = new G4UIcmdWithADoubleAndUnit("/Detector/TargetY", this);
    yCmd->SetGuidance("Set y half length of the target");
    yCmd->SetParameterName("lengthY", false);
    yCmd->SetUnitCategory("Length");
    yCmd->SetRange("lengthY>0");
    yCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    zCmd = new G4UIcmdWithADoubleAndUnit("/Detector/TargetZ", this);
    zCmd->SetGuidance("Set z half length of the target");
    zCmd->SetParameterName("lengthZ", false);
    zCmd->SetUnitCategory("Length");
    zCmd->SetRange("lengthZ>0");
    zCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

//    stepCmd = new G4UIcmdWithADoubleAndUnit("/Detector/MaxStepLength", this);
//    stepCmd->SetGuidance("Set Maximum Step Length");
//    stepCmd->SetParameterName("maxStep", false);
//    stepCmd->SetUnitCategory("Length");
//    stepCmd->SetRange("maxStep>0");
//    stepCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    updateCmd = new G4UIcmdWithoutParameter("/Detector/Update", this);
    updateCmd->SetGuidance("Update geometry.");
    updateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
    updateCmd->SetGuidance("if you changed geometrical value(s)");
    updateCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger() {
    delete matCmd;
    delete mat1Cmd;
    delete xCmd;
    delete yCmd;
    delete zCmd;
    delete updateCmd;
    delete testDir;
   // delete stepCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == matCmd)
        Detector->SetTargetMaterial(newValue);
    else if (command == mat1Cmd)
        Detector->SetWorldMaterial(newValue);
    else if (command == xCmd)
        Detector->SetTargetX(xCmd->GetNewDoubleValue(newValue));
    else if (command == yCmd)
        Detector->SetTargetY(yCmd->GetNewDoubleValue(newValue));
    else if (command == zCmd)
        Detector->SetTargetZ(zCmd->GetNewDoubleValue(newValue));
    //else if (command == stepCmd)
    //    Detector->SetMaxStepLength(stepCmd->GetNewDoubleValue(newValue));
    else if (command == updateCmd)
        Detector->UpdateGeometry();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

