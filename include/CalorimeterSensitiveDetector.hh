#ifndef CALSENSDET_HH
#define CALSENSDET_HH

#include <G4VSensitiveDetector.hh>
#include <G4SDManager.hh>
#include <iostream>

#include "GeometrySize.hh"
#include "TupleID.hh"

class CalorimeterSensitiveDetector: public G4VSensitiveDetector {
public:
    CalorimeterSensitiveDetector(G4String name, TupleID *tupleID);
    ~CalorimeterSensitiveDetector();

    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *history);
    void Initialize(G4HCofThisEvent *event);
    void EndOfEvent(G4HCofThisEvent *event);
private:
    TupleID *tupleID;

    double right[calorimeter_number_of_segment];
    double left[calorimeter_number_of_segment];
};

#endif
