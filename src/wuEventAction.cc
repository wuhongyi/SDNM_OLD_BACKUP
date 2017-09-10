// wuEventAction.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 4月 20 14:04:43 2014 (+0800)
// Last-Updated: 日 5月  7 09:52:07 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 26
// URL: http://wuhongyi.cn 

#include "wuEventAction.hh"
#include "Event.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuEventAction::wuEventAction()
 : G4UserEventAction()
{
  //wuG4DataControl* dataControl = wuG4DataControl::Instance();


}

wuEventAction::~wuEventAction()
{

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void wuEventAction::BeginOfEventAction(const G4Event* event)
{
  //event->GetEventID();
  
  // G4cout << "### Event " << event->GetEventID() << " start." << G4endl;

  Event::Instance()->Reset();

  if(event->GetEventID()%10000 == 0) G4cout << "### Event " << event->GetEventID() << G4endl;

  

  
}

void wuEventAction::EndOfEventAction(const G4Event* event)
{
  // G4cout << "### Event " << event->GetEventID() << " end." << G4endl<<G4endl;

  Event::Instance()->FillOneEvent();

  // G4cout<<Event::Instance()->hits<<" ";
  // for (int i = 0; i < Event::Instance()->hits; ++i)
  //   {
  //     G4cout<<Event::Instance()->sid[i]<<"  ";
  //   }
  // G4cout<<G4endl;

  // if(Event::Instance()->hits1 > 0)
  //   {
  //     G4cout<<"Hits1:  "<<Event::Instance()->hits1<<G4endl;

  //     for (int i = 0; i < Event::Instance()->hits1; ++i)
  // 	{
  // 	  G4cout<<Event::Instance()->s1id[i]<<"  "<<Event::Instance()->s1sid[i]<<"  "<<Event::Instance()->s1pid[i]<<"  "<<Event::Instance()->s1time[i]<<"  "<<Event::Instance()->s1e[i]<<G4endl;
  // 	}
  //     G4cout<<G4endl;
  //   }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


// 
// wuEventAction.cc ends here










