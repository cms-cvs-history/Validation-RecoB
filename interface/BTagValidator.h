#ifndef RecoB_BTagValidator_h
#define RecoB_BTagValidator_h

/**_________________________________________________________________
   class:   BTagValidator.h
   package: Validation/RecoB
   

 author: Victor Bazterra, UIC
         Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)

 version $Id: BTagValidator.h,v 1.1 2007/02/13 16:31:46 yumiceva Exp $

________________________________________________________________**/

// system include files
#include <memory>

// user include files
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "RecoBTag/Analysis/interface/BTagPABase.h"

//
// class decleration
//

class BTagValidator : public edm::EDAnalyzer
{
  public:

    explicit BTagValidator(const edm::ParameterSet&);
    ~BTagValidator();

  private:

    virtual void beginJob(const edm::EventSetup&) {}
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    // ---------- input variables -----------------------
    typedef std::vector<std::string> vstring;
    
    std::string algorithm_;
    std::string rootFile_;    
    std::string DQMFile_;
	std::string referenceFilename_;
    vstring histogramList_;
	bool doCompare_;
	
    // performance analicer base class
    BTagPABaseABC * petBase_;
};


#endif
