// wuRunAction.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 4月 20 12:06:10 2014 (+0800)
// Last-Updated: 五 5月  5 22:11:25 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 18
// URL: http://wuhongyi.cn 

#ifndef _WURUNACTION_H_
#define _WURUNACTION_H_

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <cstring>
#include "G4ios.hh"
#include "G4Timer.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class G4Run;

class wuRunAction : public G4UserRunAction
{
public:
  wuRunAction(G4String fn="ReadData.txt");
  virtual ~wuRunAction();

  virtual void BeginOfRunAction(const G4Run* run);
  virtual void   EndOfRunAction(const G4Run* run);

private:
  G4String fnInput;
  G4Timer* timer;


};


#endif /* _WURUNACTION_H_ */

// 
// wuRunAction.hh ends here
