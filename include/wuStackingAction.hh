// wuStackingAction.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 12月 20 19:59:08 2014 (+0800)
// Last-Updated: 六 12月 20 20:06:34 2014 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#ifndef _WUSTACKINGACTION_H_
#define _WUSTACKINGACTION_H_

#include "G4UserStackingAction.hh"
#include "globals.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class wuStackingAction : public G4UserStackingAction
{
public:
  wuStackingAction();
  virtual ~wuStackingAction();

  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
  virtual void NewStage();
  virtual void PrepareNewEvent();
};


#endif /* _WUSTACKINGACTION_H_ */
// 
// wuStackingAction.hh ends here
