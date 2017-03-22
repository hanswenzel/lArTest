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
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#include "G4PhysListFactory.hh"
#include "G4OpticalPhysics.hh"
#include "G4VModularPhysicsList.hh"
#include "G4StepLimiter.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
// project headers
#include "ConfigurationManager.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv) {
    //
    // Detect interactive mode (if no arguments) and define UI session
    //
    G4UIExecutive* ui = 0;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    //choose the Random engine
    G4Random::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
    // Construct the default run manager
    //
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
#else
    G4RunManager* runManager = new G4RunManager;
#endif


    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = NULL;
    G4String physName = "";
    //-----------------------------------------------------
    // Physics List name defined via environmental variable
    // The following physics lists are available PHYSLIST:
    //"FTFP_BERT"
    //"FTFP_BERT_TRV"
    //"FTFP_BERT_ATL"
    //"FTFP_BERT_HP"
    //"FTFP_INCLXX"
    //"FTFP_INCLXX_HP"
    //"FTF_BIC"
    //"LBE"
    //"QBBC"
    //"QGSP_BERT"
    //"QGSP_BERT_HP"
    //"QGSP_BIC"
    //"QGSP_BIC_HP"
    //"QGSP_BIC_AllHP"
    //"QGSP_FTFP_BERT"
    //"QGSP_INCLXX"
    //"QGSP_INCLXX_HP"
    //"QGS_BIC"
    //"Shielding"
    //"ShieldingLEND"
    //"ShieldingM"
    //"NuBeam"
    //
    // The following electromagnetic options are available:
    //"",
    //"_EMV"
    //"_EMX"
    //"_EMY"
    //"_EMZ"
    //"_LIV"
    //"_PEN"
    //"__GS"
    //-----------------------------------------------------
    char* path = getenv("PHYSLIST");
    if (path) {
        physName = G4String(path);
    } else {
        physName = "FTFP_BERT"; // default
    }
    // reference PhysicsList via its name
    if (factory.IsReferencePhysList(physName)) {
        phys = factory.GetReferencePhysList(physName);
    }
    // now add optical physics constructor:
    G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
    phys->RegisterPhysics(opticalPhysics);
    // Cerenkov off by default
    opticalPhysics->Configure(kCerenkov, false);
    opticalPhysics->SetCerenkovStackPhotons(false);
    // Scintillation on by default, optical photons are not put on the stack 
    opticalPhysics->Configure(kScintillation, true);
    opticalPhysics->SetScintillationYieldFactor(1.0);
    opticalPhysics->SetScintillationExcitationRatio(0.0);
    opticalPhysics->SetScintillationStackPhotons(false);
    opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true); // only relevant if we actually stack and trace the optical photons
    opticalPhysics->SetTrackSecondariesFirst(kScintillation, true); // only relevant if we actually stack and trace the optical photons
    opticalPhysics->SetMaxNumPhotonsPerStep(100);
    opticalPhysics->SetMaxBetaChangePerStep(10.0);
    //    
    phys->DumpList();
    //set mandatory initialization classes
    runManager->SetUserInitialization(new DetectorConstruction());
    //StepLimiter:
    if (ConfigurationManager::getInstance()->GetstepLimit()) {
        phys->RegisterPhysics(new G4StepLimiterPhysics());
    }
    runManager->SetUserInitialization(phys);
    //set user action classes
    runManager->SetUserAction(new PrimaryGeneratorAction());
    runManager->SetUserAction(new RunAction());
    runManager->SetUserAction(new EventAction());
    //
    // Initialize visualization
    //
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    //get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();

    // Process macro or start UI session
    //
    if (!ui) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    } else {
        // interactive mode
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted 
    // in the main() program !
    delete visManager;
    delete runManager;
    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......