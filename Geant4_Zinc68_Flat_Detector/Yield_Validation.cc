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

// G4Runmanager and mandatory classes
#include "G4RunManager.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

// Randomize class to set seed number
#include "Randomize.hh"

// UI and visualization classes
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"


//MPI
#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <MpiManagerWJ.hh>

#include "G4Timer.hh"

int main(int argc, char** argv)
{
	G4Timer Timer;
	Timer.Start();

	MpiManagerWJ *MpiManager = new MpiManagerWJ(argc, argv);
	MpiManager->SetRankNumber();
	G4int rank = MpiManager->GetRankNumber();


	// Seed number setting
	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	G4int SEED = time(NULL)+rank;
	G4Random::setTheSeed(SEED);

	// Construct runmanager
	G4RunManager* runManager = new G4RunManager();

	runManager->SetUserInitialization(new DetectorConstruction());
	runManager->SetUserInitialization(new PhysicsList());
	runManager->SetUserInitialization(new ActionInitialization());

	runManager->Initialize();

	// Construct UI and visualization manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	G4VisManager* visManager = new G4VisExecutive();
	visManager->Initialize();

	if(argc==1)	// GUI (qt) based interactive mode
	{
	   G4UIExecutive* UI = new G4UIExecutive(argc, argv, "qt");
	   UImanager->ApplyCommand("/control/execute vis.mac");
	   UI->SessionStart();
	   delete UI;
	}
	else		// batch mode
	{
	   G4String command = "/control/execute ";
	   G4String filename = argv[1];
	   UImanager->ApplyCommand(command+filename);
	}


	//Close MPICH
	MPI_Finalize();

	// Free the store
	delete runManager;


	// Timer


	   Timer.Stop();
	   G4cout << G4endl;
	   G4cout << "******************************************";
	   G4cout << G4endl;
	   G4cout << "Total Real Elapsed Time is: "<< Timer.GetRealElapsed()/3600;
	   G4cout << G4endl;
	   G4cout << "Total System Elapsed Time: " << Timer.GetSystemElapsed()/3600;
	   G4cout << G4endl;
	   G4cout << "Total GetUserElapsed Time: " << Timer.GetUserElapsed()/3600;
	   G4cout << G4endl;
	   G4cout << "******************************************";
	   G4cout << G4endl;


	return 0;
}
