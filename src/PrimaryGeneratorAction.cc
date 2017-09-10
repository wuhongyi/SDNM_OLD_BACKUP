//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file electromagnetic/TestEm4/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
// $Id: PrimaryGeneratorAction.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"
// #include "G4fissionEvent.hh"

#include <cmath>
#include <vector>
#include <iostream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),fParticleGun(0)
{
  G4int         n_particle        = 1;
  fParticleGun                    = new G4ParticleGun(n_particle);
 
  fParticleGun -> SetParticlePosition(G4ThreeVector(-76.2*mm,0.0,76.2*mm));
  
  PrimaryTimesContainer = new std::vector<G4double> ;
  fissionEvent = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  if( PrimaryTimesContainer) delete PrimaryTimesContainer;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // clean up PrimaryTimesContainer
  PrimaryTimesContainer->clear();  

  // G4double tot(0.0);
  // G4int nu(1);
  // PrimaryTimesContainer->push_back(tot);
  
  // G4double mean=1.0*s/4.1E4;//1.0*s/4.1E4;

  // G4RandExponential * fRExp = new G4RandExponential( new CLHEP::RanecuEngine, mean);
  // while (tot <= 0.2*s)
  // {
  //   tot   += fRExp->shoot(mean);
  //   PrimaryTimesContainer->push_back(tot);
  //   nu    += 1;
  // }
  // delete fRExp;

  // For Debug 
  // G4cout<<"Number of Particles in a Event:  "<<std::setw(6)<<nu<<G4endl;
  
  // for(G4int i=1; i < nu ; i++)
  // { 
    
    if(fissionEvent == NULL) fissionEvent= new wuFissionEvent(98252, 0.0, -1.0, 0.0);
    G4int nnu  = fissionEvent->getNeutronNu();
    G4int gnu  = fissionEvent->getPhotonNu();

//    G4cout<<" nnu: "<<nnu<<G4endl;
//    G4cout<<" gnu: "<<gnu<<G4endl;

    while( nnu == 0 && gnu == 0)
    {
      delete fissionEvent;
      fissionEvent=new wuFissionEvent(98252, 0.0, -1.0, 0.0);
      nnu  = fissionEvent->getNeutronNu();
      gnu  = fissionEvent->getPhotonNu();
    } 
   
    if( nnu > 0)
    {
       G4ParticleDefinition* particle  = G4ParticleTable::GetParticleTable()
                                     ->FindParticle("neutron");
       fParticleGun -> SetParticleDefinition(particle); 
       for(G4int j=0; j<nnu; j++)
       {
          G4ThreeVector direct;
          direct.setX(fissionEvent->getNeutronDircosu(j));
          direct.setY(fissionEvent->getNeutronDircosv(j));
          direct.setZ(fissionEvent->getNeutronDircosw(j));
          fParticleGun->SetParticleMomentumDirection(direct); 

          fParticleGun->SetParticleEnergy(fissionEvent->getNeutronEnergy(j));
          // fParticleGun->SetParticleTime( (*PrimaryTimesContainer)[i-1] );
          fParticleGun->GeneratePrimaryVertex(anEvent);
	  fParticleGun->SetParticleTime( 0 );
       }
       
    }



    if( gnu > 0)
    {
       G4ParticleDefinition* particle  = G4ParticleTable::GetParticleTable()
                                     ->FindParticle("gamma");
       fParticleGun -> SetParticleDefinition(particle); 
       for(G4int j=0; j<gnu; j++)
       {
          G4ThreeVector direct;
          direct.setX(fissionEvent->getPhotonDircosu(j));
          direct.setY(fissionEvent->getPhotonDircosv(j));
          direct.setZ(fissionEvent->getPhotonDircosw(j));
          fParticleGun->SetParticleMomentumDirection(direct); 

          fParticleGun->SetParticleEnergy(fissionEvent->getPhotonEnergy(j));
          // fParticleGun->SetParticleTime( (*PrimaryTimesContainer)[i-1] );
	  fParticleGun->SetParticleTime( 0 );
          fParticleGun->GeneratePrimaryVertex(anEvent);          
       }
    }
 
    delete fissionEvent;
    fissionEvent = NULL;

  // }     
 
  //For Debug
 // G4cout<<" Number of times in PrimaryTimesContainer: " << std::setw(6)
 //       << PrimaryTimesContainer->size() <<G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

