import FWCore.ParameterSet.Config as cms

bTagPostValidation = cms.EDAnalyzer("HLTBTagHarvestingAnalyzer",
   HLTPathName     = cms.string('HLT_DiJet40Eta2p6_BTagIP3DFastPV'),
)
