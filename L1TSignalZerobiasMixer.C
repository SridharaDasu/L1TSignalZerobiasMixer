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

// Utility functions

const double Pi = 3.1415927;
const size_t N_REGIONS_ETA = 14;
const size_t N_REGIONS_PHI = 18;
const size_t N_REGIONS = N_REGIONS_ETA * N_REGIONS_PHI;
const size_t N_TOWERS_ETA = N_REGIONS_ETA * 4;
const size_t N_TOWERS_PHI = N_REGIONS_PHI * 4;
const size_t N_TOWERS = N_TOWERS_ETA * N_TOWERS_PHI;

uint32_t getRegionEta(double eta) {
  const double etaBoundaries[N_REGIONS_ETA + 1] =
    {
     -3.000,
     -2.172,
     -1.740,
     -1.392,
     -1.044,
     -0.695,
     -0.348,
     +0.000,
     +0.348,
     +0.695,
     +1.044,
     +1.392,
     +1.740,
     +2.172,
     +3.000
    };						
  for(int rEta = 0; rEta < N_REGIONS_ETA; rEta++) {
    if(eta >= etaBoundaries[rEta] && eta < etaBoundaries[rEta + 1]) {
      return rEta;
    }
  }
  return 999;
}

uint32_t getTowerEta(double eta) {
  const double etaBoundaries[N_TOWERS_ETA + 1] =
    {
     -3.000,
     -2.650,
     -2.500,
     -2.322,
     -2.172,
     -2.043,
     -1.930,
     -1.830,
     -1.740,
     -1.653,
     -1.566,
     -1.479,
     -1.392,
     -1.305,
     -1.218,
     -1.131,
     -1.044,
     -0.957,
     -0.870,
     -0.783,
     -0.695,
     -0.609,
     -0.522,
     -0.435,
     -0.348,
     -0.261,
     -0.174,
     -0.087,
     +0.000,
     +0.087,
     +0.174,
     +0.261,
     +0.348,
     +0.435,
     +0.522,
     +0.609,
     +0.695,
     +0.783,
     +0.870,
     +0.957,
     +1.044,
     +1.131,
     +1.218,
     +1.305,
     +1.392,
     +1.479,
     +1.566,
     +1.653,
     +1.740,
     +1.830,
     +1.930,
     +2.043,
     +2.172,
     +2.322,
     +2.500,
     +2.650,
     +3.000
    };						
  for(int tEta = 0; tEta < N_TOWERS_ETA; tEta++) {
    if(eta >= etaBoundaries[tEta] && eta < etaBoundaries[tEta + 1]) {
      return tEta;
    }
  }
  return 999;
}

uint32_t getRegionPhi(double phi) {
  static bool first = true;
  static double phiBoundaries[N_REGIONS_PHI + 1];
  if(first) {
    first = false;
    double dPhi = 2. * Pi / double(N_REGIONS_PHI);
    phiBoundaries[0] = -Pi;
    for(int rPhi = 1; rPhi <= N_REGIONS_PHI; rPhi++) {
      phiBoundaries[rPhi] = phiBoundaries[rPhi - 1] + dPhi;
    }
  }
  for(int rPhi = 0; rPhi < N_REGIONS_PHI; rPhi++) {
    if(phi >= phiBoundaries[rPhi] && phi < phiBoundaries[rPhi + 1]) {
      return rPhi;
    }
  }
  return 999;
}

uint32_t getTowerPhi(double phi) {
  static bool first = true;
  static double phiBoundaries[N_TOWERS_PHI + 1];
  if(first) {
    first = false;
    double dPhi = 2. * Pi / double(N_TOWERS_PHI);
    phiBoundaries[0] = -Pi;
    for(int tPhi = 1; tPhi <= N_TOWERS_PHI; tPhi++) {
      phiBoundaries[tPhi] = phiBoundaries[tPhi - 1] + dPhi;
    }
  }
  for(int tPhi = 0; tPhi < N_TOWERS_PHI; tPhi++) {
    if(phi >= phiBoundaries[tPhi] && phi < phiBoundaries[tPhi + 1]) {
      return tPhi;
    }
  }
  return 999;
}

//------------------------------------------------------------------------------

void L1TSignalZerobiasMixer(const char *inputFile, const char* zerobiasCSVFile)
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

  std::ifstream zbFile(zerobiasCSVFile, ios::in);
  if(!zbFile.is_open()) {
    std::cerr << "Failed to open zerobias file - quitting" << std::endl;
    return;
  }
  std::ofstream signalFile("L1TSignalZerobiasMixer.csv", ios::out);
  if(!signalFile.is_open()) {
    std::cerr << "Failed to open signal output file - quitting" << std::endl;
    return;
  }
  uint32_t etArray[N_REGIONS_ETA][N_REGIONS_PHI];
  uint32_t locationArray[N_REGIONS_ETA][N_REGIONS_PHI];
  bool eleBitArray[N_REGIONS_ETA][N_REGIONS_PHI];
  bool tauBitArray[N_REGIONS_ETA][N_REGIONS_PHI];
  
  // Loop over all events
  for(Int_t event = 0; event < numberOfEntries; ++event) {
    // Initialize to zero
    for(int ieta = 0; ieta < N_REGIONS_ETA; ieta++) {
      for(int iphi = 0; iphi < N_REGIONS_PHI; iphi++) {
	etArray[ieta][iphi] = 0;
	locationArray[ieta][iphi] = 0;
	eleBitArray[ieta][iphi] = false;
	tauBitArray[ieta][iphi] = false;
      }
    }
    // Add one zerobias event
    int zbRegionEta = -1;
    int zbRegionPhi = -1;
    uint32_t zbET, zbLocation;
    bool zbEleBit, zbTauBit;
    std::string line;
    for(int i = 0; i < N_REGIONS; i++) {
      getline(zbFile, line);
      std::stringstream lineStream(line);
      std::string word;
      getline(lineStream, word, ','); zbRegionEta = atoi(word.c_str());
      getline(lineStream, word, ','); zbRegionPhi = atoi(word.c_str());
      getline(lineStream, word, ','); zbET = atoi(word.c_str());
      getline(lineStream, word, ','); zbLocation = atoi(word.c_str());
      getline(lineStream, word, ','); zbEleBit = atoi(word.c_str());
      getline(lineStream, word, ','); zbTauBit = atoi(word.c_str());
      if (zbRegionEta > -1 && zbRegionEta < N_REGIONS_ETA && zbRegionPhi > -1 && zbRegionPhi < N_REGIONS_PHI) {
	etArray[zbRegionEta][zbRegionPhi] = zbET;
	locationArray[zbRegionEta][zbRegionPhi] = zbLocation;
	eleBitArray[zbRegionEta][zbRegionPhi] = zbEleBit;
	tauBitArray[zbRegionEta][zbRegionPhi] = zbTauBit;
      }
    }
    // Load selected branches with data from specified event
    std::cout << "Getting event = " << event;
    if(!treeReader->ReadEntry(event)) {
      std::cerr << " ... Failed to get event = " << event << std::endl;
      continue;
    }
    else {
      std::cout << " ... got event ... ";
    }
    Long64_t nTowers = branchTower->GetEntries();
    std::cout << "nTowers = " << nTowers;
    // If event contains at least 1 tower
    if(nTowers > 0) {

      double goodET = 0;
      double lostET = 0;
      for(Int_t i = 0; i < nTowers; i++) {
	
	Tower *tower = (Tower*) branchTower->At(i);
	if(tower == 0) {
	  std::cerr << "Bad tower" << std::endl;
	  continue;
	}
	if(tower->Eta < -3.0 || tower->Eta > +3.0 || tower->Phi < -Pi || tower->Phi > +Pi) {
	  lostET += tower->ET;
	  continue;
	}
	goodET += tower->ET;
	
	// Print tower
	
	uint32_t hitTowerEta = getTowerEta(tower->Eta);
	uint32_t hitTowerPhi = getTowerPhi(tower->Phi);
	if(hitTowerEta > N_TOWERS_ETA || hitTowerPhi > N_TOWERS_PHI) {
	  std::cerr << "Crap: " << hitTowerEta << "," << hitTowerPhi << std::endl;
	  std::cerr << "Tower (pt, eta, phi) = (" << tower->ET << "," << tower->Eta << "," << tower->Phi << ")" << std::endl;
	  continue;
	}
	uint32_t hitRegionEta = hitTowerEta / 4;
	uint32_t hitRegionPhi = hitTowerPhi / 4;
	uint32_t hitTowerInRegionEta = hitTowerEta % 4;
	uint32_t hitTowerInRegionPhi = hitTowerPhi % 4;
	uint32_t location = hitTowerInRegionEta | (hitTowerInRegionPhi << 2);
	bool eleBit = false;
	bool tauBit = false;
	if((tower->ET - etArray[hitRegionEta][hitRegionPhi]) > 0.6 * tower->ET) {
	  tauBit = true;
	  if((tower->E - tower->Eem) > 0.6 * tower->E) {
	    eleBit = true;
	  }
	}
	if(hitRegionEta > N_REGIONS_ETA || hitRegionPhi > N_REGIONS_PHI) {
	  std::cerr << "Crap: " << hitRegionEta << "," << hitRegionPhi << std::endl;
	  std::cerr << "Tower (pt, eta, phi) = (" << tower->ET << "," << tower->Eta << "," << tower->Phi << ")" << std::endl;
	  continue;
	}
	if((tower->ET  - etArray[hitRegionEta][hitRegionPhi]) > 0.5 * tower->ET ) {
	  locationArray[hitRegionEta][hitRegionPhi] = location;
	  eleBitArray[hitRegionEta][hitRegionPhi] = eleBit;
	  tauBitArray[hitRegionEta][hitRegionPhi] = tauBit;
	}
	etArray[hitRegionEta][hitRegionPhi] += tower->ET;
      }
      std::cout << "; Signal ET added = " << goodET << "; Out of acceptance ET that is lost = " << lostET;
    }
    for(uint32_t ieta = 0; ieta < N_REGIONS_ETA; ieta++) {
      for(uint32_t iphi = 0; iphi < N_REGIONS_PHI; iphi++) {
	signalFile
	  << ieta
	  << "," << iphi
	  << "," << etArray[ieta][iphi]
	  << "," << locationArray[ieta][iphi]
	  << "," << eleBitArray[ieta][iphi]
	  << "," << tauBitArray[ieta][iphi]
	  << std::endl;
      }
    }
    std::cout << " ... Done " << event << std::endl;
  }
  std::cout << "End of processing" << std::endl;
}
