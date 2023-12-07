# Key4hep environment setup
```
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
```

# Event generator: Whizard
The [WHIZARD manual website](https://whizard.hepforge.org/manual/index.html).
To run the fast simulation in Delphes, change the output file format in ".sin" file into "stdhep", then we will get an events file with name ".hep".
## The .sin code structure
* model
  ```
  model="model name"
  ```
  where the avaliable "model name" can be found in [10.1.2](https://whizard.hepforge.org/manual/manual011.html#tab%3Amodels).
* process 
# Fast detector simulation: Delphes
Converting the stdhep file to edm4hep and run the Delphes simulation.
```
DelphesSTDHEP_EDM4HEP ${DELPHES_DIR}/cards/delphes_card_IDEA.tcl \
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

# Edm4hep analysis
After getting the '.root' file from the last step, run this command
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
