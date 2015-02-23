//
// ********************************************************************

#ifndef HadDetectorConstruction_h
#define HadDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"

class G4LogicalVolume;
class HadDetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HadDetectorConstruction : public G4VUserDetectorConstruction
{
public:

  HadDetectorConstruction();
  virtual ~HadDetectorConstruction();

public:

  G4VPhysicalVolume* Construct();

  void SetWorldMaterial(const G4String&);
  void SetTargetMaterial(const G4String&);

  void SetTargetWidth(G4double val);
  void SetTargetHeight(G4double val);
  void SetTargetLength(G4double val);

  void UpdateGeometry();

private:

  HadDetectorConstruction & operator=(const HadDetectorConstruction &right);
  HadDetectorConstruction(const HadDetectorConstruction&);

  G4double pos_x;
  G4double pos_y;
  G4double pos_z;

  G4Material*  targetMaterial;
  G4Material*  worldMaterial;

  G4LogicalVolume* logicTarget;
  G4LogicalVolume* logicWorld;

  HadDetectorMessenger* detectorMessenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

#endif

