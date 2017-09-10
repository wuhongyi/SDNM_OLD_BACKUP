// wuRunAction.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 4月 20 12:06:28 2014 (+0800)
// Last-Updated: 日 5月  7 09:41:17 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 94
// URL: http://wuhongyi.cn 

#include "G4Run.hh"
#include "G4RunManager.hh"

#include <ctime>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cstring>
#include "wuReadData.hh"
#include "wuRunAction.hh"
#include "Event.hh"
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuRunAction::wuRunAction(G4String fn)
{
  fnInput=fn;
  timer = new G4Timer;
}

wuRunAction::~wuRunAction()
{

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void wuRunAction::BeginOfRunAction(const G4Run* run)
{ 
  G4cout<<"BeginOfRunAction..."<<G4endl;

  timer->Start();

  // string FileFormat=wuReadData::ReadValue<string>("FileFormat", fnInput);
  Event::Instance()->SetFileNumber(time(NULL));
  Event::Instance()->InitWriteFile();
 
}


void wuRunAction::EndOfRunAction(const G4Run* run)
{


  Event::Instance()->CloseWriteFile();

  timer->Stop();
  G4cout <<"number of event = "<< run->GetNumberOfEvent() 
         <<" "<<*timer<< G4endl; 
  G4cout<<"EndOfRunAction..."<<G4endl;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 
// wuRunAction.cc ends here
