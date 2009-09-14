#include <map>
#include <string>
#include <cmath>
#include <Math/Functions.h>
#include <Math/SVector.h>
#include <Math/SMatrix.h>
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "SimTracker/TrackHistory/interface/VertexClassifierByProxy.h"
#include "CommonTools/Statistics/interface/ChiSquaredProbability.h"
#include "DataFormats/Math/interface/Vector.h"
#include "TROOT.h"
#include "Math/VectorUtil.h"
#include <TVector3.h>
#include <Math/GenVector/PxPyPzE4D.h>
#include <Math/GenVector/PxPyPzM4D.h>
#include "DataFormats/Math/interface/LorentzVector.h"
//
// class decleration
//
using namespace reco;
using namespace std;
using namespace edm;

class recoBSVTagInfoValidationAnalyzer : public edm::EDAnalyzer
{

public:

    explicit recoBSVTagInfoValidationAnalyzer(const edm::ParameterSet&);

private:

    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    // Member data

    VertexClassifierByProxy<reco::SecondaryVertexTagInfoCollection> classifier_;

    Int_t numberVertexClassifier_;
    edm::InputTag trackingTruth_;
    edm::InputTag svTagInfoProducer_;

    DQMStore * dqmStore_;
    std::string dqmLabel;

    Int_t n_event;
    Int_t rs_total_nall ; 
    Int_t rs_total_nsv ;
    Int_t rs_total_nbv ;
    Int_t rs_total_nbsv ;
    Int_t rs_total_ncv ;
    Int_t rs_total_nlv ;
    Int_t total_nfake ;

    Int_t sr_total_nall ; 
    Int_t sr_total_nsv ;
    Int_t sr_total_nbv ;
    Int_t sr_total_nbsv ;
    Int_t sr_total_ncv ;
    Int_t sr_total_nlv ;
    Int_t total_nmiss ;
        

  // Bookeeping of all the histograms per category
  void bookRecoToSim(std::string const &);
    void bookSimToReco(std::string const &);

    // Fill all histogram per category
    void fillRecoToSim(std::string const &, reco::Vertex const &, TrackingVertexRef const &);
    void fillSimToReco(std::string const &, reco::VertexRef const &, TrackingVertexRef const &);

    // Histogram handlers
    std::map<std::string, MonitorElement *> HistIndex_;

};


recoBSVTagInfoValidationAnalyzer::recoBSVTagInfoValidationAnalyzer(const edm::ParameterSet& config) : classifier_(config)
{
  //Initialize counters
    n_event = 0;
    rs_total_nall = 0; 
    rs_total_nsv = 0;
    rs_total_nbv = 0;
    rs_total_nbsv = 0;
    rs_total_ncv = 0;
    rs_total_nlv = 0;
    total_nfake = 0;

    sr_total_nall = 0; 
    sr_total_nsv = 0;
    sr_total_nbv = 0;
    sr_total_nbsv = 0;
    sr_total_ncv = 0;
    sr_total_nlv = 0;
    total_nmiss = 0;

      //  get the store
  dqmStore_ = edm::Service<DQMStore>().operator->();
  dqmLabel = "SVValidation/";
  dqmStore_->setCurrentFolder(dqmLabel);


    // Get the track collection
    svTagInfoProducer_ = config.getUntrackedParameter<edm::InputTag> ( "svTagInfoProducer" );

    // Name of the traking pariticle collection
    trackingTruth_ = config.getUntrackedParameter<edm::InputTag> ( "trackingTruth" );

    // Number of track categories
    numberVertexClassifier_ = VertexCategories::Unknown+1;

    // Define histogram for counting categories
    HistIndex_["VertexClassifier"] = dqmStore_->book1D(
                                        "VertexClassifier",
                                        "Frequency for the different track categories",
                                        numberVertexClassifier_,
                                        -0.5,
                                        numberVertexClassifier_ - 0.5
                                    );
    
    //--- RecoToSim
    HistIndex_["rs_All_FlightDistance2d"]= dqmStore_->book1D( "rs_All_FlightDistance2d", "Transverse flight distance [cm]", 100, 0, 5 );
    HistIndex_["rs_SecondaryVertex_FlightDistance2d"]= dqmStore_->book1D( "rs_SecondaryVertex_FlightDistance2d", "Transverse flight distance [cm]", 100, 0, 5 );
    HistIndex_["rs_BSV_FlightDistance2d"]= dqmStore_->book1D( "rs_BSV_FlightDistance2d", "Transverse flight distance [cm]", 100, 0, 5 );
    HistIndex_["rs_BWeakDecay_FlightDistance2d"]= dqmStore_->book1D( "rs_BWeakDecay_FlightDistance2d", "Transverse flight distance [cm]", 100, 0, 5 );
    HistIndex_["rs_CWeakDecay_FlightDistance2d"]= dqmStore_->book1D( "rs_CWeakDecay_FlightDistance2d", "Transverse flight distance [cm]", 100, 0, 5 );
    HistIndex_["rs_Light_FlightDistance2d"]= dqmStore_->book1D( "rs_Light_FlightDistance2d", "Transverse flight distance [cm]", 100, 0, 5 );

    HistIndex_["rs_All_nRecVtx"]= dqmStore_->book1D( "rs_All_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["rs_SecondaryVertex_nRecVtx"]= dqmStore_->book1D( "rs_SecondaryVertex_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["rs_BSV_nRecVtx"]= dqmStore_->book1D( "rs_BSV_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["rs_BWeakDecay_nRecVtx"]= dqmStore_->book1D( "rs_BWeakDecay_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["rs_CWeakDecay_nRecVtx"]= dqmStore_->book1D( "rs_CWeakDecay_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["rs_Light_nRecVtx"]= dqmStore_->book1D( "rs_Light_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );


    //--- SimToReco
    HistIndex_["sr_All_nRecVtx"]= dqmStore_->book1D( "sr_All_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["sr_SecondaryVertex_nRecVtx"]= dqmStore_->book1D( "sr_SecondaryVertex_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["sr_BSV_nRecVtx"]= dqmStore_->book1D( "sr_BSV_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["sr_BWeakDecay_nRecVtx"]= dqmStore_->book1D( "sr_BWeakDecay_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["sr_CWeakDecay_nRecVtx"]= dqmStore_->book1D( "sr_CWeakDecay_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );
    HistIndex_["sr_Light_nRecVtx"]= dqmStore_->book1D( "sr_Light_nRecVtx", "Number of Vertices per event", 11, -0.5, 10.5 );  


    // Set the proper categories names
    for (Int_t i = 0; i < numberVertexClassifier_; ++i)
      HistIndex_["VertexClassifier"]->getTH1F()->GetXaxis()->SetBinLabel(i+1, VertexCategories::Names[i]);
    
    // book histograms
    bookRecoToSim("rs_All");
    bookRecoToSim("rs_SecondaryVertex");
    bookRecoToSim("rs_BSV");
    bookRecoToSim("rs_BWeakDecay");
    bookRecoToSim("rs_CWeakDecay");
    bookRecoToSim("rs_Light");

    bookSimToReco("sr_All"); 
    bookSimToReco("sr_SecondaryVertex");
    bookSimToReco("sr_BSV");  
    bookSimToReco("sr_BWeakDecay");  
    bookSimToReco("sr_CWeakDecay");  
    bookSimToReco("sr_Light");  
}   


void recoBSVTagInfoValidationAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup)
{
  ++n_event;

  std::cout << "*** Analyzing " << event.id() << " n_event = " << n_event << std::endl << std::endl;

  // Set the classifier for a new event
  classifier_.newEvent(event, setup);
  

  // Vertex collection
  edm::Handle<reco::SecondaryVertexTagInfoCollection> svTagInfoCollection;
  event.getByLabel(svTagInfoProducer_, svTagInfoCollection);

  // Get a constant reference to the track history associated to the classifier
  VertexHistory const & tracer = classifier_.history();

  cout << "* Event " << n_event << " ; svTagInfoCollection->size() = " << svTagInfoCollection->size() << endl; 
  
  int rs_nall = 0; 
  int rs_nsv = 0;
  int rs_nbv = 0;
  int rs_nbsv = 0;
  int rs_ncv = 0;
  int rs_nlv = 0;
  int nfake = 0;

  int sr_nall = 0; 
  int sr_nsv = 0;
  int sr_nbv = 0;
  int sr_nbsv = 0;
  int sr_ncv = 0;
  int sr_nlv = 0;
  int nmiss = 0;

  // Loop over the svTagInfo collection.
  for (std::size_t index = 0; index < svTagInfoCollection->size(); ++index){

    reco::SecondaryVertexTagInfoRef svTagInfo(svTagInfoCollection, index);

    // Loop over the vertexes in svTagInfo
    for ( std::size_t vindex = 0; vindex < svTagInfo->nVertices(); ++vindex ){

 
      // Classify the vertices
      classifier_.evaluate(svTagInfo, vindex);

      // Fill the histogram with the categories
      for (Int_t i = 0; i != numberVertexClassifier_; ++i) {

	if ( classifier_.is( (VertexCategories::Category) i ) ) {

	  HistIndex_["VertexClassifier"]->Fill(i);
	}
      }
      if ( !classifier_.is(VertexCategories::Fake) ) {

	fillRecoToSim("rs_All", svTagInfo->secondaryVertex(vindex), tracer.simVertex());
	HistIndex_["rs_All_FlightDistance2d"]->Fill( svTagInfo->flightDistance( vindex, true ).value() );
        rs_nall++;

	if ( classifier_.is(VertexCategories::SecondaryVertex) ) {

	  fillRecoToSim("rs_SecondaryVertex", svTagInfo->secondaryVertex(vindex), tracer.simVertex());
	  HistIndex_["rs_SecondaryVertex_FlightDistance2d"]->Fill( svTagInfo->flightDistance( vindex, true ).value() );
          rs_nsv++;
	}
        
	if ( classifier_.is(VertexCategories::BWeakDecay) ) {

	  fillRecoToSim("rs_BWeakDecay", svTagInfo->secondaryVertex(vindex), tracer.simVertex());
	  HistIndex_["rs_BWeakDecay_FlightDistance2d"]->Fill( svTagInfo->flightDistance( vindex, true ).value() );
          rs_nbv++;

          if ( classifier_.is(VertexCategories::SecondaryVertex) ) {

	    fillRecoToSim("rs_BSV", svTagInfo->secondaryVertex(vindex), tracer.simVertex());
            HistIndex_["rs_BSV_FlightDistance2d"]->Fill( svTagInfo->flightDistance( vindex, true ).value() );
	    rs_nbsv++;	 

          }          
	}//BWeakDecay

	else if ( classifier_.is(VertexCategories::CWeakDecay) ) {

	  fillRecoToSim("rs_CWeakDecay", svTagInfo->secondaryVertex(vindex), tracer.simVertex());
	  HistIndex_["rs_CWeakDecay_FlightDistance2d"]->Fill( svTagInfo->flightDistance( vindex, true ).value() );
          rs_ncv++;

	}                
	else {

	  fillRecoToSim("rs_Light", svTagInfo->secondaryVertex(vindex), tracer.simVertex());
	  HistIndex_["rs_Light_FlightDistance2d"]->Fill( svTagInfo->flightDistance( vindex, true ).value() );
          rs_nlv++;
	}
      }//end if classifier

      else {
        cout << "    VertexCategories::Fake!!" << endl;
        nfake++;
      }


    }//end loop over vertices in svTagInfo

  }//loop over svTagInfo

  HistIndex_["rs_All_nRecVtx"]->Fill( rs_nall );
  HistIndex_["rs_SecondaryVertex_nRecVtx"]->Fill( rs_nsv );
  HistIndex_["rs_BWeakDecay_nRecVtx"]->Fill( rs_nbv );
  HistIndex_["rs_BSV_nRecVtx"]->Fill( rs_nbsv );
  HistIndex_["rs_CWeakDecay_nRecVtx"]->Fill( rs_ncv );  
  HistIndex_["rs_Light_nRecVtx"]->Fill( rs_nlv );  
  cout << endl;  

  //----------------------------------------------------------------
  // SIM TO RECO!

  // Vertex collection
  edm::Handle<TrackingVertexCollection>  TVCollection;
  event.getByLabel(trackingTruth_, TVCollection);
    
  // Loop over the TV collection.
  for (std::size_t index = 0; index < TVCollection->size(); ++index){
        
    TrackingVertexRef trackingVertex(TVCollection, index);

    classifier_.evaluate(trackingVertex);
 	
    if ( classifier_.is(VertexCategories::Reconstructed) ) {

      fillSimToReco("sr_All", tracer.recoVertex(), trackingVertex);
      sr_nall++;

      if ( classifier_.is(VertexCategories::SecondaryVertex) ) {

        fillSimToReco("sr_SecondaryVertex", tracer.recoVertex(), trackingVertex);
        sr_nsv++;
      }

      if ( classifier_.is(VertexCategories::BWeakDecay) ) {

        fillSimToReco("sr_BWeakDecay", tracer.recoVertex(), trackingVertex);
        sr_nbv++;

        if ( classifier_.is(VertexCategories::SecondaryVertex) ) {

         fillSimToReco("sr_BSV", tracer.recoVertex(), trackingVertex);
         sr_nbsv++;
	}
    
      }//BWeakDecay

      else if ( classifier_.is(VertexCategories::CWeakDecay) ) {

	fillSimToReco("sr_CWeakDecay", tracer.recoVertex(), trackingVertex);
        sr_ncv++;
      }
 
      else {

	fillSimToReco("sr_Light", tracer.recoVertex(), trackingVertex);
        sr_nlv++;
      }

    }//Reconstructed
    else {
      //cout << "##### Not reconstructed!" << endl;
      nmiss++;
    }

  }//TVCollection.size()

  HistIndex_["sr_All_nRecVtx"]->Fill( sr_nall );
  HistIndex_["sr_SecondaryVertex_nRecVtx"]->Fill( sr_nsv );
  HistIndex_["sr_BWeakDecay_nRecVtx"]->Fill( sr_nbv );
  HistIndex_["sr_BSV_nRecVtx"]->Fill( sr_nbsv );
  HistIndex_["sr_CWeakDecay_nRecVtx"]->Fill( sr_ncv );  
  HistIndex_["sr_Light_nRecVtx"]->Fill( rs_nlv );  

  rs_total_nall += rs_nall;  
  rs_total_nsv += rs_nsv;
  rs_total_nbv += rs_nbv;
  rs_total_nbsv += rs_nbsv;
  rs_total_ncv += rs_ncv ;
  rs_total_nlv += rs_nlv;
  total_nfake += nfake;

  sr_total_nall += sr_nall; 
  sr_total_nsv += sr_nsv;
  sr_total_nbv += sr_nbv;
  sr_total_nbsv += sr_nbsv;
  sr_total_ncv += sr_ncv;
  sr_total_nlv += sr_nlv;
  total_nmiss += nmiss;


}


void recoBSVTagInfoValidationAnalyzer::bookRecoToSim(std::string const & prefix){
  // Book pull histograms

  std::string name = prefix + "_Pullx";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -10., 10.);
  name = prefix + "_Pully";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -10., 10.);
  name = prefix + "_Pullz";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -10., 10.);

   name = prefix + "_Resx";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -0.05, 0.05);
  name = prefix + "_Resy";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -0.05, 0.05); 
  name = prefix + "_Resz";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -0.05, 0.05); 

  name = prefix + "_Chi2Norm";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, 0, 10.); 
  name = prefix + "_Chi2Prob";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, 0., 1.); 
 
  name = prefix + "_nRecTrks";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 501, -0.5, 500.5);

  name = prefix + "_AverageTrackWeight";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -0.1, 1.1);

  name = prefix + "_Mass";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 65, 0., 6.5);

  name = prefix + "_RecPt";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 2000, 0., 1000.);

  name = prefix + "_RecEta";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 200, -3., 3.);

  name = prefix + "_RecCharge";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 21, -0.5, 20.5);

  name = prefix + "_RecTrackPt";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 2000, 0., 1000.);

  name = prefix + "_RecTrackEta";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 200, -3., 3.);

  name = prefix + "_nSimTrks";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 501, -0.5, 500.5); 

  name = prefix + "_SimPt";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 2000, 0., 1000.);

  name = prefix + "_SimEta";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 200, -3., 3.);

  name = prefix + "_SimCharge";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 21, -0.5, 20.5);
  
  name = prefix + "_SimTrackPt";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 500, 0., 500.);

  name = prefix + "_SimTrackEta";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 200, -3., 3.);


}


void recoBSVTagInfoValidationAnalyzer::bookSimToReco(std::string const & prefix){
  // Book pull histograms

  std::string name = prefix + "_Pullx";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -10., 10.);
  name = prefix + "_Pully";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -10., 10.);
  name = prefix + "_Pullz";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -10., 10.);

   name = prefix + "_Resx";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -0.05, 0.05);
  name = prefix + "_Resy";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -0.05, 0.05); 
  name = prefix + "_Resz";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -0.05, 0.05); 

  name = prefix + "_Chi2Norm";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, 0, 10.); 
  name = prefix + "_Chi2Prob";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, 0., 1.); 
 
  name = prefix + "_nRecTrks";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 501, -0.5, 500.5);

  name = prefix + "_AverageTrackWeight";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 100, -0.1, 1.1);

  name = prefix + "_Mass";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 65, 0., 6.5);

  name = prefix + "_RecPt";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 2000, 0., 1000.);

  name = prefix + "_RecEta";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 200, -3., 3.);

  name = prefix + "_RecCharge";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 21, -0.5, 20.5);

  name = prefix + "_RecTrackPt";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 2000, 0., 1000.);

  name = prefix + "_RecTrackEta";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 200, -3., 3.);

  name = prefix + "_nSimTrks";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 501, -0.5, 500.5); 

  name = prefix + "_SimPt";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 2000, 0., 1000.);

  name = prefix + "_SimEta";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 200, -3., 3.);

  name = prefix + "_SimCharge";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 21, -0.5, 20.5);
  
  name = prefix + "_SimTrackPt";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 500, 0., 500.);

  name = prefix + "_SimTrackEta";
  HistIndex_[name] = dqmStore_->book1D(name.c_str(), name.c_str(), 200, -3., 3.);


}


void recoBSVTagInfoValidationAnalyzer::fillRecoToSim(std::string const & prefix, reco::Vertex const & vertex, TrackingVertexRef const & simVertex)
{
 
  double pullx = (vertex.x() - simVertex->position().x())/vertex.xError();
  double pully = (vertex.y() - simVertex->position().y())/vertex.yError();
  double pullz = (vertex.z() - simVertex->position().z())/vertex.zError();

  double resx = vertex.x() - simVertex->position().x();
  double resy = vertex.y() - simVertex->position().y();
  double resz = vertex.z() - simVertex->position().z();

  double chi2norm = vertex.normalizedChi2();
  double chi2prob = ChiSquaredProbability( vertex.chi2(), vertex.ndof() ); 

 double sum_weight = 0.;
 double weight = 0.;
 double tracksize =  vertex.tracksSize();
 math::XYZVector momentum;
 math::XYZTLorentzVector sum;
 int charge = 0;
  double thePiMass = 0.13957;
  for ( reco::Vertex::trackRef_iterator recDaughter = vertex.tracks_begin() ; recDaughter != vertex.tracks_end(); ++recDaughter) {

    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<double> > vec;

    vec.SetPx( (**recDaughter).px() );
    vec.SetPy( (**recDaughter).py() );
    vec.SetPz( (**recDaughter).pz() );
    vec.SetM(thePiMass); 

    sum += vec;
  
    weight = vertex.trackWeight(*recDaughter); 
    sum_weight += weight;

    math::XYZVector p;
    p = (**recDaughter).momentum();
    momentum += p; 

    charge += (*recDaughter)->charge();

    HistIndex_[prefix + "_RecTrackPt"]->Fill( (*recDaughter)->pt() );
    HistIndex_[prefix + "_RecTrackEta"]->Fill( (*recDaughter)->eta() );      
  }//end loop to recDaughters
  //cout << "                   average sum of weights = " << sum_weight/tracksize << endl; 
  
  double mass = sum.M();
  double pt = sqrt( momentum.Perp2() );
  double eta = momentum.Eta();

  math::XYZVector simmomentum;
  int simcharge = 0;
  for ( TrackingVertex::tp_iterator simDaughter = simVertex->daughterTracks_begin(); simDaughter != simVertex->daughterTracks_end(); ++simDaughter ){

    math::XYZVector p;
    p = (**simDaughter).momentum();
    simmomentum += p; 

    simcharge += (*simDaughter)->charge();

    HistIndex_[prefix + "_SimTrackPt"]->Fill( (*simDaughter)->pt() );
    HistIndex_[prefix + "_SimTrackEta"]->Fill( (*simDaughter)->eta() );   
  }

  double simpt = sqrt( simmomentum.Perp2() );
  double simeta = simmomentum.Eta();

  //cout << "[fillRecoToSim]  vertex.tracksSize() = " << vertex.tracksSize() << " ; simVertex->nDaughterTracks() = " << simVertex->nDaughterTracks() << endl;  

  HistIndex_[prefix + "_nRecTrks"]->Fill( vertex.tracksSize() );
  HistIndex_[prefix + "_nSimTrks"]->Fill( simVertex->nDaughterTracks() );
  HistIndex_[prefix + "_Pullx"]->Fill(pullx);
  HistIndex_[prefix + "_Pully"]->Fill(pully);
  HistIndex_[prefix + "_Pullz"]->Fill(pullz);
  HistIndex_[prefix + "_Resx"]->Fill(resx);
  HistIndex_[prefix + "_Resy"]->Fill(resy);
  HistIndex_[prefix + "_Resz"]->Fill(resz);
  HistIndex_[prefix + "_AverageTrackWeight"]->Fill( sum_weight/tracksize );   
  HistIndex_[prefix + "_Chi2Norm"]->Fill(chi2norm);
  HistIndex_[prefix + "_Chi2Prob"]->Fill(chi2prob);
  HistIndex_[prefix + "_RecPt"]->Fill(pt);
  HistIndex_[prefix + "_RecEta"]->Fill(eta);
  HistIndex_[prefix + "_RecCharge"]->Fill(charge);
  HistIndex_[prefix + "_Mass"]->Fill(mass);
  HistIndex_[prefix + "_SimPt"]->Fill(simpt);
  HistIndex_[prefix + "_SimEta"]->Fill(simeta);
  HistIndex_[prefix + "_SimCharge"]->Fill(simcharge);  
}


void recoBSVTagInfoValidationAnalyzer::fillSimToReco(std::string const & prefix, reco::VertexRef const & vertex, TrackingVertexRef const & simVertex)
{

  double pullx = (vertex->x() - simVertex->position().x())/vertex->xError();
  double pully = (vertex->y() - simVertex->position().y())/vertex->yError();
  double pullz = (vertex->z() - simVertex->position().z())/vertex->zError();

  double resx = vertex->x() - simVertex->position().x();
  double resy = vertex->y() - simVertex->position().y();
  double resz = vertex->z() - simVertex->position().z();

  double chi2norm = vertex->normalizedChi2();
  double chi2prob = ChiSquaredProbability(vertex->chi2(), vertex->ndof()); 

  double sum_weight = 0.;
  double weight = 0.;
  double tracksize =  vertex->tracksSize();
  math::XYZVector momentum;
  math::XYZTLorentzVector sum;
  int charge = 0;
  double thePiMass = 0.13957;
  for ( reco::Vertex::trackRef_iterator recDaughter = vertex->tracks_begin() ; recDaughter != vertex->tracks_end(); ++recDaughter ) {

    ROOT::Math::LorentzVector<ROOT::Math::PxPyPzM4D<double> > vec;

    vec.SetPx( (**recDaughter).px() );
    vec.SetPy( (**recDaughter).py() );
    vec.SetPz( (**recDaughter).pz() );
    vec.SetM(thePiMass); 

    sum += vec;
  
    weight = vertex->trackWeight(*recDaughter); 
    sum_weight += weight;

    math::XYZVector p;
    p = (**recDaughter).momentum();
    momentum += p; 

    charge += (*recDaughter)->charge();

    HistIndex_[prefix + "_RecTrackPt"]->Fill( (*recDaughter)->pt() );
    HistIndex_[prefix + "_RecTrackEta"]->Fill( (*recDaughter)->eta() );  
  
  }
  //cout << "                   average sum of weights = " << sum_weight/tracksize << endl; 

  double mass = sum.M();
  double pt = sqrt( momentum.Perp2() );
  double eta = momentum.Eta();

   math::XYZVector simmomentum;
  int simcharge = 0;
  for ( TrackingVertex::tp_iterator simDaughter = simVertex->daughterTracks_begin(); simDaughter != simVertex->daughterTracks_end(); ++simDaughter ){

    math::XYZVector p;
    p = (**simDaughter).momentum();
    simmomentum += p; 

    simcharge += (*simDaughter)->charge();

    HistIndex_[prefix + "_SimTrackPt"]->Fill( (*simDaughter)->pt() );
    HistIndex_[prefix + "_SimTrackEta"]->Fill( (*simDaughter)->eta() );   
  }

  double simpt = sqrt( simmomentum.Perp2() );
  double simeta = simmomentum.Eta();

  //cout << "[fillSimToReco]  vertex->tracksSize() = " << vertex->tracksSize() << " ; simVertex->nDaughterTracks() = " << simVertex->nDaughterTracks() << endl;  

  HistIndex_[prefix + "_nRecTrks"]->Fill( vertex->tracksSize() );
  HistIndex_[prefix + "_nSimTrks"]->Fill( simVertex->nDaughterTracks() );
  HistIndex_[prefix + "_Pullx"]->Fill(pullx);
  HistIndex_[prefix + "_Pully"]->Fill(pully);
  HistIndex_[prefix + "_Pullz"]->Fill(pullz);
  HistIndex_[prefix + "_Resx"]->Fill(resx);
  HistIndex_[prefix + "_Resy"]->Fill(resy);
  HistIndex_[prefix + "_Resz"]->Fill(resz);
  HistIndex_[prefix + "_AverageTrackWeight"]->Fill( sum_weight/tracksize );   
  HistIndex_[prefix + "_Chi2Norm"]->Fill(chi2norm);
  HistIndex_[prefix + "_Chi2Prob"]->Fill(chi2prob);
  HistIndex_[prefix + "_RecPt"]->Fill(pt);
  HistIndex_[prefix + "_RecEta"]->Fill(eta);
  HistIndex_[prefix + "_RecCharge"]->Fill(charge);
  HistIndex_[prefix + "_Mass"]->Fill(mass);
  HistIndex_[prefix + "_SimPt"]->Fill(simpt);
  HistIndex_[prefix + "_SimEta"]->Fill(simeta);
  HistIndex_[prefix + "_SimCharge"]->Fill(simcharge);  

}


void 
recoBSVTagInfoValidationAnalyzer::endJob() {

  std::cout << std::endl;
  std::cout << " ====== Total Number of analyzed events: " << n_event << " ====== " << std::endl;
  std::cout << " ====== Total Number of R2S All:                         " << rs_total_nall << " ====== " << std::endl;
  std::cout << " ====== Total Number of R2S SecondaryVertex:             " << rs_total_nsv  << " ====== " << std::endl;
  std::cout << " ====== Total Number of R2S BWeakDecay:                  " << rs_total_nbv  << " ====== " << std::endl;
  std::cout << " ====== Total Number of R2S BWeakDecay::SecondaryVertex: " << rs_total_nbsv << " ====== " << std::endl;
  std::cout << " ====== Total Number of R2S CWeakDecay:                  " << rs_total_ncv  << " ====== " << std::endl;
  std::cout << " ====== Total Number of R2S Light:                       " << rs_total_nlv  << " ====== " << std::endl;
  std::cout << std::endl; 
  std::cout << " ====== Total Number of S2R All:                         " << sr_total_nall << " ====== " << std::endl;
  std::cout << " ====== Total Number of S2R SecondaryVertex:             " << sr_total_nsv  << " ====== " << std::endl;
  std::cout << " ====== Total Number of S2R BWeakDecay:                  " << sr_total_nbv  << " ====== " << std::endl;
  std::cout << " ====== Total Number of S2R BWeakDecay::SecondaryVertex: " << sr_total_nbsv << " ====== " << std::endl;
  std::cout << " ====== Total Number of S2R CWeakDecay:                  " << sr_total_ncv  << " ====== " << std::endl;
  std::cout << " ====== Total Number of S2R Light:                       " << sr_total_nlv  << " ====== " << std::endl;
  std::cout << std::endl; 
  std::cout << " ====== Total Number of Fake Vertices:              " << total_nfake   << " ====== " << std::endl;
 
} 

DEFINE_FWK_MODULE(recoBSVTagInfoValidationAnalyzer);
