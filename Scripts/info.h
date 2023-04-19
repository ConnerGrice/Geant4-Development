/*
 * info.h
 *
 *  Created on: Apr 19, 2023
 *      Author: local1
 */

#ifndef GEANT4_DEVELOPMENT_SCRIPTS_INFO_H_
#define GEANT4_DEVELOPMENT_SCRIPTS_INFO_H_

#include <math.h>

const double ENERGY    = 500.;       // Beam energy (MeV/u)
const double Exe       = 0.0;        // Residual excitation energy (MeV) (change it for deeply bound states)
const double UNIT = 931.4940954;          // Atomic mass unit MeV/c�


//Nuclear masses from http://wwwndc.jaea.go.jp/NuC/
// WARNING (M. Winkel): website above lists ATOMIC masses!

//C-12
const int    A 	 = 12;                            // Mass number of the nucleus A
const double MA  = 12.*UNIT;		          // Nuclear mass of initital A nucleus(MeV/c�)
const double MB  = 11.009305404*UNIT + Exe;       // Nuclear mass of the residual fragment B (MeV/c�)
const double MOM_SIGMA = 100.00;                  // Internal momentum spread (Gauss)

//
const double Ma  = 938.272;  			 // Mass of the knocked-out nucleon (MeV/c�)
const double Mi  = 938.272;  			 // Mass of the scattered nucleon (MeV/c�)

//Constants
const double PI = M_PI;



#endif /* GEANT4_DEVELOPMENT_SCRIPTS_INFO_H_ */
