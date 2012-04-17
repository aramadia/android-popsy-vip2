/*---------------------------------------------------------------------+
 |                          PlansClock.cpp                              |
 |                                                                      |
 |  C'est l'animation de toute cette jolie horloge "I Tek" (se prononce |
 |   aille tèque ) Hard codé ke l'on retouve  dans cette partie         |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/


#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class PlansClock |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
PlansClockAnim::PlansClockAnim(MondeU3D* monde) {
    U32 objnum = 0;

    Monde = monde;

    AllElem = NULL;

    for (monde->IniObjIterator(); monde->GetObjFromIterator(); monde->IncObjIterator(),objnum++) {
        // vois si l'objet est bon pour le service, si oui l'ajoute a la liste
        if (monde->GetObjFromIterator()->Ptab[0].m->GetTrTex1() == MatTranspMulAlpha)
            AddObj(monde->GetObjFromIterator(), objnum&0x1, 1.25f+.4f*Random1());
        else if (strcmp(monde->GetObjFromIterator()->Ptab[0].m->GetNom(), "APE") == 0)
            AddObj(monde->GetObjFromIterator(), TRUE, 3.14f);
    }

    intensBouge = 0.0f;

}
//----------------------------------------------------------------------------------------------------------------------------------------
PlansClockAnim::~PlansClockAnim() {
    PlansClockElem* tmp;

    while (AllElem) {
        tmp = AllElem->suiv;
        delete AllElem;
        AllElem = tmp;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansClockAnim::AddObj(ObjetU3D* obj, bool posAngle, Ufloat speed) {
    PlansClockElem* tmp;

    tmp = new PlansClockElem;
    tmp->obj= obj;
    tmp->speedRotate = speed;
    if (posAngle) tmp->speedRotate = -tmp->speedRotate;

    tmp->suiv = AllElem;
    AllElem = tmp;

}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansClockAnim::Anim(Ufloat ptime) {
    PlansClockElem* tmp;
    Quat            q;
    Ufloat          depY;

    depY = intensBouge * 50.f /** Sin( ptime*10.f )*/;

    tmp = AllElem;
    while (tmp) {
        q.SetFromAxis(0.0f, 0.0f, 1.0f, ptime*tmp->speedRotate);

        tmp->obj->GetObjToWorld().SetPos(0.0f, depY*tmp->speedRotate*Random1(), 0.0f);
        q.matrix(tmp->obj->GetObjToWorld());
        tmp->obj->GetObjToWorld() *= tmp->obj->GetObjMatrix();
        tmp->obj->Transform(tmp->obj->GetObjToWorld());

        tmp = tmp->suiv;
    }

    intensBouge += -intensBouge / 4.f;
}
//----------------------------------------------------------------------------------------------------------------------------------------

