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
