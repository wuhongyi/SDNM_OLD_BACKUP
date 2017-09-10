// wuRegionInformation.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 4月 29 21:15:43 2015 (+0800)
// Last-Updated: 五 5月  5 10:24:01 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#include "wuRegionInformation.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuRegionInformation::wuRegionInformation()
  :G4VUserRegionInformation(),
   fIsWorld(false),fIsRegion(false),fIsRegion1(false),fIsRegion2(false),fIsRegion3(false),fRegionNumber(-1)
{

}
wuRegionInformation::~wuRegionInformation()
{

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void wuRegionInformation::Print() const
{
  G4cout << "I'm ";
  if(fIsWorld) { G4cout << "World."; }
  else if(fIsRegion) { G4cout << "Region."; }
  else if(fIsRegion1) { G4cout << "Region1."; }
  else if(fIsRegion2) { G4cout << "Region2."; }
  else if(fIsRegion3) { G4cout << "Region3."; }
  else { G4cout << "unknown."; }
  G4cout << G4endl;
}


// 
// wuRegionInformation.cc ends here
