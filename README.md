# Key4hep environment setup
``` source /cvmfs/sw-nightlies.hsf.org/key4hep/setup.sh ```

# Event generator: Whizard
# Fast detector simulation: Delphes
```DelphesSTDHEP_EDM4HEP delphes_card.tcl \
                      edm4hep_output_config.tcl \
                      edm4hep_output.root \
                      input_file.stdhep```

Here is example macro files for analysis. Note that this files assumes the data with a different structure.
# Edm4hep analysis
Example file
