// wuSteppingAction.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 日 4月 20 15:18:00 2014 (+0800)
// Last-Updated: 五 5月  5 10:14:36 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 49
// URL: http://wuhongyi.cn 

#ifndef _WUSTEPPINGACTION_H_
#define _WUSTEPPINGACTION_H_

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include <vector>
class G4VAnalysisManager;
class wuG4DataControl;
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class wuSteppingAction : public G4UserSteppingAction
{
public:
  wuSteppingAction(G4String fn="ReadData.txt");
  virtual ~wuSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);

private:
  // G4int    VolID;//宋老师特有
  G4int    EventID;//
  G4int    ParentID;//
  G4int    TrackID;//
  G4int    CurrentStepNumber;//       // Total steps number up to now
  G4String PName;//                           //particle name
  G4double TrackWeight;//                        // Track Weight
  G4String CreatorProcess;//
  G4double EDep;//
  G4double TrackLength;//          // Accumulated track length
  G4double StepLength;//
  G4int    TrackStatus;//

  G4double Mass;//                 // Dynamical mass of the particle静质量
  G4double Charge;//             // Dynamical Charge of the particle
  G4double MagneticMoment;//    // Dynamical MagneticMoment of the particle


  G4String VolNamePre;//                   //G4VPhysicalVolume name
  G4String VolNamePost;//这个暂时没用！！！
  G4double GlobalTimePre;//                   //Time since event is created
  G4double GlobalTimePost;//
  G4double LocalTimePre;//                  // Time since track is created
  G4double LocalTimePost;//
  G4double ProperTimePre;//            // Time since track is created (in rest frame of particle)
  G4double ProperTimePost;//
  G4int    StepStatusPre;//
  G4int    StepStatusPost;//
  G4double EkPre;//                    
  G4double EkPost;//
  G4ThreeVector PosPre;//
  G4ThreeVector PosPost;//
  G4ThreeVector MomentumDirectionPre;//
  G4double VelocityPre;//
  G4ThreeVector MomentumDirectionPost;//
  G4double VelocityPost;//
  G4int Info;//


  G4LogicalVolume * preStepVolume;
  G4LogicalVolume * postStepVolume;

  G4String fnInput;


  G4int FilterRegion;


  //G4int    StepIOVol;//宋老师特有  
  //G4int AtomicNumber;//宋老师特有,从theparticle获取信息 
  //G4int AtomicMass;//宋老师特有,从theparticle获取信息 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
enum G4StepStatus
  0-WorldBoundary,// Step reached the world boundary
  1-GeomBoundary,// Step defined by a geometry boundary
  2-AtRestDoItProc,// Step defined by a PreStepDoItVector
  3-AlongStepDoItProc,// Step defined by a AlongStepDoItVector
  4-PostStepDoItProc,// Step defined by a PostStepDoItVector
  5-UserDefinedLimit,// Step defined by the user Step limit in the logical volume
  6-ExclusivelyForcedProc,// Step defined by an exclusively forced PostStepDoIt process 
  7-Undefined.// Step not defined yet

enum G4TrackStatus
  0-Alive,// Continue the tracking
  1-StopButAlive,// Invoke active rest physics processes and kill the current track afterward
  2-StopAndKill,// Kill the current track
  3-KillTrackAndSecondaries,// Kill the current track and also associated secondaries.
  4-Suspend,// Suspend the current track
  5-PostponeToNextEvent.// Postpones the tracking of thecurrent track to the next event.
*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
  
//想要获取的数据从G4Track.hh跟G4StepPoint.hh文件中找，上面有的都可以输出。
// Member data 这些是G4StepPoint还没加进去的
   // G4TouchableHandle fpTouchable;
   //    //  Touchable Handle  
   // G4Material* fpMaterial;//感觉这个没意义，知道哪个几何体就知道材料了
   //    // Material of the volmue
   // const G4MaterialCutsCouple* fpMaterialCutsCouple;
   //    // MaterialCutsCouple of the volmue
   // G4VSensitiveDetector* fpSensitiveDetector;//探测器用不到
   // G4double fSafety;
   // G4ThreeVector fPolarization;


// Member data 这些是G4Track还没加进去的
  //  G4TouchableHandle fpTouchable;
  //  G4TouchableHandle fpNextTouchable;
  //  G4TouchableHandle fpOriginTouchable;
  // // Touchable Handle
  //  G4bool  fBelowThreshold;
  //  // This flag is set to true if this track energy is below
  //  // threshold energy in this material determined by the range cut value
  //  G4bool  fGoodForTracking;
  //  // This flag is set by processes if this track should be tracked
  //  // even if the energy is below threshold
  //  const G4LogicalVolume* fpLVAtVertex; //Logical Volume at the vertex
  //  G4int fCreatorModelIndex;           // Index of the physics model which created the track   
  //  G4VUserTrackInformation* fpUserInformation;
  //  // cached values for CalculateVelocity  
  //  mutable G4Material*               prev_mat;
  //  mutable G4MaterialPropertyVector* groupvel;
  //  mutable G4double                  prev_velocity;
  //  mutable G4double                  prev_momentum;
  //  G4bool          is_OpticalPhoton; 
  //  static G4ThreadLocal G4VelocityTable*  velTable;
  //  G4bool          useGivenVelocity;
  //     // do not calclulate velocity and just use current fVelocity
  //     // if this flag is set
};

#endif /* _WUSTEPPINGACTION_H_ */
// 
// wuSteppingAction.hh ends here
