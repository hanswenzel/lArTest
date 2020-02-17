// -----------------------------------------------------------------------------
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
// Update: add performance profiling functionality (S.Y. Jun, Mar. 30, 2017)
// -----------------------------------------------------------------------------
// Geant4 headers
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4Event.hh"
// project headers
#include "EventAction.hh"
#include "ConfigurationManager.hh"
// performance headers
#include "G4Timer.hh"
#ifdef MEMCHECK
#include "MemoryService.hh"
#endif
#include "SteppingAction.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include <dlfcn.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
using namespace std;
EventAction::EventAction():
  nSelected(0)
{
  UI = G4UImanager::GetUIpointer();
  selectedEvents.clear();

  //ubstantiate stepping action
  InstanciateSteppingAction();

  totalEventCPUTime = 0;
  eventTimer = new G4Timer;
#ifdef MEMCHECK
  eventMemory = new MemoryService();
#endif
/*
  //instantiate igprof service
  if (void *sym = dlsym(0, "igprof_dump_now")) {
    dump_ = __extension__ (void(*)(const char *)) sym;
  } else {
    dump_=0;
    G4cout << "Heap profile requested but application is not "
  		<< "currently being profiled with igprof" << G4endl;
  }
 */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

EventAction::~EventAction()
{
  delete eventTimer;
#ifdef MEMCHECK
  delete eventMemory;
#endif

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::BeginOfEventAction(const G4Event* evt)
{
  // New event
  G4int nEvt = evt->GetEventID();
  if(nEvt%100000==0)G4cout<<"EventID " <<nEvt<<G4endl;
  if(nSelected>0) {
    for(G4int i=0; i<nSelected; i++) {
      if(nEvt == selectedEvents[i]) {
        UI->ApplyCommand("/random/saveThisEvent");
        UI->ApplyCommand("/tracking/verbose  2");
        ConfigurationManager::getInstance()->SetdebugEvent(true);
        break;
      }
    }
  }

  if(ConfigurationManager::getInstance()->GetdoProfile()) {
    eventTimer->Start();
#ifdef MEMCHECK
    eventMemory->Start();
#endif

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::EndOfEventAction(const G4Event* evt)
{
  
  //  if(debugStarted) {
  if(ConfigurationManager::getInstance()->GetdebugEvent()) {
    G4cout<<"End of debug: eventID: "<<(evt->GetEventID())<<G4endl;
    UI->ApplyCommand("/tracking/verbose  0");
    ConfigurationManager::getInstance()->SetdebugEvent(false);
  }

  //performance profile service
  if(ConfigurationManager::getInstance()->GetdoProfile()) {

    eventTimer->Stop();
#ifdef MEMCHECK
    eventMemory->Update();
#endif


    // igprof service: snapshot live memory on the heap for every 25 events.
/*
    if ( dump_ && evt->GetEventID() % 25 == 0 ) { 
      char outfile[256];
      sprintf(outfile,"|gzip -9c > IgProf.%d.gz",evt->GetEventID()+1);
      dump_(outfile);
    }
*/
    // emulating the framework printout, here note the +1s

    double eventCpuTime = eventTimer->GetUserElapsed() 
      + eventTimer->GetSystemElapsed();
    totalEventCPUTime += eventCpuTime;

    G4int precision_t = G4cout.precision(3);
    std::ios::fmtflags flags_t = G4cout.flags();
    G4cout.setf(std::ios::fixed,std::ios::floatfield); 
    G4cout << "TimeEvent> "<< evt->GetEventID()+1 << " "
	   << G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID()+1 
           << " " << eventTimer->GetRealElapsed() << " " << eventCpuTime << " "
	   << totalEventCPUTime << G4endl;
    G4cout.setf(flags_t);
    G4cout.precision(precision_t);

    // print memory usage in megabyte: [vsize] [rss] [share]
#ifdef MEMCHECK
    eventMemory->Print(evt);
#endif

    //print the number of steps and tracks: gamma e+ e- pi- pi+ p others
    theSteppingAction->Print(evt);
    theSteppingAction->Reset();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void EventAction::InstanciateSteppingAction() {      
  G4UserSteppingAction* theUserAction = const_cast< G4UserSteppingAction* >
    ( G4RunManager::GetRunManager()->GetUserSteppingAction() );
  if (theUserAction == 0) {
    theSteppingAction = new SteppingAction;  
    G4RunManager::GetRunManager()->SetUserAction( theSteppingAction );
  }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
