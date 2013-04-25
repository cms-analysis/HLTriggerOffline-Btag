#ifndef HLTBTagPerformanceAnalyzer_H
#define HLTBTagPerformanceAnalyzer_H

// system include files
#include <memory>
#include <string>
#include <vector>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"

// Trigger
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

//Vertex
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "DataFormats/Math/interface/deltaR.h"

#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"

/** \class HLTBTagPerformanceAnalyzer
 *
 *  Top level steering routine for HLT b tag performance analysis.
 *
 */
 
using namespace reco;
using namespace edm;
 

class HLTBTagPerformanceAnalyzer : public edm::EDAnalyzer {
   public:
      explicit HLTBTagPerformanceAnalyzer(const edm::ParameterSet&);
      ~HLTBTagPerformanceAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
        
      struct JetRefCompare :
         public std::binary_function<edm::RefToBase<reco::Jet>, edm::RefToBase<reco::Jet>, bool> {
         inline bool operator () (const edm::RefToBase<reco::Jet> &j1,
                     const edm::RefToBase<reco::Jet> &j2) const
         { return j1.id() < j2.id() || (j1.id() == j2.id() && j1.key() < j2.key()); }
      };
      
      typedef std::map<edm::RefToBase<reco::Jet>, float, JetRefCompare> JetTagMap;
      
      // ----------member data ---------------------------
      InputTag hlTriggerResults_;
      std::string hltPathName_;
      HLTConfigProvider hltConfigProvider_;
      bool triggerConfChanged_;

      
      InputTag l25IPTagInfoCollection_;
      InputTag l3IPTagInfoCollection_;
      InputTag l25JetTagCollection_;
      InputTag l3JetTagCollection_;
      InputTag trackIPTagInfoCollection_;
      InputTag offlineJetTagCollection_;
      
      double minJetPT_;
      unsigned int hltPathIndex_;
//      bool isData_;
      std::string btagAlgo_;

      DQMStore * dqm;

      // Histogram handler
      std::map<std::string, MonitorElement *> H1_;
      std::map<std::string, MonitorElement *> H2_;

};


#endif
