from WMCore.Configuration import Configuration
config = Configuration()

config.section_("User")
config.User.voGroup = 'dcms'

config.section_("General")
config.General.requestName = 'MC-DY-Sc2'
config.General.workArea = 'MC-DY-Sc2'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName  = 'l1NtupleMC_RAW2DIGI.py'
config.JobType.allowUndistributedCMSSW = True

# send the external libarary of mxnet
config.JobType.sendExternalFolder = True

config.section_("Data")
#config.Data.inputDataset = '/DYToLL_M-50_TuneCP5_14TeV-pythia8/Run3Summer21DRPremix-120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
config.Data.userInputFiles = open('filesDY.txt').readlines()
config.Data.outputPrimaryDataset = 'DYToLL_M-50_TuneCP5_14TeV-pythia8'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.outputDatasetTag = 'MC-DY'

config.section_("Site")
config.Site.storageSite = "T2_DE_DESY"
