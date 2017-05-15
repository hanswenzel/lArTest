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

#include "G4VUserDetectorConstruction.hh"
#include "G4GDMLParser.hh"

class G4LogicalVolume;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction {
public:

    DetectorConstruction(G4String fname);
    virtual ~DetectorConstruction();

    void PrepareLArTest();
    void ReadGDML();

   // G4VPhysicalVolume* BuildLArTest();
    //    void BuildByGDML();

    G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetWorldMaterial(const G4String&);
    void SetTargetMaterial(const G4String&);
  //  void SetTargetX(G4double val);
  //  void SetTargetY(G4double val);
  //  void SetTargetZ(G4double val);
    void SetWriteHits(G4bool val);
    void SetDoAnalysis(G4bool val);
//    void SetUseGDML(G4bool val);

    void SetMaxStepLength(G4double val);

    void UpdateGeometry();

    // calculates the scintillation emission spectrum of  LAr 
    G4double ArScintillationSpectrum(const G4double k);

    // Calculates the dielectric constant of LAr from the Bideau-Sellmeier formula.
    // See : A. Bideau-Mehu et al., "Measurement of refractive indices of Ne, Ar,
    // Kr and Xe ...", J. Quant. Spectrosc. Radiat. Transfer, Vol. 25 (1981), 395
    G4double LArEpsilon(const G4double lambda);

    // Calculates the refractive index of LAr
    G4double LArRefIndex(const G4double lam);


private:

    DetectorConstruction & operator=(const DetectorConstruction &right);
    DetectorConstruction(const DetectorConstruction&);

    G4String gdmlFile;
    G4bool writeHits;
    G4bool doAnalysis;

//    G4Material* targetMaterial;
//    G4Material* worldMaterial;

    G4LogicalVolume* logicTarget;
    G4LogicalVolume* logicWorld;

    // GDMLparser
    G4GDMLParser parser;

    //    G4GDMLReadStructure* fReader;
    DetectorMessenger* detectorMessenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

#endif

