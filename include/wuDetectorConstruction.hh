// wuDetectorConstruction.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 5月 22 16:45:21 2014 (+0800)
// Last-Updated: 四 5月 25 20:54:36 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 24
// URL: http://wuhongyi.cn 

#ifndef _WUDETECTORCONSTRUCTION_H_
#define _WUDETECTORCONSTRUCTION_H_

// #include "wuSensitiveDetector.hh"//
#include "wuReadData.hh"//填卡式读取数据
#include "G4TransportationManager.hh"
#include "G4PropagatorInField.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SDManager.hh"

// GDML parser include，可视化界面几何体颜色设置用
#include "G4GDMLParser.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// class description:
//
//  This is the abstract base class of the user's mandatory initialization class
// for detector setup. It has only one pure virtual method Construct() which is
// invoked by G4RunManager when it's Initialize() method is invoked.
//  The Construct() method must return the G4VPhysicalVolume pointer which represents
// the world volume.
//
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VisAttributes;

class wuDetectorConstruction: public G4VUserDetectorConstruction
{
public:
  wuDetectorConstruction(G4String fn="ReadData.txt");
  virtual ~wuDetectorConstruction();

public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  //virtual void ConstructSDandField();
  //This method is used in multi-threaded applications to build
  //per-worker non-shared objects: SensitiveDetectors and Field managers

private:
  // methods
  //
  void DefineMaterials();
  G4VPhysicalVolume* DefineVolumes();

  // void BuildSensitiveDetector(G4LogicalVolume* lv);//

  
private: 
  G4String fnInput;
  G4bool checkOverlaps; // Option to switch on/off checking of volumes overlaps
  std::vector<G4VisAttributes*> fVisAttributes;//可视化界面几何体颜色设置用

  G4LogicalVolume* logicWorld;
  G4VPhysicalVolume* physWorld;
  G4LogicalVolume* fLVDetShell2;
  G4LogicalVolume* fLVDetShell3;
  G4LogicalVolume* fLVDetShell4;
  G4LogicalVolume* fLVDetShell5;
  G4LogicalVolume *fLVScint2;
  G4LogicalVolume *fLVScint3;
  G4LogicalVolume *fLVScint4;
  G4LogicalVolume *fLVScint5;
  G4LogicalVolume* fLVDiaphragm;
  G4LogicalVolume* fCFeDiaphragm;
  G4LogicalVolume *fLVTest;

};

#endif /* _WUDETECTORCONSTRUCTION_H_ */
// 
// wuDetectorConstruction.hh ends here
