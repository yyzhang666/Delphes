# Key4hep environment setup
```
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
```
*This environment includes: root, whizard, Delphes, edm4hep, podio...
*This environment not includes: Delphes2LCIO

# Event generator: Whizard
The [WHIZARD manual website](https://whizard.hepforge.org/manual/index.html).
The steering file of Whizard is file with format ".sin", according to the intended events and following detector simulation, we mainly need modify:
*beam
*process
*output format
*model
*...

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

We eventually get a '.root' file.
# Edm4hep analysis
To inspect the root file within the command line, run this command
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
## Analysis macros
Here are example macro files for analysis. Note that these files assumes the data with a different structure.
Example file
