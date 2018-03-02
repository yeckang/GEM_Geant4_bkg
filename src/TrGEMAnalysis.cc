#include "TrGEMAnalysis.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4String.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

#include <TROOT.h>
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>

TrGEMAnalysis* TrGEMAnalysis::singleton = 0 ;

TrGEMAnalysis* TrGEMAnalysis::GetInstance() {
  if ( singleton == 0 ) {
    static TrGEMAnalysis analysis;
    singleton = &analysis;
  }
  return singleton;
}

TrGEMAnalysis::~TrGEMAnalysis() 
{}

TrGEMAnalysis::TrGEMAnalysis() 
{
  m_ROOT_file = 0;
}

void TrGEMAnalysis::SetFileName(const G4String name)
{
  fileName = name;
}


void TrGEMAnalysis::PrepareNewRun(const G4Run* /*aRun*/)
{

  eventCounter = 0 ;
  //Reset variables relative to the run
  thisRunTotEM  = 0;
  thisRunTotEM2 = 0;
  thisRunCentralEM  = 0;
  thisRunCentralEM2 = 0;
  n_gamma = 0;
  n_electron = 0;
  n_positron = 0;
  
  // create ROOT file
  fileName = fileName + ".root";
  m_ROOT_file = new TFile(fileName.c_str(),"RECREATE");
  if(m_ROOT_file) 
    G4cout << "ROOT file task3.root is created " << G4endl;
  else {
      //G4Exception("ROOT file task3.root has not been created!");
    G4cerr << "ROOT file task3.root has not been created!" << G4endl;
  }

  // create histograms
  m_ROOT_histo0 = new TH1D("etot","Total energy deposit normalized to beam energy",100,0,1);
  //m_ROOT_histo1 = new TH1D("e0","Energy deposit in central crystal normalized to beam energy",100,0,1);
  m_ROOT_histo2 = new TH1D("ez","Energy profile along the GEM (mm)",100,0,20);
  m_ROOT_histo3 = new TH1D("egap","Energy profile vs. GasGap",5,0,5);
  //ntuple = new TNtuple("gasgap1","secondaries in gas gap 1","gapGamma:gapElectron:gapPositron") ;

  // create tree and branch(es)
  t = new TTree("MyRun","Run with awesome data") ;

  // t->Branch("driftEdep",&driftEdep,"driftEdep/D") ;
  // t->Branch("driftEdepI",&driftEdepI,"driftEdepI/D") ;
  // t->Branch("transferEdep",&transferEdep, "transferEdep/D") ;
  // t->Branch("transferEdepI",&transferEdepI, "transferEdepI/D") ;
  // t->Branch("transfer2Edep",&transfer2Edep, "transfer2Edep/D") ;
  // t->Branch("transfer2EdepI",&transfer2EdepI, "transfer2EdepI/D") ;
  // t->Branch("inductionEdep",&inductionEdep, "inductionEdep/D") ;
  // t->Branch("inductionEdepI",&inductionEdepI, "inductionEdepI/D") ;
  // t->Branch("driftEdep_B",&driftEdep_B,"driftEdep_B/D") ;
  // t->Branch("driftEdepI_B",&driftEdepI_B,"driftEdepI_B/D") ;
  // t->Branch("transferEdep_B",&transferEdep_B, "transferEdep_B/D") ;
  // t->Branch("transferEdepI_B",&transferEdepI_B, "transferEdepI_B/D") ;
  // t->Branch("transfer2Edep_B",&transfer2Edep_B, "transfer2Edep_B/D") ;
  // t->Branch("transfer2EdepI_B",&transfer2EdepI_B, "transfer2EdepI_B/D") ;
  // t->Branch("inductionEdep_B",&inductionEdep_B, "inductionEdep_B/D") ;
  // t->Branch("inductionEdepI_B",&inductionEdepI_B, "inductionEdepI_B/D") ;

  // t->Branch("driftEdep",&driftEdep) ;
  // t->Branch("driftEdepI",&driftEdepI) ;
  // t->Branch("transfer1Edep",&transfer1Edep) ;
  // t->Branch("transfer1EdepI",&transfer1EdepI) ;
  // t->Branch("transfer2Edep",&transfer2Edep) ;
  // t->Branch("transfer2EdepI",&transfer2EdepI) ;
  // t->Branch("inductionEdep",&inductionEdep) ;
  // t->Branch("inductionEdepI",&inductionEdepI) ;

  t->Branch("primaryEne",&primaryEne, "primaryEne/D") ;
  t->Branch("zInteraction",&zInteraction, "zInteraction/D") ;
  
  t->Branch("gapTrackPart",&gapTrackPart);
  t->Branch("gapTrackCharge", &gapTrackCharge);
  t->Branch("gapTrackGeneration", &gapTrackGeneration);
  t->Branch("gapTrackName", &gapTrackName);
  t->Branch("gapTrackGenProcess",&gapTrackGenProcess);
  t->Branch("gapTrackVolume",&gapTrackVolume);
  t->Branch("gapTrackGenZ",&gapTrackGenZ);
  t->Branch("gapTrackGap",&gapTrackGap);
  t->Branch("gapTrackEne",&gapTrackEne);

  
  t->Branch("postTrackPart",&postTrackPart);
  t->Branch("postTrackEne",&postTrackEne);

 
  g = new TTree("Garfield","Interesting variables for Garfield") ;
  //  g->Branch("globalTime",&globalTime,"globalTime/D") ;
  g->Branch("pdgCode",&pdgCode) ;
  g->Branch("kineticEnergy",&kineticEnergy) ;
  g->Branch("positionX",&positionX) ;
  g->Branch("positionY",&positionY) ;
  g->Branch("positionZ",&positionZ) ;
  g->Branch("momentumX",&momentumX) ;
  g->Branch("momentumY",&momentumY) ;
  g->Branch("momentumZ",&momentumZ) ;
  // g->Branch("momentumDirectionX",&momentumDirectionX,"momentumDirectionX/D") ;
  // g->Branch("momentumDirectionY",&momentumDirectionY,"momentumDirectionY/D") ;
  // g->Branch("momentumDirectionZ",&momentumDirectionZ,"momentumDirectionZ/D") ;

  v = new TTree("Generating", "Generating Data") ;
  v->Branch("generatingPartCode", &generatingPartCode) ;
  v->Branch("generatingPartName", &generatingPartName) ;
  v->Branch("generatingProcess", &generatingProcess) ;
  v->Branch("generatingEnergy", &generatingEnergy) ;
  v->Branch("generatingVolume", &generatingVolume) ;
  v->Branch("generatingIntNum", &generatingIntNum) ;

  d = new TTree("Edep", "Energy Deposition Data") ;
  d->Branch("edepVolume", &edepVolume);
  d->Branch("edep", &edep);
  d->Branch("edepI", &edepI);
  d->Branch("edepTime", &edepTime);
}

void TrGEMAnalysis::PrepareNewEvent(const G4Event* /*anEvent*/)
{
  //Reset variables relative to this event
  isNewEvent = true ;
  thisEventTotEM = 0;
  thisEventCentralEM = 0;
  thisEventSecondaries = 0;

  elexevt = 0 ;
  posxevt = 0 ;
  gammaxevt = 0 ;
  secoxevt = 0 ;

  // driftEdep = 0. ;
  // driftEdepI = 0. ;

  // transferEdep = 0. ;
  // transferEdepI = 0. ;
  
  // transfer2Edep  = 0. ;
  // transfer2EdepI  = 0. ;

  // inductionEdep  = 0. ;
  // inductionEdepI  = 0. ;

  // driftEdep_B  = 0. ;
  // driftEdepI_B  = 0. ;

  // transferEdep_B  = 0. ;
  // transferEdepI_B  = 0. ;

  // transfer2Edep_B  = 0. ;
  // transfer2EdepI_B  = 0. ;

  // inductionEdep_B  = 0. ;
  // inductionEdepI_B  = 0. ;
  
  // driftEdep.clear();
  // driftEdepI.clear();

  // transfer1Edep.clear();
  // transfer1EdepI.clear();
  
  // transfer2Edep.clear();
  // transfer2EdepI.clear();

  // inductionEdep.clear();
  // inductionEdepI.clear();

  edepVolume.clear();
  edep.clear();
  edepI.clear();
  edepTime.clear();

  primaryEne=0.;
  zInteraction=0.;
  
  gapTrackPart.clear();
  gapTrackCharge.clear();
  gapTrackGeneration.clear();
  gapTrackName.clear();
  gapTrackGenProcess.clear();
  gapTrackVolume.clear();
  gapTrackGenZ.clear();
  gapTrackGap.clear();
  gapTrackEne.clear();
  
  postTrackPart.clear();
  postTrackEne.clear();
  
  pdgCode.clear();
  kineticEnergy.clear();
  positionX.clear();
  positionY.clear();
  positionZ.clear();
  momentumX.clear();
  momentumY.clear();
  momentumZ.clear();

  generatingPartCode.clear();
  generatingPartName.clear();
  generatingProcess.clear();
  generatingEnergy.clear();
  generatingVolume.clear();
  generatingIntNum.clear();
  genMap.clear();
}

void TrGEMAnalysis::EndOfEvent(const G4Event* /*anEvent*/)
{
  //Accumulate over the run
  thisRunTotEM += thisEventTotEM;

  //Uncomment these lines for more verbosity:
  //G4cout<<"Event: "<< anEvent->GetEventID() <<" Energy in EM calo: "
  //<<G4BestUnit(thisEventTotEM,"Energy")<<" Secondaries: "<<thisEventSecondaries<<G4endl;
  //G4cout<<G4BestUnit(thisEventCentralEM,"Energy")<<G4endl;

  // save information to ROOT
  m_ROOT_histo0->Fill(thisEventTotEM/beamEnergy, 1.0);
  //ntuple->Fill(double(n_gapGamma[0]),double(n_gapElectron[0]),double(n_gapPositron[0])) ;

  t->Fill() ; 
  g->Fill() ;
  v->Fill() ;
  d->Fill() ;
}

void TrGEMAnalysis::EndOfRun(const G4Run* aRun)
{

  //Some print outs
  G4int numEvents = aRun->GetNumberOfEvent();
  if(numEvents == 0) { return; }

  //G4double norm = numEvents*beamEnergy;
  G4cout << "================="<<G4endl;
  G4cout << "Summary for run: " << aRun->GetRunID() << G4endl ;
  G4cout << "  Beam of " << beamParticle->GetParticleName() 
      << " kinetic energy: "<< G4BestUnit(beamEnergy,"Energy") << G4endl ;
  G4cout << "  Event processed:         " << numEvents << G4endl ;
  G4cout << "  Average number of gamma: "<<(G4double)n_gamma/(G4double)numEvents<<G4endl ;
  G4cout << "  Average number of e-   : "<<(G4double)n_electron/(G4double)numEvents<<G4endl ;
  G4cout << "  Average number of e+   : "<<(G4double)n_positron/(G4double)numEvents<<G4endl ;
  for(G4int i = 0; i < 4; i++) {
    G4cout << "Gap number " << i + 1 << G4endl ;
    G4cout << "  Average number of gamma: " << (G4double)n_gapGamma[i]/(G4double)numEvents << G4endl ;
    G4cout << "  Average number of e-   : " << (G4double)n_gapElectron[i]/(G4double)numEvents << G4endl ;
    G4cout << "  Average number of e+   : " << (G4double)n_gapPositron[i]/(G4double)numEvents << G4endl ;
    G4cout << "  - - - - - - - - - - " << G4endl ;
  }
  G4cout << "  Average energy deposition in TrGEM: "
         << G4BestUnit(thisRunTotEM/(G4double)numEvents,"Energy") << G4endl ;

  // Writing and closing the ROOT file
  m_ROOT_file->cd() ;
  t->Write() ;
  g->Write() ;
  v->Write() ;
  d->Write() ;
  G4cout << "ROOT: files writing..." << G4endl;
  m_ROOT_file->Write();
  G4cout << "ROOT: files closing..." << G4endl;
  m_ROOT_file->Close();
  delete m_ROOT_file;

}

void TrGEMAnalysis::AddSecondary(const G4ParticleDefinition* part)
{
  if(part == G4Gamma::Gamma())            { ++n_gamma; }
  else if(part == G4Electron::Electron()) { ++n_electron; }  
  else if(part == G4Positron::Positron()) { ++n_positron; }
}


void TrGEMAnalysis::AddGapSecondary(const G4ParticleDefinition* part, G4int gapNum)
{
  gapNum-- ;
  if(part == G4Gamma::Gamma())            { ++n_gapGamma[gapNum]; }
  else if(part == G4Electron::Electron()) { ++n_gapElectron[gapNum]; }  
  else if(part == G4Positron::Positron()) { ++n_gapPositron[gapNum]; }
}

void TrGEMAnalysis::AddEDep(G4double edep, G4double z)
{
  thisEventTotEM += edep;
  m_ROOT_histo2->Fill(z, edep);
}

void TrGEMAnalysis::AddEDepSD(G4double edep, G4int layerIndex)
{
  m_ROOT_histo3->Fill(layerIndex, edep);
}

void TrGEMAnalysis::SetBeam(const G4ParticleDefinition* part, G4double energy)
{
  beamParticle = part;
  beamEnergy = energy;
}

void TrGEMAnalysis::AddParticlesPerEvent(G4int PDGCode) {
  secoxevt++ ;
  if(PDGCode == 11) elexevt++ ;
  else if(PDGCode == -11) posxevt++ ;
  else if(PDGCode == 22) gammaxevt++ ;
  else G4cout << "You must implement a new variable" << G4endl ; 
}

//Save Energy Deposition/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// void TrGEMAnalysis::SetDriftSensitivity(G4double someDriftEdep,G4double someDriftEdepI) 
// {
//   driftEdep.push_back(someDriftEdep);
//   driftEdepI.push_back(someDriftEdepI);
// }

// void TrGEMAnalysis::SetTransfer1Sensitivity(G4double someTransfer1Edep,G4double someTransfer2EdepI) 
// {
//   transfer1Edep.push_back(someTransfer1Edep);
//   transfer1EdepI.push_back(someTransfer2EdepI);
// }

// void TrGEMAnalysis::SetTransfer2Sensitivity(G4double someTransfer2Edep,G4double someTransfer2EdepI) 
// {
//   transfer2Edep.push_back(someTransfer2Edep);
//   transfer2EdepI.push_back(someTransfer2EdepI);
// }

// void TrGEMAnalysis::SetInductionSensitivity(G4double someInductionEdep,G4double someInductionEdepI) 
// {
//   inductionEdep.push_back(someInductionEdep);
//   inductionEdepI.push_back(someInductionEdepI);
// }

void TrGEMAnalysis::SetEnergyDeposition(std::string someVolme, G4double someEdep, G4double someEdepI, G4double someTime)
{
  edepVolume.push_back(someVolme);
  edep.push_back(someEdep);
  edepI.push_back(someEdepI);
  edepTime.push_back(someTime);
}

//Save Primary data/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TrGEMAnalysis::SavePrimary(G4double primaryene, G4double zinteraction){

  primaryEne = primaryene;
  zInteraction = zinteraction;
   
}

//Save gap track data///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TrGEMAnalysis::SaveGapTrack(
  G4int gapPart, 
  G4int gapCharge,
  G4int generation,
  std::string name,
  std::string genprocess, 
  std::string genvolume, 
  G4double genz, 
  std::string volname,  
  G4double kinene) 
{
  gapTrackPart.push_back(gapPart) ;
  gapTrackCharge.push_back(gapCharge) ;
  gapTrackGeneration.push_back(generation) ;
  gapTrackName.push_back(name) ;
  gapTrackGenProcess.push_back(genprocess) ;
  gapTrackVolume.push_back(genvolume) ;
  gapTrackGenZ.push_back(genz) ;
  gapTrackGap.push_back(volname) ;
  gapTrackEne.push_back(kinene) ;
}

void TrGEMAnalysis::SavePostShieldTrack(G4int postPart,  G4double postEne ) {
  postTrackPart.push_back(postPart) ;
  postTrackEne.push_back(postEne) ;
}

//Save Garfield data////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void TrGEMAnalysis::SaveGarfieldQuantities( 
  G4int aPdgCode,
  G4double aKineticEnergy,
  G4double aPositionX, 
  G4double aPositionY, 
  G4double aPositionZ,
  G4double aMomentumX, 
  G4double aMomentumY, 
  G4double aMomentumZ) {

  pdgCode.push_back(aPdgCode) ;
  kineticEnergy.push_back( aKineticEnergy ) ;
  positionX.push_back( aPositionX );
  positionY.push_back( aPositionY );
  positionZ.push_back( aPositionZ );
  momentumX.push_back( aMomentumX );
  momentumY.push_back( aMomentumY );
  momentumZ.push_back( aMomentumZ );

}

void TrGEMAnalysis::SaveGeneratingTrack(
  G4int partCode, 
  std::string partName, 
  std::string process, 
  G4double energy, 
  std::string volume, 
  G4int trackID, 
  G4int parentID)
{
  G4int intNum = 0;
  G4int id = trackID;
  genMap[id] = parentID;
  while(genMap[id] != 0)
  {
    intNum++;
    id = genMap[id];
  }
  generatingPartCode.push_back(partCode);
  generatingPartName.push_back(partName);
  generatingProcess.push_back(process);
  generatingEnergy.push_back(energy);
  generatingVolume.push_back(volume);
  generatingIntNum.push_back(intNum);
}