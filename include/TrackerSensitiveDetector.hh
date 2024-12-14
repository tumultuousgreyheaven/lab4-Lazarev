#ifndef TRACKSENSDET_HH
#define TRACKSENSDET_HH

#include <G4VSensitiveDetector.hh>
#include <G4SDManager.hh>
#include <G4Gamma.hh>
#include <iostream>
#include <vector>

#include "TupleID.hh"

struct Address {
    int detector;
    int segment;
    int layer;
    int cell;
};

struct TrackerData {
    Address address;
    double time;
    double energyDeposit;
    G4ThreeVector position;
    int detector;
};

class TrackerSensitiveDetector: public G4VSensitiveDetector {
public:
    TrackerSensitiveDetector(G4String name, TupleID *tupleID);
    ~TrackerSensitiveDetector();

    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *history);
    void Initialize(G4HCofThisEvent *event);
    void EndOfEvent(G4HCofThisEvent *event);
private:
    TupleID *tupleID;
    std::vector<TrackerData> data;
};

#endif
