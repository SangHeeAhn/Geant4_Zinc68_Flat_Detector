#ifndef SensitiveDetector_add_hh_
#define SensitiveDetector_add_hh_

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"


#include <string.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <stdio.h>
using namespace std;


class G4HCofThisEvent;
class G4TouchableHistory;

class SensitiveDetector_add: public G4VSensitiveDetector
{
public:
	SensitiveDetector_add(G4String name);
	virtual ~SensitiveDetector_add();

	void Initialize(G4HCofThisEvent*);
	G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*);
	void EndOfEvent(G4HCofThisEvent*);
private:
	// MPI
	G4int rank;

	// Save File
	std::ofstream ofs;
	G4double DEMatrix[300][300];
	string filenameForSave;
	G4double dE;
	G4double Dose;
	G4double Mass;
	G4double gamma_E;

};

#endif
