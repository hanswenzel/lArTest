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
 
#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Event;
class G4Track;
class EventActionMessenger;
class G4UImanager;

class EventAction : public G4UserEventAction
{
public: // Without description

  EventAction();
  virtual ~EventAction();

  void BeginOfEventAction(const G4Event*);
  void   EndOfEventAction(const G4Event*);

  void SetPrintModulo(G4int val)   {printModulo = val;};
  void SetDrawFlag(G4String val)   {drawFlag = val;};
  void AddEventToDebug(G4int val)  {selectedEvents.push_back(val);
                                    nSelected++;};


private:

  EventActionMessenger* eventMessenger;
  G4UImanager*          UI;
  std::vector<G4int>    selectedEvents;

  G4int        printModulo;
  G4int        nSelected;

  // drawFlags = all, charged, neutral, charged+n
  G4String     drawFlag;
  G4bool       debugStarted;
  //std::vector<TrackInfo_t> TrackInfoVec;
  
};

#endif


