// wuNeutronHPphysics.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 12月 20 20:56:04 2014 (+0800)
// Last-Updated: 四 3月 23 11:13:50 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 14
// URL: http://wuhongyi.cn 

#include "wuNeutronHPphysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

// Processes
#include "G4HadronicProcess.hh"
#include "G4HadronElasticProcess.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPThermalScatteringData.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPThermalScattering.hh"

#include "G4NeutronInelasticProcess.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4NeutronHPInelastic.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPCapture.hh"

#include "G4HadronFissionProcess.hh"
#include "G4NeutronHPFissionData.hh"
#include "G4NeutronHPFission.hh"

#include "G4FissLib.hh"

#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuNeutronHPphysics::wuNeutronHPphysics(G4String fin,const G4String& name)
   :  G4VPhysicsConstructor(name), fThermal(true),fNameInput(fin)
{
    fThermal=wuReadData::ReadValue<bool>("Thermal", fNameInput);
}

wuNeutronHPphysics::~wuNeutronHPphysics()
{

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void wuNeutronHPphysics::ConstructParticle()
{

}

void wuNeutronHPphysics::ConstructProcess()
{
   G4ProcessManager* pManager = G4Neutron::Neutron()->GetProcessManager();
   
   // G4NeutronHPManager::GetInstance()->SetNeglectDoppler(true);

   // process: elastic
   //
   G4HadronElasticProcess* process1 = new G4HadronElasticProcess();
   //G4HadronicProcess* process1 = new G4HadronicProcess();
   pManager->AddDiscreteProcess(process1);   
   //
   // cross section data set
   G4NeutronHPElasticData* dataSet1a = new G4NeutronHPElasticData();
   G4NeutronHPThermalScatteringData* dataSet1b 
                               = new G4NeutronHPThermalScatteringData();
   process1->AddDataSet(dataSet1a);                               
   if (fThermal) process1->AddDataSet(dataSet1b);
   //
   // models
   G4NeutronHPElastic*           model1a = new G4NeutronHPElastic();
   G4NeutronHPThermalScattering* model1b = new G4NeutronHPThermalScattering();
  if (fThermal)  model1a->SetMinEnergy(4*eV);
   process1->RegisterMe(model1a);    
   if (fThermal) process1->RegisterMe(model1b);
   
   // process: inelastic
   //
   G4NeutronInelasticProcess* process2 = new G4NeutronInelasticProcess();
   pManager->AddDiscreteProcess(process2);   
   //
   // cross section data set
   G4NeutronHPInelasticData* dataSet2 = new G4NeutronHPInelasticData();
   process2->AddDataSet(dataSet2);                               
   //
   // models
   G4NeutronHPInelastic* model2 = new G4NeutronHPInelastic();
   process2->RegisterMe(model2);  
   

   // process: nCapture   
   //
   G4HadronCaptureProcess* process3 = new G4HadronCaptureProcess();
   pManager->AddDiscreteProcess(process3);    
   //
   // cross section data set
   G4NeutronHPCaptureData* dataSet3 = new G4NeutronHPCaptureData();
   process3->AddDataSet(dataSet3);                               
   //
   // models
   G4NeutronHPCapture* model3 = new G4NeutronHPCapture();
   process3->RegisterMe(model3);
   
   // process: nFission   
   //
   G4HadronFissionProcess* process4 = new G4HadronFissionProcess();
   pManager->AddDiscreteProcess(process4);    
   //
   // cross section data set
   G4NeutronHPFissionData* dataSet4 = new G4NeutronHPFissionData();
   process4->AddDataSet(dataSet4);                               
   //
   // models
   G4NeutronHPFission* model4 = new G4NeutronHPFission();
   process4->RegisterMe(model4); 


   // process4->RegisterMe(new G4FissLib());
}


// 
// wuNeutronHPphysics.cc ends here
