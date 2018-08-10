#ifndef TRGEMTRACKINGACTION_H
#define TRGEMTRACKINGACTION_H 1

#include "globals.hh"
#include "G4UserTrackingAction.hh"


class G4Track;

class TrGEMTrackingAction : public G4UserTrackingAction 
{
  public:
    // Constructor & Destructor
    TrGEMTrackingAction();
    virtual ~TrGEMTrackingAction();

    virtual void PreUserTrackingAction( const G4Track* aTrack );
    virtual void PostUserTrackingAction( const G4Track* aTrack );
};

#endif