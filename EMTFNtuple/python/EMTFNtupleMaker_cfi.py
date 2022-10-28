import FWCore.ParameterSet.Config as cms

EMTFNtuple = cms.EDAnalyzer('EMTFNtuple',

    CSCInputTag      = cms.InputTag('simCscTriggerPrimitiveDigis','MPCSORTED'),
    RPCInputTag      = cms.InputTag('simMuonRPCDigis'),
    CPPFInputTag     = cms.InputTag('simCPPFDigis'),
    GEMInputTag      = cms.InputTag('simMuonGEMPadDigiClusters'),

    IRPCInputTag     = cms.InputTag('simMuonRPCDigis'),
    ME0InputTag      = cms.InputTag('me0TriggerConvertedPseudoDigis'),
    DTInputTag       = cms.InputTag('simDtTriggerPrimitiveDigis'),

    EMTFHitTag       = cms.InputTag('simEmtfDigis'),
    EMTFUnpHitTag    = cms.InputTag('emtfStage2Digis'),

    EMTFTrackTag     = cms.InputTag('simEmtfDigis'),
    EMTFUnpTrackTag  = cms.InputTag('emtfStage2Digis'),

    GMTMuonTag       = cms.InputTag('simGmtStage2Digis'),
    GMTUnpMuonTag    = cms.InputTag('gmtStage2Digis', 'Muon'),

    GENPartTag       = cms.InputTag('genParticles'),
    RECOMuonTag      = cms.InputTag('muons'),
    RECOVertexTag    = cms.InputTag('offlinePrimaryVertices'),
    muonTriggers     = cms.vstring("HLT_IsoMu27", "HLT_IsoTkMu27", "HLT_Mu50"),
    TriggerEventTag  = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
    RECOBeamSpotTag  = cms.InputTag("offlineBeamSpot"),


    CSCSegmentTag    = cms.InputTag('cscSegments'),

    # RECO muon extrapolation to 1st station
    muProp1st = cms.PSet(
        useTrack          = cms.string("tracker"),  # 'none' to use Candidate P4; or 'tracker', 'muon', 'global'
        useState          = cms.string("atVertex"), # 'innermost' and 'outermost' require the TrackExtra
        useSimpleGeometry = cms.bool(True),
        useStation2       = cms.bool(False),
    ),
                                
    # RECO muon extrapolation to 2nd station
    muProp2nd = cms.PSet(
        useTrack          = cms.string("tracker"),  # 'none' to use Candidate P4; or 'tracker', 'muon', 'global'
        useState          = cms.string("atVertex"), # 'innermost' and 'outermost' require the TrackExtra
        useSimpleGeometry = cms.bool(True),
        useStation2       = cms.bool(True),
        fallbackToME1     = cms.bool(False),
    ),

    outFileName      = cms.string('EMTFNtuple.root'),
    verbosity        = cms.untracked.int32(0),

    # Change defaults to enable/disable processing of various inputs
    enablePhase2     = cms.bool(False),

    useCSC           = cms.bool(True),
    useRPC           = cms.bool(True),
    useCPPF          = cms.bool(True),
    useGEM           = cms.bool(False),

    useIRPC          = cms.bool(False),
    useME0           = cms.bool(False),
    useDT            = cms.bool(False),

    useEMTFHits      = cms.bool(True),
    useEMTFUnpHits   = cms.bool(False),

    useEMTFTracks    = cms.bool(True),
    useEMTFUnpTracks = cms.bool(False),

    useGMTMuons      = cms.bool(True),
    useGMTUnpMuons   = cms.bool(False),

    useGENParts      = cms.bool(True),
    useRECOMuons     = cms.bool(False),
    useEventInfo     = cms.bool(False),

    useCSCSegments   = cms.bool(False),
    matchCSCSegments   = cms.bool(False),

    isReco           = cms.bool(True),
  )
