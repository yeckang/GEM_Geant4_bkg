#ifndef LouvainDetectorConstruction_hh
#define LouvainDetectorConstruction_hh

class G4LogicalVolume ;
class G4PhysicalVolume ;


// Standard includes
#include <list>
#include <string>
#include <cmath>

// Geant4 includes
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4ProductionCuts.hh"
#include "G4Region.hh"
#include "G4Trd.hh"

// //For Electric Field
// #include "G4UniformElectricField.hh"
// #include "G4EqMagElectricField.hh"
// #include "G4FieldManager.hh"
// #include "G4MagIntegratorStepper.hh"
// #include "G4MagIntegratorDriver.hh"

class LouvainDetectorConstruction : public G4VUserDetectorConstruction
{
    public :
        // Constructor
        LouvainDetectorConstruction() ;
        // Destructor
        virtual ~LouvainDetectorConstruction() ;

        virtual void ConstructSDandField();

        void DefineMaterials() ;
        G4VPhysicalVolume* Construct() ;
        G4Trd* Trapezoid(G4String name, G4double width) ;
        void PlaceGeometry(G4RotationMatrix *pRot, G4ThreeVector tlate, G4LogicalVolume *pMotherLogical) ; 

    private :
        G4Material*        fFR4Mat;
        G4Material*        fGasMat;
        G4Material*        fEmptyMat;
        G4Material*        fAirMat;
        G4Material*        fCuMat;
        G4Material*        fKAPTONMat;
        //    G4ProductionCuts*  fGasDetectorCuts;
        G4double           tripleGemThinBase; 
        G4double           tripleGemLargeBase;
        G4double           tripleGemHeight;

        //   std::vector<G4Trd*> 	    TrdStrati;
        //   G4LogicalVolume*		    logicStrati[21];
        std::vector<G4Trd*>           trdCollection ;
        std::vector<G4LogicalVolume*> trdLogCollection ;

        // //For Electric Field
        // G4ElectricField*        fEMfield;
        // G4EqMagElectricField*   fEquation;
        // G4MagIntegratorStepper* fStepper;
        // G4FieldManager*         fFieldMgr;
        // G4double                fMinStep ;
        // G4ChordFinder*          fChordFinder ;
        // G4MagInt_Driver*        fIntgrDriver;

};

#endif

