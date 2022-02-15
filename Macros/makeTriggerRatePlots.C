/* EMTF Rate Analysis plotting script
 *
 */

#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include "TLatex.h"
#include <boost/algorithm/string.hpp>
#define SF TString::Format

float DPhi(double phi1,double phi2){
  float temp=phi1-phi2;
  if (temp>3.14) temp=temp-6.28;
  if (temp<-3.14) temp=temp+6.28;
  return temp;
}

TLatex cms_latex(){
  TLatex cms_label;
  cms_label.SetTextSize(0.04);
  cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary Simulation}}");
  return cms_label;
}

TLatex head(){
  TLatex header; 
  header.SetTextSize(0.03);
  // header.DrawLatexNDC(0.63, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC");
  header.DrawLatexNDC(0.63, 0.92, "BDT");
  return header; 
}

int DefaultColor(int j,int i){
  if (j-i==1) return 2;
  else if (j-i==2) return 4;
  else if (j-i==3) return 6;
  else if (j-i==4) return 8;
  else if (j-i==5) return 9;
  else return j;
}


int makeTriggerRatePlots(){

  //read data
  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValMinBias_14TeV/MC-MB/220214_123148/0000/EMTFNtuple_*.root";

  TChain * cc=new TChain("EMTFNtuple/tree");
  cc->Add(ntuple);
  
  TTreeReader readerGetEntries(cc);
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
  // TTreeReaderArray<short  > gmtMuonDxy(reader,"gmtMuon_dxy");
  // TTreeReaderArray<float  > gmtMuonDxyNN(reader,"gmtMuon_dxyNN");
  TTreeReaderArray<float  > gmtMuonPhi(reader,"gmtMuon_phi");
  TTreeReaderArray<float  > gmtMuonEta(reader,"gmtMuon_eta");
  // TTreeReaderArray<short  > gmtMuonCharge(reader,"gmtMuon_q");
  TTreeReaderArray<short  > gmtMuonQual(reader,"gmtMuon_qual");

  gStyle->SetOptStat(0);
  gStyle->SetLegendBorderSize(0);
  gStyle->SetLegendTextSize(0.03);
  std::cout<<"Running on "<<cc->GetEntries()<<" evts "<<std::endl;

  //plot containers
  std::vector<TString> canvasname;
  std::vector<std::string> kwds;
  std::vector<TString> legs;
  std::vector<TGraphAsymmErrors*> errors;

  // cosmetic options
  std::vector<bool> grid,logY,logX;



  // initialize cuts

  TH1F *h_pT_noDxy = new TH1F("h_pT_noDxy", "", 300, 0.5, 300.5);
  TH1F *h_pT_25Dxy = new TH1F("h_pT_25Dxy", "", 300, 0.5, 300.5);
  TH1F *h_pT_50Dxy = new TH1F("h_pT_50Dxy", "", 300, 0.5, 300.5);
  TH1F *h_pT_75Dxy = new TH1F("h_pT_75Dxy", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEta1 = new TH1F("h_pT_noDxyEta1", "NNv6", 300, 0.5, 300.5);
  TH1F *h_pT_25DxyEta1 = new TH1F("h_pT_25DxyEta1", "", 300, 0.5, 300.5);
  TH1F *h_pT_50DxyEta1 = new TH1F("h_pT_50DxyEta1", "", 300, 0.5, 300.5);
  TH1F *h_pT_75DxyEta1 = new TH1F("h_pT_75DxyEta1", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEta2 = new TH1F("h_pT_noDxyEta2", "NNv6", 300, 0.5, 300.5);
  TH1F *h_pT_25DxyEta2 = new TH1F("h_pT_25DxyEta2", "", 300, 0.5, 300.5);
  TH1F *h_pT_50DxyEta2 = new TH1F("h_pT_50DxyEta2", "", 300, 0.5, 300.5);
  TH1F *h_pT_75DxyEta2 = new TH1F("h_pT_75DxyEta2", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyEta3 = new TH1F("h_pT_noDxyEta3", "NNv6", 300, 0.5, 300.5);
  TH1F *h_pT_25DxyEta3 = new TH1F("h_pT_25DxyEta3", "", 300, 0.5, 300.5);
  TH1F *h_pT_50DxyEta3 = new TH1F("h_pT_50DxyEta3", "", 300, 0.5, 300.5);
  TH1F *h_pT_75DxyEta3 = new TH1F("h_pT_75DxyEta3", "", 300, 0.5, 300.5);

  TH1F *h_pT_noDxyDimu = new TH1F("h_pT_noDxyDimu", "NNv6", 30, 0.5, 30.5);
  TH1F *h_pT_25DxyDimu = new TH1F("h_pT_25DxyDimu", "", 30, 0.5, 30.5);
  TH1F *h_pT_50DxyDimu = new TH1F("h_pT_50DxyDimu", "", 30, 0.5, 30.5);
  TH1F *h_pT_75DxyDimu = new TH1F("h_pT_75DxyDimu", "", 30, 0.5, 30.5);

  TH1F *h_pT_noDxyDimu10 = new TH1F("h_pT_noDxyDimu10", "NNv6", 30, 0.5, 30.5);
  TH1F *h_pT_25DxyDimu10 = new TH1F("h_pT_25DxyDimu10", "", 30, 0.5, 30.5);
  TH1F *h_pT_50DxyDimu10 = new TH1F("h_pT_50DxyDimu10", "", 30, 0.5, 30.5);
  TH1F *h_pT_75DxyDimu10 = new TH1F("h_pT_75DxyDimu10", "", 30, 0.5, 30.5);

  TH1F *h_pT_noDxyDimu20 = new TH1F("h_pT_noDxyDimu20", "NNv6", 30, 0.5, 30.5);
  TH1F *h_pT_25DxyDimu20 = new TH1F("h_pT_25DxyDimu20", "", 30, 0.5, 30.5);
  TH1F *h_pT_50DxyDimu20 = new TH1F("h_pT_50DxyDimu20", "", 30, 0.5, 30.5);
  TH1F *h_pT_75DxyDimu20 = new TH1F("h_pT_75DxyDimu20", "", 30, 0.5, 30.5);


  int eventCount = 0;
  int nBunches = 2760; 
  float fLHC = 11.246; // khZ
  float rate;
  rate = nBunches*fLHC/cc->GetEntries();


  bool doSingleMuon = true;
  bool doDimuon     = false;

  if (doSingleMuon){
    // Single muon rates 
    while(reader.Next()){
      eventCount++;
      if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
      for (int ptThreshold = 0; ptThreshold < 300; ptThreshold++){
	for (int i=0; i<*gmtMuonSize; i++){
	  bool passedMuon = false;
	  if(abs(gmtMuonEta[i]) < 1.24 or abs(gmtMuonEta[i]) > 2.5) continue;
	  float dR_EMTF = 99.0;
	  int idx_EMTF = -99;
	  // GMT muon EMTF track matching
	  for (int j=0; j<*emtfTrackSize; j++){
	    if(emtfTrackBX[j] != 0) continue;
	    
	    float GMTEta = emtfTrackGMTEta[j] * 0.010875;
	    int globPhi = (emtfTrackSector[j] - 1) * 96 + emtfTrackGMTPhi[j];
	    globPhi = (globPhi + 600) % 576;
	    float GMTPhi = globPhi * 0.010908;
	    float dR_new_EMTF = TMath::Sqrt((gmtMuonEta[i]-GMTEta)*(gmtMuonEta[i]-GMTEta)+DPhi(gmtMuonPhi[i],GMTPhi)*DPhi(gmtMuonPhi[i],GMTPhi));
	    if(dR_new_EMTF < dR_EMTF){
	      idx_EMTF = j;
	      dR_EMTF = dR_new_EMTF;
	    }	  
	  }

	  if (idx_EMTF < 0) continue;
	  if (emtfTrackMode[idx_EMTF] < 11) continue;

	  if(ptThreshold < emtfTrackPt[idx_EMTF]){	  
	    h_pT_noDxy->Fill(ptThreshold,rate);
	    if (abs(gmtMuonEta[i]) > 2.1){
	      h_pT_noDxyEta3->Fill(ptThreshold,rate);
	    }
	    else if (abs(gmtMuonEta[i]) > 1.6){
	      h_pT_noDxyEta2->Fill(ptThreshold,rate);
	    }
	    else if (abs(gmtMuonEta[i]) > 1.2){
	      h_pT_noDxyEta1->Fill(ptThreshold,rate);
	    }
	  }
	}// GMT loop
      }// ptThreshold loop
    }// event loop
  }



  if(doDimuon){
    // Dimuon rates 
    while(reader.Next()){
      eventCount++;
      if (eventCount % 10000 == 0) std::cout << eventCount << " events read!" << std::endl;
      for (int ptThreshold = 0; ptThreshold < 21; ptThreshold++){
        int passedMuonCount = 0;
        bool passed20 = false;
        bool passed10 = false;
        int mu1 = -1;
        int mu2 = -1;
        int mu10 = -1;
        int mu20 = -1;

        for (int i=0; i<*gmtMuonSize; i++){
          if(abs(gmtMuonEta[i]) < 1.24 or abs(gmtMuonEta[i]) > 2.5) continue;

          float dR_EMTF = 99.0;
          int idx_EMTF = -99;
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
          if (idx_EMTF < 0) continue;
          if (emtfTrackPtDxy[idx_EMTF] > 20){
            mu20 = idx_EMTF;
          }
          if (emtfTrackPtDxy[idx_EMTF] > 10){
            mu10 = idx_EMTF;
          }

          if (emtfTrackPtDxy[idx_EMTF] > ptThreshold){
            passedMuonCount++;
            if (passedMuonCount == 1) mu1 = idx_EMTF;
            if (passedMuonCount == 2) mu2 = idx_EMTF;
          }

          bool filled = false;
          if (passedMuonCount > 1 && !filled){
            filled = true;
            h_pT_noDxyDimu->Fill(ptThreshold,rate);
            
            if     ( abs(emtfTrackDxy[mu1]) > 75 && abs(emtfTrackDxy[mu2]) > 75) h_pT_75DxyDimu->Fill(ptThreshold,rate);
            else if( abs(emtfTrackDxy[mu1]) > 50 && abs(emtfTrackDxy[mu2]) > 50) h_pT_50DxyDimu->Fill(ptThreshold,rate);
            else if( abs(emtfTrackDxy[mu1]) > 25 && abs(emtfTrackDxy[mu2]) > 25) h_pT_25DxyDimu->Fill(ptThreshold,rate);

          }

          bool filled10 = false;
          if (passedMuonCount > 1 && mu10 > -1 && !filled10 && ptThreshold <= 10){
            filled10 = true;
            h_pT_noDxyDimu10->Fill(ptThreshold,rate);

            int muOther = -1;

            if (mu1 == mu10) muOther = mu2;
            else muOther = mu1;

            
            if     ( abs(emtfTrackDxy[mu10]) > 75 && abs(emtfTrackDxy[muOther]) > 75) h_pT_75DxyDimu10->Fill(ptThreshold,rate);
            else if( abs(emtfTrackDxy[mu10]) > 50 && abs(emtfTrackDxy[muOther]) > 50) h_pT_50DxyDimu10->Fill(ptThreshold,rate);
            else if( abs(emtfTrackDxy[mu10]) > 25 && abs(emtfTrackDxy[muOther]) > 25) h_pT_25DxyDimu10->Fill(ptThreshold,rate);

          }

          bool filled20 = false;
          if (passedMuonCount > 1 && mu20 > -1 && !filled20 && ptThreshold <= 20){
            filled20 = true;
            h_pT_noDxyDimu20->Fill(ptThreshold,rate);

            int muOther = -1;

            if (mu1 == mu20) muOther = mu2;
            else muOther = mu1;

            
            if     ( abs(emtfTrackDxy[mu20]) > 75 && abs(emtfTrackDxy[muOther]) > 75) h_pT_75DxyDimu20->Fill(ptThreshold,rate);
            else if( abs(emtfTrackDxy[mu20]) > 50 && abs(emtfTrackDxy[muOther]) > 50) h_pT_50DxyDimu20->Fill(ptThreshold,rate);
            else if( abs(emtfTrackDxy[mu20]) > 25 && abs(emtfTrackDxy[muOther]) > 25) h_pT_25DxyDimu20->Fill(ptThreshold,rate);

          }
          
            
        } // GMT loop
      } // pTThreshold loop
    } // event loop
  }


  // canvasname.push_back("out_ptUnc_vs_dxy_wME11a"); 
  TString titlePt="L1 pT threshold [GeV]";
  TString titleDxy="Gen Muon D_{0} [cm]";
  TString titleLxy="Gen Muon Lxy [cm]";
  TString titleEta="Gen Muon #eta";
  TString titlePhi="Gen Muon #phi";



  h_pT_noDxy->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxy->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_noDxy->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_noDxy->GetYaxis()->SetRangeUser(0.01,10000);
  h_pT_noDxy->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxy->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyEta1->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyEta1->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_noDxyEta1->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_noDxyEta1->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_noDxyEta1->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyEta1->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyEta2->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyEta2->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_noDxyEta2->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_noDxyEta2->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_noDxyEta2->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyEta2->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyEta3->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyEta3->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_noDxyEta3->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_noDxyEta3->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_noDxyEta3->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyEta3->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyDimu->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyDimu->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_noDxyDimu->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_noDxyDimu->GetYaxis()->SetRangeUser(0.001,1000);
  h_pT_noDxyDimu->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyDimu->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyDimu10->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyDimu10->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_noDxyDimu10->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_noDxyDimu10->GetYaxis()->SetRangeUser(0.001,1000);
  h_pT_noDxyDimu10->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyDimu10->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxyDimu20->GetXaxis()->SetTitle(titlePt);
  h_pT_noDxyDimu20->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_noDxyDimu20->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_noDxyDimu20->GetYaxis()->SetRangeUser(0.001,1000);
  h_pT_noDxyDimu20->GetYaxis()->SetTitleOffset(1.3);
  h_pT_noDxyDimu20->GetXaxis()->SetTitleOffset(1.3);

  h_pT_noDxy->SetLineWidth(3);
  h_pT_noDxy->SetLineColor(1);

  h_pT_25Dxy->SetLineWidth(3);
  h_pT_25Dxy->SetLineColor(2);

  h_pT_50Dxy->SetLineWidth(3);
  h_pT_50Dxy->SetLineColor(4);

  h_pT_75Dxy->SetLineWidth(3);
  h_pT_75Dxy->SetLineColor(6);

  h_pT_noDxyEta1->SetLineWidth(3);
  h_pT_noDxyEta1->SetLineColor(1);

  h_pT_25DxyEta1->SetLineWidth(3);
  h_pT_25DxyEta1->SetLineColor(2);

  h_pT_50DxyEta1->SetLineWidth(3);
  h_pT_50DxyEta1->SetLineColor(4);

  h_pT_75DxyEta1->SetLineWidth(3);
  h_pT_75DxyEta1->SetLineColor(6);

  h_pT_noDxyEta2->SetLineWidth(3);
  h_pT_noDxyEta2->SetLineColor(1);

  h_pT_25DxyEta2->SetLineWidth(3);
  h_pT_25DxyEta2->SetLineColor(2);

  h_pT_50DxyEta2->SetLineWidth(3);
  h_pT_50DxyEta2->SetLineColor(4);

  h_pT_75DxyEta2->SetLineWidth(3);
  h_pT_75DxyEta2->SetLineColor(6);

  h_pT_noDxyEta3->SetLineWidth(3);
  h_pT_noDxyEta3->SetLineColor(1);

  h_pT_25DxyEta3->SetLineWidth(3);
  h_pT_25DxyEta3->SetLineColor(2);

  h_pT_50DxyEta3->SetLineWidth(3);
  h_pT_50DxyEta3->SetLineColor(4);

  h_pT_75DxyEta3->SetLineWidth(3);
  h_pT_75DxyEta3->SetLineColor(6);

  h_pT_noDxyDimu->SetLineWidth(3);
  h_pT_noDxyDimu->SetLineColor(1);

  h_pT_25DxyDimu->SetLineWidth(3);
  h_pT_25DxyDimu->SetLineColor(2);

  h_pT_50DxyDimu->SetLineWidth(3);
  h_pT_50DxyDimu->SetLineColor(4);

  h_pT_75DxyDimu->SetLineWidth(3);
  h_pT_75DxyDimu->SetLineColor(6);

  h_pT_noDxyDimu10->SetLineWidth(3);
  h_pT_noDxyDimu10->SetLineColor(1);

  h_pT_25DxyDimu10->SetLineWidth(3);
  h_pT_25DxyDimu10->SetLineColor(2);

  h_pT_50DxyDimu10->SetLineWidth(3);
  h_pT_50DxyDimu10->SetLineColor(4);

  h_pT_75DxyDimu10->SetLineWidth(3);
  h_pT_75DxyDimu10->SetLineColor(6);

  h_pT_noDxyDimu20->SetLineWidth(3);
  h_pT_noDxyDimu20->SetLineColor(1);

  h_pT_25DxyDimu20->SetLineWidth(3);
  h_pT_25DxyDimu20->SetLineColor(2);

  h_pT_50DxyDimu20->SetLineWidth(3);
  h_pT_50DxyDimu20->SetLineColor(4);

  h_pT_75DxyDimu20->SetLineWidth(3);
  h_pT_75DxyDimu20->SetLineColor(6);

 
  TString leg = "1.24 < |#eta| < 2.5";
  TString leg2 = "|L1 Dxy| > 25 cm";
  TString leg3 = "|L1 Dxy| > 50 cm";
  TString leg4 = "|L1 Dxy| > 75 cm";


  TCanvas * c1=new TCanvas("c1","c1",1200,1200);

  TPad* pad = new TPad("","",0.3,0.1,1,1);  

  pad->SetLeftMargin(0.3);

  gPad->SetLogy(1);
  gPad->SetLogx(1);

  // TLatex header=head();  
  // header.Draw();
  // gStyle->SetErrorY(0)
  if (doSingleMuon){
    h_pT_noDxy->Draw("hist e0");
    //  h_pT_25Dxy->Draw("hist same");
    //h_pT_50Dxy->Draw("hist same");
    //h_pT_75Dxy->Draw("hist same");
    
    TLegend * leg11 =new TLegend(0.6,0.75,0.88,0.88);    
    leg11->AddEntry(h_pT_noDxy,leg);
    //leg11->AddEntry(h_pT_25Dxy,leg2);
    //leg11->AddEntry(h_pT_50Dxy,leg3);
    //leg11->AddEntry(h_pT_75Dxy,leg4);
    leg11->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/TriggerRate.pdf");

    h_pT_noDxyEta1->Draw("hist");
    h_pT_25DxyEta1->Draw("hist same");
    h_pT_50DxyEta1->Draw("hist same");
    h_pT_75DxyEta1->Draw("hist same");
    
    TLegend * leg22 =new TLegend(0.6,0.75,0.88,0.88);    
    leg22->AddEntry(h_pT_noDxyEta1,leg);
    leg22->AddEntry(h_pT_25DxyEta1,leg2);
    leg22->AddEntry(h_pT_50DxyEta1,leg3);
    leg22->AddEntry(h_pT_75DxyEta1,leg4);
    leg22->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/TriggerRate_eta1.pdf");

    h_pT_noDxyEta2->Draw("hist");
    h_pT_25DxyEta2->Draw("hist same");
    h_pT_50DxyEta2->Draw("hist same");
    h_pT_75DxyEta2->Draw("hist same");
    
    TLegend * leg33 =new TLegend(0.6,0.75,0.88,0.88);    
    leg33->AddEntry(h_pT_noDxyEta2,leg);
    leg33->AddEntry(h_pT_25DxyEta2,leg2);
    leg33->AddEntry(h_pT_50DxyEta2,leg3);
    leg33->AddEntry(h_pT_75DxyEta2,leg4);
    leg33->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/TriggerRate_eta2.pdf");

    h_pT_noDxyEta3->Draw("hist");
    h_pT_25DxyEta3->Draw("hist same");
    h_pT_50DxyEta3->Draw("hist same");
    h_pT_75DxyEta3->Draw("hist same");
    
    TLegend * leg44 =new TLegend(0.6,0.75,0.88,0.88);    
    leg44->AddEntry(h_pT_noDxyEta3,leg);
    leg44->AddEntry(h_pT_25DxyEta3,leg2);
    leg44->AddEntry(h_pT_50DxyEta3,leg3);
    leg44->AddEntry(h_pT_75DxyEta3,leg4);
    leg44->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/TriggerRate_eta3.pdf");
  }

  if (doDimuon){
    h_pT_noDxyDimu->Draw("hist");
    h_pT_25DxyDimu->Draw("hist same");
    h_pT_50DxyDimu->Draw("hist same");
    h_pT_75DxyDimu->Draw("hist same");
    
    TLegend * leg11 =new TLegend(0.6,0.75,0.88,0.88);    
    leg11->AddEntry(h_pT_noDxyDimu,leg);
    leg11->AddEntry(h_pT_25DxyDimu,leg2);
    leg11->AddEntry(h_pT_50DxyDimu,leg3);
    leg11->AddEntry(h_pT_75DxyDimu,leg4);
    leg11->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/dimuon/NuGun_AdditionalRate_NNv6.pdf");

    h_pT_noDxyDimu10->Draw("hist");
    h_pT_25DxyDimu10->Draw("hist same");
    h_pT_50DxyDimu10->Draw("hist same");
    h_pT_75DxyDimu10->Draw("hist same");
    
    TLegend * leg22 =new TLegend(0.6,0.75,0.88,0.88);    
    leg22->AddEntry(h_pT_noDxyDimu10,leg);
    leg22->AddEntry(h_pT_25DxyDimu10,leg2);
    leg22->AddEntry(h_pT_50DxyDimu10,leg3);
    leg22->AddEntry(h_pT_75DxyDimu10,leg4);
    leg22->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/dimuon/NuGun_asym10_AdditionalRate_NNv6.pdf");

    h_pT_noDxyDimu20->Draw("hist");
    h_pT_25DxyDimu20->Draw("hist same");
    h_pT_50DxyDimu20->Draw("hist same");
    h_pT_75DxyDimu20->Draw("hist same");
    
    TLegend * leg33 =new TLegend(0.6,0.75,0.88,0.88);    
    leg33->AddEntry(h_pT_noDxyDimu20,leg);
    leg33->AddEntry(h_pT_25DxyDimu20,leg2);
    leg33->AddEntry(h_pT_50DxyDimu20,leg3);
    leg33->AddEntry(h_pT_75DxyDimu20,leg4);
    leg33->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/dimuon/NuGun_asym20_AdditionalRate_NNv6.pdf");
  }


  return 0;
 }
