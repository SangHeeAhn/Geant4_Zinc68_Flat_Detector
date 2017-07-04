// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// 	Author: yskim
//

#include "SensitiveDetector.hh"
#include <MpiManagerWJ.hh>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include "G4Track.hh"

using namespace std;



SensitiveDetector::SensitiveDetector(G4String name)
:G4VSensitiveDetector(name)
{

	// MPI rank
	MpiManagerWJ *MpiManager = new MpiManagerWJ();
	MpiManager->SetRankNumber();
	this->rank = MpiManager->GetRankNumber();

	// ofs.open("output.txt");


	//// Time
	time_t now;
	struct tm t;


	time(&now);
	t = *localtime(&now);
	G4int year = t.tm_year + 1900;
	G4int month = t.tm_mon +1;
	G4int day = t.tm_mday;
	G4int hour = t.tm_hour;
	G4int min = t.tm_min;
	G4int sec = t.tm_sec;

	// File name
	stringstream ss_filename;
	ss_filename<<"Gamma_511keV_"<<year << month << hour << day << "_" << hour << ":" << min <<":" <<sec <<"_RankNum:" << rank;
	filenameForSave = ss_filename.str();


	ofs.open(filenameForSave.c_str()); // Hist

	dE = 0;
	gamma_E = 0;
	total_gamma_E =0;
	// 	for (G4int ii = 0; ii < 100 ; ii++){
	// 		for (G4int jj = 0; jj < 100 ; jj++){
	// 			DEMatrix[ii][jj] = 0;
	// 		}
	// 	}
	//
	//
	// total_gamma_E = 0;
}


SensitiveDetector::~SensitiveDetector()
{

	// G4int nPrimaries = G4RunManager::GetRunManager()->GetNumberOfEventsToBeProcessed();
	// for (G4int ii = 0; ii < 100 ; ii++){
	// 	for (G4int jj = 0; jj < 100 ; jj++){
	// 		ofs << ii << "\t" << jj << "\t" << DEMatrix[ii][jj]/nPrimaries/MeV << G4endl;
	// 	}
	// 	ofs<<G4endl;
	// }
	//G4cout << " Rank check : " << rank << G4endl;

	this->ofs.close();
	//ofs.close((filenameForSave.c_str());

	//ofs.close(); // Hist
}

void SensitiveDetector::Initialize(G4HCofThisEvent*)
{



}




G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{


	//G4int RepZNo = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(0);
	//G4int RepYNo = aStep->GetPreStepPoint()->GetTouchable()->GetReplicaNumber(1);
	// dE = aStep->GetTotalEnergyDeposit();
	// DEMatrix[RepZNo][RepXNo] +=dE;
	// if(aStep->GetTrack()->GetParentID() == 0){
	// 	if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName()== "gamma"){
	// 		G4double Primary_gamma_E = aStep->GetPreStepPoint()->GetKineticEnergy();
	// 		//G4cout << Primary_gamma_E/keV << G4endl;
	// 		ofs<< Primary_gamma_E/keV << G4endl;
	// 	}
	// }


	// Spectrum check //
	//dE = aStep->GetPreStepPoint()->GetKineticEnergy();

		if(aStep->GetTrack()->GetParentID() != 0 ){
			if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName()=="gamma"){
				//G4cout<< aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName() << G4endl;
				if(aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName()== "Material"){

				G4String Origin_Volume = aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName();


				gamma_E = aStep->GetPreStepPoint()->GetKineticEnergy();
				//if (gamma_E/keV > 509 && gamma_E/keV < 511){
				// Get Particles Position
				G4ThreeVector wpos = aStep->GetPreStepPoint()->GetPosition();
				//G4ThreeVector wpos = aStep->GetPostStepPoint()->GetPosition();
				G4double X = wpos.x();
				G4double Y = wpos.y();
				G4double Z = wpos.z();

				// Get Particles Momentum
				G4ThreeVector Momentum = aStep->GetPreStepPoint()->GetMomentumDirection();
				//G4ThreeVector Momentum = aStep->GetPostStepPoint()->GetMomentumDirection();
				G4double X_mom = Momentum.x();
				G4double Y_mom = Momentum.y();
				G4double Z_mom = Momentum.z();

				//G4cout <<X_mom<<", "<<Y_mom<<", "<<Z_mom<<endl;
				//ofs <<X_mom<<", "<<Y_mom<<", "<<Z_mom<<endl;

				// Get Particles Energy
				gamma_E = aStep->GetPreStepPoint()->GetKineticEnergy();
				//G4double gamma_E = aStep->GetPostStepPoint()->GetKineticEnergy();
				//G4cout<<aStep->GetPreStepPoint()->GetKineticEnergy()<<G4endl;

				// Get Creator ProcessHits
				G4String Creator_Porcess = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();

				char tab = '\t';

				// Get Particles PDG code
				G4double PDG_type = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();

				// Get TOF
				G4double TOF = aStep->GetTrack()->GetGlobalTime();
				//ofs<<X/cm<<tab<<Y/cm<<tab<<Z/cm<<tab<<X_mom<<tab<<Y_mom<<tab<<Z_mom<<tab<<gamma_E/MeV<<tab<<PDG_type<<tab<<Creator_Porcess<<tab<<TOF<<G4endl;
				ofs<<X/cm<<tab<<Y/cm<<tab<<Z/cm<<tab<<X_mom<<tab<<Y_mom<<tab<<Z_mom<<tab<<gamma_E/MeV<<tab<<Origin_Volume<<G4endl;


				// aStep->GetPostStepPoint()->GetLocalTime();
				// DEMatrix[RepZNo][RepYNo] +=dE;
				//G4cout << dE/keV << G4endl;
				//G4cout << aStep->GetTrack()->GetOriginTouchable()->GetVolume()->GetName() << G4endl;
				//G4cout << aStep->GetTrack()->GetParticleDefinition()->GetParticleName() << G4endl;
				//G4ThreeVector wpos = aStep->GetPreStepPoint()->GetPosition();
				//ofs << X/cm<< ","<< Y/cm << "," << Z/cm << G4endl;
				//total_gamma_E +=gamma_E;
				//G4cout << dE <<"," <<gamma_E<<G4endl;
				//ofs<< total_gamma_E/keV << G4endl;
				//}
			}
		}
	}
	return true;
}

//DEMatrix[RepZNo][RepXNo] += dE;
//G4cout << " Rank check : " << rank << G4endl;

// return true;
// }

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

}
