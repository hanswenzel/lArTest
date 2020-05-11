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
#include "G4RunManager.hh"
#include "G4SDManager.hh"
// Project headers
#include "RunAction.hh"
#include "ConfigurationManager.hh"
#ifdef MEMCHECK
#include "MemoryService.hh"
#endif
#include "RootIO.hh"
#include "Analysis.hh"
#define UNUSED(expr) do { (void)(expr); } while (0)
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

RunAction::RunAction() : G4UserRunAction() {
    timer = new G4Timer;
#ifdef MEMCHECK
    memory = new MemoryService;
#endif
    // set printing event number per each event
    G4RunManager::GetRunManager()->SetPrintProgress(1);
    // Create analysis manager
    // The choice of analysis technology is done via selecting of a namespace
    // in Analysis.hh

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

RunAction::~RunAction() {
    delete G4AnalysisManager::Instance();
    delete timer;
#ifdef MEMCHECK
    delete memory;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void RunAction::BeginOfRunAction(const G4Run* aRun) {
    G4int id = aRun->GetRunID();
    G4cout << "### Run " << id << " start" << G4endl;
    ConfigurationManager* cfMgr = ConfigurationManager::getInstance();
    if (cfMgr->GetdoAnalysis()) {
        auto analysisManager = G4AnalysisManager::Instance();
        //G4SDManager* SDman = G4SDManager::GetSDMpointer();
        //std::cout << "999999999999999999999999999999999999999999999999999" << std::endl;
        //std::cout << "Using " << analysisManager->GetType() << std::endl;

        // Create directories 
        analysisManager->SetHistoDirectoryName("histograms");
        analysisManager->SetNtupleDirectoryName("ntuple");
        analysisManager->SetVerboseLevel(1);
        analysisManager->SetNtupleMerging(true);
        // Note: merging ntuples is available only with Root output
        // Book histograms, ntuple
        //
        std::vector<G4String> *sdnames = ConfigurationManager::getInstance()->getSDNames();
        std::map<G4String, int> *mapOfntIDs = ConfigurationManager::getInstance()->getMapOfntIDs();
        std::cout << "size of  " << sdnames->size() << std::endl;
        for (unsigned int i = 0; i < sdnames->size(); i++) {
            //G4int TrackerNTID = analysisManager->CreateNtuple(sdnames->at(i).c_str(), "Tracker Hits");
            //mapOfntIDs->insert(std::make_pair(sdnames->at(i), TrackerNTID));
            if (sdnames->at(i).contains("_Tracker")) {
                G4int TrackerNTID = analysisManager->CreateNtuple(sdnames->at(i).c_str(), "Tracker Hits");
                mapOfntIDs->insert(std::make_pair(sdnames->at(i), TrackerNTID));
                analysisManager->CreateNtupleDColumn(TrackerNTID, "Edep");
                analysisManager->CreateNtupleDColumn(TrackerNTID, "x");
                analysisManager->CreateNtupleDColumn(TrackerNTID, "y");
                analysisManager->CreateNtupleDColumn(TrackerNTID, "z");
                analysisManager->CreateNtupleDColumn(TrackerNTID, "t");
                analysisManager->CreateNtupleDColumn(TrackerNTID, "steplength");
                analysisManager->CreateNtupleIColumn(TrackerNTID, "NPhotons");
                analysisManager->CreateNtupleIColumn(TrackerNTID, "ID");
                analysisManager->CreateNtupleIColumn(TrackerNTID, "Evt");
                analysisManager->CreateNtupleIColumn(TrackerNTID, "pId");
                analysisManager->CreateNtupleDColumn(TrackerNTID, "kE");
                analysisManager->FinishNtuple(TrackerNTID);
            } else if (sdnames->at(i).contains("_Photondetector")) {
                G4int PhotonNTID = analysisManager->CreateNtuple(sdnames->at(i).c_str(), "Photon Hits");
                mapOfntIDs->insert(std::make_pair(sdnames->at(i), PhotonNTID));
                analysisManager->CreateNtupleDColumn(PhotonNTID, "E");
                analysisManager->CreateNtupleDColumn(PhotonNTID, "x");
                analysisManager->CreateNtupleDColumn(PhotonNTID, "y");
                analysisManager->CreateNtupleDColumn(PhotonNTID, "z");
                analysisManager->CreateNtupleDColumn(PhotonNTID, "t");
                analysisManager->CreateNtupleDColumn(PhotonNTID, "px");
                analysisManager->CreateNtupleDColumn(PhotonNTID, "py");
                analysisManager->CreateNtupleDColumn(PhotonNTID, "pz");
                analysisManager->CreateNtupleIColumn(PhotonNTID, "Evt");
                analysisManager->FinishNtuple(PhotonNTID);
            }
        }
    }


    // std::vector<G4String> *sdnames = ConfigurationManager::getInstance()->getSDNames();
    // std::cout << "size of  " << sdnames->size() << std::endl;
    // for (unsigned int i = 0; i < sdnames->size(); i++) {
    //     std::cout << sdnames->at(i) << std::endl;

    nEvts = aRun->GetNumberOfEventToBeProcessed();
    G4UImanager* UI = G4UImanager::GetUIpointer();
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager) {
        UI->ApplyCommand("/vis/scene/notifyHandlers");
    }
    // ConfigurationManager* cfMgr = ConfigurationManager::getInstance();

    if (cfMgr->GetdoProfile()) {
        //start benchmark
        timer->Start();
#ifdef MEMCHECK
        memory->Start();
#endif
    }
    if (cfMgr->GetdoAnalysis()) {
        // Get analysis manager
        auto analysisManager = G4AnalysisManager::Instance();
        // Open an output file
        //
        G4String fileName = "lArTest";
        analysisManager->OpenFile(fileName);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void RunAction::EndOfRunAction(const G4Run* aRun) {
   UNUSED(aRun); 
    if (G4VVisManager::GetConcreteInstance())
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
   RootIO::GetInstance()->Close();
    ConfigurationManager* cfMgr = ConfigurationManager::getInstance();
    if (cfMgr->GetdoAnalysis()) {
        // print histogram statistics
        //
        auto analysisManager = G4AnalysisManager::Instance();
        // save histograms & ntuple
        //
        analysisManager->Write();
        analysisManager->CloseFile();
    }

    if (cfMgr->GetdoProfile()) {
        //Stop benchmark
        timer->Stop();
#ifdef MEMCHECK
        memory->Update();
#endif
        // printout time and memory information
        G4int oldPrecision = G4cout.precision(3);
        std::ios::fmtflags oldFlags = G4cout.flags();
        G4cout.setf(std::ios::fixed, std::ios::floatfield);
        G4cout << "TimeReport> Time report complete in ";
        if (timer->IsValid()) {
            G4cout << timer->GetRealElapsed();
        } else {
            G4cout << "UNDEFINED";
        }
        G4cout << " seconds" << G4endl;
        G4cout.setf(oldFlags);
        G4cout.precision(oldPrecision);
#ifdef MEMCHECK
        memory->Print(aRun);
#endif
        G4cout << "RunAction: End of run actions are started" << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
