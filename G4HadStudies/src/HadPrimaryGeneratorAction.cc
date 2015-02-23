
#include "HadPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "Randomize.hh"
#include "HadAnalysis.hh"
#include <time.h>
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadPrimaryGeneratorAction::HadPrimaryGeneratorAction()
{
  particleGun  = new G4GeneralParticleSource();
  //particleGun->SetParticlePosition()
 // particleGun->SetParticleMomentumDirection(G4ParticleMomentum(0.,0.,1.));
  particleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  //particleGun->
  //particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadPrimaryGeneratorAction::~HadPrimaryGeneratorAction()
{
  delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //G4double zVertex = -46.*cm;
  particleGun->GeneratePrimaryVertex(anEvent);
  HadAnalysis* analysis = HadAnalysis::getInstance();
  Double_t ener = particleGun->GetParticleEnergy();
  //G4cout << "energyPrim " << ener << G4endl;
  //G4cout << ener << G4endl;
  G4ParticleDefinition* part  = particleGun->GetParticleDefinition();
  analysis->GetPrimGenInfo(ener,part);
  
 //G4double energy;
//G4double sigma= 0.;
//G4double ener =0.;
 // analysis->GetPrimGenInfo(ener,sigma,part);
  //energy = G4RandGauss::shoot(E,sigma);
//sigma1=sigma;
//energy = G4RandGauss::shoot(ener,sigma);
//energy *= MeV; 

 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
