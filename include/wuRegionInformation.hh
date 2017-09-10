// wuRegionInformation.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 三 4月 29 21:15:20 2015 (+0800)
// Last-Updated: 五 5月  5 10:24:01 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#ifndef _WUREGIONINFORMATION_H_
#define _WUREGIONINFORMATION_H_

#include "globals.hh"
#include "G4VUserRegionInformation.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class wuRegionInformation : public G4VUserRegionInformation
{
public:
  wuRegionInformation();
  virtual ~wuRegionInformation();

  virtual void Print() const;

  inline void SetWorld(G4bool v=true) {fIsWorld = v;}
  inline void SetRegion(G4bool v=true) {fIsRegion = v;}//将要通过Step输出信息的区域设为此区域
  inline void SetRegion1(G4bool v=true) {fIsRegion1 = v;}//预留，特殊模拟用
  inline void SetRegion2(G4bool v=true) {fIsRegion2 = v;}
  inline void SetRegion3(G4bool v=true) {fIsRegion3 = v;}
  inline G4bool IsRegion() const {return fIsRegion;}
  inline G4bool IsRegion1() const {return fIsRegion1;}
  inline G4bool IsRegion2() const {return fIsRegion2;}
  inline G4bool IsRegion3() const {return fIsRegion3;}

  inline void SetRegionNumber(G4int n) {fRegionNumber = n;}
  inline G4int GetRegionNumber() {return fRegionNumber;}
private:
  G4bool fIsWorld;
  G4bool fIsRegion;
  G4bool fIsRegion1;
  G4bool fIsRegion2;
  G4bool fIsRegion3;

  G4int fRegionNumber;


};


#endif /* _WUREGIONINFORMATION_H_ */
// 
// wuRegionInformation.hh ends here
