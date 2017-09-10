// Event.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 5月  4 12:55:13 2017 (+0800)
// Last-Updated: 四 5月 25 21:53:21 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 37
// URL: http://wuhongyi.cn 

#include "Event.hh"
#include <cfloat>
#include "TSystem.h"

G4ThreadLocal Event* Event::fgInstance = 0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Event::Event()
{
  fgInstance = this;

}

Event::~Event()
{

}

Event* Event::Instance()
{
  if ( fgInstance == 0 ) {
    fgInstance = new Event;
  }
  
  return fgInstance;
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Event::Reset()
{
  hits = 0;
  hits1 = 0;
  hits2 = 0;
  hits3 = 0;
  hits4 = 0;

}

void Event::ResetTrack()
{
  if(scin[0]) counttest[0]++;
  if(scin[1]) counttest[1]++;
  if(scin[2]) counttest[2]++;
  if(scin[3]) counttest[3]++;
  
  scin[0] = false;
  scin[1] = false;
  scin[2] = false;
  scin[3] = false;
  trackid = -1;
  scinenergy[0] = 0;
  scinenergy[1] = 0;
  scinenergy[2] = 0;
  scinenergy[3] = 0;
  scintime[0] = DBL_MAX;
  scintime[1] = DBL_MAX;
  scintime[2] = DBL_MAX;
  scintime[3] = DBL_MAX;
}

void Event::ProcessOneTrack()
{
  bool flag = false;
  int flagint = -1;
  if(scin[0] && scinenergy[0] > 0.1)
    {
      for (int i = 0; i < hits1; ++i)
	{
	  if(scinpid == s1pid[i])
	    {
	      flag = true;
	      flagint = i;
	    }
	}

      if(flag)
	{
	  s1e[flagint] += scinenergy[0];
	}
      else
	{
	  s1id[hits1] = trackid;
	  s1time[hits1] = scintime[0];
	  s1sid[hits1] = scininfo;
	  s1e[hits1] = scinenergy[0];
	  s1pid[hits1] = scinpid;

	  hits1++;
	  counttest[0]++;
	}
    }

  // ==
  
  if(scin[1] && scinenergy[1] > 0.1)
    {
      for (int i = 0; i < hits2; ++i)
	{
	  if(scinpid == s2pid[i])
	    {
	      flag = true;
	      flagint = i;
	    }
	}

      if(flag)
	{
	  s2e[flagint] += scinenergy[1];
	}
      else
	{
	  s2id[hits2] = trackid;
	  s2time[hits2] = scintime[1];
	  s2sid[hits2] = scininfo;
	  s2e[hits2] = scinenergy[1];
	  s2pid[hits2] = scinpid;


	  hits2++;
	  counttest[1]++;
	}
    }

  // ==

  if(scin[2] && scinenergy[2] > 0.1)
    {
      for (int i = 0; i < hits3; ++i)
	{
	  if(scinpid == s3pid[i])
	    {
	      flag = true;
	      flagint = i;
	    }
	}

      if(flag)
	{
	  s3e[flagint] += scinenergy[2];
	}
      else
	{
	  s3id[hits3] = trackid;
	  s3time[hits3] = scintime[2];
	  s3sid[hits3] = scininfo;
	  s3e[hits3] = scinenergy[2];
	  s3pid[hits3] = scinpid;

	  hits3++;
	  counttest[2]++;
	}
    }

  // ==

  if(scin[3] && scinenergy[3] > 0.1)
    {
      for (int i = 0; i < hits4; ++i)
	{
	  if(scinpid == s4pid[i])
	    {
	      flag = true;
	      flagint = i;
	    }
	}

      if(flag)
	{
	  s4e[flagint] += scinenergy[3];
	}
      else
	{
	  s4id[hits4] = trackid;
	  s4time[hits4] = scintime[3];
	  s4sid[hits4] = scininfo;
	  s4e[hits4] = scinenergy[3];
	  s4pid[hits4] = scinpid;

	  hits4++;
	  counttest[3]++;
	}
    }
  
  
  
}

void Event::InitWriteFile()
{
  char rootfilename[1024];
  sprintf(rootfilename,"sdnm%d_%d.root",runnumber,gSystem->GetPid());
  file = new TFile(rootfilename,"RECREATE");
  t = new TTree("tree","PKU Geant4 Data");

  t->Branch("hits",&hits,"hits/S");
  t->Branch("sid",&sid,"sid[hits]/I");
  
  t->Branch("hits1",&hits1,"hits1/S");
  t->Branch("s1id",&s1id,"s1id[hits1]/I");
  t->Branch("s1time",&s1time,"s1time[hits1]/D");
  t->Branch("s1sid",&s1sid,"s1sid[hits1]/I");
  t->Branch("s1pid",&s1pid,"s1pid[hits1]/I");
  t->Branch("s1e",&s1e,"s1e[hits1]/D");

  t->Branch("hits2",&hits2,"hits2/S");
  t->Branch("s2id",&s2id,"s2id[hits2]/I");
  t->Branch("s2time",&s2time,"s2time[hits2]/D");
  t->Branch("s2sid",&s2sid,"s2sid[hits2]/I");
  t->Branch("s2pid",&s2pid,"s2pid[hits2]/I");
  t->Branch("s2e",&s2e,"s2e[hits2]/D");

  t->Branch("hits3",&hits3,"hits3/S");
  t->Branch("s3id",&s3id,"s3id[hits3]/I");
  t->Branch("s3time",&s3time,"s3time[hits3]/D");
  t->Branch("s3sid",&s3sid,"s3sid[hits3]/I");
  t->Branch("s3pid",&s3pid,"s3pid[hits3]/I");
  t->Branch("s3e",&s3e,"s3e[hits3]/D");

  t->Branch("hits4",&hits4,"hits4/S");
  t->Branch("s4id",&s4id,"s4id[hits4]/I");
  t->Branch("s4time",&s4time,"s4time[hits4]/D");
  t->Branch("s4sid",&s4sid,"s4sid[hits4]/I");
  t->Branch("s4pid",&s4pid,"s4pid[hits4]/I");
  t->Branch("s4e",&s4e,"s4e[hits4]/D");

  
  // t->Branch("",&,"/");


  counttest[0] = 0;
  counttest[1] = 0;
  counttest[2] = 0;
  counttest[3] = 0;
}

void Event::CloseWriteFile()
{
  file->cd();
  t->Write();
  file->Close();
  G4cout << "@@@@@@@@@@@: "<<counttest[0]<<"   "<<counttest[1]<<"   "<<counttest[2]<<"   "<<counttest[3]<<G4endl;
}

void Event::FillOneEvent()
{
  t->Fill();
}

// 
// Event.cc ends here
