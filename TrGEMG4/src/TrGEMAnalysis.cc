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
  NomeStrati= 
  {
    "FakeBottom",                                   //Fake
    "DriftCopper1","DriftBoard","DriftCopper2",     //Drift Board
    "GasGap1",                                      //Drift Gap
    "Gem1Copper1","Gem1","Gem1Copper2",             //gem1
    "GasGap2",                                      //Transfer I Gap
    "Gem2Copper1","Gem2","Gem2Copper2",             //gem2
    "GasGap3",                                      //Transfer II Gap
    "Gem3Copper1","Gem3","Gem3Copper2",             //gem3
    "GasGap4",                                      //Induction Gap
    "ReadCopper1","ReadoutBoard","ReadCopper2",     //Readout Board
    "FakeTop"                                       //Fake
  };
  posProcess=                                       //It comes from FTFP_BERT_HP
  {
    "Transportation", "msc", "hIoni", "ionIoni", "hBrems", "hPairProd", "CoulombScat",
    "eIoni", "eBrem", "annihil", "phot", "compt", "conv", "muIoni", "muBrems", "muPairProd",
    "photonNuclear", "electronNuclear", "positronNuclear", "muonNuclear", "Decay", "hadElastic",
    "neutronInelastic", "nCapture", "nFission", "protonInelastic", 
    "pi+Inelastic", "pi-Inelastic", "kaon+Inelastic", "kaon-Inelastic", "kaon0LInelastic", "kaon0SInelastic",
    "lambdaInelastic", "anti-lambdaInelastic", "sigma-Inelastic", "anti_sigma-Inelastic", "sigma+Inelastic",
    "anti_sigma+Inelastic", "xi-Inelastic", "anti_xi-Inelastic", "xi0Inelastic", "anti_xi0Inelastic",
    "omega-Inelastic", "anti_omega-Inelastic", "anti_protonInelastic", "anti_neutronInelastic", "anti_deuteronInelastic",
    "anti_tritonInelastic", "anti_He3Inelastic", "anti_alphaInelastic", "hFritiofCaptureAtRest", "hBertiniCaptureAtRest",
    "muMinusCaptureAtRest", "dInelastic", "tInelastic", "He3Inelastic", "alphaInelastic", "ionInelastic ",
  };
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
    G4cout << "ROOT file " << fileName << " is created " << G4endl;
  else {
      //G4Exception("ROOT file task3.root has not been created!");
    G4cerr << "ROOT file task3.root has not been created!" << G4endl;
  }

  // create histograms
  m_ROOT_histo0 = new TH1D("etot","Total energy deposit normalized to beam energy",100,0,1);
  //m_ROOT_histo1 = new TH1D("e0","Energy deposit in central crystal normalized to beam energy",100,0,1);
  m_ROOT_histo2 = new TH1D("ez","Energy profile along the GEM (mm)",100,0,20);
  m_ROOT_histo3 = new TH1D("egap","Energy profile vs. GasGap",5,0,5);

  // create tree and branch(es)
  t = new TTree("Event","Run with awesome data") ;

  t->Branch("primaryEne",&primaryEne, "primaryEne/D") ;
  t->Branch("zInteraction",&zInteraction, "zInteraction/D") ;

  t->Branch("EleGap", &eleGap, "EleGap/I");
  t->Branch("PosGap", &posGap, "PosGap/I");
  t->Branch("ChargeGap", &chargeGap, "ChargeGap/I");
  
  //Variables for sensitivity study, It will be used for drawing plots
  t->Branch("gapTrackPart",&gapTrackPart);
  t->Branch("gapTrackCharge", &gapTrackCharge);
  t->Branch("gapTrackIntNum", &gapTrackGeneration);
  t->Branch("gapTrackGenProcessNum",&gapTrackGenProcessNum);
  t->Branch("gapTrackEne",&gapTrackEne);
  t->Branch("gapTrackVolume",&gapTrackVolume);

  //If you want to use geant4's result as garfield simulation input, you can use Garfield* variables.
  t->Branch("GarfieldPdg",&pdgCode) ;
  t->Branch("GarfieldEnergy",&kineticEnergy) ;
  t->Branch("GarfieldPosX",&positionX) ;
  t->Branch("GarfieldPosY",&positionY) ;
  t->Branch("GarfieldPosZ",&positionZ) ;
  t->Branch("GarfieldMomX",&momentumX) ;
  t->Branch("GarfieldMomY",&momentumY) ;
  t->Branch("GarfieldMomZ",&momentumZ) ;

  //gapTrack* variables are only saved when particles get in gasGap.
  //If you need more information of other particles you can use generating* variables.
  t->Branch("GenPart", &generatingPartCode) ;
  t->Branch("GenProcessNum", &generatingProcessNum) ;
  t->Branch("GenEne", &generatingEnergy) ;
  t->Branch("GenVolume", &generatingVolume) ;
  t->Branch("GenIntNum", &generatingIntNum) ;

  //We can use energy depostion for calculating number of ionization.
  t->Branch("edepVolume", &edepVolume);
  t->Branch("edep", &edep);
  t->Branch("edepI", &edepI);
  t->Branch("edepTime", &edepTime);
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

  edepVolume.clear();
  edep.clear();
  edepI.clear();
  edepTime.clear();

  primaryEne=0.;
  zInteraction=0.;

  eleGap = 0;
  posGap = 0;
  chargeGap = 0;
  
  gapTrackPart.clear();
  gapTrackCharge.clear();
  gapTrackGeneration.clear();
  gapTrackGenProcessNum.clear();
  gapTrackVolume.clear();
  gapTrackGenZ.clear();
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
  generatingProcessNum.clear();
  generatingEnergy.clear();
  generatingVolume.clear();
  generatingIntNum.clear();
  genMap.clear();
}

void TrGEMAnalysis::EndOfEvent(const G4Event* /*anEvent*/)
{
  //Accumulate over the run
  thisRunTotEM += thisEventTotEM;

  t->Fill() ; 
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

void TrGEMAnalysis::SetEnergyDeposition(std::string someVolme, G4double someEdep, G4double someEdepI, G4double someTime)
{

  // edepVolume.push_back(someVolme);
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
  std::string genprocess, 
  std::string genvolume, 
  G4double genz, 
  std::string volname,  
  G4double kinene) 
{
  if(genprocess == "primary") return;
  if(gapCharge != 0) chargeGap = 1;
  if(gapPart == 11) eleGap = 1;
  if(gapPart == -11) posGap = 1;
  G4int num = FindVolume(genvolume);
  G4int processNum = FindGapTrackProcess(genprocess);

  gapTrackPart.push_back(gapPart) ;
  gapTrackCharge.push_back(gapCharge) ;
  gapTrackGeneration.push_back(generation) ;
  gapTrackGenProcessNum.push_back(processNum) ;
  gapTrackVolume.push_back(num);
  gapTrackGenZ.push_back(genz) ;
  gapTrackEne.push_back(kinene) ;
}

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
  std::string process, 
  G4double energy, 
  std::string volume, 
  G4int trackID, 
  G4int parentID)
{
  if(process == "primary") return;
  G4int intNum = 0;
  G4int id = trackID;
  genMap[id] = parentID;
  while(genMap[id] != 0)
  {
    intNum++;
    id = genMap[id];
  }
  G4int num = FindVolume(volume);
  G4int processNum = FindGeneratingProcess(process);
  generatingPartCode.push_back(partCode);
  generatingProcessNum.push_back(processNum);
  generatingVolume.push_back(num);
  generatingEnergy.push_back(energy);
  generatingIntNum.push_back(intNum);
}

G4int TrGEMAnalysis::FindVolume(std::string volume)
{
  for(G4int i = 0; i < 21; i++)
  {
    if(volume == (NomeStrati[i]+"Log")) return i;
  }
  return -1;
}

G4int TrGEMAnalysis::FindGapTrackProcess(std::string process)
{
  for(G4int i = 0; i < posProcess.size(); i++)
  {
    if(posProcess[i] == process) return i;
  }
  G4cout << process << G4endl;
  return -1;
}

G4int TrGEMAnalysis::FindGeneratingProcess(std::string process)
{
  for(G4int i = 0; i < posProcess.size(); i++)
  {
    if(posProcess[i] == process) return i;
  }
  G4cout << process << G4endl;
  return -1;
}