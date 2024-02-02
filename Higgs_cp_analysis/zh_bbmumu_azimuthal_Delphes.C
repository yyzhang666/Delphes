#ifdef __CLING__
R__LOAD_LIBRARY(libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#endif

#include "TH1F.h"
#include "TSystem.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TMath.h"

void zh_bbmumu_azimuthal_Delphes(const char *inputFile, int Bbit) {
    gSystem->Load("libDelphes");

    TChain chain("Delphes");
    chain.Add(inputFile);

    ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
    Long64_t numberOfEntries = treeReader->GetEntries();

    TClonesArray *branchMuon = treeReader->UseBranch("Muon");
    TClonesArray *branchJet = treeReader->UseBranch("Jet_N2");

    TH1F *histAzimuthal = new TH1F("azimuthal_angle_transform", "Phi/rad", 100, -TMath::Pi(), TMath::Pi());

    // Assuming e+e- system energy (e.g., 250 GeV per beam in a center-of-mass system)
    TLorentzVector e_cms(0, 0, 0, 250);

    for(Int_t entry = 0; entry < numberOfEntries; ++entry) {
        treeReader->ReadEntry(entry);

        if(branchMuon->GetEntries() != 2) {
            continue;
        }else{
          if(branchJet->GetEntries()!=2) continue;
            Jet *jet1 = (Jet *) branchJet->At(0);
            Jet *jet2 = (Jet *) branchJet->At(1);

            int nBtag;  
            nBtag=(jet1->BTag & Bbit)/Bbit+(jet2->BTag & Bbit)/Bbit;
            if(nBtag != 2) continue;

            TLorentzVector p4_jet1 = jet1->P4();
            TLorentzVector p4_jet2 = jet2->P4();
          
            Muon *mu1 = (Muon *) branchMuon->At(0);
            Muon *mu2 = (Muon *) branchMuon->At(1);

            TLorentzVector p4_mu1 = mu1->P4();
            TLorentzVector p4_mu2 = mu2->P4();

            // Calculating Z boson four-momentum
            TLorentzVector p4_Z = p4_mu1 + p4_mu2;
            // Calculating H four-momentum
            TLorentzVector p4_H=p4_jet1 + p4_jet2;

            // Boost vectors
            TVector3 boostZ = -p4_Z.BoostVector();
            TVector3 boostH = -p4_H.BoostVector();

            // Boost muons to the Z boson rest frame
            p4_mu1.Boost(boostZ);
            // Boost e beam to the H boson rest frame
            p4_jet1.Boost(boostH);

            // Now calculating angles between the Z boson rest frame and H rest frame
            Double_t phi = p4_mu1.Phi() - p4_jet1.Phi();

            histAzimuthal->Fill(phi);
        }
    }

    TFile *outputFile = new TFile("bbmumu_azimuthal_angle.root", "RECREATE");
    histAzimuthal->Write();
    outputFile->Close();

    delete treeReader;
    delete outputFile;
}
