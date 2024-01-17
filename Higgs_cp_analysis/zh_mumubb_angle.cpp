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

int main(){
    //Read files
    auto reader = podio::ROOTFrameReader();
    reader.openFile("higgs_recoil_mumubb.root");

    const auto e_cms = edm4hep::LorentzVectorE(0, 0, 0, 250.);

    //book histograms
    auto azimuthal= new TH1D("azimuthal_angle","Phi/rad",100, 0.0, 100.0);
    

    //loop over events
    for (size_t i = 0; i < reader.getEntries("events"); ++i) {
               auto event = podio::Frame(reader.readNextEntry("events"));
                   const auto &muons =
        event.get<edm4hep::ReconstructedParticleCollection>("Muons");
			if (muons.size()==2){
                const auto mu1 = edm4hep::utils::p4(muons[0]);
                const auto mu2 = edm4hep::utils::p4(muons[1]);

                //不确定edm4hep里有没有boost这个方法，要先转成root的TLorentz格式

                TLorentzVector Z_boson=mu1+mu2;
                TLorentzVector H_boson=e_cms-Z_boson

                TVector3 boostZ=-Z_boson.BoostVector();
                TVector3 boostH=-H_boson.BoostVector();

                mu1.Boost(boostZ);
                const auto p_mu1=edm4hep::Vector3D(mu1.Px,mu1.Py,mu1.Pz);//再查一下语法

                const auto &jets =event.get<edm4hep::ReconstructedParticleCollection>("Jet");
                if (jets.size()>0){
                    for (const auto &jet:jets){
                        if (jet.BTag&1==1){
                            const auto b=edm4hep::utils::p4(jet)
                            TLorentzVector ;
                            b.Boost(boostH);
                            auto p_b=edm4hep::Vector3D(b.Px,b.Py,b.Pz);

                            angle=edm4hep::utils::angleBetween(p_mu1,p_b);

                            azimuthal->Fill(angle);

                        }
                    }
                        
                }
                else {
                    std::cout<<"No jet in this event!"<<std::endl;
                }
                
        
            }
            
		}
}
//读取文件，创建树，book柱状图，
//遍历事件，筛选出合适的事件
    //!!!btag在edm4hep里没有，必要时请引入delphes lib
//用快度方法，分别换到Z静止参考系，和H静止参考系，得到极角，两个极角相减，得到方位角
    //edm4hep看起来没有直接的方法算角和转换参考系，需要include一些root库，但是有函数可以算极角和eta之间的转换，也可以通过两个动量算方位角
    //所以应该 1.把b和mu分别转换到各自妈妈的参考系中得到动量（写一个函数）
            //2.用edm4hep的方位角方法得到方位角
//cross section先放一下，画方位角的直方图就可以了
//最后得到柱状图
//！！柱状图的写入逻辑还需要再疏通一下
