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
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"
#ifdef USE_OLD
    #include "G4PhysListFactory.hh" 
#else
   #include "G4PhysListFactoryAlt.hh" 
#endif
#include "G4PhysicsConstructorRegistry.hh"
#include "G4PhysListRegistry.hh"
#include "G4OpticalPhysics.hh"
#include "G4VModularPhysicsList.hh"
#include "G4StepLimiter.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4Timer.hh"
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
// project headers
#include "ActionInitialization.hh"
#include "ConfigurationManager.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv) {
    if (argc < 2) {
        G4cout << "Error! Mandatory input file is not specified!" << G4endl;
        G4cout << G4endl;
        G4cout << G4endl;
        G4cout << "Usage: lArTest <intput_gdml_file:mandatory>" << G4endl;
        G4cout << G4endl;
        return -1;
    }
    //
    // Detect interactive mode (if only one argument) and define UI session
    //
    G4UIExecutive* ui = 0;
    if (argc == 2) {
        ui = new G4UIExecutive(argc, argv);
    }
    //choose the Random engine
    G4Random::HepRandom::setTheEngine(new CLHEP::RanecuEngine());

    //start time
    G4Timer *eventTimer = new G4Timer;
    eventTimer->Start();

    // Construct the default run manager
#ifdef G4MULTITHREADED
    //the default number of threads
    G4int nThreads = 4;
    //set it from the -t option is provided
    for ( G4int i = 1 ; i < argc ; i = i+2 ) {
      if ( G4String(argv[i]) == "-t" ) {
        nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
      }
    }
#endif

#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(nThreads);
    //
    // ConfigurationManager is lazily initialized
    // Therefore in multi-threading mode we initialize before multiple threads are started
    //
    ConfigurationManager * CMgr  = ConfigurationManager::getInstance();
#else
    G4RunManager* runManager = new G4RunManager;
#endif
  // Access to registries and factories
  //
  G4PhysicsConstructorRegistry* g4pcr = G4PhysicsConstructorRegistry::Instance();
  G4PhysListRegistry* g4plr = G4PhysListRegistry::Instance();
  //if ( gPrintCtorList  > 0 ) 
  g4pcr->PrintAvailablePhysicsConstructors();
  //if ( gPrintPLRegList > 0 ) 
  g4plr->PrintAvailablePhysLists();
#ifdef USE_OLD
        G4PhysListFactory factory;
#else
	g4alt::G4PhysListFactory factory;
#endif

      //    G4PhysListFactory factory;

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
    opticalPhysics->SetScintillationStackPhotons(true);
    opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true); // only relevant if we actually stack and trace the optical photons
    opticalPhysics->SetTrackSecondariesFirst(kScintillation, true); // only relevant if we actually stack and trace the optical photons
    opticalPhysics->SetMaxNumPhotonsPerStep(100);
    opticalPhysics->SetMaxBetaChangePerStep(10.0);
    //StepLimiter:
    G4cout << ConfigurationManager::getInstance()->GetdoAnalysis() << G4endl;
    if (ConfigurationManager::getInstance()->GetstepLimit()) {
        G4cout << "step limiter enabled limit: " << ConfigurationManager::getInstance()->Getlimitval() * cm << " cm" << G4endl;
        phys->RegisterPhysics(new G4StepLimiterPhysics());
    }
    //    
    phys->DumpList();
    //set mandatory initialization classes
    DetectorConstruction* detConstruction = new DetectorConstruction(argv[1]);
    runManager->SetUserInitialization(detConstruction);

    runManager->SetUserInitialization(phys);
    //set user action classes
    //ActionInitialization* actionInitialization = 
    //  new ActionInitialization(detConstruction);
    ActionInitialization* actionInitialization = new ActionInitialization();
    runManager->SetUserInitialization(actionInitialization);

#ifdef G4VIS_USE
    //
    // Initialize visualization
    //
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    //get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
#endif
    // Process macro or start UI session
    //
    if (!ui) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[2];
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
#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;

    //stop time - the total elapsed time including initialization
    eventTimer->Stop();
    double totalCPUTime = eventTimer->GetUserElapsed() 
                        + eventTimer->GetSystemElapsed();
    
    G4int precision_t = G4cout.precision(3);
    std::ios::fmtflags flags_t = G4cout.flags();
    G4cout.setf(std::ios::fixed,std::ios::floatfield); 
    G4cout << "TimeTotal> " << eventTimer->GetRealElapsed() << " " 
           << totalCPUTime << G4endl;
    G4cout.setf(flags_t);
    G4cout.precision(precision_t);
    delete eventTimer;

    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
