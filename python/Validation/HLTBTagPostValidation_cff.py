import FWCore.ParameterSet.Config as cms

from HLTriggerOffline.BTag.Validation.HLTBTagHarvestingAnalyzer_cff import *

PostValMCBTagIP3DFastPV  = bTagPostValidation.clone()
PostValMCBTagIP3DFastPV.HLTPathName    = cms.string('HLT_DiJet40Eta2p6_BTagIP3DFastPV')

HLTBTagPostVal = cms.Sequence(PostValMCBTagIP3DFastPV)

