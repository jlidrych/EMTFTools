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
## Setup - configuration file
The configuration file is available in ``` EMTFNtuple/python/EMTFNtupleMaker_cfi.py ```.
One can change defaults to enable/disable processing of various inputs (CSC/RPC/CPPF/GEM etc.),(EMTFHits/EMTFUnpackedHits), (EMTFTracks/EMTFUnpackedTracks) etc.

## Ntuple production
Scripts for ntuple production are available in ``` crab``` directory.
SingleNeutrino sample is used for trigger rate study, while the DYToLL sample is used for the trigger efficiency measurement.
We use so called ``` GEN-SIM-DIGI-RAW``` production.
```bash
crab submit -c crabSingleNeutrino.py
```
or
```bash
crab submit -c crabDY.py
```
