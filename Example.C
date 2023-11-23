/*

#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif
*/
//change to podio/edm4hep

#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/utils/kinematics.h"

#include "podio/Frame.h"
#include "podio/ROOTFrameReader.h"

#include "TFile.h"
#include "TH1D.h"

#include <string>
#include <vector>

void Example1(const char *inputFile)
{
  //gSystem->Load("libDelphes");

  // Create chain of root trees
  //TChain chain("events");
  //chain.Add(inputFile);

  // Create object of class ExRootTreeReader
  //ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  //Long64_t numberOfEntries = treeReader->GetEntries();

  // Get pointers to branches used in this analysis
  //TClonesArray *branchJet = treeReader->UseBranch("Jet");
  //TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  //TClonesArray *branchEvent = treeReader->UseBranch("Event");

  auto reader = podio::ROOTFrameReader();
  reader.openFiles(inputFile);
  //use Reader from podio

  // Book histograms
  TH1 *histJetPT = new TH1F("jet_pt", "jet P_{T}", 100, 0.0, 100.0);
  TH1 *histMass = new TH1F("mass", "M_{inv}(e_{1}, e_{2})", 100, 40.0, 140.0);


  // Loop over all events

  /*for(Int_t entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

    
    //HepMCEvent *event = (HepMCEvent*) branchEvent -> At(0);
    //LHEFEvent *event = (LHEFEvent*) branchEvent -> At(0);
    //Float_t weight = event->Weight;

    // If event contains at least 1 jet
    if(branchJet->GetEntries() > 0)
    {
      // Take first jet
      Jet *jet = (Jet*) branchJet->At(0);

      // Plot jet transverse momentum
      histJetPT->Fill(jet->PT);

      // Print jet transverse momentum
      cout << "Jet pt: "<<jet->PT << endl;
    }

    Electron *elec1, *elec2;

    // If event contains at least 2 electrons
    //if(branchElectron->GetEntries() > 1)
    //{
    //  // Take first two electrons
    //  elec1 = (Electron *) branchElectron->At(0);
    //  elec2 = (Electron *) branchElectron->At(1);

      // Plot their invariant mass
    //  histMass->Fill(((elec1->P4()) + (elec2->P4())).M());
    //}
  }*/
  for (size_t i = 0; i < reader.getEntries("events"); ++i) {
    const auto event = podio::Frame(reader.readNextEntry("events"));

    const auto &jets =
        event.get<edm4hep::ReconstructedParticleCollection>("Jet");
    if(jets.size() > 0)
    {

      // Plot jet transverse momentum
      histJetPT->Fill(jet[0].PT);

      // Print jet transverse momentum
      cout << "Jet pt: "<<jet[0].PT << endl;
    }

    //Electron *elec1, *elec2;

    // If event contains at least 2 electrons
    //if(branchElectron->GetEntries() > 1)
    //{
    //  // Take first two electrons
    //  elec1 = (Electron *) branchElectron->At(0);
    //  elec2 = (Electron *) branchElectron->At(1);

      // Plot their invariant mass
    //  histMass->Fill(((elec1->P4()) + (elec2->P4())).M());
    //}
  }

  // Show resulting histograms
  histJetPT->Draw();
  //histMass->Draw();
}

