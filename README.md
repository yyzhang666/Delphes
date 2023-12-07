# Key4hep environment setup
```
source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh
```

# Event generator: Whizard
To run the fast simulation in Delphes, change the output file format in ".sin" file into "stdhep", then we will get an events file with name ".hep".
# Fast detector simulation: Delphes
Converting the stdhep file to edm4hep and run the Delphes simulation.
```
DelphesSTDHEP_EDM4HEP ${DELPHES_DIR}/cards/delphes_card_IDEA.tcl \
                      ${K4SIMDELPHES}/edm4hep_output_config.tcl \
                      edm4hep_output.root \
                      input_file.stdhep
 ```
Note that there are other excutable commond in Delphes. We don't need do PYTHIA8 since WHIZARD had done that.

DelphesSTDHEP_EDM4HEP - for reading STDHEP inputs
DelphesROOT_EDM4HEP - for reading ROOT files in the Delphes format
DelphesPythia8_EDM4HEP - for running Pythia8 as part of the simulation

Here are example macro files for analysis. Note that these files assumes the data with a different structure.
# Edm4hep analysis
Example file
