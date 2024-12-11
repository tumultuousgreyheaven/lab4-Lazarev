#include "CalorimeterSensitiveDetector.hh"

CalorimeterSensitiveDetector::CalorimeterSensitiveDetector(G4String name,
                                                           TupleID *tupleID) : G4VSensitiveDetector(name) {
    this->tupleID = tupleID;
}

CalorimeterSensitiveDetector::~CalorimeterSensitiveDetector() {}

G4bool CalorimeterSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *history) {
    auto aTrack = aStep->GetTrack();
//    std::cout <<std::endl;
//    std::cout << "<< Start Calorimeter Hit >>"<<std::endl;
//    std::cout <<"Track no.: "  << aStep->GetTrack()->GetTrackID()<<std::endl;
//    std::cout <<"Volume name: "  << aStep->GetTrack()->GetVolume()->GetName()<<std::endl;
//    auto parentVolume = aStep->GetTrack()->GetTouchable()->GetVolume(1);
//    std::cout << "Parent volume name and number: " << parentVolume->GetName()<< " " << parentVolume->GetCopyNo() <<std::endl;
//    auto grandParentVolume = aStep->GetTrack()->GetTouchable()->GetVolume(2);
//    std::cout << "Grandparent volume name and number: " << grandParentVolume->GetName()<<" "<< grandParentVolume->GetCopyNo()<<std::endl;
//    std::cout << "Energy deposit in the step: "<<aStep->GetTotalEnergyDeposit()<<std::endl;
//    std::cout << "<< End Calorimeter Hit >>"<<std::endl;
    auto detectorNumber = aTrack->GetTouchable()->GetVolume(2)->GetCopyNo();
    auto segmentNumber = aTrack->GetTouchable()->GetVolume(1)->GetCopyNo();

    if (detectorNumber == 0) {
        right[segmentNumber] += aStep->GetTotalEnergyDeposit();
    } else {
        left[segmentNumber] += aStep->GetTotalEnergyDeposit();
    }

    return true;
}

void CalorimeterSensitiveDetector::Initialize(G4HCofThisEvent *event) {
    G4VSensitiveDetector::Initialize(event);

    //Clear the stored data
    for (int iSegment = 0; iSegment < calorimeter_number_of_segment; ++iSegment) {
        right[iSegment] = 0;
        left[iSegment] = 0;
    }
}

void CalorimeterSensitiveDetector::EndOfEvent(G4HCofThisEvent *event) {
    G4VSensitiveDetector::EndOfEvent(event);

    //Save the data
    auto analysisManager = tupleID->analysisManager;
    for (int iSegment = 0; iSegment < calorimeter_number_of_segment; ++iSegment) {
        analysisManager->FillNtupleDColumn(tupleID->leftCalID, iSegment, left[iSegment]);
        analysisManager->FillNtupleDColumn(tupleID->rightCalID, iSegment, right[iSegment]);
    }
    analysisManager->AddNtupleRow(tupleID->leftCalID);
    analysisManager->AddNtupleRow(tupleID->rightCalID);
}
