//
// ************************
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
// ************************
//
//
/// \file B4/B4a/src/SteppingAction.cc
/// \brief Implementation of the B4a::SteppingAction class

#include "SteppingAction.hh"

#include <G4OpticalPhoton.hh>
#include <unordered_set>

#include "DetectorConstruction.hh"
#include "EventAction.hh"
//#include "EventCounter.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "globals.hh"

#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"



using namespace B4;
namespace B4a {
  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



  SteppingAction::SteppingAction(const DetectorConstruction* detConstruction)
  : fDetConstruction(detConstruction)
  {}


  //EventCounter photonCpunter;



  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  void SteppingAction::UserSteppingAction(const G4Step* step)
  {

    auto fEventAction =
    static_cast<EventAction*>(
      G4EventManager::GetEventManager()->GetUserEventAction()
    );

    if (!fEventAction) return; // safety

    auto vol = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  //  auto vol = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();


    //if (!vol) return;

    G4String volName = vol->GetName();

    //G4cout<<"Volume "<<volName<<"\n";

   // G4StepPoint* post = step->GetPostStepPoint();

    //  G4VPhysicalVolume* prePV  = pre->GetPhysicalVolume();
   // G4VPhysicalVolume* postPV = post->GetPhysicalVolume();




    if ((step->GetTrack()->GetDefinition() == G4MuonPlus::MuonPlusDefinition() ||
        step->GetTrack()->GetDefinition() == G4MuonMinus::MuonMinusDefinition())
        && step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary)
    {

      auto volume = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();

      if (!volume)
        return;

      if (volume->GetName() == "physWorld")
        return;

      G4int trackID = step->GetTrack()->GetTrackID();


      G4cout<<"Found meuon! "<<G4endl;


     // G4cout<<"Has meuon hit? "<<fEventAction->VerifyMeuonHasHit(trackID) << G4endl;

      if (fEventAction->VerifyMeuonHasHit(trackID))
      {
        G4cout<<"Has meuon hit? "<<fEventAction->VerifyMeuonHasHit(trackID) << G4endl;
        fEventAction->InsertMeuonInMap(trackID);
        G4ThreeVector pos = step->GetPostStepPoint()->GetPosition(); ///This too
        fEventAction->AddMeuon(pos);

        G4cout<<"possss " << pos << G4endl;
      }

    }else if (step->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
    {

      G4StepPoint* post = step->GetPostStepPoint();

      //  G4VPhysicalVolume* prePV  = pre->GetPhysicalVolume();
      G4VPhysicalVolume* postPV = post->GetPhysicalVolume();

      if (post->GetStepStatus() == fGeomBoundary)
      {
        G4String volume = postPV->GetName();
        if (volume == "physCapDet1" || volume == "physCapDet2")
        {
          G4double energy = step->GetTrack()->GetKineticEnergy();
          G4ThreeVector startingPosition = step->GetTrack()->GetVertexPosition();
          G4ThreeVector endingPosition = step->GetTrack()->GetPosition();

          const G4Track* photon = step->GetTrack();
          G4int parentID = photon->GetParentID();


          fEventAction->AddPhoton();


          step->GetTrack()->SetTrackStatus(fStopAndKill);
        }

      }
    }


  }

  //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
}  // namespace B4a