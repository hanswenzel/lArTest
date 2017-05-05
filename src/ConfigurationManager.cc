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
#include "G4SystemOfUnits.hh"
// project headers
#include "ConfigurationManager.hh"
#include "ConfigurationManagerMessenger.hh"

using namespace std;

ConfigurationManager* ConfigurationManager::instance = 0;


ConfigurationManager::ConfigurationManager() {
    confMessenger = new ConfigurationManagerMessenger(this);
    doAnalysis = true;      // by default we do analysis
    stepLimit = true;       // by default we enable the step limiter
    limitval = 0.1 * mm;
}

ConfigurationManager* ConfigurationManager::getInstance() {
    if (instance == 0) instance = new ConfigurationManager;
    return instance;
}
