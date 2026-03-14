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
/// \file B4/B4a/src/EventAction.cc
/// \brief Implementation of the B4a::EventAction class

/*#include "EventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include <iomanip>

#include "EventCounter.hh"
#include "PhotonMuonHelper.hh"
extern PhotonMuonHelper gPhotonMuonHelper;
namespace B4a
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void EventAction::BeginOfEventAction(const G4Event* event)
{
  // initialisation per event
  fEnergyAbs = 0.;
  fEnergyGap = 0.;
  fTrackLAbs = 0.;
  fTrackLGap = 0.;

  //Only needed in the event we should reset the variables after each event.
  EventCounter::BeginEvent();

  gPhotonMuonHelper.Clear();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{

  auto analysisManager = G4AnalysisManager::Instance();


  analysisManager->FillH1(0, fEnergyAbs);
  analysisManager->FillH1(1, fEnergyGap);
  analysisManager->FillH1(2, fTrackLAbs);
  analysisManager->FillH1(3, fTrackLGap);


  analysisManager->FillNtupleDColumn(0,0, 500);
  analysisManager->FillNtupleDColumn(0,1, 800);
  analysisManager->FillNtupleDColumn(0,2, 500);
  analysisManager->FillNtupleDColumn(0,3, 500);


  G4int photonCount = EventCounter::GetCounter();

  EventCounter::AddPhotonCOunt(photonCount);


  for (G4int i = 0; i < EventCounter::GetNumberOfEvents(); i++)
  {
    analysisManager->FillNtupleDColumn(0,4, EventCounter::GetCountAt(i));
    analysisManager->AddNtupleRow(0);
  }

//  G4cout<<"Are this many Photons this run:" << EventCounter::GetCounter()<<G4endl;


  analysisManager->AddNtupleRow(0);
 // AddVector(1, 2, 3, analysisManager, photonStartingPositions);

    auto eventID = event->GetEventID();


    for (G4int i = 0; i < EventCounter::GetTotalMeuons(); i++)
    {

        EventCounter::SetEventAt(eventID, i);

        analysisManager->FillNtupleIColumn(1, 0, eventID);     // event ID
        analysisManager->FillNtupleDColumn(1, 1, EventCounter::GetMeuonAT(i).first.x());
        analysisManager->FillNtupleDColumn(1, 2, EventCounter::GetMeuonAT(i).first.y());
        analysisManager->FillNtupleDColumn(1, 3, EventCounter::GetMeuonAT(i).first.z());

        analysisManager->AddNtupleRow(1);
    }


  // Print per event (modulo n)
  //

  auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ((printModulo > 0) && (eventID % printModulo == 0)) {

  }


}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

}  // namespace B4a
*/
#include "EventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Event.hh"

namespace B4a
{

  void EventAction::BeginOfEventAction(const G4Event*)
  {
    fEnergyAbs = 0.;
    fEnergyGap = 0.;
    fTrackLAbs = 0.;
    fTrackLGap = 0.;

    photonCounter = 0;

    photonCounterDet1 = 0;
    photonCounterDet2 = 0;

    meuonData.clear();
    meuonAlreadyHit.clear();
  }

  void EventAction::EndOfEventAction(const G4Event* event)
  {
    auto analysisManager = G4AnalysisManager::Instance();

    // =========================
    // NTUPLE 0 — ONE ROW / EVENT
    // =========================
    analysisManager->FillNtupleDColumn(0, 0, fEnergyAbs);
    analysisManager->FillNtupleDColumn(0, 1, fEnergyGap);
    analysisManager->FillNtupleDColumn(0, 2, fTrackLAbs);
    analysisManager->FillNtupleDColumn(0, 3, fTrackLGap);
    analysisManager->FillNtupleIColumn(0, 4, GetCounter());
    //G4cout << "Number of photons from det1 " << GetCounterDet1() << G4endl;
    analysisManager->FillNtupleIColumn(0, 5, GetCounterDet1());

    G4cout<<"Number of photons this run: " << GetCounter() << G4endl;
    analysisManager->AddNtupleRow(0);


    G4int eventID = event->GetEventID();

    for (G4int i = 0; i < GetNumberOfMeuons(); i++)
    {
      analysisManager->FillNtupleIColumn(1, 0, eventID);
      analysisManager->FillNtupleDColumn(1, 1, GetMeuonAt(i).first.x());
      analysisManager->FillNtupleDColumn(1, 2, GetMeuonAt(i).first.y());
      analysisManager->FillNtupleDColumn(1, 3, GetMeuonAt(i).first.z());

      analysisManager->AddNtupleRow(1);
    }

    //G4cout << "End of event " << event->GetEventID() << G4endl;

  }

} // namespace B4a

