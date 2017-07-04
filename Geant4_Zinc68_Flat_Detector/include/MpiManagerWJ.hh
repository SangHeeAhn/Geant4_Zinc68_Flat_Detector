/*
 * MpiManagerWJ.hh
 *
 *  Created on: Nov 10, 2015
 *      Author: jason
 */

#ifndef INCLUDE_MPIMANAGERWJ_HH_
#define INCLUDE_MPIMANAGERWJ_HH_

#include "globals.hh"
#include "G4SystemOfUnits.hh"

class MpiManagerWJ {
public:
	MpiManagerWJ();
	MpiManagerWJ(G4int, char **);
	virtual ~MpiManagerWJ();
	void SetRankNumber();
	G4int GetRankNumber();

private:
	G4int rank;
};

#endif /* INCLUDE_MPIMANAGERWJ_HH_ */
