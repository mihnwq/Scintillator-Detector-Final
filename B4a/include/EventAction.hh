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
/// \file B4/B4a/include/EventAction.hh
/// \brief Definition of the B4a::EventAction class

#ifndef B4aEventAction_h
#define B4aEventAction_h

#include <G4ThreeVector.hh>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <CLHEP/Units/SystemOfUnits.h>

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4GenericAnalysisManager;
class G4Event;


namespace B4a
{
/// Event action class
///
/// It defines data members to hold the energy deposit and track lengths
/// of charged particles in Absober and Gap layers:
/// - fEnergyAbs, fEnergyGap, fTrackLAbs, fTrackLGap
/// which are collected step by step via the functions
/// - AddAbs(), AddGap()

class EventAction : public G4UserEventAction
{
  public:
    EventAction() = default;
    ~EventAction() override = default;

 //   void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;
    void BeginOfEventAction(const G4Event *anEvent) override;

    void AddAbs(G4double de, G4double dl);
    void AddGap(G4double de, G4double dl);
    void AddVector(G4int , G4int, G4int, G4GenericAnalysisManager* , std::vector<G4ThreeVector> );

   void AddPhoton();
  void AddPhotonDet1();
  void AddPhotonDet2();
   void AddMeuon(G4ThreeVector);

   G4int GetCounter();
   G4int GetCounterDet1();
   G4int GetCounterDet2();
   G4int GetNumberOfMeuons();

    std::pair<G4ThreeVector, G4int> GetMeuonAt(G4int);


  void AddEdep(G4double edep) { fEdep += edep; }

  void InsertMeuonInMap(G4int);
  G4bool VerifyMeuonHasHit(G4int);

  private:
    G4double fEnergyAbs = 0.;
    G4double fEnergyGap = 0.;
    G4double fTrackLAbs = 0.;
    G4double fTrackLGap = 0.;
    G4double fEdep = 0.;

   G4int photonCounter = 0;

   G4int photonCounterDet1 = 0;
   G4int photonCounterDet2 = 0;

   std::vector<std::pair<G4ThreeVector , G4int>> meuonData;

  std::unordered_set<G4int> meuonAlreadyHit;


};

// inline functions

inline void EventAction::AddAbs(G4double de, G4double dl)
{
  fEnergyAbs += de;
  fTrackLAbs += dl;
}

inline void EventAction::AddGap(G4double de, G4double dl)
{
  fEnergyGap += de;
  fTrackLGap += dl;
}

inline void EventAction::AddPhoton() {
  photonCounter++;
}

  inline void EventAction::AddPhotonDet1() {
  photonCounterDet1++;
}

  inline void EventAction::AddPhotonDet2() {
  photonCounterDet2++;
}

  inline G4int EventAction::GetCounterDet1() {
    return photonCounterDet1;
  }

  inline G4int EventAction::GetCounterDet2() {
  return photonCounterDet2;
}


  inline void EventAction::AddMeuon(G4ThreeVector v)
  {
   meuonData.push_back({v,0});
  }

  inline G4int EventAction::GetCounter() {
  return photonCounter;
}

  inline std::pair<G4ThreeVector, G4int> EventAction::GetMeuonAt(G4int i)
  {
  return meuonData.at(i);
 }

  inline G4int EventAction::GetNumberOfMeuons() {
  return meuonData.size();
}

  inline void EventAction::InsertMeuonInMap(G4int trackId) {
    meuonAlreadyHit.insert(trackId);
  }

  inline G4bool EventAction::VerifyMeuonHasHit(G4int trackId) {
  return meuonAlreadyHit.find(trackId) == meuonAlreadyHit.end();
}



}  // namespace B4a
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif