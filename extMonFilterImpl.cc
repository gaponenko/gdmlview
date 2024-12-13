#include "TGeoNode.h"
#include "TGeoVolume.h"
#include "Rtypes.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void extMonFilterImpl( TGeoNode * node, int depth ) {

  std::string currentName(node->GetVolume()->GetName());

  // Volumes will be made invisible if their name contains one
  // of these strings.
  static std::vector<string> substrings  { "CRS", "ExtShield", "Ceiling",
    "backfill", "dirt", "concrete", "VirtualDetector", "CRV", "CRS",
    "HatchLid", "Door", "Chiller",
    "PTM", /* The device between the PS and the beam dump */
    "PSVacVessel", "PSCoil", "PSShield"
  };

  // Volumes with these material names will be made invisible.
  static std::vector<string> materials { "MBOverburden", "CONCRETE" };

  if ( depth < 2 ){
    node->SetVisibility(kFALSE); // do not visualize HallAir
  } else{
    bool vis = true;
    for ( auto const& substring : substrings ){
      if ( currentName.find(substring) != string::npos ){
        vis = false;
        node->SetVisibility(kFALSE);
        break;
      }
    }
    if ( vis ){
      string material(node->GetVolume()->GetMaterial()->GetName());
      for ( auto const& mat : materials ){
        if ( material.find(mat) != string::npos ){
          vis = false;
          node->SetVisibility(kFALSE);
          break;
        }
      }
    }
    if ( currentName == "ExtMonDetectorRoom" ){
      cout << "Have: " << currentName << endl;
      vis = true;
      node->SetVisibility(kTRUE);
      node->GetVolume()->VisibleDaughters(kFALSE);
      node->GetVolume()->SetTransparency(63);
      node->GetVolume()->SetLineColor(kRed);
    }
    if ( (currentName == "ExtMonFNALColl2Shielding") || (currentName == "ProtonBeamDumpFront") ){
      cout << "Have: " << currentName << endl;
      vis = true;
      node->SetVisibility(kTRUE);
      node->GetVolume()->VisibleDaughters(kFALSE);
      node->GetVolume()->SetTransparency(63);
      node->GetVolume()->SetLineColor(kGray+1);
    }
    if ( currentName == "ProductionTargetMother" ){
      cout << "Have: " << currentName << endl;
      vis = true;
      node->GetVolume()->VisibleDaughters(kFALSE);
      node->SetVisibility(kTRUE);
      node->GetVolume()->SetTransparency(63);
      node->GetVolume()->SetLineColor(kRed+1);
    }
    /*
    if ( !vis ){
      auto l1 = currentName.find("ExtMon");
      auto l2 = currentName.find("extMon");
      if ( l1 != string::npos && l2 !=string::npos ){
          vis = true;
          node->SetVisibility(kTRUE);
          cout << "Saved" << currentName << endl;
      }
    }
    */
  }

  // Descend recursively into each daughter TGeoNode.
  int ndau = node->GetNdaughters();
  for ( int i=0; i<ndau; ++i ){
    TGeoNode * dau = node->GetDaughter(i);
    extMonFilterImpl(dau, depth+1);
  }

}
