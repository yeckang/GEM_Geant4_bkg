#define GeantAnalysis_cxx

#include "GeantAnalysis.h"
#include <iostream>

using namespace std;

void GeantAnalysis::Analysis()
{
  Long64_t nEntries = fChain->GetEntries();
  ResetBranch();
  for(Long64_t i = 0; i < nEntries; i++)
  {
    ResetBranch();
    fChain->GetEntry(i);
    hPrimaryEne->Fill(primaryEne);
    if(EleGap == 1) hEleGap->Fill(primaryEne);
    if(PosGap == 1) hPosGap->Fill(primaryEne);
    if(ChargeGap == 1) hChargeGap->Fill(primaryEne);

    for(UInt_t i = 0 ; i < gapTrackPart->size(); i++)
    {
      if((*gapTrackPart)[i] == 11) 
      {
        hEleEne->Fill((*gapTrackEne)[i]);
        hElectronGenProcess->Fill(primaryEne, (*gapTrackGenProcessNum)[i]);
      }
      if((*gapTrackPart)[i] == -11) hPosEne->Fill((*gapTrackEne)[i]);
      if((*gapTrackCharge)[i] != 0) hChargeEne->Fill((*gapTrackEne)[i]);
      if((*gapTrackIntNum)[i] == 2) hNeutronProcess->Fill(primaryEne,(*gapTrackGenProcessNum)[i]);
    }
  }
}

int main(Int_t argc, Char_t** argv)
{
  if(argc == 1)
  {
    cout << "Error : no arguments\n";
    return 0;
  }
  else
  {
    string temp = argv[1];
    GeantAnalysis analysis(temp);
    for(Int_t i = 2; i < argc; i++)
    { 
      temp = argv[i];
      analysis.SetFile(temp);
      analysis.Analysis();
    }
    return 0;
  }
}