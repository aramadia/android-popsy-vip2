/*---------------------------------------------------------+
 |                    ColonneAnim.cpp                       |
 |                                                          |
 |     va gérer l'animation de cette grandiose colonne      |
 |                                                          |
 |  U2^PoPsy TeAm 2000                                  |
 +--------------------------------------------------------*/

#include "stdafx.h"

#define INER_BONE 500.f
#define OUTER_BONE 1200.f

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |  Class ColonneAnim    |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
ColonneAnim::ColonneAnim(MondeU3D* monde) {
    ObjetU3D* obj;

    monde->IniObjIterator();
    obj = monde->GetObjFromIterator();

    obj->SetRecalcFlag(ObjetU3D::culling2d);     // permet seulement le recalcul des normales de poly ( pour le backface culling )

    bone[0] = new OneObjBones(OUTER_BONE, INER_BONE);
    bone[0]->SetPos(obj->GetObjToWorld().GetXpos(), 2070.f, obj->GetObjToWorld().GetZpos());

    bone[1] = new OneObjBones(OUTER_BONE, INER_BONE);
    bone[1]->SetPos(obj->GetObjToWorld().GetXpos(), 1150.f, obj->GetObjToWorld().GetZpos());

    /*  bone[2] = new OneObjBones( 700.f, 400.f );
        bone[2]->SetPos( obj->GetObjToWorld().GetXpos(), 600.f, obj->GetObjToWorld().GetZpos() );*/

    bone[2] = new OneObjBones(OUTER_BONE, INER_BONE);
    bone[2]->SetPos(obj->GetObjToWorld().GetXpos(), 220.f, obj->GetObjToWorld().GetZpos());

    /*  bone[4] = new OneObjBones( 700.f, 400.f );
        bone[4]->SetPos( obj->GetObjToWorld().GetXpos(), -250.f, obj->GetObjToWorld().GetZpos() );*/

    bone[3] = new OneObjBones(OUTER_BONE, INER_BONE);
    bone[3]->SetPos(obj->GetObjToWorld().GetXpos(), -1190.f, obj->GetObjToWorld().GetZpos());

    /*  bone[6] = new OneObjBones( 700.f, 400.f );
        bone[6]->SetPos( obj->GetObjToWorld().GetXpos(), -1670.f, obj->GetObjToWorld().GetZpos() );*/

    bone[4] = new OneObjBones(OUTER_BONE, INER_BONE);
    bone[4]->SetPos(obj->GetObjToWorld().GetXpos(), -2030.f, obj->GetObjToWorld().GetZpos());

    /*  bone1 = new OneObjBones( 1000.f, 500.f );
        bone1->SetPos( obj->GetObjToWorld().GetXpos(), 1670.f, obj->GetObjToWorld().GetZpos() );

        bone2 = new OneObjBones( 1000.f, 500.f );
        bone2->SetPos( obj->GetObjToWorld().GetXpos(), 1170.f, obj->GetObjToWorld().GetZpos() );

        bone3 = new OneObjBones( 1000.f, 500.f );
        bone3->SetPos( obj->GetObjToWorld().GetXpos(), 670.f, obj->GetObjToWorld().GetZpos() );

        bone4 = new OneObjBones( 1000.f, 500.f );
        bone4->SetPos( obj->GetObjToWorld().GetXpos(), -170.f, obj->GetObjToWorld().GetZpos() );*/

    for (U32 a=0; a<5; a++)
        obj->AddBones(bone[a]);
    obj->ConfirmBones();
}
//----------------------------------------------------------------------------------------------------------------------------------------
ColonneAnim::~ColonneAnim() {
}
//----------------------------------------------------------------------------------------------------------------------------------------
void ColonneAnim::Anim(Ufloat time, Ufloat laptime) {
    Ufloat      vec[3];

    for (U32 a=0; a<5; a++) {
        vec3_set(vec, 0.5f*sin(time*.4f + a*2.f), 1.0f, 0.5f*cos(time*.6f + a*2.3f +.416f));
        Quat q(vec, M_PI*.45f * sin(time*1.91f + a));

        q.matrix(bone[a]->GetLocaMatrix());
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
