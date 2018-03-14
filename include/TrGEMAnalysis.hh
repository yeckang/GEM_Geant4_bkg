#ifndef TRGEMANALYSIS_HH
#define TRGEMANALYSIS_HH 1

#include "globals.hh"
#include <vector>
#include <map>

class G4Run;
class G4Event;
class G4ParticleDefinition;
class TFile;
class TH1D;
// class TNtuple;
class TTree;
class G4Track;

class TrGEMAnalysis {

  public:

    static TrGEMAnalysis* GetInstance();
    ~TrGEMAnalysis();

    void SetFileName(const G4String name);
    void PrepareNewEvent(const G4Event* anEvent);
    void PDGHisto(std::vector<G4double> PDGvector) ;
    void EndOfEvent(const G4Event* anEvent);
    void PrepareNewRun(const G4Run* aRun);
    void EndOfRun(const G4Run* aRun);
    void AddSecondary(const G4ParticleDefinition* part);
    void AddGapSecondary(const G4ParticleDefinition* part, G4int gapNum);
    void AddEDep(G4double edep, G4double z);
    void AddEDepSD(G4double z, G4int copyno);
    void SetBeam(const G4ParticleDefinition* part, G4double energy);
    void AddParticlesPerEvent(G4int PDGCode) ;
    
    void SetEnergyDeposition(std::string someVolume, G4double someEdep, G4double someEdepI, G4double someTime);

    void SavePrimary(G4double primaryene, G4double zinteraction);
    void SaveGapTrack(G4int gapPart, 
                      G4int aCharge,
                      G4int generation,
                      std::string genprocess, 
                      std::string genvolume, 
                      G4double genz, 
                      std::string volname,
                      G4double kinene );
    void SaveGarfieldQuantities(G4int aPdgCode,
                                G4double aKineticEnergy,
                                G4double aPositionX, 
                                G4double aPositionY, 
                                G4double aPositionZ,
                                G4double aMomentumX, 
                                G4double aMomentumY, 
                                G4double aMomentumZ) ;

    void SaveGeneratingTrack(G4int partCode, std::string process, G4double energy, std::string volume, G4int trackID, G4int parentID);

    G4int FindVolume(std::string volume);
    G4int FindGapTrackProcess(std::string process);
    G4int FindGeneratingProcess(std::string process);
    
  private:

    TrGEMAnalysis();
    static TrGEMAnalysis* singleton;

    std::vector<std::string> NomeStrati;
    std::vector<std::string> posProcess;

    bool isNewEvent ;
    G4int eventCounter ;
    G4String fileName;

    // beam and calorimeter geometry
    const G4ParticleDefinition* beamParticle;
    G4double beamEnergy;
    //G4double eCalZposition;

    // simple analysis parameters
    G4double thisEventTotEM;
    G4double thisEventCentralEM;
    G4double thisRunTotEM;
    G4double thisRunTotEM2;
    G4double thisRunCentralEM;
    G4double thisRunCentralEM2;

    // counters
    G4int thisEventSecondaries;
    G4int n_gamma;
    G4int n_electron;
    G4int n_positron;
    G4int n_gapGamma[4];
    G4int n_gapElectron[4];
    G4int n_gapPositron[4];

    //const G4int kMaxTrack ;
    G4int elexevt ;
    G4int posxevt ;
    G4int gammaxevt ;
    G4int secoxevt ;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<G4int> edepVolume;
    std::vector<G4double> edep;
    std::vector<G4double> edepI;
    std::vector<G4double> edepTime;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    G4double primaryEne;
    G4double zInteraction;

    G4int eleGap;
    G4int posGap;
    G4int chargeGap;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<G4int> gapTrackPart ;
    std::vector<G4int> gapTrackCharge ;
    std::vector<G4int> gapTrackGeneration ;
    std::vector<G4int> gapTrackGenProcessNum ;
    std::vector<G4int> gapTrackVolume ;
    std::vector<G4double> gapTrackGenZ ;
    std::vector<G4double> gapTrackEne ;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::vector<G4int> postTrackPart ;
    std::vector<G4double> postTrackEne ;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::vector<G4int>    generatingPartCode;
    std::vector<G4int> generatingProcessNum ;
    std::vector<G4double> generatingEnergy;
    std::vector<G4int> generatingVolume;
    std::vector<G4int> generatingIntNum;

    std::map<G4int, G4int> genMap;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    G4int vecProcNo ;
    G4bool neutronSensitivity ;

    // GARFIELD quantities
    std::vector<G4int>    pdgCode ;

    std::vector<G4double>  kineticEnergy ;

    std::vector<G4double>  positionX ;
    std::vector<G4double>  positionY ;
    std::vector<G4double>  positionZ ;
    
    std::vector<G4double>   momentumX ;
    std::vector<G4double>   momentumY ;
    std::vector<G4double>   momentumZ ;
    
    // std::vector<G4int>  gammaContainer;        s

    // ROOT objects
    TFile*    m_ROOT_file;
    TH1D*     m_ROOT_histo0;
    //TH1D*     m_ROOT_histo1;
    TH1D*     m_ROOT_histo2;
    TH1D*     m_ROOT_histo3;
    //TNtuple*  ntuple;
    TTree     *t ;
};

#endif /* TRGEMANALYSIS_HH */
