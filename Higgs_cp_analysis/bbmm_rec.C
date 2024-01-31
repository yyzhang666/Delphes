/*

Example reconstruction of   e+e- ->  S Z ->  bb mumu event

root -l 'bbmm_rec.C++("events/bbll_sig_50_eLpR.root",2)'

Second parameter is b-tagging binary flag:
   1 - loose tagging
   2 - medium tagging
   4 - tight tagging

*/

#include "TSystem.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include "TH1F.h"
#include "TClonesArray.h"

#include <iostream>
#include <fstream>    
#include <iomanip>   
using namespace std;


//#ifdef __CLING__
R__LOAD_LIBRARY(delphes/libDelphes)
#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"
//#else
//class ExRootTreeReader;
//class ExRootResult;
//#endif



void bbmm_rec(const char *genFile="events/bbll_sig_50_eLpR.root", int Bbit=2,
	       int nbin=150, double mmin=-20., double mmax=130.)
{
  // (re)book control histrogram

  if( gDirectory->FindObject("hmm")!=NULL) gDirectory->Delete("hmm");
  TH1F *hmm = new TH1F("hmm","Di-muon invariant mass",nbin, mmin, mmax);


  if( gDirectory->FindObject("hbb")!=NULL) gDirectory->Delete("hbb");
  TH1F *hbb = new TH1F("hbb","Di-jet invariant mass",nbin, mmin, mmax);


  if( gDirectory->FindObject("hrec")!=NULL) gDirectory->Delete("hrec");
  TH1F *hrec = new TH1F("hrec","Recoil mass",nbin, mmin, mmax);


    
  gSystem->Load("delphes/libDelphes");
  
  // Create chain of input root trees
  
  TChain chain("Delphes");
  chain.Add(genFile);

  // Create object of class ExRootTreeReader

  ExRootTreeReader *genReader = new ExRootTreeReader(&chain);
  Long64_t genEntries = genReader->GetEntries();

  if(genEntries <=0)
    {
      cout << "No events in input file " << genFile << " ?" << endl;
      return;
    }
  
  cout << "Input chain contains " << genEntries << " events" << endl;


  // Get pointers to branches used in this analysis
  

  TClonesArray *branchJet = genReader->UseBranch("Jet_N2");
  Jet *jet;
  
  TClonesArray *branchElectron = genReader->UseBranch("Electron");
  Electron *electron;

  TClonesArray *branchMuon = genReader->UseBranch("Muon");
  Muon *muon;

  TClonesArray *branchPhoton = genReader->UseBranch("Photon");
  Photon *photon;


  int _nEvt=0;
  
  // Loop over tree events
  
  for(Int_t entry = 0; entry < genEntries; ++entry)
  {
    bool debug =  entry<10 || entry%5000==0;
    
    // Load selected branches with data from specified event
    
    genReader->ReadEntry(entry);

    // Skip events with isolated electrons

    int nElectron = branchElectron->GetEntriesFast();

    if(nElectron > 0 ) continue;

    // Skip events with isolated photons

    int nPhoton = branchPhoton->GetEntriesFast();

    if(nPhoton > 0 ) continue;
    
    // Require two muons

    int nMuon = branchMuon->GetEntriesFast();

    if(nMuon != 2) continue;
    
    TLorentzVector P4Z;
    P4Z.SetPxPyPzE(0.,0.,0.,0.);

    for(Int_t imuon=0; imuon<nMuon; imuon++)
      {
      muon = (Muon*) branchMuon->At(imuon);

      TLorentzVector v = muon->P4() ;
      
      P4Z+=v;
      }
    
    // Require two b-jets


    // Jets
    
    int nJet = branchJet->GetEntriesFast();

    if(nJet != 2) continue;   // Unlikely, as we use Jet_N2 collection
    
    TLorentzVector P4S;
    P4S.SetPxPyPzE(0.,0.,0.,0.);

    int nBtag = 0;
    
    for(Int_t ijet=0; ijet<nJet; ijet++)
      {
      jet = (Jet*) branchJet->At(ijet);

      nBtag += (jet->BTag & Bbit)/Bbit;
      
      TLorentzVector v = jet->P4() ;
      
      P4S+=v;
      }

    if(nBtag != 2) continue;
    
    // Recoil mass
    
    TLorentzVector P4rec;
    P4rec.SetPxPyPzE(0.,0.,0.,250.);
    P4rec -= P4Z;
    
    hmm->Fill(P4Z.M());
    hbb->Fill(P4S.M());
    hrec->Fill(P4rec.M());

   _nEvt++;
  }
  
  // Open canvas

   gStyle->SetPadBottomMargin(0.10);
   gStyle->SetPadTopMargin(0.08);
   gStyle->SetPadRightMargin(0.05);
   gStyle->SetPadLeftMargin(0.15);
   
  
  TCanvas  *ch1 = (TCanvas *) gROOT->FindObject("ch1");
  if(!ch1)  ch1 = new TCanvas("ch1","Delphes histograms",20,20,900,450);

  ch1->Clear();
  ch1->Divide(3,1);
 
  ch1->cd(1);
  hmm->SetLineWidth(2);
  hmm->SetLineColor(4);
  hmm->GetXaxis()->SetTitle("M_{#mu#mu} [GeV]");
  hmm->GetYaxis()->SetTitle("Entries");
  hmm->SetMinimum(0.7);
  hmm->Draw();
  
  ch1->cd(2);
  hbb->SetLineWidth(2);
  hbb->SetLineColor(4);
  hbb->GetXaxis()->SetTitle("M_{jj} [GeV]");
  hbb->GetYaxis()->SetTitle("Entries");
  hbb->SetMinimum(0.7);
  hbb->Draw();
  
  ch1->cd(3);
  hrec->SetLineWidth(2);
  hrec->SetLineColor(4);
  hrec->GetXaxis()->SetTitle("M_{rec} [GeV]");
  hrec->GetYaxis()->SetTitle("Entries");
  hrec->SetMinimum(0.7);
  hrec->Draw();
}
