// wuNeutronHPphysics.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 12月 20 20:56:19 2014 (+0800)
// Last-Updated: 六 12月 20 21:26:10 2014 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 2
// URL: http://wuhongyi.cn 

#ifndef _WUNEUTRONHPPHYSICS_H_
#define _WUNEUTRONHPPHYSICS_H_

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

#include "wuReadData.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class wuNeutronHPphysics : public G4VPhysicsConstructor
{
public:
  wuNeutronHPphysics(G4String fin="ReadData.txt",const G4String& name="neutron");
  virtual ~wuNeutronHPphysics();

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


#endif /* _WUNEUTRONHPPHYSICS_H_ */
// 
// wuNeutronHPphysics.hh ends here
