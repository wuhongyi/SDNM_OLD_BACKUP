#define MakeBlock_cxx
// The class definition in MakeBlock.h has been generated automatically
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
// root> T->Process("MakeBlock.C")
// root> T->Process("MakeBlock.C","some options")
// root> T->Process("MakeBlock.C+")
//

#include "MakeBlock.h"
#include <TH2.h>
#include <TStyle.h>
#include "TBenchmark.h"
#include <iostream>
#include <cstring>

void MakeBlock::ProgressBar(Long64_t i,Long64_t total)
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


void MakeBlock::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

   gBenchmark->Start("tree");//计时开始
   CountEntry = 0;
   Percent = 0;
   PercentOld = 100;

   totaltime = 0;
   mean = 1000000000.0/1.0e5;
   
   BinNumber = 1024;
   BinWidth = 1;//ns
   FreqWidth = 1000000000.0/(2*BinNumber*BinWidth);

   processtime = 204800000;
   BlockNum = processtime/(BinNumber*BinWidth);
   
   file = new TFile("test.root","RECREATE");//"RECREATE" "READ"
   if(!file->IsOpen())
     {
       std::cout<<"Can't open root file"<<std::endl;
     }
   // file->ls("");
   // TObject->Write();
   // TH1D *h = (TH1D*)file->Get("name");

   tree = new TTree("tree","Hongyi Wu Data");
   tree->Branch("blocknumber",&blocknumber,"blocknumber/I");
   tree->Branch("hit0data",&hit0data,"hit0data[1024]/S");
   tree->Branch("hit1data",&hit1data,"hit1data[1024]/S"); 
   tree->Branch("hit2data",&hit2data,"hit2data[1024]/S");
   tree->Branch("flagfull",&flagfull,"flagfull[3]/O");

   
   memset(hit0data, 0, sizeof(short)*BinNumber);
   memset(hit1data, 0, sizeof(short)*BinNumber);
   memset(hit2data, 0, sizeof(short)*BinNumber);

   blocknumber = 0;
   flagfull[0] = false;
   flagfull[1] = false;
   flagfull[2] = false;




   
}

void MakeBlock::SlaveBegin(TTree * /*tree*/)
{
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave server.
   // The tree argument is deprecated (on PROOF 0 is passed).

   TString option = GetOption();

}

Bool_t MakeBlock::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either MakeBlock::GetEntry() or TBranch::GetEntry()
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
  if(CountEntry%10000 == 0) ProgressBar(CountEntry,fChain->GetEntries());

  b_hits->GetEntry(entry);
  b_sid->GetEntry(entry);
  b_hits1->GetEntry(entry);
  b_hits2->GetEntry(entry);

  double random = gRandom->Exp(mean);
  totaltime += random;

  if(totaltime > processtime)//处理一次
    {
      std::cout<<data0.size()<<"  "<<data1.size()<<"  "<<data2.size()<<std::endl;

      int jj = 0;
      int jjj = 0;
      int jjjj = 0;
      
      for (int i = 0; i < BlockNum; ++i)
	{
	  Long64_t frontedge = i*(BinNumber*BinWidth);
	  Long64_t endedge = (i+1)*(BinNumber*BinWidth);
	  memset(hit0data, 0, sizeof(short)*BinNumber);
	  memset(hit1data, 0, sizeof(short)*BinNumber);
	  memset(hit2data, 0, sizeof(short)*BinNumber);
	  flagfull[0] = false;
	  flagfull[1] = false;
	  flagfull[2] = false;

	  
	  for (int j = jj; j < data0.size(); ++j)
	    {
	      if(data0[j] >= endedge)
		{
		  jj = j;
		  break;
		}
	      if(data0[j] >= frontedge)
		{
		  hit0data[int(data0[j]-frontedge)] += 1;
		  flagfull[0] = true;
		}
	    }
	  
	  for (int j = jjj; j < data1.size(); ++j)
	    {
	      if(data1[j] >= endedge)
		{
		  jjj = j;
		  break;
		}
	      if(data1[j] >= frontedge)
		{
		  hit1data[int(data1[j]-frontedge)] += 1;
		  flagfull[1] = true;
		}
	    }

	  for (int j = jjjj; j < data2.size(); ++j)
	    {
	      if(data2[j] >= endedge)
		{
		  jjjj = j;
		  break;
		}
	      if(data2[j] >= frontedge)
		{
		  hit2data[int(data2[j]-frontedge)] += 1;
		  flagfull[2] = true;
		}
	    }	  

	  if(flagfull[0] || flagfull[1] || flagfull[2])
	    {
	      tree->Fill();//loop
	      blocknumber++;
	    }
	  
	}
     
      data0.clear();
      data1.clear();
      data2.clear();
      totaltime = random;
    }


  data0.push_back(totaltime);

  if(hits1 > 0)
    {
      b_s1time->GetEntry(entry);
      b_s1sid->GetEntry(entry);
      b_s1pid->GetEntry(entry);
      
      for (int i = 0; i < hits1; ++i)
	{
	  if(TMath::Abs(s1sid[i]) != TMath::Abs(s1pid[i]))
	    {
	      data1.push_back(totaltime+Long64_t(s1time[i]));
	    }
	}
    }
  
  if(hits2 > 0)
    {
      b_s2time->GetEntry(entry);
      b_s2sid->GetEntry(entry);
      b_s2pid->GetEntry(entry);
      
      for (int i = 0; i < hits2; ++i)
	{
	  if(TMath::Abs(s2sid[i]) != TMath::Abs(s2pid[i]))
	    {
	      data2.push_back(totaltime+Long64_t(s2time[i]));
	    }
	}
    }

  
  // if(random > 30)
  //   {
  //     if(totaltime/BinNumber > blocknumber)
  // 	{//下一个block
  // 	  copy0[totaltime%BinNumber] = 1;
  // 	  flagfull[0] = true;
  // 	}
  //     else
  // 	{//当前block
  // 	  hit0data[totaltime%BinNumber] = 1;
  // 	}
  //   }

  // if(hits1 > 0)
  //   {
  //     b_s1time->GetEntry(entry);
  //     b_s1sid->GetEntry(entry);
  //     b_s1pid->GetEntry(entry);
      
  //     for (int i = 0; i < hits1; ++i)
  // 	{
  // 	  if(TMath::Abs(s1sid[i]) != TMath::Abs(s1pid[i]))
  // 	    {
  // 	      totaltimetemp = (totaltime+int(s1time[i])) + gRandom->Gaus(0,10);
  // 	      if(totaltimetemp/BinNumber > blocknumber)
  // 		{//下一个block
  // 		  copy1[totaltimetemp%BinNumber] = 1;
  // 		  flagfull[1] = true;
  // 		}
  // 	      else
  // 		{//当前block
  // 		  hit1data[totaltimetemp%BinNumber] = 1;
  // 		}
  // 	    }
  // 	}
  //   }

  // if(hits2 > 0)
  //   {
  //     b_s2time->GetEntry(entry);
  //     b_s2sid->GetEntry(entry);
  //     b_s2pid->GetEntry(entry);
      
  //     for (int i = 0; i < hits2; ++i)
  // 	{
  // 	  if(TMath::Abs(s2sid[i]) != TMath::Abs(s2pid[i]))
  // 	    {
  // 	      totaltimetemp = (totaltime+int(s2time[i])) + gRandom->Gaus(0,10);
  // 	      if(totaltimetemp/BinNumber > blocknumber)
  // 		{//下一个block
  // 		  copy2[totaltimetemp%BinNumber] = 1;
  // 		  flagfull[2] = true;
  // 		}
  // 	      else
  // 		{//当前block
  // 		  hit2data[totaltimetemp%BinNumber] = 1;
  // 		}
  // 	    }
  // 	}
  //   }

  // if(flagfull[0] && flagfull[1] && flagfull[2])
  //   {
  //     tree->Fill();//loop
  //     flagfull[0] = false;
  //     flagfull[1] = false;
  //     flagfull[2] = false;
  //     blocknumber++;
      
  //     memcpy(hit0data,copy0,sizeof(short)*BinNumber);
  //     memcpy(hit1data,copy1,sizeof(short)*BinNumber);
  //     memcpy(hit2data,copy2,sizeof(short)*BinNumber);
      

  //   }
  

   return kTRUE;
}

void MakeBlock::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

}

void MakeBlock::Terminate()
{
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.

  std::cout<<std::endl<<"totaltime: "<<totaltime<<"  Block size: "<<totaltime/(BinNumber*BinWidth)+1<<std::endl;
  
  file->cd();
  
  tree->Write();

  file->Close();

  gBenchmark->Show("tree");//计时结束并输出时间
  
}
