// Event Header Branch
unsigned long runid {};
unsigned long eventid {};
std::string rawfile {};

// Event Header Branch
Events_Tree->SetBranchStatus("eventHeader.runID",1);
Events_Tree->SetBranchStatus("eventHeader.eventID",1);
Events_Tree->SetBranchStatus("eventHeader.rawFileName",1);

// Event Header Branch
Events_Tree->SetBranchAddress("eventHeader.runID",&runid);
Events_Tree->SetBranchAddress("eventHeader.eventID",&eventid);
Events_Tree->SetBranchAddress("eventHeader.rawFileName",&rawfile);

// Now within event loop you can do

//SS and MS in event
  
  if (nSingleScatters>0 || nMultipleScatters >0) {      
      outfile << "/global/projecta/projectdirs/lz/data/MDC3/calibration/BACCARAT-4.10.3_DER-8.5.13/20180222/"
          << rawfile << "," << runID << "," << eventID  <<endl;
}
 }

// After looping through files write a txt file in a format:
// rawfile runid eventid
// Then run LZap on the sparse data file to get an LZap file with only events that pass your cut
