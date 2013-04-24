#include "HLTriggerOffline/BTag/interface/HLTBTagHarvestingAnalyzer.h"

//
// constructors and destructor
//
HLTBTagHarvestingAnalyzer::HLTBTagHarvestingAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   hltPathName_        = iConfig.getParameter<std::string> ("HLTPathName");

   // DQMStore services   
   dqm = edm::Service<DQMStore>().operator->();
   
}


HLTBTagHarvestingAnalyzer::~HLTBTagHarvestingAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void
HLTBTagHarvestingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
}


// ------------ method called once each job just before starting event loop  ------------
void 
HLTBTagHarvestingAnalyzer::beginJob()
{   
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HLTBTagHarvestingAnalyzer::endJob() 
{
   if ( dqm )
   {
      std::string dqmFolder = Form("HLT/BTag/%s",hltPathName_.c_str());
      dqm->cd(dqmFolder);
   }
}

// ------------ method called when starting to processes a run  ------------
void 
HLTBTagHarvestingAnalyzer::beginRun(edm::Run const&, edm::EventSetup const& )
{
}

// ------------ method called when ending the processing of a run  ------------
void 
HLTBTagHarvestingAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HLTBTagHarvestingAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const & , edm::EventSetup const & )
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HLTBTagHarvestingAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HLTBTagHarvestingAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTBTagHarvestingAnalyzer);
