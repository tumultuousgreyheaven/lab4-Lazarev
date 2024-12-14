#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4ParticleGun.hh>
#include <G4ParticleTable.hh>

#include "PiDecayGenerator.hh"


class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
public:
    explicit MyPrimaryGenerator(bool isPiDecay);
    ~MyPrimaryGenerator();

    void GeneratePrimaries(G4Event *anEvent) override;

private:
    bool isPiDecay;
    G4ParticleGun *fParticleGun;
    PiDecayGenerator *piDecay;
    G4ParticleTable *particleTable;
};


#endif //PRIMARYGENERATORACTION_HH