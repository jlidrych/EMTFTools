# EMTFTools

Collection of tools useful for EMTF studies.

## Quick start
```bash
cmsrel CMSSW_12_3_0_pre4
cd CMSSW_12_3_0_pre4/src
cmsenv
git cms-init
git cms-addpkg L1Trigger/L1TMuonEndCap
git clone git@github.com:jlidrych/EMTFTools.git
scram b -j 10
```

## Run
```bash
cmsRun l1NtupleMC_RAW2DIGI.py
```
## Ntuple production
Scripts for ntuple production are available in ```bash crab``` directory.
SingleNeutrino sample is used for trigger rate study, while the DYToLL sample is used for the trigger efficiency measurement.
We use so called ```bash GEN-SIM-DIGI-RAW``` production.
```bash
crab submit -c crabSingleNeutrino.py
```
or
```bash
crab submit -c crabDY.py
```
