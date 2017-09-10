// main.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 4月 19 14:25:47 2014 (+0800)
// Last-Updated: 四 5月  4 11:20:32 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 106
// URL: http://wuhongyi.cn 

//这是多线程的头文件
// #ifdef G4MULTITHREADED
// #include "G4MTRunManager.hh"
// #else
// #include "G4RunManager.hh"
// #endif

#include "G4RunManager.hh"

#include "wuReadData.hh"

//探测器构建
//madatory class - Detector Construction
#include "wuDetectorConstruction.hh"

//调用物理过程
//mandatory class - physics 
#include "wuPhysicsList.hh"
#include "FTF_BIC.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysic G4HadronPhysicsFTF_BIC G4StoppingPhysics G4IonPhysics G4NeutronTrackingCut
#include "FTFP_BERT_ATL.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysic G4HadronPhysicsFTFP_BERT_ATL G4StoppingPhysics G4IonPhysics G4NeutronTrackingCut 
#include "FTFP_BERT.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysic G4HadronPhysicsFTFP_BERT G4StoppingPhysics G4IonPhysics G4NeutronTrackingCut        
#include "FTFP_BERT_HP.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysicsHP G4HadronPhysicsFTFP_BERT_HP G4StoppingPhysics G4IonPhysics            
#include "FTFP_BERT_TRV.hh"// G4EmStandardPhysicsGS G4EmExtraPhysics G4DecayPhysics G4HadronHElasticPhysics G4HadronPhysicsFTFP_BERT_TRV G4StoppingPhysics G4IonPhysics G4NeutronTrackingCut
#include "FTFP_INCLXX.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysics G4HadronPhysicsINCLXX G4StoppingPhysics G4IonINCLXXPhysics G4NeutronTrackingCut
#include "FTFP_INCLXX_HP.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysicsHP G4HadronPhysicsINCLXX G4StoppingPhysics G4IonINCLXXPhysics
#include "G4GenericPhysicsList.hh"                    
#include "LBE.hh"// 这个比较复杂 -_-
#include "NuBeam.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysics G4HadronPhysicsNuBeam G4StoppingPhysics G4IonPhysics G4NeutronTrackingCut
#include "QBBC.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysicsXS G4StoppingPhysics G4IonPhysics G4HadronInelasticQBBC G4NeutronTrackingCut
#include "QGS_BIC.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysics G4HadronPhysicsQGS_BIC G4StoppingPhysics G4IonPhysics G4NeutronTrackingCut
#include "QGSP_BERT.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysics G4HadronPhysicsQGSP_BERT G4StoppingPhysics G4IonPhysics G4NeutronTrackingCut
#include "QGSP_BERT_HP.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysicsHP G4HadronPhysicsQGSP_BERT_HP G4StoppingPhysics G4IonPhysics
#include "QGSP_BIC_AllHP.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysicsPHP G4HadronPhysicsQGSP_BIC_AllHP G4StoppingPhysics G4IonPhysicsPHP
#include "QGSP_BIC.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysics G4HadronPhysicsQGSP_BIC G4StoppingPhysics G4IonPhysics G4NeutronTrackingCut
#include "QGSP_BIC_HP.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysicsHP G4HadronPhysicsQGSP_BIC_HP G4StoppingPhysics G4IonPhysics
#include "QGSP_FTFP_BERT.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysics G4HadronPhysicsQGSP_FTFP_BERT G4StoppingPhysics G4IonPhysics G4NeutronTrackingCut
#include "QGSP_INCLXX.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysics G4HadronPhysicsINCLXX G4StoppingPhysics G4IonINCLXXPhysics G4NeutronTrackingCut
#include "QGSP_INCLXX_HP.hh"// G4EmStandardPhysics G4EmExtraPhysics G4DecayPhysics G4HadronElasticPhysicsHP G4HadronPhysicsINCLXX G4StoppingPhysics G4IonINCLXXPhysics
#include "Shielding.hh"// 这个比较复杂,分好几种情况 -_-

#include "G4ParticleHPManager.hh"


//其它过程的管理
//not mandatory class - ActionInitialization
#include "wuActionInitialization.hh"

// 关于图形界面与交互接口
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "G4Version.hh"
#include "Randomize.hh"//随机数这里产生
#include <ctime>// initialize random seed
using namespace std;

int main(int argc,char** argv)
{
#if G4VERSION_NUMBER >= 1011
  
#else
  G4cout<<"Geant4 版本太低，无法支持本程序运行！"<<G4endl;
  return 0;
#endif


  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  if(wuReadData::ReadValue<int>("EnableRandSeed", "ReadData.txt")>0)
    {
      G4int seconds =  time(NULL);
      G4Random::setTheSeed(seconds);
    }


  // Construct the default run manager
  //
  //如果使用多线程，在这里设置线程数
// #ifdef G4MULTITHREADED
//   G4MTRunManager* runManager = new G4MTRunManager;
//   int threads=wuReadData::ReadValue<int>("NumberOfThreads", "ReadData.txt");
//   runManager->SetNumberOfThreads(threads); 
//   runManager->SetUserInitialization(new wuWorkerInitialization);
// #else
//   G4RunManager* runManager = new G4RunManager;
// #endif

  G4RunManager* runManager = new G4RunManager;
  
  // Set mandatory initialization classes ，410版本的框架是这样的，ActionInitialization来管理。
  //
  // Detector construction
  runManager->SetUserInitialization(new wuDetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physicsList = new wuPhysicsList();
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new wuActionInitialization());

  // Initialize G4 kernel
  runManager->Initialize();


  // Print   Data source of this Partile HP calculation
  G4ParticleHPManager::GetInstance()->DumpDataSource();

  
#ifdef G4VIS_USE
  G4VisManager* visManager = 0;
#endif
  
  
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if(argc == 1)
    {
      // 不带参数默认 运行Readata.txt中粒子数
      int RunBeamOn = wuReadData::ReadValue<int>("RunBeamOn", "ReadData.txt");
      runManager->BeamOn(RunBeamOn);
    }
  else
    {
      G4String commandopt = argv[1];
      if(commandopt == "vis.mac")// 开启图形界面模式
	{
#ifdef G4VIS_USE
	  visManager = new G4VisExecutive;
	  visManager->Initialize();
#endif   
#ifdef G4UI_USE
	  G4UIExecutive* ui = new G4UIExecutive(argc, argv);
	  UImanager->ApplyCommand("/control/execute vis.mac");
	  ui->SessionStart();
	  delete ui;
#endif
	}
      else
	{
	  if(commandopt == "-l")// 开启命令行模式
	    {
#ifdef G4UI_USE_TCSH    
	      G4UIsession* session = new G4UIterminal(new G4UItcsh);
	      session->SessionStart();
	      delete session;
#endif
	    }
	  else
	    {// 无图形界面执行脚本模式
	      G4String command = "/control/execute ";
	      G4String fileName = argv[1];
	      UImanager->ApplyCommand(command + fileName);
	    }
	}
    }


  
  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted 
  // in the main() program !
  

#ifdef G4VIS_USE
  if(visManager != 0)
    delete visManager;
#endif

  delete runManager;

  return 0;
}


// 
// main.cc ends here
