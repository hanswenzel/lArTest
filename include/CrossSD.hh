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
#ifndef CrossSD_h
#define CrossSD_h 1

#include "G4VSensitiveDetector.hh"
#include "SimStep.hh"
#include "SimTrajectory.hh"
#include <map> 
class G4Step;
class G4HCofThisEvent;
//class SimTrajectory;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// B2Cross sensitive detector class

class CrossSD : public G4VSensitiveDetector {
public:
    CrossSD(G4String name);
    virtual ~CrossSD();

    // methods from base class
    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

private:
    std::map<int, SimTrajectory*>* tmap;
    //std::vector<SimStep*> stVector;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
