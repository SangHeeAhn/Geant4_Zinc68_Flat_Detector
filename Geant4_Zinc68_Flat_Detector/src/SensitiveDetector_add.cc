/*
 * SensitiveDetector_add.cc
 *
 *  Created on: Jan 24, 2017
 *      Author: ash
 */
#include "SensitiveDetector_add.hh"
#include "DetectorConstruction.hh"
#include <MpiManagerWJ.hh>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <stdio.h>

#include "G4Track.hh"

using namespace std;



SensitiveDetector_add::SensitiveDetector_add(G4String name)
:G4VSensitiveDetector(name)
{

	// MPI rank
	MpiManagerWJ *MpiManager = new MpiManagerWJ();
	MpiManager->SetRankNumber();
	this->rank = MpiManager->GetRankNumber();

	// ofs.open("output.txt");


	// Time
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
	ss_filename<<"Proton_In_Zinc_"<<year << month << hour << day << "_" << hour << ":" << min <<":" <<sec <<"_RankNum:" << rank;
	filenameForSave = ss_filename.str();


	ofs.open(filenameForSave.c_str()); // Hist


	Mass = 0;
	dE = 0;
	Dose = 0;
	gamma_E=0;
}


SensitiveDetector_add::~SensitiveDetector_add()
{

	this->ofs.close();

}
void SensitiveDetector_add::Initialize(G4HCofThisEvent*)
{


}


G4bool SensitiveDetector_add::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
	//dE += aStep->GetTotalEnergyDeposit();

	//G4cout << dE/MeV << G4endl;

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


	if(aStep->GetTrack()->GetParentID() == 0 ){
			if(aStep->GetTrack()->GetParticleDefinition()->GetParticleName()=="proton"){
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
				//ofs<<X/cm<<tab<<Y/cm<<tab<<Z/cm<<tab<<X_mom<<tab<<Y_mom<<tab<<Z_mom<<tab<<gamma_E/MeV<<tab<<G4endl;


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



void SensitiveDetector_add::EndOfEvent(G4HCofThisEvent*)
{
	//const DetectorConstruction* detectorConstruction  = static_cast<const DetectorConstruction*>

	//(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	//Mass = detectorConstruction->GetScoringVolume()->GetMass();

	//G4cout << Mass/kg <<" kg"<<G4endl;

	//G4double J = 1.602176487*pow(10,-16);

	//Dose  = ((dE/keV)*J)/(Mass/kg);

	//G4cout << Dose << "Gy" << G4endl;

	//ofs << Dose << G4endl;


}
