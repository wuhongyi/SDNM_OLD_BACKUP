// wuDetectorConstruction.cc --- 
// 
// Description: Geant4.10样式
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 5月 22 16:44:39 2014 (+0800)
// Last-Updated: 四 5月 25 21:33:37 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 187
// URL: http://wuhongyi.cn 


#include "wuDetectorConstruction.hh"
#include "wuRegionInformation.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UserLimits.hh"
#include "G4AutoDelete.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4PSEnergyDeposit3D.hh"
#include "G4PSNofStep3D.hh"
#include "G4PSCellFlux3D.hh"
#include "G4PSPassageCellFlux3D.hh"
#include "G4PSFlatSurfaceFlux3D.hh"
#include "G4PSFlatSurfaceCurrent3D.hh"
#include "G4SDParticleWithEnergyFilter.hh"
#include "G4SDParticleFilter.hh"
#include "G4SDChargedFilter.hh"
#include "G4SDNeutralFilter.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

wuDetectorConstruction::wuDetectorConstruction(G4String fn)
  : G4VUserDetectorConstruction(),checkOverlaps(true),fVisAttributes(),logicWorld(0)
{
  fnInput=fn;
}

wuDetectorConstruction::~wuDetectorConstruction()
{
  //释放可视化界面的颜色设置变量
  for (G4int i=0; i<G4int(fVisAttributes.size()); ++i) 
    {
      delete fVisAttributes[i];
    } 
}

G4VPhysicalVolume* wuDetectorConstruction::Construct()
{
  // Define materials 
  DefineMaterials();

  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void wuDetectorConstruction::ConstructSDandField()
{
  //快速添加SD
  // BuildSensitiveDetector(logicWorld);
  // BuildSensitiveDetector(fLVScint2);
  // BuildSensitiveDetector(fLVScint3);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void wuDetectorConstruction::DefineMaterials()
{ 
  //在这里先定义所有可能用到的材料
  // Get nist material manager
  G4NistManager* man = G4NistManager::Instance();
  
  G4bool isotopes = false;

  G4Element* H=man->FindOrBuildElement("H",isotopes);
  G4Element* C=man->FindOrBuildElement("C",isotopes);
  G4Element* Mn=man->FindOrBuildElement("Mn",isotopes);
  G4Element* Si=man->FindOrBuildElement("Si",isotopes);
  G4Element* P=man->FindOrBuildElement("P",isotopes);
  G4Element* S=man->FindOrBuildElement("S",isotopes);
  G4Element* Cr=man->FindOrBuildElement("Cr",isotopes);
  G4Element* Ni=man->FindOrBuildElement("Ni",isotopes);
  G4Element* N=man->FindOrBuildElement("N",isotopes);
  G4Element* Fe=man->FindOrBuildElement("Fe",isotopes);

  G4Isotope* U5= new G4Isotope("U235",92,235,235.01*g/mole);
  G4Isotope* U8= new G4Isotope("U238",92,238,238.03*g/mole);
  G4Isotope* U4= new G4Isotope("U234",92,234,234.02*g/mole);
  G4Isotope* U6= new G4Isotope("U236",92,236,236.04*g/mole);

  G4Element* u_5= new G4Element("ENU","u_5",4);
  u_5->AddIsotope(U5,0.9315);
  u_5->AddIsotope(U8,0.0564);
  u_5->AddIsotope(U4,0.0097);
  u_5->AddIsotope(U6,0.0024);
  
  G4Material* u5_mat=new G4Material("Enrich_U5",18.75*g/cm3,1);
  u5_mat->AddElement(u_5, 1.0);

  G4Material* air_mat = man->FindOrBuildMaterial("G4_AIR", isotopes);
  G4Material* al_mat  = man->FindOrBuildMaterial("G4_Al", isotopes);

  G4Material* sci_mat = new G4Material("Scintillator",1.032*g/cm3,2);
  sci_mat->AddElement(H,10);
  sci_mat->AddElement(C,9);

  G4Material* steel_mat= new G4Material("Steel",7.788*g/cm3,9);
  steel_mat->AddElement(C,0.04*perCent);
  steel_mat->AddElement(Mn,9.37*perCent);
  steel_mat->AddElement(Si,0.34*perCent);
  steel_mat->AddElement(P,0.018*perCent);
  steel_mat->AddElement(S,0.0015*perCent);
  steel_mat->AddElement(Cr,20.26*perCent);
  steel_mat->AddElement(Ni,7.5*perCent);
  steel_mat->AddElement(N,0.29*perCent);
  steel_mat->AddElement(Fe,62.1805*perCent);


  // Print materials，运行时在终端输出材料信息
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume*wuDetectorConstruction::DefineVolumes()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //通过G4Material::GetMaterial()获取DefineMaterials()中定义的材料！
  G4Material* world_mat =  G4Material::GetMaterial("G4_AIR");//"G4_WATER" "G4_Pb"
  G4Material* steel_mat =  G4Material::GetMaterial("Steel");
  G4Material* u5_mat =  G4Material::GetMaterial("Enrich_U5");
  G4Material* al_mat =  G4Material::GetMaterial("G4_Al");
  G4Material* sci_mat =  G4Material::GetMaterial("Scintillator");

  //     
  // World
  //
  G4double worldX = 1600.0*cm;
  G4double worldY = 1600.0*cm;
  G4double worldZ = 2000.0*cm;
  

  G4Box* solidWorld =    
    new G4Box("World",                       //its name
  	      0.5*worldX, 0.5*worldY, 0.5*worldZ);     //its size
      
  logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  G4Region* defaultRegion = (*(G4RegionStore::GetInstance()))[0];
  wuRegionInformation* defaultRInfo = new wuRegionInformation();
  defaultRInfo->SetRegionNumber(0);
  defaultRegion->SetUserInformation(defaultRInfo);


  G4Tubs* fSLDTest  = new G4Tubs("TestObject",
				 44.45*mm,
				 63.5*mm,
				 76.2*mm,
				 0.0,
				 twopi);

  fLVTest = new G4LogicalVolume(fSLDTest,
				u5_mat,
				"LVTest");
  new G4PVPlacement(0,  //no rotation
		    G4ThreeVector(0,0,76.2*mm),     //at (0,0,0)
		    fLVTest,             //its logical volume
		    "test_object",       //its name
		    logicWorld,           // its mother volume
		    false,              // no boolean operation
		    0,checkOverlaps);       // checking overlaps

  G4Region* RegionU = new G4Region("RegionU");
  wuRegionInformation* InfoU = new wuRegionInformation();
  InfoU->SetRegionNumber(1);
  RegionU->SetUserInformation(InfoU);
  fLVTest->SetRegion(RegionU);
  RegionU->AddRootLogicalVolume(fLVTest);
  
  // G4Region* emTest = new G4Region("EM-Test"); 
  // fLVTest->SetRegion(emTest);
  // emTest->AddRootLogicalVolume(fLVTest);   


  G4Tubs* fSLDDiaphragm = new G4Tubs("diaphragm",  //its name
  				     75.946*mm,        // inner rad
  				     76.2*mm,     //outer rad
  				     88.9*mm,   // depth in z ax
  				     0.0,
  				     twopi );
  fLVDiaphragm = new G4LogicalVolume(fSLDDiaphragm,
  				     steel_mat,
  				     "diaphragm" );
  new G4PVPlacement(0,                                //no rotation
  		    G4ThreeVector(0.0,0.0,88.9*mm),   // at point ()
  		    fLVDiaphragm,                     // its logical volume
  		    "diaphragm",                      // its name
  		    logicWorld,                       // its mother volume
  		    false,                            // no boolean operation
  		    0,                                //copy number
  		    checkOverlaps );                //checking overlaps
  
  
  //Steel seat
  G4Box* fSCFeDiaphragm = new G4Box("CFediaphragm",30*cm,30*cm,3.17*mm);
  fCFeDiaphragm = new G4LogicalVolume(fSCFeDiaphragm,
  				     steel_mat,
  				     "CFediaphragm" );
  new G4PVPlacement(0,                                //no rotation
  		    G4ThreeVector(0.0,0.0,-3.17*mm),   // at point ()
  		    fCFeDiaphragm,                     // its logical volume
  		    "CFediaphragm",                      // its name
  		    logicWorld,                       // its mother volume
  		    false,                            // no boolean operation
  		    0,                                //copy number
  		    checkOverlaps );                //checking overlaps
  


  
  // detector2 and 3

  G4Box* fSLDDetShell2 = new G4Box("DetShell2",52.4*mm, 38.1*mm,38.1*mm);
     
  fLVDetShell2 = new G4LogicalVolume(fSLDDetShell2,
				     al_mat,
				     "DetShell2");
  new G4PVPlacement(0, G4ThreeVector(128.6*mm,-38.1*mm,38.1*mm),//rotation, position
		    fLVDetShell2,          //its logical volume
		    "DetShell2",              //its name
		    logicWorld,              //its mother volume
		    false,
		    0,
		    checkOverlaps); 

  G4Box* fSLDDetShell3 = new G4Box("DetShell3",52.4*mm, 38.1*mm,38.1*mm);
     
  fLVDetShell3 = new G4LogicalVolume(fSLDDetShell3,
				     al_mat,
				     "DetShell3");
  new G4PVPlacement(0, G4ThreeVector(128.6*mm,38.1*mm,38.1*mm),//rotation, position
		    fLVDetShell3,          //its logical volume
		    "DetShell3",              //its name
		    logicWorld,              //its mother volume
		    false,
		    0,
		    checkOverlaps); 

  G4Box* fSLDDetShell4 = new G4Box("DetShell4",52.4*mm, 38.1*mm,38.1*mm);
     
  fLVDetShell4 = new G4LogicalVolume(fSLDDetShell4,
				     al_mat,
				     "DetShell4");
  new G4PVPlacement(0, G4ThreeVector(128.6*mm,-38.1*mm,114.3*mm),//rotation, position
		    fLVDetShell4,          //its logical volume
		    "DetShell4",              //its name
		    logicWorld,              //its mother volume
		    false,
		    0,
		    checkOverlaps); 

  G4Box* fSLDDetShell5 = new G4Box("DetShell5",52.4*mm, 38.1*mm,38.1*mm);
     
  fLVDetShell5 = new G4LogicalVolume(fSLDDetShell5,
				     al_mat,
				     "DetShell5");
  new G4PVPlacement(0, G4ThreeVector(128.6*mm,38.1*mm,114.3*mm),//rotation, position
		    fLVDetShell5,          //its logical volume
		    "DetShell5",              //its name
		    logicWorld,              //its mother volume
		    false,
		    0,
		    checkOverlaps); 
  
  
  // G4Tubs* fSLDDetShell3 = new G4Tubs("DetShell3",   //its name
  // 				    0.0,       // inner rad
  // 				    1.5*2.64*cm,    //  outter rad
  // 				    2.64*cm,    // half length in z ax direction
  // 				    0.0,
  // 				    twopi );
     
  // fLVDetShell3 = new G4LogicalVolume(fSLDDetShell3,
  // 				     al_mat,
  // 				     "DetShell3");
  // new G4PVPlacement(transform,               //rotation, position
  // 		    fLVDetShell3,            //its logical volume
  // 		    "DetShell3",              //its name
  // 		    logicWorld,              //its mother volume
  // 		    false,
  // 		    0,
  // 		    checkOverlaps);


  G4Box* fSLDScint2 = new G4Box("Scint2", 50.8*mm, 34.925*mm, 34.925*mm);
     
  fLVScint2 = new G4LogicalVolume( fSLDScint2,         // solid pointer   
				   sci_mat,          // material pointer
				   "LVScint2");       // name
  new G4PVPlacement(0,                       //no rotation
		    G4ThreeVector(1.6*mm,0,0),         // position(0,0,0)
		    fLVScint2,               //its logical volume
		    "Scint2",                //its name
		    fLVDetShell2,            //its mother volume
		    false,
		    0,
		    checkOverlaps);

  
  G4Box* fSLDScint3 = new G4Box("Scint3", 50.8*mm, 34.925*mm, 34.925*mm);
     
  fLVScint3 = new G4LogicalVolume( fSLDScint3,         // solid pointer   
				   sci_mat,          // material pointer
				   "LVScint3");       // name
  new G4PVPlacement(0,                       //no rotation
		    G4ThreeVector(1.6*mm,0,0),         // position(0,0,0)
		    fLVScint3,               //its logical volume
		    "Scint3",                //its name
		    fLVDetShell3,            //its mother volume
		    false,
		    0,
		    checkOverlaps);

  G4Box* fSLDScint4 = new G4Box("Scint4", 50.8*mm, 34.925*mm, 34.925*mm);
     
  fLVScint4 = new G4LogicalVolume( fSLDScint4,         // solid pointer   
				   sci_mat,          // material pointer
				   "LVScint4");       // name
  new G4PVPlacement(0,                       //no rotation
		    G4ThreeVector(1.6*mm,0,0),         // position(0,0,0)
		    fLVScint4,               //its logical volume
		    "Scint4",                //its name
		    fLVDetShell4,            //its mother volume
		    false,
		    0,
		    checkOverlaps);

  
  G4Box* fSLDScint5 = new G4Box("Scint5", 50.8*mm, 34.925*mm, 34.925*mm);
     
  fLVScint5 = new G4LogicalVolume( fSLDScint5,         // solid pointer   
				   sci_mat,          // material pointer
				   "LVScint5");       // name
  new G4PVPlacement(0,                       //no rotation
		    G4ThreeVector(1.6*mm,0,0),         // position(0,0,0)
		    fLVScint5,               //its logical volume
		    "Scint5",                //its name
		    fLVDetShell5,            //its mother volume
		    false,
		    0,
		    checkOverlaps);
  
  

  // G4Tubs* fSLDScint3 = new G4Tubs("Scint3",   //its name
  // 				  0.0,      // inner rad
  // 				  1.5*2.54*cm,    //  outter rad
  // 				  2.54*cm,    // half length in z ax direction
  // 				  0.0,
  // 				  twopi );
     
  // fLVScint3 = new G4LogicalVolume( fSLDScint3,         // solid pointer   
  // 				   sci_mat,          // material pointer
  // 				   "LVScint3");       // name
  // new G4PVPlacement(0,                       //no rotation
  // 		    G4ThreeVector(),         // position(0,0,0)
  // 		    fLVScint3,               //its logical volume
  // 		    "Scint3",                //its name
  // 		    fLVDetShell3,            //its mother volume
  // 		    false,
  // 		    0,
  // 		    checkOverlaps);



  
  G4Region* RegionS2 = new G4Region("RegionS2");
  wuRegionInformation* InfoS2 = new wuRegionInformation();
  InfoS2->SetRegionNumber(2);
  RegionS2->SetUserInformation(InfoS2);
  fLVScint2->SetRegion(RegionS2);
  RegionS2->AddRootLogicalVolume(fLVScint2);

  G4Region* RegionS3 = new G4Region("RegionS3");
  wuRegionInformation* InfoS3 = new wuRegionInformation();
  InfoS3->SetRegionNumber(3);
  RegionS3->SetUserInformation(InfoS3);
  fLVScint3->SetRegion(RegionS3);
  RegionS3->AddRootLogicalVolume(fLVScint3);

  G4Region* RegionS4 = new G4Region("RegionS4");
  wuRegionInformation* InfoS4 = new wuRegionInformation();
  InfoS4->SetRegionNumber(4);
  RegionS4->SetUserInformation(InfoS4);
  fLVScint4->SetRegion(RegionS4);
  RegionS4->AddRootLogicalVolume(fLVScint4);

  G4Region* RegionS5 = new G4Region("RegionS5");
  wuRegionInformation* InfoS5 = new wuRegionInformation();
  InfoS5->SetRegionNumber(5);
  RegionS5->SetUserInformation(InfoS5);
  fLVScint5->SetRegion(RegionS5);
  RegionS5->AddRootLogicalVolume(fLVScint5);

  



  //===============================

  // G4Region* EnvelopeRegion = new G4Region("EnvelopeRegion");
  // wuRegionInformation* EnvelopeInfo = new wuRegionInformation();
  // EnvelopeInfo->SetRegion();
  // EnvelopeRegion->SetUserInformation(EnvelopeInfo);
  // logicEnv->SetRegion(EnvelopeRegion);
  // EnvelopeRegion->AddRootLogicalVolume(logicEnv);


    // visualization attributes ------------------------------------------------
    //可视化界面几何体颜色设置（可有可无）
    
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    //visAttributes->SetVisibility(false);//不显示边框
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());
    fVisAttributes.push_back(visAttributes);

    
    visAttributes = new G4VisAttributes(G4Colour(195.,195.,0.0));
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVTest->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour::Gray());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVDiaphragm->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour::Blue());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fCFeDiaphragm->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    

    visAttributes = new G4VisAttributes(G4Colour::White());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVDetShell2->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour::White());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVDetShell3->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour::White());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVDetShell4->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);


    visAttributes = new G4VisAttributes(G4Colour::White());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVDetShell5->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);
    

    visAttributes = new G4VisAttributes(G4Colour::Green());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVScint2->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour::Green());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVScint3->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);

    visAttributes = new G4VisAttributes(G4Colour::Green());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVScint4->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);


    visAttributes = new G4VisAttributes(G4Colour::Green());
    visAttributes->SetForceSolid(true);
    visAttributes->SetForceAuxEdgeVisible(true);
    fLVScint5->SetVisAttributes(visAttributes);
    fVisAttributes.push_back(visAttributes);


    
    // G4Colour  white   (1.0, 1.0, 1.0) ;
    // G4Colour  grey    (0.5, 0.5, 0.5) ;
    // G4Colour  lgrey   (.85, .85, .85) ;
    // G4Colour  red     (1.0, 0.0, 0.0) ;
    // G4Colour  blue    (0.0, 0.0, 1.0) ;
    // G4Colour  cyan    (0.0, 1.0, 1.0) ;
    // G4Colour  magenta (1.0, 0.0, 1.0) ; 
    // G4Colour  yellow  (1.0, 1.0, 0.0) ;
    // G4Colour  orange  (.75, .55, 0.0) ;
    // G4Colour  lblue   (0.0, 0.0, .75) ;
    // G4Colour  lgreen  (0.0, .75, 0.0) ;
    // G4Colour  green   (0.0, 1.0, 0.0) ;
    // G4Colour  brown   (0.7, 0.4, 0.1) ;

  //===================================================================
  //
  //always return the physical World
  //
  return physWorld;  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void wuDetectorConstruction::BuildSensitiveDetector(G4LogicalVolume* lv)
// {
//   if(!lv){
//     G4cout<<"$$No given logical volume, no SD built"<<G4endl;
//     return;
//   }
//   G4SDManager* SDman=G4SDManager::GetSDMpointer();
//   wuSensitiveDetector* sd1=new wuSensitiveDetector(lv->GetName()); //
//   SDman->AddNewDetector(sd1);
//   lv->SetSensitiveDetector(sd1);
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// 
// wuDetectorConstruction.cc ends here
