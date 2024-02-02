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

void zh_mumu_angle_delphes_transform(const char *inputFile) {
    gSystem->Load("libDelphes");

    TChain chain("Delphes");
    chain.Add(inputFile);

    ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
    Long64_t numberOfEntries = treeReader->GetEntries();

    TClonesArray *branchMuon = treeReader->UseBranch("Muon");

    TH1F *histAzimuthal = new TH1F("azimuthal_angle_transform", "Phi/rad", 100, -TMath::Pi(), TMath::Pi());

    // Assuming e+e- system energy (e.g., 250 GeV per beam in a center-of-mass system)
    TLorentzVector e_cms(0, 0, 0, 250);

    for(Int_t entry = 0; entry < numberOfEntries; ++entry) {
        treeReader->ReadEntry(entry);

        if(branchMuon->GetEntries() != 2) {
            continue;
        }else{
            Muon *mu1 = (Muon *) branchMuon->At(0);
            Muon *mu2 = (Muon *) branchMuon->At(1);

            TLorentzVector p4_mu1 = mu1->P4();
            TLorentzVector p4_mu2 = mu2->P4();

            // Calculating Z boson four-momentum
            TLorentzVector p4_Z = p4_mu1 + p4_mu2;
            // Calculating H four-momentum
            TLorentzVector p4_H=e_cms-p4_Z;

            // Boost vectors
            TVector3 boostZ = -p4_Z.BoostVector();
            TVector3 boostH = -p4_H.BoostVector();

            // Boost muons to the Z boson rest frame
            p4_mu1.Boost(boostZ);
            // Boost e beam to the H boson rest frame
            e_cms.Boost(boostH);

            // Now calculating angles between the Z boson rest frame and H rest frame
            Double_t phi = p4_mu1.Phi() - e_cms.Phi();

            histAzimuthal->Fill(phi);
        }
    }

    TFile *outputFile = new TFile("azimuthal_angle_histogram_transform.root", "RECREATE");
    histAzimuthal->Write();
    outputFile->Close();

    delete treeReader;
    delete outputFile;
}


