#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4LogicalVolume.hh>
#include <G4MagneticField.hh>
#include <G4UniformMagField.hh>
#include <G4FieldManager.hh>

#include "CalorimeterSensitiveDetector.hh"
#include "TrackerSensitiveDetector.hh"
#include "TupleID.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction{
public:
    MyDetectorConstruction(TupleID *tupleID);
    ~MyDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();
    virtual void ConstructSDandField();

    void setupDetectors();

private:
    G4Material* vacuum;
    G4Material* lead;
    G4Material* plastic;
    G4LogicalVolume* CreateDetector();
    G4LogicalVolume* CreateCalorimeterSection();
    G4LogicalVolume *detectorLogic;
    G4LogicalVolume *plasticLogic;
    G4LogicalVolume *magnetLogic;
    G4LogicalVolume* createMagnet();
    G4LogicalVolume *siliconLogic;

    G4Material *silicon;
    G4LogicalVolume *createTrackingLayer();
    G4LogicalVolume *createTrackingSection();

    G4Box* plasticSolid;

    TupleID *tupleID;

    void defineMaterials();

};


#endif //DETECTORCONSTRUCTION_HH