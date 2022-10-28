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
  
  //  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220411_165523/0000/*.root";
  // TString file1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220411_165523/0001/*.root";
  // TString file2 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220411_165523/0002/*.root";
  
  //  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220418_155624/0000/*.root";
  // TString file1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220418_155624/0001/*.root";
  //TString file2 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220418_155624/0002/*.root";
  

  //  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220518_122404/0000/*.root";
  // TString file1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220518_122404/0001/*.root";
  //TString file2 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220518_122404/0002/*.root";

  TString file0 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220530_122150/0000/*.root";
  TString file1 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220530_122150/0001/*.root";
  TString file2 = "/pnfs/desy.de/cms/tier2/store/user/jlidrych/DYToLL_M-50_TuneCP5_14TeV-pythia8/MC-DY/220530_122150/0002/*.root";

  TChain * cc=new TChain("EMTFNtuple/tree");
  cc->Add(file0);
  cc->Add(file1);
  cc->Add(file2);
  TTreeReader reader(cc);
  
  TTreeReaderArray<float  > genPartPt(reader,"genPart_pt");
  TTreeReaderArray<short  > genPartCharge(reader,"genPart_q");
  TTreeReaderArray<float  > genPartEta(reader,"genPart_eta");
  TTreeReaderArray<float  > genPartPhi(reader,"genPart_phi");
  TTreeReaderArray<short  > genPartID(reader,"genPart_ID");
  TTreeReaderArray<int32_t> genPartParentID(reader,"genPart_parentID");
  
  TTreeReaderValue<int32_t> emtfTrackSize(reader,"emtfTrack_size");
  TTreeReaderArray<float  > emtfTrackPt(reader,"emtfTrack_xml_pt");
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
  

  TH1::SetDefaultSumw2();
  // eta binned plots
  TH1F *h_resolution_eta6 = new TH1F("h_resolution_eta6", "2.2 < |#eta| < 2.5 ;(EMTF p_{T}/gen p_{T}) -1 ;", 400, -2., 2.);
  //
  TH1F *h_resolution_eta5 = new TH1F("h_resolution_eta5", "1.5 < |#eta| < 2.2;(EMTF p_{T}/gen p_{T}) -1 ;", 400, -2., 2.);
  //
  TH1F *h_resolution_eta4 = new TH1F("h_resolution_eta4", "1.2 < |#eta| < 1.5;(EMTF p_{T}/gen p_{T}) -1 ;", 400, -2., 2.);
  //
  TH1F *h_resolution_eta3 = new TH1F("h_resolution_eta3", "2.1 < |#eta| < 2.5;(EMTF p_{T}/gen p_{T}) -1 ;", 200, -1., 3.);
  TH2F *h_resolution_eta3_gen = new TH2F("h_resolution_eta3_gen", "", 24,3.,51.,200, -1., 3.);
  TH2F *h_resolution_eta3_gen_open = new TH2F("h_resolution_eta3_gen_open", "", 24,3.,51.,200, -1., 3.);
  TH2F *h_resolution_eta3_gen_double = new TH2F("h_resolution_eta3_gen_double", "", 24,3.,51.,200, -1., 3.);
  TH2F *h_resolution_eta3_gen_tight = new TH2F("h_resolution_eta3_gen_tight", "", 24,3.,51.,200, -1., 3.);
  TH1F *h_resolution_eta3_bias = new TH1F("h_resolution_eta3_bias", "", 24,3.,51.);
  TH1F *h_resolution_eta3_resolution = new TH1F("h_resolution_eta3_resolution", "", 24,3.,51.);
  TH1F *h_resolution_eta3_bias_open = new TH1F("h_resolution_eta3_bias_open", "", 24,3.,51.);
  TH1F *h_resolution_eta3_resolution_open = new TH1F("h_resolution_eta3_resolution_open", "", 24,3.,51.);
  TH1F *h_resolution_eta3_bias_double = new TH1F("h_resolution_eta3_bias_double", "", 24,3.,51.);
  TH1F *h_resolution_eta3_resolution_double = new TH1F("h_resolution_eta3_resolution_double", "", 24,3.,51.);
  TH1F *h_resolution_eta3_bias_tight = new TH1F("h_resolution_eta3_bias_tight", "", 24,3.,51.);
  TH1F *h_resolution_eta3_resolution_tight = new TH1F("h_resolution_eta3_resolution_tight", "", 24,3.,51.);
  //
  TH1F *h_resolution_eta2 = new TH1F("h_resolution_eta2", "1.6 < |#eta| < 2.1;(EMTF p_{T}/gen p_{T}) -1 ;", 200, -1., 3.);
  TH2F *h_resolution_eta2_gen = new TH2F("h_resolution_eta2_gen", "", 24,3.,51.,200, -1., 3.);
  TH2F *h_resolution_eta2_gen_open = new TH2F("h_resolution_eta2_gen_open", "", 24,3.,51.,200, -1., 3.);
  TH2F *h_resolution_eta2_gen_double = new TH2F("h_resolution_eta2_gen_double", "", 24,3.,51.,200, -1., 3.);
  TH2F *h_resolution_eta2_gen_tight = new TH2F("h_resolution_eta2_gen_tight", "", 24,3.,51.,200, -1., 3.);
  TH1F *h_resolution_eta2_bias = new TH1F("h_resolution_eta2_bias", "", 24,3.,51.);
  TH1F *h_resolution_eta2_resolution = new TH1F("h_resolution_eta2_resolution", "", 24,3.,51.);
  TH1F *h_resolution_eta2_bias_open = new TH1F("h_resolution_eta2_bias_open", "", 24,3.,51.);
  TH1F *h_resolution_eta2_resolution_open = new TH1F("h_resolution_eta2_resolution_open", "", 24,3.,51.);
  TH1F *h_resolution_eta2_bias_double = new TH1F("h_resolution_eta2_bias_double", "", 24,3.,51.);
  TH1F *h_resolution_eta2_resolution_double = new TH1F("h_resolution_eta2_resolution_double", "", 24,3.,51.);
  TH1F *h_resolution_eta2_bias_tight = new TH1F("h_resolution_eta2_bias_tight", "", 24,3.,51.);
  TH1F *h_resolution_eta2_resolution_tight = new TH1F("h_resolution_eta2_resolution_tight", "", 24,3.,51.);
  //
  TH1F *h_resolution_eta1 = new TH1F("h_resolution_eta1", "1.2 < |#eta| < 1.6;(EMTF p_{T}/gen p_{T}) -1 ;", 200, -1., 3.);
  TH2F *h_resolution_eta1_gen = new TH2F("h_resolution_eta1_gen", "", 24,3.,51.,200, -1., 3.);
  TH2F *h_resolution_eta1_gen_open = new TH2F("h_resolution_eta1_gen_open", "", 24,3.,51.,200, -1., 3.);
  TH2F *h_resolution_eta1_gen_double = new TH2F("h_resolution_eta1_gen_double", "", 24,3.,51.,200, -1., 3.);
  TH2F *h_resolution_eta1_gen_tight = new TH2F("h_resolution_eta1_gen_tight", "", 24,3.,51.,200, -1., 3.);
  TH1F *h_resolution_eta1_bias = new TH1F("h_resolution_eta1_bias", "", 24,3.,51.);
  TH1F *h_resolution_eta1_resolution = new TH1F("h_resolution_eta1_resolution", "", 24,3.,51.);
  TH1F *h_resolution_eta1_bias_open = new TH1F("h_resolution_eta1_bias_open", "", 24,3.,51.);
  TH1F *h_resolution_eta1_resolution_open = new TH1F("h_resolution_eta1_resolution_open", "", 24,3.,51.);
  TH1F *h_resolution_eta1_bias_double = new TH1F("h_resolution_eta1_bias_double", "", 24,3.,51.);
  TH1F *h_resolution_eta1_resolution_double = new TH1F("h_resolution_eta1_resolution_double", "", 24,3.,51.);
  TH1F *h_resolution_eta1_bias_tight = new TH1F("h_resolution_eta1_bias_tight", "", 24,3.,51.);
  TH1F *h_resolution_eta1_resolution_tight = new TH1F("h_resolution_eta1_resolution_tight", "", 24,3.,51.);

  //
  TH1F *h_deltaR = new TH1F("h_deltaR", "", 120, 0., 1.2);
  //
  TH1F *h_resolution = new TH1F("h_resolution", "", 350, -2., 5.);
  TH2F *h_resolution_pt = new TH2F("h_resolution_pt", "", 40, -1., 3.,250,0,250);
  TH2F *h_resolution_deltaR = new TH2F("h_resolution_deltaR", "", 40, -1., 3.,50,0,1);
  TH2F *h_resolution_mode = new TH2F("h_resolution_mode", "", 350, -2., 5.,33,-16.5,16.5);
  TH2F *h_resolution_gen_vs_reco = new TH2F("gen_vs_reco","",100,-0.5,0.5, 100,0.,0.5);

  TH1F *h_track_mode = new TH1F("track_mode","",18,-0.5,17.5);

  std::cout<<"Running on "<<cc->GetEntries()<<" evts "<<std::endl;

  int iCount = -1;

  double scale_eta3_gen_tight = 1.;
  double scale_eta2_gen_tight = 1.;
  double scale_eta1_gen_tight = 1.;

  double scale_eta3_gen_double = 1.;
  double scale_eta2_gen_double = 1.;
  double scale_eta1_gen_double = 1.;

  double scale_eta3_gen_open = 1.;
  double scale_eta2_gen_open = 1.;
  double scale_eta1_gen_open = 1.;

  double scale_eta3_gen = 1.;
  double scale_eta2_gen = 1.;
  double scale_eta1_gen = 1.;

  for(int ix = 0; ix < 2; ++ix){
    iCount= -1;
    while(reader.Next()){
      iCount++;
      if((iCount%100000)==0)
	cout<<iCount<<" events done"<<endl;

      for (int i=0; i<genPartPt.GetSize(); i++){
	if(abs(genPartID[i]) != 13 ) continue;
	if(genPartParentID[i]!=23) continue;
	
	float dR = 5.0;
	int gmt_idx = -99;
	for (int j=0; j<*emtfTrackSize; j++){
	  if(emtfTrackBX[j]!=0) continue;
	  emtfTrackPhi[j] = emtfTrackPhi[j]*3.14159/180.;
	  float dR_new = TMath::Sqrt((emtfTrackEta[j]-genPartEta[i])*(emtfTrackEta[j]-genPartEta[i])+DPhi(emtfTrackPhi[j],genPartPhi[i])*DPhi(emtfTrackPhi[j],genPartPhi[i]));
	  
	  if(dR_new < dR){
	    gmt_idx = j;
	    dR = dR_new;
	  }
	}// gmtMuonSize
	
	if(gmt_idx >= 0){
	  h_resolution_deltaR->Fill( (emtfTrackPt[gmt_idx] -genPartPt[i])/genPartPt[i], dR);
	  h_track_mode->Fill(emtfTrackMode[gmt_idx]);
	  h_track_mode->Fill(17);
	}
	h_deltaR->Fill(dR);

	if(gmt_idx >= 0 && dR < 0.2){
	  double var = emtfTrackPt[gmt_idx]/genPartPt[i];//(emtfTrackPt[gmt_idx] -genPartPt[i])/genPartPt[i];
	  
	  if(abs(genPartEta[i]) > 1.2 && abs(genPartEta[i]) < 2.5){
	    h_resolution->Fill( var);//(gmtMuonPt[gmt_idx] -genPartPt[i])/genPartPt[i]);
	    h_resolution_pt->Fill( var, genPartPt[i]);
	    h_resolution_mode->Fill(var,emtfTrackMode[gmt_idx]*emtfTrackEta[gmt_idx]/abs(emtfTrackEta[gmt_idx]));
	    h_resolution_gen_vs_reco->Fill(genPartCharge[i]/genPartPt[i],1./emtfTrackPt[gmt_idx]);
	  }
	  if(abs(genPartEta[i]) > 2.1 && abs(genPartEta[i]) < 2.5){
	    h_resolution_eta3->Fill(var);
	    h_resolution_eta3_gen->Fill(genPartPt[i],var*scale_eta3_gen);
	   if(emtfTrackMode[gmt_idx]>=4)
	     h_resolution_eta3_gen_open->Fill(genPartPt[i],var*scale_eta3_gen_open);
	   if(emtfTrackMode[gmt_idx]>=8)
	     h_resolution_eta3_gen_double->Fill(genPartPt[i],var*scale_eta3_gen_double);
	   if(emtfTrackMode[gmt_idx]>=12)
	     h_resolution_eta3_gen_tight->Fill(genPartPt[i],var*scale_eta3_gen_tight);
	  }
	  if(abs(genPartEta[i]) > 1.6 && abs(genPartEta[i]) < 2.1){
	    h_resolution_eta2->Fill(var);
	    h_resolution_eta2_gen->Fill(genPartPt[i],var*scale_eta2_gen);
	    if(emtfTrackMode[gmt_idx]>=4)
	      h_resolution_eta2_gen_open->Fill(genPartPt[i],var*scale_eta2_gen_open);
	    if(emtfTrackMode[gmt_idx]>=8)
	      h_resolution_eta2_gen_double->Fill(genPartPt[i],var*scale_eta2_gen_double);
	    if(emtfTrackMode[gmt_idx]>=12)
	      h_resolution_eta2_gen_tight->Fill(genPartPt[i],var*scale_eta2_gen_tight);
	  }
	  if(abs(genPartEta[i]) > 1.2 && abs(genPartEta[i]) < 1.6){
	    h_resolution_eta1->Fill(var);
	    h_resolution_eta1_gen->Fill(genPartPt[i],var*scale_eta1_gen);
	    if(emtfTrackMode[gmt_idx]>=4)
	      h_resolution_eta1_gen_open->Fill(genPartPt[i],var*scale_eta1_gen_open);
	    if(emtfTrackMode[gmt_idx]>=8)
	      h_resolution_eta1_gen_double->Fill(genPartPt[i],var*scale_eta1_gen_double);
	    if(emtfTrackMode[gmt_idx]>=12)
	      h_resolution_eta1_gen_tight->Fill(genPartPt[i],var*scale_eta1_gen_tight);
	  }
	
	  if(abs(genPartEta[i]) > 1.2 && abs(genPartEta[i]) < 1.5)
	    h_resolution_eta4->Fill(var);

	  if(abs(genPartEta[i]) > 1.5 && abs(genPartEta[i]) < 2.2)
	    h_resolution_eta5->Fill(var);
	  
	  if(abs(genPartEta[i]) > 2.2 && abs(genPartEta[i]) < 2.6)
	    h_resolution_eta6->Fill(var);
	}
      }
    }//reader.Next()
    if(ix==0){
      cout<<"scaling is: "<<h_resolution_eta1_gen->ProjectionY("heta1-tmp",1,24,"e")->GetMean()<<endl;
      cout<<"inv scaling: "<<1./h_resolution_eta1_gen->ProjectionY("heta1-tmp",1,24,"e")->GetMean()<<endl;

      scale_eta1_gen = 1./h_resolution_eta1_gen->ProjectionY("heta1-tmp",1,24,"e")->GetMean();
      scale_eta1_gen_open = 1./h_resolution_eta1_gen_open->ProjectionY("heta1-o-tmp",1,24,"e")->GetMean();
      scale_eta1_gen_double = 1./h_resolution_eta1_gen_double->ProjectionY("heta1-d-tmp",1,24,"e")->GetMean();
      scale_eta1_gen_tight = 1./h_resolution_eta1_gen_tight->ProjectionY("heta1-t-tmp",1,24,"e")->GetMean();

      scale_eta2_gen = 1./h_resolution_eta2_gen->ProjectionY("heta2-tmp",1,24,"e")->GetMean();
      scale_eta2_gen_open = 1./h_resolution_eta2_gen_open->ProjectionY("heta2-o-tmp",1,24,"e")->GetMean();
      scale_eta2_gen_double = 1./h_resolution_eta2_gen_double->ProjectionY("heta2-d-tmp",1,24,"e")->GetMean();
      scale_eta2_gen_tight = 1./h_resolution_eta2_gen_tight->ProjectionY("heta2-t-tmp",1,24,"e")->GetMean();

      scale_eta3_gen = 1./h_resolution_eta3_gen->ProjectionY("heta3-tmp",1,24,"e")->GetMean();
      scale_eta3_gen_open = 1./h_resolution_eta3_gen_open->ProjectionY("heta3-o-tmp",1,24,"e")->GetMean();
      scale_eta3_gen_double = 1./h_resolution_eta3_gen_double->ProjectionY("heta3-d-tmp",1,24,"e")->GetMean();
      scale_eta3_gen_tight = 1./h_resolution_eta3_gen_tight->ProjectionY("heta3-t-tmp",1,24,"e")->GetMean();

      reader.Restart();
      h_resolution->Reset("ICESM");
      h_resolution_pt->Reset("ICESM");
      h_resolution_deltaR->Reset("ICESM");
      h_resolution_eta1->Reset("ICESM");
      h_resolution_eta2->Reset("ICESM");
      h_resolution_eta3->Reset("ICESM");
      h_resolution_eta4->Reset("ICESM");
      h_resolution_eta5->Reset("ICESM");
      h_resolution_eta6->Reset("ICESM");
      h_resolution_eta3_gen->Reset("ICESM");
      h_resolution_eta2_gen->Reset("ICESM");
      h_resolution_eta1_gen->Reset("ICESM");
      h_deltaR->Reset("ICESM");
      h_resolution_mode->Reset("ICESM");
      h_resolution_gen_vs_reco->Reset("ICESM");

      h_resolution_eta3_gen_tight->Reset("ICESM");
      h_resolution_eta2_gen_tight->Reset("ICESM");
      h_resolution_eta1_gen_tight->Reset("ICESM");

      h_resolution_eta3_gen_double->Reset("ICESM");
      h_resolution_eta2_gen_double->Reset("ICESM");
      h_resolution_eta1_gen_double->Reset("ICESM");

      h_resolution_eta3_gen_open->Reset("ICESM");
      h_resolution_eta2_gen_open->Reset("ICESM");
      h_resolution_eta1_gen_open->Reset("ICESM");

      h_track_mode->Reset("ICESM");
    }
  }



  // here we will calculate bias and resolution
  for(int i=1 ; i < 25; ++i){
    TH1D* heta1 = h_resolution_eta1_gen->ProjectionY("heta1",i,i,"e");
    h_resolution_eta1_bias->SetBinContent(i,heta1->GetMean()-1);
    h_resolution_eta1_bias->SetBinError(i,heta1->GetMeanError());
    h_resolution_eta1_resolution->SetBinContent(i,heta1->GetRMS());
    h_resolution_eta1_resolution->SetBinError(i,heta1->GetRMSError());

    TH1D* heta1o = h_resolution_eta1_gen_open->ProjectionY("heta1-o",i,i,"e");
    TH1D* heta1d = h_resolution_eta1_gen_double->ProjectionY("heta1-d",i,i,"e");
    TH1D* heta1t = h_resolution_eta1_gen_tight->ProjectionY("heta1-t",i,i,"e");

    h_resolution_eta1_bias_open->SetBinContent(i,heta1o->GetMean()-1);
    h_resolution_eta1_bias_open->SetBinError(i,heta1o->GetMeanError());
    h_resolution_eta1_resolution_open->SetBinContent(i,heta1o->GetRMS());
    h_resolution_eta1_resolution_open->SetBinError(i,heta1o->GetRMSError());

    h_resolution_eta1_bias_double->SetBinContent(i,heta1d->GetMean()-1);
    h_resolution_eta1_bias_double->SetBinError(i,heta1d->GetMeanError());
    h_resolution_eta1_resolution_double->SetBinContent(i,heta1d->GetRMS());
    h_resolution_eta1_resolution_double->SetBinError(i,heta1d->GetRMSError());

    h_resolution_eta1_bias_tight->SetBinContent(i,heta1t->GetMean()-1);
    h_resolution_eta1_bias_tight->SetBinError(i,heta1t->GetMeanError());
    h_resolution_eta1_resolution_tight->SetBinContent(i,heta1t->GetRMS());
    h_resolution_eta1_resolution_tight->SetBinError(i,heta1t->GetRMSError());


    TH1D* heta2 = h_resolution_eta2_gen->ProjectionY("heta2",i,i,"e");
    h_resolution_eta2_bias->SetBinContent(i,heta2->GetMean()-1);
    h_resolution_eta2_bias->SetBinError(i,heta2->GetMeanError());
    h_resolution_eta2_resolution->SetBinContent(i,heta2->GetRMS());
    h_resolution_eta2_resolution->SetBinError(i,heta2->GetRMSError());

    TH1D* heta2o = h_resolution_eta2_gen_open->ProjectionY("heta2-o",i,i,"e");    
    TH1D* heta2d = h_resolution_eta2_gen_double->ProjectionY("heta2-d",i,i,"e");
    TH1D* heta2t = h_resolution_eta2_gen_tight->ProjectionY("heta2-t",i,i,"e");

    h_resolution_eta2_bias_open->SetBinContent(i,heta2o->GetMean()-1);
    h_resolution_eta2_bias_open->SetBinError(i,heta2o->GetMeanError());
    h_resolution_eta2_resolution_open->SetBinContent(i,heta2o->GetRMS());
    h_resolution_eta2_resolution_open->SetBinError(i,heta2o->GetRMSError());

    h_resolution_eta2_bias_double->SetBinContent(i,heta2d->GetMean()-1);
    h_resolution_eta2_bias_double->SetBinError(i,heta2d->GetMeanError());
    h_resolution_eta2_resolution_double->SetBinContent(i,heta2d->GetRMS());
    h_resolution_eta2_resolution_double->SetBinError(i,heta2d->GetRMSError());

    h_resolution_eta2_bias_tight->SetBinContent(i,heta2t->GetMean()-1);
    h_resolution_eta2_bias_tight->SetBinError(i,heta2t->GetMeanError());
    h_resolution_eta2_resolution_tight->SetBinContent(i,heta2t->GetRMS());
    h_resolution_eta2_resolution_tight->SetBinError(i,heta2t->GetRMSError());

    TH1D* heta3 = h_resolution_eta3_gen->ProjectionY("heta3",i,i,"e");
    h_resolution_eta3_bias->SetBinContent(i,heta3->GetMean()-1);
    h_resolution_eta3_bias->SetBinError(i,heta3->GetMeanError());
    h_resolution_eta3_resolution->SetBinContent(i,heta3->GetRMS());
    h_resolution_eta3_resolution->SetBinError(i,heta3->GetRMSError());

    TH1D* heta3o = h_resolution_eta3_gen_open->ProjectionY("heta3-o",i,i,"e");    
    TH1D* heta3d = h_resolution_eta3_gen_double->ProjectionY("heta3-d",i,i,"e");
    TH1D* heta3t = h_resolution_eta3_gen_tight->ProjectionY("heta3-t",i,i,"e");

    h_resolution_eta3_bias_open->SetBinContent(i,heta3o->GetMean()-1);
    h_resolution_eta3_bias_open->SetBinError(i,heta3o->GetMeanError());
    h_resolution_eta3_resolution_open->SetBinContent(i,heta3o->GetRMS());
    h_resolution_eta3_resolution_open->SetBinError(i,heta3o->GetRMSError());

    h_resolution_eta3_bias_double->SetBinContent(i,heta3d->GetMean()-1);
    h_resolution_eta3_bias_double->SetBinError(i,heta3d->GetMeanError());
    h_resolution_eta3_resolution_double->SetBinContent(i,heta3d->GetRMS());
    h_resolution_eta3_resolution_double->SetBinError(i,heta3d->GetRMSError());

    h_resolution_eta3_bias_tight->SetBinContent(i,heta3t->GetMean()-1);
    h_resolution_eta3_bias_tight->SetBinError(i,heta3t->GetMeanError());
    h_resolution_eta3_resolution_tight->SetBinContent(i,heta3t->GetRMS());
    h_resolution_eta3_resolution_tight->SetBinError(i,heta3t->GetRMSError());
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
  h_resolution_eta3_gen->Write();
  h_resolution_eta2_gen->Write();
  h_resolution_eta1_gen->Write();
  h_deltaR->Write();
  h_resolution_mode->Write();
  h_resolution_gen_vs_reco->Write();

  h_resolution_eta1_bias->Write();
  h_resolution_eta1_resolution->Write();
  h_resolution_eta2_bias->Write();
  h_resolution_eta2_resolution->Write();
  h_resolution_eta3_bias->Write();
  h_resolution_eta3_resolution->Write();

  h_resolution_eta1_bias_open->Write();
  h_resolution_eta1_resolution_open->Write();
  h_resolution_eta1_bias_double->Write();
  h_resolution_eta1_resolution_double->Write();
  h_resolution_eta1_bias_tight->Write();
  h_resolution_eta1_resolution_tight->Write();

  h_resolution_eta2_bias_open->Write();
  h_resolution_eta2_resolution_open->Write();
  h_resolution_eta2_bias_double->Write();
  h_resolution_eta2_resolution_double->Write();
  h_resolution_eta2_bias_tight->Write();
  h_resolution_eta2_resolution_tight->Write();

  h_resolution_eta3_bias_open->Write();
  h_resolution_eta3_resolution_open->Write();
  h_resolution_eta3_bias_double->Write();
  h_resolution_eta3_resolution_double->Write();
  h_resolution_eta3_bias_tight->Write();
  h_resolution_eta3_resolution_tight->Write();

  double scaling = h_track_mode->GetBinContent(18);
  if(scaling!=0)
    h_track_mode->Scale(1./scaling);
  h_track_mode->Write();
  outputFile->Close();

  return 0;
 }
