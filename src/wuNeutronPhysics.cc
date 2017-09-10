// wuNeutronPhysics.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 一 6月  1 20:15:06 2015 (+0800)
// Last-Updated: 一 6月  1 20:20:13 2015 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 1
// URL: http://wuhongyi.cn 

#include "wuNeutronPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"
// Processes
#include "G4HadronicProcess.hh"
#include "G4HadronElasticProcess.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPThermalScatteringData.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPThermalScattering.hh"
#include "G4HadronElasticDataSet.hh"
#include "G4NeutronElasticXS.hh"
#include "G4ChipsElasticModel.hh"
#include "G4ChipsNeutronElasticXS.hh"

#include "G4NeutronInelasticProcess.hh"
#include "G4NeutronHPInelasticData.hh"
#include "G4NeutronHPInelastic.hh"
#include "G4BinaryCascade.hh"//inelastic
#include "G4TheoFSGenerator.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4FTFModel.hh"
#include "G4LundStringFragmentation.hh"
#include "G4ExcitedStringDecay.hh"
#include "G4QuasiElasticChannel.hh"
#include "G4BGGNucleonInelasticXS.hh"
#include "G4TheoFSGenerator.hh"
#include "G4QGSModel.hh"
#include "G4QGSParticipants.hh"
#include "G4QGSMFragmentation.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPCapture.hh"
#include "G4NeutronRadCapture.hh"
#include "G4NeutronCaptureXS.hh"
#include "G4LFission.hh"
#include "G4CrossSectionDataSetRegistry.hh"
#include "G4PhysicsConstructorFactory.hh"

#include "G4HadronFissionProcess.hh"
#include "G4NeutronHPFissionData.hh"
#include "G4NeutronHPFission.hh"

#include "G4SystemOfUnits.hh"

#include "G4LENDElasticCrossSection.hh"
#include "G4LENDElastic.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuNeutronPhysics::wuNeutronPhysics(G4String fin,const G4String& name)
:  G4VPhysicsConstructor(name), fThermal(true),fNameInput(fin)
{
    fThermal=wuReadData::ReadValue<bool>("Thermal", fNameInput);
}

wuNeutronPhysics::~wuNeutronPhysics()
{

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void wuNeutronPhysics::ConstructParticle()
{

}

void wuNeutronPhysics::ConstructProcess()
{
  //G4ParticleDefinition* neutron = G4Neutron::Neutron();
  G4ProcessManager* pManager = G4Neutron::Neutron()->GetProcessManager();
   
   // process: elastic
   //
   G4HadronElasticProcess* process1 = new G4HadronElasticProcess();
   // G4HadronicProcess* process1 = new G4HadronicProcess();
   pManager->AddDiscreteProcess(process1);

   G4HadronElastic *neutronModel= new G4ChipsElasticModel();
   neutronModel->SetMinEnergy(20.0*MeV);
   neutronModel->SetMaxEnergy(100*TeV);
   //process1->AddDataSet(new G4BGGNucleonElasticXS(particle));
   process1->AddDataSet(G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4ChipsNeutronElasticXS::Default_Name()));
   process1->RegisterMe(neutronModel);

   //
   // cross section data set
   //G4LENDElasticCrossSection* dataSet = new G4LENDElasticCrossSection( G4Neutron::Neutron() );
   //G4NeutronElasticXS* dataSet0a = new G4NeutronElasticXS();
   G4NeutronHPElasticData* dataSet1a = new G4NeutronHPElasticData();
   // wuNeutronHPElasticData* dataSet1a = new wuNeutronHPElasticData();
   //dataSet1a->IgnoreOnFlightDopplerBroadening();
   G4HadronElasticDataSet* dataSet2a = new G4HadronElasticDataSet();
   G4NeutronHPThermalScatteringData* dataSet1b 
                               = new G4NeutronHPThermalScatteringData();
   //process1->AddDataSet(dataSet); 
   // process1->AddDataSet(dataSet2a); 
   process1->AddDataSet(dataSet1a); 
   
   if (fThermal) process1->AddDataSet(dataSet1b);
   //
   // models
   //G4LENDElastic* model1a = new G4LENDElastic( G4Neutron::Neutron() );
   G4NeutronHPElastic*           model1a = new G4NeutronHPElastic();
   G4NeutronHPThermalScattering* model1b = new G4NeutronHPThermalScattering();
  if (fThermal)  model1a->SetMinEnergy(4*eV);
   process1->RegisterMe(model1a);    
   if (fThermal) process1->RegisterMe(model1b);
   
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

   // process: inelastic
   //
   G4NeutronInelasticProcess* process2 = new G4NeutronInelasticProcess();
   pManager->AddDiscreteProcess(process2);   

   //25.0GeV-100TeV
   G4bool quasiElasticQGS= true;    // For QGS, it must use it.
   G4TheoFSGenerator * theModel;
   {
     G4GeneratorPrecompoundInterface * theCascade;
     G4QGSModel< G4QGSParticipants > * theStringModel;
     G4ExcitedStringDecay * theStringDecay;
     G4QuasiElasticChannel * theQuasiElastic;
     G4QGSMFragmentation * theQGSM;
     theModel = new G4TheoFSGenerator("QGSP");
     theStringModel = new G4QGSModel< G4QGSParticipants >;
     theStringDecay = new G4ExcitedStringDecay(theQGSM = new G4QGSMFragmentation);
     theStringModel->SetFragmentationModel(theStringDecay);
     theCascade = new G4GeneratorPrecompoundInterface();
     theModel->SetTransport(theCascade);
     theModel->SetHighEnergyGenerator(theStringModel);
     if (quasiElasticQGS)
       {
	 theQuasiElastic=new G4QuasiElasticChannel;
	 theModel->SetQuasiElasticChannel(theQuasiElastic);
       } else 
       {  theQuasiElastic=0;}  
     theModel->SetMinEnergy(25.0*GeV);
     theModel->SetMaxEnergy(100*TeV);
     process2->RegisterMe(theModel);
     process2->AddDataSet(new G4BGGNucleonInelasticXS(G4Neutron::Neutron()));
   }

   //9.9GeV-25.0GeV
   G4bool quasiElasticFTF= false;   // Use built-in quasi-elastic (not add-on)
   G4TheoFSGenerator *TheoFSGenerator= new G4TheoFSGenerator("FTFP");
   {
     G4GeneratorPrecompoundInterface * theCascade;
     G4FTFModel * theStringModel;//
     G4ExcitedStringDecay * theStringDecay;//
     G4QuasiElasticChannel * theQuasiElastic;//
     G4LundStringFragmentation * theLund;//
     theStringModel = new G4FTFModel;//
     theStringDecay = new G4ExcitedStringDecay(theLund = new G4LundStringFragmentation);//
     theStringModel->SetFragmentationModel(theStringDecay);//
     theCascade = new G4GeneratorPrecompoundInterface();//
     TheoFSGenerator->SetTransport(theCascade);//
     TheoFSGenerator->SetHighEnergyGenerator(theStringModel);//
     if (quasiElasticFTF)
       {
	 theQuasiElastic=new G4QuasiElasticChannel;
	 TheoFSGenerator->SetQuasiElasticChannel(theQuasiElastic);
       } else 
       {  theQuasiElastic=0;}  
     TheoFSGenerator->SetMinEnergy(9.9*GeV);
     TheoFSGenerator->SetMaxEnergy(25.0*GeV);
     process2->RegisterMe(TheoFSGenerator);
     // process2->AddDataSet(new G4BGGNucleonInelasticXS(G4Neutron::Neutron()));
   }

   //20MeV-9.9GeV
   G4BinaryCascade *BinaryCascade= new G4BinaryCascade(); 
   BinaryCascade->SetMinEnergy(20.0*MeV);
   BinaryCascade->SetMaxEnergy(9.9*GeV);
   process2->RegisterMe(BinaryCascade);

   //0-20MeV
   G4NeutronHPInelasticData* dataSet2 = new G4NeutronHPInelasticData();
   process2->AddDataSet(dataSet2);                               
   G4NeutronHPInelastic* model2 = new G4NeutronHPInelastic();
   process2->RegisterMe(model2);
   
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

   // process: nCapture   
   //
   G4HadronCaptureProcess* process3 = new G4HadronCaptureProcess();
   pManager->AddDiscreteProcess(process3);    

   //20MeV-100TeV
   G4VCrossSectionDataSet *xsNeutronCaptureXS = (G4NeutronCaptureXS*)G4CrossSectionDataSetRegistry::Instance()->GetCrossSectionDataSet(G4NeutronCaptureXS::Default_Name());
  process3->AddDataSet(xsNeutronCaptureXS);
  G4NeutronRadCapture* theNeutronRadCapture = new G4NeutronRadCapture(); 
  theNeutronRadCapture->SetMinEnergy( 20.0*MeV ); 
  process3->RegisterMe( theNeutronRadCapture );

  //0-20MeV
   G4NeutronHPCaptureData* dataSet3 = new G4NeutronHPCaptureData();
   process3->AddDataSet(dataSet3);                               
   G4NeutronHPCapture* model3 = new G4NeutronHPCapture();
   process3->RegisterMe(model3);
   
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

   // process: nFission   
   //
   G4HadronFissionProcess* process4 = new G4HadronFissionProcess();
   pManager->AddDiscreteProcess(process4);    

   //20MeV-100TeV
  G4LFission* theNeutronLEPFission = new G4LFission();
  theNeutronLEPFission->SetMinEnergy( 20.0*MeV );
  process4->RegisterMe( theNeutronLEPFission );


   //0-20MeV
   G4NeutronHPFissionData* dataSet4 = new G4NeutronHPFissionData();
   process4->AddDataSet(dataSet4);                               
   G4NeutronHPFission* model4 = new G4NeutronHPFission();
   process4->RegisterMe(model4); 

}





// 
// wuNeutronPhysics.cc ends here
