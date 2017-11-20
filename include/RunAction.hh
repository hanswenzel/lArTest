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

class G4Timer;
#ifdef MEMCHECK
class MemoryService;
#endif
class G4Run;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class RunAction : public G4UserRunAction
{
public: // Without description

  RunAction();
  virtual ~RunAction();

public: // With description
 
  virtual void BeginOfRunAction(const G4Run*);
  // In this method histogramms are booked

  virtual void EndOfRunAction(const G4Run*);
  // In this method bookHisto method is called in which histogramms are filled
  int nEvts;

private:

  G4Timer* timer;
#ifdef MEMCHECK
  MemoryService* memory;
#endif
};

#endif

