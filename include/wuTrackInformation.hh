// wuTrackInformation.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 六 12月 13 19:28:22 2014 (+0800)
// Last-Updated: 五 5月  5 20:15:16 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 21
// URL: http://wuhongyi.cn 

#ifndef _WUTRACKINFORMATION_H_
#define _WUTRACKINFORMATION_H_

#include "G4VUserTrackInformation.hh"
#include "G4Allocator.hh"
#include "globals.hh"
#include "G4Track.hh"
// enum TrackFlag {undefined,flag1,flag2,flag3};//暂时随便设置

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class wuTrackInformation : public G4VUserTrackInformation
{
public:
  wuTrackInformation();
  virtual ~wuTrackInformation();
  wuTrackInformation(const G4Track* aTrack);//未完待续
  wuTrackInformation(const wuTrackInformation* Info);
  inline virtual void Print() const{};

  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);


  //想传递什么信息都可通过Set Get 函数实现

  // Try adding a flag and return if it is successful or not
  // Cannot Add Undefine or a flag that conflicts with another flag
  // Return true if the addition of flag is successful, false otherwise
  G4bool AddFlag(G4int ss);
  G4int PrintFlag()const { return fFlag; }
  // Check if a certain flag is on
  G4bool IsFlag(G4int ss)
  {return ss != 0 ? true : false; }


  void SetFissionFlag(G4bool fission) {fFission = fission;}
  G4bool GetFissionFlag() {return fFission;}
private:
  G4int fFlag;//默认0
  G4bool fFission;
};

extern G4ThreadLocal G4Allocator<wuTrackInformation> * aTrackInformationAllocator;

inline void* wuTrackInformation::operator new(size_t)
{
  if(!aTrackInformationAllocator)
    aTrackInformationAllocator = new G4Allocator<wuTrackInformation>;
  return (void*)aTrackInformationAllocator->MallocSingle();
}

inline void wuTrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator->FreeSingle((wuTrackInformation*)aTrackInfo);}


#endif /* _WUTRACKINFORMATION_H_ */

// 
// wuTrackInformation.hh ends here
