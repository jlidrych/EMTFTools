/* L1T analysis package 
 * module: main module of Orthogonal dataset method 
 * Author: George Karathanasis, georgios.karathanasis@cern.ch
 *
 *
 */

#include <iostream>
#include "TH1F.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TChain.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "TString.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#define SF TString::Format
// #include "analysis.h"
// #include "CutReader.h"

float DPhi(double phi1,double phi2){
  float temp=phi1-phi2;
  if (temp>3.14) temp=temp-6.28;
  if (temp<-3.14) temp=temp+6.28;
  return temp;
}

int makeMatchedNtuplesFromCRAB(){

  // TString file = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/CRAB_PrivateMC/DisplacedMuGun_flatPt2to1000_posEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv6_2M/210421_153714/DisplacedMuGun_flatPt2to1000_posEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv6_2M.root";
  // TString file = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/CRAB_PrivateMC/DisplacedMuGun_flatPt2to1000_negEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv6_2M/210421_153922/DisplacedMuGun_flatPt2to1000_negEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv6_2M.root";
  // TString file = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/CRAB_PrivateMC/DisplacedMuGun_flatPt2to1000_negEndcap_flatXYZEtaPhi_11_3_0_pre5_10M/210413_105349/DisplacedMuGun_flatPt2to1000_negEndcap_flatXYZEtaPhi_11_3_0_pre5_10M.root";
  // TString file = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/CRAB_PrivateMC/DisplacedMuGun_flatPt2to1000_negEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv7_1M/210503_111501/DisplacedMuGun_flatPt2to1000_negEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv7_1M.root";
  // TString file = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/CRAB_PrivateMC/DisplacedMuGun_flatPt2to1000_posEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv7_1M/210503_111437/DisplacedMuGun_flatPt2to1000_posEndcap_flatXYZEtaPhi_11_3_0_pre5_NNv7_1M.root";
  // TString file = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_TuneCP5_14TeV_pythia8/HTo2LongLivedTo4mu_MH-125_MFF-50_CTau-3000mm_NNv6/210427_070220/0000/EMTFNtuple_1.root";
  // TString file = "/eos/cms/store/user/eyigitba/emtf/L1Ntuples/Run3/crabOut/HTo2LongLivedTo4mu_MH-125_MFF-25_CTau-1500mm_TuneCP5_14TeV_pythia8/HTo2LongLivedTo4mu_MH-125_MFF-25_CTau-1500mm_NNv6/210503_172337/HTo2LongLivedTo4mu_MH-125_MFF-25_CTau-1500mm_NNv6.root";
  //  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY-fix-v1/220225_150602/0000/*.root";
  //  TString file1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY-fix-v1/220225_150602/0001/*.root";
  //TString file2 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY-fix-v1/220225_150602/0002/*.root";
  //TString file3 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY-fix-v1/220225_150602/0003/*.root";



  //    TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220411_165523/0000/*.root";

  //  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220518_122404/0000/*.root";
  // TString file1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220518_122404/0001/*.root";
  // TString file2 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220518_122404/0002/*.root";

  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220530_122150/0000/*.root";
  TString file1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220530_122150/0001/*.root";
  TString file2 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220530_122150/0002/*.root";
  //  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220309_175727/0000/*.root";
  // load trees
  TString tree = "EMTFNtuple/tree";

  TFile *fout =new TFile("/nfs/dust/cms/user/lidrychj/EMTF/Emulator/CMSSW_12_3_0_pre4/src/matchedNtuple.root","RECREATE");
  TTree * t1 =new TTree("tree","tree");


    TChain * cc=new TChain(tree);

  //  for(int i = 1; i<102; ++i){
    //    cc->Add(SF("/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValZMM_14/MC-ZMM/220214_100256/0000/EMTFNtuple_%d.root",i));
    //    cc->Add(SF("/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValMinBias_14TeV/MC-MB/220214_113722/0000/EMTFNtuple_%d.root",i));
    //    cc->Add(SF("/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValZMM_14/MC-ZMM/220214_123131/0000/EMTFNtuple_%d.root",i));
    //    cc->Add(SF("/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValMinBias_14TeV/MC-MB/220214_123148/0000/EMTFNtuple_%d.root",i));
  // }

    cc->Add(file0);
    cc->Add(file1);
    cc->Add(file2);
  //cc->Add(file3);

  TTreeReader reader(cc);

  TTreeReaderValue<int32_t> emtfTrackSize(reader,"emtfTrack_size");
  TTreeReaderArray<float  > emtfTrackPt(reader,"emtfTrack_pt");
  TTreeReaderArray<float  > emtfTrackPtDxy(reader,"emtfTrack_pt_dxy");
  TTreeReaderArray<float  > emtfTrackDxy(reader,"emtfTrack_dxy");
  TTreeReaderArray<float  > emtfTrackPhi(reader,"emtfTrack_phi");
  TTreeReaderArray<float  > emtfTrackTheta(reader,"emtfTrack_theta");
  TTreeReaderArray<float  > emtfTrackEta(reader,"emtfTrack_eta");
  TTreeReaderArray<int    > emtfTrackGMTPhi(reader,"emtfTrack_GMT_phi");
  TTreeReaderArray<int    > emtfTrackGMTEta(reader,"emtfTrack_GMT_eta");
  TTreeReaderArray<short  > emtfTrackMode(reader,"emtfTrack_mode");
  TTreeReaderArray<short  > emtfTrackEndcap(reader,"emtfTrack_endcap");
  TTreeReaderArray<short  > emtfTrackSector(reader,"emtfTrack_sector");
  TTreeReaderArray<short  > emtfTrackBX(reader,"emtfTrack_bx");


  TTreeReaderValue<int32_t> gmtMuonSize(reader,"gmtMuon_size");
  TTreeReaderArray<float  > gmtMuonPt(reader,"gmtMuon_pt");
  TTreeReaderArray<float  > gmtMuonPtDxy(reader,"gmtMuon_pt_dxy");
  // TTreeReaderArray<float  > gmtMuonPtDxyNN(reader,"gmtMuon_pt_dxyNN");
  TTreeReaderArray<short  > gmtMuonDxy(reader,"gmtMuon_dxy");
  // TTreeReaderArray<float  > gmtMuonDxyNN(reader,"gmtMuon_dxyNN");
  TTreeReaderArray<float  > gmtMuonPhi(reader,"gmtMuon_phi");
  TTreeReaderArray<float  > gmtMuonEta(reader,"gmtMuon_eta");
  TTreeReaderArray<short  > gmtMuonCharge(reader,"gmtMuon_q");
  TTreeReaderArray<short  > gmtMuonQual(reader,"gmtMuon_qual");


  TTreeReaderArray<short  > genPartCharge(reader,"genPart_q");
  TTreeReaderArray<float  > genPartPt(reader,"genPart_pt");
  TTreeReaderArray<float  > genPartDxy(reader,"genPart_dxy");
  TTreeReaderArray<float  > genPartEta(reader,"genPart_eta");
  TTreeReaderArray<float  > genPartPhi(reader,"genPart_phi");
  TTreeReaderArray<short  > genPartID(reader,"genPart_ID");
  TTreeReaderArray<int32_t> genPartParentID(reader,"genPart_parentID");
  TTreeReaderArray<float  > genPartVx(reader,"genPart_vx");
  TTreeReaderArray<float  > genPartVy(reader,"genPart_vy");
  TTreeReaderArray<float  > genPartVz(reader,"genPart_vz");

  std::vector<float> l1_pt, l1_ptDxy, l1_eta, l1_phi, l1_qual, l1_dxyNN,
                     gen_pt, gen_eta, gen_phi, gen_etaStar, gen_phiStar, gen_Lxy, gen_dR, gen_vx, gen_vy, gen_vz, gen_d0,
                     gendimu_pt, gendimu_eta, gendimu_phi, gendimu_Lxy, gendimu_vz;
  std::vector<int>   gen_parent, gen_idx, gen_charge, gen_matchedL1Mu, l1_dxy, l1_charge, l1_emtfMode, gendimu_daughter1, gendimu_daughter2;

  t1->Branch("l1_pt",& l1_pt);
  t1->Branch("l1_ptDxy",& l1_ptDxy);
  t1->Branch("l1_eta",& l1_eta);
  t1->Branch("l1_phi",& l1_phi);
  t1->Branch("l1_qual",& l1_qual);
  t1->Branch("l1_dxyNN",& l1_dxyNN);
  t1->Branch("l1_dxy",& l1_dxy);
  t1->Branch("l1_charge",& l1_charge);
  t1->Branch("l1_emtfMode",& l1_emtfMode);

  t1->Branch("gen_pt",&gen_pt);
  t1->Branch("gen_eta",&gen_eta);
  t1->Branch("gen_etaStar",&gen_etaStar);
  t1->Branch("gen_phi",&gen_phi);
  t1->Branch("gen_phiStar",&gen_phiStar);
  t1->Branch("gen_Lxy",&gen_Lxy);
  t1->Branch("gen_dR",&gen_dR);
  t1->Branch("gen_vx",&gen_vx);
  t1->Branch("gen_vy",&gen_vy);
  t1->Branch("gen_vz",&gen_vz);
  t1->Branch("gen_d0",&gen_d0);
  t1->Branch("gen_parent",&gen_parent);
  t1->Branch("gen_idx",&gen_idx);
  t1->Branch("gen_charge",&gen_charge);
  t1->Branch("gen_matchedL1Mu",&gen_matchedL1Mu);

  t1->Branch("gendimu_pt",&gendimu_pt);
  t1->Branch("gendimu_eta",&gendimu_eta);
  t1->Branch("gendimu_phi",&gendimu_phi);
  t1->Branch("gendimu_Lxy",&gendimu_Lxy);
  t1->Branch("gendimu_vz",&gendimu_vz);
  t1->Branch("gendimu_daughter1",&gendimu_daughter1);
  t1->Branch("gendimu_daughter2",&gendimu_daughter2);




  int eventCount = 0;
  int matchedCount = 0;
  int matchedEMTF = 0;

  float z_ME2 = 808.0; // ME2 z coordinate in [cm]

  float r = 0.;
  float rStar = 0.;
  
  float etaStar_gen = 0.;

  float phiStar_gen = 0.;

  float dR, l1mu_idx, dR_EMTF, idx_EMTF;

  //  cout<<"cc->GetEntries(): "<<cc->GetEntries()<<endl;
  while(reader.Next()){
    eventCount++;
    if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
    // std::cout << eventCount << " events read!" << std::endl;
    if(eventCount==2000000){
      break;
    }
    // clear stuff
    l1_pt.clear();
    l1_ptDxy.clear();
    l1_eta.clear();
    l1_phi.clear();
    l1_qual.clear();
    l1_charge.clear();
    l1_emtfMode.clear();
    l1_dxy.clear();
    l1_dxyNN.clear();

    gen_pt.clear();
    gen_eta.clear();
    gen_etaStar.clear();
    gen_phi.clear();
    gen_phiStar.clear();
    gen_Lxy.clear();
    gen_dR.clear();
    gen_vx.clear();
    gen_vy.clear();
    gen_vz.clear();
    gen_parent.clear();
    gen_idx.clear();
    gen_matchedL1Mu.clear();
    gen_d0.clear();
    gen_charge.clear();

    gendimu_pt.clear();
    gendimu_Lxy.clear();
    gendimu_eta.clear();
    gendimu_phi.clear();
    gendimu_vz.clear();
    gendimu_daughter1.clear();
    gendimu_daughter2.clear();


    // int genMuon = 0;
    // loop over GEN muons
    for (int i=0; i<genPartPt.GetSize(); i++){

      if(abs(genPartID[i]) != 13 ) continue;

      // genMuon++;
      // std::cout << gmtMuon << " gmt muon" << std::endl;

      // cpnvert displaced eta and phi to prompt-like and calculate dR between gen and l1 muon
      if (genPartEta[i] > 0) r = abs(z_ME2 - genPartVz[i])/abs(TMath::SinH(genPartEta[i]));
      else r = abs(-z_ME2 - genPartVz[i])/abs(TMath::SinH(genPartEta[i]));      
      float xStar = genPartVx[i] + r * TMath::Cos(genPartPhi[i]);
      float yStar = genPartVy[i] + r * TMath::Sin(genPartPhi[i]);
      rStar = TMath::Sqrt(xStar * xStar + yStar * yStar);
      
      etaStar_gen = TMath::ASinH(z_ME2/rStar) * (genPartEta[i]/abs(genPartEta[i]));

      // if (abs(etaStar_gen) < 1.2 || abs(etaStar_gen) > 2.5 ) continue;

      if (xStar >= 0) phiStar_gen = TMath::ATan(yStar/xStar); 
      else if (yStar >= 0 && xStar < 0) phiStar_gen = TMath::Pi() + TMath::ATan(yStar/xStar); 
      else if (yStar <= 0 && xStar < 0) phiStar_gen = TMath::ATan(yStar/xStar) - TMath::Pi(); 

      float d0 = -999.0;
      float invPt = genPartCharge[i]/genPartPt[i];
      invPt = (std::abs(invPt) < 1./10000) ? (invPt < 0 ? -1./10000 : +1./10000) : invPt;
      double R = -1.0 / (0.003 * 3.811 * invPt);
      float xc = genPartVx[i] - (R * std::sin(genPartPhi[i]));
      float yc = genPartVy[i] + (R * std::cos(genPartPhi[i]));
      d0 = R - ((R < 0 ? -1. : +1.) * TMath::Sqrt(xc*xc + yc*yc));

      float Lxy = TMath::Sqrt(genPartVx[i]*genPartVx[i] + genPartVy[i]*genPartVy[i]);


      gen_pt.push_back(genPartPt[i]);
      gen_eta.push_back(genPartEta[i]);
      gen_phi.push_back(genPartPhi[i]);
      gen_Lxy.push_back(Lxy);
      gen_etaStar.push_back(etaStar_gen);
      gen_phiStar.push_back(phiStar_gen);
      gen_vx.push_back(genPartVx[i]);
      gen_vy.push_back(genPartVy[i]);
      gen_vz.push_back(genPartVz[i]);
      gen_parent.push_back(genPartParentID[i]);
      gen_idx.push_back(i);
      gen_d0.push_back(d0);
      gen_charge.push_back(genPartCharge[i]);


      // GEN muon GMT muon matching and GMT muon EMTF track matching
      dR = 5.0;
      l1mu_idx = -99;
      for (unsigned int j=0; j<*gmtMuonSize; j++){
        if (abs(gmtMuonEta[i]) < 1.2) continue;

        float dR_new = TMath::Sqrt((gmtMuonEta[j]-genPartEta[i])*(gmtMuonEta[j]-genPartEta[i])+DPhi(gmtMuonPhi[j],genPartPhi[i])*DPhi(gmtMuonPhi[j],genPartPhi[i]));
        // std::cout << "dR GEN: " << dR_new << std::endl;
        if(dR_new > dR){
          continue;
        }
        else{
          l1mu_idx = j;
          dR = dR_new;
        }
      }

      gen_dR.push_back(dR);
      gen_matchedL1Mu.push_back(l1mu_idx);
    } 


    // Find EMTF tracks matched to GMT muon and fill L1 muon info 
    for (int i=0; i<*gmtMuonSize; i++){
      // if(abs(l1mueta[i]) < 1.24 or abs(l1mueta[i]) > 2.5) continue;

      dR_EMTF = 5.0;
      idx_EMTF = -99;
      // GMT muon EMTF track matching
      for (int j=0; j<*emtfTrackSize; j++){
        if(emtfTrackBX[j] != 0) continue;

        float GMTEta = emtfTrackGMTEta[j] * 0.010875;

        int globPhi = (emtfTrackSector[j] - 1) * 96 + emtfTrackGMTPhi[j];

        globPhi = (globPhi + 600) % 576;

        float GMTPhi = globPhi * 0.010908;

        float dR_new_EMTF = TMath::Sqrt((gmtMuonEta[i]-GMTEta)*(gmtMuonEta[i]-GMTEta)+DPhi(gmtMuonPhi[i],GMTPhi)*DPhi(gmtMuonPhi[i],GMTPhi));
        // std::cout << "dR EMTF: " << dR_new_EMTF << std::endl;

        if(dR_new_EMTF > dR_EMTF){
          continue;
        }
        else{
          idx_EMTF = j;
          dR_EMTF = dR_new_EMTF;
        }
      }

      if (idx_EMTF < 0){
        l1_pt.push_back(-9999);
        l1_ptDxy.push_back(-9999);
        l1_eta.push_back(-9999);
        l1_phi.push_back(-9999);
        l1_qual.push_back(-9999);
        l1_dxy.push_back(-9999);
        l1_dxyNN.push_back(-9999);
        l1_charge.push_back(int(-9999));
        l1_emtfMode.push_back(-9999);
      } 

      else{
        int GMT_dxy = -1;

        if (abs(emtfTrackDxy[idx_EMTF]) < 25){
          GMT_dxy = 0;
        }
        else if (abs(emtfTrackDxy[idx_EMTF]) < 50){
          GMT_dxy = 1;
        }
        else if (abs(emtfTrackDxy[idx_EMTF]) < 75){
          GMT_dxy = 2;
        }
        else {
          GMT_dxy = 3;
        }

	//	std::cout<<"emtfTrackPt[idx_EMTF]: "<<emtfTrackPt[idx_EMTF]<<" vs gmt: "<<gmtMuonPt[i]<<std::endl;
        l1_pt.push_back(gmtMuonPt[i]);//emtfTrackPt[idx_EMTF]);
        l1_ptDxy.push_back(emtfTrackPtDxy[idx_EMTF]);
        l1_eta.push_back(gmtMuonEta[i]);
        l1_phi.push_back(gmtMuonPhi[i]);
        l1_qual.push_back(gmtMuonQual[i]);
        l1_dxy.push_back(int(GMT_dxy));
        l1_dxyNN.push_back(emtfTrackDxy[idx_EMTF]);
        l1_charge.push_back(int(gmtMuonCharge[i]));
        l1_emtfMode.push_back(int(emtfTrackMode[idx_EMTF]));
      }
    }

    t1->Fill();

  } // end event loop
  
  // std::cout << matchedCount << " vs " << matchedEMTF << std::endl;

  t1->Write();

  return 0;
} // end function
