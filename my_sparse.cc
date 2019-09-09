// ****************************************
// Spare processing
// my_sparse.cc
// Written by Madan Timalsina
// Last update Sep/06/2019
// ****************************************

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

void my_sparse(std::string lzap_files) {

  // Set ROOT settings
  gROOT->Reset();
  gStyle->SetOptStat(0);


  // **********
  // File Input
  // **********
  
  // Define vector of input files containg RQs
  std::vector<std::string> rqFiles {};

  std::ifstream in_file;
  std::string line {};

  // Open data file
  in_file.open(lzap_files);

  while(!in_file.eof()) {
    in_file >> line;
    rqFiles.push_back(line);
  }

  // Close data file
  in_file.close();

  // Event header storage vectors
  std::vector<string> rawfiles {};
  std::vector<unsigned long> runids {};
  std::vector<unsigned long> eventids {};

				      
  // Loop over each file
  for (int iF {}; iF < rqFiles.size(); ++iF) {
    
    // Open RQ file
    TFile *File = new TFile(rqFiles[iF].c_str());

    // Define RQ tree
    TTree *Events_Tree = (TTree*) File->Get("Events");
    TTree *Scatter_Tree = (TTree*) File->Get("Scatters");

    // Must be turned on (figure out what this does exactly)
    Events_Tree->SetMakeClass(1);
    Events_Tree->AddFriend(Scatter_Tree);
    Events_Tree->GetFriend("Scatters")->SetMakeClass(1);
    
    // *********************************************
    // Define Variables to Read in TTree Information
    // *********************************************

    // Event Header Branch
    unsigned long runid {};
    unsigned long eventid {};
    std::string rawfile {};
    
    // Single Scatters Branch
    int nSingleScatters {};
    
    // // Multiple Scatters Branch
    int nMultipleScatters {};

    // ********************************
    // Turn on RQ Branches for Analysis
    // ********************************

    // First turn off all RQs for all trees
    Events_Tree->SetBranchStatus("*",0);

    // Event Header Branch
    Events_Tree->SetBranchStatus("eventHeader.runID",1);
    Events_Tree->SetBranchStatus("eventHeader.eventID",1);
    Events_Tree->SetBranchStatus("eventHeader.rawFileName",1);
    
    // Single Scatters Branch
    Events_Tree->SetBranchStatus("ss.nSingleScatters",1);

    // // Multiple Scatters Branch
     Events_Tree->SetBranchStatus("ms.nMultipleScatters",1);


    // **********************************
    // Fill Variables with RQ Branch Data
    // **********************************

    // Event Header Branch
    Events_Tree->SetBranchAddress("eventHeader.runID",&runid);
    Events_Tree->SetBranchAddress("eventHeader.eventID",&eventid);
    Events_Tree->SetBranchAddress("eventHeader.rawFileName",&rawfile);
    
    // Single Scatters Branch
    Events_Tree->SetBranchAddress("ss.nSingleScatters",&nSingleScatters);

     // Multiple Scatters Branch
     Events_Tree->SetBranchAddress("ms.nMultipleScatters",&nMultipleScatters);


    // ****************
    // Loop Over Events
    // ****************
    
    // Identify number of events in file
    Int_t numEvents {};

    numEvents = Events_Tree->GetEntries();

    std::cout << "**********************" << std::endl;
    std::cout << "Beginning event loop!" << std::endl;
    std::cout << "LZap file name: " << rqFiles.at(iF) << std::endl;
    std::cout << "Number of events: " << numEvents << std::endl;
    std::cout << "**********************" << std::endl;

    // Loop over each event
    for (int iE {}; iE < numEvents; ++iE) {
      
      // Load event and fill branch variables with RQ values
      Events_Tree->GetEntry(iE);

      // **************************************
      // Extract SS & MS Info from Event
      // **************************************
      
      // SS and MS 
      //for (int iS {}; iS < nSingleScatters; ++iS) 
      if (nSingleScatters>0 || nMultipleScatters >0){


      	// ***************
      	// Perform RQ Cuts
      	// ***************
        
          rawfiles.push_back(rawfile);
          runids.push_back(runid);
          eventids.push_back(eventid);

       
      }
      
    }
    
    // Close input file
    File->Close();

  }

  // ***************************
  // Sparse Processing Text File
  // ***************************

  // Commisson AmLi raw
  std::string fullpath {"/global/projecta/projectdirs/lz/data/MDC3/calibration/BACCARAT-4.10.3_DER-8.5.13/20180222/"};

  std::string file_name {"sparsedata.txt"};
  std::ofstream out_file {file_name};
  if (out_file) {
    std::cout << "Writing .txt file" << std::endl;
    for (size_t i {};i<rawfiles.size();i++) 
      out_file << fullpath+rawfiles.at(i)  << " " << runids.at(i) << " "
  	       << eventids.at(i) << std::endl; 
  }
  else 
    std::cout << "File not opened" << std::endl;
  
  out_file.close();
    
}
