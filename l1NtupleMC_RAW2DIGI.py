# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: l1NtupleMC -s RAW2DIGI --era=Run2_2016 --customise=L1Trigger/Configuration/customiseReEmul.L1TReEmulFromRAW --customise=L1Trigger/L1TNtuples/customiseL1Ntuple.L1NtupleEMU --conditions=106X_mcRun2_asymptotic_preVFP_v5 -n 10 --mc --no_exec --no_output --filein=/eos/cms/store/relval/CMSSW_10_6_8/RelValZMM_13UP16/GEN-SIM/106X_mcRun2_asymptotic_v8_UL16hltval_preVFP_v5-v1/10000/1336A7AE-A915-294E-A2C9-75E8AA7AA5DB.root",
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run3_cff import Run3
# from Configuration.Eras.Era_Run2_2018_cff import Run2_2018

process = cms.Process('RAW2DIGI',Run3)
# process = cms.Process('RAW2DIGI',Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load('Configuration.StandardSequences.RecoSim_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring("/store/relval/CMSSW_12_3_0_pre3/RelValZMM_14/GEN-SIM-DIGI-RAW/123X_mcRun3_2021_realistic_v3-v1/10000/2384b0c3-c96f-48c7-b5f3-b29526b1fb69.root",),

)

process.options = cms.untracked.PSet(    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1),
    # numberOfConcurrentRuns = cms.untracked.uint32(1),
    # numberOfStreams = cms.untracked.uint32(0),
    # numberOfThreads = cms.untracked.uint32(1),

)
readFiles = cms.untracked.vstring();
readFiles2 = cms.untracked.vstring();



# process.source = cms.Source("PoolSource", fileNames = readFiles, secondaryFileNames = readFiles2)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

process.TFileService = cms.Service("TFileService", fileName = cms.string("EMTFNtuple.root") )

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('l1NtupleMC nevts:-1'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '123X_mcRun3_2021_realistic_v3', '')

# process.muonGEMDigis.useDBEMap = False


# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.endjob_step = cms.EndPath(process.endOfProcess)

# Schedule definition
# from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
# associatePatAlgosToolsTask(process)

# customisation of the process.
process.options.numberOfThreads=cms.untracked.uint32(1)
process.options.numberOfStreams=cms.untracked.uint32(0)
process.options.numberOfConcurrentLuminosityBlocks=cms.untracked.uint32(1)


process.load('EMTFTools.EMTFNtuple.EMTFNtupleMaker_cfi')

process.EMTFNtuple.useEMTFTracks   = cms.bool(True)

process.L1simulation_step = cms.Path(process.SimL1Emulator)

#calo
del process.simCaloStage2Layer1Digis
del process.simCaloStage2Digis
#other TFs
del process.simDtTriggerPrimitiveDigis
del process.simTwinMuxDigis
del process.simBmtfDigis
del process.simKBmtfStubs
del process.simKBmtfDigis
del process.simOmtfDigis
# GMT/GT/Showers
del process.simGmtCaloSumDigis
del process.simGmtStage2Digis
del process.simEmtfShowers
del process.simGmtShowerDigis
del process.simGtExtFakeStage2Digis
del process.simGtStage2Digis


process.ntuple_step = cms.Path(process.EMTFNtuple)

process.schedule = cms.Schedule(process.raw2digi_step,process.L1simulation_step,process.endjob_step,process.ntuple_step)


# End of customisation functions

# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
