#include "CalorimeterSensitiveDetector.hh"

CalorimeterSensitiveDetector::CalorimeterSensitiveDetector(G4String name) : G4VSensitiveDetector(name) {}

CalorimeterSensitiveDetector::~CalorimeterSensitiveDetector() {}

G4bool CalorimeterSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *history) {
    std::cout <<std::endl;
    std::cout << "<< Start Calorimeter Hit >>"<<std::endl;
    std::cout <<"Track no.: "  << aStep->GetTrack()->GetTrackID()<<std::endl;
    std::cout <<"Volume name: "  << aStep->GetTrack()->GetVolume()->GetName()<<std::endl;
    auto parentVolume = aStep->GetTrack()->GetTouchable()->GetVolume(1);
    std::cout << "Parent volume name and number: " << parentVolume->GetName()<< " " << parentVolume->GetCopyNo() <<std::endl;
    auto grandParentVolume = aStep->GetTrack()->GetTouchable()->GetVolume(2);
    std::cout << "Grandparent volume name and number: " << grandParentVolume->GetName()<<" "<< grandParentVolume->GetCopyNo()<<std::endl;
    std::cout << "Energy deposit in the step: "<<aStep->GetTotalEnergyDeposit()<<std::endl;
    std::cout << "<< End Calorimeter Hit >>"<<std::endl;

    return true;
}

void CalorimeterSensitiveDetector::Initialize(G4HCofThisEvent *event) {
    std::cout << "This is start of " << event << "\n";
    G4VSensitiveDetector::Initialize(event);
}

void CalorimeterSensitiveDetector::EndOfEvent(G4HCofThisEvent *event) {
    std::cout << "This is end of " << event << "\n";
    G4VSensitiveDetector::EndOfEvent(event);
}
