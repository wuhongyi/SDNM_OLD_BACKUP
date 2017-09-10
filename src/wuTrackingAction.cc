// wuTrackingAction.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 12月 13 20:12:34 2014 (+0800)
// Last-Updated: 五 5月  5 20:58:00 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 125
// URL: http://wuhongyi.cn 

#include "wuTrackInformation.hh"
#include "wuTrackingAction.hh"
#include "Event.hh"

#include "globals.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4ParticleTypes.hh"
#include "G4TrackingManager.hh"
#include "G4AutoDelete.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuTrackingAction::wuTrackingAction()
  : G4UserTrackingAction()
{

}
wuTrackingAction::~wuTrackingAction()
{

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void wuTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  // G4cout<<"BeginOfPreUserTrackingAction..."<<G4endl;
  // G4cout<<"Begin of track: "<<aTrack->GetTrackID()<<G4endl;
  
  
  wuTrackInformation* trackInformation;// = new wuTrackInformation();
  if(!aTrack->GetUserInformation()) 
    {
      trackInformation = new wuTrackInformation();
      fpTrackingManager->SetUserTrackInformation(trackInformation);
    }
  else
    {
      trackInformation = (wuTrackInformation*)(aTrack->GetUserInformation());
    }
  
  if (aTrack->GetParentID() == 0) 
    {
      Event *event = Event::Instance();
      if(aTrack->GetDefinition()->GetParticleName() == "gamma")
	{
	  trackInformation->AddFlag(-aTrack->GetTrackID());
	  
	  event->sid[event->hits] = -aTrack->GetTrackID();
	  event->hits++;
	}
      else
	{
	  trackInformation->AddFlag(aTrack->GetTrackID());

	  event->sid[event->hits] = aTrack->GetTrackID();
	  event->hits++;
	}
      // fpTrackingManager->SetUserTrackInformation(trackInformation);
    }


  // G4String CreatorProcess;
  // const G4VProcess* pcr=aTrack->GetCreatorProcess();
  // if(pcr) CreatorProcess=pcr->GetProcessName();
  // else CreatorProcess="##";
  // if(CreatorProcess == "nFission")
  //   {
  //     trackInformation->SetFissionFlag(true);
  //   }



  
  Event::Instance()->ResetTrack();

  
}

void wuTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{

  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
    {
      wuTrackInformation* info = (wuTrackInformation*)(aTrack->GetUserInformation());
      
      size_t nSeco = secondaries->size();

      if(nSeco>0)
  	{
  	  for(size_t i=0;i<nSeco;i++)
  	    { 
  	      wuTrackInformation* infoNew = new wuTrackInformation(info);

	      // G4String CreatorProcess;
	      // const G4VProcess* pcr=(*secondaries)[i]->GetCreatorProcess();
	      // if(pcr) CreatorProcess=pcr->GetProcessName();
	      // else CreatorProcess="##";
	      // if(CreatorProcess == "nFission")
	      // 	{
	      // 	  infoNew->SetFissionFlag(true);
	      // 	}

	      (*secondaries)[i]->SetUserInformation(infoNew);
  	    }
  	}

    }


  Event::Instance()->ProcessOneTrack();




  
  // G4cout<<"End of track: "<<aTrack->GetTrackID()<<G4endl<<G4endl;
}




// 
// wuTrackingAction.cc ends here
