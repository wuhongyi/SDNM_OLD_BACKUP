// wuFissionEvent.cc --- 
// 
// Description: 
// Author: Hongyi Wu(吴鸿毅)
// Email: wuhongyi@qq.com 
// Created: 四 4月 20 16:27:19 2017 (+0800)
// Last-Updated: 四 4月 20 18:39:45 2017 (+0800)
//           By: Hongyi Wu(吴鸿毅)
//     Update #: 10
// URL: http://wuhongyi.cn 

// This software was developed by Lawrence Livermore National Laboratory.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//   derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2006 The Regents of the University of California.
// All rights reserved.
// UCRL-CODE-224807
//
// $Id: G4fissionEvent.cc 68799 2013-04-05 13:29:46Z gcosmo $
//

#include "wuFissionEvent.hh"

#define nfissg 40
#define alphanegbin 26
#define nSPfissIso 8
#define nSPfissn 11
#define nSPfissNubarIso 18
#define nSPfissg 30
#define TWOPI 6.283185307
#define SQRT2 1.414213562
#define BSHIFT -0.43287
#define WIDTH 1.079
#define nZAfis 39    /* 38 fissionable isotopes in ENDL + U-232 from ENDF7 */
#define WATTEMIN 1.0e-6
#define WATTEMAX 20.0

G4int wuFissionEvent::delayoption=0;
G4int wuFissionEvent::correlationoption=0;
G4int wuFissionEvent::nudistoption=3;
G4int wuFissionEvent::Cf252ndistoption=0;
G4int wuFissionEvent::Cf252nengoption=0;

wuFissionEvent::wuFissionEvent(G4int isotope, G4double time,
                               G4double nubar, G4double eng)
 :neutronNu(0), neutronEnergies(0), neutronVelocities(0), neutronDircosu(0),
  neutronDircosv(), neutronDircosw(), neutronAges(0),
  photonNu(0), photonEnergies(0), photonVelocities(0), photonDircosu(0),
  photonDircosv(0), photonDircosw(0), photonAges(0)
{
   /*
    * Constructs a fission event with neutronNu neutrons and photonNu
    * photons.
    */
   G4int i;

   if (nubar == -1.) {
      /* spontaneous fission */
      neutronNu = G4SmpSpNuDistData(isotope, Cf252ndistoption);
      photonNu = G4SmpSpNugDistData(isotope);
   } else {
      /* induced fission */
      if (nudistoption == 0 || nudistoption == 1) {
         switch (isotope) {
            case 92235:
               neutronNu = G4SmpNuDistDataU235(eng,nudistoption);
               break;
            case 92238:
               neutronNu = G4SmpNuDistDataU238(eng);
               break;
            case 94239:
               neutronNu = G4SmpNuDistDataPu239(eng);
               break;
            default:
               neutronNu = (G4int) G4SmpTerrell(nubar);
               break;
         } 
      } else if (nudistoption == 2) {
         switch (isotope) {
            case 92232:
            case 92234:
            case 92236:
            case 92238:
               neutronNu = G4SmpNuDistDataU232_234_236_238(nubar);
               break;
            case 92233:
            case 92235:
               neutronNu = (G4int) G4SmpNuDistDataU233_235(nubar);
               break;
            case 94239:
            case 94241:
               neutronNu = G4SmpNuDistDataPu239_241(nubar);
               break;
            default:
               neutronNu = (G4int) G4SmpTerrell(nubar);
               break;
         }
      } else if (nudistoption == 3) {
         switch (isotope) {
            case 92232:
            case 92234:
            case 92236:
            case 92238:
               neutronNu = G4SmpNuDistDataU232_234_236_238_MC(nubar);
               break;
            case 92233:
            case 92235:
               neutronNu = (G4int) G4SmpNuDistDataU233_235_MC(nubar);
               break;
            case 94239:
            case 94241:
               neutronNu = G4SmpNuDistDataPu239_241_MC(nubar);
               break;
            default:
               neutronNu = (G4int) G4SmpTerrell(nubar);
               break;
         } 
      }
      photonNu = G4SmpNugDist(isotope, nubar);
   }
   if (neutronNu > 0) {
      neutronEnergies = new G4double[ neutronNu ];
      neutronVelocities = new G4double[ neutronNu ];
      neutronDircosu = new G4double[ neutronNu ];
      neutronDircosv = new G4double[ neutronNu ];
      neutronDircosw = new G4double[ neutronNu ];
      neutronAges = new G4double[neutronNu];
      for (i=0; i<neutronNu; i++) {
         if (isotope == 98252) neutronEnergies[i] = G4SmpNEngCf252(Cf252nengoption);
         else neutronEnergies[i] = G4SmpWatt(eng, isotope);
         neutronVelocities[i] = G4SmpNVel(
                 neutronEnergies[i],
                 &(neutronDircosu[i]),
                 &(neutronDircosv[i]),
                 &(neutronDircosw[i])
                );
         neutronAges[i] = time;
      }
   }
   if (photonNu > 0) {
      photonEnergies = new G4double[photonNu];
      photonVelocities = new G4double[photonNu];
      photonDircosu = new G4double[photonNu];
      photonDircosv = new G4double[photonNu];
      photonDircosw = new G4double[photonNu];
      photonAges = new G4double[photonNu];
      for (i=0; i<photonNu; i++) {
         photonEnergies[i] = G4SmpGEng();
         photonVelocities[i] = G4SmpPVel(
                 photonEnergies[i],
                 &(photonDircosu[i]),
                 &(photonDircosv[i]),
                 &(photonDircosw[i])
                );
         photonAges[i] = time;
      }
   }
}

wuFissionEvent::~wuFissionEvent() {
   if (neutronNu > 0) {
      delete [] neutronEnergies;
      delete [] neutronVelocities;
      delete [] neutronDircosu;
      delete [] neutronDircosv;
      delete [] neutronDircosw;
      delete [] neutronAges;
   }

   if (photonNu > 0) {
      delete [] photonEnergies;
      delete [] photonVelocities;
      delete [] photonDircosu;
      delete [] photonDircosv;
      delete [] photonDircosw;
      delete [] photonAges;
   }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <cmath>
#include "G4Pow.hh"
#include "G4Exp.hh"
#include "G4Log.hh"
#include "G4PhysicalConstants.hh"

G4int wuFissionEvent::G4SmpNuDistDataU232_234_236_238(G4double nubar) {

/*
  Description
    Sample Number of Neutrons from fission in U-232, U-234, U-236 
    and U-238 using Zucker and Holden's tabulated data for U-238
    The P(nu) distribution is given as a function of the average 
    number of neutrons from fission, based on interpolation of the 
    U-238 data from Zucker and Holden.
*/

/*
  Input
    nubar    - average number of neutrons per fission
  Output
    G4SmpNuDistDataU232_234_236_238  - sampled multiplicity
    
*/

  G4double pnu[9], cpnu, sum;
  G4double r;

/* 
  Check if nubar is within the range of experimental values
*/
  if (nubar >= 2.25 && nubar <= 3.80) {
/*
     Use Zucker and Holden Data
*/
     G4Pow* Pow=G4Pow::GetInstance();
     pnu[0]=-7.705432e-3*Pow->powN(nubar,3)+8.904671e-2*Pow->powN(nubar,2)-3.488123e-1*nubar+4.627291e-1;
     pnu[1]=-2.879938e-2*Pow->powN(nubar,3)+3.629189e-1*Pow->powN(nubar,2)-1.545284*nubar+2.229503;
     pnu[2]=6.543684e-2*Pow->powN(nubar,3)-6.673117e-1*Pow->powN(nubar,2)+2.087358*nubar-1.771396;
     pnu[3]=1.412971e-2*Pow->powN(nubar,3)-2.309842e-1*Pow->powN(nubar,2)+1.022451*nubar-1.032235;
     pnu[4]=-5.163167e-2*Pow->powN(nubar,3)+4.457516e-1*Pow->powN(nubar,2)-1.114981*nubar+9.484241e-1;
     pnu[5]=8.758841e-4*Pow->powN(nubar,3)+3.707461e-2*Pow->powN(nubar,2)-1.565149e-1*nubar+1.851039e-1;
     pnu[6]=-3.871089e-5*Pow->powN(nubar,3)+1.936524e-2*Pow->powN(nubar,2)-8.091057e-2*nubar+9.019871e-2;
     pnu[7]=3.945995e-3*Pow->powN(nubar,3)-2.697509e-2*Pow->powN(nubar,2)+6.237296e-2*nubar-4.820745e-2;
     pnu[8]=1.708054e-3*Pow->powN(nubar,4)-1.706039e-2*Pow->powN(nubar,3)+6.550213e-2*Pow->powN(nubar,2)-1.135e-1*nubar+7.443828e-2;

     sum=pnu[0]+pnu[1]+pnu[2]+pnu[3]+pnu[4]+pnu[5]+pnu[6]+pnu[7]+pnu[8];

     pnu[0]=pnu[0]/sum;
     pnu[1]=pnu[1]/sum;
     pnu[2]=pnu[2]/sum;
     pnu[3]=pnu[3]/sum;
     pnu[4]=pnu[4]/sum;
     pnu[5]=pnu[5]/sum;
     pnu[6]=pnu[6]/sum;
     pnu[7]=pnu[7]/sum;
     pnu[8]=pnu[8]/sum;

     r=fisslibrng();

     if(r <= pnu[0]) return 0;

     cpnu=pnu[0]+pnu[1];
     if(r <= cpnu) return 1;

     cpnu=cpnu+pnu[2];
     if(r <= cpnu) return 2;

     cpnu=cpnu+pnu[3];
     if(r <= cpnu) return 3;

     cpnu=cpnu+pnu[4];
     if(r <= cpnu) return 4;

     cpnu=cpnu+pnu[5];
     if(r <= cpnu) return 5;

     cpnu=cpnu+pnu[6];
     if(r <= cpnu) return 6;

     cpnu=cpnu+pnu[7];
     if(r <= cpnu) return 7;
     else return 8;

  } else {
/*
     Use Terrell's formula
*/
     return (G4int) G4SmpTerrell(nubar);
  }
}

G4int wuFissionEvent::G4SmpNuDistDataU232_234_236_238_MC(G4double nubar) {

/*
  Description
    Sample Number of Neutrons from fission in U-232, U-234, U-236, 
    and U-238 using Zucker and Holden's tabulated data for U-238
    The 11 P(nu) distributions are given as a function of nubar, 
    the average number of neutrons from induced fission for the 
    11 different energies (0 to 10 MeV), based on the U-238 data 
    from Zucker and Holden.
*/

/*
  Input
    nubar    - average number of neutrons per fission
  Output
    G4SmpNuDistDataU232_234_236_238_MC  - sampled multiplicity
    
*/

  static G4double U238nu [11] [9] = {
     {.0396484, .2529541, .2939544, .2644470, .1111758, .0312261, .0059347, .0005436, .0001158},
     {.0299076, .2043215, .2995886, .2914889, .1301480, .0363119, .0073638, .0006947, .0001751},
     {.0226651, .1624020, .2957263, .3119098, .1528786, .0434233, .0097473, .0009318, .0003159},
     {.0170253, .1272992, .2840540, .3260192, .1779579, .0526575, .0130997, .0013467, .0005405},
     {.0124932, .0984797, .2661875, .3344938, .2040116, .0640468, .0173837, .0020308, .0008730},
     {.0088167, .0751744, .2436570, .3379711, .2297901, .0775971, .0225619, .0030689, .0013626},
     {.0058736, .0565985, .2179252, .3368863, .2541575, .0933127, .0286200, .0045431, .0031316},
     {.0035997, .0420460, .1904095, .3314575, .2760413, .1112075, .0355683, .0065387, .0031316},
     {.0019495, .0309087, .1625055, .3217392, .2943792, .1313074, .0434347, .0091474, .0046284},
     {.0008767, .0226587, .1356058, .3076919, .3080816, .1536446, .0522549, .0124682, .0067176},
     {.0003271, .0168184, .1111114, .2892434, .3160166, .1782484, .0620617, .0166066, .0095665}
    };
  static G4double U238nubar [11] = {
      2.2753781,
      2.4305631,
      2.5857481,
      2.7409331,
      2.8961181,
      3.0513031,
      3.2064881,
      3.3616731,
      3.5168581,
      3.6720432,
      3.8272281
    };
  G4double fraction, r, cum;
  G4int engind, nu;

/* 
  Check if nubar is within the range of experimental values
*/
  if(nubar >= U238nubar[0] && nubar <= U238nubar[10]) {
/*
     Use Zucker and Holden Data
*/
     engind = 1;
     while (nubar > U238nubar[engind]){ engind++;}
     // Loop checking, 11.03.2015, T. Koi
     fraction = (nubar-U238nubar[engind-1])/(U238nubar[engind]-U238nubar[engind-1]);
     if(fisslibrng() > fraction) engind--;

     r = fisslibrng();
     nu = 0;
     cum = U238nu[engind][0];
     while (r > cum && nu < 8){ 
     // Loop checking, 11.03.2015, T. Koi
       nu++;
       cum += U238nu[engind][nu];
     }
     return nu;
  } else {
/*
     Use Terrell's formula
*/
     return (G4int) G4SmpTerrell(nubar);
  }
}

int wuFissionEvent::G4SmpNuDistDataU233_235(G4double nubar) {

  G4Pow* Pow = G4Pow::GetInstance();
/*
  Description
    Sample Number of Neutrons from fission in U-233 and U-235 using 
    Zucker and Holden's tabulated data for U-235
    The P(nu) distribution is given as a function of the average 
    number of neutrons from fission, based on interpolation of the 
    U-235 data from Zucker and Holden.
*/

/*
  Input
    nubar    - average number of neutrons per fission
  Output
    G4SmpNuDistDataU233_235  - sampled multiplicity
    
*/

  G4double pnu[8], cpnu, sum;
  G4double r;

/* 
  Check if nubar is within the range of experimental values
*/
  if(nubar >= 2.25 && nubar <= 4.0) {
/*
     Use Zucker and Holden Data
*/
     if(nubar <= 2.8738) pnu[0]=-9.279554e-02*Pow->powN(nubar,3)+8.036687e-01*Pow->powN(nubar,2)-2.342684*nubar+2.309035;
     else if(nubar > 2.8738 && nubar <= 3.4272) pnu[0]=1.50072e-2*Pow->powN(nubar,2)-1.109109e-1*nubar+2.063133e-1;
     else pnu[0]=1.498897e+3*G4Exp(-3.883864*nubar);

     if(nubar <= 3.2316) pnu[1]=3.531126e-2*Pow->powN(nubar,3)-2.787213e-1*Pow->powN(nubar,2)+5.824072e-1*nubar-1.067136e-1;
     else pnu[1]=6.574492e-2*Pow->powN(nubar,2)-5.425741e-1*nubar+1.123199;

     pnu[2]=1.274643e-2*Pow->powN(nubar,3)-1.387954e-1*Pow->powN(nubar,2)+3.264669e-1*nubar+1.77148e-1;

     pnu[3]=5.473738e-2*Pow->powN(nubar,5)-8.835826e-1*Pow->powN(nubar,4)+5.657201*Pow->powN(nubar,3)-1.802669e+1*Pow->powN(nubar,2)+2.867937e+1*nubar-1.794296e+1;

     pnu[4]=-3.591076e-2*Pow->powN(nubar,3)+3.092624e-1*Pow->powN(nubar,2)-7.184805e-1*nubar+5.649400e-1;

     if(nubar <= 2.8738) pnu[5]=1.699374e-2*Pow->powN(nubar,2)-1.069558e-3*nubar-6.981430e-2;
     else pnu[5]=2.100175e-2*Pow->powN(nubar,3)-1.705788e-1*Pow->powN(nubar,2)+5.575467e-1*nubar-6.245873e-1;

     if(nubar <= 3.0387) pnu[6]=9.431919e-7*Pow->powA(nubar,8.958848);
     else pnu[6]=4.322428e-3*Pow->powN(nubar,3)-2.094790e-2*Pow->powN(nubar,2)+4.449671e-2*nubar-4.435987e-2;

     pnu[7]=5.689084e-3*Pow->powN(nubar,4)-6.591895e-2*Pow->powN(nubar,3)+2.886861e-1*Pow->powN(nubar,2)-5.588146e-1*nubar+4.009166e-1;

     sum=pnu[0]+pnu[1]+pnu[2]+pnu[3]+pnu[4]+pnu[5]+pnu[6]+pnu[7];

     pnu[0]=pnu[0]/sum;
     pnu[1]=pnu[1]/sum;
     pnu[2]=pnu[2]/sum;
     pnu[3]=pnu[3]/sum;
     pnu[4]=pnu[4]/sum;
     pnu[5]=pnu[5]/sum;
     pnu[6]=pnu[6]/sum;
     pnu[7]=pnu[7]/sum;

     r=fisslibrng();

     if(r <= pnu[0]) return (int) 0;

     cpnu=pnu[0]+pnu[1];
     if(r <= cpnu) return (int) 1;

     cpnu=cpnu+pnu[2];
     if(r <= cpnu) return (int) 2;

     cpnu=cpnu+pnu[3];
     if(r <= cpnu) return (int) 3;

     cpnu=cpnu+pnu[4];
     if(r <= cpnu) return (int) 4;

     cpnu=cpnu+pnu[5];
     if(r <= cpnu) return (int) 5;

     cpnu=cpnu+pnu[6];
     if(r <= cpnu) return (int) 6;
     else return (int) 7;
  } else {
/*
     Use Terrell's formula
*/
     return (int) G4SmpTerrell(nubar);
  }
}


G4int wuFissionEvent::G4SmpNuDistDataU233_235_MC(G4double nubar) {

/*
  Description
    Sample Number of Neutrons from fission in U-233 and U-235 using 
    Zucker and Holden's tabulated data for U-235
    The 11 P(nu) distributions are given as a function of nubar, 
    the average number of neutrons from induced fission for the 
    11 different energies (0 to 10 MeV), based on the U-235 data 
    from Zucker and Holden.
*/

/*
  Input
    nubar    - average number of neutrons per fission
  Output
    G4SmpNuDistDataU233_235_MC  - sampled multiplicity
    
*/

  static G4double U235nu [11] [8] = {
     {.0317223, .1717071, .3361991, .3039695, .1269459, .0266793, .0026322, .0001449},
     {.0237898, .1555525, .3216515, .3150433, .1444732, .0356013, .0034339, .0004546},
     {.0183989, .1384891, .3062123, .3217566, .1628673, .0455972, .0055694, .0011093},
     {.0141460, .1194839, .2883075, .3266568, .1836014, .0569113, .0089426, .0019504},
     {.0115208, .1032624, .2716849, .3283426, .2021206, .0674456, .0128924, .0027307},
     {.0078498, .0802010, .2456595, .3308175, .2291646, .0836912, .0187016, .0039148},
     {.0046272, .0563321, .2132296, .3290407, .2599806, .1045974, .0265604, .0056322},
     {.0024659, .0360957, .1788634, .3210507, .2892537, .1282576, .0360887, .0079244},
     {.0012702, .0216090, .1472227, .3083032, .3123950, .1522540, .0462449, .0107009},
     {.0007288, .0134879, .1231200, .2949390, .3258251, .1731879, .0551737, .0135376},
     {.0004373, .0080115, .1002329, .2779283, .3342611, .1966100, .0650090, .0175099}
    };
  static G4double U235nubar [11] = {
      2.4140000, 
      2.5236700, 
      2.6368200, 
      2.7623400, 
      2.8738400, 
      3.0386999, 
      3.2316099, 
      3.4272800, 
      3.6041900, 
      3.7395900, 
      3.8749800
    };
  G4double fraction, r, cum;
  G4int engind, nu;

/* 
  Check if nubar is within the range of experimental values
*/
  if(nubar >= U235nubar[0] && nubar <= U235nubar[10]) {
/*
     Use Zucker and Holden Data
*/
     engind = 1;
     while (nubar > U235nubar[engind]){ engind++;}
     // Loop checking, 11.03.2015, T. Koi
     fraction = (nubar-U235nubar[engind-1])/(U235nubar[engind]-U235nubar[engind-1]);
     if(fisslibrng() > fraction) engind--;

     r = fisslibrng();
     nu = 0;
     cum = U235nu[engind][0];
     while (r > cum && nu < 7){ 
     // Loop checking, 11.03.2015, T. Koi
       nu++;
       cum += U235nu[engind][nu];
     }
     return nu;
  } else {
/*
     Use Terrell's formula
*/
     return (G4int) G4SmpTerrell(nubar);
  }
}


G4int wuFissionEvent::G4SmpNuDistDataU235(G4double erg, G4int option) {

/*
  Description
    Sample Number of Neutrons from fission in U-235 using probability
    distributions based on either
      (option 0) Zucker and Holden's tabulated data for U-235
      (option 1) Zucker and Holden's tabulated data for U-235 and
                 Gwin, Spencer and Ingle tabulated data for U-235 
                 at thermal energies
*/

/*
  Input
    erg      - incident neutron energy
    option   - 0 for sampling Zucker and Holden probability distributions
               1 for sampling probability distributions based on Zucker 
                 and Holden tabulated distributions as well as Gwin, 
                 Spencer and Ingle tabulated distributions at thermal 
                 energies
  Output
    G4SmpNuDistDataU235  - sampled multiplicity
    
*/
 
  G4double cpnu;
  G4double pnu[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  G4double eng;
  G4double r;

/* 
  Check if energy is within the range of experimental values
*/
  if (erg > 10) eng=10.;
  else eng=erg;

  r=fisslibrng();
/*
  U-235 nu distribution
*/
  G4Pow* Pow=G4Pow::GetInstance();
  if (option == 0) {
     if (eng <= 3.0) pnu[0]=0.0317223e0-9.67117e-3*eng+1.9726e-3*Pow->powN(eng,2)-2.33933e-4*Pow->powN(eng,3);
     if (eng > 3 && eng <= 7) pnu[0]=-1.24147e-2+2.52982e-2*eng-7.88108e-3*Pow->powN(eng,2)+9.10008e-4*Pow->powN(eng,3)-3.67208e-5*Pow->powN(eng,4);
     if (eng > 7 && eng <= 10) pnu[0]=6.31258e-2-1.89764e-2*eng+1.94475e-3*Pow->powN(eng,2)-6.74e-5*Pow->powN(eng,3);
     if (r <= pnu[0]) return 0;

     if (eng <= 4.0) pnu[1]=0.171707e0-0.0178305e0*eng+3.42286e-3*Pow->powN(eng,2)-2.1168e-3*Pow->powN(eng,3)+3.84226e-4*Pow->powN(eng,4)-1.44289e-5*Pow->powN(eng,5);
     if (eng > 4 && eng <= 7) pnu[1]=9.8633e-2+3.53323e-2*eng-1.15037e-2*Pow->powN(eng,2)+7.4e-4*Pow->powN(eng,3);
     if (eng > 7 && eng <= 10) pnu[1]=0.628295-0.180677*eng+1.80664e-2*Pow->powN(eng,2)-6.2015e-4*Pow->powN(eng,3);
     cpnu=pnu[0]+pnu[1];
     if (r <= cpnu) return 1;

     if (eng <= 4.0) pnu[2]=0.336199e0-1.59569e-2*eng+2.78036e-3*Pow->powN(eng,2)-1.59278e-3*Pow->powN(eng,3)+2.21742e-4*Pow->powN(eng,4);
     if (eng > 4 && eng <= 8) pnu[2]=0.229153e0+5.27561e-2*eng-1.29288e-2*Pow->powN(eng,2)+5.67233e-4*Pow->powN(eng,3)+8.06667e-6*Pow->powN(eng,4);
     if (eng > 8 && eng <= 10) pnu[2]=-0.395206e0+0.227399e0*eng-2.86051e-2*Pow->powN(eng,2)+1.08196e-3*Pow->powN(eng,3);
     cpnu=cpnu+pnu[2];
     if (r <= cpnu) return 2;

     if (eng <= 5.0) pnu[3]=0.30395461e0+0.01348261e0*eng-0.00262298e0*Pow->powN(eng,2)+1.99482407e-4*Pow->powN(eng,3);
     if (eng > 5 && eng <= 10) pnu[3]=0.10992355e0+0.09246839e0*eng-0.00885344e0*Pow->powN(eng,2)-7.60589252e-4*Pow->powN(eng,3)+1.50973591e-4*Pow->powN(eng,4)-6.20436503e-6*Pow->powN(eng,5);
     cpnu=cpnu+pnu[3];
     if (r <= cpnu) return 3;

     if (eng <= 4.0) pnu[4]=0.126946e0+1.64489e-2*eng+2.44029e-3*Pow->powN(eng,2)-2.1019e-3*Pow->powN(eng,3)+8.50104e-4*Pow->powN(eng,4)-1.10127e-4*Pow->powN(eng,5);
     if (eng > 4 && eng <= 8) pnu[4]=0.263373e0-7.47799e-2*eng+2.0588e-2*Pow->powN(eng,2)-1.55132e-3*Pow->powN(eng,3)+3.025e-5*Pow->powN(eng,4);
     if (eng > 8 && eng <= 10) pnu[4]=-0.277491e0+0.157606e0*eng-1.38467e-2*Pow->powN(eng,2)+4.20357e-4*Pow->powN(eng,3);
     cpnu=cpnu+pnu[4];
     if (r <= cpnu) return 4;

     if (eng <= 4.0) pnu[5]=0.0266793e0+9.05206e-3*eng-6.58754e-4*Pow->powN(eng,2)+6.26292e-4*Pow->powN(eng,3)-9.75958e-5*Pow->powN(eng,4);
     if (eng > 4 && eng <= 8) pnu[5]=0.0693092e0-1.46524e-2*eng+3.2841e-3*Pow->powN(eng,2)+1.50833e-4*Pow->powN(eng,3)-2.13e-5*Pow->powN(eng,4);
     if (eng > 8 && eng <= 10) pnu[5]=0.881442e0-0.271486e0*eng+3.15097e-2*Pow->powN(eng,2)-1.12095e-3*Pow->powN(eng,3);
     cpnu=cpnu+pnu[5];
     if (r <= cpnu) return 5;


     if (eng <= 4.0) pnu[6]=0.0026322e0+2.44017e-4*eng+4.55992e-4*Pow->powN(eng,2)+1.25233e-4*Pow->powN(eng,3)-2.35417e-5*Pow->powN(eng,4);
     if (eng > 4 && eng <= 8) pnu[6]=-5.3989e-3+9.48298e-3*eng-2.95864e-3*Pow->powN(eng,2)+5.43025e-4*Pow->powN(eng,3)-2.75625e-5*Pow->powN(eng,4);
     if (eng > 8 && eng <= 10) pnu[6]=0.177058-5.57839e-2*eng+6.81359e-3*Pow->powN(eng,2)-2.35568e-4*Pow->powN(eng,3);
     cpnu=cpnu+pnu[6];
     if (r <= cpnu) return 6;
     else return 7;

  } else if (option == 1) {
     if (eng <= 3.0) pnu[0]=0.0291000e0-4.836167e-3*eng-6.72500e-4*Pow->powN(eng,2)+2.076667e-4*Pow->powN(eng,3);
     if (eng > 3 && eng <= 7) pnu[0]=-1.23950e-2+2.52790e-2*eng-7.874333e-3*Pow->powN(eng,2)+9.09000e-4*Pow->powN(eng,3)-3.666667e-5*Pow->powN(eng,4);
     if (eng > 7 && eng <= 10) pnu[0]=6.328200e-2-1.903283e-2*eng+1.951500e-3*Pow->powN(eng,2)-6.766667e-5*Pow->powN(eng,3);
     if (r <= pnu[0]) return 0;

     if (eng <= 4.0) pnu[1]=0.166000e0-0.005591833e0*eng-5.624500e-3*Pow->powN(eng,2)+7.673333e-4*Pow->powN(eng,3)-2.00000e-6*Pow->powN(eng,4);
     if (eng > 4 && eng <= 7) pnu[1]=9.860600e-2+3.534733e-2*eng-1.150650e-2*Pow->powN(eng,2)+7.401667e-4*Pow->powN(eng,3);
     if (eng > 7 && eng <= 10) pnu[1]=0.628401e0-0.1807157e0*eng+1.807100e-2*Pow->powN(eng,2)-6.203333e-4*Pow->powN(eng,3);
     cpnu=pnu[0]+pnu[1];
     if (r <= cpnu) return 1;

     if (eng <= 4.0) pnu[2]=0.336200e0-1.596058e-2*eng+2.783625e-3*Pow->powN(eng,2)-1.593917e-3*Pow->powN(eng,3)+2.21875e-4*Pow->powN(eng,4);
     if (eng > 4 && eng <= 8) pnu[2]=0.2292350e0+5.26925e-2*eng-1.291067e-2*Pow->powN(eng,2)+5.650000e-4*Pow->powN(eng,3)+8.166667e-6*Pow->powN(eng,4);
     if (eng > 8 && eng <= 10) pnu[2]=0.3838230e0-3.4439e-2*eng+6.0800e-4*Pow->powN(eng,2);
     cpnu=cpnu+pnu[2];
     if (r <= cpnu) return 2;

     if (eng <= 4.0) pnu[3]=0.3074000e0+0.00794125e0*eng-0.0002580417e0*Pow->powN(eng,2)-1.875000e-5*Pow->powN(eng,3)-2.145833e-5*Pow->powN(eng,4);
     if (eng > 4 && eng <= 7) pnu[3]=0.3152270e0-2.623667e-3*eng+2.785000e-3*Pow->powN(eng,2)-3.273333e-4*Pow->powN(eng,3);
     if (eng > 7 && eng <= 10) pnu[3]=0.6476430e0-0.1046148e0*eng+1.181600e-2*Pow->powN(eng,2)-5.051667e-4*Pow->powN(eng,3);
     cpnu=cpnu+pnu[3];
     if (r <= cpnu) return 3;

     if (eng <= 4.0) pnu[4]=0.133300e0+5.853750e-3*eng+6.200875e-3*Pow->powN(eng,2)-8.95250e-4*Pow->powN(eng,3)+1.36250e-5*Pow->powN(eng,4);
     if (eng > 4 && eng <= 7) pnu[4]=0.2379650e0-5.548167e-2*eng+1.517350e-2*Pow->powN(eng,2)-8.858333e-4*Pow->powN(eng,3);
     if (eng > 7 && eng <= 10) pnu[4]=-0.5408690e0+0.2461313e0*eng-2.372350e-2*Pow->powN(eng,2)+7.861667e-4*Pow->powN(eng,3);
     cpnu=cpnu+pnu[4];
     if (r <= cpnu) return 4;

     if (eng <= 4.0) pnu[5]=0.025900e0+1.067450e-2*eng-1.794000e-3*Pow->powN(eng,2)+9.50500e-4*Pow->powN(eng,3)-1.3000e-4*Pow->powN(eng,4);
     if (eng > 4 && eng <= 7) pnu[5]=0.0871960e0-2.823683e-2*eng+7.0955e-3*Pow->powN(eng,2)-3.176667e-4*Pow->powN(eng,3);
     if (eng > 7 && eng <= 10) pnu[5]=-0.591650e0+0.2236360e0*eng-2.373100e-2*Pow->powN(eng,2)+9.25000e-4*Pow->powN(eng,3);
     cpnu=cpnu+pnu[5];
     if (r <= cpnu) return 5;

     if (eng <= 4.0) pnu[6]=0.002100e0+1.35500e-3*eng-3.235833e-4*Pow->powN(eng,2)+3.48500e-4*Pow->powN(eng,3)-4.591667e-5*Pow->powN(eng,4);
     if (eng > 4 && eng <= 8) pnu[6]=1.767200e-2-8.055667e-3*eng+1.96650e-3*Pow->powN(eng,2)-6.283333e-5*Pow->powN(eng,3);
     if (eng > 8 && eng <= 10) pnu[6]=-0.2485310e0+8.72590e-2*eng-9.14550e-3*Pow->powN(eng,2)+3.555000e-4*Pow->powN(eng,3);
     cpnu=cpnu+pnu[6];
     if (r <= cpnu) return 6;
     else return 7;
  }
  //
  // Fall through
  //

  G4cout << " SmpNuDistDataU235: unrecognized option = " << option << G4endl;
  return -1;

}

G4int wuFissionEvent::G4SmpNuDistDataPu239(G4double erg) {

/*
  Description
    Sample Number of Neutrons from fission in Pu-239 using 
    Zucker and Holden's tabulated data for Pu-239
*/

/*
  Input
    erg      - incident neutron energy
  Output
    G4SmpNuDistDataPu239  - sampled multiplicity
    
*/
 
  G4double cpnu;
  G4double pnu[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  G4double eng;
  G4double r;

/* 
  Check if energy is within the range of experimental values
*/
  if (erg > 10) eng=10.;
  else eng=erg;

  r=fisslibrng();

/*
  Pu-239 nu distribution
*/
  G4Pow* Pow=G4Pow::GetInstance();
  if (eng <= 5.0) pnu[0] = 0.0108826e0 - 0.00207694e0*eng 
                         - 6.5e-4*Pow->powN(eng,2) + 4.023e-4*Pow->powN(eng,3)
                         - 7.93e-5*Pow->powN(eng,4) + 5.53666667e-6*Pow->powN(eng,5);       
  if (eng > 5 && eng <= 10) pnu[0] = 0.078606e0 - 5.17531e-2*eng 
                                   + 1.42034e-2*Pow->powN(eng,2) - 1.96292e-3*Pow->powN(eng,3)
                                   + 1.34512e-4*Pow->powN(eng,4) - 3.63416e-6*Pow->powN(eng,5);
  if (r <= pnu[0]) return 0;


  if (eng <= 5.0) pnu[1] = 0.0994916e0 - 0.01979542e0*eng 
                         - 0.00236583e0*Pow->powN(eng,2) + 0.0020581e0*Pow->powN(eng,3)
                         - 4.14016667e-4*Pow->powN(eng,4) + 2.85666667e-5*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[1] = 0.10052e0 - 2.61361e-2*eng 
                                   + 3.78355e-3*Pow->powN(eng,2) - 3.70667e-4*Pow->powN(eng,3) 
                                   + 1.95458e-5*Pow->powN(eng,4) - 3.87499e-7*Pow->powN(eng,5);
  cpnu=pnu[0]+pnu[1];
  if (r <= cpnu) return 1;


  if (eng <= 5.0) pnu[2] = 0.2748898e0 - 0.01565248e0*eng 
                         - 0.00749681e0*Pow->powN(eng,2) + 0.00217121e0*Pow->powN(eng,3)
                         - 3.13041667e-4*Pow->powN(eng,4) + 1.88183333e-5*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[2] = 0.282487e0 - 0.0261342e0*eng 
                                   - 1.16895e-3*Pow->powN(eng,2) + 1.9888e-4*Pow->powN(eng,3)
                                   - 6.41257e-6*Pow->powN(eng,4) + 1.02502e-7*Pow->powN(eng,5);
  cpnu=cpnu+pnu[2];
  if (r <= cpnu) return 2;

  if (eng <= 5.0) pnu[3] = 0.3269196e0 + 0.00428312e0*eng 
                         - 0.00189322e0*Pow->powN(eng,2) - 4.31925001e-4*Pow->powN(eng,3)
                         + 1.18466667e-4*Pow->powN(eng,4) - 9.04166668e-6*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[3] = 0.329058e0 + 4.023e-3*eng
                                   - 3.06402e-3*Pow->powN(eng,2) + 2.2628e-4*Pow->powN(eng,3)
                                   - 1.50875e-5*Pow->powN(eng,4) + 4.39168e-7*Pow->powN(eng,5);
  cpnu=cpnu+pnu[3];
  if (r <= cpnu) return 3;

  if (eng <= 5.0) pnu[4] = 0.2046061e0 + 0.02633899e0*eng
                         + 0.0041514e0*Pow->powN(eng,2) - 0.00275542e0*Pow->powN(eng,3)
                         + 5.0325e-4*Pow->powN(eng,4) - 3.32158333e-5*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[4] = 0.18992e0 + 4.55188e-2*eng
                                   - 7.06316e-3*Pow->powN(eng,2) + 7.29916e-4*Pow->powN(eng,3)
                                   - 4.71791e-5*Pow->powN(eng,4) + 1.185e-6*Pow->powN(eng,5);
  cpnu=cpnu+pnu[4];
  if (r <= cpnu) return 4;

  if (eng <= 5.0) pnu[5] = 0.0726834e0 + 0.00116043e0*eng
                         + 0.007572e0*Pow->powN(eng,2) - 0.00161972e0*Pow->powN(eng,3)
                         + 2.3545e-4*Pow->powN(eng,4) - 1.546e-5*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[5] = 0.0779212e0 - 1.35849e-3*eng
                                   + 6.68583e-3*Pow->powN(eng,2) - 7.98649e-4*Pow->powN(eng,3)
                                   + 4.88625e-5*Pow->powN(eng,4) - 1.54167e-6*Pow->powN(eng,5);
  cpnu=cpnu+pnu[5];
  if (r <= cpnu) return 5;

  if (eng <= 5.0) pnu[6] = 0.0097282e0 + 0.00494589e0*eng
                         + 0.00115294e0*Pow->powN(eng,2) - 3.25191667e-4*Pow->powN(eng,3)
                         + 6.00083333e-5*Pow->powN(eng,4) - 3.745e-6*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[6] = 7.85432e-3 + 7.33182e-3*eng
                                   - 2.03705e-4*Pow->powN(eng,2) + 8.73787e-5*Pow->powN(eng,3)
                                   - 4.24164e-6*Pow->powN(eng,4) + 2.37499e-7*Pow->powN(eng,5);
  cpnu=cpnu+pnu[6];
  if (r <= cpnu) return 6;

  if (eng <= 5.0) pnu[7] = 6.301e-4 + 1.10666667e-4*eng
                         + 4.28016667e-4*Pow->powN(eng,2) + 1.12041667e-5*Pow->powN(eng,3)
                         - 4.31666667e-6*Pow->powN(eng,4) + 3.29166667e-7*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[7] = 1.5323e-3 - 7.91857e-4*eng
                                   + 8.01017e-4*Pow->powN(eng,2) - 6.82833e-5*Pow->powN(eng,3)
                                   + 4.38333e-6*Pow->powN(eng,4) - 6.0e-8*Pow->powN(eng,5);
  cpnu=cpnu+pnu[7];
  if (r <= cpnu) return 7;
  else return 8;
}

G4int wuFissionEvent::G4SmpNuDistDataPu239_241(G4double nubar) {

/*
  Description
    Sample Number of Neutrons from fission in Pu-239 and Pu-241 using 
    Zucker and Holden's tabulated data for Pu-239
    The P(nu) distribution is given as a function of the average 
    number of neutrons from fission, based on interpolation of the 
    Pu-239 data from Zucker and Holden.
*/

/*
  Input
    nubar    - average number of neutrons per fission
  Output
    G4SmpNuDistDataPu239_241  - sampled multiplicity
    
*/

  G4double pnu[9], cpnu, sum;
  G4double r;

/* 
  Check if nubar is within the range of experimental values
*/
  if(nubar >= 2.85 && nubar <= 4.25) {
/*
     Use Zucker and Holden Data
*/
     G4Pow* Pow=G4Pow::GetInstance();
     pnu[0]=-2.412937e-3*Pow->powN(nubar,3)+3.210687e-2*Pow->powN(nubar,2)-1.434037e-1*nubar+2.150733e-1;
     pnu[1]=-2.650615e-2*Pow->powN(nubar,3)+3.290389e-1*Pow->powN(nubar,2)-1.389007*nubar+2.002327;
     pnu[2]=3.232028e-2*Pow->powN(nubar,3)-3.176093e-1*Pow->powN(nubar,2)+8.605098e-1*nubar-3.411191e-1;
     pnu[3]=1.623289e-2*Pow->powN(nubar,3)-2.414705e-1*Pow->powN(nubar,2)+1.007282*nubar-9.583769e-1;
     pnu[4]=1.932275e-2*Pow->powN(nubar,3)-2.923666e-1*Pow->powN(nubar,2)+1.421383*nubar-1.924025;
     pnu[5]=-6.185679e-2*Pow->powN(nubar,3)+6.82888e-1*Pow->powN(nubar,2)-2.347653*nubar+2.647049;
     pnu[6]=1.79773e-2*Pow->powN(nubar,3)-1.60516e-1*Pow->powN(nubar,2)+5.228077e-1*nubar-5.939556e-1;
     pnu[7]=3.530038e-3*Pow->powN(nubar,4)-4.925425e-2*Pow->powN(nubar,3)+2.726784e-1*Pow->powN(nubar,2)-6.81281e-1*nubar+6.347577e-1;
     pnu[8]=2.837523e-3*Pow->powN(nubar,3)-2.678644e-2*Pow->powN(nubar,2)+8.545638e-2*nubar-9.156078e-2;

     sum=pnu[0]+pnu[1]+pnu[2]+pnu[3]+pnu[4]+pnu[5]+pnu[6]+pnu[7]+pnu[8];

     pnu[0]=pnu[0]/sum;
     pnu[1]=pnu[1]/sum;
     pnu[2]=pnu[2]/sum;
     pnu[3]=pnu[3]/sum;
     pnu[4]=pnu[4]/sum;
     pnu[5]=pnu[5]/sum;
     pnu[6]=pnu[6]/sum;
     pnu[7]=pnu[7]/sum;
     pnu[8]=pnu[8]/sum;

     r=fisslibrng();

     if(r <= pnu[0]) return 0;

     cpnu=pnu[0]+pnu[1];
     if(r <= cpnu) return 1;

     cpnu=cpnu+pnu[2];
     if(r <= cpnu) return 2;

     cpnu=cpnu+pnu[3];
     if(r <= cpnu) return 3;

     cpnu=cpnu+pnu[4];
     if(r <= cpnu) return 4;

     cpnu=cpnu+pnu[5];
     if(r <= cpnu) return 5;

     cpnu=cpnu+pnu[6];
     if(r <= cpnu) return 6;

     cpnu=cpnu+pnu[7];
     if(r <= cpnu) return 7;
     else return 8;

  } else {
/*
     Use Terrell's formula
*/
     return (G4int) G4SmpTerrell(nubar);
  }
}

G4int wuFissionEvent::G4SmpNuDistDataPu239_241_MC(G4double nubar) {

/*
  Description
    Sample Number of Neutrons from fission in Pu-239 and Pu-241 using 
    Zucker and Holden's tabulated data for Pu-239
    The 11 P(nu) distributions are given as a function of nubar, 
    the average number of neutrons from induced fission for the 
    11 different energies (0 to 10 MeV), based on the Pu-239 data 
    from Zucker and Holden.
*/

/*
  Input
    nubar    - average number of neutrons per fission
  Output
    G4SmpNuDistDataPu239_241_MC  - sampled multiplicity
    
*/

  static G4double Pu239nu [11] [9] = {
     {.0108826, .0994916, .2748898, .3269196, .2046061, .0726834, .0097282, .0006301, .0001685},
     {.0084842, .0790030, .2536175, .3289870, .2328111, .0800161, .0155581, .0011760, .0003469},
     {.0062555, .0611921, .2265608, .3260637, .2588354, .0956070, .0224705, .0025946, .0005205},
     {.0045860, .0477879, .1983002, .3184667, .2792811, .1158950, .0301128, .0048471, .0007233},
     {.0032908, .0374390, .1704196, .3071862, .2948565, .1392594, .0386738, .0078701, .0010046},
     {.0022750, .0291416, .1437645, .2928006, .3063902, .1641647, .0484343, .0116151, .0014149},
     {.0014893, .0222369, .1190439, .2756297, .3144908, .1892897, .0597353, .0160828, .0029917},
     {.0009061, .0163528, .0968110, .2558524, .3194566, .2134888, .0729739, .0213339, .0020017},
     {.0004647, .0113283, .0775201, .2335926, .3213289, .2356614, .0886183, .0274895, .0039531},
     {.0002800, .0071460, .0615577, .2089810, .3200121, .2545846, .1072344, .0347255, .0054786},
     {.0002064, .0038856, .0492548, .1822078, .3154159, .2687282, .1295143, .0432654, .0075217}
    };
  static G4double Pu239nubar [11] = {
      2.8760000,
      3.0088800,
      3.1628300,
      3.3167800,
      3.4707300,
      3.6246800,
      3.7786300,
      3.9325800,
      4.0865300,
      4.2404900,
      4.3944400
    };
  G4double fraction, r, cum;
  G4int engind, nu;

/* 
  Check if nubar is within the range of experimental values
*/
  if(nubar >= Pu239nubar[0] && nubar <= Pu239nubar[10]) {
/*
     Use Zucker and Holden Data
*/
     engind = 1;
     while (nubar > Pu239nubar[engind]){ engind++;}
     // Loop checking, 11.03.2015, T. Koi
     fraction = (nubar-Pu239nubar[engind-1])/(Pu239nubar[engind]-Pu239nubar[engind-1]);
     if(fisslibrng() > fraction) engind--;

     r = fisslibrng();
     nu = 0;
     cum = Pu239nu[engind][0];
     while (r > cum && nu < 8){ 
     // Loop checking, 11.03.2015, T. Koi
       nu++;
       cum += Pu239nu[engind][nu];
     }
     return nu;
  } else {
/*
     Use Terrell's formula
*/
     return (G4int) G4SmpTerrell(nubar);
  }
}

G4int wuFissionEvent::G4SmpNuDistDataU238(G4double erg) {

/*
  Description
    Sample Number of Neutrons from fission in U-238 using 
      Zucker and Holden's tabulated data for U-238
*/

/*
  Input
    erg      - incident neutron energy
  Output
    G4SmpNuDistDataU238  - sampled multiplicity
    
*/
 
  G4double cpnu;
  G4double pnu[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  G4double eng;
  G4double r;

/* 
  Check if energy is within the range of experimental values
*/
  if (erg > 10) eng=10.;
  else eng=erg;

  r=fisslibrng();
/*
  U-238 nu distribution
*/
  G4Pow* Pow=G4Pow::GetInstance();
  if (eng <= 5.0) pnu[0]=0.0396484e0-1.14202e-2*eng+1.94627e-3*Pow->powN(eng,2)-2.95412e-4*Pow->powN(eng,3)+2.98333e-5*Pow->powN(eng,4)-1.31417e-6*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[0]=0.0360013e0-8.06662e-3*eng+6.59461e-4*Pow->powN(eng,2)-3.54123e-5*Pow->powN(eng,3)+2.03749e-6*Pow->powN(eng,4)-5.91663e-8*Pow->powN(eng,5);
  if (r <= pnu[0]) return 0;

  if (eng <= 5.0) pnu[1]=0.252954e0-5.17151e-2*eng+2.84558e-3*Pow->powN(eng,2)+2.93563e-4*Pow->powN(eng,3)-5.99833e-5*Pow->powN(eng,4)+3.34417e-6*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[1]=0.259093e0-5.7936e-2*eng+5.50734e-3*Pow->powN(eng,2)-3.09607e-4*Pow->powN(eng,3)+1.20957e-5*Pow->powN(eng,4)-2.49997e-7*Pow->powN(eng,5);
  cpnu=pnu[0]+pnu[1];
  if (r <= cpnu) return 1;

  pnu[2]=0.29395353e0+0.01098908e0*eng-0.00565976e0*Pow->powN(eng,2)+3.14515399e-4*Pow->powN(eng,3)-5.66793415e-6*Pow->powN(eng,4)+1.54070513e-7*Pow->powN(eng,5);
  cpnu=cpnu+pnu[2];
  if (r <= cpnu) return 2;

  if (eng <= 5.0) pnu[3]=0.264447e0+3.02825e-2*eng-3.12762e-3*Pow->powN(eng,2)-1.5875e-4*Pow->powN(eng,3)+4.91667e-5*Pow->powN(eng,4)-3.38667e-6*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[3]=0.262024e0+3.37134e-2*eng-5.01711e-3*Pow->powN(eng,2)+3.58761e-4*Pow->powN(eng,3)-2.17959e-5*Pow->powN(eng,4)+5.10834e-7*Pow->powN(eng,5);
  cpnu=cpnu+pnu[3];
  if (r <= cpnu) return 3;

  if (eng <= 5) pnu[4]=0.111176e0+1.66321e-2*eng+2.56307e-3*Pow->powN(eng,2)-2.17754e-4*Pow->powN(eng,3)-5.96667e-6*Pow->powN(eng,4)+7.44167e-7*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[4]=0.107859e0+1.88862e-2*eng+2.07521e-3*Pow->powN(eng,2)-2.08099e-4*Pow->powN(eng,3)+3.23745e-6*Pow->powN(eng,4)-1.24999e-7*Pow->powN(eng,5);
  cpnu=cpnu+pnu[4];
  if (r <= cpnu) return 4;

  if (eng <= 5.0) pnu[5]=0.0312261e0+4.12932e-3*eng+9.18413e-4*Pow->powN(eng,2)+4.36542e-5*Pow->powN(eng,3)-5.9125e-6*Pow->powN(eng,4)+3.20833e-7*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[5]=0.0324008e0+3.04772e-3*eng+1.3327e-3*Pow->powN(eng,2)-3.96916e-5*Pow->powN(eng,3)+2.94583e-6*Pow->powN(eng,4)-7.66666e-8*Pow->powN(eng,5);
  cpnu=cpnu+pnu[5];
  if (r <= cpnu) return 5;

  if (eng <= 5.0) pnu[6]=5.9347e-3+9.80023e-4*eng+4.24667e-4*Pow->powN(eng,2)+3.04458e-5*Pow->powN(eng,3)-6.46667e-6*Pow->powN(eng,4)+4.30833e-7*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[6]=6.5421e-3+3.07834e-4*eng+7.39454e-4*Pow->powN(eng,2)-4.70459e-5*Pow->powN(eng,3)+3.44583e-6*Pow->powN(eng,4)-8.91667e-8*Pow->powN(eng,5);
  cpnu=cpnu+pnu[6];
  if (r <= cpnu) return 6;

  if (eng <= 5.0) pnu[7]=5.436e-4+1.3756e-4*eng-5.0e-7*Pow->powN(eng,2)+1.35917e-5*Pow->powN(eng,3)+5.0e-7*Pow->powN(eng,4)-5.16667e-8*Pow->powN(eng,5);
  if (eng > 5 && eng <= 10) pnu[7]=9.212e-4-1.57585e-4*eng+8.41126e-5*Pow->powN(eng,2)+4.14166e-6*Pow->powN(eng,3)+5.37501e-7*Pow->powN(eng,4)-6.66668e-9*Pow->powN(eng,5);
  cpnu=cpnu+pnu[7];
  if (r <= cpnu) return 7;
  else return 8;
}

G4double wuFissionEvent::G4SmpNVel(G4double eng, G4double* cosdiru, G4double* cosdirv, G4double* cosdirw) {

/*
  Description
    Knowing the neutron energy, determine its velocity and the 3 direction cosines
*/

/*
  Input
    eng     - energy of neutron
  Output
            - velocity
            - 3 components of the neutron velocity
              in arguments (cosdiru, cosdirv, cosdirw)
*/

   const G4double rme = 939.56563; /* Rest Mass Energy equivalents for neutrons (MeV) */
   const G4double cspeed = 2.99792458e+8; /* speed of light (m/sec) */

   G4double vel;
   G4double ratio = eng/rme;

   if (ratio > 1.e-8) {
/*    Relativistic vel calculation */
      vel = cspeed*std::sqrt(1.-rme*rme/((rme+eng)*(rme+eng)));
   } else {
/*    Non-relativistic vel calculation */
      vel = cspeed*std::sqrt(2.*eng/rme);
   }

   G4SmpIsoDir(cosdiru, cosdirv, cosdirw);

   return vel;
}

G4double wuFissionEvent::G4SmpNEngCf252(G4int option) {

/*
  Description
    Sample energy spectrum for Cf-252. 
*/

/*
  Input
    option    - 0 Mannhart corrected Maxwellian spectrum
                1 Madland-Nix theoretical spectrum
                2 Froehner Watt spectrum
  Return
    energy of neutron emitted by spontaneous fission
*/

  G4Pow* Pow=G4Pow::GetInstance();

  G4double a,b;
  G4double gpar;
  G4double g2;
  G4double ferg;
  G4double r;

  r = fisslibrng();

/*
   Mannhart Corrected Spectrum
*/
  if(option == 0) {
    if(r == 0) return 0.000001;
    if (r > 0.0 && r <= 0.0005)
      return 0.00003 + 0.04992*(r/0.0005) - 0.59473*Pow->powN(r/0.0005,2) 
           + 5.44877*Pow->powN(r/0.0005,3) - 29.38086*Pow->powN(r/0.0005,4)
           + 97.14014*Pow->powN(r/0.0005,5) - 202.82112*Pow->powN(r/0.0005,6)
           + 268.2301*Pow->powN(r/0.0005,7) - 217.75316*Pow->powN(r/0.0005,8)
           + 98.96285*Pow->powN(r/0.0005,9) - 19.27077*Pow->powN(r/0.0005,10);

    if (r > 0.0005 && r <= 0.005)
      return 0.01118 + 0.06715*((r-.0005)/.0045)
           - 0.09236*Pow->powN((r-.0005)/.0045,2) + 0.26224*Pow->powN((r-.0005)/.0045,3)
           - 0.64784*Pow->powN((r-.0005)/.0045,4) + 1.16830*Pow->powN((r-.0005)/.0045,5)
           - 1.43858*Pow->powN((r-.0005)/.0045,6) + 1.13771*Pow->powN((r-.0005)/.0045,7)
           - 0.51839*Pow->powN((r-.0005)/.0045,8) + 0.10302*Pow->powN((r-.0005)/.0045,9);

    if (r > 0.005 && r <= 0.05)
      return 0.05244+0.32101*((r-.005)/.045)
           -  0.52574*Pow->powN((r-.005)/.045,2) +   2.80540*Pow->powN((r-.005)/.045,3)
           - 14.88036*Pow->powN((r-.005)/.045,4) +  55.46869*Pow->powN((r-.005)/.045,5)
           -133.64517*Pow->powN((r-.005)/.045,6) + 202.88434*Pow->powN((r-.005)/.045,7)
           -186.86758*Pow->powN((r-.005)/.045,8) +  95.19530*Pow->powN((r-.005)/.045,9)
           - 20.55275*Pow->powN((r-.005)/.045,10);

    if(r > 0.05 && r <= 0.25) return 0.25585+0.75532*((r-.05)/.2)-0.73676*Pow->powN((r-.05)/.2,2)+3.65653*Pow->powN((r-.05)/.2,3)-13.80528*Pow->powN((r-.05)/.2,4)+33.35932*Pow->powN((r-.05)/.2,5)-50.0410*Pow->powN((r-.05)/.2,6)+45.13793*Pow->powN((r-.05)/.2,7)-22.4072*Pow->powN((r-.05)/.2,8)+4.70141*Pow->powN((r-.05)/.2,9);

    if(r > 0.25 && r <= 0.50) return 0.87609+0.74687*((r-.25)/.25)+0.02849*Pow->powN((r-.25)/.25,2)+0.06145*Pow->powN((r-.25)/.25,3)-0.09589*Pow->powN((r-.25)/.25,4)+0.29798*Pow->powN((r-.25)/.25,5)-0.57707*Pow->powN((r-.25)/.25,6)+0.66181*Pow->powN((r-.25)/.25,7)-0.40720*Pow->powN((r-.25)/.25,8)+0.10370*Pow->powN((r-.25)/.25,9);

    if(r > 0.5 && r <= 0.75) return 1.69622+0.93896*((r-.5)/.25)+0.16428*Pow->powN((r-.5)/.25,2)+0.21761*Pow->powN((r-.5)/.25,3)-0.96904*Pow->powN((r-.5)/.25,4)+3.34951*Pow->powN((r-.5)/.25,5)-6.35177*Pow->powN((r-.5)/.25,6)+6.90120*Pow->powN((r-.5)/.25,7)-3.98682*Pow->powN((r-.5)/.25,8)+0.95276*Pow->powN((r-.5)/.25,9);

    if(r > 0.75 && r <= 0.95) return 2.91217+1.52474*((r-.75)/.2)-4.99340*Pow->powN((r-.75)/.2,2)+58.72977*Pow->powN((r-.75)/.2,3)-313.30984*Pow->powN((r-.75)/.2,4)+946.0791*Pow->powN((r-.75)/.2,5)-1679.85559*Pow->powN((r-.75)/.2,6)+1740.83984*Pow->powN((r-.75)/.2,7)-973.51886*Pow->powN((r-.75)/.2,8)+227.06831*Pow->powN((r-.75)/.2,9);
     if(r > 0.95 && r <= 0.975) return 5.50137-0.99765*((r-.95)/.025)+27.57678*Pow->powN((r-.95)/.025,2)-218.47931*Pow->powN((r-.95)/.025,3)+1024.0426*Pow->powN((r-.95)/.025,4)-3005.86182*Pow->powN((r-.95)/.025,5)+5684.52295*Pow->powN((r-.95)/.025,6)-6919.36182*Pow->powN((r-.95)/.025,7)+5235.71777*Pow->powN((r-.95)/.025,8)-2240.06934*Pow->powN((r-.95)/.025,9)+413.9299*Pow->powN((r-.95)/.025,10);

     if(r > 0.975 && r <= 0.995) return 6.52172+1.21273*((r-.975)/.02)+0.69998*Pow->powN((r-.975)/.02,2)-1.78886*Pow->powN((r-.975)/.02,3)+11.57883*Pow->powN((r-.975)/.02,4)-39.41592*Pow->powN((r-.975)/.02,5)+88.32992*Pow->powN((r-.975)/.02,6)-127.68685*Pow->powN((r-.975)/.02,7)+115.97678*Pow->powN((r-.975)/.02,8)-60.09069*Pow->powN((r-.975)/.02,9)+13.66798*Pow->powN((r-.975)/.02,10);
     if(r > 0.995 && r <= 0.999) return 9.00502+1.31798*((r-.995)/.004)-1.17448*Pow->powN((r-.995)/.004,2)+20.15941*Pow->powN((r-.995)/.004,3)-114.27763*Pow->powN((r-.995)/.004,4)+370.04855*Pow->powN((r-.995)/.004,5)-701.888*Pow->powN((r-.995)/.004,6)+776.28204*Pow->powN((r-.995)/.004,7)-462.68823*Pow->powN((r-.995)/.004,8)+115.05296*Pow->powN((r-.995)/.004,9);
     if(r > 0.999 && r <= 0.9997) return 11.83792-1.8952*((r-.999)/.0007)+50.30901*Pow->powN((r-.999)/.0007,2)-239.56978*Pow->powN((r-.999)/.0007,3)+514.90747*Pow->powN((r-.999)/.0007,4)-508.73672*Pow->powN((r-.999)/.0007,5)+191.09637*Pow->powN((r-.999)/.0007,6);
     if(r > 0.9997) return 20.;
  }
/*
   Madland-Nix Spectrum
*/
  if(option == 1) {
     if(r <= 1.001065092e-03) return 1.946313876*Pow->powA(r,0.6667261950);
     else if(r > 1.001065092e-03 && r <= 1.001389105e-02) return 2.00504119*Pow->powA(r,0.6709990736);
     else if(r > 1.001389105e-02 && r <= 5.022359145e-02) return 2.107978578*Pow->powA(r,0.7077041191);
     else if(r > 5.022359145e-02 && r <= 1.000989427e-01) return 2.280517358*Pow->powA(r,0.7077041191);
     else if(r > 1.000989427e-01 && r <= 1.500872491e-01) return 2.444108408*Pow->powA(r,0.73764526215);
     else if(r > 1.500872491e-01 && r <= 2.002079974e-01) return 2.621855634*Pow->powA(r,0.7745779546);
     else if(r > 2.002079974e-01 && r <= 2.25221648e-01) return 2.753099265*Pow->powA(r,0.8044994010);
     else if(r > 2.25221648e-01 && r <= 2.501564538e-01) return 2.834010751*Pow->powA(r,0.8239187384);
     else if(r > 2.501564538e-01 && r <= 2.752546770e-01) return 2.911676280*Pow->powA(r,0.8434235719);
     else if(r > 2.752546770e-01 && r <= 3.000964724e-01) return 2.988430135*Pow->powA(r,0.8635883266);
     else if(r > 3.000964724e-01 && r <= 3.500470095e-01) return 3.099471293*Pow->powA(r,0.8942289512);
     else if(r > 3.500470095e-01 && r <= 4.001118970e-01) return 3.244686176*Pow->powA(r,0.9378302608);
     else if(r > 4.001118970e-01 && r <= 5.000461778e-01) return 3.543403932*Pow->powA(r,1.0411008510);
     else if(r > 5.000461778e-01 && r <= 5.501318506e-01) return 3.708358099*Pow->powA(r,1.1068317830);
     else if(r > 5.501318506e-01 && r <= 6.000655433e-01) return 3.889805304*Pow->powA(r,1.1868908770);
     else if(r > 6.000655433e-01 && r <= 6.500147305e-01) return 4.092497225*Pow->powA(r,1.2865658570);
     else if(r > 6.500147305e-01 && r <= 7.000271284e-01) return 4.322906068*Pow->powA(r,1.4140909190);
     else if(r > 7.000271284e-01 && r <= 7.501159110e-01) return 4.589909069*Pow->powA(r,1.5828217210);
     else if(r > 7.501159110e-01 && r <= 8.000662513e-01) return 4.906598744*Pow->powA(r,1.8162034790);
     else if(r > 8.000662513e-01 && r <= 8.500772033e-01) return 5.297053797*Pow->powA(r,2.1626825870);
     else if(r > 8.500772033e-01 && r <= 8.750123088e-01) return 5.650277904*Pow->powA(r,2.5517142900);
     else if(r > 8.750123088e-01 && r <= 9.000106866e-01) return 5.947741976*Pow->powA(r,2.9383159800);
     else if(r > 9.000106866e-01 && r <= 9.250286977e-01) return 6.317014169*Pow->powA(r,3.5155713570);
     else if(r > 9.250286977e-01 && r <= 9.350074655e-01) return 6.625757778*Pow->powA(r,4.1118364020);
     else if(r > 9.350074655e-01 && r <= 9.400070002e-01) return 6.784126941*Pow->powA(r,4.4594479870);
     else if(r > 9.400070002e-01 && r <= 9.500026229e-01) return 6.969180156*Pow->powA(r,4.9019105900);
     else if(r > 9.500026229e-01 && r <= 9.600065896e-01) return 7.254643542*Pow->powA(r,5.6894827520);
     else if(r > 9.600065896e-01 && r <= 9.700165577e-01) return 7.613500497*Pow->powA(r,6.8841593900);
     else if(r > 9.700165577e-01 && r <= 9.750157135e-01) return 7.944100103*Pow->powA(r,8.2544400860);
     else if(r > 9.750157135e-01 && r <= 9.800101585e-01) return 8.228439642*Pow->powA(r,9.6531190300);
     else if(r > 9.800101585e-01 && r <= 9.850018119e-01) return 8.586524083*Pow->powA(r,11.783756400);
     else if(r > 9.850018119e-01 && r <= 9.875072929e-01) return 8.917364901*Pow->powA(r,14.240137310);
     else if(r > 9.875072929e-01 && r <= 9.900006975e-01) return 9.202675761*Pow->powA(r,16.76089029);
     else if(r > 9.900006975e-01 && r <= 9.925048152e-01) return 9.562781386*Pow->powA(r,20.61962568);
     else if(r > 9.925048152e-01 && r <= 9.935030103e-01) return 9.867915664*Pow->powA(r,24.69147261);
     else if(r > 9.935030103e-01 && r <= 9.945000177e-01) return 10.08727342*Pow->powA(r,28.07701487);
     else if(r > 9.945000177e-01 && r <= 9.950025127e-01) return 10.27382614*Pow->powA(r,31.36001051);
     else if(r > 9.950025127e-01 && r <= 9.955029368e-01) return 10.41724243*Pow->powA(r,34.13127669);
     else if(r > 9.955029368e-01 && r <= 9.960005970e-01) return 10.57636221*Pow->powA(r,37.50088614);
     else if(r > 9.960005970e-01 && r <= 9.965016080e-01) return 10.75639015*Pow->powA(r,41.72354164);
     else if(r > 9.965016080e-01 && r <= 9.970001795e-01) return 10.96366661*Pow->powA(r,47.18729543);
     else if(r > 9.970001795e-01 && r <= 9.975004375e-01) return 11.20771170*Pow->powA(r,54.54899604);
     else if(r > 9.975004375e-01 && r <= 9.978504408e-01) return 11.45202216*Pow->powA(r,63.11906699);
     else if(r > 9.978504408e-01 && r <= 9.989524675e-01)
       return 2.72756636666e5-5.47258138432e5*r+2.74514044871e5*Pow->powN(r,2);
     else if(r > 9.989524675e-01 && r <= 9.994929298e-01)
       return 1.14946879661e6-2.30252188973e6*r+1.15306661788e6*Pow->powN(r,2);
     else if(r > 9.994929298e-01 && r <= 9.997558922e-01)
       return 4.90621526236e6-9.81982943883e6*r+4.91362868673e6*Pow->powN(r,2);
     else if(r > 9.997558922e-01 && r <= 9.998830120e-01)
       return 2.11365688795184e7-4.22884732250404e7*r+2.11519198434219e7*Pow->powN(r,2);
     else if(r > 9.998830120e-01 && r <= 9.999441620e-01)
       return 9.18987945911229e7-1.83829506875257e8*r+9.19307287711182e7*Pow->powN(r,2);
     else if(r > 9.999441620e-01 && r <= 9.999734440e-01)
       return 4.02781481130433e8-8.05629656768407e8*r+4.02848193115356e8*Pow->powN(r,2);
     else if(r > 9.999734440e-01 && r <= 9.999874120e-01)
       return 1.77804635135775e9-3.55623257045546e9*r+1.77818623756641e9*Pow->powN(r,2);
     else if(r > 9.999874120e-01 && r <= 9.999940510e-01)
       return 7.90099032702915e9-1.58022749659903e10*r+7.90128465842187e9*Pow->powN(r,2);
     else if(r > 9.999940510e-01 && r <= 9.999971960e-01)
       return 3.53223507413091e10-7.06453227162775e10*r+3.53229719954219e10*Pow->powN(r,2);
     else if(r > 9.999971960e-01 && r <= 9.999986820e-01)
       return 1.58786475903785e11-3.17574266841213e11*r+1.58787790958875e11*Pow->powN(r,2);
     else if(r > 9.999986820e-01 && r <= 9.999993820e-01)
       return 7.17433904438156e11-1.43487059972047e12*r+7.17436695304750e11*Pow->powN(r,2);
     else if(r > 9.999993820e-01 && r <= 9.999997110e-01)
       return 3.257374123945330e12-6.514754184993900e12*r+3.257380061072000e12*Pow->powN(r,2);
     else if(r > 9.999997110e-01 && r <= 9.999998650e-01)
       return 1.48641255466171e13-2.97282637539286e13*r+1.48641382073360e13*Pow->powN(r,2);
     else if(r > 9.999998650e-01 && r <= 9.999999370e-01)
       return 6.82056055248876e13-1.36411238119518e14*r+6.82056325946560e13*Pow->powN(r,2);
     else if(r > 9.999999370e-01 && r <= 1.000000000e00)
       return 3.14919363013517e14-6.29838784079090e14*r+3.14919421065600e14*Pow->powN(r,2);
  }
/*
   Frohner Watt Spectrum
*/
  if (option == 2) {
    a=1.175;
    b=1.040;

    G4int icounter=0;
    G4int icounter_max=1024;
    do {
      gpar = std::sqrt(Pow->powA(1+0.125*a*b,2.)-1)+(1+0.125*a*b);
      g2=-G4Log(fisslibrng());
      ferg=a*gpar*g2;
      
      icounter++;
      if ( icounter > icounter_max ) { 
	 G4cout << "Loop-counter exceeded the threshold value at " << __LINE__ << "th line of " << __FILE__ << "." << G4endl;
         break;
      }

    } while (Pow->powA((1-gpar)*(1+g2)-G4Log(fisslibrng()),2.) > b*ferg);
    // Loop checking, 11.03.2015, T. Koi
    return ferg;
  }

  //
  // Fall through
  //
   
  G4cout << " SmpNEngCf252: unrecognized option = " << option << G4endl;
  return -1.0;
}

void wuFissionEvent::G4SmpIsoDir(G4double* cosdiru, G4double* cosdirv, G4double* cosdirw) {

/*
  Description
    determine the 3 direction cosines of a particle emitted isotropically
*/

/*
  Input
  Output
            - 3 components of the direction cosines
              (cosdiru, cosdirv, cosdirw)
*/

   G4double cospolang, sinpolang, phi;

/*
   Choose emission angle isotropically.
   Select a polar angle direction cosine.
*/
   G4Pow* Pow=G4Pow::GetInstance();
   cospolang = 1.-2.*fisslibrng();
   // sinpolang = std::sqrt(1.-Pow->powA(cospolang, 2.));
   sinpolang = std::sqrt(1.0-cospolang*cospolang);//wuhongyi
/*
   Select an azimuthal angle uniformly on (0,2*pi)
*/
   phi = twopi*fisslibrng();
   *cosdiru = sinpolang * std::cos(phi);
   *cosdirv = sinpolang * std::sin(phi);
   *cosdirw = cospolang;
}

G4double wuFissionEvent::G4SmpGEng() {

/*
  Description
    Sample energy spectrum for photons emitted by fission-induced reactions. 
    The energy spectrum of the prompt fission gamma rays is obtained from
    Maienschein's measurements.
*/

/*
  Input
  Return
    energy of photon emitted by neutron-induced fission
*/

   G4double r;
/*
  Calculate the energy of photons emitted from fission
*/
   G4Pow* Pow=G4Pow::GetInstance();
   r=fisslibrng();

   if (r == 0.0) return 0.085;

   if (r <= 0.0001) return 0.0855+0.01692*(r/0.0001)-0.02401*Pow->powA(r/0.0001,2.)+0.01274*Pow->powA(r/0.0001,3.);

   if (r > 0.0001 && r <= 0.01) return 0.09141 +  0.23846*((r-0.0001)/0.0099) 
				               -  1.75947*Pow->powA((r-.0001)/0.0099,2.)
                                               + 10.98611*Pow->powA((r-0.0001)/0.0099,3.)
                                               - 43.19181*Pow->powA((r-.0001)/0.0099,4.)
                                               +105.70005*Pow->powA((r-.0001)/.0099,5.)
                                               -160.72894*Pow->powA((r-.0001)/.0099,6.)
                                               +147.43399*Pow->powA((r-.0001)/.0099,7.)
                                               - 74.60043*Pow->powA((r-.0001)/0.0099,8.)
                                               + 15.97547*Pow->powA((r-.0001)/0.0099,9.);

   if (r > 0.01 && r <= 0.1537) return 0.14486 + 0.40914*((r-.01)/.1437)
				               - 1.28150*Pow->powA((r-0.01)/0.1437,2.)
                                               + 5.07377*Pow->powA((r-0.01)/0.1437,3.)
                                               -15.42031*Pow->powA((r-0.01)/0.1437,4.)
                                               +31.96346*Pow->powA((r-0.01)/0.1437,5.)
                                               -43.12605*Pow->powA((r-0.01)/0.1437,6.)
                                               +36.02908*Pow->powA((r-0.01)/0.1437,7.)
                                               -16.87185*Pow->powA((r-0.01)/0.1437,8.)
                                               + 3.37941*Pow->powA((r-0.01)/0.1437,9.);

   if (r > 0.1537 && r <= 0.7114) return (-1./2.3)*G4Log(0.71956*(0.1537-r)+0.50158);

   if (r > 0.7114 && r <= 1.0) return (-1./1.1)*G4Log(1.15292*(0.7114-r)+0.33287);
   //
   // Fall through 
   //
   G4cout << " Random number out of range in SmpGEng " << G4endl;
   return -1.0;
}

G4int wuFissionEvent::G4SmpNugDist(G4int isotope, G4double nubar) {

/*
  Description
    Sample Number of Photons from neutron induced fission in 
    all isotopes using Tim Valentine's model (negative binomial
    distribution, using nubar as a model parameter)
*/

/*
  Input
    iso          - isotope
  Output
    G4SmpNugDist - sampled multiplicity
*/
 
  static G4double logcoeff[nfissg+1] = {
     0.00000000000000e+00,
     3.25809653802149e+00,
     5.86078622346587e+00,
     8.09437844497297e+00,
     1.00753799138395e+01,
     1.18671393830676e+01,
     1.35093671183247e+01,
     1.50291928720691e+01,
     1.64462588918558e+01,
     1.77753948391357e+01,
     1.90281578076311e+01,
     2.02137814732888e+01,
     2.13397927361450e+01,
     2.24124295384099e+01,
     2.34369338549243e+01,
     2.44177631079360e+01,
     2.53587464524005e+01,
     2.62632027266277e+01,
     2.71340310844251e+01,
     2.79737817391769e+01,
     2.87847119553932e+01,
     2.95688309141589e+01,
     3.03279360625106e+01,
     3.10636428574894e+01,
     3.17774093252521e+01,
     3.24705565058120e+01,
     3.31442856005149e+01,
     3.37996924530920e+01,
     3.44377798564689e+01,
     3.50594680730467e+01,
     3.56656038766170e+01,
     3.62569683628670e+01,
     3.68342837279018e+01,
     3.73982191769817e+01,
     3.79493960962713e+01,
     3.84883925970040e+01,
     3.90157475227212e+01,
     3.95319639951220e+01,
     4.00375125617872e+01,
     4.05328339990172e+01,
     4.10183418147990e+01
  };
  G4int i, A, Z;
  G4double cpi[nfissg+1];
  G4double p, q, nubarg;
  G4double r;

/* 
  No data is available for induced fission gamma number
  distributions. Sample the negative binomial cumulative 
  probability distribution.
*/
  A = (G4int) (isotope-1000*((G4int)(isotope/1000)));
  Z = (G4int) ((isotope-A)/1000);
  G4Pow* Pow = G4Pow::GetInstance();
  nubarg = ((2.51-1.13e-5*Pow->powA(G4double(Z),2.)*std::sqrt(G4double(A)))*nubar+4.0)
           /(-1.33+119.6*Pow->A13(G4double(Z))/G4double(A));
  p = 1.*alphanegbin/(alphanegbin+nubarg);
  q = 1.-p;
  cpi[0] = G4Exp(logcoeff[0]+26.*G4Log(p));
  for (i=1; i<=nfissg; i++) cpi[i] = cpi[i-1] + G4Exp(logcoeff[i]+26.*G4Log(p)+i*G4Log(q));
  for (i=0; i<=nfissg; i++) cpi[i] = cpi[i]/cpi[nfissg-1];

  r=fisslibrng();

  for(i=0; i<=nfissg; i++) if (r <= cpi[i]) return i;

  //
  // Fall through
  //

  G4cout << " SmpNugDist: random number " << r << " out of range " << G4endl;
  return -1;

}


G4double wuFissionEvent::G4SmpPVel(G4double /*eng*/, G4double* cosdiru, G4double* cosdirv, G4double* cosdirw) {

/*
  Description
    Knowing the photon energy, determine its velocity and the 3 direction cosines
*/

/*
  Input
    eng     - energy of photon
  Output
            - velocity
            - 3 components of the photon velocity
              as arugments (cosdiru, cosdirv, cosdirw)
*/

   const G4double cspeed = 2.99792458e+8; /* speed of light (m/sec) */

   G4SmpIsoDir(cosdiru, cosdirv, cosdirw);

   return cspeed;
}

G4int wuFissionEvent::G4SmpSpNuDistData(G4int isotope, G4int Cf252option) {

/*
  Description
    Sample Number of Neutrons from spontaneous fission 
    (a) from the neutron multiplicity data for 
        U-238, Pu-238, Pu-240, Pu-242, Cm-242, Cm-244
           using Holden and Zucker's tabulated data
        Cf-252 using either Spencer's tabulated data or 
           Boldeman's data
    (b) from Terrell's approximation using nubar for
        Th-232, 
        U-232, U-233, U-234, U-235, U-236,
        Np-237, 
        Pu-239, Pu-241, 
        Am-241, 
        Bk-249
           using Ensslin's data.
*/

/*
  Input
    iso          - isotope
    Cf252option  - 0 to use Spencer's tabulated data
                   1 to use Boldeman's data
  Output
    G4SmpSpNuDistData - sampled multiplicity
                      -1 is the isotope has 
                         no multiplicity data,
                         nor any nubar data
*/
 
  G4int i, index;
  G4double sum, nubar;
  G4double r;

  static G4double sfnu [nSPfissIso][nSPfissn] = { 
    {0.0481677,0.2485215,0.4253044,0.2284094,0.0423438,0.0072533,
     0.0000000,0.0000000,0.0000000,0.0000000,0.0000000},

    {0.0631852,0.2319644,0.3333230,0.2528207,0.0986461,0.0180199,
     0.0020407,0.0000000,0.0000000,0.0000000,0.0000000},

    {0.0679423,0.2293159,0.3341228,0.2475507,0.0996922,0.0182398,
     0.0031364,0.0000000,0.0000000,0.0000000,0.0000000},

    {0.0212550,0.1467407,0.3267531,0.3268277,0.1375090,0.0373815,
     0.0025912,0.0007551,0.0001867,0.0000000,0.0000000},

    {0.0150050,0.1161725,0.2998427,0.3331614,0.1837748,0.0429780,
     0.0087914,0.0002744,0.0000000,0.0000000,0.0000000},

    {0.0540647,0.2053880,0.3802279,0.2248483,0.1078646,0.0276366,
     0.0000000,0.0000000,0.0000000,0.0000000,0.0000000},

    {0.0021100,0.0246700,0.1229000,0.2714400,0.3076300,0.1877000,
     0.0677000,0.0140600,0.0016700,0.0001000,0.0000000},

    {0.0020900,0.0262100,0.1262000,0.2752000,0.3018000,0.1846000,
     0.0668000,0.0150000,0.0021000,0.0000000,0.0000000} };

/*
  sample the spontaneous fission neutron number distribution
*/
  index = -1;

  if (isotope == 92238) index = 0;
  else if (isotope == 94240) index = 1;
  else if (isotope == 94242) index = 2;
  else if (isotope == 96242) index = 3;
  else if (isotope == 96244) index = 4;
  else if (isotope == 94238) index = 5;
  else if (isotope == 98252 && Cf252option == 0) index = 6;
  else if (isotope == 98252 && Cf252option == 1) index = 7;

  if (index != -1) { 
    r=fisslibrng();

    sum = 0.;
    for (i = 0; i < nSPfissn-1; i++) {
      sum = sum + sfnu[index][i];
      if (r <= sum || sfnu[index][i+1] == 0.) return i;
    }
    //
    // Fall through
    //
    G4cout << " Random number out of range in SmpSpNuDistData " << G4endl;
    return -1;

  } else {
// There is no full multiplicity distribution data available
// for that isotope, let's try to find a nubar for it in
// N. Ensslin, et.al., "Application Guide to Neutron
// Multiplicity Counting," LA-13422-M (November 1998)
// and use Terrell's approximation
    nubar = G4SmpSpNubarData(isotope);
    if (nubar != -1.) {
      return (G4int) G4SmpTerrell(nubar);
    } else {
// There is no nubar information for that isotope, return -1,
// meaning no data available for that isotope
      return -1;
    }
  }
}

G4double wuFissionEvent::G4SmpSpNubarData(G4int isotope) {

/*
  Description
    Determine average number of neutrons from spontaneous fission for
        Th-232, 
        U-232, U-233, U-234, U-235, U-236, U-238
        Np-237, 
        Pu-239, Pu-240, Pu-241,  Pu-242
        Am-241, 
        Cm-242, Cm-244, 
        Bk-249,
        Cf-252
    Based on Ensslin's data.
    N. Ensslin, et.al., "Application Guide to Neutron Multiplicity Counting," 
    LA-13422-M (November 1998)
*/

/*
  Input
    iso          - isotope
  Output
    G4SmpSpNubarData - average number of neutrons
                     -1. is the isotope has 
                         no nubar data
*/
 
  G4int i;

  static G4int spzaid [nSPfissNubarIso] = {
      90232, 92232, 92233, 92234, 92235,
      92236, 92238, 93237, 94238, 94239,
      94240, 94241, 94242, 95241, 96242,
      96244, 97249, 98252 };
  static G4double spnubar [nSPfissNubarIso] = {
      2.14,  1.71, 1.76,  1.81, 1.86,
      1.91,  2.01, 2.05,  2.21, 2.16,
      2.156, 2.25, 2.145, 3.22, 2.54,
      2.72,  3.40, 3.757
      };

// Find nubar
  for (i=0; i<nSPfissNubarIso; i++) {
    if (isotope == spzaid[i]) {
      return spnubar[i];
    }
  }
// no nubar available for that isotope
  return -1.;
}

G4int wuFissionEvent::G4SmpSpNugDistData(G4int isotope) {

/*
  Description
    Sample Number of Photons from spontaneous fission in 
    (a) Cf-252 using the double Poisson model from Brunson;
    (b) Th-232, 
        U-232, U-233, U-234, U-235, U-236, U-238*,
        Np-237, 
        Pu-238*, Pu-239, Pu-240*, Pu-241, Pu-242*,
        Am-241, 
        Cm-242*, Cm-244*, 
        Bk-249
        using the negative binomial distribution based on the
        spontaneous fission neutron nubar from Ensslin's 
        tabulated data or Holden and Zucker's tabulated data 
        (for isotopes denoted with asterix *).
*/

/*
  Input
    iso          - isotope
  Output
    G4SmpSpNugDistData - sampled multiplicity
                       -1 if there is no multiplicity data for that isotope
*/
 
  G4int i;
  G4double sum, nubar;
  G4double r;

  static G4double Cf252spdist [nSPfissg] = { 
         5.162699e-4,3.742057e-3,1.360482e-2,3.312786e-2,6.090540e-2,
         9.043537e-2,1.133984e-1,1.240985e-1,1.216759e-1,1.092255e-1,
         9.137106e-2,7.219960e-2,5.438060e-2,3.923091e-2,2.714690e-2,
         1.800781e-2,1.143520e-2,6.942099e-3,4.025720e-3,2.229510e-3,
         1.179602e-3,5.966936e-4,2.888766e-4,1.340137e-4,5.965291e-5,
         2.551191e-5,1.049692e-5,4.160575e-6,1.590596e-6,0.000000e+0
      };

/*
  sample the spontaneous fission photon number distribution 
*/
  nubar=0.;
  if (isotope == 98252) {
//  Cf-252 using the G4double Poisson model from Brunson;
    r=fisslibrng();

    sum = 0.;
    for (i = 0; i < nSPfissg-1; i++) {
      sum = sum + Cf252spdist[i];
      if (r <= sum || Cf252spdist[i+1] == 0.) return i;
    }
  } else if (isotope == 92238) {
/*
    using the spontaneous fission nubar from
    Holden and Zucker's tabulated data 
*/
    nubar = 1.9900002;
  } else if (isotope == 94240) {
    nubar = 2.1540006;
  } else if (isotope == 94242) {
    nubar = 2.1489998;
  } else if (isotope == 96242) {
    nubar = 2.54;
  } else if (isotope == 96244) {
    nubar = 2.7200005;
  } else if (isotope == 94238) {
    nubar = 2.2100301;
  }

  if (nubar != 0.) {
    return G4SmpNugDist(isotope, nubar);
  } else {
/*
    using the spontaneous fission nubar from
    N. Ensslin, et.al., "Application Guide to Neutron
    Multiplicity Counting," LA-13422-M (November 1998)
*/
    nubar = G4SmpSpNubarData(isotope);
    if (nubar != -1.) {
      return G4SmpNugDist(isotope, nubar);
    } else {
// There is no nubar information for that isotope, return -1,
// meaning no data available for that isotope
      return -1;
    }
  }
}

G4double wuFissionEvent::G4SmpTerrell(G4double nubar) {
/*
  Description
    Sample Fission Number from Terrell's modified Gaussian distribution

    method uses Red Cullen's algoritm UCRL-TR-222526
*/

/*
  Input
    nubar    - average number of neutrons per fission
  Output
    G4SmpTerrell  - sampled multiplicity
    
*/

  G4double width;
  G4double temp1, temp2, expo, cshift;
  G4double rw, theta, sampleg;


  if (nubar < WIDTH) {
    std::ostringstream o;
    o << nubar;
    std::string errMsg = "fission nubar out of range, nubar=" + o.str();
    G4fissionerr(6, "SmpTerrell", errMsg);
  }

  width = SQRT2 * WIDTH;
  temp1 = nubar + 0.5;
  temp2 = temp1/width;
  temp2 *= temp2;
  expo = G4Exp(-temp2);
  cshift = temp1 + BSHIFT * WIDTH * expo/(1. - expo);

  G4int icounter = 0;
  G4int icounter_max = 1024;
  do {
    rw = std::sqrt(-G4Log(fisslibrng()));
    theta = TWOPI * fisslibrng();
    sampleg = width * rw * std::cos(theta) + cshift;
    icounter++;
    if ( icounter > icounter_max ) { 
      G4cout << "Loop-counter exceeded the threshold value at " << __LINE__ << "th line of " << __FILE__ << "." << G4endl;
      break;
    }
  } while (sampleg < 0.0);
  // Loop checking, 11.03.2015, T. Koi

  return std::floor(sampleg);
}

G4double wuFissionEvent::G4SmpWatt(G4double ePart, G4int iso) {

/*
  Description
    Sample Watt Spectrum as in TART (Kalos algorithm)
*/

/*
  Input
    ePart     - energy of incoming particle
    iso       - isotope
  Output
              - energy of incoming particle
*/

  static G4int nZA [nZAfis]= {
                 90231, 90232, 90233,
                 91233,
                 92232, 92233, 92234, 92235, 92236, 92237, 92238, 92239, 92240,
                 93235, 93236, 93237, 93238,
                 94237, 94238, 94239, 94240, 94241, 94242, 94243,
                 95241, 95242, 95243,
                 96242, 96243, 96244, 96245, 96246, 96247, 96248,
                 97249,
                 98249, 98250, 98251, 98252};

  static G4double Watta [nZAfis][3] = {
                      {6.00949285e-05, -8.36695381e-03,  9.50939496e-01},
                      {6.54348443e-05, -8.86574327e-03,  9.55404490e-01},
                      {7.08173682e-05, -9.22676286e-03,  9.50088329e-01},
                      {6.35839062e-05, -8.63645973e-03,  9.24583535e-01},
                      {8.21929628e-05,  4.01922936e-03,  1.152121164e00},
                      {6.21335718e-05, -8.45651858e-03,  9.14717276e-01},
                      {6.81386135e-05, -8.99142394e-03,  9.21954824e-01},
                      {7.32627297e-05, -9.36908697e-03,  9.20107976e-01},
                      {8.06505279e-05, -9.95416671e-03,  9.27890410e-01},
                      {8.33208285e-05, -1.01073057e-02,  9.17691654e-01},
                      {8.96944680e-05, -1.06491070e-02,  9.25496030e-01},
                      {9.44608097e-05, -1.08940419e-02,  9.17795511e-01},
                      {1.01395704e-04, -1.15098159e-02,  9.29395462e-01},
                      {6.81110009e-05, -8.91619352e-03,  9.00047566e-01},
                      {7.21126359e-05, -9.20179363e-03,  8.95722889e-01},
                      {7.82371142e-05, -9.67050621e-03,  8.99574933e-01},
                      {8.27256297e-05, -9.99353009e-03,  8.97461897e-01},
                      {7.29458059e-05, -9.22415170e-03,  8.80996165e-01},
                      {8.02383914e-05, -9.78291439e-03,  8.88964070e-01},
                      {8.50641730e-05, -1.01099145e-02,  8.87304833e-01},
                      {9.10537157e-05, -1.05303084e-02,  8.89438514e-01},
                      {9.43014320e-05, -1.07133543e-02,  8.82632055e-01},
                      {1.02655616e-04, -1.13154691e-02,  8.91617174e-01},
                      {1.06118094e-04, -1.14971777e-02,  8.85181637e-01},
                      {9.08474473e-05, -1.04296303e-02,  8.71942958e-01},
                      {9.35633054e-05, -1.05612167e-02,  8.63930371e-01},
                      {1.01940441e-04, -1.11573929e-02,  8.73153437e-01},
                      {9.19501202e-05, -1.04229157e-02,  8.58681822e-01},
                      {9.42991674e-05, -1.05098872e-02,  8.49103546e-01},
                      {1.02747171e-04, -1.11371417e-02,  8.60434431e-01},
                      {1.05024967e-04, -1.12138980e-02,  8.51101942e-01},
                      {1.14130011e-04, -1.18692049e-02,  8.62838259e-01},
                      {1.15163673e-04, -1.18553822e-02,  8.51306646e-01},
                      {1.27169055e-04, -1.27033210e-02,  8.68623539e-01},
                      {1.24195213e-04, -1.24047085e-02,  8.48974077e-01},
                      {1.12616150e-04, -1.15135023e-02,  8.19708800e-01},
                      {1.23637465e-04, -1.22869889e-02,  8.35392018e-01},
                      {1.22724317e-04, -1.21677963e-02,  8.22569523e-01},
                      {1.33891595e-04, -1.29267762e-02,  8.37122909e-01} };

   G4double a;  /* Watt Parameters */
   G4double b=1.0;

   G4double rand1,rand2;
   G4double x,y,z;
   G4double eSmp;
   G4int i;


/*
   Find Watt parameters for isotope
*/
   G4int isoindex=-1;
   for (i=0; isoindex == -1 && i<nZAfis; i++) {
      if (iso == nZA[i]) isoindex = i;
   }
   if (isoindex == -1) {
      std::ostringstream o;
      o << iso;
      std::string errMsg = "No Watt spectrum available for iso " + o.str();
      G4fissionerr(6, "SmpWatt", errMsg);
   }
   
   a= Watta[isoindex][2] + ePart*(Watta[isoindex][1] + ePart*Watta[isoindex][0]);

   x= 1. + (b/(8.*a));
   y= (x + std::sqrt(x*x-1.))/a;
   z= a*y - 1.;

   G4int icounter = 0;
   G4int icounter_max = 1024;
   do {

      rand1= -G4Log(fisslibrng());
      rand2= -G4Log(fisslibrng());
      eSmp= y*rand1;

      icounter++;
      if ( icounter > icounter_max ) { 
	 G4cout << "Loop-counter exceeded the threshold value at " << __LINE__ << "th line of " << __FILE__ << "." << G4endl;
         break;
      }

   } while ((rand2-z*(rand1+1.))*(rand2-z*(rand1+1.)) > b*y*rand1 ||
             eSmp < WATTEMIN || eSmp > WATTEMAX);
   // Loop checking, 11.03.2015, T. Koi
   
   return eSmp;
}

std::string itoa(const G4int& x);

void wuFissionEvent::G4fissionerr(G4int iSever, std::string chSubNam, std::string chMsg)

/*
  Description
    multi error handling routine
*/

/*
   Input
     iSever   - severity code:  larger number:  more severe
                0    : no error
                1-5  : non-fatal
                6-   : fatal
     chSubNm  - calling subroutine name
     chMsg    - error message
*/

{
   G4int doExit;
   std::string ExitMsg;
 
   
   if (iSever <= 5) {   /* warning */
     doExit = 0;
   }
   else {
     doExit = 1;
   }

   ExitMsg = "Error in Function "+chSubNam+", Severity=" + itoa(iSever) + " : "+chMsg;

   std::cerr << "Fission " << ExitMsg << std::endl;
   if (doExit == 1) G4Exception("wuFissionEvent::G4fissionerr()", "601",
                                FatalException, "Fatal Error");

   return;
}


std::string itoa(const G4int& x)
{
  std::ostringstream o;
  if (!(o << x)) return "ERROR";
  return o.str();
}


#include "Randomize.hh"

G4double (*wuFissionEvent::rngdptr)(void) = 0;
G4float (*wuFissionEvent::rngfptr)(void) = 0;

G4double wuFissionEvent::fisslibrng(void) {
  //   if (rngdptr == 0) {
  //      return drand48();
  //   } else {
  //      return rngdptr();
  //   }

  return G4UniformRand();
}

G4double wuFissionEvent::rngf2d() {
   return (G4double) rngfptr();
}



// 
// wuFissionEvent.cc ends here
