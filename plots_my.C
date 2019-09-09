{
  TFile* f = TFile::Open("myAmLi_LZAP440_SSMS.root");

      TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
      gStyle->SetOptStat(0);
      

      TH2F* hS2dT = (TH2F*)f->Get("hS2dTms");
      TH2F* hS2dTs = (TH2F*)f->Get("hS2dTss");
      hS2dT->SetTitle("log10(S2) Vs Drift Time");
      hS2dT->GetXaxis()->SetTitle("Drift Time [#mus]");
      hS2dT->GetYaxis()->SetTitle("log10(S2 [phd])");
      //      hS2dTXe131m->GetYaxis()->SetRangeUser(5.2, 7);
      hS2dT->GetYaxis()->SetRangeUser(4.0, 8.0);
      hS2dT->SetMinimum(0);
      hS2dT->SetMaximum(450);
      hS2dT->SetMarkerColor(3);
      hS2dT->SetMarkerStyle(8);
      hS2dT->Draw();
      hS2dTs->SetMarkerColor(2);
      hS2dTs->SetMarkerStyle(29);
      hS2dTs->Draw("same");
    
      TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
      gStyle->SetOptStat(0);

      TH2F* hS2S1 = (TH2F*)f->Get("hrs2s1s1_ms");
      TH2F* hS2S1s = (TH2F*)f->Get("hrs2s1s1");
      hS2S1->SetTitle("log10(S2) Vs S1");
      hS2S1->GetXaxis()->SetTitle("S1 [phd]");
      hS2S1->GetYaxis()->SetTitle("log10(S2 [phd])");
      //      hS2dTXe131m->GetYaxis()->SetRangeUser(5.2, 7);
      hS2S1->GetYaxis()->SetRangeUser(1.0, 4.0);
      hS2S1->SetMinimum(0);
      hS2S1->SetMaximum(450);
      hS2S1->SetMarkerColor(3);
      hS2S1->SetMarkerStyle(8);
      hS2S1->Draw();
      hS2S1s->SetMarkerColor(2);
      hS2S1s->SetMarkerStyle(29);
      hS2S1s->Draw("same");


}
