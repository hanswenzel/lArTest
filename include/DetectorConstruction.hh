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
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1
#include "ColorReader.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4GDMLParser.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction {
public:

    DetectorConstruction(G4String fname);
    virtual ~DetectorConstruction();

    //void PrepareLArTest();
    void ReadGDML();

    G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetWorldMaterial(const G4String&);
    void SetTargetMaterial(const G4String&);
    void SetMaxStepLength(G4double val);
    void UpdateGeometry();

private:

    DetectorConstruction & operator=(const DetectorConstruction &right);
    DetectorConstruction(const DetectorConstruction&);

    G4String gdmlFile;
    std::vector<G4String> *sdnames;
    G4GDMLParser *parser;
    ColorReader* fReader;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

#endif

