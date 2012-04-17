/*---------------------------------------------------------------------+
 |                          PlansAnim.cpp                               |
 |                                                                      |
 |      va faire bouger en hard coding tout ces merveilleux plans       |
 |      ( PS : j'aime kan un plan se deroule sans accros ( Annibal ) )  |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#include "stdafx.h"


#define ZMAX    22811.f
#define ZMIN    -7774.f

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class PlansAnim  |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
PlansAnim::PlansAnim(MondeU3D* monde) {
    U32 objnum = 0;

    AllElem  = NULL;

    for (monde->IniObjIterator(); monde->GetObjFromIterator(); monde->IncObjIterator(),objnum++) {
        if (monde->GetObjFromIterator()->Ptab[0].m->GetTrTex1() != MatTranspRien)
            AddObj(monde->GetObjFromIterator(), objnum&0x1, 1000.f + 1800.f*Random1());

    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
PlansAnim::~PlansAnim() {
    PlansElem* tmp;

    while (AllElem) {
        tmp = AllElem->suiv;
        delete AllElem;
        AllElem = tmp;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansAnim::AddObj(ObjetU3D* obj, bool posDir, Ufloat speed) {
    PlansElem* tmp;

    tmp = new PlansElem;
    tmp->obj= obj;
    tmp->zpos = obj->GetObjToWorld().GetZpos();
    tmp->speedAvance = speed;
    if (posDir) tmp->speedAvance = -tmp->speedAvance;

    tmp->suiv = AllElem;
    AllElem = tmp;

}
//----------------------------------------------------------------------------------------------------------------------------------------
void PlansAnim::Anim(Ufloat laptime) {
    PlansElem* tmp;

    tmp = AllElem;
    while (tmp) {
        tmp->zpos += laptime*tmp->speedAvance;

        tmp->obj->GetObjToWorld().SetZPos(tmp->zpos);
        tmp->obj->Transform(tmp->obj->GetObjToWorld());

        if (tmp->zpos> ZMAX)
            tmp->zpos = ZMIN;
        else if (tmp->zpos< ZMIN)
            tmp->zpos = ZMAX;

        tmp = tmp->suiv;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
