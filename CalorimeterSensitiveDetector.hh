#ifndef CALORIMETERSENSDET_HH
#define CALORIMETERSENSDET_HH

#include <G4VSensitiveDetector.hh>

class CalorimeterSensitiveDetector : public G4VSensitiveDetector {
public:
    CalorimeterSensitiveDetector(G4String name);

    ~CalorimeterSensitiveDetector();

    virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *history);

    virtual void Initialize(G4HCofThisEvent *event);

    virtual void EndOfEvent(G4HCofThisEvent *event);
};

#endif
