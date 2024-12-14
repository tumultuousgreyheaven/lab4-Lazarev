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

    auto rotation = new G4RotationMatrix(CLHEP::pi, CLHEP::pi, CLHEP::pi);
    auto leftDetector = new G4PVPlacement(rotation, G4ThreeVector(0, 0, -(detector_length / 2 + 0.1 * meter)), detectorLogic,
                                           "leftDetector", logicWorld, false, 1);

    return physWorld;
}

MyDetectorConstruction::MyDetectorConstruction(TupleID *tupleID) {
    this->tupleID = tupleID;
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

    auto trackingSectionLogic = createTrackingSection();

    auto trackingLeftPhys = new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, -0.5 * m),
                                              trackingSectionLogic, "leftSectionLogic", detector, false, 0);
    auto trackingRightPhys = new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, -0.5 * m + distance_tracking_area),
                                              trackingSectionLogic, "rightSectionLogic", detector, false, 0);

    auto magnetLogic = createMagnet();
    auto magnetPhys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -0.5 * m + 0.5 * distance_tracking_area),
                                        magnetLogic, "magnet", detector, false, 0);

    return detector;
}

G4LogicalVolume *MyDetectorConstruction::CreateCalorimeterSection() {
    auto segmentSolid = new G4Box("segment", 0.5 * detector_side_size, 0.5 * detector_side_size,
                                  0.5 * (calorimeter_lead_thickness + calorimeter_plastic_thickness));
    auto segmentLogic = new G4LogicalVolume(segmentSolid, vacuum, "segment");
    auto leadSolid = new G4Box("lead", 0.5 * detector_side_size, 0.5 * detector_side_size,
                               0.5 * calorimeter_lead_thickness);
    plasticSolid = new G4Box("plastic", 0.5 * detector_side_size, 0.5 * detector_side_size,
                                  0.5 * calorimeter_plastic_thickness);
    auto leadLogic = new G4LogicalVolume(leadSolid, lead, "lead");

    plasticLogic = new G4LogicalVolume(plasticSolid, plastic, "plastic");

    auto leadPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, -calorimeter_lead_thickness / 2), leadLogic, "lead",
                                      segmentLogic, false, 0);
    auto plasticPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, calorimeter_plastic_thickness / 2), plasticLogic,
                                         "plastic", segmentLogic, false, 0);

    return segmentLogic;
}

G4LogicalVolume *MyDetectorConstruction::createTrackingLayer() {
    auto layerSolid = new G4Box("layer",
                                0.5 * detector_side_size,
                                0.5 * detector_side_size,
                                0.5 * tracking_cell_thickness);
    auto layerLogic = new G4LogicalVolume(layerSolid, vacuum, "layer");

    auto siliconSolid = new G4Box("siliconBox",
                                  0.5 * tracking_cell_size,
                                  0.5 * tracking_cell_size,
                                  0.5 * tracking_cell_thickness);
    siliconLogic = new G4LogicalVolume(siliconSolid, silicon, "siliconBox");

    for (int i = 0; i < number_of_tracking_cell; ++i) {
        for (int j = 0; j < number_of_tracking_cell; ++j) {
            double x = tracking_cell_size / 2 + i * tracking_cell_size - tracking_cell_size * (number_of_tracking_cell / 2);
            double y = tracking_cell_size / 2 + j * tracking_cell_size - tracking_cell_size * (number_of_tracking_cell / 2);

            auto siliconPhys = new G4PVPlacement(nullptr, G4ThreeVector(x, y, 0.0), siliconLogic, "siliconPhys", layerLogic, false, i * number_of_tracking_cell + j);
        }
    }

    return layerLogic;
}

G4LogicalVolume *MyDetectorConstruction::createTrackingSection() {
    auto sectionSolid = new G4Box("segment",
                                  0.5 * detector_side_size,
                                  0.5 * detector_side_size,
                                  0.5 * (distance_tracking_layer + 2 * tracking_cell_thickness));
    auto sectionLogic = new G4LogicalVolume(sectionSolid, vacuum, "segment");

    auto layerLogic = createTrackingLayer();
    auto rightLayer = new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, -0.5 * (distance_tracking_layer + tracking_cell_thickness)),
                                        layerLogic, "rightLayer", sectionLogic, false, 0);
    auto leftLayer = new G4PVPlacement(nullptr, G4ThreeVector(0.0, 0.0, 0.5 * (distance_tracking_layer + tracking_cell_thickness)),
                                        layerLogic, "rightLayer", sectionLogic, false, 0);
    
    return sectionLogic;
}

void MyDetectorConstruction::defineMaterials() {
    auto nist = G4NistManager::Instance();
    vacuum = nist->FindOrBuildMaterial("G4_Galactic");
    lead = nist->FindOrBuildMaterial("G4_Pb");
    plastic = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
    silicon = nist->FindOrBuildMaterial("G4_Si");
}

G4LogicalVolume* MyDetectorConstruction::createMagnet() {
    auto magnetSolid = new G4Box("magnet", 0.5 * detector_side_size, 0.5 * detector_side_size, 1 * cm);
    magnetLogic = new G4LogicalVolume(magnetSolid, vacuum, "magnet");

    return magnetLogic;
}

void MyDetectorConstruction::setupDetectors() {
    auto sdman = G4SDManager::GetSDMpointer();
    auto calorimeterSensDet = new CalorimeterSensitiveDetector("CalorimeterSensitiveDetector", tupleID);
    sdman->AddNewDetector(calorimeterSensDet);
    plasticLogic->SetSensitiveDetector(calorimeterSensDet);

    auto trackerSensDet = new TrackerSensitiveDetector("TrackerSensitiveDetector", tupleID);
    sdman->AddNewDetector(trackerSensDet);
    siliconLogic->SetSensitiveDetector(trackerSensDet);
    // sdman->Activate("CalorimeterSensitiveDetector", false);
}

void MyDetectorConstruction::ConstructSDandField() {
    G4MagneticField *magneticField;
    magneticField = new G4UniformMagField(G4ThreeVector(0.0, 70 * kilogauss, 0.0));

    auto fieldManager = new G4FieldManager();
    fieldManager->SetDetectorField(magneticField);
    fieldManager->CreateChordFinder(magneticField);
    magnetLogic->SetFieldManager(fieldManager, true);

    setupDetectors();
}
