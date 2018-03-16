#include "HistoMaker.h"
#include <iostream>

using namespace std;

int HistoDrawer()
{
  HistoMaker histDrawer("test.root");
  auto c1 = new TCanvas("c1","c1",600,500);
  // TH1D* h = histDrawer.SensitivityCharge(100);
  TH1D* h = histDrawer.PrimaryEnergy(100);
  h->Draw("E1 same");
  // h->Draw("L hist same");

  // TH2D* h = histDrawer.ElectronProcess(100);
  // TH2D* h = histDrawer.NeutronProcess(100);
  // h->Draw("colz");
  
  c1->SetLogx();
  c1->SetLogy();
  c1->SetGrid();

  c1->Modified();
  c1->Update();
  return 0;
}