#include "TrGEMActionInitialization.hh"
#include "TrGEMPrimaryGeneratorAction.hh"
#include "TrGEMRunAction.hh"
#include "TrGEMEventAction.hh"
#include "TrGEMSteppingAction.hh"
#include "TrGEMStackingAction.hh"
#include "TrGEMTrackingAction.hh"

TrGEMActionInitialization::TrGEMActionInitialization()
: G4VUserActionInitialization()
{
}

TrGEMActionInitialization::~TrGEMActionInitialization()
{
}

//0000000000000000000000000000000000000000000000000000000000000000000000000000

void TrGEMActionInitialization::BuildForMaster() const {
  TrGEMRunAction* runAction = new TrGEMRunAction;
	SetUserAction(runAction);
}

void TrGEMActionInitialization::Build() const {
  SetUserAction(new TrGEMPrimaryGeneratorAction);
	
  TrGEMRunAction* runAction = new TrGEMRunAction;
  SetUserAction(runAction);

  TrGEMEventAction* eventAction = new TrGEMEventAction();
  SetUserAction(eventAction);

  TrGEMSteppingAction* steppingAction = new TrGEMSteppingAction();
  SetUserAction(steppingAction);

  TrGEMStackingAction* stackingAction = new TrGEMStackingAction();
  SetUserAction(stackingAction);
  
  TrGEMTrackingAction* trackingAction = new TrGEMTrackingAction();
  SetUserAction(trackingAction);
}
