from WMCore.Configuration import Configuration
config = Configuration()

config.section_("User")
config.User.voGroup = 'dcms'

config.section_("General")
config.General.requestName = 'MC-SingleNeutrino-Sc2'
config.General.workArea = 'MC-SingleNeutrino-Sc2'

config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName  = 'l1NtupleMC_RAW2DIGI.py'
config.JobType.allowUndistributedCMSSW = True

# send the external libarary of mxnet
config.JobType.sendExternalFolder = True

config.section_("Data")
config.Data.inputDataset = '/SingleNeutrino_E-10-gun/Run3Winter22DR-L1TPU0to99FEVT_SNB_122X_mcRun3_2021_realistic_v9-v2/GEN-SIM-DIGI-RAW'
# /SingleNeutrino_Pt-2To20-gun/Run3Summer21DRPremix-SNB_120X_mcRun3_2021_realistic_v6-v2/GEN-SIM-DIGI-RAW'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 18
config.Data.outputDatasetTag = 'MC-SingleNeutrino'

config.section_("Site")
config.Site.storageSite = "T2_DE_DESY"
