//
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
/// \file polarisation/Pol01/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// $Id: DetectorConstruction.cc 98772 2016-08-09 14:25:31Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"

#include "G4PolarizationManager.hh"
// #include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(G4String version)
: G4VUserDetectorConstruction(),
  PhysicalWorld(0), PhysicalCore(0), fConvMaterial(0), fWorldMaterial(0), fCaloMaterial(0)
{
  versionType=version;
  allMaterials = new Materials();
  allMaterials->DefineMaterials();

  fSizeXY = 50*mm;
  fCoreThick = 75*mm;
  fConvThick = 1.75*mm;
  fWorldSize = 4.1*m;

  SetConvMaterial("G4_W");
  SetWorldMaterial("Galactic");
  SetCaloMaterial("TF101");

  fMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4PolarizationManager::GetInstance()->Clean();

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Geometry parameters
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //
  // Polarimeter
  //
  G4double  maggap1 = 48.5*mm;
  G4double  maggap2 = 12.5*mm;
  G4double  absrad  = fSizeXY/2.;
  G4double shieldrad=75.0*mm;
  G4double vacthick = 1.0*mm;
  G4double corethick = fCoreThick;
  G4double convthick = fConvThick;
  G4double  coilthick = corethick + 25.0*mm;
  G4double  shieldthick = corethick - 25.0*mm;
  G4double  conedist = corethick/2. + maggap2;
  G4double  magthick = 2.*(maggap1+convthick+maggap2)+corethick;

  //
  // Calorimeter
  //
  G4double detthick = 45.*cm; // crystal size
  G4double detx = 3.8 *cm;
  G4double dety = 3.8 *cm;

  G4double alairgapthick = 0.001 *mm;    // thickness of the air gap between the aluwrapping and the crystal
  G4double alairgapx = detx + 2*alairgapthick;
  G4double alairgapy = dety + 2*alairgapthick;
  G4double alairgaplength = detthick + alairgapthick;

  G4double aluwrapthick = 0.01  *mm;   // wikipedia: alu foil thickness between 0.004 and 0.02 mm
  G4double aluwrapx = alairgapx + 2*aluwrapthick;
  G4double aluwrapy = alairgapy + 2*aluwrapthick;
  G4double aluwraplength = alairgaplength + aluwrapthick + vacthick;

  //defining the size of the Calorimeterzell and the virtual calorimeter (mother volume of the calorimetercells)
  G4int NbofCalor = 9; //here later free paramter to select numer of crystals
  G4double calorcellxy = aluwrapx;
  G4double calorcelllength = aluwraplength;
  G4double virtcalorxy = NbofCalor*calorcellxy/3;
  G4double virtcalorlength = aluwraplength;


  G4double vac3x = alairgapx;// this version is to place the vacstep in the aluwrapping
  G4double vac3y = alairgapy; // this version is to place the vacstep in the aluwrapping

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  //Get materials
  G4Material* absMat    = allMaterials->GetMat("G4_W");
  G4Material* magMat    = allMaterials->GetMat("G4_Fe");
  G4Material* coilMat   = allMaterials->GetMat("G4_Cu");
  G4Material* shieldMat = allMaterials->GetMat("G4_Pb");
  G4Material* Air       = allMaterials->GetMat("Air");
  G4Material* Al        = allMaterials->GetMat("Aluminium");

  // World
  //
  G4Box*
  SolidWorld = new G4Box("World",                            //name
                   fWorldSize/2,fWorldSize/2,fWorldSize/2); //dimensions

  G4LogicalVolume*
  LogicalWorld = new G4LogicalVolume(SolidWorld,                   //shape
                               fWorldMaterial,           //material
                              "World");                  //name

  PhysicalWorld = new G4PVPlacement(0,                          //no rotation
                             G4ThreeVector(),            //at (0,0,0)
                             LogicalWorld,                     //logical volume
                             "World",                    //name
                             0,                          //mother volume
                             false,                      //no boolean operation
                             0);                         //copy number

// Magnet
//
  G4double DzArrayMagnet   [] = {-magthick/2.  , -conedist , -corethick/2. , corethick/2.,  conedist, magthick/2.    };
  G4double RminArrayMagnet [] = {36.84308*mm,  absrad,  absrad , absrad,  absrad,  36.84308*mm};
  G4double RmaxArrayMagnet [] = {196.0*mm   ,  196.0*mm, 196.0*mm ,196.0*mm, 196.0*mm, 196.0*mm    };

  G4Polycone *solidMagnet = new G4Polycone("solidMagnet", 	 //its name
            0.0*deg, 		 //its start angle
            360.0*deg,		 //its opening angle
            6, 		         //its nZ
            DzArrayMagnet, 	 //z value
            RminArrayMagnet, 	 //rmin
            RmaxArrayMagnet ); 	 //rmax

  G4LogicalVolume * LogicalMagnet = new G4LogicalVolume(solidMagnet, //its solid
                  magMat, 	 //its material
                   "Magnet" ,		 //its name
                   0,0,0);

  G4VPhysicalVolume *  PhysicalMagnet= new G4PVPlacement(0,	//rotation
  								 G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm),// translation position
  								 LogicalMagnet,      //its logical volume
  						       "PhysicalMagnet",   //its name  (2nd constructor)
  						       LogicalWorld,         //its mother volume
  						       false,              //no boolean operation
  						       0);                 //copy number


  //Copper Coils
  //
  G4Tubs *solidCuTube= new G4Tubs("solidCuTube", //name
                                  shieldrad, // inner radius
                                  170.0*mm,  // outer radius
                                  coilthick/2., // half length in z
                                  0.0*deg,  // starting angle
                                  360.0*deg ); // total angle

  G4LogicalVolume * LogicalCuTube = new G4LogicalVolume(solidCuTube, //its solid
             coilMat,              //its material
             "CuTube" ,		         //its name
             0,0,0);

  G4VPhysicalVolume *PhysicalCuTube= new G4PVPlacement(0,	//rotation
         G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm), // translation position
           LogicalCuTube,      // its logical volume
           "PhysicalCuTube",   // its name  (2nd constructor)
           LogicalMagnet,     // its mother volume
           false,              // no boolean operation
           0);

  // Lead Tube
  //
  G4Tubs *solidPbtube= new G4Tubs("solidPbtube", // name
                                  absrad, // inner radius
                                  shieldrad, // outer radius
                                  shieldthick/2., // half length in z
                                  0.0*deg, // start angle
                                  360.0*deg ); // total angle

  G4LogicalVolume * LogicalPbtube = new G4LogicalVolume(solidPbtube, 	 //its solid
  						shieldMat, 		 //its material
  						"Pbtube" ,		 //its name
  						0,0,0);

  G4VPhysicalVolume *PhysicalPbTube= new G4PVPlacement(0,	//rotation
  				G4ThreeVector(0.0*mm, 0.0*mm, 0.0*mm),
  				LogicalPbtube,      //its logical volume
  			    "PhysicalPbTube",   //its name  (2nd constructor)
  			    LogicalMagnet,     //its mother volume
  			    false,              //no boolean operation
  			    0);                 //copy number

  // Reconversion Target
  //
  auto solidReconversion = new G4Tubs("solidReconversion", // name
                                      0.0*mm, // inner radius
                                      absrad, // outer radius
                                      convthick/2, // half length in z
                                      0.0*deg, // starting angle
                                      360.0*deg ); // total angle

  auto LogicalReconversion = new G4LogicalVolume(solidReconversion, 	 //its solid
              absMat,          //its material
              "solidReconversion" ,	 //its name
              0,0,0);

  auto PhysicalReconversion= new G4PVPlacement(0,	//rotation
              G4ThreeVector(0.0*mm, 0.0*mm, -maggap2-convthick/2-corethick/2),
            LogicalReconversion,         //its logical volume
            "PhysicalReconversion",   //its name  (2nd constructor)
            LogicalWorld,              //its mother volume
            false,                 //no boolean operation
            0);                       //copy number

  // Iron Core
  //
  auto solidCore = new G4Tubs ("Container",                           //its name
                   0.0*mm, absrad*mm, corethick/2, 0.0*deg, 360.0*deg );//its dimensions

  G4LogicalVolume*
  LogicalCore = new G4LogicalVolume(solidCore,                        //its shape
                             magMat,             //its material
                             "IronCore");                   //its name

  PhysicalCore = new G4PVPlacement(0,                             //no rotation
                           G4ThreeVector(),               //at (0,0,0)
                           LogicalCore,                          //its logical volume
                           "IronCorePV",    //its name
                           LogicalWorld,                        //its mother  volume
                           false,                         //no boolean operation
                           0);                            //copy number

  // register logical Volume in PolarizationManager with polarization
  G4PolarizationManager * polMgr = G4PolarizationManager::GetInstance();
  polMgr->SetVolumePolarization(LogicalCore,G4ThreeVector(0.,0.,1.));

  PrintParameters();

  //
  //vacuum step 1
  //
  auto VacStepS1 = new G4Tubs("VacStep1",  //Name
                              0.,         // inner radius
                              absrad,     // outer radius
                              vacthick/2., // half length in z
                              0.0*deg,    // starting phi angle
                              360.0*deg); // angle of the segment


auto  VacStepLV1 = new G4LogicalVolume(VacStepS1,    //its solid
                                       fWorldMaterial,    //its material
                                       "VacStep1");  //its name

  fVacStepPV1 = new G4PVPlacement(0,                   //no rotation
                       G4ThreeVector(0.,0., - corethick/2 -maggap2 +vacthick/2 +1.0*mm),    //its position
                               VacStepLV1,            //its logical volume
                               "VacStep1",                 //its name
                               LogicalWorld,               //its mother
                               false,                     //no boolean operat
                               0);                        //copy number

   //





  // VacStep
  //
  auto VacStepS2 = new G4Tubs("VacStep2",  //Name
                               0.,         // inner radius
                               absrad,     // outer radius
                               vacthick/2., // half length in z
                               0.0*deg,    // starting phi angle
                               360.0*deg); // angle of the segment

   auto VacStepLV2 = new G4LogicalVolume(VacStepS2,    //its solid
                                        fWorldMaterial,    //its material
                                        "VacStep1");       //its name

   fVacStepPV2 = new G4PVPlacement(0,                   //no rotation
                        G4ThreeVector(0.,0.,corethick/2 + vacthick/2 + 10.0*mm),    //its position
                                VacStepLV2,            //its logical volume
                                "VacStep2",                 //its name
                                LogicalWorld,               //its mother
                                false,                     //no boolean operat
                                0);                        //copy number

  //always return the root volume
  //
  return PhysicalWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintParameters()
{
  G4cout << "\n The ConverterTarget is made of " << fConvMaterial->GetName()
          << " , " << G4BestUnit(fSizeXY,"Length")<<  "in diameter and "
         <<  G4BestUnit(fConvThick,"Length") << " thick"
           << G4endl;
 G4cout << "\n The IronCore is"
        <<  G4BestUnit(fCoreThick,"Length") << " thick"
          << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetConvMaterial(G4String materialChoice)
{
  // search the material by its name
  // G4Material* mat =
  //   G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  G4Material* mat = allMaterials->GetMat(materialChoice);
  if (mat != fConvMaterial) {
    if(mat) {
      fConvMaterial = mat;
      UpdateGeometry();
    } else {
      G4cout << "### Warning!  Converter Target material: <"
           << materialChoice << "> not found" << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // search the material by its name
  // G4Material* mat =
  //   G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  G4Material* mat = allMaterials->GetMat(materialChoice);
  if (mat != fWorldMaterial) {
    if(mat) {
      fWorldMaterial = mat;
      UpdateGeometry();
    } else {
      G4cout << "### Warning! World material: <"
           << materialChoice << "> not found" << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetCaloMaterial(G4String materialChoice)
{
  // search the material by its name
  // G4Material* mat =
  //   G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  G4Material* mat = allMaterials->GetMat(materialChoice);
  if (mat != fCaloMaterial) {
    if(mat) {
      fCaloMaterial = mat;
      UpdateGeometry();
    } else {
      G4cout << "### Warning! Calorimeter material: <"
           << materialChoice << "> not found" << G4endl;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetSizeXY(G4double value)
{
  fSizeXY = value;
  if (fWorldSize<fSizeXY) fWorldSize = 10*fSizeXY;
  UpdateGeometry();
}

void DetectorConstruction::SetCoreThick(G4double value)
{
  fCoreThick = value;
  if (fWorldSize<fCoreThick) fWorldSize = 10*fCoreThick;
  UpdateGeometry();
}

void DetectorConstruction::SetConvThick(G4double value)
{
  fConvThick = value;
  UpdateGeometry();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  if (PhysicalWorld)
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
