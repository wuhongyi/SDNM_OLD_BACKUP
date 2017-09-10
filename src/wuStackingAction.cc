// wuStackingAction.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 12月 20 19:59:23 2014 (+0800)
// Last-Updated: 五 5月  5 10:59:14 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

#include "wuStackingAction.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuStackingAction::wuStackingAction()
:G4UserStackingAction()
{
 
}

wuStackingAction::~wuStackingAction()
{

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4ClassificationOfNewTrack wuStackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
//---------------------------------------------------------------
//    Reply G4ClassificationOfNewTrack determined by the
//  newly coming G4Track.
//
//    enum G4ClassificationOfNewTrack
//    {
//      fUrgent,    // put into the urgent stack
//      fWaiting,   // put into the waiting stack
//      fPostpone,  // postpone to the next event
//      fKill       // kill without stacking
//    };
//
//    The parent_ID of the track indicates the origin of it.
//                
//    G4int parent_ID = aTrack->get_parentID();
//   
//      parent_ID = 0 : primary particle
//                > 0 : secondary particle
//                < 0 : postponed from the previous event
//
//---------------------------------------------------------------

  G4ParticleDefinition* particleType = aTrack->GetDefinition();
  
  G4ClassificationOfNewTrack classification = fUrgent; 

  if(particleType != G4Neutron::NeutronDefinition() &&
     particleType != G4Proton::ProtonDefinition() &&
     particleType != G4Gamma::GammaDefinition() &&
     particleType != G4Electron::ElectronDefinition() &&
     particleType != G4Alpha::AlphaDefinition() )
    {
      classification = fKill;
    }


  // if(particleType == G4Electron::ElectronDefinition())
  // {
  //   if(aTrack->GetCreatorProcess())
  //   {
  //      G4String ProName= aTrack->GetCreatorProcess()->GetProcessName();
  //      if(ProName == "eIoni" || ProName == "hIoni") classification = fKill; 
  //   }
  //  if (aTrack->GetVolume()->GetLogicalVolume() ==  fDet->GetLVTest() ) classification = fKill;  
  // }

  

  return classification;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  //这是一个例子
  //keep primary particle
  if (aTrack->GetParentID() == 0) return fUrgent;

  //kill all secondaries  
  return fKill;



}

void wuStackingAction::NewStage()
{
//---------------------------------------------------------------
//    This method is called by G4StackManager when the urgentStack
//  becomes empty and contents in the waitingStack are transtered
//  to the urgentStack.
//    Note that this method is not called at the begining of each
//  event, but "PrepareNewEvent" is called.
//
//    In case re-classification of the stacked tracks is needed,
//  use the following method to request to G4StackManager.
//
//    stackManager->ReClassify();
//
//  All of the stacked tracks in the waitingStack will be re-classified 
//  by "ClassifyNewTrack" method.
//    To abort current event, use the following method.
//
//    stackManager->clear();
//
//  Note that this way is valid and safe only for the case it is called
//  from this user class. The more global way of event abortion is
//
//    G4UImanager * UImanager = G4UImanager::GetUIpointer();
//    UImanager->ApplyCommand("/event/abort");
//
//---------------------------------------------------------------


  ;
}

void wuStackingAction::PrepareNewEvent()
{
//---------------------------------------------------------------
//    This method is called by G4StackManager at the begining of
//  each event.
//    Be careful that the urgentStack and the waitingStack of 
//  G4StackManager are empty at this moment, because this method
//  is called before accepting primary particles. Also, note that
//  the postponeStack of G4StackManager may have some postponed
//  tracks.
//
//---------------------------------------------------------------


  ;
}

// 
// wuStackingAction.cc ends here
