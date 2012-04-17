/*-----------------------------------------------------------------+
 |                      CoupleRPluie.cpp                            |
 |                                                                  |
 |   une classe pour gérer les rebond de la pluie sur le couple     |
 |  ( enfin un truc ki simule :) inspiré de Metal Gear Solid2 )     |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#include "stdafx.h"


#define TOLERANCE_ANGLE .4f

#define MIN_SIZE_BRUINE 1.f
#define RND_SIZE_BRUINE 5.f

#define TIME_LIFE_BRUINE_MIN .2f
#define TIME_LIFE_BRUINE_RND .3f


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |  Class BruineParticul |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
BruineParticul::BruineParticul() {
    parent = NULL;

    vec3_set(Pos, 0.0f, 0.0f, 0.0f);
    vitessDep = 0.0f;

    size = 0.0f;
    incsize = 0.0f;

    alpha = 0.0f;
    incalpha = 0.0f;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void BruineParticul::Genere(BruineEmet* emet) {
    parent = emet;

    vec3_eg(Pos, emet->pos);
    vitessDep = 0.0f/*2.f + 5.f*Random1()*/;

    size = MIN_SIZE_BRUINE + RND_SIZE_BRUINE*Random1();
    incsize  = 1.f+Random1()*3.5f;

    alpha = .9f + .1f*Random1();
    incalpha = -alpha / (TIME_LIFE_BRUINE_MIN + TIME_LIFE_BRUINE_RND*Random1());
}
//----------------------------------------------------------------------------------------------------------------------------------------
void BruineParticul::Update(Ufloat laptime) {
    Ufloat addvec[3];

    vec3_mul(addvec, parent->dir, vitessDep*laptime);
    vec3_add(Pos, Pos, addvec);

    size += incsize*laptime;

    alpha += incalpha*laptime;
    if (alpha <0.0f) alpha = 0.0f;
}
//----------------------------------------------------------------------------------------------------------------------------------------






//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |  Class CoupleRPluie   |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
CoupleRPluie::CoupleRPluie(MondeU3D* monde) {
    Monde = monde;

    AllEmet = NULL;
    nextEmet = NULL;

    //-------------------- ini et load l'image de particul
    TextureManager::SetRepertoire("");
    partTex = TextureManager::OpenJpg("Datas/meshes/Couple/Bruine.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    partTex->IncUser();

    partMat = new MaterialU3D("partMat");
    partMat->SetTexture1(partTex);
    partMat->SetTransp(MatTranspAddAlpha);
    partMat->SetShading(MatShadingFlat);
    partMat->Confirm();

    partBmp = new BitmapU3D(partMat);


    //-------------- va rechercher tout les vertex ki pourront émettre del a bruine
    NbGenerators = 0;
    for (Monde->IniObjIterator(); Monde->GetObjFromIterator(); monde->IncObjIterator()) {
        ObjetU3D* obj = Monde->GetObjFromIterator();

        for (U32 a=0; a<obj->nbcoords; a++)
            AddGeneratorIfGood(obj, &obj->Ctab[a]);
    }

    AllBruinePart = new BruineParticul[NbGenerators];


    //-------------- initialise le rendu
    BruineParticul* tmpe = AllBruinePart;
    for (U32 a=0; a<NbGenerators; a++,tmpe++)
        Genere(tmpe);

}
//----------------------------------------------------------------------------------------------------------------------------------------
CoupleRPluie::~CoupleRPluie() {
    BruineEmet* tmpe;

    delete [] AllBruinePart;

    while (AllEmet) {
        tmpe = AllEmet->suiv;
        delete AllEmet;
        AllEmet = tmpe;
    }

    //------------------ KIll image de particul
    delete partBmp;
    delete partMat;
    TextureManager::SubUse(partTex);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleRPluie::AddGeneratorIfGood(ObjetU3D* obj, CoordU3D* coord) {
    Ufloat vecDir[3]    = {0.0f, -1.0f, 0.0f};
    Ufloat vecDest[3];


    obj->GetObjToWorld().GetTransfoR(vecDest, coord->normale);

    Ufloat res = vec3_dot(vecDest, vecDir);

    if (res<(-1.f+TOLERANCE_ANGLE)) {    // le vertex conviens alors il pourra balancer des particules
        BruineEmet* tmp = new BruineEmet;

        vec3_eg(tmp->dir, vecDest);
        obj->GetObjToWorld().GetTransfo(tmp->pos, coord->origine);

        tmp->suiv = AllEmet;
        AllEmet = tmp;
        NbGenerators++;
    }

}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleRPluie::Genere(BruineParticul* bpart) {
    U32 a;

    if (!nextEmet)  nextEmet = AllEmet;


    //------------- trouve un nouvel émetteur de bruine
    a = 1/* + rand()%2*/;
    while (a) {
        nextEmet = nextEmet->suiv;
        if (!nextEmet) nextEmet = AllEmet;

        a--;
    }

    if (!bpart->GetParent())
        bpart->Genere(nextEmet);
    else
        bpart->Genere(bpart->GetParent());
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleRPluie::AffOnePart(BruineParticul* bpart) {
    Camera*     cam = Monde->GetActualCamera();
    CoordU3D    tmpc;
    Ufloat      size;

    vec3_eg(tmpc.trans, bpart->GetPos());
    cam->ProjetUni(&tmpc, 0);

    if (!(tmpc.ClipInfo&ClipProche)) {
        size = bpart->GetSize() * tmpc.ecran[3] * .5f;

        partMat->SetAlpha(bpart->GetAlpha());

        partBmp->SetCoord(0, tmpc.ecran[0] - size, tmpc.ecran[1] - size, tmpc.ecran[2]-4.f);
        partBmp->SetCoord(1, tmpc.ecran[0] + size, tmpc.ecran[1] - size, tmpc.ecran[2]-4.f);
        partBmp->SetCoord(2, tmpc.ecran[0] + size, tmpc.ecran[1] + size, tmpc.ecran[2]-4.f);
        partBmp->SetCoord(3, tmpc.ecran[0] - size, tmpc.ecran[1] + size, tmpc.ecran[2]-4.f);

        partBmp->Affiche();
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleRPluie::UpdateFrame(Ufloat laptime) {
    BruineParticul* tmpb;

    //------------- update les déplacements de la bruine
    tmpb = AllBruinePart;
    for (U32 a=0; a<NbGenerators; a++,tmpb++) {
        tmpb->Update(laptime);
        if (tmpb->IsFinish())   Genere(tmpb);
    }

    //------------ affiche le tout
    U3D3Pipeline->Begin(partMat, FALSE, TRUE);

    tmpb = AllBruinePart;
    for (U32 a=0; a<NbGenerators; a++,tmpb++)
        AffOnePart(tmpb);

    U3D3Pipeline->End(partMat);
}
//----------------------------------------------------------------------------------------------------------------------------------------
