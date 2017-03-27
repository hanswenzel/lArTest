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
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4StepLimiter.hh"
#include "G4HadronicProcessStore.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4NistManager.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
// project headers
#include "Analysis.hh"
#include "ConfigurationManager.hh"
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "TrackerSD.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
using namespace std;

DetectorConstruction::DetectorConstruction() {
    logicTarget = 0;
    logicWorld = 0;
    detectorMessenger = new DetectorMessenger(this);

    targetX = 4.0 * m;
    targetY = 4.0 * m;
    targetZ = 4.5 * m;
    //maxStep = .3 * mm;

    targetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_lAr");
    worldMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    //G4NistManager::Instance()->ListMaterials("all");

    G4MaterialPropertiesTable* LArMPT = new G4MaterialPropertiesTable();

    //LArMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);

    //->SetSpline(true);
    //
    //  simple description of scintillation yield  
    //  [J Chem Phys vol 91 (1989) 1469]
    //
    G4double Rindex[] = {1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5, 1.5};
    G4double FastScintEnergies[] = {6.7 * eV, 7.1 * eV, 7.4 * eV, 7.7 * eV, 7.9 * eV, 8.1 * eV, 8.4 * eV, 8.5 * eV, 8.6 * eV, 8.8 * eV, 9.0 * eV, 9.1 * eV, 9.4 * eV, 9.8 * eV, 10.4 * eV};
    G4double FastScintSpectrum[] = {0.04, 0.12, 0.27, 0.44, 0.62, 0.80, 0.91, 0.92, 0.85, 0.70, 0.50, 0.31, 0.13, 0.04, 0.01};
    assert(sizeof (FastScintSpectrum) == sizeof (FastScintEnergies));
    assert(sizeof (Rindex) == sizeof (FastScintEnergies));
    const G4int num = sizeof (FastScintEnergies) / sizeof (G4double);
    for (int jj = 0; jj < num; jj++) {
        G4double lam = ((h_Planck * c_light) / FastScintEnergies[jj]) / nm;
        G4cout << FastScintEnergies[jj] / eV << "  " << ((h_Planck * c_light) / FastScintEnergies[jj]) / nm << "   " << LArRefIndex(lam) << "  " << ArScintillationSpectrum(lam) << G4endl;

    }
    LArMPT->AddProperty("RINDEX", FastScintEnergies, Rindex, num);
    LArMPT->AddProperty("FASTCOMPONENT", FastScintEnergies, FastScintSpectrum, num)->SetSpline(true);
    LArMPT->AddProperty("SLOWCOMPONENT", FastScintEnergies, FastScintSpectrum, num)->SetSpline(true);
    //    LArMPT->AddProperty("FASTCOMPONENT", Scnt_spe, Scnt_pp, scinum);
    //    LArMPT->AddProperty("SLOWCOMPONENT", Scnt_spe, Scnt_pp, scinum);
    LArMPT->AddConstProperty("FASTTIMECONSTANT", 7. * ns);
    LArMPT->AddConstProperty("SLOWTIMECONSTANT", 1400. * ns);
    LArMPT->AddConstProperty("YIELDRATIO", 0.75);
    G4double scint_yield = 1.0 / (19.5 * eV);
    LArMPT->AddConstProperty("SCINTILLATIONYIELD", scint_yield / MeV);
    LArMPT->DumpTable();
    G4double fano = 0.11;
    // Doke et al, NIM 134 (1976)353
    //LArMPT->AddConstProperty("RESOLUTIONSCALE", fano);
    LArMPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
    targetMaterial->SetMaterialPropertiesTable(LArMPT);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {
    delete detectorMessenger;
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    // Cleanup old geometry

    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    // Sizes
    G4double worldX = targetX + 0.5 * m;
    G4double worldY = targetY + 0.5 * m;
    G4double worldZ = targetZ + 0.5 * m;

    //
    // World
    //

    G4Box* solidW = new G4Box("World", worldX, worldY, worldZ);
    logicWorld = new G4LogicalVolume(solidW, worldMaterial, "World");
    G4VPhysicalVolume* world = new G4PVPlacement(0, G4ThreeVector(),
            logicWorld, "World", 0, false, 0);
    //
    // Target volume
    //
    G4Box* solidT = new G4Box("Tracker", targetX, targetY, targetZ);
    logicTarget = new G4LogicalVolume(solidT, targetMaterial, "Tracker");
    new G4PVPlacement(0, G4ThreeVector(), logicTarget, "Tracker", logicWorld, false, 0);
    G4String SDname = "TrackerSD";
    TrackerSD* aTrackerSD = new TrackerSD(SDname, "HitsCollection");
    // Setting aTrackerSD to all logical volumes with the same name 
    // of "Target".
    SetSensitiveDetector("Tracker", aTrackerSD, true);
    // colors
    G4VisAttributes zero = G4VisAttributes::Invisible;
    logicWorld->SetVisAttributes(&zero);

    G4VisAttributes regCcolor(G4Colour(0., 0.3, 0.7));
    logicTarget->SetVisAttributes(&regCcolor);
    // Analysis:
    if (ConfigurationManager::getInstance()->GetdoAnalysis()) {
        //for ntuple:
        Analysis* analysis = Analysis::getInstance();
        std::vector<G4double> vDCinfo;
        vDCinfo.push_back(2. * targetX);
        vDCinfo.push_back(2. * targetY);
        vDCinfo.push_back(2. * targetZ);
        G4double dens = targetMaterial->GetDensity() / (g / cm3);
        G4String nameM = targetMaterial->GetName();
        vDCinfo.push_back(dens);

        analysis->SetDetConstInfo(vDCinfo, targetMaterial);
    }
    //step limits
    if (ConfigurationManager::getInstance()->GetstepLimit()) {
        G4double mxStep = ConfigurationManager::getInstance()->Getlimitval();
        G4cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&mxStep" << mxStep / mm << " mm" << G4endl;
        G4UserLimits *fStepLimit = new G4UserLimits(mxStep);
        logicTarget->SetUserLimits(fStepLimit);
    }

    return world;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetMaterial(const G4String& mat) {
    // search the material by its name
    G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial(mat);

    if (material && material != targetMaterial) {
        targetMaterial = material;
        if (logicTarget) logicTarget->SetMaterial(targetMaterial);
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(const G4String& mat) {
    // search the material by its name
    G4Material* material = G4NistManager::Instance()->FindOrBuildMaterial(mat);

    if (material && material != worldMaterial) {
        worldMaterial = material;
     
        if (logicWorld) logicWorld->SetMaterial(worldMaterial);
        G4RunManager::GetRunManager()->PhysicsHasBeenModified();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::UpdateGeometry() {
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void DetectorConstruction::SetMaxStepLength(G4double val) {
//    if (val > 0.0) {
//        this->maxStep = val;
//        G4RunManager::GetRunManager()->GeometryHasBeenModified();
//    }
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetX(G4double val) {
    if (val > 0.0) {
        targetX = val;
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetY(G4double val) {
    if (val > 0.0) {
        targetY = val;
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetZ(G4double val) {
    if (val > 0.0) {
        targetZ = val;
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}
// The following 3 functions are described in
// https://indico.cern.ch/event/44566/contributions/1101918/attachments/943057/1337650/dipompeo.pdf
// and references therein
//

G4double DetectorConstruction::ArScintillationSpectrum(G4double kk) {
    G4double waveL;
    waveL = exp(-0.5 * ((kk - 128.0) / (2.929))*((kk - 128.0) / (2.929)));
    return waveL;
}

// Calculates the dielectric constant of LAr from the Bideau-Sellmeier formula.
// See : A. Bideau-Mehu et al., "Measurement of refractive indices of Ne, Ar,
// Kr and Xe ...", J. Quant. Spectrosc. Radiat. Transfer, Vol. 25 (1981), 395

G4double DetectorConstruction::LArEpsilon(const G4double lambda) {
    G4double epsilon;
    G4double lowLambda = 110.0; // lowLambda in  nanometer
    if (lambda < lowLambda) return 1.0e4; // lambda MUST be > 110.0 nm
    epsilon = lambda / 1000; // switch to micrometers
    epsilon = 1.0 / (epsilon * epsilon); // 1 / (lambda)^2
    epsilon = 1.2055e-2 * (0.2075 / (91.012 - epsilon) +
            0.0415 / (87.892 - epsilon) +
            4.3330 / (214.02 - epsilon));
    epsilon *= (8. / 12.); // Bideau-Sellmeier -> Clausius-Mossotti
    G4double GArRho = 0.001784; // density in g/cm3
    G4double LArRho = 1.3954; // density in g/cm3
    epsilon *= (LArRho / GArRho); // density correction (Ar gas -> LAr liquid)
    if (epsilon < 0.0 || epsilon > 0.999999) return 4.0e6;
    epsilon = (1.0 + 2.0 * epsilon) / (1.0 - epsilon); // solve Clausius-Mossotti
    return epsilon;
}
//
// Calculates the refractive index of LAr
//

G4double DetectorConstruction::LArRefIndex(const G4double lambda) {
    return ( sqrt(LArEpsilon(lambda))); // square root of dielectric constant
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
