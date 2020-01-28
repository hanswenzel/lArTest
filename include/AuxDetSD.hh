//=============================================================================
// AuxDetSD.h: Class representing a sensitive tracking detector
// Author: Hans Wenzel (Fermilab)
//=============================================================================
#ifndef AuxDetSD_h
#define AuxDetSD_h 1
#include "TempHit.hh"
#include "G4VSensitiveDetector.hh"
#if defined __clang__
#pragma clang diagnostic push
#pragma( clang diagnostic ignored "-Wunused-private-field"
#endif

class G4Step;
class G4HCofThisEvent;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class AuxDetSD : public G4VSensitiveDetector {
public:

    AuxDetSD(G4String name);
    virtual ~AuxDetSD();
    void Initialize(G4HCofThisEvent*);
    void EndOfEvent(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);

private:
    TempHitCollection temphitCollection;

};


#if defined __clang__
#pragma clang diagnostic pop
#endif

#endif

