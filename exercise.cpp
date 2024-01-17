#include "podio/ROOTFrameReader.h"
//#include "podio/ROOTLegacyReader.h" // For reading legacy files
#include "podio/Frame.h"

#include "edm4hep/MCParticleCollection.h"

int main() {
  auto reader = podio::ROOTFrameReader();
  // auto reader = podio::ROOTLegacyReader(); // For reading legacy files
     reader.openFile("higgs_recoil_from_gaudi_0.edm4hep.root");
     // higgs_recoil_from_gaudi_0.edm4hep.root
         // Loop over all events   
	for (size_t i = 0; i < reader.getEntries("events"); ++i) {
               auto event = podio::Frame(reader.readNextEntry("events"));
                   auto& mcParticles = event.get<edm4hep::MCParticleCollection>("MCParticles");
                       auto muons=edm4hep::ReconstructedParticleCollection();
		       auto recos=edm4hep::ReconstructedParticleCollection();
                       auto p=recos.create()
 		       muons.setSubsetCollection
			for (auto reco:recos){
				muons.push_back(reco[0]);
                           // do more stuff with this event
                             }
  			for (auto muon:muons){
				mass=muon.getMass()
		}
                               return 0;
                              }
}