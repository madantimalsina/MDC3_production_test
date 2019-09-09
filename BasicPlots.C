// ********************************
// LZ Data Analysis Plotting Code
// ********************************

// Includes
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <ctime>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TChain.h"

// Main Executable
void BasicPlots() {

  // Set ROOT settings
  gROOT->Reset();
  gStyle->SetOptStat(0);
  delete gROOT->FindObject("Canvas");
  // **********
  // File Input
  // **********
  
  // Define vector of input files containg RQs
  std::vector<std::string> rqFiles = {"lzap_waveforms.root"};
				      
  // Loop over each file
  for (int iF {}; iF < rqFiles.size(); ++iF) {

    // Open RQ file
    TFile *File = new TFile(rqFiles[iF].c_str());

    // Define RQ tree
    TTree *Events_Tree = (TTree*) File->Get("Events");
    TTree *Scatter_Tree = (TTree*) File->Get("Scatters");
    TTree *PodWaveforms_Tree = (TTree*) File->Get("PodWaveforms");

    // Must be turned on (figure out what this does exactly)
    Events_Tree->SetMakeClass(1);
    Events_Tree->AddFriend(Scatter_Tree);
    Events_Tree->GetFriend("Scatters")->SetMakeClass(1);
    Events_Tree->SetMakeClass(2);
    Events_Tree->AddFriend(PodWaveforms_Tree);
    Events_Tree->GetFriend("PodWaveforms")->SetMakeClass(2);

    // *************************
    // Read In TTree Information
    // *************************

    // Pulses TPC HG Branch
    int nPulses {};                                  
    std::vector<float> pulseArea_phd {};
    std::vector<int> areaFractionTime5_ns {};
    std::vector<int> areaFractionTime95_ns {};
    std::vector<float> s1Probability {};
    std::vector<float> s2Probability {};
    std::vector<float> s2Xposition {};
    std::vector<float> s2Yposition {};

    // Single Scatters Branch
    int nSingleScatters {};
/*    std::vector<float> s2Area_phd {};
    std::vector<float> s1Area_phd {};
    std::vector<float> correctedX {};
    std::vector<float> correctedY {};
    std::vector<float> correctedZ {};
    std::vector<float> energyER {};
    std::vector<float> energyNR {};*/

    // Multiple Scatters Branch
    int nMultipleScatters {};

    // Pile Up Branch
    int nPileUpScatters {};

    // Other Scatters Branch
    int nOtherScatters {};

    unsigned long runid {};
    unsigned long eventid {};
    unsigned long eventid2 {};
    unsigned long s2pIDSS {};
    unsigned long s1pIDMS {};
    
    // Turn off all RQs for all trees
    Events_Tree->SetBranchStatus("*",0);
    PodWaveforms_Tree->SetBranchStatus("*",0);

    // Turn on specific RQ branches for analysis
    Events_Tree->SetBranchStatus("ss.nSingleScatters",1);
    Events_Tree->SetBranchStatus("ss.s2PulseID",1);
    Events_Tree->SetBranchStatus("ms.nMultipleScatters",1);
    Events_Tree->SetBranchStatus("ms.s1PulseID",1);
    Events_Tree->SetBranchStatus("pileUp.nPileUpScatters",1);
    Events_Tree->SetBranchStatus("other.nOtherScatters",1);
    //Events_Tree->SetBranchStatus("eventHeader.runID",1);
    //Events_Tree->SetBranchStatus("eventHeader.eventID",1);
    Events_Tree->SetBranchStatus("eventHeader.runID",1);
    Events_Tree->SetBranchStatus("eventHeader.eventID",1);

    Events_Tree->SetBranchStatus("calibratedPods.nPods",2);

    Events_Tree->SetBranchAddress("ss.nSingleScatters",&nSingleScatters);
    Events_Tree->SetBranchAddress("ss.s2PulseID",&s2pIDSS);
    Events_Tree->SetBranchAddress("ms.nMultipleScatters",&nMultipleScatters);
    Events_Tree->SetBranchAddress("ms.s1PulseID",&s1pIDMS);
    Events_Tree->SetBranchAddress("pileUp.nPileUpScatters",&nPileUpScatters);
    Events_Tree->SetBranchAddress("other.nOtherScatters",&nOtherScatters);
    Events_Tree->SetBranchAddress("eventHeader.runID",&runid);
    Events_Tree->SetBranchAddress("eventHeader.eventID",&eventid);
    Events_Tree->SetBranchAddress("calibratedPods.nPods",&eventid2);
    //Events_Tree->SetBranchAddress("eventHeader.runID",&runID);
    //Events_Tree->SetBranchAddress("eventHeader.eventID",&eventID);
    // ****************
    // Loop over events
    // ****************
    
    // Identify number of events in file
    Int_t numEvents {};
    //Int_t runID {};
    

    numEvents = Events_Tree->GetEntries();
    //runID = Events_Tree->eventHeader.runID();
    std::cout << "**********************" << std::endl;
    std::cout << "Beginning event loop!" << std::endl;
    std::cout << "Number of events: " << numEvents << std::endl;
    std::cout << "**********************" << std::endl;

    // Define scatter counters
    int num_SS {};
    int num_PS {};
    int num_MS {};
    int num_OS {};
    int num_id {};
    
    // Loop over each event
    for (int iE {}; iE < numEvents; ++iE) {
      
      // Load event and fill branch variables with RQ values
      Events_Tree->GetEntry(iE);

      // *************************************
      // Extract otherScatters info from event
      // *************************************
      if (nSingleScatters ==1)
          {
            ++num_SS;
            //cout << "S2ID = " << s2pIDSS  <<endl;
            cout << "SS "<< num_SS << ": --> runID = " << runid << ",   " << "eventID = "<<eventid  <<  endl;
          }
      // Loop through other scatter in event
      if (nOtherScatters ==1)
      {
         	++num_OS;
      }
      if (nMultipleScatters ==1)
      {
        ++num_MS; 
        //cout << "S1ID = " << s1pIDMS  <<endl;
        //cout << runID << ",   " << eventID  <<endl;
        cout << "MS "<< num_MS << ": --> runID = " << runid << ",   " << "eventID = "<<eventid  <<  endl;
      }
      if (nPileUpScatters ==1)
      {
        ++num_PS; 
        //cout << "PileUp --> runID = " << runid << ",   " << "eventID = "<<eventid  <<  endl;
      }
      if (nSingleScatters>0 || nMultipleScatters >0) {
        //cout << "runID = " << runid << ",   " << "eventID = "<<eventid  <<  endl;

        ++num_id;}

      // Output final numbers of each scatter type
      if (iE == numEvents-1) {
	std::cout << "*******************************" << std::endl;
	std::cout << "Total Single Scatters: " << num_SS << "\n";
  std::cout << "Total Multiple Scatters: " << num_MS << "\n";
  std::cout << "Total MS & SS: " << num_id << "\n";
  std::cout << "Total pileUp Scatters: " << num_PS << "\n";
	std::cout << "Total Other Scatters: " << num_OS << "\n";
  std::cout << "Total Events: " << num_OS + num_PS+num_MS+num_SS << "\n";
	std::cout << "*******************************" << std::endl;
      }
      
    }

    // Close input file
    File->Close();
  }

}  
