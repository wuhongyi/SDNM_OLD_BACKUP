// wuPhysicsList.hh --- 
// 
// Description: Coping from YushouSong,宋玉收.
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 5月 22 16:57:10 2014 (+0800)
// Last-Updated: 一 7月 25 21:11:02 2016 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 6
// URL: http://wuhongyi.cn 

#ifndef _WUPHYSICSLIST_H_
#define _WUPHYSICSLIST_H_

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

// #include "CompileTimeConstraints.hh"
// #include "G4VPhysicsConstructor.hh"
#include "wuReadData.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class wuPhysicsList : public G4VModularPhysicsList
{
public:
  wuPhysicsList(G4String fin="ReadData.txt", G4int ver=1);
  virtual ~wuPhysicsList();

public:
  virtual void SetCuts();
  virtual void ConstructParticle();
  
// private:
//   enum {ok = CompileTimeConstraints::IsA<G4VModularPhysicsList, 
//   G4VModularPhysicsList>::ok };

protected:
  G4String  fNameInput;
};


#endif /* _WUPHYSICSLIST_H_ */
// 
// wuPhysicsList.hh ends here
