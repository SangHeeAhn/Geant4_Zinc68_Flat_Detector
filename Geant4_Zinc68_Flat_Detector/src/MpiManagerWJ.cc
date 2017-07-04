/*
 * MpiManagerWJ.cc
 *
 *  Created on: Nov 10, 2015
 *      Author: jason
 */

#include "MpiManagerWJ.hh"
#include "mpi.h"

MpiManagerWJ::MpiManagerWJ() {
	// TODO Auto-generated constructor stub

}

MpiManagerWJ::MpiManagerWJ(G4int argc, char** argv) {
	// TODO Auto-generated constructor stub
	MPI_Init(&argc, &argv);

	//MPI_Comm_rank(MPI_COMM_WORLD, &(this->rank));

	//G4cout<< "rank is " << rank << " on the MpiManager "<< G4endl;


}


MpiManagerWJ::~MpiManagerWJ() {
	// TODO Auto-generated destructor stub
}


void MpiManagerWJ::SetRankNumber(){

	MPI_Comm_rank(MPI_COMM_WORLD, &(this->rank));
//	G4cout<< "rank is " << rank << " on the MpiManager "<< G4endl;

}

G4int MpiManagerWJ::GetRankNumber(){

	return rank;

}
