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

void zh_mumubb_angle(){
    //Read files
    auto reader = podio::ROOTFrameReader();
    reader.openFile("higgs_recoil_mumubb.root");

    const auto e_cms = edm4hep::LorentzVectorE(0, 0, 0, 250.);

    //book histograms
    auto azimuthal= new TH1D("azimuthal_angle_mb","Phi/rad",100, 0.0, 3.2);

    //Define 3d vector
    struct Vector3D {
    double x, y, z;
    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
	};
    

    //loop over events
    for (size_t i = 0; i < reader.getEntries("events"); ++i) {
               auto event = podio::Frame(reader.readNextEntry("events"));
                   const auto &muons =
        event.get<edm4hep::ReconstructedParticleCollection>("Muon");
			if (muons.size()==2){
                const auto mu1 = edm4hep::utils::p4(muons[0]);
                const auto mu2 = edm4hep::utils::p4(muons[1]);

		const auto Z_boson=mu1+mu2;
                const auto H_boson=e_cms-Z_boson;
				
                //Change the format to TLorentzVector for transforming the reference
                TLorentzVector TZ_boson(Z_boson.Px(),Z_boson.Py(),Z_boson.Pz(),Z_boson.E());
		TLorentzVector TH_boson(H_boson.Px(),H_boson.Py(),H_boson.Pz(),H_boson.E())
                TLorentzVector Tmu1(mu1.Px(),mu1.Py(),mu1.Pz(),mu1.E());

                TVector3 boostZ=-TZ_boson.BoostVector();
		TVector3 boostH=-TH_boson.BoostVector();
		//Boost mu to Z rest frame
                Tmu1.Boost(boostZ);

                Vector3D p_mu1(Tmu1.Px(),Tmu1.Py(),Tmu1.Pz());

                const auto &jets =event.get<edm4hep::ReconstructedParticleCollection>("Jet");
                if (jets.size()>0){
			//loop over jets with btag
                    for (const auto &jet:jets){
                        if (jet.BTag&1()==1){
                            const auto b=edm4hep::utils::p4(jet);
                            TLorentzVector Tb(b.Px(),b.Py(),b.Pz(),b.E()); 
			    //boost b-quark to Higgs rest frame
                            Tb.Boost(boostH);
                            Vector3D p_b(Tb.Px(),Tb.Py(),Tb.Pz());

			    //The angle between mu in Z and b in Higgs is the azimuthal angle
                            const auto angle=edm4hep::utils::angleBetween(p_mu1,p_b);

                            azimuthal->Fill(angle);

                        }
                    }
                        
                }
                else {
                    std::cout<<"No jet in this event!"<<std::endl;
                }
                
        
            }
            
		}
	auto hist_file= new TFile("higgs_mb_azimuthal.root","recreate");
	azimuthal->Write();
	hist_file->Close();
}

