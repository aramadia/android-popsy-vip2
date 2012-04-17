/*-----------------------------------------------------------------+
 |                          GhostEtincelle.cpp                      |
 |                                                                  |
 |  en plus des eclaires, je rajoute des etincelles aux points      |
 |      d'impacts pour donner encore plus de vie a l'ensemble       |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#include "stdafx.h"


#define GRAVITE 525.0f

#define VITESSE_MIN 100.f
#define VITESSE_RND 1000.f

#define LIFE_MIN 1.5f
#define LIFE_RND 2.0f


#define FROTTEMENT .5f


#define PARTI_SIZE 7.f  // taille des particules au bout des etincelles

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |  Class GhostEtincelle |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
GhostEtincelle::GhostEtincelle() {
    vec3_set(Vitesse, 0.0f, 0.0f, 0.0f);
    vec3_set(LastPos, 0.0f, 0.0f, 0.0f);
    vec3_set(Pos, 0.0f, 0.0f, 0.0f);

    life = 0.0f;
    Tlife = 0.0f;

    gereCollision = rand()%2;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostEtincelle::Ini(Ufloat pos[3], Ufloat vitesse[3], Ufloat tlife) {
    vec3_eg(Pos, pos);
    vec3_eg(LastPos, pos);

    vec3_eg(Vitesse, vitesse);

    life = Tlife = tlife;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// calcul position en gerant les collisions si besoin
void GhostEtincelle::CalcPos(Ufloat laptime, MondeU3D* monde) {
    Ufloat      accel[3];
    Ufloat      tmp[3];
    PolygonU3D* poly=NULL;

//  vec3_eg( LastPos, Pos );

    vec3_set(accel, 0.0f, -GRAVITE, 0.0f);
    vec3_mul(accel, accel, laptime);


    gereCollision = ! gereCollision;
    if (gereCollision) {
        vec3_eg(LastPos, Pos);
    }

    vec3_add(Vitesse, Vitesse, accel);
    vec3_mul(tmp, Vitesse, laptime);
    vec3_add(Pos, Pos, tmp);

    if (monde->SegmentIntersecPoly(LastPos, Pos, poly)) {
        vec3_rebond(Vitesse, poly->plan.normal, Vitesse);     // si collision avec le decor alors on calcul la nouvelle vitesse !
        vec3_mul(Vitesse, Vitesse, FROTTEMENT);     // l'etincelle perds de la vitesse avec le rebond
    }
    /*  }
        else
        {
            vec3_add( Vitesse, Vitesse, accel );
            vec3_mul( tmp, Vitesse, laptime );
            vec3_add( Pos, Pos, tmp );
        }*/

    life -= laptime;
}
//----------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +---------------------------+
//                                      |  Class GhostAllEtincelle  |
//                                      +---------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
GhostAllEtincelle::GhostAllEtincelle(U32 nbmax) {
    GhostEtincelle* tmp;

    AllActive = NULL;
    AllInActive = NULL;

    //-------------------- ini et load l'image d'une particul !
    TextureManager::SetRepertoire("");
    partTex = TextureManager::OpenJpg("Datas/meshes/Ghost/etincelle.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    partTex->IncUser();

    partMat = new MaterialU3D("partMat");
    partMat->SetTexture1(partTex);
    partMat->SetTransp(MatTranspAddAlpha);
    partMat->SetShading(MatShadingFlat);
    partMat->Confirm();

    partBmp = new BitmapU3D(partMat);


    while (nbmax) {
        tmp = new GhostEtincelle;
        tmp->suiv = AllInActive;
        AllInActive = tmp;

        nbmax--;
    }

}
//----------------------------------------------------------------------------------------------------------------------------------------
GhostAllEtincelle::~GhostAllEtincelle() {
    GhostEtincelle* tmp;

    //------------------ KIll image de la particul
    delete partBmp;
    delete partMat;
    TextureManager::SubUse(partTex);


    while (AllActive) {
        tmp = AllActive->suiv;
        delete AllActive;
        AllActive = tmp;
    }

    while (AllInActive) {
        tmp = AllInActive->suiv;
        delete AllInActive;
        AllInActive = tmp;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostAllEtincelle::GenereEtincelle(Ufloat pos[3], Ufloat dir[3], U32 nb) {
    Ufloat          rnddir[3];
    GhostEtincelle* tmp,*tmp1;

    tmp = AllInActive;

    if (tmp) {
        while (nb) {
            AllInActive = AllInActive->suiv;

            //-------- met des variations aléatoires de direction
            vec3_eg(rnddir, dir);
            vec3_mul(rnddir, rnddir, .8f);
            rnddir[0] = rnddir[0]*Random1() -  rnddir[0]*.5f;
            rnddir[1] = rnddir[1]*Random1() -  rnddir[1]*.5f;
            rnddir[2] = rnddir[2]*Random1() -  rnddir[2]*.5f;

            vec3_add(rnddir, rnddir, dir);
            vec3_mul(rnddir, rnddir, VITESSE_MIN + VITESSE_RND*Random1());

            tmp->Ini(pos, rnddir, LIFE_MIN + LIFE_RND*Random1());

            tmp1 = tmp->suiv;
            tmp->suiv = AllActive;
            AllActive = tmp;
            tmp = tmp1;
            if (! tmp) break;

            nb--;
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostAllEtincelle::UpdateFrame(Ufloat laptime, MondeU3D* monde) {
    GhostEtincelle* tmp;
    GhostEtincelle* preced=NULL;


    //----------- calcul les nouvelles positions
    tmp = AllActive;
    while (tmp) {
        if (tmp->IsFinish()) {   // si finit alors l'enleve de la liste des actifs
            if (preced) {
                preced->suiv = tmp->suiv;

                tmp->suiv = AllInActive;
                AllInActive = tmp;

                tmp = preced->suiv;

            } else {
                AllActive = tmp->suiv;

                tmp->suiv = AllInActive;
                AllInActive = tmp;

                tmp = AllActive;
            }
        } else {
            tmp->CalcPos(laptime, monde);

            preced = tmp;
            tmp = tmp->suiv;
        }

    }

}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostAllEtincelle::AffOneParti(Ufloat pos[3], Ufloat alpha) {
    Camera*             cam = U3D3Monde3D->GetActualCamera();

//  if( !Monde->SegmentIntersecPoly( cam->GetPos(), pos) )
    {
        CoordU3D    tmpc;
        Ufloat      size;

        vec3_eg(tmpc.trans, pos);
        cam->ProjetUni(&tmpc, 0);

        if (!(tmpc.ClipInfo&ClipProche)) {
            size = PARTI_SIZE * tmpc.ecran[3] * .5f;

            partMat->SetAlpha(alpha);

            partBmp->SetCoord(0, tmpc.ecran[0] - size, tmpc.ecran[1] - size, tmpc.ecran[2]);
            partBmp->SetCoord(1, tmpc.ecran[0] + size, tmpc.ecran[1] - size, tmpc.ecran[2]);
            partBmp->SetCoord(2, tmpc.ecran[0] + size, tmpc.ecran[1] + size, tmpc.ecran[2]);
            partBmp->SetCoord(3, tmpc.ecran[0] - size, tmpc.ecran[1] + size, tmpc.ecran[2]);

            partBmp->Affiche();
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostAllEtincelle::Affiche() {
    GhostEtincelle* tmp;
    CoordU3D        pt1,pt2;
    Ufloat          RVBA[2][4];
    Ufloat          pos[3];

    //----------- affiche le tout
    U3D3Pipeline->Begin(MatTranspAddAlpha, FALSE, TRUE);

    tmp = AllActive;
    while (tmp) {
        pt1.frame_process = -1;
        pt2.frame_process = -1;

        vec3_eg(pt1.trans, tmp->GetPos());
        vec3_eg(pt2.trans, tmp->GetLastPos());
        vec4_set(RVBA[0], 1.0f, 1.0f, 0.5f, tmp->GetAlpha());
        vec4_set(RVBA[1], 1.0f, 0.2f, 0.1f, tmp->GetAlpha());

        U3D3Pipeline->AfficheLigneFromEngine(&pt1, &pt2, RVBA);

        tmp = tmp->suiv;
    }

    U3D3Pipeline->End(NULL);


    //----------------- affiche particules
    U3D3Pipeline->Begin(partMat, FALSE, TRUE);

    tmp = AllActive;
    while (tmp) {
        Ufloat alpha = tmp->GetAlpha();

        vec3_eg(pos, tmp->GetPos());
        AffOneParti(pos, alpha);

        tmp = tmp->suiv;
    }

    U3D3Pipeline->End(partMat);


}
//----------------------------------------------------------------------------------------------------------------------------------------
