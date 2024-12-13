#include "extMonFilterImpl.cc+"

void extMonFilter(const char* fn = "mu2e.gdml") {

  TGeoManager *geom = TGeoManager::Import(fn);
  if(geom) {

    TGeoNode *top = geom->GetTopNode();
    extMonFilterImpl(top,0);

    gEnv->SetValue("Viewer3D.DefaultDrawOption", "ogl");
    TBrowser *b       = new TBrowser();

    geom->GetVolume("HallAir")->Draw();
  }
}
