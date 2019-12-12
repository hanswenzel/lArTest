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
#ifndef SimEnergyDepositSD_h
#define SimEnergyDepositSD_h 1

#include "G4VSensitiveDetector.hh"
#include "SimStep.hh"
#include "SimEnergyDeposit.hh"
#include <vector> 
class G4Step;
class G4HCofThisEvent;
//class SimTrajectory;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// B2Tracker sensitive detector class

class SimEnergyDepositSD : public G4VSensitiveDetector {
public:
    SimEnergyDepositSD(G4String name);
    virtual ~SimEnergyDepositSD();

    // methods from base class
    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

private:
    std::vector<SimEnergyDeposit*>* sedvec;
    //std::vector<SimStep*> stVector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
