// wuActionInitialization.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 二 5月 20 11:29:02 2014 (+0800)
// Last-Updated: 三 3月 11 08:58:23 2015 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 7
// URL: http://wuhongyi.cn 

#ifndef _WUACTIONINITIALIZATION_H_
#define _WUACTIONINITIALIZATION_H_

#include "G4VUserActionInitialization.hh"

using namespace std;

/// Action initialization class.
class wuActionInitialization : public G4VUserActionInitialization
{
public:
  wuActionInitialization();
  virtual ~wuActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

};

#endif /* _WUACTIONINITIALIZATION_H_ */
// 
// wuActionInitialization.hh ends here
