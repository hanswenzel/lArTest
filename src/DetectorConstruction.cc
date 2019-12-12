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
// Update: add a gdml interface (S.Y. Jun, Mar. 30, 2017)
// -----------------------------------------------------
// Geant4 headers
#include "G4PVPlacement.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4StepLimiter.hh"
#include "G4HadronicProcessStore.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"
#include "G4NistManager.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4SDManager.hh"
#include "globals.hh"
#include "G4String.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Colour.hh"
#include "G4ElectricField.hh"
#include "G4UniformElectricField.hh"
#include "G4FieldManager.hh"
#include "G4EqMagElectricField.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
// project headers
#include "ConfigurationManager.hh"
#include "DetectorConstruction.hh"
#include "TrackerSD.hh"
#include "SimEnergyDepositSD.hh"
#include "PhotonSD.hh"
#include <stdlib.h>   
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
using namespace std;

DetectorConstruction::DetectorConstruction(G4String fname) {
    gdmlFile = fname;
    sdnames = ConfigurationManager::getInstance()->getSDNames();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction() {
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    ReadGDML();
    G4VPhysicalVolume* worldPhysVol = parser->GetWorldVolume();
    return worldPhysVol;
}

void DetectorConstruction::ConstructSDandField() {
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    const G4GDMLAuxMapType* auxmap = parser->GetAuxMap();
    std::cout << "Found " << auxmap->size()
            << " volume(s) with auxiliary information."
            << std::endl << std::endl;
    for (G4GDMLAuxMapType::const_iterator iter = auxmap->begin();
            iter != auxmap->end(); iter++) {
        std::cout << "Volume " << ((*iter).first)->GetName()
                << " has the following list of auxiliary information: "
                << std::endl;
        for (G4GDMLAuxListType::const_iterator vit = (*iter).second.begin();
                vit != (*iter).second.end(); vit++) {
            std::cout << "--> Type: " << (*vit).type
                    << " Value: " << (*vit).value << std::endl;
            if ((*vit).type == "SensDet") {
                if ((*vit).value == "PhotonDetector") {
                    G4String name = ((*iter).first)->GetName() + "_Photondetector";
                    PhotonSD* aPhotonSD = new PhotonSD(name);
                    SDman->AddNewDetector(aPhotonSD);
                    sdnames->push_back(name);
                    std::cout << "new size: " << sdnames->size() << std::endl;
                    ((*iter).first)->SetSensitiveDetector(aPhotonSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    //DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
                } else if ((*vit).value == "Tracker") {
                    G4String name = ((*iter).first)->GetName() + "_Tracker";
                    TrackerSD* aTrackerSD = new TrackerSD(name);
                    SDman->AddNewDetector(aTrackerSD);
                    sdnames->push_back(name);
                    std::cout << "new size: " << sdnames->size() << std::endl;
                    ((*iter).first)->SetSensitiveDetector(aTrackerSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    //DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));                
               } else if ((*vit).value == "SimEnergyDeposit") {
                    G4String name = ((*iter).first)->GetName() + "_SimEnergyDeposit";
                    SimEnergyDepositSD* aSimEnergyDepositSD = new SimEnergyDepositSD(name);
                    SDman->AddNewDetector(aSimEnergyDepositSD);
                    sdnames->push_back(name);
                    std::cout << "new size: " << sdnames->size() << std::endl;
                    ((*iter).first)->SetSensitiveDetector(aSimEnergyDepositSD);
                    std::cout << "Attaching sensitive Detector: " << (*vit).value
                            << " to Volume:  " << ((*iter).first)->GetName() << std::endl;
                    //DetectorList.push_back(std::make_pair((*iter).first->GetName(), (*vit).value));
                } 
            } else if ((*vit).type == "StepLimit") {
                G4double value = atof((*vit).value);
                G4double val_unit = 1; //--no unit
                G4String provided_category = "NONE";
                if (((*vit).unit) && ((*vit).unit != "")) { // -- if provided and non-NULL
                    std::cout << "AuxUnit Unit parsed = " << (*vit).unit << std::endl;
                    val_unit = G4UnitDefinition::GetValueOf((*vit).unit);
                    value *= val_unit; //-- Now do something with the value, making sure that the unit is appropriate
                    provided_category = G4UnitDefinition::GetCategory((*vit).unit);
                    std::cout << "AuxUnit provided_category:  " <<provided_category<<std::endl;
                }
                G4UserLimits *fStepLimit = new G4UserLimits(atof((*vit).value));
                G4String steplimit_category = "Length";
                if (provided_category == steplimit_category) {
                    std::cout << "Valid steplimit category obtained: " << provided_category << std::endl;
                    ((*iter).first)->SetUserLimits(fStepLimit);
                    std::cout << "fStepLimit:  " << value << "  " << value / CLHEP::cm << " cm" << std::endl;
                } else if (provided_category == "NONE") { //--no unit category provided, use the default CLHEP::mm
                    std::cout << "StepLimitUnit StepLimit in geometry file does not have a unit! Defaulting to mm..." << std::endl;
                    value *= CLHEP::mm;
                    fStepLimit = new G4UserLimits(value);
                    std::cout << "fStepLimit:  " << value << "  " << value / CLHEP::cm << " cm" << std::endl;
                } else { //--wrong unit category provided
                    std::cout << "StepLimit does not have a valid length unit! Category of unit provided = " << provided_category << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else if ((*vit).type == "Solid") {
                if ((*vit).value == "True") {
                    G4VisAttributes * visibility = new G4VisAttributes();
                    visibility->SetForceSolid(true);
                    G4VisAttributes * visatt = new G4VisAttributes(((*iter).first)->GetVisAttributes()->GetColour());
                    visatt->SetVisibility(true);
                    visatt->SetForceSolid(true);
                    visatt->SetForceAuxEdgeVisible(true);
                    ((*iter).first)->SetVisAttributes(visatt);
                }
            } else if ((*vit).type == "Efield") {
                std::cout << "Setting E-Field of " << ((*iter).first)->GetName() << " to " << (*vit).value << " V/cm" << std::endl;
                double E = atof((*vit).value.c_str());
                std::cout << E << std::endl;
            }
        }




    }// loop over volumes with aux info
    G4Region* region = G4RegionStore::GetInstance()
            ->GetRegion("DefaultRegionForTheWorld", false);

    std::vector<G4Material*>::const_iterator mItr = region->GetMaterialIterator();
    size_t nMaterial = region->GetNumberOfMaterials();
    std::cout << nMaterial << G4endl;
    for (size_t iMate = 0; iMate < nMaterial; iMate++) {
        std::cout << (*mItr)->GetName() << G4endl;
        G4MaterialPropertiesTable* mt = (*mItr)->GetMaterialPropertiesTable();
        if (mt != nullptr) {
            mt->DumpTable();
            if (mt->GetProperty("SLOWCOMPONENT", true) != nullptr) {
                mt->GetProperty("SLOWCOMPONENT", true)->SetSpline(true);
                std::cout << "Scint " << mt->GetProperty("SLOWCOMPONENT", true)->GetVectorLength() << std::endl;
            }
        }
        mItr++;
    }
    std::cout << G4endl;
}//ConstructSDandField

void DetectorConstruction::ReadGDML() {
    fReader = new ColorReader;
    parser = new G4GDMLParser(fReader);
    parser->SetOverlapCheck(true);
    //    parser->Read(gdmlFile, false);
    parser->Read(gdmlFile);

    G4VPhysicalVolume *World = parser->GetWorldVolume();
    //----- GDML parser makes world invisible, this is a hack to make it
    //visible again...
    G4LogicalVolume* pWorldLogical = World->GetLogicalVolume();
    pWorldLogical->SetVisAttributes(0);
    std::cout << World->GetTranslation() << std::endl << std::endl;
    std::cout << "Found World:  " << World-> GetName() << std::endl;
    std::cout << "World LV:  " << World->GetLogicalVolume()->GetName() << std::endl;
    G4LogicalVolumeStore *pLVStore = G4LogicalVolumeStore::GetInstance();
    std::cout << "Found " << pLVStore->size()
            << " logical volumes."
            << std::endl << std::endl;
    G4PhysicalVolumeStore *pPVStore = G4PhysicalVolumeStore::GetInstance();
    std::cout << "Found " << pPVStore->size()
            << " physical volumes."
            << std::endl << std::endl;
}

void DetectorConstruction::UpdateGeometry() {
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

