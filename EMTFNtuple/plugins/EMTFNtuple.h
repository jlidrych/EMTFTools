// -*- C++ -*-
//
// Package:    EMTFTools/EMTFNtuple
// Class:      EMTFNtuple
//
/**\class EMTFNtuple EMTFNtuple.cc EMTFTools/EMTFNtuple/plugins/EMTFNtuple.cc

 Description: Creates flat ntuples to be used for EMTF studies.

*/
//
// Original Author:  Efe Yigitbasi
//         Created:  Tue, 01 Sep 2020 10:52:51 GMT
//
//

// system include files
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
//#include <regex>
#include <boost/regex.hpp>
// ROOT includes
#include "TFile.h"
#include "TMath.h"
#include "TString.h"
#include "TTree.h"

// CMSSW includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/CSCRecHit/interface/CSCSegmentCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "Geometry/CSCGeometry/interface/CSCChamber.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"

#include "L1Trigger/L1TMuon/interface/GeometryTranslator.h"
#include "L1Trigger/L1TMuonEndCap/interface/Common.h"
#include "L1Trigger/L1TMuonEndCap/interface/EMTFSubsystemCollector.h"
#include "L1Trigger/L1TMuonEndCap/interface/TrackTools.h"

#include "DataFormats/L1Trigger/interface/Muon.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "L1Trigger/L1TNtuples/interface/MuonID.h"
//#include "L1Trigger/L1TNtuples/interface/L1AnalysisRecoMuon2.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/HitPattern.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
// RECO vertices
#include "DataFormats/VertexReco/interface/VertexFwd.h"
// HLT configuration
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
//
// RECO muon track extrapolation
#include "MuonAnalysis/MuonAssociators/interface/PropagateToMuon.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
// HLT trigger
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/Math/interface/deltaR.h"
// ----------------------------------------------------
//

class EMTFNtuple : public edm::one::EDAnalyzer<edm::one::SharedResources,edm::one::WatchRuns> {
  public:
    explicit EMTFNtuple(const edm::ParameterSet &);
    ~EMTFNtuple();

    static void fillDescriptions(edm::ConfigurationDescriptions &descriptions);
  
  private:
    void beginJob() override;
    void beginRun(const edm::Run&,   const edm::EventSetup&);
    void endRun(const edm::Run&,   const edm::EventSetup&);
    auto getHitRefs(const l1t::EMTFTrack &trk,
                    const l1t::EMTFHitCollection &hits);
  
    auto hltMatching(const reco::Muon muon, 
		     const trigger::TriggerEvent* trigEvent,
		     std::vector<std::string> trigModLabels,
		     const double _muon_trig_dR,
		     const double _muon_trig_pt);

    void analyze(const edm::Event &, const edm::EventSetup &) override;
    void endJob() override;
  
    // Aux functions
    void getHandles(const edm::Event &iEvent, const edm::EventSetup &iSetup);
    void fillTree();
    void makeTree();
  

    template <typename T> edm::Handle<T> make_handle(T &t) {
        return edm::Handle<T>();
    }

    template <typename T> edm::Handle<T> make_handle(T *t) {
        return edm::Handle<T>();
    }

    // ---------- Member data ---------------------------

    // Input parameters

    // Run 3 TPs
    const edm::InputTag CSCInputTag_;
    const edm::InputTag RPCInputTag_;
    const edm::InputTag CPPFInputTag_;
    const edm::InputTag GEMInputTag_;

    // Phase 2 TPs
    const edm::InputTag IRPCInputTag_;
    const edm::InputTag ME0InputTag_;
    const edm::InputTag DTInputTag_;

    // L1T collections
    const edm::InputTag EMTFHitTag_;
    const edm::InputTag EMTFUnpHitTag_;

    const edm::InputTag EMTFTrackTag_;
    const edm::InputTag EMTFUnpTrackTag_;

    const edm::InputTag GMTMuonTag_;
    const edm::InputTag GMTUnpMuonTag_;

    const edm::InputTag GENPartTag_;
    const edm::InputTag RECOMuonTag_;
    const edm::InputTag RECOVertexTag_;

    // User defined settings
    const std::vector<std::string> muonTriggers_;  // List of unprescale SingleMuon triggers
  
    const edm::InputTag RECOBeamSpotTag_;
    const edm::InputTag TriggerEventTag_;
    // HLT trigger matching
    std::vector<std::string> trigNames_;      // HLT triggers matching the desired names
    std::vector<std::string> trigModLabels_;  // HLT 3rd-to-last module label for each trigger

    // Reco CSC segments
    const edm::InputTag CSCSegmentTag_;

    const std::string outFileName_;
    int verbose_;

    bool enablePhase2_;

    bool useCSC_;
    bool useRPC_;
    bool useCPPF_;
    bool useGEM_;

    bool useIRPC_;
    bool useME0_;
    bool useDT_;

    bool useEMTFHits_;
    bool useEMTFUnpHits_;

    bool useEMTFTracks_;
    bool useEMTFUnpTracks_;

    bool useGMTMuons_;
    bool useGMTUnpMuons_;

    bool useGENParts_;
    bool useRECOMuons_;
    bool useEventInfo_;

    bool useCSCSegments_;
    bool matchCSCSegments_;

    bool isReco_;

    // Run 3 TPs
    edm::EDGetTokenT<emtf::CSCTag::digi_collection> CSCInputToken_;
    edm::EDGetTokenT<emtf::RPCTag::digi_collection> RPCInputToken_;
    edm::EDGetTokenT<emtf::CPPFTag::digi_collection> CPPFInputToken_;
    edm::EDGetTokenT<emtf::GEMTag::digi_collection> GEMInputToken_;

    // Phase 2 TPs
    edm::EDGetTokenT<emtf::IRPCTag::digi_collection> IRPCInputToken_;
    edm::EDGetTokenT<emtf::ME0Tag::digi_collection> ME0InputToken_;
    edm::EDGetTokenT<emtf::DTTag::digi_collection> DTInputToken_;

    // L1T collections
    edm::EDGetTokenT<l1t::EMTFHitCollection> EMTFHitToken_;
    edm::EDGetTokenT<l1t::EMTFHitCollection> EMTFUnpHitToken_;

    edm::EDGetTokenT<l1t::EMTFTrackCollection> EMTFTrackToken_;
    edm::EDGetTokenT<l1t::EMTFTrackCollection> EMTFUnpTrackToken_;

    edm::EDGetTokenT<l1t::MuonBxCollection> GMTMuonToken_;
    edm::EDGetTokenT<l1t::MuonBxCollection> GMTUnpMuonToken_;

    edm::EDGetTokenT<reco::GenParticleCollection> GENPartToken_;
    edm::EDGetTokenT<reco::MuonCollection> RECOMuonToken_;
    edm::EDGetTokenT<reco::VertexCollection> RECOVertexToken_;
    edm::EDGetTokenT<reco::BeamSpot> RECOBeamSpotToken_;
    edm::EDGetTokenT<trigger::TriggerEvent> TriggerEventToken_;
  
    edm::EDGetTokenT<CSCSegmentCollection> CSCSegmentToken_;

    edm::ESGetToken<CSCGeometry, MuonGeometryRecord> cscGeomToken_;
    edm::ESHandle<CSCGeometry> cscGeom_;



    PropagateToMuon muProp1st_;
    PropagateToMuon muProp2nd_;
    GeometryTranslator geometryTranslator_;
    HLTConfigProvider hltConfig_;
    EMTFSubsystemCollector collector_;

    TriggerPrimitiveCollection CSCInputs_;
    TriggerPrimitiveCollection RPCInputs_;
    TriggerPrimitiveCollection CPPFInputs_;
    TriggerPrimitiveCollection GEMInputs_;

    TriggerPrimitiveCollection IRPCInputs_;
    TriggerPrimitiveCollection ME0Inputs_;
    TriggerPrimitiveCollection DTInputs_;

    l1t::EMTFHitCollection EMTFHits_;
    l1t::EMTFHitCollection EMTFUnpHits_;

    l1t::EMTFTrackCollection EMTFTracks_;
    l1t::EMTFTrackCollection EMTFUnpTracks_;

    const l1t::MuonBxCollection *GMTMuons_;
    const l1t::MuonBxCollection *GMTUnpMuons_;

    const reco::GenParticleCollection *GENParts_;
    const reco::MuonCollection *RECOMuons_;
    const reco::VertexCollection *RECOVertices_;
    const reco::BeamSpot *RECOBeamSpot_;
    const trigger::TriggerEvent *TriggerEvents_;
    const CSCSegmentCollection *CSCSegments_;

    // TTree
    TTree *tree;

    bool firstEvent_;

    // Output collections

    // CSC Inputs
    std::unique_ptr<std::vector<int16_t>> cscInput_endcap;
    std::unique_ptr<std::vector<int16_t>> cscInput_station;
    std::unique_ptr<std::vector<int16_t>> cscInput_ring;
    std::unique_ptr<std::vector<int16_t>> cscInput_sector;
    std::unique_ptr<std::vector<int16_t>> cscInput_chamber;
    std::unique_ptr<std::vector<int16_t>> cscInput_cscid;
    std::unique_ptr<std::vector<int16_t>> cscInput_bx;
    std::unique_ptr<std::vector<int16_t>> cscInput_strip;
    std::unique_ptr<std::vector<int16_t>> cscInput_wire;
    std::unique_ptr<std::vector<int16_t>> cscInput_quality;
    std::unique_ptr<std::vector<int16_t>> cscInput_pattern;
    std::unique_ptr<std::vector<int16_t>> cscInput_bend;

    // RPC Inputs
    std::unique_ptr<std::vector<int16_t>> rpcInput_region;
    std::unique_ptr<std::vector<int16_t>> rpcInput_station;
    std::unique_ptr<std::vector<int16_t>> rpcInput_ring;
    std::unique_ptr<std::vector<int16_t>> rpcInput_sector;
    std::unique_ptr<std::vector<int16_t>> rpcInput_subsector;
    std::unique_ptr<std::vector<int16_t>> rpcInput_roll;
    std::unique_ptr<std::vector<int16_t>> rpcInput_bx;
    std::unique_ptr<std::vector<int16_t>> rpcInput_strip;
    std::unique_ptr<std::vector<int16_t>> rpcInput_strip_high;
    std::unique_ptr<std::vector<int16_t>> rpcInput_strip_low;
    std::unique_ptr<std::vector<int16_t>> rpcInput_time;
    std::unique_ptr<std::vector<int16_t>> rpcInput_valid;

    // GEM Inputs
    std::unique_ptr<std::vector<int16_t>> gemInput_region;
    std::unique_ptr<std::vector<int16_t>> gemInput_station;
    std::unique_ptr<std::vector<int16_t>> gemInput_ring;
    std::unique_ptr<std::vector<int16_t>> gemInput_sector;
    std::unique_ptr<std::vector<int16_t>> gemInput_chamber;
    std::unique_ptr<std::vector<int16_t>> gemInput_roll;
    std::unique_ptr<std::vector<int16_t>> gemInput_bx;
    std::unique_ptr<std::vector<int16_t>> gemInput_layer;
    std::unique_ptr<std::vector<int16_t>> gemInput_pad;
    std::unique_ptr<std::vector<int16_t>> gemInput_pad_low;
    std::unique_ptr<std::vector<int16_t>> gemInput_pad_high;

    // ME0 Inputs
    std::unique_ptr<std::vector<int16_t>> me0Input_region;
    std::unique_ptr<std::vector<int16_t>> me0Input_station;
    std::unique_ptr<std::vector<int16_t>> me0Input_ring;
    std::unique_ptr<std::vector<int16_t>> me0Input_sector;
    std::unique_ptr<std::vector<int16_t>> me0Input_chamber;
    std::unique_ptr<std::vector<int16_t>> me0Input_roll;
    std::unique_ptr<std::vector<int16_t>> me0Input_bx;
    std::unique_ptr<std::vector<int16_t>> me0Input_layer;
    std::unique_ptr<std::vector<int16_t>> me0Input_phiposition;
    std::unique_ptr<std::vector<int16_t>> me0Input_deltaphi;
    std::unique_ptr<std::vector<int16_t>> me0Input_quality;
    std::unique_ptr<std::vector<int16_t>> me0Input_bend;
    std::unique_ptr<std::vector<int16_t>> me0Input_partition;

    // DT Inputs
    std::unique_ptr<std::vector<int16_t>> dtInput_wheel;
    std::unique_ptr<std::vector<int16_t>> dtInput_station;
    std::unique_ptr<std::vector<int16_t>> dtInput_btigroup;
    std::unique_ptr<std::vector<int16_t>> dtInput_bx;
    std::unique_ptr<std::vector<int16_t>> dtInput_strip;
    std::unique_ptr<std::vector<int16_t>> dtInput_wire;
    std::unique_ptr<std::vector<int16_t>> dtInput_quality;
    std::unique_ptr<std::vector<int16_t>> dtInput_bend;

    // EMTF Hits
    std::unique_ptr<std::vector<int16_t>> emtfHit_endcap;
    std::unique_ptr<std::vector<int16_t>> emtfHit_station;
    std::unique_ptr<std::vector<int16_t>> emtfHit_ring;
    std::unique_ptr<std::vector<int16_t>> emtfHit_sector;
    std::unique_ptr<std::vector<int16_t>> emtfHit_subsector;
    std::unique_ptr<std::vector<int16_t>> emtfHit_chamber;
    std::unique_ptr<std::vector<int16_t>> emtfHit_cscid;
    std::unique_ptr<std::vector<int16_t>> emtfHit_bx;
    std::unique_ptr<std::vector<int16_t>>
        emtfHit_type; // subsystem: DT=0,CSC=1,RPC=2,GEM=3,ME0=4
    std::unique_ptr<std::vector<int16_t>> emtfHit_neighbor;
    //
    std::unique_ptr<std::vector<int16_t>> emtfHit_strip;
    std::unique_ptr<std::vector<int16_t>> emtfHit_wire;
    std::unique_ptr<std::vector<int16_t>> emtfHit_roll;
    std::unique_ptr<std::vector<int16_t>> emtfHit_quality;
    std::unique_ptr<std::vector<int16_t>> emtfHit_pattern;
    std::unique_ptr<std::vector<int16_t>> emtfHit_bend;
    std::unique_ptr<std::vector<int16_t>> emtfHit_time;
    std::unique_ptr<std::vector<int16_t>> emtfHit_fr;
    std::unique_ptr<std::vector<int32_t>> emtfHit_emtf_phi;   // integer unit
    std::unique_ptr<std::vector<int32_t>> emtfHit_emtf_theta; // integer unit
    //
    std::unique_ptr<std::vector<float>> emtfHit_sim_phi;   // in degrees
    std::unique_ptr<std::vector<float>> emtfHit_sim_theta; // in degrees
    std::unique_ptr<std::vector<float>> emtfHit_sim_r;     // in cm
    std::unique_ptr<std::vector<float>> emtfHit_sim_z;     // in cm
    // std::unique_ptr<std::vector<int32_t> >  emtfHit_sim_tp1;
    // std::unique_ptr<std::vector<int32_t> >  emtfHit_sim_tp2;
    std::unique_ptr<std::vector<int32_t>> emtfHit_match_iSeg;
    std::unique_ptr<std::vector<int32_t>> emtfHit_match_iSeg2;
    std::unique_ptr<std::vector<int32_t>> emtfHit_match_nSegs;
    std::unique_ptr<std::vector<int16_t>> emtfHit_match_seg_unique;
    std::unique_ptr<int32_t> emtfHit_size;

    // EMTF Unpacked Hits
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_endcap;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_station;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_ring;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_sector;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_subsector;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_chamber;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_cscid;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_bx;
    std::unique_ptr<std::vector<int16_t>>
        emtfUnpHit_type; // subsystem: DT=0,CSC=1,RPC=2,GEM=3,ME0=4
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_neighbor;
    //
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_strip;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_wire;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_roll;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_quality;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_pattern;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_bend;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_time;
    std::unique_ptr<std::vector<int16_t>> emtfUnpHit_fr;
    std::unique_ptr<std::vector<int32_t>> emtfUnpHit_emtf_phi;   // integer unit
    std::unique_ptr<std::vector<int32_t>> emtfUnpHit_emtf_theta; // integer unit
    //
    std::unique_ptr<std::vector<float>> emtfUnpHit_sim_phi;   // in degrees
    std::unique_ptr<std::vector<float>> emtfUnpHit_sim_theta; // in degrees
    std::unique_ptr<std::vector<float>> emtfUnpHit_sim_r;     // in cm
    std::unique_ptr<std::vector<float>> emtfUnpHit_sim_z;     // in cm
    // std::unique_ptr<std::vector<int32_t> >  emtfUnpHit_sim_tp1;
    // std::unique_ptr<std::vector<int32_t> >  emtfUnpHit_sim_tp2;
    std::unique_ptr<int32_t> emtfUnpHit_size;

    // EMTF Tracks
    std::unique_ptr<std::vector<float>> emtfTrack_pt;
    std::unique_ptr<std::vector<float>> emtfTrack_xml_pt;
    std::unique_ptr<std::vector<float>> emtfTrack_pt_dxy;
    std::unique_ptr<std::vector<float>> emtfTrack_dxy;
    std::unique_ptr<std::vector<float>> emtfTrack_invpt_prompt;
    std::unique_ptr<std::vector<float>> emtfTrack_invpt_displ;
    std::unique_ptr<std::vector<float>> emtfTrack_phi;        // in degrees
    std::unique_ptr<std::vector<int32_t>> emtfTrack_phi_fp;   // in degrees
    std::unique_ptr<std::vector<float>> emtfTrack_theta;      // in degrees
    std::unique_ptr<std::vector<int32_t>> emtfTrack_theta_fp; // in degrees
    std::unique_ptr<std::vector<float>> emtfTrack_eta;
    std::unique_ptr<std::vector<int32_t>> emtfTrack_GMT_phi;
    std::unique_ptr<std::vector<int32_t>> emtfTrack_GMT_eta;
    std::unique_ptr<std::vector<int16_t>> emtfTrack_q; // charge
    //
    std::unique_ptr<std::vector<int16_t>> emtfTrack_mode;
    std::unique_ptr<std::vector<int16_t>> emtfTrack_endcap;
    std::unique_ptr<std::vector<int16_t>> emtfTrack_sector;
    std::unique_ptr<std::vector<int16_t>> emtfTrack_bx;
    std::unique_ptr<std::vector<int16_t>> emtfTrack_nhits;
    std::unique_ptr<std::vector<int32_t>> emtfTrack_hitref1;
    std::unique_ptr<std::vector<int32_t>> emtfTrack_hitref2;
    std::unique_ptr<std::vector<int32_t>> emtfTrack_hitref3;
    std::unique_ptr<std::vector<int32_t>> emtfTrack_hitref4;
    std::unique_ptr<int32_t> emtfTrack_size;

    std::unique_ptr<std::vector<int16_t>> emtfTrack_straightness;

    // PtLUT information
    std::unique_ptr<std::vector<uint64_t>> emtfTrack_ptLUT_address;
    std::unique_ptr<std::vector<uint16_t>> emtfTrack_ptLUT_mode;
    std::unique_ptr<std::vector<uint16_t>> emtfTrack_ptLUT_theta;
    std::unique_ptr<std::vector<uint16_t>> emtfTrack_ptLUT_st1_ring2;
    std::unique_ptr<std::vector<uint16_t>> emtfTrack_ptLUT_eta;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfTrack_ptLUT_deltaPh;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfTrack_ptLUT_deltaTh;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfTrack_ptLUT_signPh;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfTrack_ptLUT_signTh;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfTrack_ptLUT_cpattern;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfTrack_ptLUT_fr;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfTrack_ptLUT_bt_vi;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfTrack_ptLUT_bt_hi;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfTrack_ptLUT_bt_ci;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfTrack_ptLUT_bt_si;

    // EMTF Unpacked Tracks
    std::unique_ptr<std::vector<float>> emtfUnpTrack_pt;
    std::unique_ptr<std::vector<float>> emtfUnpTrack_xml_pt;
    std::unique_ptr<std::vector<float>> emtfUnpTrack_pt_dxy;
    std::unique_ptr<std::vector<float>> emtfUnpTrack_dxy;
    std::unique_ptr<std::vector<float>> emtfUnpTrack_invpt_prompt;
    std::unique_ptr<std::vector<float>> emtfUnpTrack_invpt_displ;
    std::unique_ptr<std::vector<float>> emtfUnpTrack_phi;   // in degrees
    std::unique_ptr<std::vector<float>> emtfUnpTrack_theta; // in degrees
    std::unique_ptr<std::vector<float>> emtfUnpTrack_eta;
    std::unique_ptr<std::vector<int16_t>> emtfUnpTrack_q; // charge
    //
    std::unique_ptr<std::vector<int16_t>> emtfUnpTrack_mode;
    std::unique_ptr<std::vector<int16_t>> emtfUnpTrack_endcap;
    std::unique_ptr<std::vector<int16_t>> emtfUnpTrack_sector;
    std::unique_ptr<std::vector<int16_t>> emtfUnpTrack_bx;
    std::unique_ptr<std::vector<int16_t>> emtfUnpTrack_nhits;
    std::unique_ptr<std::vector<int32_t>> emtfUnpTrack_hitref1;
    std::unique_ptr<std::vector<int32_t>> emtfUnpTrack_hitref2;
    std::unique_ptr<std::vector<int32_t>> emtfUnpTrack_hitref3;
    std::unique_ptr<std::vector<int32_t>> emtfUnpTrack_hitref4;
    std::unique_ptr<int32_t> emtfUnpTrack_size;
    // PtLUT information
    std::unique_ptr<std::vector<uint64_t>> emtfUnpTrack_ptLUT_address;
    std::unique_ptr<std::vector<uint16_t>> emtfUnpTrack_ptLUT_mode;
    std::unique_ptr<std::vector<uint16_t>> emtfUnpTrack_ptLUT_theta;
    std::unique_ptr<std::vector<uint16_t>> emtfUnpTrack_ptLUT_st1_ring2;
    std::unique_ptr<std::vector<uint16_t>> emtfUnpTrack_ptLUT_eta;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfUnpTrack_ptLUT_deltaPh;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfUnpTrack_ptLUT_deltaTh;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfUnpTrack_ptLUT_signPh;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfUnpTrack_ptLUT_signTh;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfUnpTrack_ptLUT_cpattern;
    std::unique_ptr<std::vector<std::vector<uint16_t>>> emtfUnpTrack_ptLUT_fr;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfUnpTrack_ptLUT_bt_vi;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfUnpTrack_ptLUT_bt_hi;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfUnpTrack_ptLUT_bt_ci;
    std::unique_ptr<std::vector<std::vector<uint16_t>>>
        emtfUnpTrack_ptLUT_bt_si;

    // GMT Muons
    std::unique_ptr<std::vector<float>> gmtMuon_pt;
    std::unique_ptr<std::vector<float>> gmtMuon_pt_dxy;
    std::unique_ptr<std::vector<int16_t>> gmtMuon_dxy;
    std::unique_ptr<std::vector<float>> gmtMuon_phi; // in degrees
    std::unique_ptr<std::vector<float>> gmtMuon_eta;
    std::unique_ptr<std::vector<int16_t>> gmtMuon_q; // charge
    std::unique_ptr<std::vector<int16_t>> gmtMuon_qual;
    std::unique_ptr<int32_t> gmtMuon_size;

    // GMT Unpacked Muons
    std::unique_ptr<std::vector<float>> gmtUnpMuon_pt;
    std::unique_ptr<std::vector<float>> gmtUnpMuon_pt_dxy;
    std::unique_ptr<std::vector<int16_t>> gmtUnpMuon_dxy;
    std::unique_ptr<std::vector<float>> gmtUnpMuon_phi; // in degrees
    std::unique_ptr<std::vector<float>> gmtUnpMuon_eta;
    std::unique_ptr<std::vector<int16_t>> gmtUnpMuon_q; // charge
    std::unique_ptr<std::vector<int16_t>> gmtUnpMuon_qual;
    std::unique_ptr<int32_t> gmtUnpMuon_size;

    // GEN particles
    std::unique_ptr<std::vector<float>> genPart_pt;
    std::unique_ptr<std::vector<float>> genPart_dxy;
    std::unique_ptr<std::vector<float>> genPart_eta;
    std::unique_ptr<std::vector<float>> genPart_phi;
    std::unique_ptr<std::vector<int16_t>> genPart_q; // charge
    std::unique_ptr<std::vector<int16_t>> genPart_ID;
    std::unique_ptr<std::vector<int32_t>> genPart_parentID;
    std::unique_ptr<std::vector<float>> genPart_vx;
    std::unique_ptr<std::vector<float>> genPart_vy;
    std::unique_ptr<std::vector<float>> genPart_vz;

    //RECO muons
    std::unique_ptr<std::vector<float>> recoMuon_pt;
    std::unique_ptr<std::vector<float>> recoMuon_eta;
    std::unique_ptr<std::vector<float>> recoMuon_eta_st1;
    std::unique_ptr<std::vector<float>> recoMuon_eta_st2;
    std::unique_ptr<std::vector<float>> recoMuon_phi;
    std::unique_ptr<std::vector<float>> recoMuon_phi_st1;
    std::unique_ptr<std::vector<float>> recoMuon_phi_st2;
    std::unique_ptr<std::vector<float>> recoMuon_theta;
    std::unique_ptr<std::vector<float>> recoMuon_theta_st1;
    std::unique_ptr<std::vector<float>> recoMuon_theta_st2;
    std::unique_ptr<std::vector<float>> recoMuon_dxy_bs; //beam spot
    std::unique_ptr<std::vector<float>> recoMuon_dz_bs;
    std::unique_ptr<std::vector<float>> recoMuon_dxy_pv; //primary vertex
    std::unique_ptr<std::vector<float>> recoMuon_dz_pv; 
    std::unique_ptr<std::vector<float>> recoMuon_samPt;//StandAloneMuon pt
    std::unique_ptr<std::vector<int16_t>> recoMuon_q;//charge
    std::unique_ptr<std::vector<int16_t>> recoMuon_matchedStations;//matchedstation
    std::unique_ptr<std::vector<float>> recoMuon_chi2Norm;
    std::unique_ptr<std::vector<float>> recoMuon_iso;
    std::unique_ptr<std::vector<bool>> recoMuon_isLoose;
    std::unique_ptr<std::vector<bool>> recoMuon_isMedium;
    std::unique_ptr<std::vector<bool>> recoMuon_isTight;
    std::unique_ptr<std::vector<float>> recoMuon_hlt_dR;
    std::unique_ptr<std::vector<int16_t>> recoMuon_hlt_ID; 
    std::unique_ptr<int32_t> recoMuon_size;

    // Event info
    std::unique_ptr<std::vector<uint64_t>> eventInfo_event;
    std::unique_ptr<std::vector<uint32_t>> eventInfo_run;
    std::unique_ptr<std::vector<uint32_t>> eventInfo_lumi;
    std::unique_ptr<std::vector<float>>
        eventInfo_npv; // getTrueNumInteractions()
    std::unique_ptr<std::vector<int32_t>>
        eventInfo_nvtx; // getPU_NumInteractions()
    std::unique_ptr<int32_t> eventInfo_size;

    // CSC Segment info
    std::unique_ptr<std::vector<float>> cscSegment_locX;
    std::unique_ptr<std::vector<float>> cscSegment_locY;
    std::unique_ptr<std::vector<float>> cscSegment_globX;
    std::unique_ptr<std::vector<float>> cscSegment_globY;
    std::unique_ptr<std::vector<float>> cscSegment_globZ;
    std::unique_ptr<std::vector<float>> cscSegment_eta;
    std::unique_ptr<std::vector<float>> cscSegment_phi;
    std::unique_ptr<std::vector<float>> cscSegment_theta;
    std::unique_ptr<std::vector<float>> cscSegment_chi2;
    std::unique_ptr<std::vector<float>> cscSegment_time;
    std::unique_ptr<std::vector<float>> cscSegment_dirX;
    std::unique_ptr<std::vector<float>> cscSegment_dirY;
    std::unique_ptr<std::vector<float>> cscSegment_dirZ;
    std::unique_ptr<std::vector<float>> cscSegment_bend_theta;
    std::unique_ptr<std::vector<float>> cscSegment_bend_phi;
    std::unique_ptr<std::vector<int32_t>> cscSegment_endcap;
    std::unique_ptr<std::vector<int32_t>> cscSegment_ring;
    std::unique_ptr<std::vector<int32_t>> cscSegment_station;
    std::unique_ptr<std::vector<int32_t>> cscSegment_chamber;
    std::unique_ptr<std::vector<int32_t>> cscSegment_sector;
    std::unique_ptr<std::vector<int32_t>> cscSegment_CSC_ID;
    std::unique_ptr<std::vector<int32_t>> cscSegment_nRecHits;
    std::unique_ptr<std::vector<int32_t>> cscSegment_wire_min;
    std::unique_ptr<std::vector<int32_t>> cscSegment_wire_max;
    std::unique_ptr<std::vector<int32_t>> cscSegment_strip_min;
    std::unique_ptr<std::vector<int32_t>> cscSegment_strip_max;
    std::unique_ptr<std::vector<int32_t>> cscSegment_match_iHit;
    std::unique_ptr<std::vector<int32_t>> cscSegment_match_iHit2;
    std::unique_ptr<std::vector<int32_t>> cscSegment_match_nHits;
    std::unique_ptr<std::vector<int16_t>> cscSegment_match_hit_unique;
    std::unique_ptr<int32_t> cscSegment_size;
    std::unique_ptr<int32_t> cscSegment_nBx0;
};
