// Event.hh --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 5月  4 12:52:18 2017 (+0800)
// Last-Updated: 四 5月 25 21:45:23 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 18
// URL: http://wuhongyi.cn 

#ifndef _EVENT_H_
#define _EVENT_H_

#include "globals.hh"

#include "TFile.h"
#include "TTree.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Event
{
public:
  Event();
  virtual ~Event();

  static Event* Instance();
  void Reset();
  void ResetTrack();
  void ProcessOneTrack();

  void SetFileNumber(int run) {runnumber = run;}
  void InitWriteFile();
  void CloseWriteFile();
  void FillOneEvent();
public:

public:

  short hits;//一次252Cf裂变释放出来的n+g总数
  int sid[1024];//源的track id，其中+为neutron，-为gamma

  short hits1;//1号探测器的响应数
  int s1id[1024];//track id，其中+为proton，-为e
  double s1time[1024];
  int s1sid[1024];//track 最初的裂变出射粒子
  int s1pid[1024];//parent id
  double s1e[1024];//energy

  short hits2;//2号探测器的响应数
  int s2id[1024];//track id，其中+为proton，-为e
  double s2time[1024];
  int s2sid[1024];//track 最初的裂变出射粒子
  int s2pid[1024];//parent id
  double s2e[1024];//energy

  short hits3;//3号探测器的响应数
  int s3id[1024];//track id，其中+为proton，-为e
  double s3time[1024];
  int s3sid[1024];//track 最初的裂变出射粒子
  int s3pid[1024];//parent id
  double s3e[1024];//energy

  short hits4;//4号探测器的响应数
  int s4id[1024];//track id，其中+为proton，-为e
  double s4time[1024];
  int s4sid[1024];//track 最初的裂变出射粒子
  int s4pid[1024];//parent id
  double s4e[1024];//energy

  
  // ====
  bool scin[4];
  int trackid;
  double scintime[4];
  double scinenergy[4];
  int scininfo;
  int scinpid;
private:
  int runnumber;
  TFile *file;
  TTree *t;

  int counttest[4];
  
  static G4ThreadLocal Event* fgInstance;
};


#endif /* _EVENT_H_ */
// 
// Event.hh ends here
