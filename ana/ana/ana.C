#define ana_cxx
// The class definition in ana.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// root> T->Process("ana.C")
// root> T->Process("ana.C","some options")
// root> T->Process("ana.C+")
//

#include "ana.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include "TBenchmark.h"
#include <cstring>
#include "TFile.h"
#include "TH1.h"



void ana::ProgressBar(Long64_t i,Long64_t total)
{
  Percent=(int)(100.0*(double)i/(double)total);
  if(Percent!=PercentOld)
    {
      std::cout<<"\r"<<Percent<<"% [";

      int temp = (int)((double)Percent/(double)100 * 50);
      for (int i = 0; i < temp-1; ++i)
	{
	  std::cout << "=";
	}
      std::cout<<">";
      for (int j = 0; j <50 - temp-1; ++j)
	{
	  std::cout<<" ";
	}
      std::cout<<"]  ";
    }
  else
    {
      std::cout << "\r" ;
    }
  std::cout << flush;
  PercentOld=Percent;
}

void ana::Autocorrelation(int n,Short_t *in1,Short_t *in2,double *out)
{
  for (int i = 0; i < n; ++i)
    {
      out[i] = 0;
      for (int j = 0; j <= n-1-i; ++j)
	{
	  out[i] += in1[j]*in2[j+i];
	}
      out[i] /= n;//out[i] /= (n-i);
    }
}


void ana::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   gBenchmark->Start("tree");//计时开始
   CountEntry=0;
   Percent=0;
   PercentOld=100;

   Fs = 1000;
   N = 1024;
   N2 = N*2;
   Scale = 10;
   memset(blockr00, 0, sizeof(double)*N);
   memset(blockr11, 0, sizeof(double)*N);
   memset(blockr22, 0, sizeof(double)*N);
   memset(blockr01, 0, sizeof(double)*N);
   memset(blockr02, 0, sizeof(double)*N);
   memset(blockr12, 0, sizeof(double)*N);
   memset(blocktemp, 0, sizeof(double)*N);

   r00 = new TH1D("r00","",1024,0,1024);
   r11 = new TH1D("r11","",1024,0,1024);
   r22 = new TH1D("r22","",1024,0,1024);
   r01 = new TH1D("r01","",1024,0,1024);
   r02 = new TH1D("r02","",1024,0,1024);
   r12 = new TH1D("r12","",1024,0,1024);

   g00 = new TH1D("g00","",Scale*N/2,0,Fs/2);
   g11 = new TH1D("g11","",Scale*N/2,0,Fs/2);
   g22 = new TH1D("g22","",Scale*N/2,0,Fs/2);
   g01 = new TH1D("g01","",Scale*N/2,0,Fs/2);
   g02 = new TH1D("g02","",Scale*N/2,0,Fs/2);
   g12 = new TH1D("g12","",Scale*N/2,0,Fs/2);

   x=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);
   y=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N);
   xx=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N*Scale);
   yy=(fftw_complex*)fftw_malloc(sizeof(fftw_complex)*N*Scale);

   
   signala_ext = (fftw_complex *) fftw_malloc(N2 * sizeof(fftw_complex));
   signalb_ext = (fftw_complex *) fftw_malloc(N2 * sizeof(fftw_complex));
   signalc_ext = (fftw_complex *) fftw_malloc(N2 * sizeof(fftw_complex));
   result = (fftw_complex *) fftw_malloc(N2 * sizeof(fftw_complex));

   outa =(fftw_complex *) fftw_malloc(N2 * sizeof(fftw_complex));
   outb = (fftw_complex *) fftw_malloc(N2 * sizeof(fftw_complex));
   outc = (fftw_complex *) fftw_malloc(N2 * sizeof(fftw_complex));
   out = (fftw_complex *) fftw_malloc(N2 * sizeof(fftw_complex));
  
   pa = fftw_plan_dft_1d(N2, signala_ext, outa, FFTW_FORWARD, FFTW_ESTIMATE);
   px = fftw_plan_dft_1d(N2, out, result, FFTW_BACKWARD, FFTW_ESTIMATE);
   pp = fftw_plan_dft_1d(Scale*N, xx, yy, FFTW_FORWARD, FFTW_ESTIMATE);
   
  outatemp = reinterpret_cast<std::complex<double>*>(outa);
  outbtemp = reinterpret_cast<std::complex<double>*>(outb);
  outctemp = reinterpret_cast<std::complex<double>*>(outc);
  outtemp = reinterpret_cast<std::complex<double>*>(out);

  scale = 1.0/N2;
}

void ana::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t ana::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either ana::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  CountEntry++;//自加，每执行一次自加一
  if(CountEntry%1000 == 0) ProgressBar(CountEntry,fChain->GetEntries());

  b_hit0data->GetEntry(entry);
  b_hit1data->GetEntry(entry);
  b_hit2data->GetEntry(entry);
  b_flagfull->GetEntry(entry);

  if(flagfull[0])
    {
      for (int i = 0; i < N; ++i)
	{
	  x[i][0] = hit0data[i];
	  x[i][1] = 0;
	}

      memset(signala_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signala_ext + (N - 1), x, sizeof(fftw_complex) * N);
      memset(signalb_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signalb_ext, x, sizeof(fftw_complex) * N);
      fftw_execute_dft(pa,signala_ext,outa);
      fftw_execute_dft(pa,signalb_ext,outb);

      for (int i = 0; i < N2; ++i)
	outtemp[i] = outatemp[i] * std::conj(outbtemp[i]) * scale;

      fftw_execute_dft(px,out,result);
      for (int i = 0; i < N; ++i)
	blockr00[i] += result[N-1-i][0];
    }
  
  if(flagfull[1])
    {
      for (int i = 0; i < N; ++i)
	{
	  x[i][0] = hit1data[i];
	  x[i][1] = 0;
	}

      memset(signala_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signala_ext + (N - 1), x, sizeof(fftw_complex) * N);
      memset(signalb_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signalb_ext, x, sizeof(fftw_complex) * N);
      fftw_execute_dft(pa,signala_ext,outa);
      fftw_execute_dft(pa,signalb_ext,outb);

      for (int i = 0; i < N2; ++i)
	outtemp[i] = outatemp[i] * std::conj(outbtemp[i]) * scale;

      fftw_execute_dft(px,out,result);
      for (int i = 0; i < N; ++i)
	blockr11[i] += result[N-1-i][0];

    }

  if(flagfull[2])
    {
      for (int i = 0; i < N; ++i)
	{
	  x[i][0] = hit2data[i];
	  x[i][1] = 0;
	}

      memset(signala_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signala_ext + (N - 1), x, sizeof(fftw_complex) * N);
      memset(signalb_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signalb_ext, x, sizeof(fftw_complex) * N);
      fftw_execute_dft(pa,signala_ext,outa);
      fftw_execute_dft(pa,signalb_ext,outb);

      for (int i = 0; i < N2; ++i)
	outtemp[i] = outatemp[i] * std::conj(outbtemp[i]) * scale;

      fftw_execute_dft(px,out,result);
      for (int i = 0; i < N; ++i)
	blockr22[i] += result[N-1-i][0];

    }  

  if(flagfull[0] && flagfull[1])
    {
      for (int i = 0; i < N; ++i)
	{
	  x[i][0] = hit0data[i];
	  x[i][1] = 0;
	  y[i][0] = hit1data[i];
	  y[i][1] = 0;
	}

      memset(signala_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signala_ext + (N - 1), x, sizeof(fftw_complex) * N);
      memset(signalb_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signalb_ext, y, sizeof(fftw_complex) * N);
      fftw_execute_dft(pa,signala_ext,outa);
      fftw_execute_dft(pa,signalb_ext,outb);

      for (int i = 0; i < N2; ++i)
	outtemp[i] = outatemp[i] * std::conj(outbtemp[i]) * scale;

      fftw_execute_dft(px,out,result);
      for (int i = 0; i < N; ++i)
	blockr01[i] += result[N-1-i][0];
    }  

  if(flagfull[0] && flagfull[2])
    {
      for (int i = 0; i < N; ++i)
	{
	  x[i][0] = hit0data[i];
	  x[i][1] = 0;
	  y[i][0] = hit2data[i];
	  y[i][1] = 0;
	}

      memset(signala_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signala_ext + (N - 1), x, sizeof(fftw_complex) * N);
      memset(signalb_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signalb_ext, y, sizeof(fftw_complex) * N);
      fftw_execute_dft(pa,signala_ext,outa);
      fftw_execute_dft(pa,signalb_ext,outb);

      for (int i = 0; i < N2; ++i)
	outtemp[i] = outatemp[i] * std::conj(outbtemp[i]) * scale;

      fftw_execute_dft(px,out,result);
      for (int i = 0; i < N; ++i)
	blockr02[i] += result[N-1-i][0];
    }  

  if(flagfull[1] && flagfull[2])
    {
      for (int i = 0; i < N; ++i)
	{
	  x[i][0] = hit1data[i];
	  x[i][1] = 0;
	  y[i][0] = hit2data[i];
	  y[i][1] = 0;
	}

      memset(signala_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signala_ext + (N - 1), x, sizeof(fftw_complex) * N);
      memset(signalb_ext, 0, sizeof(fftw_complex) * N2);
      memcpy(signalb_ext, y, sizeof(fftw_complex) * N);
      fftw_execute_dft(pa,signala_ext,outa);
      fftw_execute_dft(pa,signalb_ext,outb);

      for (int i = 0; i < N2; ++i)
	outtemp[i] = outatemp[i] * std::conj(outbtemp[i]) * scale;

      fftw_execute_dft(px,out,result);
      for (int i = 0; i < N; ++i)
	blockr12[i] += result[N-1-i][0];
    }    
  
  
   return kTRUE;
}

void ana::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void ana::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  TFile *file = new TFile("output.root","RECREATE");//"RECREATE" "READ"
  if(!file->IsOpen())
    {
      std::cout<<"Can't open root file"<<std::endl;
    }

  for (int i = 0; i < N; ++i)
    {
      r00->SetBinContent(i+1,blockr00[i]);
      r11->SetBinContent(i+1,blockr11[i]);
      r22->SetBinContent(i+1,blockr22[i]);
      r01->SetBinContent(i+1,blockr01[i]);
      r02->SetBinContent(i+1,blockr02[i]);
      r12->SetBinContent(i+1,blockr12[i]);
    }

  memset(xx, 0, sizeof(fftw_complex) * N * Scale);
  for (int i = 0; i < N; ++i)
    {
      xx[i][0] = blockr00[i];
    }
  fftw_execute(pp);
  for (int i = 0; i < Scale*N; ++i)
    {
      yy[i][0] = yy[i][0]/N*2;
      yy[i][1] = yy[i][1]/N*2;
    }
  yy[0][0]/=2.0;
  yy[0][1]/=2.0;
  for (int i = 0; i < Scale*N/2; ++i)
    {
      g00->SetBinContent(i+1,TMath::Sqrt(yy[i][0]*yy[i][0]+yy[i][1]*yy[i][1]));
    }


  memset(xx, 0, sizeof(fftw_complex) * N * Scale);
  for (int i = 0; i < N; ++i)
    {
      xx[i][0] = blockr11[i];
    }
  fftw_execute(pp);
  for (int i = 0; i < Scale*N; ++i)
    {
      yy[i][0] = yy[i][0]/N*2;
      yy[i][1] = yy[i][1]/N*2;
    }
  yy[0][0]/=2.0;
  yy[0][1]/=2.0;
  for (int i = 0; i < Scale*N/2; ++i)
    {
      g11->SetBinContent(i+1,TMath::Sqrt(yy[i][0]*yy[i][0]+yy[i][1]*yy[i][1]));
    }


  memset(xx, 0, sizeof(fftw_complex) * N * Scale);
  for (int i = 0; i < N; ++i)
    {
      xx[i][0] = blockr22[i];
    }
  fftw_execute(pp);
  for (int i = 0; i < Scale*N; ++i)
    {
      yy[i][0] = y[i][0]/N*2;
      yy[i][1] = y[i][1]/N*2;
    }
  yy[0][0]/=2.0;
  yy[0][1]/=2.0;
  for (int i = 0; i < Scale*N/2; ++i)
    {
      g22->SetBinContent(i+1,TMath::Sqrt(yy[i][0]*yy[i][0]+yy[i][1]*yy[i][1]));
    }
  

  memset(xx, 0, sizeof(fftw_complex) * N * Scale);
  for (int i = 0; i < N; ++i)
    {
      xx[i][0] = blockr01[i];
    }
  fftw_execute(pp);
  for (int i = 0; i < Scale*N; ++i)
    {
      yy[i][0] = yy[i][0]/N*2;
      yy[i][1] = yy[i][1]/N*2;
    }
  yy[0][0]/=2.0;
  yy[0][1]/=2.0;
  for (int i = 0; i < Scale*N/2; ++i)
    {
      g01->SetBinContent(i+1,TMath::Sqrt(yy[i][0]*yy[i][0]+yy[i][1]*yy[i][1]));
    }

  memset(xx, 0, sizeof(fftw_complex) * N * Scale);
  for (int i = 0; i < N; ++i)
    {
      xx[i][0] = blockr02[i];
    }
  fftw_execute(pp);
  for (int i = 0; i < Scale*N; ++i)
    {
      yy[i][0] = yy[i][0]/N*2;
      yy[i][1] = yy[i][1]/N*2;
    }
  yy[0][0]/=2.0;
  yy[0][1]/=2.0;
  for (int i = 0; i < Scale*N/2; ++i)
    {
      g02->SetBinContent(i+1,TMath::Sqrt(yy[i][0]*yy[i][0]+yy[i][1]*yy[i][1]));
    }

  memset(xx, 0, sizeof(fftw_complex) * N * Scale);
  for (int i = 0; i < N; ++i)
    {
      xx[i][0] = blockr12[i];
    }
  fftw_execute(pp);
  for (int i = 0; i < Scale*N; ++i)
    {
      yy[i][0] = yy[i][0]/N*2;
      yy[i][1] = yy[i][1]/N*2;
    }
  yy[0][0]/=2.0;
  yy[0][1]/=2.0;
  for (int i = 0; i < Scale*N/2; ++i)
    {
      g12->SetBinContent(i+1,TMath::Sqrt(yy[i][0]*yy[i][0]+yy[i][1]*yy[i][1]));
    }

  
  r00->Write();
  r11->Write();
  r22->Write();
  r01->Write();
  r02->Write();
  r12->Write();

  g00->Write();
  g11->Write();
  g22->Write();
  g01->Write();
  g02->Write();
  g12->Write();

  
  // file->ls("");
  // TObject->Write();
  // TH1D *h = (TH1D*)file->Get("name");
  file->Close();


  gBenchmark->Show("tree");//计时结束并输出时间
}
