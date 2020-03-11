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
#include "G4PhysListFactoryAlt.hh" 
#include "G4PhysicsConstructorRegistry.hh"
#include "G4PhysListRegistry.hh"
#include "G4OpticalPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4VModularPhysicsList.hh"
#include "G4StepLimiter.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4Timer.hh"
#include "G4EmCalculator.hh"
#include "G4ParticleDefinition.hh"
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
    bool interactive = false;
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

#ifdef G4UI_USE
    G4UIExecutive* ui = nullptr;
#endif
    if (argc == 2) {
        interactive = true;
#ifdef G4UI_USE
        ui = new G4UIExecutive(argc, argv);
#endif
    }
    //choose the Random engine
    G4Random::HepRandom::setTheEngine(new CLHEP::RanecuEngine());

    //start time
    G4Timer *eventTimer = new G4Timer;
    eventTimer->Start();

    // Construct the default run manager
#ifdef G4MULTITHREADED
    //the default number of threads
    G4int nThreads = 1;
    //set it from the -t option is provided
    for (G4int i = 1; i < argc; i = i + 2) {
        if (G4String(argv[i]) == "-t") {
            nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
        }
    }
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(nThreads);
    //
    // ConfigurationManager is lazily initialized
    // Therefore in multi-threading mode we initialize before multiple threads are started
    //
    ConfigurationManager * CMgr = ConfigurationManager::getInstance();
#else
    G4RunManager* runManager = new G4RunManager;
#endif
    // Access to registries and factories
    //
    G4PhysicsConstructorRegistry* g4pcr = G4PhysicsConstructorRegistry::Instance();
    G4PhysListRegistry* g4plr = G4PhysListRegistry::Instance();
    // print state of the factory after loading 2nd library
    /*
    G4cout<< "Available Physics Constructors:  "<< g4pcr->AvailablePhysicsConstructors().size()<<G4endl;
    G4cout<< "Available Physics Lists:         "<< g4plr->AvailablePhysLists().size()<<G4endl;
    G4cout<< "Available Physics Extensions:    "<< g4plr->AvailablePhysicsExtensions().size()<<G4endl;
    G4cout<< "Available Physics Lists Em:      "<< g4plr->AvailablePhysListsEM().size()<<G4endl;
     */

    g4plr->AddPhysicsExtension("OPTICAL", "G4OpticalPhysics");
    g4plr->AddPhysicsExtension("STEPLIMIT", "G4StepLimiterPhysics");
    g4plr->AddPhysicsExtension("NEUTRONLIMIT", "G4NeutronTrackingCut");

    g4pcr->PrintAvailablePhysicsConstructors();
    g4plr->PrintAvailablePhysLists();
    g4alt::G4PhysListFactory factory;
    G4VModularPhysicsList* phys = nullptr;
    G4String physName = "FTFP_BERT+OPTICAL+STEPLIMIT+NEUTRONLIMIT";
    //
    // currently using the Constructor names doesn't work otherwise it would be:
    // G4String physName = "FTFP_BERT+G4OpticalPhysics+G4StepLimiterPhysics";
    // using the name doesn't work either
    //G4String physName = "FTFP_BERT+Optical+stepLimiter";
    // reference PhysicsList via its name
    if (factory.IsReferencePhysList(physName)) {
        phys = factory.GetReferencePhysList(physName);
    }
    /*
        // now add optical physics constructor:
        G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
        phys->RegisterPhysics(opticalPhysics);
        // Cerenkov off by default
     */

    G4cout << phys->GetPhysicsTableDirectory() << G4endl;
    G4OpticalPhysics* opticalPhysics = (G4OpticalPhysics*) phys->GetPhysics("Optical");
    //G4OpticalPhysics* opticalPhysics = (G4OpticalPhysics*) phys->GetPhysics("G4OpticalPhysics");
    opticalPhysics->Configure(kCerenkov, false);
    opticalPhysics->SetCerenkovStackPhotons(true);
    opticalPhysics->Configure(kWLS, false);
    opticalPhysics->Configure(kScintillation, false);
    opticalPhysics->SetScintillationYieldFactor(1.0);
    opticalPhysics->SetScintillationExcitationRatio(0.0);
    opticalPhysics->Configure(kRayleigh, true);
    opticalPhysics->Configure(kBoundary, true);
    opticalPhysics->Configure(kAbsorption, true);
    opticalPhysics->SetScintillationStackPhotons(false);
    opticalPhysics->SetTrackSecondariesFirst(kCerenkov, true); // only relevant if we actually stack and trace the optical photons
    opticalPhysics->SetTrackSecondariesFirst(kScintillation, true); // only relevant if we actually stack and trace the optical photons
    opticalPhysics->SetMaxNumPhotonsPerStep(100);
    opticalPhysics->SetMaxBetaChangePerStep(10.0);


    /*
    G4NeutronTrackingCut * neutrcut = (G4NeutronTrackingCut*) phys->GetPhysics("neutronTrackingCut");
    //    G4NeutronTrackingCut * neutrcut = (G4NeutronTrackingCut*) phys->GetPhysics("G4NeutronTrackingCut");
    neutrcut->SetTimeLimit(10000);
    //G4cout << "step limiter enabled limit: " << ConfigurationManager::getInstance()->Getlimitval() * cm << " cm" << G4endl;
    //
     */
    phys->DumpList();

    //    G4cout << "Analysis set to:  " << ConfigurationManager::getInstance()->GetdoAnalysis() << G4endl;
    //set mandatory initialization classes
    DetectorConstruction* detConstruction = new DetectorConstruction(argv[1]);
    runManager->SetUserInitialization(detConstruction);
    runManager->SetUserInitialization(phys);
    ActionInitialization* actionInitialization = new ActionInitialization();
    runManager->SetUserInitialization(actionInitialization);
    G4cout << "Analysis set to:  " << ConfigurationManager::getInstance()->GetdoAnalysis() << G4endl;

    //
    // Initialize visualization
    //
#ifdef G4UI_USE
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
#endif
    if (interactive) {
#ifdef G4VIS_USE
        G4VisManager* visManager = new G4VisExecutive;
        visManager->Initialize();
#endif
        //get the pointer to the User Interface manager
#ifdef G4UI_USE
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
#endif
#ifdef G4VIS_USE
        delete visManager;
#endif
    } else {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[2];
#ifdef G4UI_USE
        UImanager->ApplyCommand(command + fileName);
#endif
    }
    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted 
    // in the main() program !
    G4EmCalculator emCalculator;
    static const G4ParticleDefinition* pd = G4ParticleTable::GetParticleTable()->FindParticle("proton");
    G4cout << "##### DEDX Table for " << pd->GetParticleName() << G4endl;
    emCalculator.PrintDEDXTable(pd);
    delete runManager;

    //stop time - the total elapsed time including initialization
    eventTimer->Stop();
    double totalCPUTime = eventTimer->GetUserElapsed()
            + eventTimer->GetSystemElapsed();

    G4int precision_t = G4cout.precision(3);
    std::ios::fmtflags flags_t = G4cout.flags();
    G4cout.setf(std::ios::fixed, std::ios::floatfield);
    G4cout << "TimeTotal> " << eventTimer->GetRealElapsed() << " "
            << totalCPUTime << G4endl;
    G4cout.setf(flags_t);
    G4cout.precision(precision_t);
    delete eventTimer;

    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
