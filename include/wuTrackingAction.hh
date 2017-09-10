// wuTrackingAction.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 5月 17 19:27:00 2014 (+0800)
// Last-Updated: 四 3月 12 21:23:52 2015 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 9
// URL: http://wuhongyi.cn 

#ifndef _WUTRACKINGACTION_H_
#define _WUTRACKINGACTION_H_

#include "G4UserTrackingAction.hh"

#include "G4Types.hh"

class wuTrackingAction : public G4UserTrackingAction
{
public:
  wuTrackingAction();
  virtual ~wuTrackingAction();

  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

};

#endif /* _WUTRACKINGACTION_H_ */
// 
// wuTrackingAction.hh ends here
