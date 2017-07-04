#include "DetectorConstruction.hh"
#include "SensitiveDetector_add.hh"

#include "G4Box.hh"
#include "G4Isotope.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "SensitiveDetector.hh"

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),fScoringVolume(0)
{

}


DetectorConstruction::~DetectorConstruction()
{

}

void DetectorConstruction::SetDimention(){

	WorldX = 1*m;
	WorldY = 1*m;
	WorldZ = 1*m;


	IsotopeRMin = 0*cm;
	IsotopeRMax = 0.1*cm;
	Isotopelength = 0.3*cm;
	IsotopeSphi = 0.0*deg;
	IsotopeDphi = 360.0*deg;

	Tumor_TissueX = 2.5*cm;
	Tumor_TissueY = 2.5*cm;
	Tumor_TissueZ = 2.5*cm;

	PhspRMin = 0.*cm;
	PhspRMax = 0.1*cm;
	Phsplength = 0.3*cm;
	PhspSphi = 0.0*deg;
	PhspDphi = 360.0*deg;

	WaterBoxX = 30*cm;
	WaterBoxY = 30*cm;
	WaterBoxZ = 30*cm;

	Detector_1_X = 0.5*cm;
	Detector_1_Y = 10.0*cm;
	Detector_1_Z = 10.0*cm;

	Detector_2_X = 0.5*cm;
	Detector_2_Y = 10.0*cm;
	Detector_2_Z = 10.0*cm;


	NumberOfReplica = 100;

}

G4VPhysicalVolume* DetectorConstruction::Construct()
{

	SetDimention();

		// Material //
	G4double z;
	G4String name;
	G4String symbol;
	G4double density, ncomponents;
	G4double a_carbon = 12.01*g/mole;
	G4Element* ele_C = new G4Element(name="Carbon",symbol="C",z= 12.,a_carbon);
	
	G4double a_oxygen = 16.00*g/mole;
	G4Element* ele_O = new G4Element(name="Oxygen",symbol="O",z= 8.,a_oxygen);
	
	G4double a_hydrogen = 1.007*g/mole;
	G4Element* ele_H = new G4Element(name="Hydrogen",symbol="H",z= 1.,a_hydrogen);

	density = 2.0*g/cm3;
	G4Material* Polymer = new G4Material(name="Polymer",density,ncomponents=3);
	G4double fracMass;
	Polymer->AddElement(ele_C, fracMass=28.0*perCent);
	Polymer->AddElement(ele_O, fracMass=16.0*perCent);
	Polymer->AddElement(ele_H, fracMass=56.0*perCent);

       	
	// Zn isotope //
	G4double Z, A, a1, n_comp,Zn68density;
	Zn68density = 5.91*g/cm3;
	G4Isotope* IsoZinc68 = new G4Isotope("Zn68", Z=31, A=68, a1= 67.71*g/mole);
	G4Element* enrichedZinc =  new G4Element("enriched Zinc", "enZn", n_comp=1);
	G4Material* Zn68riched = new G4Material("enriched Zinc68",Zn68density,1);
	enrichedZinc -> AddIsotope(IsoZinc68, 100.0*perCent);
	Zn68riched  ->AddElement(enrichedZinc,1);

	// Poly_Ga68 marker //
	//density = 1.4583*g/cm3;
	density = 1.0*g/cm3;
	//density = 5.91*g/cm3;
	G4Material* Poly_Ga68_Marker = new G4Material("Poly_Ga68_Marker",density,ncomponents=2);
	Poly_Ga68_Marker->AddMaterial(Polymer,80.0*perCent);
	Poly_Ga68_Marker->AddMaterial(Zn68riched,20.0*perCent);


	// World
	G4VSolid* sol_World = new G4Box("World", WorldX*0.5, WorldY*0.5, WorldZ*0.5);
	G4Material* AIR = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
	G4LogicalVolume* lv_World = new G4LogicalVolume(sol_World, AIR, "World");
	G4VPhysicalVolume* pv_World =
			new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0), lv_World, "World", 0, false, 0);


	//Water box (x: 30 cm, y: 30 cm, z: 30 cm, at (0, 0, 0))
	G4VSolid* sol_Geom = new G4Box("Geom", WaterBoxX*0.5, WaterBoxX*0.5, WaterBoxX*0.5);
	G4Material* WATER = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
	G4LogicalVolume* lv_Geom = new G4LogicalVolume(sol_Geom, AIR, "Geom");
	G4VPhysicalVolume* pv_Geom =
			new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0*cm), lv_Geom, "Geom", lv_World, false, 10);


	//Tumor Tissue (x: 5 cm, y: 5 cm, z: 5 cm, at (0, 0, 0))
	// Get nist material manager
	//G4NistManager* nist = G4NistManager::Instance();
	//G4Material* A_150 = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
	//G4VSolid* sol_Tissue = new G4Box("Tissue", Tumor_TissueX*0.5, Tumor_TissueY*0.5, Tumor_TissueZ*0.5);
	//G4LogicalVolume* lv_Tissue = new G4LogicalVolume(sol_Tissue, A_150, "Tissue");
	//G4VPhysicalVolume* pv_Tissue =
	//		new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, 0.0*cm), lv_Tissue, "Tissue", lv_Geom, false, 20);


	//Detector1 (x: 30 cm, y: 30 cm, z: 30 cm, at (0, 0, 0))
	G4VSolid* sol_Detector_1 = new G4Box("Detector_1", Detector_1_X*0.5, Detector_1_Y*0.5, Detector_1_Z*0.5);
	G4Material* Galactic = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	G4LogicalVolume* lv_Detector_1 = new G4LogicalVolume(sol_Detector_1, Galactic, "Detector_1");
	G4VPhysicalVolume* pv_Detector_1 =
			new G4PVPlacement(0, G4ThreeVector(5.0*cm, 0.0, 0.0*cm), lv_Detector_1, "Detector_1", lv_Geom, false, 10);



	//Detector2 (x: 30 cm, y: 30 cm, z: 30 cm, at (0, 0, 0))
	G4VSolid* sol_Detector_2 = new G4Box("Detector_2", Detector_2_X*0.5, Detector_2_Y*0.5, Detector_2_Z*0.5);
	G4LogicalVolume* lv_Detector_2  = new G4LogicalVolume(sol_Detector_2, Galactic, "Detector_2");
	G4VPhysicalVolume* pv_Detector_2 =
			new G4PVPlacement(0, G4ThreeVector(-5.0*cm, 0.0, 0.0*cm), lv_Detector_2, "Detector_2", lv_Geom, false, 10);


	// Replica //
	// G4VSolid* sol_RepX = new G4Box("RepX", (WaterBoxX/NumberOfReplica)*0.5, WaterBoxX*0.5, WaterBoxX*0.5);
	// G4LogicalVolume* lv_RepX = new G4LogicalVolume(sol_RepX,WATER,"RepX");
	// G4VPhysicalVolume* pv_RepX = new G4PVReplica("RepX",lv_RepX,lv_Geom,kXAxis,NumberOfReplica,WaterBoxX/NumberOfReplica);
	//
	// G4VSolid* sol_RepZ = new G4Box("RepZ", (WaterBoxZ/NumberOfReplica)*0.5, WaterBoxZ*0.5, (WaterBoxZ/NumberOfReplica)*0.5);
	// G4LogicalVolume* lv_RepZ = new G4LogicalVolume(sol_RepZ,WATER,"RepZ");
	// G4VPhysicalVolume* pv_RepZ = new G4PVReplica("RepZ",lv_RepZ,lv_RepX,kZAxis,NumberOfReplica,WaterBoxZ/NumberOfReplica);

	//User geometry
	// G4double thetaX = 0*deg;
	// G4double thetaY = 0*deg;
	// G4double thetaZ = 0*deg;
	// G4RotationMatrix *rotM = new G4RotationMatrix();
	// rotM ->rotateY(thetaY);
	// G4VSolid* sol_Det = new G4Box("Detector", DetectorX*0.5, DetectorY*0.5, DetectorZ*0.5);
	// G4Material* WATER = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
	// G4LogicalVolume* lv_Det = new G4LogicalVolume(sol_Det, WATER, "Detector");
	// G4VPhysicalVolume* pv_Det = new G4PVPlacement(rotM, G4ThreeVector(8.0*cm, 0.0*cm, 0.0*cm), lv_Det, "Detector", lv_World, false, 10);

	// G4VSolid* sol_RepX = new G4Box("RepX", (DetectorX/NumberOfReplica)*0.5, DetectorX*0.5, DetectorX*0.5);
	// G4LogicalVolume* lv_RepX = new G4LogicalVolume(sol_RepX,WATER,"RepX");
	// G4VPhysicalVolume* pv_RepX = new G4PVReplica("RepX",lv_RepX,lv_Det,kXAxis,NumberOfReplica,DetectorX/NumberOfReplica);

	// G4VSolid* sol_RepY = new G4Box("RepY", DetectorY*0.5, (DetectorY/NumberOfReplica)*0.5, DetectorY*0.5);
	// G4LogicalVolume* lv_RepY = new G4LogicalVolume(sol_RepY,WATER,"RepY");
	// G4VPhysicalVolume* pv_RepY = new G4PVReplica("RepY",lv_RepY,lv_Det,kYAxis,NumberOfReplica,DetectorY/NumberOfReplica);
	//
	// G4VSolid* sol_RepZ = new G4Box("RepZ", DetectorZ*0.5, (DetectorZ/NumberOfReplica)*0.5, (DetectorZ/NumberOfReplica)*0.5);
	// G4LogicalVolume* lv_RepZ = new G4LogicalVolume(sol_RepZ,WATER,"RepZ");
	// G4VPhysicalVolume* pv_RepZ = new G4PVReplica("RepZ",lv_RepZ,lv_RepY,kZAxis,NumberOfReplica,DetectorZ/NumberOfReplica);


	// Surface Detector //
	G4double thetaX = 90*deg;
	G4double thetaY = 0*deg;
	G4double thetaZ = 0*deg;

	G4RotationMatrix*rotM = new G4RotationMatrix();
	rotM -> rotateX(thetaX);
	G4Material* Phsp = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
	G4VSolid* Phsp_sol_Material = new G4Tubs("Phsp_Material", PhspRMin*0.5, PhspRMax*0.5, Phsplength*0.5,PhspSphi,PhspDphi);
	G4LogicalVolume* Phsp_lv_Material= new G4LogicalVolume(Phsp_sol_Material, Phsp, "Phsp_Material");
	//G4VPhysicalVolume* pv_Material = new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, 0.6*cm), lv_Material, "Material", lv_Geom, true, 10);
	G4VPhysicalVolume* Phsp_pv_Material = new G4PVPlacement(rotM , G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm), Phsp_lv_Material, "Phsp_Material", lv_Geom, true, 20);
	//G4VPhysicalVolume* pv_Material = new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, 0.6*cm), lv_Material, "Material", lv_RepZ, true, 10);

	// Zn-fiducial material //
	//G4Material* Zinc = G4NistManager::Instance()->FindOrBuildMaterial("G4_Zn");
	G4VSolid* sol_Material = new G4Tubs("Material", IsotopeRMin*0.5, IsotopeRMax*0.5, Isotopelength*0.5,IsotopeSphi,IsotopeDphi);
	G4LogicalVolume* lv_Material= new G4LogicalVolume(sol_Material, Poly_Ga68_Marker, "Material");
	//G4VPhysicalVolume* pv_Material = new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, 0.6*cm), lv_Material, "Material", lv_Geom, true, 10);
	G4VPhysicalVolume* pv_Material = new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm), lv_Material, "Material", Phsp_lv_Material, true, 10);
	//G4VPhysicalVolume* pv_Material = new G4PVPlacement(0, G4ThreeVector(0.0*cm, 0.0*cm, 0.6*cm), lv_Material, "Material", lv_RepZ, true, 10);

	// Visualization
	G4VisAttributes* va_World = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
	va_World->SetForceWireframe(true);
	lv_World->SetVisAttributes(va_World);

	// G4VisAttributes* va_Geom = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
	// va_Geom->SetForceWireframe(true);
	// lv_Geom->SetVisAttributes(va_Geom);

	// WaterPhantom
	G4VisAttributes* va_Geom = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
	va_Geom->SetForceWireframe(true);
	lv_Geom->SetVisAttributes(va_Geom);

	//Surface Phsp
	G4VisAttributes* va_Phsp_Material = new G4VisAttributes(G4Colour(0.0, 0.0,1.0,0.3));
	//va_Phsp_Material->SetForceWireframe(true);
	va_Phsp_Material->SetForceSolid(true);
	Phsp_lv_Material->SetVisAttributes(va_Phsp_Material);

	// Zinc
	G4VisAttributes* va_Material = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.8));
	//va_Material->SetForceWireframe(true);
	va_Material->SetForceSolid(true);
	lv_Material->SetVisAttributes(va_Material);

	// Tumor_Tissue
	//G4VisAttributes* va_Tissue = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0, 0.8));
	//va_Material->SetForceWireframe(true);
	//va_Tissue->SetForceSolid(true);
	//lv_Tissue->SetVisAttributes(va_Tissue);

	// Detector_1
	G4VisAttributes* va_sol_Detector_1 = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
	va_sol_Detector_1->SetForceWireframe(true);
	lv_Detector_1->SetVisAttributes(va_sol_Detector_1);

	// Detector_2
	G4VisAttributes* va_sol_Detector_2 = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0));
	va_sol_Detector_2 ->SetForceWireframe(true);
	lv_Detector_2->SetVisAttributes(va_sol_Detector_2);



	// G4VisAttributes* va_Rep = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0, 0.3));
	// va_Rep->SetForceWireframe(true);
	// //lv_RepX->SetVisAttributes(va_Rep);
	// lv_RepY->SetVisAttributes(va_Rep);
	// lv_RepZ->SetVisAttributes(va_Rep);

	// G4VisAttributes* va_Det = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
	// va_Det->SetForceWireframe(true);
	// lv_Det->SetVisAttributes(va_Det);

	//fScoringVolume=lv_Tissue;
	//fScoringVolume=lv_Geom;

	return pv_World;
}

void DetectorConstruction::ConstructSDandField()
{
	// Set sensitive detector on "Geom"
	SetSensitiveDetector("Material", new SensitiveDetector("Material"));
	//SetSensitiveDetector("Phsp_Material", new SensitiveDetector_add("Phsp_Material"));
	//SetSensitiveDetector("Tissue", new SensitiveDetector_add("Tissue"));
	//SetSensitiveDetector("Geom", new SensitiveDetector("detector"));
	//SetSensitiveDetector("RepZ", new SensitiveDetector("detector"));                    // replica set up
	//SetSensitiveDetector("Detector_1", new SensitiveDetector("Detector_1"));
	//SetSensitiveDetector("Detector_2", new SensitiveDetector_add("Detector_2"));
}
