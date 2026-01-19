#ifndef B4aSteppingAction_h
#define B4aSteppingAction_h

#include "G4UserSteppingAction.hh"
#include "globals.hh"

namespace B4 {
  class DetectorConstruction;
}

namespace B4a {

  class SteppingAction : public G4UserSteppingAction
  {
  public:
    // ❌ REMOVE EventAction* FROM CONSTRUCTOR
    SteppingAction(const B4::DetectorConstruction* detConstruction);
    ~SteppingAction() override = default;

    void UserSteppingAction(const G4Step* step) override;

  private:
    const B4::DetectorConstruction* fDetConstruction = nullptr;
    // ❌ REMOVE EventAction* MEMBER
  };

}  // namespace B4a

#endif

