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
  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValMinBias_14TeV/MC-MB/220215_195151/0000/EMTFNtuple_*.root";
  //  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValNuGun/MC-NuGun/220215_133739/0000/EMTFNtuple_*.root";
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

  TH1F *h_pT = new TH1F("h_pT", "", 300, 0.5, 300.5);
  TH1F *h_pT_Eta1 = new TH1F("h_pT_Eta1", "", 300, 0.5, 300.5);
  TH1F *h_pT_Eta2 = new TH1F("h_pT_Eta2", "", 300, 0.5, 300.5);
  TH1F *h_pT_Eta3 = new TH1F("h_pT_Eta3", "", 300, 0.5, 300.5);


  int eventCount = 0;
  int nBunches = 2760; 
  float fLHC = 11.246; // khZ
  float rate;
  rate = nBunches*fLHC/cc->GetEntries();


  bool doSingleMuon = true;

  if (doSingleMuon){
    // Single muon rates 
    while(reader.Next()){
      eventCount++;
      if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
      for (int ptThreshold = 0; ptThreshold < 300; ptThreshold++){

	for (int i=0; i<*gmtMuonSize; i++){
	  bool passedMuon = false;

	  if(abs(gmtMuonEta[i]) < 1.24 || abs(gmtMuonEta[i]) > 2.5) continue;
       	  if(gmtMuonQual[i] <= 11) continue;

	  if( gmtMuonPt[i] > ptThreshold){
       	    passedMuon = true;
	    h_pT->Fill(ptThreshold,rate);
	    if (abs(gmtMuonEta[i]) > 2.1){
	      h_pT_Eta3->Fill(ptThreshold,rate);
	    }
	    else if (abs(gmtMuonEta[i]) > 1.6){
	      h_pT_Eta2->Fill(ptThreshold,rate);
	    }
	    else if (abs(gmtMuonEta[i]) > 1.2){
	      h_pT_Eta1->Fill(ptThreshold,rate);
	    }
	  }
	  if (passedMuon) break;
	}// GMT loop
      }// ptThreshold loop
    }// event loop
  }

  // canvasname.push_back("out_ptUnc_vs_dxy_wME11a"); 
  TString titlePt="L1 pT threshold [GeV]";

  h_pT->GetXaxis()->SetTitle(titlePt);
  h_pT->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT->GetXaxis()->SetRangeUser(0.4,300);
  h_pT->GetYaxis()->SetRangeUser(0.01,10000);
  h_pT->GetYaxis()->SetTitleOffset(1.3);
  h_pT->GetXaxis()->SetTitleOffset(1.3);

  h_pT_Eta1->GetXaxis()->SetTitle(titlePt);
  h_pT_Eta1->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_Eta1->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_Eta1->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_Eta1->GetYaxis()->SetTitleOffset(1.3);
  h_pT_Eta1->GetXaxis()->SetTitleOffset(1.3);

  h_pT_Eta2->GetXaxis()->SetTitle(titlePt);
  h_pT_Eta2->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_Eta2->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_Eta2->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_Eta2->GetYaxis()->SetTitleOffset(1.3);
  h_pT_Eta2->GetXaxis()->SetTitleOffset(1.3);

  h_pT_Eta3->GetXaxis()->SetTitle(titlePt);
  h_pT_Eta3->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_Eta3->GetXaxis()->SetRangeUser(0.4,300);
  h_pT_Eta3->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_Eta3->GetYaxis()->SetTitleOffset(1.3);
  h_pT_Eta3->GetXaxis()->SetTitleOffset(1.3);


  h_pT->SetLineWidth(3);
  h_pT->SetLineColor(1);

  h_pT_Eta1->SetLineWidth(3);
  h_pT_Eta1->SetLineColor(1);

  h_pT_Eta2->SetLineWidth(3);
  h_pT_Eta2->SetLineColor(1);

  h_pT_Eta3->SetLineWidth(3);
  h_pT_Eta3->SetLineColor(1);
 
  TString leg = "1.24 < |#eta| < 2.5";
  TString leg1 = "1.24 < |#eta| < 1.6";
  TString leg2 = "1.6 < |#eta| < 2.1";
  TString leg3 = "2.1 < |#eta| < 2.5";


  TCanvas * c1=new TCanvas("c1","c1",1200,1200);

  TPad* pad = new TPad("","",0.3,0.1,1,1);  

  pad->SetLeftMargin(0.3);

  gPad->SetLogy(1);
  gPad->SetLogx(1);

  // TLatex header=head();  
  // header.Draw();
  // gStyle->SetErrorY(0)
  if (doSingleMuon){
    h_pT->Draw("hist e0");    
    TLegend * leg11 =new TLegend(0.6,0.75,0.88,0.88);    
    leg11->AddEntry(h_pT,leg);
    leg11->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/TriggerRate.pdf");

    h_pT_Eta1->Draw("hist e0");
    
    TLegend * leg22 =new TLegend(0.6,0.75,0.88,0.88);    
    leg22->AddEntry(h_pT_Eta1,leg1);
    leg22->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/TriggerRate_eta1.pdf");

    h_pT_Eta2->Draw("hist e0");
    
    TLegend * leg33 =new TLegend(0.6,0.75,0.88,0.88);    
    leg33->AddEntry(h_pT_Eta2,leg2);
    leg33->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/TriggerRate_eta2.pdf");

    h_pT_Eta3->Draw("hist e");
    
    TLegend * leg44 =new TLegend(0.6,0.75,0.88,0.88);    
    leg44->AddEntry(h_pT_Eta3,leg3);
    leg44->Draw("sames");

    c1->SaveAs("./output_files/comparisons/rates/TriggerRate_eta3.pdf");
  }

  return 0;
 }
