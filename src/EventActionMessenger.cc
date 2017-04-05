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
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIdirectory.hh"

// project headers
#include "EventActionMessenger.hh"
#include "EventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
using namespace std;
EventActionMessenger::EventActionMessenger(EventAction* EvAct)
 : eventAction(EvAct)
{ 
  drawCmd = new G4UIcmdWithAString("/testhadr/DrawTracks", this);
  drawCmd->SetGuidance("Draw the tracks in the event");
  drawCmd->SetGuidance("  Choice : neutral, charged, all");
  drawCmd->SetParameterName("choice",true);
  drawCmd->SetDefaultValue("all");
  drawCmd->SetCandidates("none charged all");
  drawCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  printCmd = new G4UIcmdWithAnInteger("/testhadr/PrintModulo",this);
  printCmd->SetGuidance("Print events modulo n");
  printCmd->SetParameterName("EventNb",false);
  printCmd->SetRange("EventNb>0");
  printCmd->AvailableForStates(G4State_PreInit,G4State_Idle);      

  dCmd = new G4UIcmdWithAnInteger("/testhadr/DebugEvent",this);
  dCmd->SetGuidance("D event to debug");
  dCmd->SetParameterName("fNb",false);
  dCmd->SetRange("fNb>0");
  dCmd->AvailableForStates(G4State_PreInit,G4State_Idle);     

  //UI for performance profiling

  profDirectory = new G4UIdirectory("/profile/");
  profDirectory->SetGuidance("Performance Profile.");

  profCmd = new G4UIcmdWithABool("/profile/profFlag",this);
  profCmd->SetGuidance("Enable/disable performance profile");
  profCmd->SetParameterName("profFlag",false);
  profCmd->SetDefaultValue(false);
  profCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventActionMessenger::~EventActionMessenger()
{
  delete drawCmd;
  delete printCmd;   
  delete dCmd;

  delete profDirectory;
  delete profCmd;   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventActionMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{ 
  if(command == drawCmd)
    {eventAction->SetDrawFlag(newValue);}
    
  if(command == printCmd)
    {eventAction->SetPrintModulo(printCmd->GetNewIntValue(newValue));}       

  if(command == profCmd)
    {eventAction->SetProfileFlag(profCmd->GetNewBoolValue(newValue));}
   
  if(command == dCmd)
    {eventAction->AddEventToDebug(dCmd->GetNewIntValue(newValue));}           

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
