//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu May 11 15:07:02 2017 by ROOT version 6.08/06
// from TChain tree/
//////////////////////////////////////////////////////////

#ifndef MakeBlock_h
#define MakeBlock_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>

#include "TFile.h"
#include "TTree.h"
#include <vector>

// Header file for the classes stored in the TTree if any.

class MakeBlock : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Short_t         hits;
   Int_t           sid[1024];   //[hits]
   Short_t         hits1;
   Int_t           s1id[1024];   //[hits1]
   Double_t        s1time[1024];   //[hits1]
   Int_t           s1sid[1024];   //[hits1]
   Int_t           s1pid[1024];   //[hits1]
   Double_t        s1e[1024];   //[hits1]
   Short_t         hits2;
   Int_t           s2id[1024];   //[hits2]
   Double_t        s2time[1024];   //[hits2]
   Int_t           s2sid[1024];   //[hits2]
   Int_t           s2pid[1024];   //[hits2]
   Double_t        s2e[1024];   //[hits2]

   // List of branches
   TBranch        *b_hits;   //!
   TBranch        *b_sid;   //!
   TBranch        *b_hits1;   //!
   TBranch        *b_s1id;   //!
   TBranch        *b_s1time;   //!
   TBranch        *b_s1sid;   //!
   TBranch        *b_s1pid;   //!
   TBranch        *b_s1e;   //!
   TBranch        *b_hits2;   //!
   TBranch        *b_s2id;   //!
   TBranch        *b_s2time;   //!
   TBranch        *b_s2sid;   //!
   TBranch        *b_s2pid;   //!
   TBranch        *b_s2e;   //!

   MakeBlock(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~MakeBlock() { }
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

  TFile *file;
  TTree *tree;
  short hit0data[1024];
  short hit1data[1024];
  short hit2data[1024];
  
  int blocknumber;
  bool flagfull[3];
  
  int BinNumber;//ok  
  int BinWidth;//ok
  double FreqWidth;

  Long64_t totaltime;
  Long64_t totaltimetemp;
  double mean;

  Long64_t processtime;
  int BlockNum;

  std::vector<Long64_t>  data0;
  std::vector<Long64_t>  data1;
  std::vector<Long64_t>  data2;
  
  void Autocorrelation(int n,double *in1,double *in2,double *out);

  
   ClassDef(MakeBlock,0);
};

#endif

#ifdef MakeBlock_cxx
void MakeBlock::Init(TTree *tree)
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

   fChain->SetBranchAddress("hits", &hits, &b_hits);
   fChain->SetBranchAddress("sid", sid, &b_sid);
   fChain->SetBranchAddress("hits1", &hits1, &b_hits1);
   fChain->SetBranchAddress("s1id", s1id, &b_s1id);
   fChain->SetBranchAddress("s1time", s1time, &b_s1time);
   fChain->SetBranchAddress("s1sid", s1sid, &b_s1sid);
   fChain->SetBranchAddress("s1pid", s1pid, &b_s1pid);
   fChain->SetBranchAddress("s1e", s1e, &b_s1e);
   fChain->SetBranchAddress("hits2", &hits2, &b_hits2);
   fChain->SetBranchAddress("s2id", s2id, &b_s2id);
   fChain->SetBranchAddress("s2time", s2time, &b_s2time);
   fChain->SetBranchAddress("s2sid", s2sid, &b_s2sid);
   fChain->SetBranchAddress("s2pid", s2pid, &b_s2pid);
   fChain->SetBranchAddress("s2e", s2e, &b_s2e);
}

Bool_t MakeBlock::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef MakeBlock_cxx
