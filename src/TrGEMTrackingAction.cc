#include "TrGEMTrackingAction.hh"
#include "TrGEMAnalysis.hh"

#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4VProcess.hh"

TrGEMTrackingAction::TrGEMTrackingAction()
{}

TrGEMTrackingAction::~TrGEMTrackingAction()
{}

void TrGEMTrackingAction::PreUserTrackingAction( const G4Track* aTrack )
{
  G4int partCode = aTrack->GetParticleDefinition()->GetPDGEncoding();
  G4String process;
  if(aTrack->GetCreatorProcess()!=0) {
    const G4VProcess * genproc = aTrack->GetCreatorProcess();  
    process = genproc->GetProcessName();
  }
  else  {process="primary";}
  G4double energy = aTrack->GetKineticEnergy();
  G4String volume = aTrack->GetLogicalVolumeAtVertex()->GetName();
  G4int trackID = aTrack->GetTrackID();
  G4int parentID = aTrack->GetParentID();
  
  TrGEMAnalysis::GetInstance()->SaveGeneratingTrack(partCode, process, energy, volume, trackID, parentID);
}

void TrGEMTrackingAction::PostUserTrackingAction( const G4Track* aTrack )
{}