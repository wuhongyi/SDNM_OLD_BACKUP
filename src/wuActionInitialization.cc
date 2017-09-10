// wuActionInitialization.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 5月 20 11:28:49 2014 (+0800)
// Last-Updated: 五 5月  5 22:07:52 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 133
// URL: http://wuhongyi.cn 

#include "wuActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"

#include "wuEventAction.hh"

#include "wuRunAction.hh"
// #include "wuRunActionSD.hh"
// #include "wuSteppingAction.hh"
// #include "wuRunActionTimer.hh"
// #include "wuEventActionLog.hh"
// #include "wuEventActionSD.hh"

// #include "wuVRunAction.hh"
// #include "wuVEventAction.hh"
// #include "wuVTrackingAction.hh"

#include "wuEventAction.hh"
#include "wuStackingAction.hh"
#include "wuTrackingAction.hh"
#include "wuSteppingAction.hh"

#include "wuReadData.hh"

#include "G4Version.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

wuActionInitialization::wuActionInitialization()
 : G4VUserActionInitialization()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

wuActionInitialization::~wuActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void wuActionInitialization::BuildForMaster() const
{

  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void wuActionInitialization::Build() const
{

  SetUserAction(new PrimaryGeneratorAction);

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  

  // wuVRunAction* actsRun = new wuVRunAction;
  // wuVEventAction* actsEvent = new wuVEventAction;
  // wuVTrackingAction* actsTrack = new wuVTrackingAction;
  // actsTrack->AddAct(new wuTrackingAction);

  // //...0123456789876543210...0123456789876543210...
  // //以下 step数据输出 和 SD数据输出 只能选一个使用。因为存数据是静态类，只支持一个。
  // if(wuReadData::ReadValue<int>("OutputStyle", "ReadData.txt")==1)
  //   {
  //     actsRun->AddAct(new wuRunActionSD("ReadData.txt"));//SD数据输出
  //     actsEvent->AddAct(new wuEventActionSD("ReadData.txt"));//SD数据输出
  //   }
  // else
  //   {
  //     actsRun->AddAct(new wuRunAction("ReadData.txt"));//step数据输出
  //     SetUserAction(new wuSteppingAction("ReadData.txt"));//step数据输出
  //   }
  // //...0123456789876543210...0123456789876543210...


  // // actsEvent->AddAct(new wuEventAction);//
  // // actsEvent->AddAct(new wuEventActionLog);

  // SetUserAction(actsRun);
  // SetUserAction(actsTrack);
  // SetUserAction(actsEvent);

  SetUserAction(new wuRunAction("ReadData.txt"));
  SetUserAction(new wuStackingAction);

  SetUserAction(new wuEventAction);
  SetUserAction(new wuTrackingAction);
  SetUserAction(new wuSteppingAction("ReadData.txt"));

}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 
// wuActionInitialization.cc ends here
