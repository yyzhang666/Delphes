//include the packages to read the data with edm4hep format
#include "edm4hep/ReconstructedParticleCollection.h"
#include "edm4hep/utils/kinematics.h"
#include "edm4hep/utils/vector_utils.h"

#include "podio/Frame.h"
#include "podio/ROOTFrameReader.h"

//include Delphes package to get information of btag
#include "classes/DelphesClasses.h"

#include "TFile.h"
#include "TH1D.h"
#include "TLorentzVector.h"
#include "TVector3.h"

#include <string>
#include <vector>
#include<iostream>

void zh_mumu_angle(){
    //Read files
    auto reader = podio::ROOTFrameReader();
    reader.openFile("delphes_events_edm4hep.root");

    const auto e_cms = edm4hep::LorentzVectorE(0, 0, 0, 250.);

    //book histograms
    auto azimuthal= new TH1D("azimuthal_angle","Phi/rad",100, 0.0, 10.0);

    //Define 3d vector
    struct Vector3D {
    double x, y, z;
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
	};


    //loop over events
    for (size_t i = 0; i < reader.getEntries("events"); ++i) {
               auto event = podio::Frame(reader.readNextEntry("events"));
                   const auto &muons =
        event.get<edm4hep::ReconstructedParticleCollection>("Muons");
			if (muons.size()==2){
                const auto mu1 = edm4hep::utils::p4(muons[0]);
                const auto mu2 = edm4hep::utils::p4(muons[1]);

                const auto Z_boson=mu1+mu2;
                const auto H_boson=e_cms-Z_boson;

                //By definition edm4hep::utils::p4 is ROOT::Math::PxPyPzEVector/ROOT::Math::PxPyPzEVector, 
                //so it can be acted on the same method.

                //Change the format to TLorentzVector for transforming the reference
                TLorentzVector TZ_boson(Z_boson.Px(),Z_boson.Py(),Z_boson.Pz(),Z_boson.E());
                TLorentzVector Tmu1(mu1.Px(),mu1.Py(),mu1.Pz(),mu1.E());

                TVector3 boostZ=-TZ_boson.BoostVector();
                Tmu1.Boost(boostZ);

                Vector3D p_mu1(Tmu1.Px(),Tmu1.Py(),Tmu1.Pz());

                const auto angle=edm4hep::utils::anglePolar(p_mu1);
                azimuthal->Fill(angle);

                        }
                else {
                    std::cout<<"No muons in this event!"<<std::endl;
                }
    }
	auto hist_file = new TFile("higgs_mu_azimuthal.root", "recreate");
  azimuthal->Write();
  hist_file->Close();
}               
        
