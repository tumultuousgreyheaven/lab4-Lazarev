#ifndef TUPLEID_HH
#define TUPLEID_HH

#include <G4AnalysisManager.hh>

struct TupleID {
    G4AnalysisManager *analysisManager;
    int leftCalID;
    int rightCalID;
    int leftTrackID;
    int rightTrackID;
};

#endif
