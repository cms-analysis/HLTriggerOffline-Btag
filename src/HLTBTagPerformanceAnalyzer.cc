#include "HLTriggerOffline/Btag/interface/HLTBTagPerformanceAnalyzer.h"
#include "DataFormats/Common/interface/View.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

/// for gen matching 
/// _BEGIN_
#include <Math/GenVector/VectorUtil.h>
#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"

#include <algorithm>

// find the index of the object key of an association vector closest to a given jet, within a given distance
template <typename T, typename V>
//int closestJet(const reco::Jet & jet, const edm::AssociationVector<T, V> & association, double distance) {
int closestJet(const RefToBase<reco::Jet>   jet, const edm::AssociationVector<T, V> & association, double distance) {
  int closest = -1;
  for (unsigned int i = 0; i < association.size(); ++i) {
    double d = ROOT::Math::VectorUtil::DeltaR(jet->momentum(), association[i].first->momentum());
    if (d < distance) {
      distance = d;
      closest  = i;
    }
  }
  return closest;
}


/// 
/// _END_


//
// constructors and destructor
//
HLTBTagPerformanceAnalyzer::HLTBTagPerformanceAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed
   hlTriggerResults_   = iConfig.getParameter<InputTag> ("TriggerResults");
   hltPathName_        = iConfig.getParameter<std::string> ("HLTPathName");

   // trigger   
   l25IPTagInfoCollection_       = iConfig.getParameter<InputTag> ("L25IPTagInfo");
   l3IPTagInfoCollection_        = iConfig.getParameter<InputTag> ("L3IPTagInfo");
   l25JetTagCollection_          = iConfig.getParameter<InputTag> ("L25JetTag");
   l3JetTagCollection_           = iConfig.getParameter<InputTag> ("L3JetTag");
   
   // offline
   trackIPTagInfoCollection_       = iConfig.getParameter<InputTag> ("TrackIPTagInfo");
   offlineJetTagCollection_        = iConfig.getParameter<InputTag> ("OfflineJetTag");
   minJetPT_                       = iConfig.getParameter<double>   ("MinJetPT");
   btagAlgo_                       = iConfig.getParameter<std::string> ("BTagAlgorithm");


  //gen level partons

  edm::ParameterSet mc = iConfig.getParameter<edm::ParameterSet>("mcFlavours");
m_mcPartons =  iConfig.getParameter<edm::InputTag>("mcPartons"); 
  m_mcLabels = mc.getParameterNamesForType<std::vector<unsigned int> >();  
    for (unsigned int i = 0; i < m_mcLabels.size(); ++i)
    m_mcFlavours.push_back( mc.getParameter<std::vector<unsigned int> >(m_mcLabels[i]) );
  
  m_mcMatching = m_mcPartons.label() != "none" ;

   // various parameters
  //   isData_                         = iConfig.getParameter<bool>   ("IsData");
   
   // DQMStore services   
   dqm = edm::Service<DQMStore>().operator->();
   
}


HLTBTagPerformanceAnalyzer::~HLTBTagPerformanceAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void
HLTBTagPerformanceAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<TriggerResults> TriggerResulsHandler;
   iEvent.getByLabel(hlTriggerResults_, TriggerResulsHandler);
   const TriggerResults & triggerResults = *(TriggerResulsHandler.product());
   
   if ( triggerResults.accept(hltPathIndex_) )
   {
   // ONLINE BTAGGING  

      Handle<reco::TrackIPTagInfoCollection> l25IPTagInfoHandler;
      
      if ( l25IPTagInfoCollection_.label() != "" && l25IPTagInfoCollection_.label() != "NULL" )
      {
         // IPTagInfo L25
         iEvent.getByLabel(l25IPTagInfoCollection_, l25IPTagInfoHandler);
         const reco::TrackIPTagInfoCollection & l25IPTagInfos = *(l25IPTagInfoHandler.product());
         // JetTag L25
         Handle<reco::JetTagCollection> l25JetTagHandler;
         iEvent.getByLabel(l25JetTagCollection_, l25JetTagHandler);
         // Btag mapping
         JetTagMap l25JetTag;
         for ( reco::JetTagCollection::const_iterator iter = l25JetTagHandler->begin(); iter != l25JetTagHandler->end(); iter++ )
             l25JetTag.insert(JetTagMap::value_type(iter->first, iter->second));
         for ( size_t j = 0; j != l25IPTagInfos.size(); ++j )
         {  
            const reco::TrackIPTagInfo & l25IPTag = l25IPTagInfos[j];
            // jet
            RefToBase<reco::Jet> jet  = l25IPTag.jet();
            H1_["JetTag_L25"] -> Fill(l25JetTag[jet]);
//			std::cout<<"# "<<j<<" "<<l25JetTag[jet]<<std::endl;
         }
      } else std::cout<<"trobles with  l25IPTagInfoCollection"<<std::endl;

      Handle<reco::TrackIPTagInfoCollection> l3IPTagInfoHandler;
       reco::TrackIPTagInfoCollection l3IPTagInfos;
      JetTagMap l3JetTag;
    
   if ( l3IPTagInfoCollection_.label() != "" && l3IPTagInfoCollection_.label() != "NULL" ) {   
      // IPTagInfo L3
      iEvent.getByLabel(l3IPTagInfoCollection_, l3IPTagInfoHandler);
//      const reco::TrackIPTagInfoCollection & l3IPTagInfos = *(l3IPTagInfoHandler.product());
       l3IPTagInfos = *(l3IPTagInfoHandler.product());
   
      // JetTag L3
      Handle<reco::JetTagCollection> l3JetTagHandler;
      iEvent.getByLabel(l3JetTagCollection_, l3JetTagHandler);
      
      // Btag mapping
//      JetTagMap l3JetTag;
      
      for ( reco::JetTagCollection::const_iterator iter = l3JetTagHandler->begin(); iter != l3JetTagHandler->end(); iter++ )
          l3JetTag.insert(JetTagMap::value_type(iter->first, iter->second));
      
      for ( size_t j = 0; j != l3IPTagInfos.size(); ++j )
      {  
         const reco::TrackIPTagInfo & l3IPTag = l3IPTagInfos[j];
         // jet
         RefToBase<reco::Jet> jet  = l3IPTag.jet();
         H1_["JetTag_L3"] -> Fill(l3JetTag[jet]);
         
      }
} ///if l3IPTagInfoCollection_

 // GEN MATCHING  & FLAVOUR DEPENDENT PLOTS for L25 & L3
  // match to MC parton flavour - accessed on demand
          if (m_mcMatching) {


	  edm::Handle<reco::JetFlavourMatchingCollection> h_mcPartons;
   iEvent.getByLabel(m_mcPartons, h_mcPartons);


    if ( l25IPTagInfoCollection_.label() != "" && l25IPTagInfoCollection_.label() != "NULL" )
    if (l25IPTagInfoHandler.isValid())
      {

		
         const reco::TrackIPTagInfoCollection & l25IPTagInfos = *(l25IPTagInfoHandler.product());
         // JetTag L25
         Handle<reco::JetTagCollection> l25JetTagHandler;
         iEvent.getByLabel(l25JetTagCollection_, l25JetTagHandler);
		for ( reco::JetTagCollection::const_iterator iter = l25JetTagHandler->begin(); iter != l25JetTagHandler->end(); iter++ )
        {
	     edm::RefToBase<reco::Jet> jetBase=     iter->first;
    ///matching L25  to partons       
          // match to MC parton
          unsigned int flavour = 0;
            int m = closestJet(jetBase, *h_mcPartons, m_mcRadius);
            flavour = (m != -1) ? abs((*h_mcPartons)[m].second.getFlavour()) : 0;

            for (unsigned int i = 0; i < m_mcLabels.size(); ++i) {
				 TString flavour_str= m_mcLabels[i].c_str();
                 
				flavours_t flav_collection=  m_mcFlavours[i];
                flavours_t::iterator it = std::find(flav_collection.begin(), flav_collection.end(), flavour);
                if (it!= flav_collection.end()) 	{
						TString label="JetTag_L25_";
						 label+=flavour_str;
					     H1_[label.Data()]->Fill( iter->second);                       
						    label="JetTag_L25_";
						    label+=flavour_str+TString("_disc_pT");
						    H2_[label.Data()]->Fill(iter->second,jetBase->pt());
					} /// if it
                 
			} ///mcLabels

     }//for reco::JetTagCollection L25
    
	} ///l25IPTagInfoCollection


     if ( l3IPTagInfoCollection_.label() != "" && l3IPTagInfoCollection_.label() != "NULL" ) 
	if (l3IPTagInfoHandler.isValid())  
    {
		
         const reco::TrackIPTagInfoCollection & l3IPTagInfos = *(l3IPTagInfoHandler.product());
         // JetTag L3
         Handle<reco::JetTagCollection> l3JetTagHandler;
         iEvent.getByLabel(l3JetTagCollection_, l3JetTagHandler);
		for ( reco::JetTagCollection::const_iterator iter = l3JetTagHandler->begin(); iter != l3JetTagHandler->end(); iter++ )
        {
	     edm::RefToBase<reco::Jet> jetBase=     iter->first;
    ///matching L3  to partons       
          // match to MC parton
          unsigned int flavour = 0;
            int m = closestJet(jetBase, *h_mcPartons, m_mcRadius);
            flavour = (m != -1) ? abs((*h_mcPartons)[m].second.getFlavour()) : 0;

            for (unsigned int i = 0; i < m_mcLabels.size(); ++i) {
				 TString flavour_str= m_mcLabels[i].c_str();
                 
				flavours_t flav_collection=  m_mcFlavours[i];
                flavours_t::iterator it = std::find(flav_collection.begin(), flav_collection.end(), flavour);
                if (it!= flav_collection.end()) 	{
						TString label="JetTag_L3_";
						 label+=flavour_str;
					     H1_[label.Data()]->Fill( iter->second);                       
						    label="JetTag_L3_";
						    label+=flavour_str+TString("_disc_pT");
						    H2_[label.Data()]->Fill(iter->second,jetBase->pt());
					} /// if it

			} /// for m_mcLabels.size

		} //for  l3JetTagHandler

	} /// if l3IPTagInfoCollection_.label

 
        } ///matching


   
   // OFFLINE BTAGGING
      Handle<reco::TrackIPTagInfoCollection> trackIPTagInfoHandler;
      Handle<reco::JetTagCollection> offlineJetTagHandler;

    if ( trackIPTagInfoCollection_.label() != "" && trackIPTagInfoCollection_.label() != "NULL" )
    if ( offlineJetTagCollection_.label() != "" && offlineJetTagCollection_.label() != "NULL" )
	{

      // IPTagInfo
      iEvent.getByLabel(trackIPTagInfoCollection_, trackIPTagInfoHandler);
      const reco::TrackIPTagInfoCollection & trackIPTagInfos = *(trackIPTagInfoHandler.product());
   
      // JetTag Offline
      iEvent.getByLabel(offlineJetTagCollection_, offlineJetTagHandler);
      
      // Btag mapping
      JetTagMap offlineJetTag;
      
      for ( reco::JetTagCollection::const_iterator iter = offlineJetTagHandler->begin(); iter != offlineJetTagHandler->end(); iter++ )
          offlineJetTag.insert(JetTagMap::value_type(iter->first, iter->second));
      
      for ( size_t j = 0; j != trackIPTagInfos.size(); ++j )
      {  
         const reco::TrackIPTagInfo & trackIPTag = trackIPTagInfos[j];
         // jet
         RefToBase<reco::Jet> jet  = trackIPTag.jet();
         
         if ( jet -> pt() > minJetPT_ )
         {
            H1_["JetTag_Offline"] -> Fill(offlineJetTag[jet]);
         }
         
         double drMatch = 9999.;
         float l3BtagMatch = 9999.;
         for ( size_t k = 0; k != l3IPTagInfos.size(); ++k )
         {
            const reco::TrackIPTagInfo & l3IPTag = l3IPTagInfos[k];
            // jet
            RefToBase<reco::Jet> hltjet  = l3IPTag.jet();
            // match offline and online jets
            double dr = reco::deltaR(*(jet),*(hltjet));
            if ( dr < drMatch )
            {
               drMatch = dr;
               l3BtagMatch = l3JetTag[hltjet];
            }
         } ///for L3IPTagInfos

         if ( drMatch < 0.5 and jet -> pt() > 10. )
         {
               H2_["JetTag_OffvsL3"]  -> Fill (offlineJetTag[jet],l3BtagMatch);
         }
         
      } /// for trackIPTagInfos

   // Online primary vertex
      const reco::Vertex & hltVertex = (*(l3IPTagInfos.at(0).primaryVertex().product())).at(0);
      
   // Offline primary vertex
      const reco::Vertex & offVertex = (*(trackIPTagInfos.at(0).primaryVertex().product())).at(0);
      
   // Primary vertex histograms
//      H1_["Vertex_HLT_x"] -> Fill(hltVertex.x());
      H1_["Vertex_HLT_x"] -> Fill(hltVertex.y());
      H1_["Vertex_HLT_y"] -> Fill(hltVertex.y());
      H1_["Vertex_HLT_z"] -> Fill(hltVertex.z());
      H1_["Vertex_Off_x"] -> Fill(offVertex.x());
      H1_["Vertex_Off_y"] -> Fill(offVertex.y());
      H1_["Vertex_Off_z"] -> Fill(offVertex.z());
      
      H2_["Vertex_OffvsHLT_x"] -> Fill(hltVertex.x(),offVertex.x());
      H2_["Vertex_OffvsHLT_y"] -> Fill(hltVertex.y(),offVertex.y());
      H2_["Vertex_OffvsHLT_z"] -> Fill(hltVertex.z(),offVertex.z());
                    
	} /// if trackIPTagInfoCollection
   } /// if trg accept
}


// ------------ method called once each job just before starting event loop  ------------
void 
HLTBTagPerformanceAnalyzer::beginJob()
{
   std::string title;

// ---------------------------------------------   
   // BTag Histograms
   // discriminant range default TC (track counting)

//   float btagL = -110.;
//   float btagU = 50.;
//   int   btagBins = 320;

   float btagL = -10.;
   float btagU = 50.;
   int   btagBins = 300;
   
   if ( btagAlgo_ == "CSV" )
   {
      btagL = -3.;
      btagU = 1.;
      btagBins = 280;
   }
   
   std::string dqmFolder = Form("HLT/BTag/%s",hltPathName_.c_str());
   dqm->setCurrentFolder(dqmFolder);
   // BTag discriminant Histograms
      if ( l25IPTagInfoCollection_.label() != "" && l25IPTagInfoCollection_.label() != "NULL" )
   {
      H1_["JetTag_L25"]   = dqm->book1D("JetTag_L25",     l25JetTagCollection_.label().c_str(), btagBins, btagL, btagU );
      H1_["JetTag_L25"] -> setAxisTitle("L25 discriminant",1);
   }
   H1_["JetTag_L3"]       = dqm->book1D("JetTag_L3",      l3JetTagCollection_.label().c_str(),  btagBins, btagL, btagU );
   H1_["JetTag_Offline"]  = dqm->book1D("JetTag_Offline", offlineJetTagCollection_.label().c_str(), btagBins, btagL, btagU );
   title = Form("%s versus %s", offlineJetTagCollection_.label().c_str(),l3JetTagCollection_.label().c_str());
   H2_["JetTag_OffvsL3"]  = dqm->book2D("JetTag_OffvsL3", title.c_str(), btagBins, btagL, btagU, btagBins, btagL, btagU );
      // axis titles
   H1_["JetTag_L3"]      -> setAxisTitle("L3 discriminant",1);
   H1_["JetTag_Offline"] -> setAxisTitle("Offline discriminant",1);
   H2_["JetTag_OffvsL3"] -> setAxisTitle("Offline discriminant",1);
   H2_["JetTag_OffvsL3"] -> setAxisTitle("L3 discriminant",2);
   
   
// ---------------------------------------------   
   // Vertex Histograms
   // Vertex position ranges for MC
   float vtxXL = 0.15;
   float vtxXU = 0.35;
    
//   float vtxXL = 0.386;
//   float vtxXU = 0.400;
    float vtxYL = 0.386;
   float vtxYU = 0.400;
   float vtxZL = -20.0;
   float vtxZU =  20.0;
   
/*
   if ( isData_ )
   {
      vtxXL = 0.062;
      vtxXU = 0.076;
      vtxYL = 0.057;
      vtxYU = 0.071;
      vtxZL = -20.0;
      vtxZU =  20.0;
   }
  */ 
   
   H1_["Vertex_HLT_x"]      = dqm->book1D( "Vertex_HLT_x", "HLT vertex x position", 280, vtxXL, vtxXU );
   H1_["Vertex_HLT_y"]      = dqm->book1D( "Vertex_HLT_y", "HLT vertex y position", 280, vtxYL, vtxYU );
   H1_["Vertex_HLT_z"]      = dqm->book1D( "Vertex_HLT_z", "HLT vertex z position", 400, vtxZL, vtxZU );
   H1_["Vertex_Off_x"]      = dqm->book1D( "Vertex_Off_x", "Offline vertex x position", 280, vtxXL, vtxXU );
   H1_["Vertex_Off_y"]      = dqm->book1D( "Vertex_Off_y", "Offline vertex y position", 280, vtxYL, vtxYU );
   H1_["Vertex_Off_z"]      = dqm->book1D( "Vertex_Off_z", "Offline vertex z position", 400, vtxZL, vtxZU );
   H2_["Vertex_OffvsHLT_x"] = dqm->book2D( "Vertex_OffvsHLT_x", "Offline vs HLT vertex x position", 280, vtxXL, vtxXU, 280, vtxXL, vtxXU );
   H2_["Vertex_OffvsHLT_y"] = dqm->book2D( "Vertex_OffvsHLT_y", "Offline vs HLT vertex y position", 280, vtxYL, vtxYU, 280, vtxYL, vtxYU );
   H2_["Vertex_OffvsHLT_z"] = dqm->book2D( "Vertex_OffvsHLT_z", "Offline vs HLT vertex z position", 400, vtxZL, vtxZU, 400, vtxZL, vtxZU );
      // axis titles
   H1_["Vertex_HLT_x"]      -> setAxisTitle("HLT vtx x(cm)",1);
   H1_["Vertex_HLT_y"]      -> setAxisTitle("HLT vtx y(cm)",1);
   H1_["Vertex_HLT_z"]      -> setAxisTitle("HLT vtx z(cm)",1);
   H1_["Vertex_Off_x"]      -> setAxisTitle("Offline vtx x(cm)",1);
   H1_["Vertex_Off_y"]      -> setAxisTitle("Offline vtx y(cm)",1);
   H1_["Vertex_Off_z"]      -> setAxisTitle("Offline vtx z(cm)",1);
   H2_["Vertex_OffvsHLT_x"] -> setAxisTitle("HLT vtx x(cm)",1);
   H2_["Vertex_OffvsHLT_y"] -> setAxisTitle("HLT vtx y(cm)",1);
   H2_["Vertex_OffvsHLT_z"] -> setAxisTitle("HLT vtx z(cm)",1);
   H2_["Vertex_OffvsHLT_x"] -> setAxisTitle("Offline vtx x(cm)",2);
   H2_["Vertex_OffvsHLT_y"] -> setAxisTitle("Offline vtx y(cm)",2);
   H2_["Vertex_OffvsHLT_z"] -> setAxisTitle("Offline vtx z(cm)",2);


/// mc related plots

  for (unsigned int i = 0; i < m_mcLabels.size(); ++i)
{

/// 1D for L25 discriminator

     TString label="JetTag_L25_";
     TString flavour= m_mcLabels[i].c_str();
     label+=flavour;
	 H1_[label.Data()] = 		 dqm->book1D(label.Data(),   Form("%s_%s",flavour.Data(),l25JetTagCollection_.label().c_str()), btagBins, btagL, btagU );
     H1_[label.Data()]->setAxisTitle("disc",1);

/// 1D for L3 discriminator

     label="JetTag_L3_";
     label+=flavour;
	 H1_[label.Data()] = 		 dqm->book1D(label.Data(),   Form("%s_%s",flavour.Data(),l3JetTagCollection_.label().c_str()), btagBins, btagL, btagU );
     H1_[label.Data()]->setAxisTitle("disc",1);


/// 2D for efficiency calculations
 
///Pt turn-on
    int nBinsPt=30;
    double pTmin=0;
    double pTMax=300;

    label="JetTag_L25_";
    label+=flavour+TString("_disc_pT");
    H2_[label.Data()] =  dqm->book2D( label.Data(), label.Data(), btagBins, btagL, btagU, nBinsPt, pTmin, pTMax );
    H2_[label.Data()]->setAxisTitle("pT",2);
    H2_[label.Data()]->setAxisTitle("disc",1);

    label="JetTag_L3_";
    label+=flavour+TString("_disc_pT");
    H2_[label.Data()] =  dqm->book2D( label.Data(), label.Data(), btagBins, btagL, btagU, nBinsPt, pTmin, pTMax );
    H2_[label.Data()]->setAxisTitle("pT",2);
    H2_[label.Data()]->setAxisTitle("disc",1);

}

   
   triggerConfChanged_ = false;
   
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HLTBTagPerformanceAnalyzer::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
void 
HLTBTagPerformanceAnalyzer::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup)
{
   triggerConfChanged_ = true;
   hltConfigProvider_.init(iRun, iSetup, hlTriggerResults_.process(), triggerConfChanged_);

   hltPathIndex_ = 99999;
   const std::vector< std::string > & hltPathNames = hltConfigProvider_.triggerNames();
   for ( size_t it = 0 ; it < hltPathNames.size() ; ++it )
   {
      unsigned int found = hltPathNames.at(it).find(hltPathName_);
      if ( found == 0 )
      {
         hltPathIndex_ = it;
         break;
      }
         
   }
   if ( hltPathIndex_ == 99999 ) std::cout << "Path " << hltPathName_ << " does not exist" << std::endl;
}

// ------------ method called when ending the processing of a run  ------------
void 
HLTBTagPerformanceAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
HLTBTagPerformanceAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const & , edm::EventSetup const & )
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
HLTBTagPerformanceAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
HLTBTagPerformanceAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(HLTBTagPerformanceAnalyzer);
