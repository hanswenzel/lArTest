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
#include "G4UIcmdWithADoubleAndUnit.hh"
// project headers
#include "ConfigurationManagerMessenger.hh"
#include "ConfigurationManager.hh"
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ConfigurationManagerMessenger::ConfigurationManagerMessenger(ConfigurationManager * mgr1)
: mgr(mgr1) {
    testDir = new G4UIdirectory("/testConfig/");
    testDir->SetGuidance("Examining stepping in geant 4.");
    //    
    anaCmd = new G4UIcmdWithABool("/testConfig/DoAnalysis", this);
    anaCmd->SetGuidance("Let's you select if analysis should be done and root files be created ");
    anaCmd->SetParameterName("doAnalysis", true);
    anaCmd->SetDefaultValue(true);
    //
    steplimitCmd = new G4UIcmdWithABool("/testConfig/DoStepLimit", this);
    steplimitCmd->SetGuidance("Let's you select if step limit should be applied ");
    steplimitCmd->SetParameterName("stepLimit", true);
    steplimitCmd->SetDefaultValue(true);
    //
    slengthCmd = new G4UIcmdWithADoubleAndUnit("/testConfig/steplengthlimit", this);
    slengthCmd->SetGuidance("Set maximum step length");
    slengthCmd->SetParameterName("limitval", false);
    slengthCmd->SetUnitCategory("Length");
    slengthCmd->SetRange("limitval>0");
    //slengthCmd->AvailableForStates(G4State_PreInit);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ConfigurationManagerMessenger::~ConfigurationManagerMessenger() {
    delete testDir;
    delete anaCmd;
    delete steplimitCmd;
    delete slengthCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ConfigurationManagerMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == anaCmd)
        mgr->SetdoAnalysis(anaCmd->GetNewBoolValue(newValue));
    else if (command == steplimitCmd)
        mgr->SetstepLimit(newValue);
    else if (command == slengthCmd)
        mgr->Setlimitval(slengthCmd->GetNewDoubleValue(newValue));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

