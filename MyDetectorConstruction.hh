#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4LogicalVolume.hh>

class MyDetectorConstruction : public G4VUserDetectorConstruction{
public:
    MyDetectorConstruction();
    ~MyDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

private:
    G4Material* vacuum;
    G4Material* lead;
    G4Material* plastic;
    G4LogicalVolume* CreateDetector();
    G4LogicalVolume* CreateCalorimeterSection();
    G4LogicalVolume *detectorLogic;
//    G4LogicalVolume *plasticLogic;

    void defineMaterials();

};


#endif //DETECTORCONSTRUCTION_HH