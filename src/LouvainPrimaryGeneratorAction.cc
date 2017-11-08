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
/// \file radioactivedecay/rdecay01/src/LouvainPrimaryGeneratorAction.cc
/// \brief Implementation of the LouvainPrimaryGeneratorAction class
//
//
// $Id$
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "LouvainPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4ParticleGun.hh"
#include "G4Event.hh"
#include <fstream>

   G4double lxx[100],lyy[100],lmaxx,lmaxy;
   G4int lnneutron = 1;
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LouvainPrimaryGeneratorAction::LouvainPrimaryGeneratorAction()
{
    G4int n_particle = 1;
    //  fParticleGun  = new G4ParticleGun(n_particle);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    //  G4ParticleDefinition* particlen = particleTable -> FindParticle("neutron");
    G4ParticleDefinition* particlen = particleTable -> FindParticle("neutron");

    fParticleGun  = new G4ParticleGun(particlen,n_particle);
    
    /* It opens the file with the initialization settings */
    // std::ifstream in;
    // in.open("/home/yechan/Workspace/Kang_bkg/UserData/Flusso_Louvain96.out");

    // G4double xtemp,ytemp;

    // in >> lmaxx >> lmaxy ;
    // for(G4int i=0; i<100;i++){
    //     in >> xtemp >> ytemp ;
    //     lxx[i]=xtemp;        
    //     lyy[i]=ytemp;        
    // }

    // in.close();
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LouvainPrimaryGeneratorAction::~LouvainPrimaryGeneratorAction()
{
    delete fParticleGun;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LouvainPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4double xxx,yyy;
    G4int  ifunc=-1;
    G4double ene;

    // set direction
    // neutron
    //Parallel Direction
    fParticleGun->SetParticleMomentumDirection((G4ThreeVector(0.,0.,1.))); 

    //set position  
    G4double gX = G4UniformRand()*10.-5.;
    G4double gY = G4UniformRand()*10.-5.;
    fParticleGun->SetParticlePosition(G4ThreeVector(gX,gY,-1.));
    
    // set energy
    // neutron
    // ifunc=-1;
    // while (1){
    //     xxx=G4UniformRand()*lmaxx;
    //     yyy=G4UniformRand()*lmaxy;

    //     for(G4int i=0; i<100;i++){
    //         if(xxx<lxx[i]) {ifunc=i;break;}
    //     }
    //     // if( (xx[ifunc]-xxx) >  (xxx-xx[ifunc-1])) ifunc=ifunc-1;

    //     if(yyy<lyy[ifunc]) break;

    // }

    //   ene = xxx;
    //   ene=150000.;  // constant for the moment to 150 GeV
    //   G4cout<<" Energy of muon   "<<ene<<G4endl;
    fParticleGun->SetParticleEnergy(1*GeV);
    
    //create vertex
    //   
    fParticleGun->GeneratePrimaryVertex(anEvent);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
