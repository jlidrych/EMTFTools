/* EMTF Efficiency Analysis plotting script
 *
 */

#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include "TLatex.h"
#include <boost/algorithm/string.hpp>



TLatex cms_latex(){
  TLatex cms_label;
  cms_label.SetTextSize(0.04);
  cms_label.DrawLatexNDC(0.1, 0.92, "#bf{ #font[22]{CMS} #font[72]{Preliminary Simulation}}");
  return cms_label;
}

TLatex head(){
  TLatex header; 
  header.SetTextSize(0.03);
  header.DrawLatexNDC(0.63, 0.92, "#sqrt{s} = 13 TeV, Run 3 MC");
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


int makeTriggerEfficiencyPlots(){

  //read data
  TString ntuple = "/nfs/dust/cms/user/lidrychj/EMTF/Emulator/CMSSW_12_3_0_pre4/src/matchedNtuple.root";
                    
  TChain * cc=new TChain("tree");
  cc->Add(ntuple);

  TTreeReader ccReader(cc);

  TTreeReaderArray<float> genDiMuPt(ccReader,"gendimu_pt");
  TTreeReaderArray<float> genDiMuEta(ccReader,"gendimu_eta");
  TTreeReaderArray<float> genDiMuPhi(ccReader,"gendimu_phi");
  TTreeReaderArray<float> genDiMuLxy(ccReader,"gendimu_Lxy");
  TTreeReaderArray<float> genDiMuVz(ccReader,"gendimu_vz");
  TTreeReaderArray<int> genDiMuDaughter1(ccReader,"gendimu_daughter1");
  TTreeReaderArray<int> genDiMuDaughter2(ccReader,"gendimu_daughter2");

  TTreeReaderArray<float> genMuPt(ccReader,"gen_pt");
  TTreeReaderArray<float> genMuEta(ccReader,"gen_eta");
  TTreeReaderArray<float> genMuEtaStar(ccReader,"gen_etaStar");
  TTreeReaderArray<float> genMuPhi(ccReader,"gen_phi");
  TTreeReaderArray<float> genMuPhiStar(ccReader,"gen_phiStar");
  TTreeReaderArray<float> genMuDR(ccReader,"gen_dR");
  TTreeReaderArray<float> genMuLxy(ccReader,"gen_Lxy");
  TTreeReaderArray<float> genMuVz(ccReader,"gen_vz");
  TTreeReaderArray<float> genMuD0(ccReader,"gen_d0");
  TTreeReaderArray<int> genMuIdx(ccReader,"gen_idx");
  TTreeReaderArray<int> genMuCharge(ccReader,"gen_charge");
  TTreeReaderArray<int> genMuParent(ccReader,"gen_parent");
  TTreeReaderArray<int> genMuMatchedL1MuID(ccReader,"gen_matchedL1Mu");

  TTreeReaderArray<float> l1MuPt(ccReader,"l1_pt");
  TTreeReaderArray<float> l1MuPtDxy(ccReader,"l1_ptDxy");
  TTreeReaderArray<float> l1MuEta(ccReader,"l1_eta");
  TTreeReaderArray<float> l1MuPhi(ccReader,"l1_phi");
  TTreeReaderArray<float> l1MuQual(ccReader,"l1_qual");
  TTreeReaderArray<float> l1MuDxyNN(ccReader,"l1_dxyNN");
  TTreeReaderArray<int> l1MuDxy(ccReader,"l1_dxy");
  TTreeReaderArray<int> l1MuCharge(ccReader,"l1_charge");
  TTreeReaderArray<int> l1MuEmtfMode(ccReader,"l1_emtfMode");

  gStyle->SetOptStat(0);

  std::cout<<"Running on "<<cc->GetEntries()<<" evts "<<std::endl;

  //plot containers
  std::vector<TString> canvasname;
  std::vector<TString> canvasname_2D;
  std::vector<std::string> kwds;
  std::vector<TString> legs;
  std::vector<TString> legs_2D;
  std::vector<TGraphAsymmErrors*> errors;
  std::vector<TH2F*> errors_2D;

  // cosmetic options
  std::vector<bool> grid,logY,logX;



  // initialize cuts
  float ptThreshold = 10.0;
  // float ptThresholdSecond = 5.0;
  float etaThresholdMin = 1.24;
  float etaThresholdMax = 2.5;
  float dRThreshold = 1.0;
  int qualThreshold = 11;
  int modeThreshold = 11;
  float z0Threshold = 100.0;
  TString outputfileName = "outputfile.root";

  TH1F *h_den_pt = new TH1F("h_den_pt", "", 30, 0, 60);
  TH1F *h_num_pt = new TH1F("h_num_pt", "", 30, 0, 60);
  TH1F *h_den_phi = new TH1F("h_den_phi", "", 64, -4, 4);
  TH1F *h_num_phi = new TH1F("h_num_phi", "", 64, -4, 4);

  Double_t etaArray[15] = {-2.5, -2.1, -1.6, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.6, 2.1, 2.5};
  Double_t etaArray2[21] = {-2.5, -2.3, -2.1, -1.85, -1.6, -1.4, -1.2, -0.9, -0.3, -0.2, 0, 0.2, 0.3, 0.9, 1.2, 1.4, 1.6, 1.85, 2.1, 2.3, 2.5};

  TH1F *h_den_eta = new TH1F("h_den_eta", "", 14, etaArray);
  TH1F *h_num_eta = new TH1F("h_num_eta", "", 14, etaArray);
  TH1F *h_den_fine_eta = new TH1F("h_den_fine_eta", "", 20, etaArray2);
  TH1F *h_num_fine_eta = new TH1F("h_num_fine_eta", "", 20, etaArray2);
  TH1F *h_den_fine_fine_eta = new TH1F("h_den_fine_fine_eta", "", 50, -2.5, 2.5);
  TH1F *h_num_fine_fine_eta = new TH1F("h_num_fine_fine_eta", "", 50, -2.5, 2.5);

  // eta binned plots
  TH1F *h_den_pt_eta3 = new TH1F("h_den_pt_eta3", "", 30, 0, 60);
  TH1F *h_num_pt_eta3 = new TH1F("h_num_pt_eta3", "", 30, 0, 60);
  TH1F *h_den_phi_eta3 = new TH1F("h_den_phi_eta3", "", 64, -4, 4);
  TH1F *h_num_phi_eta3 = new TH1F("h_num_phi_eta3", "", 64, -4, 4);
  //
  TH1F *h_den_pt_eta2 = new TH1F("h_den_pt_eta2", "", 30, 0, 60);
  TH1F *h_num_pt_eta2 = new TH1F("h_num_pt_eta2", "", 30, 0, 60);
  TH1F *h_den_phi_eta2 = new TH1F("h_den_phi_eta2", "", 64, -4, 4);
  TH1F *h_num_phi_eta2 = new TH1F("h_num_phi_eta2", "", 64, -4, 4);
  //
  TH1F *h_den_pt_eta1 = new TH1F("h_den_pt_eta1", "", 30, 0, 60);
  TH1F *h_num_pt_eta1 = new TH1F("h_num_pt_eta1", "", 30, 0, 60);
  TH1F *h_den_phi_eta1 = new TH1F("h_den_phi_eta1", "", 64, -4, 4);
  TH1F *h_num_phi_eta1 = new TH1F("h_num_phi_eta1", "", 64, -4, 4);

  // Single muon efficiencies 

  int eventCount = 0;
  while(ccReader.Next()){
    eventCount++;
    if (eventCount % 10000 == 0) std::cout << eventCount << " events read!" << std::endl;
    // if (eventCount == 10000) break;
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < etaThresholdMin) continue;
      if (abs(genMuEtaStar[i]) > etaThresholdMax) continue;
      if (genMuMatchedL1MuID[i] < 0) continue;
      if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;


      h_den_pt->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_eta->Fill(genMuEtaStar[i]);
        h_den_fine_eta->Fill(genMuEtaStar[i]);
        h_den_fine_fine_eta->Fill(genMuEtaStar[i]);
        h_den_phi->Fill(genMuPhiStar[i]);
      }

      if (genMuDR[i] > 0.6) continue;
      if (l1MuPt[genMuMatchedL1MuID[i]] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;
      
      if (l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_pt->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_eta->Fill(genMuEtaStar[i]);
          h_num_fine_eta->Fill(genMuEtaStar[i]);
          h_num_fine_fine_eta->Fill(genMuEtaStar[i]);
          h_num_phi->Fill(genMuPhiStar[i]);
        }
      }
    }


    // 2.1 < eta < 2.5
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < 2.1) continue;
      if (abs(genMuEtaStar[i]) > etaThresholdMax) continue;
      if (genMuMatchedL1MuID[i] < 0) continue;
      if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;


      h_den_pt_eta3->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_phi_eta3->Fill(genMuPhiStar[i]);
      }

      if (genMuDR[i] > 0.6) continue;
      if (l1MuPt[genMuMatchedL1MuID[i]] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;
      
      if (l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_pt_eta3->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_phi_eta3->Fill(genMuPhiStar[i]);
        }
      }
    }

    // 1.6 < eta < 2.1
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < 1.6) continue;
      if (abs(genMuEtaStar[i]) > 2.1) continue;
      if (genMuMatchedL1MuID[i] < 0) continue;
      if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;

      h_den_pt_eta2->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_phi_eta2->Fill(genMuPhiStar[i]);
      }

      if (genMuDR[i] > 0.6) continue;
      if (l1MuPt[genMuMatchedL1MuID[i]] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;
      
      if (l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_pt_eta2->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_phi_eta2->Fill(genMuPhiStar[i]);
        }
      }
    }


    // 1.24 < eta < 1.6
    for(int i=0; i<genMuPt.GetSize(); i++){
      // if (gendR[i] > dRThreshold) continue;
      if (abs(genMuEtaStar[i]) < etaThresholdMin) continue;
      if (abs(genMuEtaStar[i]) > 1.6) continue;
      if (genMuMatchedL1MuID[i] < 0) continue;
      if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;

      h_den_pt_eta1->Fill(genMuPt[i]);

      if (genMuPt[i] > (ptThreshold + 5.0)){
        h_den_phi_eta1->Fill(genMuPhiStar[i]);
      }

      if (genMuDR[i] > 0.6) continue;
      if (l1MuPt[genMuMatchedL1MuID[i]] < 0) continue;
      // if (!(l1MuEmtfMode[genMuMatchedL1MuID[i]] == 11 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 13 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 14 || l1MuEmtfMode[genMuMatchedL1MuID[i]] == 15)) continue;
      
      if (l1MuPt[genMuMatchedL1MuID[i]] > ptThreshold){
        h_num_pt_eta1->Fill(genMuPt[i]);
        if (genMuPt[i] > (ptThreshold + 5.0)){
          h_num_phi_eta1->Fill(genMuPhiStar[i]);
        }
      }
    }
  }

  // Divide histograms
  TGraphAsymmErrors * error_pt = new TGraphAsymmErrors(h_num_pt,h_den_pt);
  TGraphAsymmErrors * error_eta = new TGraphAsymmErrors(h_num_eta,h_den_eta);
  TGraphAsymmErrors * error_fine_eta = new TGraphAsymmErrors(h_num_fine_eta,h_den_fine_eta);
  TGraphAsymmErrors * error_fine_fine_eta = new TGraphAsymmErrors(h_num_fine_fine_eta,h_den_fine_fine_eta);
  TGraphAsymmErrors * error_phi = new TGraphAsymmErrors(h_num_phi,h_den_phi);
  // eta binned
  TGraphAsymmErrors * error_pt_eta3 = new TGraphAsymmErrors(h_num_pt_eta3,h_den_pt_eta3);
  TGraphAsymmErrors * error_phi_eta3 = new TGraphAsymmErrors(h_num_phi_eta3,h_den_phi_eta3);
  // eta binned
  TGraphAsymmErrors * error_pt_eta2 = new TGraphAsymmErrors(h_num_pt_eta2,h_den_pt_eta2);
  TGraphAsymmErrors * error_phi_eta2 = new TGraphAsymmErrors(h_num_phi_eta2,h_den_phi_eta2);
  // eta binned
  TGraphAsymmErrors * error_pt_eta1 = new TGraphAsymmErrors(h_num_pt_eta1,h_den_pt_eta1);
  TGraphAsymmErrors * error_phi_eta1 = new TGraphAsymmErrors(h_num_phi_eta1,h_den_phi_eta1);


  TFile* outputFile = new TFile(outputfileName,"recreate");
  outputFile->cd();
  error_pt->Write();
  error_eta->Write();
  error_fine_eta->Write();
  error_fine_fine_eta->Write();
  error_phi->Write();
  error_pt_eta3->Write();
  error_phi_eta3->Write();
  error_pt_eta2->Write();
  error_phi_eta2->Write();
  error_pt_eta1->Write();
  error_phi_eta1->Write();
  outputFile->Close();

  // canvasname.push_back("out_ptUnc_vs_dxy_wME11a"); 
  TString titlePt="Gen Muon p_{T} [GeV]";
  TString titleEta="Gen Muon #eta";
  TString titlePhi="Gen Muon #phi";

  error_pt->GetXaxis()->SetTitle(titlePt);
  error_pt->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_eta->GetXaxis()->SetTitle(titleEta);
  error_eta->GetYaxis()->SetTitle("L1T Efficiency");
  error_eta->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_fine_eta->GetXaxis()->SetTitle(titleEta);
  error_fine_eta->GetYaxis()->SetTitle("L1T Efficiency");
  error_fine_eta->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_fine_fine_eta->GetXaxis()->SetTitle(titleEta);
  error_fine_fine_eta->GetYaxis()->SetTitle("L1T Efficiency");
  error_fine_fine_eta->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_phi->GetXaxis()->SetTitle(titlePhi);
  error_phi->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi->GetYaxis()->SetRangeUser(0.00001,1.2);

  // eta binned
  error_pt_eta3->GetXaxis()->SetTitle(titlePt);
  error_pt_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_phi_eta3->GetXaxis()->SetTitle(titlePhi);
  error_phi_eta3->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi_eta3->GetYaxis()->SetRangeUser(0.00001,1.2);
  // eta binned

  error_pt_eta2->GetXaxis()->SetTitle(titlePt);
  error_pt_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_phi_eta2->GetXaxis()->SetTitle(titlePhi);
  error_phi_eta2->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi_eta2->GetYaxis()->SetRangeUser(0.00001,1.2);

  // eta binned
  error_pt_eta1->GetXaxis()->SetTitle(titlePt);
  error_pt_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_pt_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);

  error_phi_eta1->GetXaxis()->SetTitle(titlePhi);
  error_phi_eta1->GetYaxis()->SetTitle("L1T Efficiency");
  error_phi_eta1->GetYaxis()->SetRangeUser(0.00001,1.2);


  TString leg = "L1 pT (prompt) > 10 GeV";

  TString leg_etaFull = "1.2 < |#eta| < 2.5";
  TString leg_eta1 = "1.2 < |#eta| < 1.6";
  TString leg_eta2 = "1.6 < |#eta| < 2.1";
  TString leg_eta3 = "2.1 < |#eta| < 2.5";

  // kwds.push_back(options);
  errors.push_back(error_pt);
  errors.push_back(error_eta);
  errors.push_back(error_fine_eta);
  errors.push_back(error_fine_fine_eta);
  errors.push_back(error_phi);

  //eta binned
  errors.push_back(error_pt_eta3);
  errors.push_back(error_phi_eta3);
  //eta binned
  errors.push_back(error_pt_eta2);
  errors.push_back(error_phi_eta2);

  //eta binned
  errors.push_back(error_pt_eta1);
  errors.push_back(error_phi_eta1);

  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);

  // eta binned
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);

  // eta binned
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);

  // eta binned
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);
  legs.push_back(leg);

  // eta same plot
  legs.push_back(leg_eta1);
  legs.push_back(leg_eta2);
  legs.push_back(leg_eta3);
  legs.push_back(leg_eta1);
  legs.push_back(leg_eta2);
  legs.push_back(leg_eta3);

  canvasname.push_back("eff_pt_pt10_NNv8vsBDT");
  canvasname.push_back("eff_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_fine_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_fine_fine_eta_pt10_NNv8vsBDT");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT");

  //eta binned
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta3");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta3");

  //eta binned
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta2");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta2");

  //eta binned
  canvasname.push_back("eff_pt_pt10_NNv8vsBDT_eta1");
  canvasname.push_back("eff_phi_pt10_NNv8vsBDT_eta1");


  for (int i=0; i<canvasname.size(); i++){
    cout<<i<<"/"<<canvasname.size()<<endl;
    if (canvasname[i]=="Canvas_name_already_used_action_skipping"){ 
      std::cout << "canvas already used. skipping..." << std::endl;
      continue;
    }
    //create canvas and save histos
    std::cout << "Drawing: " << errors[i]->GetName() << std::endl;
    TCanvas * c1=new TCanvas(canvasname[i],canvasname[i],700,700);
    errors[i]->Draw("A P");
    errors[i]->SetLineWidth(3);
    errors[i]->SetLineColor(1);
    TLatex cms_label=cms_latex();
    TLatex header=head();

    gStyle->SetLegendBorderSize(0);
    gStyle->SetLegendTextSize(0.018); 
    TLegend * leg =new TLegend(0.4,0.8,0.88,0.88);
    leg->AddEntry(errors[i],legs[i]);
    // bool printLeg=false;

    //put histos with same cnvas name in same plot
    for (int j=i+1; j<errors.size(); j++){
      if (canvasname[i]!=canvasname[j]) continue;
      errors[j]->Draw("sames P");
      canvasname[j]="Canvas_name_already_used_action_skipping";
      errors[j]->SetLineWidth(3);
      // errors[j]->SetLineColor(2);
      errors[j]->SetLineColor(DefaultColor(j,i));
      leg->AddEntry(errors[j],legs[j]);
    }
    leg->Draw("sames");

    // c1->SaveAs("./plots"+canvasname[i]+".png");
    c1->SaveAs("./output_files/comparisons/efficiencies/NNv8/"+canvasname[i]+"_muGunFlatXYZ_denomModeCut.pdf");
    canvasname[i]="Canvas_name_already_used_action_skipping";

  }

  return 0;
 }
