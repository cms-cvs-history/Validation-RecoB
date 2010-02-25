import FWCore.ParameterSet.Config as cms

# BTagPerformanceAnalyzer configuration
from DQMOffline.RecoB.bTagCombinedSVVariables_cff import *
#includes added because of block refactoring replacing simple includes by using statements
from DQMOffline.RecoB.bTagTrackIPAnalysis_cff import *
from DQMOffline.RecoB.bTagCombinedSVAnalysis_cff import *
from DQMOffline.RecoB.bTagTrackCountingAnalysis_cff import *
from DQMOffline.RecoB.bTagTrackProbabilityAnalysis_cff import *
from DQMOffline.RecoB.bTagTrackBProbabilityAnalysis_cff import *
from DQMOffline.RecoB.bTagGenericAnalysis_cff import *
from DQMOffline.RecoB.bTagSimpleSVAnalysis_cff import *
from DQMOffline.RecoB.bTagSoftLeptonAnalysis_cff import *
from DQMOffline.RecoB.bTagSoftLeptonByPtAnalysis_cff import *
from DQMOffline.RecoB.bTagSoftLeptonByIPAnalysis_cff import *
from DQMOffline.RecoB.bTagCommon_cff import *
bTagValidation = cms.EDAnalyzer("BTagPerformanceAnalyzerMC",
    bTagCommonBlock,
    finalizeOnly = cms.bool(False),
    jetCorrection = cms.string(''),
    recJetMatching = cms.PSet(
        refJetCorrection = cms.string(''),
        recJetCorrection = cms.string(''),
        maxChi2 = cms.double(50),
        # Corrected calo jets
        sigmaDeltaR = cms.double(0.1),
        sigmaDeltaE = cms.double(0.15)
    ),
    finalizePlots = cms.bool(True),
    tagConfig = cms.VPSet(
        cms.PSet(
            bTagTrackIPAnalysisBlock,
            type = cms.string('TrackIP'),
            label = cms.InputTag("impactParameterTagInfos")
        ), 
        cms.PSet(
            bTagCombinedSVAnalysisBlock,
            ipTagInfos = cms.InputTag("impactParameterTagInfos"),
            type = cms.string('GenericMVA'),
            svTagInfos = cms.InputTag("secondaryVertexTagInfos"),
            label = cms.InputTag("combinedSecondaryVertex")
        ), 
        cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("trackCountingHighEffBJetTags")
        ), 
        cms.PSet(
            bTagTrackCountingAnalysisBlock,
            label = cms.InputTag("trackCountingHighPurBJetTags")
        ), 
        cms.PSet(
            bTagProbabilityAnalysisBlock,
            label = cms.InputTag("jetProbabilityBJetTags")
        ), 
        cms.PSet(
            bTagBProbabilityAnalysisBlock,
            label = cms.InputTag("jetBProbabilityBJetTags")
        ), 
        cms.PSet(
            bTagSimpleSVAnalysisBlock,
            label = cms.InputTag("simpleSecondaryVertexHighEffBJetTags")
        ), 
        cms.PSet(
            bTagSimpleSVAnalysisBlock,
            label = cms.InputTag("simpleSecondaryVertexHighPurBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("combinedSecondaryVertexBJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("combinedSecondaryVertexMVABJetTags")
        ), 
        cms.PSet(
            bTagGenericAnalysisBlock,
            label = cms.InputTag("ghostTrackBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonAnalysisBlock,
            label = cms.InputTag("softMuonBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByIPAnalysisBlock,
            label = cms.InputTag("softMuonByIP3dBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByPtAnalysisBlock,
            label = cms.InputTag("softMuonByPtBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByIPAnalysisBlock,
            label = cms.InputTag("softElectronByIP3dBJetTags")
        ), 
        cms.PSet(
            bTagSoftLeptonByPtAnalysisBlock,
            label = cms.InputTag("softElectronByPtBJetTags")
        )
    ),
    mcPlots = cms.bool(True)
)


