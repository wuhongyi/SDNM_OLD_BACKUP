// wuSteppingAction.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 4月 20 15:23:09 2014 (+0800)
// Last-Updated: 四 5月 25 21:47:19 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 181
// URL: http://wuhongyi.cn 

#include "wuSteppingAction.hh"
#include "G4TrackVector.hh"
#include "G4SteppingManager.hh"

#include "wuTrackInformation.hh"
#include "wuRegionInformation.hh"
#include "wuReadData.hh"
#include "Event.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuSteppingAction::wuSteppingAction(G4String fn)
{
  G4cout<<"creating wuSteppingAction... "<<G4endl;

  fnInput=fn;


}

wuSteppingAction::~wuSteppingAction()
{

  G4cout<<"deleting wuSteppingAction... "<<G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void wuSteppingAction::UserSteppingAction(const G4Step* step)
{
// Collect data step by step

// Information in G4Step includes:
// Pointers to PreStep and PostStepPoint
// Geometrical step length (step length before the correction of multiple scattering)
// True step length (step length after the correction of multiple scattering)
// Increment of position and time between PreStepPoint and PostStepPoint
// Increment of momentum and energy between PreStepPoint and PostStepPoint. (Note: to get the en-
// ergy deposited in the step, you cannot use this 'Delta energy'. You have to use 'Total energy deposit' as below.)
// Pointer to G4Track
// Total energy deposited during the step - this is the sum of
// • the energy deposited by the energy loss process, and
// • the energy lost by secondaries which have NOT been generated because each of their energies was below
// the cut threshold
// Energy deposited not by ionization during the step
// Secondary tracks created during tracking for the current track.
// • NOTE: all secondaries are included. NOT only secondaries created in the CURRENT step.

  // energy deposit
  EDep = step->GetTotalEnergyDeposit();

  //Filter #1 energy deposit threshold for ONE step
  // if(EDep<FilterEdep) return ;//！

  // get volume of the step//留着接口，以后用来筛选需要记录信息的几何体
  // G4VPhysicalVolume* volume 
  //   = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

  // step length
  StepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    StepLength = step->GetStepLength();
  }



  G4Track* aTrack=step->GetTrack();//获取数据接口
  G4ParticleDefinition* theparticle=aTrack->GetDefinition();
  PName=theparticle->GetParticleName();

  // //Filter #2 According to particle name
  // G4bool fpname=true; //flag of particle name
  // size_t nwith=FilterPwith->size();
  // size_t nwithout=FilterPwithout->size();
  // if(nwith>0 ){   
  //   for(size_t i=0; i<nwith; i++){
  //     if((string)PName ==(*FilterPwith)[i])break;
  //     if(i==nwith-1)fpname=false;
  //   }
  // }else if(nwithout>0){//if nwith>0, ignore nwithout
  //   for(size_t i=0; i<nwithout; i++){
  //     if((string)PName==(*FilterPwithout)[i]){
  // 	fpname=false;
  // 	break;
  //     }
  //   } 
  // }
  // if(!fpname) return;



  const G4DynamicParticle* dyParticle=aTrack->GetDynamicParticle();//获取数据接口

  EventID=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  TrackID=aTrack->GetTrackID();
  ParentID=aTrack->GetParentID();
  CurrentStepNumber=aTrack->GetCurrentStepNumber();
  TrackLength=aTrack->GetTrackLength();
  TrackStatus=aTrack->GetTrackStatus();


// Information in G4StepPoint (PreStepPoint and PostStepPoint) includes:
// (x, y, z, t)
// (px, py, pz, Ek)
// Momentum direction (unit vector)
// Pointers to physical volumes
// Safety
// Beta, gamma
// Polarization
// Step status
// Pointer to the physics process which defined the current step and its DoIt type
// Pointer to the physics process which defined the previous step and its DoIt type
// Total track length
// Global time (time since the current event began)
// Local time (time since the current track began)
// Proper time

  G4StepPoint* preStepPoint=step->GetPreStepPoint();//获取数据接口
  G4StepPoint* postStepPoint=step->GetPostStepPoint();//获取数据接口

  preStepVolume = preStepPoint->GetPhysicalVolume()->GetLogicalVolume();//RegionInformation用
  wuRegionInformation* thePreRInfo
   = (wuRegionInformation*)(preStepVolume->GetRegion()->GetUserInformation());
  // postStepVolume = postStepPoint->GetPhysicalVolume()->GetLogicalVolume();//这个加上就错误,应该是出world造成的，想要用可以加一个判断是否在world外
  // wuRegionInformation* thePostRInfo
  //  = (wuRegionInformation*)(postStepVolume->GetRegion()->GetUserInformation());
  // check if it is entering to the volume
  
  // if(FilterRegion==1)
  //   {
  //     if(!(thePreRInfo->IsRegion()))
  // 	{
  // 	  return ;
  // 	}
  //   }
  
  // Dynamical data of the particle
  Mass=preStepPoint->GetMass();
  Charge=preStepPoint->GetCharge();
  MagneticMoment=preStepPoint->GetMagneticMoment();
  TrackWeight=preStepPoint->GetWeight(); 


  PosPre=preStepPoint->GetPosition();
  EkPre=preStepPoint->GetKineticEnergy();
  GlobalTimePre=preStepPoint->GetGlobalTime();
  LocalTimePre=preStepPoint->GetLocalTime();
  VelocityPre=preStepPoint->GetVelocity();
  MomentumDirectionPre=preStepPoint->GetMomentumDirection();
  VolNamePre=preStepPoint->GetPhysicalVolume()->GetName();
  StepStatusPre=preStepPoint->GetStepStatus();
  ProperTimePre=preStepPoint->GetProperTime();


  const G4VProcess* pcr=aTrack->GetCreatorProcess();
  if(pcr) CreatorProcess=pcr->GetProcessName();
  else CreatorProcess="##";
  

  PosPost=postStepPoint->GetPosition();
  EkPost=postStepPoint->GetKineticEnergy();
  GlobalTimePost=postStepPoint->GetGlobalTime();
  LocalTimePost=postStepPoint->GetLocalTime();
  VelocityPost=postStepPoint->GetVelocity();
  MomentumDirectionPost=postStepPoint->GetMomentumDirection();
  // VolNamePost=postStepPoint->GetPhysicalVolume()->GetName();//这个加上就错误,应该是出world造成的，想要用可以加一个判断是否在world外
  VolNamePost="**";//暂时用这个表示
  StepStatusPost=postStepPoint->GetStepStatus();
  ProperTimePost=postStepPoint->GetProperTime();

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  wuTrackInformation* trackInfo = (wuTrackInformation*)(aTrack->GetUserInformation());
  Info= trackInfo->PrintFlag();
  


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  if(!((thePreRInfo->GetRegionNumber()==2)||(thePreRInfo->GetRegionNumber()==3)||(thePreRInfo->GetRegionNumber()==4)||(thePreRInfo->GetRegionNumber()==5))) return;
  if(PName[0]=='n' || EDep < 0.01 ) return;

  Event* event = Event::Instance();
  
  if(PName[0] == 'p')
    event->trackid = TrackID;
  else
    {
      if(PName[0] == 'e')
	event->trackid = -TrackID;
      else
	return;
	// G4cout<<"wuSteppingAction Error..."<<G4endl;
    }

 
  if(thePreRInfo->GetRegionNumber()==2)
    {
      event->scin[0] = true;
      if(GlobalTimePre < event->scintime[0]) event->scintime[0] = GlobalTimePre;
      event->scinenergy[0] += EDep;
    }

  if(thePreRInfo->GetRegionNumber()==3)
    {
      event->scin[1] = true;
      if(GlobalTimePre < event->scintime[1]) event->scintime[1] = GlobalTimePre;
      event->scinenergy[1] += EDep;
    }  
  
  if(thePreRInfo->GetRegionNumber()==4)
    {
      event->scin[2] = true;
      if(GlobalTimePre < event->scintime[2]) event->scintime[2] = GlobalTimePre;
      event->scinenergy[2] += EDep;
    } 

  if(thePreRInfo->GetRegionNumber()==5)
    {
      event->scin[3] = true;
      if(GlobalTimePre < event->scintime[3]) event->scintime[3] = GlobalTimePre;
      event->scinenergy[3] += EDep;
    }   
  
  event->scininfo = Info;

  event->scinpid = ParentID;
  
  // if(trackInfo->GetFissionFlag())
  //   event->scinpid = -ParentID;
  // else
  //   event->scinpid = ParentID;

  
  // G4cout<<EventID<< "  "<<TrackID<<"  "<<ParentID<<"  "<<CurrentStepNumber<<"  "<<PName<<"  "<<CreatorProcess<<"  "<<VolNamePre<<"  "<<GlobalTimePre<<"  "<<EkPre<<"  "<<EDep<<"  "<<Info<<"  "<<thePreRInfo->GetRegionNumber()<<G4endl;







//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//功能测试   
   //  G4SteppingManager*  steppingManager = fpSteppingManager;
   //  G4TrackVector* fSecondary = steppingManager -> GetfSecondary();
     
   // for(size_t lp1=0;lp1<(*fSecondary).size(); lp1++)
   // {
   //   std::cout<<lp1<<std::endl;
   // }
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
  //这个可以用来筛选几何体的信息输出，以后想输出哪个几何体里面的信息就筛选。代替构建SD
  // if ( volume == fDetConstruction->GetAbsorberPV() ) {
  //   fEventAction->AddAbs(edep,stepLength);
  // }
  
  // if ( volume == fDetConstruction->GetGapPV() ) {
  //   fEventAction->AddGap(edep,stepLength);
  // }

  // G4String volName; 
  // if (track->GetVolume()) volName =  track->GetVolume()->GetName(); 
  // G4String nextVolName;
  // if (track->GetNextVolume()) nextVolName =  track->GetNextVolume()->GetName();

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// 
// wuSteppingAction.cc ends here
