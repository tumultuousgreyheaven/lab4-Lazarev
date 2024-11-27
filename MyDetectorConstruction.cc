#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4PVPlacement.hh>
#include <G4LogicalVolume.hh>
#include <G4SystemOfUnits.hh>

#include "MyDetectorConstruction.hh"
#include "GeometrySize.hh"

G4VPhysicalVolume *MyDetectorConstruction::Construct() {
    // Get nist material manager
    auto nist = G4NistManager::Instance();

    // Option to switch on/off checking of volumes overlaps
    G4bool checkOverlaps = true;

    // World
    G4double world_sizeXY = 3 * meter;
    G4double world_sizeZ = 5 * meter;
    G4Material *world_mat = nist->FindOrBuildMaterial("G4_AIR");

    G4Box *solidWorld =
            new G4Box("World",                       //its name
                      0.5 * world_sizeXY,
                      0.5 * world_sizeXY,
                      0.5 * world_sizeZ);     //its size

    G4LogicalVolume *logicWorld =
            new G4LogicalVolume(solidWorld,          //its solid
                                world_mat,           //its material
                                "World");            //its name

    G4VPhysicalVolume *physWorld =
            new G4PVPlacement(0,                     //no rotation
                              G4ThreeVector(),       //at (0,0,0)
                              logicWorld,            //its logical volume
                              "World",               //its name
                              0,                     //its mother  volume
                              false,                 //no boolean operation
                              0,                     //copy number
                              checkOverlaps);        //overlaps checking


    detectorLogic = CreateDetector();
//
    auto rightDetector = new G4PVPlacement(0, G4ThreeVector(0, 0, detector_length / 2 + 0.1 * meter), detectorLogic,
                                           "rightDetector", logicWorld, false, 0);


    return physWorld;
}

MyDetectorConstruction::MyDetectorConstruction() {
    defineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction() {

}

G4LogicalVolume *MyDetectorConstruction::CreateDetector() {
    auto detectorSolid = new G4Box("detector",
                                    0.5 * detector_side_size,
                                     0.5 * detector_side_size,
                                  0.5 * detector_length);
    auto detector = new G4LogicalVolume(detectorSolid, vacuum, "detector");

    auto segmentLogic = CreateCalorimeterSection();

    for (int i = 0; i < calorimeter_number_of_segment; ++i) {
        std::string name = "segment_";
        name += std::to_string(i);
        auto segmentPhys = new G4PVPlacement(
                0,
                G4ThreeVector(0, 0,
                              10 * cm + (calorimeter_plastic_thickness +
                                         calorimeter_lead_thickness) * i),
                segmentLogic, name, detector, false, i);
    }
    return detector;
}

G4LogicalVolume *MyDetectorConstruction::CreateCalorimeterSection() {
    auto segmentSolid = new G4Box("segment", 0.5 * detector_side_size, 0.5 * detector_side_size,
                                  0.5 * (calorimeter_lead_thickness + calorimeter_plastic_thickness));
    auto segmentLogic = new G4LogicalVolume(segmentSolid, vacuum, "segment");
    auto leadSolid = new G4Box("lead", 0.5 * detector_side_size, 0.5 * detector_side_size,
                               0.5 * calorimeter_lead_thickness);
    auto plasticSolid = new G4Box("plastic", 0.5 * detector_side_size, 0.5 * detector_side_size,
                                  0.5 * calorimeter_plastic_thickness);
    auto leadLogic = new G4LogicalVolume(leadSolid, lead, "lead");

    auto plasticLogic = new G4LogicalVolume(plasticSolid, plastic, "plastic");

    auto leadPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, -calorimeter_lead_thickness / 2), leadLogic, "lead",
                                      segmentLogic, false, 0);
    auto plasticPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, calorimeter_plastic_thickness / 2), plasticLogic,
                                         "plastic", segmentLogic, false, 0);

    return segmentLogic;
}

void MyDetectorConstruction::defineMaterials() {
    auto nist = G4NistManager::Instance();
    vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    lead = nist->FindOrBuildMaterial("G4_Pb");
    plastic = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
}
