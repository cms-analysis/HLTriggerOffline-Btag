#ifndef HLTBTagHarvestingAnalyzer_H
#define HLTBTagHarvestingAnalyzer_H

// system include files
#include <memory>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

/** \class HLTBTagHarvestingAnalyzer
 *
 *  Top level steering routine for HLT b tag performance analysis.
 *
 */
 
using namespace edm;
 

class HLTBTagHarvestingAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HLTBTagHarvestingAnalyzer(const edm::ParameterSet&);
      ~HLTBTagHarvestingAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
        
      // ----------member data ---------------------------
      std::string hltPathName_;

      DQMStore * dqm;

};


#endif
