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
#ifndef EventAction_h
#define EventAction_h 1
 
#include "globals.hh"
#include "G4UserEventAction.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Event;
class G4UImanager;
class G4Timer;
#ifdef MEMCHECK
class MemoryService ;
#endif
class SteppingAction;

class EventAction : public G4UserEventAction
{
public: // Without description

  EventAction();
  virtual ~EventAction();

  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

  void AddEventToDebug(G4int val)   {selectedEvents.push_back(val);
                                     nSelected++;};

  void InstanciateSteppingAction();      

  //IgProf Service
  //void (*dump_)(const char *);
  
private:

  G4UImanager*          UI;
  std::vector<G4int>    selectedEvents;
  G4int        nSelected;

  //performance profiling service
  G4double        totalEventCPUTime;
  G4Timer*        eventTimer;
#ifdef MEMCHECK
  MemoryService*  eventMemory;
#endif
  SteppingAction* theSteppingAction;
};

#endif


