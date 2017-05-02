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
#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
//#include "G4Run.hh"
//#include "globals.hh"

class G4Timer;
class MemoryService;
class G4Run;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class RunAction : public G4UserRunAction
{
public: // Without description

  RunAction();
  ~RunAction();

public: // With description
 
  void BeginOfRunAction(const G4Run*);
  // In this method histogramms are booked

  void EndOfRunAction(const G4Run*);
  // In this method bookHisto method is called in which histogramms are filled
  int nEvts;

private:

  G4Timer* timer;
  MemoryService* memory;

};

#endif

