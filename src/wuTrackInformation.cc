// wuTrackInformation.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 12月 13 19:32:51 2014 (+0800)
// Last-Updated: 五 5月  5 20:15:16 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 21
// URL: http://wuhongyi.cn 

#include "wuTrackInformation.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"    

G4ThreadLocal G4Allocator<wuTrackInformation> * aTrackInformationAllocator = 0;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuTrackInformation::wuTrackInformation()
  :G4VUserTrackInformation()
{
  fFlag=0;
  fFission = false;
}
wuTrackInformation:: wuTrackInformation(const G4Track* aTrack)
  :G4VUserTrackInformation()
{

}

wuTrackInformation::wuTrackInformation(const wuTrackInformation* Info)
  :G4VUserTrackInformation()
{
  fFlag=Info->fFlag;
}
wuTrackInformation::~wuTrackInformation()
{

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool wuTrackInformation::AddFlag(G4int ss)
{
  //预留接口
  fFlag=ss;
  return true;
}



// 
// wuTrackInformation.cc ends here
