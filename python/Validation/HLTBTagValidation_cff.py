import FWCore.ParameterSet.Config as cms

#from PhysicsTools.JetMCAlgos.AK5CaloJetsMCFlavour_cff import *
from HLTriggerOffline.Btag.Validation.HLTBTagPerformanceAnalyzer_cff import *

ValidationMCBTagIP3DFastPV  = bTagValidation.clone()
ValidationMCBTagIP3DFastPV.HLTPathName    = cms.string('HLT_DiJet40Eta2p6_BTagIP3DFastPV')
#ValidationMCBTagIP3DFastPV.IsData         = cms.bool(False)
ValidationMCBTagIP3DFastPV.BTagAlgorithm  = cms.string('TC')
ValidationMCBTagIP3DFastPV.L25IPTagInfo   = cms.InputTag('NULL')
ValidationMCBTagIP3DFastPV.L25JetTag      = cms.InputTag('NULL')

# Jet Flavour from MC true
#myAK5byRef      = AK5byRef.clone()
#myAK5byRef.jets = cms.InputTag("ak5CaloJets","","RECO")
#myAK5byValAlgo = AK5byValAlgo.clone()
#myAK5byValAlgo.srcByReference = cms.InputTag("myAK5byRef")
#myAK5Flavour=cms.Sequence(myPartons*myAK5byRef*myAK5byValAlgo)

HLTBTagValSeq = cms.Sequence(ValidationMCBTagIP3DFastPV)

