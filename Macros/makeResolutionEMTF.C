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


int makeResolutionEMTF(){

  //read data
  //  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/SingleNeutrino_Pt-2To20-gun/MC-SingleNeutrino/220411_165541/0000/EMTFNtuple_*.root";
  //TString file1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/SingleNeutrino_Pt-2To20-gun/MC-SingleNeutrino/220411_165541/0001/EMTFNtuple_*.root";

  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220411_165523/0000/*.root";
  TChain * cc=new TChain("EMTFNtuple/tree");
  cc->Add(file0);
  //cc->Add(file1);
  TTreeReader reader(cc);
  
  TTreeReaderValue<int32_t> gmtMuonSize(reader,"gmtMuon_size");
  TTreeReaderArray<float  > gmtMuonPt(reader,"gmtMuon_pt");
  TTreeReaderArray<float  > gmtMuonPtDxy(reader,"gmtMuon_pt_dxy");
  TTreeReaderArray<float  > gmtMuonPhi(reader,"gmtMuon_phi");
  TTreeReaderArray<float  > gmtMuonEta(reader,"gmtMuon_eta");
  TTreeReaderArray<short  > gmtMuonQual(reader,"gmtMuon_qual");
  TTreeReaderArray<short  > gmtMuonCharge(reader,"gmtMuon_q");

  TTreeReaderValue<int32_t> gmtUnpMuonSize(reader,"gmtUnpMuon_size");
  TTreeReaderArray<float  > gmtUnpMuonPt(reader,"gmtUnpMuon_pt");
  TTreeReaderArray<float  > gmtUnpMuonPtDxy(reader,"gmtUnpMuon_pt_dxy");
  TTreeReaderArray<float  > gmtUnpMuonPhi(reader,"gmtUnpMuon_phi");
  TTreeReaderArray<float  > gmtUnpMuonEta(reader,"gmtUnpMuon_eta");
  TTreeReaderArray<short  > gmtUnpMuonQual(reader,"gmtUnpMuon_qual");
  TTreeReaderArray<short  > gmtUnpMuonCharge(reader,"gmtUnpMuon_q");

  TTreeReaderArray<float  > genPartPt(reader,"genPart_pt");
  TTreeReaderArray<short  > genPartCharge(reader,"genPart_q");
  TTreeReaderArray<float  > genPartEta(reader,"genPart_eta");
  TTreeReaderArray<float  > genPartPhi(reader,"genPart_phi");
  TTreeReaderArray<short  > genPartID(reader,"genPart_ID");
  TTreeReaderArray<int32_t> genPartParentID(reader,"genPart_parentID");

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

  /*  
  TTreeReaderValue<int32_t> gmtMuonSize(reader,"gmtUnpMuon_size");
  TTreeReaderArray<float  > gmtMuonPt(reader,"gmtUnpMuon_pt");
  TTreeReaderArray<float  > gmtMuonPtDxy(reader,"gmtUnpMuon_pt_dxy");
  TTreeReaderArray<float  > gmtMuonPhi(reader,"gmtUnpMuon_phi");
  TTreeReaderArray<float  > gmtMuonEta(reader,"gmtUnpMuon_eta");
  TTreeReaderArray<short  > gmtMuonQual(reader,"gmtUnpMuon_qual");
  TTreeReaderArray<short  > gmtMuonCharge(reader,"gmtUnpMuon_q");
*/  

  TH1::SetDefaultSumw2();
  // eta binned plots
  TH1F *h_resolution_eta6 = new TH1F("h_resolution_eta6", "", 350, -2., 5.);
  //
  TH1F *h_resolution_eta5 = new TH1F("h_resolution_eta5", "", 350, -2., 5.);
  //
  TH1F *h_resolution_eta4 = new TH1F("h_resolution_eta4", "", 350, -2., 5.);
  //
  TH1F *h_resolution_eta3 = new TH1F("h_resolution_eta3", "", 350, -2., 5.);
  //
  TH1F *h_resolution_eta2 = new TH1F("h_resolution_eta2", "", 350, -2., 5.);
  //
  TH1F *h_resolution_eta1 = new TH1F("h_resolution_eta1", "", 350, -2., 5.);
  //
  TH1F *h_deltaR = new TH1F("h_deltaR", "", 120, 0., 1.2);
  //
  TH1F *h_resolution = new TH1F("h_resolution", "", 350, -2., 5.);
  TH2F *h_resolution_pt = new TH2F("h_resolution_pt", "", 40, -1., 3.,250,0,1);
  TH2F *h_resolution_deltaR = new TH2F("h_resolution_deltaR", "", 40, -1., 3.,50,0,1);
  TH2F *h_resolution_mode = new TH2F("h_resolution_mode", "", 350, -2., 5.,33,-16.5,16.5);


  std::cout<<"Running on "<<cc->GetEntries()<<" evts "<<std::endl;
  
  while(reader.Next()){
    /*
    cout<<"next event..."<<endl;
    cout<<"Unp GMT muons: "<<endl;
    for(int i = 0; i < *gmtUnpMuonSize; ++i){
      if(abs(gmtUnpMuonEta[i]) > 1.3 && abs(gmtUnpMuonEta[i]) < 2.6)
	cout<<"pt, eta, phi: "<<gmtUnpMuonPt[i]<<" "<<gmtUnpMuonEta[i]<<" "<<gmtUnpMuonPhi[i]<<endl;
    }

    cout<<"GMT muons: "<<endl;
    for(int i = 0; i < *gmtMuonSize; ++i){
      if(abs(gmtMuonEta[i]) > 1.3 && abs(gmtMuonEta[i]) < 2.6)
	cout<<"pt, eta, phi: "<<gmtMuonPt[i]<<" "<<gmtMuonEta[i]<<" "<<gmtMuonPhi[i]<<endl;
    }
     
    cout<<"EMTF tracks: "<<endl;
    for (int j=0; j<*emtfTrackSize; j++){
      if(abs(emtfTrackEta[j]) > 1.3 && abs(emtfTrackEta[j]) < 2.6){
	cout<<"pt, eta, phi: "<<emtfTrackPt[j]<<" "<<emtfTrackEta[j]<<" "<<emtfTrackPhi[j]*3.14159/180.<<endl;
      }
    }

    cout<<" gen muons: "<<endl;
    for (int i=0; i<genPartPt.GetSize(); i++){
      if(abs(genPartID[i]) != 13 ) continue;
      if(genPartParentID[i]!=23) continue;
      if(abs(genPartEta[i]) > 1.3 && abs(genPartEta[i]) < 2.6)
	cout<<"pt, eta, phi: "<<genPartPt[i]<<" "<<genPartEta[i]<<" "<<genPartPhi[i]<<endl;
    }
    continue;
  */

    for (int i=0; i<genPartPt.GetSize(); i++){
      if(abs(genPartID[i]) != 13 ) continue;
      if(genPartParentID[i]!=23) continue;

      float dR = 5.0;
      int gmt_idx = -99;
      for (int j=0; j<*emtfTrackSize; j++){
	if(emtfTrackBX[j]!=0) continue;
	//	cout<<"emtfTrackMode: "<<emtfTrackMode[j]<<endl;
	//	if(emtfTrackMode[j]<12) continue;
	//	if(gmtMuonQual[j] < 11) continue;
	emtfTrackPhi[j] = emtfTrackPhi[j]*3.14159/180.;
        float dR_new = TMath::Sqrt((emtfTrackEta[j]-genPartEta[i])*(emtfTrackEta[j]-genPartEta[i])+DPhi(emtfTrackPhi[j],genPartPhi[i])*DPhi(emtfTrackPhi[j],genPartPhi[i]));

	if(dR_new < dR){
          gmt_idx = j;
          dR = dR_new;
        }
      }// gmtMuonSize

      if(gmt_idx >= 0)
	h_resolution_deltaR->Fill( (emtfTrackPt[gmt_idx] -genPartPt[i])/genPartPt[i], dR);
      h_deltaR->Fill(dR);
      if(gmt_idx >= 0 && dR < 0.2){
	//	cout<<"pt, eta, phi: "<<genPartPt[i]<<" "<<genPartEta[i]<<" "<<genPartPhi[i]<<endl;
	//	cout<<" gmt: "<<gmtMuonPt[gmt_idx]<<" "<<gmtMuonEta[gmt_idx]<<" "<<gmtMuonPhi[gmt_idx]<<endl;

	//	double var = (genPartCharge[i]/genPartPt[i] - gmtMuonCharge[gmt_idx]/gmtMuonPt[gmt_idx]) / (genPartCharge[i]/genPartPt[i]);
	double var = (emtfTrackPt[gmt_idx] -genPartPt[i])/genPartPt[i];

	/*
	if(genPartPt[i] < 3)
	  cout<<"gen pT: "<<genPartPt[i]<<" rr "<<var<<endl;
	*/

	if(abs(genPartEta[i]) > 1.2 && abs(genPartEta[i]) < 2.5){
          h_resolution->Fill( var);//(gmtMuonPt[gmt_idx] -genPartPt[i])/genPartPt[i]);
	  h_resolution_pt->Fill( var, genPartPt[i]);
	  h_resolution_mode->Fill(var,emtfTrackMode[gmt_idx]*emtfTrackEta[gmt_idx]/abs(emtfTrackEta[gmt_idx]));

	}
	if(abs(genPartEta[i]) > 2.1 && abs(genPartEta[i]) < 2.5)
	  h_resolution_eta3->Fill(var);

	if(abs(genPartEta[i]) > 1.6 && abs(genPartEta[i]) < 2.1)
          h_resolution_eta2->Fill(var);

	if(abs(genPartEta[i]) > 1.2 && abs(genPartEta[i]) < 1.6)
          h_resolution_eta1->Fill(var);
	
	if(abs(genPartEta[i]) > 1.2 && abs(genPartEta[i]) < 1.5)
          h_resolution_eta4->Fill(var);

	if(abs(genPartEta[i]) > 1.5 && abs(genPartEta[i]) < 2.2)
	  h_resolution_eta5->Fill(var);

	if(abs(genPartEta[i]) > 2.2 && abs(genPartEta[i]) < 2.6)
	  h_resolution_eta6->Fill(var);
      }
    }
  }

  TString outputfileName = "outputfile.root";
  TFile* outputFile = new TFile(outputfileName,"recreate");
  outputFile->cd();
  h_resolution->Write();
  h_resolution_pt->Write();
  h_resolution_deltaR->Write();
  h_resolution_eta1->Write();
  h_resolution_eta2->Write();
  h_resolution_eta3->Write();
  h_resolution_eta4->Write();
  h_resolution_eta5->Write();
  h_resolution_eta6->Write();
  h_deltaR->Write();
  h_resolution_mode->Write();
  outputFile->Close();

  return 0;
 }
