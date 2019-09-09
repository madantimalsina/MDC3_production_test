// Standard C++ headers                                                                                                                                                             
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// ROOT Headers                                                                                                                                                                     
#include "TStyle.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TStopwatch.h"
#include "TFile.h"
#include "TString.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TChain.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TMath.h"
#include "TTimeStamp.h"



void mdc3_checks(char *infiles) {

  TH1F *h_spes = new TH1F("h_spes"," ; pulse area (phd) ; ", 200, 0, 20);
  TH2F *h_spes_width = new TH2F("h_spe_width", " ; pulse area (phd) ; pulse width (ns) ", 200,0,20,200,0,1200);
  TH1F *h_ses = new TH1F("h_ses"," ; pulse area (phd) ; ", 200, 0, 500);
  TH2F *h_logS1S2 = new TH2F("h_logS1S2", " ; log(S1 area) (phd) ; log(S2 area) (phd) ",500,0,1E5,500,0,1E7);
  TH1F *h_skinarea = new TH1F("h_skinarea", "; skin area (phd) ; ", 200, 0, 2000);
  TH1F *h_odarea = new TH1F("h_odarea", "; od area (phd) ; ", 200, 0, 1000);
  TH1F *h_skinsumarea = new TH1F("h_skinsumarea", "; skin area (phd) ; ", 200, 0, 1000);
  TH1F *h_odsumarea = new TH1F("h_odsumarea", "; od area (phd) ; ", 200, 0, 1200);
  TChain *events = new TChain("Events");
  TChain *scatters = new TChain("Scatters");

  TString txtFileList(infiles);
  if (txtFileList.Contains(".txt")) {
    cout << "Loading file names from "<<txtFileList << " into "<< events->GetName()<<endl;
    ifstream fileList(txtFileList);
    string file;
    if (fileList.is_open()) {
      while ( getline(fileList, file) ) {
	events->AddFile(file.c_str());
	scatters->AddFile(file.c_str());
      }
      fileList.close();
      }else{
      cout<<"The file "<< txtFileList <<" doesn't exist. Exiting !!"<<endl;
      exit(-1);
    }
  }
  else if (txtFileList.Contains(".root")) {
    events->Add(infiles);
    scatters->Add(infiles);
  }

  int nPulsesOD;
  std::vector<float> odPulseArea_phd;
  std::vector<int> odAft5;
  std::vector<int> odCoincidence;
  std::vector<int> odPulseStart;
  int nPulsesTPC;
  std::vector<float> tpcPulseArea_phd;
  std::vector<int> tpcPulseStart, tpcPulseEnd;
  std::vector<float> tpcPulseTBA;
  std::vector<int>   tpcAft5;
  std::vector<int>   tpcAft50;
  std::vector<int>   tpcAft95;
  std::vector<int> tpcCoincidence;

  std::vector<float> speProb;
  std::vector<float> s1Prob;
  std::vector<float> s2Prob;
  std::vector<float> seProb;
  std::vector<float> s2Xpos_cm;
  std::vector<float> s2Ypos_cm;

  int nPulsesSkin;
  std::vector<float> skinPulseArea_phd;
  std::vector<int> skinPulseStart;
  std::vector<int> skinAft5;
  std::vector<int> skinCoincidence;


  events->SetMakeClass(1);
  events->SetBranchStatus("*",0);
  // header                                                                                                                                                                                 
  /*events->SetBranchStatus("eventHeader.eventID",                    1);
  events->SetBranchAddress("eventHeader.eventID",                   &eventID);
  events->SetBranchStatus("eventHeader.runID",                      1);
  events->SetBranchAddress("eventHeader.runID",                     &runID);
  events->SetBranchStatus("eventHeader.rawFileName",                1);
  events->SetBranchAddress("eventHeader.rawFileName",               &rawFileName);
  events->SetBranchStatus("eventHeader.triggerTimeStamp_ns",        1);
  events->SetBranchAddress("eventHeader.triggerTimeStamp_ns",       &triggerTimestamp_ns);
  events->SetBranchStatus("eventHeader.triggerTimeStamp_s",         1);
  events->SetBranchAddress("eventHeader.triggerTimeStamp_s",        &triggerTimestamp_s);
  */  
  //OD                                                                                                                                                                                     
  events->SetBranchStatus("pulsesODHG.nPulses",                     1);
  events->SetBranchAddress("pulsesODHG.nPulses",                    &nPulsesOD);
  events->SetBranchStatus("pulsesODHG.pulseArea_phd",               1);
  events->SetBranchAddress("pulsesODHG.pulseArea_phd",              &odPulseArea_phd);
  events->SetBranchStatus("pulsesODHG.pulseStartTime_ns",           1);
  events->SetBranchAddress("pulsesODHG.pulseStartTime_ns",          &odPulseStart);
  events->SetBranchStatus("pulsesODHG.areaFractionTime5_ns",        1);
  events->SetBranchAddress("pulsesODHG.areaFractionTime5_ns",       &odAft5);
  events->SetBranchStatus("pulsesODHG.coincidence",                 1);
  events->SetBranchAddress("pulsesODHG.coincidence",                &odCoincidence);
  //TPC                                                                                                                                                                                     
  events->SetBranchStatus("pulsesTPCHG.nPulses",                    1);
  events->SetBranchAddress("pulsesTPCHG.nPulses",                   &nPulsesTPC);
  events->SetBranchStatus("pulsesTPCHG.pulseArea_phd",              1);
  events->SetBranchAddress("pulsesTPCHG.pulseArea_phd",             &tpcPulseArea_phd);
  events->SetBranchStatus("pulsesTPCHG.areaFractionTime5_ns",       1);
  events->SetBranchAddress("pulsesTPCHG.areaFractionTime5_ns",      &tpcAft5);
  events->SetBranchStatus("pulsesTPCHG.areaFractionTime50_ns",      1);
  events->SetBranchAddress("pulsesTPCHG.areaFractionTime50_ns",     &tpcAft50);
  events->SetBranchStatus("pulsesTPCHG.areaFractionTime95_ns",      1);
  events->SetBranchAddress("pulsesTPCHG.areaFractionTime95_ns",     &tpcAft95);
  events->SetBranchStatus("pulsesTPCHG.pulseStartTime_ns",          1);
  events->SetBranchAddress("pulsesTPCHG.pulseStartTime_ns",         &tpcPulseStart);
  events->SetBranchStatus("pulsesTPCHG.pulseEndTime_ns",            1);
  events->SetBranchAddress("pulsesTPCHG.pulseEndTime_ns",           &tpcPulseEnd);
  events->SetBranchStatus("pulsesTPCHG.topBottomAsymmetry",         1);
  events->SetBranchAddress("pulsesTPCHG.topBottomAsymmetry",        &tpcPulseTBA);
  events->SetBranchStatus("pulsesTPCHG.singlePEprobability",        1);
  events->SetBranchAddress("pulsesTPCHG.singlePEprobability",       &speProb);
  events->SetBranchStatus("pulsesTPCHG.s1Probability",              1);
  events->SetBranchAddress("pulsesTPCHG.s1Probability",             &s1Prob);
  events->SetBranchStatus("pulsesTPCHG.s2Probability",              1);
  events->SetBranchAddress("pulsesTPCHG.s2Probability",             &s2Prob);
  events->SetBranchStatus("pulsesTPCHG.singleElectronProbability",  1);
  events->SetBranchAddress("pulsesTPCHG.singleElectronProbability", &seProb);
  events->SetBranchStatus("pulsesTPCHG.s2Xposition_cm",             1);
  events->SetBranchAddress("pulsesTPCHG.s2Xposition_cm",            &s2Xpos_cm);
  events->SetBranchStatus("pulsesTPCHG.s2Yposition_cm",             1);
  events->SetBranchAddress("pulsesTPCHG.s2Yposition_cm",            &s2Ypos_cm);
  events->SetBranchStatus("pulsesTPCHG.coincidence",                1);
  events->SetBranchAddress("pulsesTPCHG.coincidence",               &tpcCoincidence);

  // Skin                                                                                                                                                                                   
  events->SetBranchStatus("pulsesSkin.nPulses",                     1);
  events->SetBranchAddress("pulsesSkin.nPulses",                    &nPulsesSkin);
  events->SetBranchStatus("pulsesSkin.pulseArea_phd",               1);
  events->SetBranchAddress("pulsesSkin.pulseArea_phd",              &skinPulseArea_phd);
  events->SetBranchStatus("pulsesSkin.pulseStartTime_ns",           1);
  events->SetBranchAddress("pulsesSkin.pulseStartTime_ns",          &skinPulseStart);
  events->SetBranchStatus("pulsesSkin.areaFractionTime5_ns",        1);
  events->SetBranchAddress("pulsesSkin.areaFractionTime5_ns",       &skinAft5);
  events->SetBranchStatus("pulsesSkin.coincidence",                 1);
  events->SetBranchAddress("pulsesSkin.coincidence",                &skinCoincidence);

  int nSingleScatters;
  int nMultipleScatters;
  int nKr83mScatters;
  int nPileUpScatters;
  int nOtherScatters;
  scatters->SetMakeClass(1);
  scatters->SetBranchAddress("ss.nSingleScatters", &nSingleScatters);
  scatters->SetBranchAddress("ms.nMultipleScatters", &nMultipleScatters);
  scatters->SetBranchAddress("kr83m.nKr83mScatters", &nKr83mScatters);
  scatters->SetBranchAddress("pileUp.nPileUpScatters", &nPileUpScatters);
  scatters->SetBranchAddress("other.nOtherScatters", &nOtherScatters);
  float ss_s1Area_phd;
  float ms_s1Area_phd;
  float ss_s2Area_phd;
  vector<float> ms_s2Area_phd;
  float ss_x_cm;
  float ss_y_cm;
  float ss_drift;
  vector<float> ms_x_cm;
  vector<float> ms_y_cm;
  float ms_drift;
  int nS2s;
  int ss_s1ID;
  int ms_s1ID;
  int ss_s2ID;
  vector<int> ms_s2ID;
  scatters->SetBranchAddress("ss.s1PulseID", &ss_s1ID);
  scatters->SetBranchAddress("ms.s1PulseID", &ms_s1ID);
  scatters->SetBranchAddress("ss.s2PulseID", &ss_s2ID);
  scatters->SetBranchAddress("ms.s2PulseIDs", &ms_s2ID);
  scatters->SetBranchAddress("ss.s1Area_phd", &ss_s1Area_phd);
  scatters->SetBranchAddress("ms.s1Area_phd", &ms_s1Area_phd);
  scatters->SetBranchAddress("ss.s2Area_phd", &ss_s2Area_phd);
  scatters->SetBranchAddress("ms.s2Area_phd", &ms_s2Area_phd);
  scatters->SetBranchAddress("ss.x_cm", &ss_x_cm);
  scatters->SetBranchAddress("ss.y_cm", &ss_y_cm);
  scatters->SetBranchAddress("ss.driftTime_ns", &ss_drift);
  scatters->SetBranchAddress("ms.x_cm", &ms_x_cm);
  scatters->SetBranchAddress("ms.y_cm", &ms_y_cm);
  scatters->SetBranchAddress("ms.weightedDriftTime_ns", &ms_drift);
  scatters->SetBranchAddress("ms.nS2s", &nS2s);

  int nEvents = events->GetEntries();
  int ss = 0;
  int ms = 0;
  int kr83m = 0;
  int pileup = 0;
  int  other = 0;
  float skinsum;
  float odsum;
  int skin = 0;
  int od = 0;
  int tpc = 0;
  int skinandod = 0;
  int odandtpc = 0;
  int skinandtpc = 0;
  int allthree = 0;
  int skinonly = 0;
  int odonly = 0;
  int tpconly = 0;
  for (int evt=0; evt < nEvents; evt++) {
   
    skinsum = 0;
    odsum = 0;
    
    if (evt%10==0) cout << "Processing event number " << evt << " of " << nEvents << endl;
    events->GetEntry(evt);
    scatters->GetEntry(evt);

    if (nSingleScatters>0) ss++;
    if (nMultipleScatters>0) ms++;
    if (nKr83mScatters>0) kr83m++;
    if (nPileUpScatters>0) pileup++;
    if (nOtherScatters>0) other++;

    if (nPulsesSkin>0) skin++;
    if (nPulsesOD>0) od++;
    if (nPulsesTPC>0) tpc++;
    if (nPulsesSkin > 0 && nPulsesOD > 0 && nPulsesTPC == 0) skinandod++;
    if (nPulsesSkin > 0 && nPulsesTPC > 0 && nPulsesOD == 0) skinandtpc++;
    if (nPulsesOD > 0 && nPulsesTPC > 0 && nPulsesSkin == 0) odandtpc++;
    if (nPulsesOD > 0 && nPulsesTPC > 0 && nPulsesSkin > 0) allthree++;
    if (nPulsesTPC>0 && nPulsesSkin == 0 && nPulsesOD == 0) tpconly++;
    if (nPulsesTPC== 0 && nPulsesSkin ==0 && nPulsesOD > 0) odonly++;
    if (nPulsesTPC == 0 && nPulsesSkin > 0 && nPulsesOD ==0) skinonly++;

    for (int p=0; p<nPulsesSkin; p++) {
      h_skinarea->Fill(skinPulseArea_phd[p]);  
      if (skinCoincidence[p] > 1) {
	skinsum += skinPulseArea_phd[p];
      }
    }
    
    for(int p=0; p<nPulsesOD; p++) {
      h_odarea->Fill(odPulseArea_phd[p]);
      if (odCoincidence[p] > 1) {
	odsum += odPulseArea_phd[p]; 
      }
    }
	
    if (skinsum>0)h_skinsumarea->Fill(skinsum);
    if (odsum>0)  h_odsumarea->Fill(odsum);
   
        for (int p=0; p<nPulsesTPC; p++) {
      //if (speProb[p] == 1) { 
      if (tpcCoincidence[p] ==1) {
	h_spes->Fill(tpcPulseArea_phd[p]);
	h_spes_width->Fill(tpcPulseArea_phd[p], tpcAft95[p]-tpcAft5[p]);
      }
      if (seProb[p]==1) {
        h_ses->Fill(tpcPulseArea_phd[p]);
      }
    }

    if (ss_s1Area_phd>0) {
      h_logS1S2->Fill(log10(ss_s1Area_phd),log10(ss_s2Area_phd));
    }

 }

  cout << "Done!" << endl;
  cout << " Of " << nEvents << " events: " << endl;
 
  cout << ss << " are single scatters" << endl;
  cout << ms << " are multiple scatters" << endl;
  cout << kr83m << " are Kr83m scatters" << endl;
  cout << pileup << " are Pile Up scatters" << endl;
  cout << other << " are other scatters" << endl;

  cout << tpc << " have TPC pulses, and there are " << tpconly << " only in the TPC" <<  endl;
  cout << od << " have OD pulses, and there are " << odonly << " only in the OD" << endl;
  cout << skin << " have Skin pulses, and there are " << skinonly << " only in the Skin" << endl;
  cout << skinandtpc << " are only in the skin and TPC" << endl;
  cout << odandtpc <<" are only in the OD and TPC"<< endl;
  cout << skinandod << " are only in the vetoes" << endl;
  cout << allthree << " are in all three." << endl;

  gStyle->SetPalette(55);
  gStyle->SetOptStat(0);

  TCanvas *c = new TCanvas();
  c->cd();
  h_spes->Draw();
  TCanvas *c2 = new TCanvas();
  c2->cd();
  h_spes_width->Draw();
  TCanvas *c3 = new TCanvas();
  /**c3->Divide(1,2);
  c3->cd(1);
  h_skinarea->Draw();*/
  c3->cd();
  //gPad->SetLogy();
  h_skinsumarea->Draw();
  TCanvas *c4 = new TCanvas();
  /*c4->Divide(1,2);
  c4->cd(1);
  h_odarea->Draw();
  c4->cd(2);*/
  
  c4->cd();
  //gPad->SetLogy();
  h_odsumarea->Draw();
}

