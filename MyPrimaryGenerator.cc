#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4SystemOfUnits.hh>

#include "MyPrimaryGenerator.hh"

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
/**
 * TODO()
 * Ваша задача, используя код который вы написали на прошлых занятиях,
 * генерировать начальные частицы возникающие при распаде Pi мезона.
 */
    G4ThreeVector mom(0., 0., 1.);

    fParticleGun->SetParticleMomentumDirection(mom);
    fParticleGun->SetParticleEnergy(60.0 * MeV);

    // Генерируем первую первичную частицы для события
    fParticleGun->SetParticleDefinition(G4Electron::Definition());
    fParticleGun->GeneratePrimaryVertex(anEvent);

    // Генерируем вторую первичную частицу для события
    fParticleGun->SetParticleDefinition(G4Gamma::Definition());
    fParticleGun->GeneratePrimaryVertex(anEvent);
}

MyPrimaryGenerator::MyPrimaryGenerator() : G4VUserPrimaryGeneratorAction() {
    G4int n_particle = 1;
    fParticleGun = new G4ParticleGun(n_particle);
}

MyPrimaryGenerator::~MyPrimaryGenerator() {

}
