// ----------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------
// Geant4 headers
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4Run.hh"
#include "G4Element.hh"
#include "Randomize.hh"
#include "G4UnitsTable.hh"
#include "G4Neutron.hh"
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4NistManager.hh"
#include "G4HadronicProcessStore.hh"
#include "G4NucleiProperties.hh"
#include "G4StableIsotopes.hh"
#include "G4SystemOfUnits.hh"
#include "G4Timer.hh"
// Project headers
#include "RunAction.hh"
#include "Analysis.hh"
#include "ConfigurationManager.hh"
#include "MemoryService.hh"

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

RunAction::RunAction() 
{
  timer = new G4Timer;
  memory = new MemoryService;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

RunAction::~RunAction() 
{
  delete timer;
  delete memory;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void RunAction::BeginOfRunAction(const G4Run* aRun) {
    G4int id = aRun->GetRunID();
    G4cout << "### Run " << id << " start" << G4endl;
    nEvts = aRun->GetNumberOfEventToBeProcessed();

    const G4long* table_entry;
    table_entry = CLHEP::HepRandom::getTheSeeds();
    G4long id0 = table_entry[0];
    G4long id1 = table_entry[1];

#ifdef G4VIS_USE
    G4UImanager* UI = G4UImanager::GetUIpointer();

    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

    if (pVVisManager) {
        UI->ApplyCommand("/vis/scene/notifyHandlers");
    }
#endif
    ConfigurationManager* cfMgr = ConfigurationManager::getInstance();
    if (cfMgr->GetdoAnalysis()) {
        Analysis* analysis = Analysis::getInstance();
        analysis->book(id0, id1);
    }

    //start benchmark
    timer->Start();
    memory->Start();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void RunAction::EndOfRunAction(const G4Run* aRun) {

    //Stop benchmark

    timer->Stop();
    memory->Update();

    // printout time and memory information
    G4int oldPrecision = G4cout.precision(3);
    std::ios::fmtflags oldFlags = G4cout.flags();
    G4cout.setf(std::ios::fixed,std::ios::floatfield); 
    G4cout << "TimeReport> Time report complete in ";
    if ( timer->IsValid() ) {
      G4cout << timer->GetRealElapsed();
    } else {
      G4cout << "UNDEFINED";
    }
    G4cout << " seconds" << G4endl;
    G4cout.setf(oldFlags);
    G4cout.precision(oldPrecision);

    memory->Print(aRun);

    G4cout << "RunAction: End of run actions are started" << G4endl;

#ifdef G4VIS_USE
    if (G4VVisManager::GetConcreteInstance())
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
#endif
    ConfigurationManager* cfMgr = ConfigurationManager::getInstance();
    if (cfMgr->GetdoAnalysis()) {
        Analysis* analysis = Analysis::getInstance();
        analysis->SetRunActInfo(nEvts);
        analysis->finish();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
