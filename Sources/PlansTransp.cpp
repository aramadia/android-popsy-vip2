/*---------------------------------------------------------------------+
 |                          PlansTransp.cpp                             |
 |                                                                      |
 |  Affichage de la scene principale en additif par dessus tout le reste|
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#include "stdafx.h"



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |  Class PlansTransp    |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
PlansTransp::PlansTransp(MondeU3D* monde) {
    Monde = monde;

    cam = new Camera("Yap");
    Monde->AddElem(cam);

    Transp = 0.0f;
    GotoTransp = 0.0f;
    IncTransp = 0.0f;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTransp::SetTransp(Ufloat tr, Ufloat time) {
    GotoTransp = tr;
    IncTransp = (GotoTransp - Transp) / time;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansTransp::Affiche(Ufloat laptime) {
    MaterialU3D* matNoAdd;
    Camera*      preced;

    //---------------------- Calcul Transparence
    Transp += laptime * IncTransp;
    if (IncTransp>=0.0f) {
        if (Transp > GotoTransp)    Transp = GotoTransp;
    } else {
        if (Transp < GotoTransp)    Transp = GotoTransp;
    }

    if (Transp < 0.01f) return;


    ClearOpenGLZbuffer();   // pas d'interférence entre les 2 scenes


    //---------------------- Modifie les paramètres de transparence des materials
    for (Monde->IniMatIterator(); Monde->GetMatFromIterator(); Monde->IncMatIterator()) {
        if (strcmp(Monde->GetMatFromIterator()->GetNom(), "PAROIE")==0) {
            matNoAdd = Monde->GetMatFromIterator();
            matNoAdd->SetTransp(MatTranspAddAlpha);
            matNoAdd->Confirm();
        }

        Monde->GetMatFromIterator()->SetAlpha(Transp);
    }

    preced = Monde->GetActualCamera();   // sauve camera precedente
    Monde->SetActualCamera(cam);     // met camera voulue
    cam->SetPos(preced->GetTarg());
    cam->SetTarg(preced->GetPos());

    Monde->UpdateFrame(0.0f);

    Monde->SetActualCamera(preced);     // op remet l'ancienne

    //---------------------- Remet les materials a leur ancienne valeur
    matNoAdd->SetTransp(MatTranspRien);
    matNoAdd->Confirm();

    for (Monde->IniMatIterator(); Monde->GetMatFromIterator(); Monde->IncMatIterator())
        Monde->GetMatFromIterator()->SetAlpha(1.0f);

}
//----------------------------------------------------------------------------------------------------------------------------------------


