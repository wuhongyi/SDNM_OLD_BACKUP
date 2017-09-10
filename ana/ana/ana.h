//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu May 11 16:12:56 2017 by ROOT version 6.08/06
// from TChain tree/
//////////////////////////////////////////////////////////

#ifndef ana_h
#define ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "TH1.h"
#include <complex>
#include <fftw3.h>
#include <cstring>
#include "TMath.h"



// Header file for the classes stored in the TTree if any.

class ana : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           blocknumber;
   Short_t         hit0data[1024];
   Short_t         hit1data[1024];
   Short_t         hit2data[1024];
   Bool_t          flagfull[3];
   
   // List of branches
   TBranch        *b_blocknumber;   //!
   TBranch        *b_hit0data;   //!
   TBranch        *b_hit1data;   //!
   TBranch        *b_hit2data;   //!
   TBranch        *b_flagfull;   //!
   
   ana(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~ana() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   // wuhongyi
   Long64_t CountEntry;//用来统计已经处理的enery个数
   Int_t Percent;//用来表示处理进度
   Int_t PercentOld;//用来表示处理进度
   void ProgressBar(Long64_t i,Long64_t total);
   
   void Autocorrelation(int n,Short_t *in1,Short_t *in2,double *out);

   TH1D *r00,*r11,*r22;
   TH1D *r01,*r02,*r12;

   TH1D *g00,*g11,*g22;
   TH1D *g01,*g02,*g12;
   
   int N;
   int N2;
   int Fs;
   int Scale;
   double blockr00[1024];
   double blockr11[1024];
   double blockr22[1024];
   double blockr01[1024];
   double blockr02[1024];
   double blockr12[1024];
   double blocktemp[1024];


   fftw_complex *x,*y;
   fftw_complex *xx,*yy;
   fftw_complex *signala_ext;
   fftw_complex *signalb_ext;
   fftw_complex *signalc_ext;
   fftw_complex *result;
   fftw_complex *outa,*outb,*outc,*out;
   fftw_plan pa,pb,pc,px,pp;
   std::complex<double> *outatemp;
   std::complex<double> *outbtemp;
   std::complex<double> *outctemp;
   std::complex<double> *outtemp;

   std::complex<double> scale;

   
   ClassDef(ana,0);
};

#endif

#ifdef ana_cxx
void ana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("blocknumber", &blocknumber, &b_blocknumber);
   fChain->SetBranchAddress("hit0data", hit0data, &b_hit0data);
   fChain->SetBranchAddress("hit1data", hit1data, &b_hit1data);
   fChain->SetBranchAddress("hit2data", hit2data, &b_hit2data);
   fChain->SetBranchAddress("flagfull", flagfull, &b_flagfull);
}

Bool_t ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef ana_cxx
