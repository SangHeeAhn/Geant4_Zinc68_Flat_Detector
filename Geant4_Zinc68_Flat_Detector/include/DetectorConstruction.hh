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

#ifndef DetectorConstruction_hh_
#define DetectorConstruction_hh_

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorConstruction: public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	virtual ~DetectorConstruction();

	virtual G4VPhysicalVolume* Construct();
	G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; } // Phantom mass

	virtual void ConstructSDandField();

	void SetDimention();

private:

	G4double WorldX;
	G4double WorldY;
	G4double WorldZ;

	G4double Tumor_TissueX;
	G4double Tumor_TissueY;
	G4double Tumor_TissueZ;

	G4double WaterBoxX;
	G4double WaterBoxY;
	G4double WaterBoxZ;

	G4double IsotopeRMin;
	G4double IsotopeRMax;
	G4double Isotopelength;
	G4double IsotopeSphi;
	G4double IsotopeDphi;

	G4double PhspRMin;
	G4double PhspRMax;
	G4double Phsplength;
	G4double PhspSphi;
	G4double PhspDphi;

	G4double Detector_1_X;
    G4double Detector_1_Y;
	G4double Detector_1_Z;

	G4double Detector_2_X;
    G4double Detector_2_Y;
	G4double Detector_2_Z;

	G4int NumberOfReplica;


	G4LogicalVolume*  fScoringVolume; // Phantom mass;



};

#endif
