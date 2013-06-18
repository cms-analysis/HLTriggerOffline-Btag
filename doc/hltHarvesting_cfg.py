import FWCore.ParameterSet.Config as cms

process = cms.Process("HLTHARVEST")

process.load("HLTriggerOffline.Common.HLTValidationHarvest_cff")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load("DQMServices.Components.EDMtoMEConverter_cff")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("")
)
#process.source.duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
process.DQMStore.collateHistograms = False

process.dqmSaver.convention = 'Offline'
#Settings equivalent to 'RelVal' convention:
process.dqmSaver.saveByRun = cms.untracked.int32(-1)
process.dqmSaver.saveAtJobEnd = cms.untracked.bool(True)
process.dqmSaver.forceRunNumber = cms.untracked.int32(1)
#End of 'RelVal convention settings

process.dqmSaver.workflow = ""
process.DQMStore.verbose=3

process.options = cms.untracked.PSet(
    fileMode = cms.untracked.string('FULLMERGE')
)

# Other statements

#Adding DQMFileSaver to the message logger configuration
process.MessageLogger.categories.append('DQMFileSaver')
process.MessageLogger.cout.DQMFileSaver = cms.untracked.PSet(
       limit = cms.untracked.int32(1000000)
       )
process.MessageLogger.cerr.DQMFileSaver = cms.untracked.PSet(
       limit = cms.untracked.int32(1000000)
       )

process.dqmSaver.workflow = "/CMSSW_5_2_7/RelVal/TrigVal"
process.DQMStore.verbose=0
#process.maxEvents.input = 20
process.source.fileNames = cms.untracked.vstring(
# /CMSSW_5_2_7/RelVal/TrigVal
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/8817D14D-3906-E211-B128-0018F3D09700.root",
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/8877CB4D-3906-E211-A77F-0018F3D09700.root",
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/9CBD153B-3906-E211-A63E-002618943810.root",
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/A694FA3D-3906-E211-8AD2-0018F3D0962E.root",
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/B0F7403F-3906-E211-A377-001A928116C4.root",
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/CCA64137-3906-E211-A999-0030486791F2.root",
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/E6FAA136-3906-E211-ADC6-002618943831.root",
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/EE03FA3B-3906-E211-8078-0018F3D095EE.root",
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/F01D2884-4206-E211-8EF5-001A92971B7C.root",
"/store/relval/CMSSW_5_2_7-START52_V10/RelValTTbar/GEN-SIM-DIGI-RAW-HLTDEBUG/v1/00000/F4BCCF33-3906-E211-A9ED-0026189437E8.root"


#'file:step2_RAW2DIGI_RECO_VALIDATION.root'
#'file:MyOutputFile.root'
)


process.load("HLTriggerOffline.Common.HLTValidation_cff")

#extra config needed in standalone
process.load("L1TriggerConfig.L1GtConfigProducers.L1GtConfig_cff")
#process.load("Configuration.StandardSequences.L1TriggerDefaultMenu_cff")
#process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.Geometry.GeometryIdeal_cff")
process.load("HLTriggerOffline.Btag.Validation.hltJetMCTools_cff")
process.validation = cms.Path(
    process.hltvalidation
   # process.HLTMuonVal
   #+process.HLTTauVal
   #+process.EcalPi0Mon
   #+process.EcalPhiSymMon
   #+process.egammaValidationSequence
   #+process.HLTTopVal
   #+process.HLTSusyExoVal
   #+process.HLTFourVector
   #+process.heavyFlavorValidationSequence
    )

process.post_validation = cms.Path(
    process.hltpostvalidation
    )

process.extra_jetmctools  = cms.Path( process.hltJetMCTools )


process.EDMtoMEconv_and_saver= cms.Path(process.EDMtoMEConverter*process.dqmSaver)

process.schedule = cms.Schedule(
	process.extra_jetmctools,
    process.validation,
    process.post_validation,
    process.EDMtoMEconv_and_saver
    )

for filter in (getattr(process,f) for f in process.filters_()):
    if hasattr(filter,"outputFile"):
        filter.outputFile=""


