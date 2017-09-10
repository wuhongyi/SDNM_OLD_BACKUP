// wuEventAction.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 4月 20 14:02:00 2014 (+0800)
// Last-Updated: 五 5月  5 21:03:43 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 11
// URL: http://wuhongyi.cn 

#ifndef _WUEVENTACTION_H_
#define _WUEVENTACTION_H_

#include "G4UserEventAction.hh"
#include "G4Event.hh"
// #include "G4THitsMap.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class wuEventAction : public G4UserEventAction
{
public:
  wuEventAction();
  virtual ~wuEventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);

private:


};

#endif /* _WUEVENTACTION_H_ */
// 
// wuEventAction.hh ends here
