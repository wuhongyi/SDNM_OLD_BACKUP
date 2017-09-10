// wuNeutronPhysics.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 6月  1 20:14:41 2015 (+0800)
// Last-Updated: 一 6月  1 20:20:13 2015 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#ifndef _WUNEUTRONPHYSICS_H_
#define _WUNEUTRONPHYSICS_H_

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

#include "wuReadData.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class wuNeutronPhysics : public G4VPhysicsConstructor
{
public:
  wuNeutronPhysics(G4String fin="ReadData.txt",const G4String& name="neutron");
  virtual ~wuNeutronPhysics();

  // This method will be invoked in the Construct() method. 
  // each particle type will be instantiated
  virtual void ConstructParticle();
 
  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type 
  virtual void ConstructProcess();


protected:
  G4String  fNameInput;
  G4bool  fThermal;
};


#endif /* _WUNEUTRONPHYSICS_H_ */
// 
// wuNeutronPhysics.hh ends here
