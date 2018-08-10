#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "FTFP_BERT_HP.hh"
// #include "TROOT.h"
#include "G4EmLivermorePhysics.hh"

#include "LouvainDetectorConstruction.hh"

#include "G4VModularPhysicsList.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "TrGEMPhysicsList.hh"

#include "TrGEMActionInitialization.hh"
#include "TrGEMAnalysis.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"

int main(int argc, char** argv) {

  G4UIExecutive* ui = 0;
  if(argc == 1){
    ui = new G4UIExecutive(argc, argv);
  }

  // Set the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine());
  G4Random::setTheSeed(time(NULL)+38999008.);  
  
  //gROOT->ProcessLine("#include <vector>");

  // construct the default run manager
// #ifdef G4MULTITHREADED
//   G4MTRunManager* runManager = new G4MTRunManager ;
// #else
  G4RunManager* runManager = new G4RunManager;
// #endif
  // set mandatory initialization classes

  runManager->SetUserInitialization(new LouvainDetectorConstruction ) ;
  G4VUserPhysicsList* physics = new FTFP_BERT_HP();

  runManager->SetUserInitialization(physics);

  runManager->SetUserInitialization(new TrGEMActionInitialization());

  // initialize G4 kernel
  runManager->Initialize();
          
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // get the pointer to the UI manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  //UImanager->ApplyCommand("/cuts/setLowEdge 100 eV");
  if (!ui){
    G4String fileName = argv[1];
    TrGEMAnalysis::GetInstance()->SetFileName(fileName);
    G4String command = "/run/beamOn ";
    fileName = argv[2];
    UImanager->ApplyCommand(command+fileName);
  }
  // if (!ui)   // batch mode  
  // {
  //   G4String command = "/control/execute ";
  //   G4String fileName = argv[1];
  //   UImanager->ApplyCommand(command+fileName);
  // }
  else           // interactive mode : define UI session
  {
    UImanager->ApplyCommand("/control/execute vis.mac");     
    ui->SessionStart();
    delete ui;
  }

  // job termination
  delete visManager;
  delete runManager;
  return 0;
}
