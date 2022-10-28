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


int makeTriggerRatePlotsUnp(int gmtMuonMode){

  //read data
  //  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValMinBias_14TeV/MC-MB/220215_195151/0000/EMTFNtuple_*.root";
  //  TString ntuple ="/nfs/dust/cms/user/lidrychj/EMTF/Emulator/CMSSW_12_3_0_pre4/src/crab/merged/*.root";
  //  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValMinBias_14TeV/MC-MB/220216_002500/0000/EMTFNtuple_*.root";
  //  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValMinBias_14TeV/MC-MB/220216_101427/0000/EMTFNtuple_*.root";
  //  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValZMM_14/MC-ZMM/220214_123131/0000/EMTFNtuple_*.root";
  // TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValMinBias_14TeV/MC-MB-v1/220223_125817/0000/EMTFNtuple_*.root";
  //  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/RelValMinBias_14TeV/MC-MB-v2/220223_170818/0000/EMTFNtuple_*.root";
  //  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY-fix-v1/220225_150602/000*/EMTFNtuple_*.root";
  //  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/SingleNeutrino_Pt-2To20-gun/MC-SingleNeutrino/220224_164155/0000/EMTFNtuple_*.root";
  //TString ntuple1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/SingleNeutrino_Pt-2To20-gun/MC-SingleNeutrino/220224_164155/0001/EMTFNtuple_*.root";
  //  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/SingleNeutrino_Pt-2To20-gun/MC-SingleNeutrino/220518_122348/0000/EMTFNtuple_*.root";
  //TString ntuple1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/SingleNeutrino_Pt-2To20-gun/MC-SingleNeutrino/220518_122348/0001/EMTFNtuple_*.root";
  TString ntuple = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/SingleNeutrino_E-10-gun/MC-SingleNeutrino/220530_122133/0000/EMTFNtuple_*.root";
  TString ntuple1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/SingleNeutrino_E-10-gun/MC-SingleNeutrino/220530_122133/0001/EMTFNtuple_*.root";
  TChain * cc=new TChain("EMTFNtuple/tree");

  cc->Add(ntuple);
  cc->Add(ntuple1);
  
  TTreeReader readerGetEntries(cc);
  TTreeReader reader(cc);

 
  TTreeReaderValue<int32_t> gmtMuonSize(reader,"gmtUnpMuon_size");
  TTreeReaderArray<float  > gmtMuonPt(reader,"gmtUnpMuon_pt");
  TTreeReaderArray<float  > gmtMuonPtDxy(reader,"gmtUnpMuon_pt_dxy");
  TTreeReaderArray<float  > gmtMuonPhi(reader,"gmtUnpMuon_phi");
  TTreeReaderArray<float  > gmtMuonEta(reader,"gmtUnpMuon_eta");
  TTreeReaderArray<short  > gmtMuonQual(reader,"gmtUnpMuon_qual");
 


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

  TString outputfileName = "outputfile-rate-unp.root";

  // initialize cuts

  TH1F *h_pT = new TH1F("h_pT", "", 60, 0.5, 60.5);
  TH1F *h_pT_Eta1 = new TH1F("h_pT_Eta1", "", 60, 0.5, 60.5);
  TH1F *h_pT_Eta2 = new TH1F("h_pT_Eta2", "", 60, 0.5, 60.5);
  TH1F *h_pT_Eta3 = new TH1F("h_pT_Eta3", "", 60, 0.5, 60.5);

  TH1F *h_pT_Eta4 = new TH1F("h_pT_Eta4", "", 60, 0.5, 60.5);
  TH1F *h_pT_Eta5 = new TH1F("h_pT_Eta5", "", 60, 0.5, 60.5);
  TH1F *h_pT_Eta6 = new TH1F("h_pT_Eta6", "", 60, 0.5, 60.5);

  TH1F *h_muon_pT = new TH1F("h_muon_pT","",100, 0., 100.);
  TH1F *h_muon_eta = new TH1F("h_muon_eta","",60,-3.,3.);
  TH1F *h_muon_phi = new TH1F("h_muon_phi","",60,-3.14,3.14);
  TH2F *h_muon_eta_vs_pT = new TH2F("h_muon_eta_vs_pT","",60,-3.,3.,50,0., 50.);
  int eventCount = 0;
  int nBunches = 2760; 
  float fLHC = 11.246; // khZ
  float rate;
  float xs = 80.;//mb
  float lumi = 60; //fb-1
  cout<<"cc->GetEntries(): "<<cc->GetEntries()<<endl;
  float eff_entries = xs*lumi*pow(10,13)/cc->GetEntries();
  cout<<"eff_entries: "<<eff_entries<<endl;
  //  rate = nBunches*fLHC/eff_entries;
  rate = nBunches*fLHC/cc->GetEntries();


  bool doSingleMuon = true;

  if (doSingleMuon){
    // Single muon rates 
    while(reader.Next()){
      eventCount++;
      if (eventCount % 1000 == 0) std::cout << eventCount << " events read!" << std::endl;
      for (int ptThreshold = 0; ptThreshold < 60; ptThreshold++){

	for (int i=0; i<*gmtMuonSize; i++){
	  bool passedMuon = false;

	  if(abs(gmtMuonEta[i]) < 1.2 || abs(gmtMuonEta[i]) > 2.5) continue;
	  if(gmtMuonQual[i] < gmtMuonMode) continue;

	  if( gmtMuonPt[i] > ptThreshold){
       	    passedMuon = true;
	    h_pT->Fill(ptThreshold,rate);
	    if (abs(gmtMuonEta[i]) > 2.1){
	      h_pT_Eta3->Fill(ptThreshold,rate);
	    }
	    if (abs(gmtMuonEta[i]) > 1.6 && abs(gmtMuonEta[i]) < 2.1){
	      h_pT_Eta2->Fill(ptThreshold,rate);
	    }
	    if (abs(gmtMuonEta[i]) > 1.24 && abs(gmtMuonEta[i]) < 1.6){
	      h_pT_Eta1->Fill(ptThreshold,rate);
	    }
            if (abs(gmtMuonEta[i]) > 1.2 && abs(gmtMuonEta[i]) < 1.5){
              h_pT_Eta4->Fill(ptThreshold,rate);
            }
            if (abs(gmtMuonEta[i]) > 1.5 && abs(gmtMuonEta[i]) < 2.2){
              h_pT_Eta5->Fill(ptThreshold,rate);
            }
            if (abs(gmtMuonEta[i]) > 2.2 && abs(gmtMuonEta[i]) < 2.5){
              h_pT_Eta6->Fill(ptThreshold,rate);
            }

	  }
	  if (passedMuon) break;
	}// GMT loop
      }// ptThreshold loop

      for (int i=0; i<*gmtMuonSize; i++){
	if(abs(gmtMuonEta[i]) < 1.24 || abs(gmtMuonEta[i]) > 2.5) continue;
	if(gmtMuonQual[i] < gmtMuonMode) continue;
	h_muon_pT->Fill(gmtMuonPt[i]);
	h_muon_eta->Fill(gmtMuonEta[i]);
	h_muon_phi->Fill(gmtMuonPhi[i]);
	h_muon_eta_vs_pT->Fill(gmtMuonEta[i],gmtMuonPt[i]);
      }// GMT loop
    }// event loop
  }

  // canvasname.push_back("out_ptUnc_vs_dxy_wME11a"); 
  TString titlePt="L1 pT threshold [GeV]";

  h_pT->GetXaxis()->SetTitle(titlePt);
  h_pT->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT->GetXaxis()->SetRangeUser(0.4,60);
  h_pT->GetYaxis()->SetRangeUser(0.01,10000);
  h_pT->GetYaxis()->SetTitleOffset(1.3);
  h_pT->GetXaxis()->SetTitleOffset(1.3);

  h_pT_Eta1->GetXaxis()->SetTitle(titlePt);
  h_pT_Eta1->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_Eta1->GetXaxis()->SetRangeUser(0.4,60);
  h_pT_Eta1->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_Eta1->GetYaxis()->SetTitleOffset(1.3);
  h_pT_Eta1->GetXaxis()->SetTitleOffset(1.3);

  h_pT_Eta2->GetXaxis()->SetTitle(titlePt);
  h_pT_Eta2->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_Eta2->GetXaxis()->SetRangeUser(0.4,60);
  h_pT_Eta2->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_Eta2->GetYaxis()->SetTitleOffset(1.3);
  h_pT_Eta2->GetXaxis()->SetTitleOffset(1.3);

  h_pT_Eta3->GetXaxis()->SetTitle(titlePt);
  h_pT_Eta3->GetYaxis()->SetTitle("Rate [kHz]");
  h_pT_Eta3->GetXaxis()->SetRangeUser(0.4,60);
  h_pT_Eta3->GetYaxis()->SetRangeUser(0.1,10000);
  h_pT_Eta3->GetYaxis()->SetTitleOffset(1.3);
  h_pT_Eta3->GetXaxis()->SetTitleOffset(1.3);

  h_muon_pT->GetXaxis()->SetTitle("Muon pT [GeV]");
  h_muon_pT->GetYaxis()->SetTitle("Count");
  h_muon_pT->GetYaxis()->SetTitleOffset(1.3);
  h_muon_pT->GetXaxis()->SetTitleOffset(1.3);

  h_muon_eta->GetXaxis()->SetTitle("Muon eta");
  h_muon_eta->GetYaxis()->SetTitle("Count");
  h_muon_eta->GetYaxis()->SetTitleOffset(1.3);
  h_muon_eta->GetXaxis()->SetTitleOffset(1.3);

  h_muon_phi->GetXaxis()->SetTitle("Muon phi");
  h_muon_phi->GetYaxis()->SetTitle("Count");
  h_muon_phi->GetYaxis()->SetTitleOffset(1.3);
  h_muon_phi->GetXaxis()->SetTitleOffset(1.3);

  h_muon_eta_vs_pT->GetXaxis()->SetTitle("Muon eta");
  h_muon_eta_vs_pT->GetYaxis()->SetTitle("Muon pT");
  h_muon_eta_vs_pT->GetYaxis()->SetTitleOffset(1.3);
  h_muon_eta_vs_pT->GetXaxis()->SetTitleOffset(1.3);

  h_pT->SetLineWidth(3);
  h_pT->SetLineColor(1);

  h_pT_Eta1->SetLineWidth(3);
  h_pT_Eta1->SetLineColor(1);

  h_pT_Eta2->SetLineWidth(3);
  h_pT_Eta2->SetLineColor(1);

  h_pT_Eta3->SetLineWidth(3);
  h_pT_Eta3->SetLineColor(1);

  h_muon_pT->SetLineWidth(3);
  h_muon_pT->SetLineColor(1);

  h_muon_eta->SetLineWidth(3);
  h_muon_eta->SetLineColor(1);

  h_muon_phi->SetLineWidth(3);
  h_muon_phi->SetLineColor(1);

  TFile* outputFile = new TFile(outputfileName,"recreate");
  outputFile->cd();
  h_pT->Write();
  h_pT_Eta1->Write();
  h_pT_Eta2->Write();
  h_pT_Eta3->Write();
  h_pT_Eta4->Write();
  h_pT_Eta5->Write();
  h_pT_Eta6->Write();
  h_muon_pT->Write();
  h_muon_eta->Write();
  h_muon_phi->Write();
  h_muon_eta_vs_pT->Write();
  outputFile->Close();

 
  TString leg = "1.24 < |#eta| < 2.5";
  TString leg1 = "1.24 < |#eta| < 1.6";
  TString leg2 = "1.6 < |#eta| < 2.1";
  TString leg3 = "2.1 < |#eta| < 2.5";


  TCanvas * c1=new TCanvas("c1","c1",1200,1200);

  TPad* pad = new TPad("","",0.3,0.1,1,1);  

  pad->SetLeftMargin(0.3);

  gPad->SetLogy(1);
  gPad->SetLogx(1);
  gPad->SetGridx(1);
  gPad->SetGridy(1);
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

    gPad->SetGridx(0);
    gPad->SetGridy(0);
    //    gPad->SetLogy(0);
    gPad->SetLogx(0);
    h_muon_pT->Draw("hist e");
    c1->SaveAs("./output_files/comparisons/rates/muon_pT.pdf");
    gPad->SetLogy(0);
    h_muon_eta->Draw("hist e");
    c1->SaveAs("./output_files/comparisons/rates/muon_eta.pdf");
    h_muon_phi->Draw("hist e");
    c1->SaveAs("./output_files/comparisons/rates/muon_phi.pdf");
    h_muon_eta_vs_pT->Draw("colz");
    c1->SaveAs("./output_files/comparisons/rates/muon_eta_vs_pt.pdf");
  }

  return 0;
 }
