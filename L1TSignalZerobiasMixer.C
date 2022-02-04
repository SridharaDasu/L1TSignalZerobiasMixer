/*
Simple macro showing how to access branches from the delphes output root file,
loop over events, and plot simple quantities such as the jet pt and the di-electron invariant
mass.

root -l examples/Example1.C'("delphes_output.root")'
*/

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

//------------------------------------------------------------------------------

void L1TSignalZerobiasMixer(const char *inputFile)
{
  gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);

  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  Long64_t numberOfEntries = treeReader->GetEntries();
  std::cout << "numberOfEntries = " << numberOfEntries << std::endl;
  // Get pointers to branches used in this analysis
  TClonesArray *branchTower = treeReader->UseBranch("Tower");
  std::cout << "*branchTower = " << branchTower << std::endl;

  // Loop over all events
  for(Int_t event = 0; event < numberOfEntries; ++event)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(event);
    std::cout << "got event = " << event << std::endl;
    
    Long64_t nTowers = branchTower->GetEntries();
    std::cout << "nTowers = " << nTowers << std::endl;
    // If event contains at least 1 tower
    if(nTowers > 0)
    {

      for(Int_t i = 0; i < nTowers; i++) {
	
	Tower *tower = (Tower*) branchTower->At(i);

	// Print tower
	std::cout << "Tower (pt, eta, phi) = (: " << tower->ET << "," << tower->Eta << "," << tower->Phi << ")" << std::endl;
	
      }

    }
  }
}
