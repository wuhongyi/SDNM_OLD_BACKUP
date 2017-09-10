// wuPhysicsList.cc --- 
// 
// Description: Coping from YushouSong,宋玉收.
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 5月 22 16:56:53 2014 (+0800)
// Last-Updated: 四 4月 20 10:22:32 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 94
// URL: http://wuhongyi.cn 

#include "wuPhysicsList.hh"
#include "wuNeutronHPphysics.hh"
#include "wuNeutronPhysics.hh"

// particles
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4RegionStore.hh"
#include "G4EmParameters.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4PhysicsListHelper.hh"
#include "G4RadioactiveDecay.hh"
#include "G4NuclideTable.hh"
#include "G4LossTableManager.hh"
#include "G4DataQuestionaire.hh"

#include "G4ProductionCuts.hh"
#include "G4NeutronTrackingCut.hh"


// Decay
#include "G4DecayPhysics.hh"
// #include "G4MuonicAtomDecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4SpinDecayPhysics.hh"
// #include "G4UnknownDecayPhysics.hh"

// Electromagnetic
#include "G4EmDNAChemistry.hh"         
// #include "G4EmDNAPhysicsActivator.hh"  
#include "G4EmDNAPhysics.hh"// Application: DNA   Extension:   Comment: Default for DNA physics 
#include "G4EmDNAPhysics_option1.hh"// Application: DNA   Extension:   Comment: WVI multiple scattering
#include "G4EmDNAPhysics_option2.hh"  
#include "G4EmDNAPhysics_option3.hh"  
#include "G4EmDNAPhysics_option4.hh"  
#include "G4EmDNAPhysics_option5.hh"
#include "G4EmDNAPhysics_option7.hh"
#include "G4EmLEPTSPhysics.hh"
#include "G4EmLivermorePhysics.hh"// Application:   Extension: LIV   Comment: Detailed   Livermore models
#include "G4EmLivermorePolarizedPhysics.hh"// Application:   Extension:   Comment: Polarized models
#include "G4EmLowEPPhysics.hh"// Application:   Extension:   Comment: New low energy models
#include "G4EmPenelopePhysics.hh"// Application:   Extension:   Comment: Detailed   Penelope models
#include "G4EmStandardPhysicsGS.hh"
#include "G4EmStandardPhysics.hh" // Application: HEP   Extension:   Comment: Default (ATLAS) 
#include "G4EmStandardPhysics_option1.hh"// Application:   Extension: EMV  Comment: Simplified (CMS)
#include "G4EmStandardPhysics_option2.hh"// Application:   Extension: EMX  Comment: Simplified (LHCb)
#include "G4EmStandardPhysics_option3.hh"// Application: Space&medicine  Extension: EMY  Comment: Detailed   Standard models 
#include "G4EmStandardPhysics_option4.hh"// Application:   Extension: EMZ  Comment: Combining   Best models
#include "G4EmStandardPhysicsSS.hh"// Application:   Extension:   Comment: Single scattering 
#include "G4EmStandardPhysicsWVI.hh"// Application:   Extension:   Comment: WVI multiple scattering
#include "G4OpticalPhysics.hh"// Production and transport of optical photons


// Gamma Lepto Nuclear  (Synchroton Radiation & GN Physics)
#include "G4EmExtraPhysics.hh"// Application:   Extension:   Comment:  

// Hadron Elastic
#include "G4ChargeExchangePhysics.hh"
#include "G4HadronDElasticPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4HadronElasticPhysicsLEND.hh"
#include "G4HadronElasticPhysicsPHP.hh"
#include "G4HadronElasticPhysicsXS.hh"
#include "G4HadronHElasticPhysics.hh"
#include "G4IonElasticPhysics.hh"

// Hadron Inelastic
#include "G4HadronInelasticQBBC.hh"         
#include "G4HadronPhysicsFTF_BIC.hh"        
#include "G4HadronPhysicsFTFP_BERT_ATL.hh"  
#include "G4HadronPhysicsFTFP_BERT.hh"      
#include "G4HadronPhysicsFTFP_BERT_HP.hh"   
#include "G4HadronPhysicsFTFP_BERT_TRV.hh"  
#include "G4HadronPhysicsINCLXX.hh"         
#include "G4HadronPhysicsNuBeam.hh"         
#include "G4HadronPhysicsQGS_BIC.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_AllHP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"
#include "G4HadronPhysicsShielding.hh"
#include "G4VHadronPhysics.hh"

// Ions
#include "G4IonBinaryCascadePhysics.hh"       
#include "G4IonINCLXXPhysics.hh"         
#include "G4IonPhysics.hh"
#include "G4IonPhysicsPHP.hh"
#include "G4IonQMDPhysics.hh"

// Limiters
// #include "G4FastSimulationPhysics.hh"
#include "G4GenericBiasingPhysics.hh"      
#include "G4ImportanceBiasing.hh"      
#include "G4NeutronTrackingCut.hh"
#include "G4ParallelWorldPhysics.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4WeightWindowBiasing.hh"

// Stopping
#include "G4StoppingPhysics.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
wuPhysicsList::wuPhysicsList(G4String fin, G4int ver)
  :fNameInput(fin)
{
  G4DataQuestionaire it(photon, neutron);
  G4cout << "<<< Reference Physics List wuPhysicsList "<<G4endl;

  this->SetVerboseLevel(ver);

  // Transportation
  AddTransportation();
  
  // EM Physics
  this->RegisterPhysics(new G4EmStandardPhysics(ver));
  // this->RegisterPhysics( new G4EmLowEPPhysics(ver) );

  // Synchroton Radiation & GN Physics
  this->RegisterPhysics(new G4EmExtraPhysics(ver));

  // Decays
  this->RegisterPhysics(new G4DecayPhysics(ver));

  // Radioactive decay
  this->RegisterPhysics(new G4RadioactiveDecayPhysics());

  // G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();
  // radioactiveDecay->SetICM(true);                //Internal Conversion
  // radioactiveDecay->SetARM(false);               //Atomic Rearangement
  // G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();  
  // ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());
  
  // // Need to initialize atomic deexcitation outside of radioactive decay
  G4LossTableManager* theManager = G4LossTableManager::Instance();
  G4VAtomDeexcitation* p = theManager->AtomDeexcitation();
  if (!p) {
     G4UAtomicDeexcitation* atomDeex = new G4UAtomicDeexcitation();
     theManager->SetAtomDeexcitation(atomDeex);
     atomDeex->SetFluo(true);
     atomDeex->SetAuger(true);
     atomDeex->SetPIXE(false);
     atomDeex->InitialiseAtomicDeexcitation();
  }
  
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1 * picosecond);// mandatory for G4NuclideTable
  G4NuclideTable::GetInstance()->SetLevelTolerance(1.0 * eV);  

  
  this->RegisterPhysics(new G4OpticalPhysics(0));

  
   // Hadron Physics
  // this->RegisterPhysics( new G4HadronElasticPhysicsXS(ver) );
  // this->RegisterPhysics( new G4StoppingPhysics(ver) );
  // this->RegisterPhysics( new G4IonPhysics(ver) );
  // this->RegisterPhysics( new G4HadronInelasticQBBC(ver));

  // Neutron tracking cut
  //this->RegisterPhysics( new G4NeutronTrackingCut(ver) );


   // Hadron Elastic scattering
  //this-> RegisterPhysics( new G4HadronElasticPhysicsHP(ver) );
  //this-> RegisterPhysics( new G4HadronElasticPhysics(ver) );


  // // Stopping Physics
  // //this->RegisterPhysics( new G4QStoppingPhysics(ver));
  // this->RegisterPhysics( new G4StoppingPhysics(ver));

  // // Ion Physics
  // this->RegisterPhysics(new G4IonQMDPhysics(ver)); //
  // //this->RegisterPhysics( new G4IonBinaryCascadePhysics(ver));
  // //this->RegisterPhysics( new G4IonPhysics(ver));

  // //Add by yushousong 20130415
  //this->RegisterPhysics(new sg4PhysNucl);
  
  // this->RegisterPhysics(new sg4PhysTrackingCut);
  // //for user limiter on logical vol

  //Add by wuhongyi
  // this->RegisterPhysics(new wuNeutronHPphysics(fNameInput));
  this->RegisterPhysics(new wuNeutronPhysics(fNameInput));

  
  //this->RegisterPhysics( new G4HadronElasticPhysicsLEND(ver));


  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
  G4EmParameters::Instance()->Dump();

  
}

wuPhysicsList::~wuPhysicsList()
{

}

void wuPhysicsList::SetCuts()
{
  
    G4double cutIn=wuReadData::ReadValue<double>("ParticleRangeCut", fNameInput)*mm;
    if(cutIn>=0.0)defaultCutValue=cutIn;
  //ParticleRangeCut in unit of mm. If a particle with E kinetic
  //less than that correponding to this range value will not be 
  //produced. This is only a producing threshold instead of a
  //tracking cut.
  
  this->SetCutsWithDefault();   
  G4cout<<"*_^Rangecut(producing threshold)="<<defaultCutValue<<"mm !"<< G4endl; 

  G4double lowLimit=wuReadData::ReadValue<double>("lowLimit", fNameInput)*eV;
  G4double highLimit=wuReadData::ReadValue<double>("highLimit", fNameInput)*GeV;
  if(lowLimit>=0.0 && highLimit>=0.0)
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowLimit, highLimit);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  //区域Production thresholds示例
  // Production thresholds for detector regions

  // G4Region* region;
  // G4String regName;
  // G4ProductionCuts* cuts;
  // regName = "tracker";
  // region = G4RegionStore::GetInstance()->GetRegion(regName);
  // cuts = new G4ProductionCuts;
  // cuts->SetProductionCut(0.01*mm); // same cuts for gamma, e- and e+
  // region->SetProductionCuts(cuts);
  // regName = "calorimeter";
  // region = G4RegionStore::GetInstance()->GetRegion(regName);
  // cuts = new G4ProductionCuts;
  // cuts->SetProductionCut(0.01*mm,G4ProductionCuts::GetIndex("gamma"));
  // cuts->SetProductionCut(0.1*mm,G4ProductionCuts::GetIndex("e-"));
  // cuts->SetProductionCut(0.1*mm,G4ProductionCuts::GetIndex("e+"));
  // region->SetProductionCuts(cuts);

  DumpCutValuesTable();
}


void wuPhysicsList::ConstructParticle()
{
  // pseudo-particles
  G4Geantino::GeantinoDefinition();
  
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  


}


// 
// wuPhysicsList.cc ends here
