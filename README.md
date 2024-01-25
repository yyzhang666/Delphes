# Key4hep environment setup
```
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
```
* This environment includes: root, whizard, Delphes, edm4hep, podio...
* This environment not includes: Delphes2LCIO

The [key4hep tutorial](https://github.com/key4hep/key4hep-tutorials)

# Event generator: Whizard
The [WHIZARD manual website](https://whizard.hepforge.org/manual/index.html).
The steering file of Whizard is file with format ".sin", according to the intended events and following detector simulation, we mainly need modify:
* beam
* process
* output format
* model
* ...

Finishing editing the ".sin" file, run command
```
whizard input_name.sin
```
## The .sin code structure
* model
  ```
  model="model name"
  ```
  where the avaliable "model name" can be found in [10.1.2](https://whizard.hepforge.org/manual/manual011.html#tab%3Amodels).
* process 
# Fast detector simulation: Delphes
To run the fast simulation in Delphes, the output file format in steering file of WHIZARD should be "stdhep", then we will get an events file with name ".hep".

Converting the stdhep file to edm4hep and run the Delphes simulation. For the ILC environment studies, we choose the "ILCgen" card,
```
DelphesSTDHEP_EDM4HEP ${DELPHES_DIR}/cards/delphes_card_ILCgen.tcl \
                      ${K4SIMDELPHES}/edm4hep_output_config.tcl \
                      edm4hep_output.root \
                      input_file.stdhep
 ```
The result data configuration is shown [here](https://github.com/key4hep/k4SimDelphes/blob/main/doc/output_config.md#known-issues).

Note that there are [other excutable commond](https://key4hep.github.io/key4hep-doc/tutorials/k4simdelphes/doc/starterkit/k4SimDelphes/Readme.html) in Delphes. We don't need do PYTHIA8 since WHIZARD had done that.

* `DelphesSTDHEP_EDM4HEP` - for reading STDHEP inputs
* `DelphesROOT_EDM4HEP` - for reading ROOT files in the Delphes format
* `DelphesPythia8_EDM4HEP` - for running Pythia8 as part of the simulation

E.g. command `DelphesSTDHEP_EDM4HEP -h` to know the arguments in it.

We eventually get a '.root' file. The root [TTree description of Delphes](https://cp3.irmp.ucl.ac.be/projects/delphes/wiki/WorkBook/RootTreeDescription)

# Full simulation

# Edm4hep analysis
The namespace and construction of edm4hep is [here](https://edm4hep.web.cern.ch)
To inspect the root file with the edm4hep data within the command line, run this command
```
podio-dump -h
```
To analysis in the root, run 
```
root -l input_edm4hep.root 
```
on the terminal and run 
```
t = new TBrowser
```
in root to see a graphic interface of the TTree.

* Note the old LCIO
## Analysis macros
The C++ macros for edm4hep analysis should include following head files
```
\\The collection of all kinds of reconstructed particles
#include "edm4hep/ReconstructedParticleCollection.h"
\\The functions to accsess the knematic variables of the particles
#include "edm4hep/utils/kinematics.h"
\\Functions and conceptions of the vector operation
#include "edm4hep/utils/vector_utils.h"

\\edm4hep data should be translated by podio
#include "podio/Frame.h"
#include "podio/ROOTFrameReader.h"
```
The name of the kinetic functions and vector operations can be found [here](https://github.com/key4hep/EDM4hep/tree/main/utils/include/edm4hep/utils)


