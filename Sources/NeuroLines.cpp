/*---------------------------------------------------------------------+
 |                          NeuroLines.cpp                              |
 |                                                                      |
 |  classe ki consitera a afficher des lignes ki ricocherons sur la     |
 |                  mesh pour faire un wiiiz effect                     |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#include "stdafx.h"


#ifdef _DEBUG
#define NB_LINE 30
#else
#define NB_LINE 80
#endif


#define TIME_DISAPPEAR 1.f

#define PART_SIZE 200   // taille des particules


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |   Struct NeuroOneLine |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
NeuroOneLine::NeuroOneLine() {
    ligne = new HSplinaeAff;
    IsFinish = FALSE;
    TimeToGo = 0.0f;
    TotalTime = 0.0f;
}
//----------------------------------------------------------------------------------------------------------------------------------------
NeuroOneLine::~NeuroOneLine() {
    delete ligne;
}
//----------------------------------------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |   Class NeuroLines    |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
NeuroLines::NeuroLines(MondeU3D* world, UImage* part) {
    monde = world;

    partMat = new MaterialU3D("tmp");
    partMat->SetTexture1(part->GetMaterial()->GetTex1());
    partMat->SetTransp(MatTranspAddAlpha);
    partBmp  = new BitmapU3D(partMat);
    partPos  = new Ufloat[ NB_LINE*4 ];

    AllLines = new NeuroOneLine[ NB_LINE ];

//  LightU3D *tmpl = (LightU3D*)world->GetLight( "MainL" )->GetPtr();
    LightU3D* tmpl = (LightU3D*)world->GetLight("MainL");
    tmpl->GetPos(PosDep);     // ini
    Rayonmax = tmpl->GetOuter()*2.f;   // rayon maxi ke le rayon atteindra !

    for (U32 a=0; a<NB_LINE; a++)
        GenereLine(&AllLines[a]);
}
//----------------------------------------------------------------------------------------------------------------------------------------
NeuroLines::~NeuroLines() {
    delete partMat;
    delete partBmp;
    delete [] partPos;
    delete [] AllLines;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void NeuroLines::GenereLine(NeuroOneLine* sp) {
    bool        sortie=FALSE;
    Ufloat      vecR[3];
    Ufloat      lastPos[3];
    Ufloat      nextPos[3];
    Ufloat      Ntime=0.0f;
    Ufloat      iter = 1.0f;
    PolygonU3D* pIntersec=NULL;
    U32         nbligne=1;

    //---------- génère une direction aléatoire
    vec3_set(vecR, Random1()*2.f - 1.f, Random1()*2.f - 1.f, Random1()*2.f - 1.f);
    vec3_normalize(vecR);

    sp->ligne->AddKey(0.0f);
    sp->ligne->SetKeyPos(PosDep[0], PosDep[1], PosDep[2]);
    sp->ligne->SetKeyColor(1.0f, 1.0f, 1.0f);


    vec3_eg(lastPos, PosDep);
    vec3_mul(nextPos, vecR, Rayonmax);
    vec3_add(nextPos, nextPos, lastPos);

    while (!sortie) {
        Ntime += 0.8f + .5f*Random1();
//      Ntime += 1.0f + .5f*Random1();   // !!!!!!!!! TEMPORAIRE paskeu j'ai oublié ma new liiib !!!!!!!

        pIntersec = NULL;
        if (monde->SegmentIntersecPoly(lastPos, nextPos, pIntersec)) {
            vec3_sub(nextPos, nextPos, lastPos);
            vec3_mul(nextPos, nextPos, 0.7f);
            vec3_add(nextPos, lastPos, nextPos);


            sp->ligne->AddKey(Ntime);
            sp->ligne->SetKeyPos(nextPos[0], nextPos[1], nextPos[2]);

            vec3_eg(lastPos, nextPos);
            vec3_rebond(vecR, pIntersec->plan.normal, vecR);
            vec3_normalize(vecR);
            vec3_mul(nextPos, vecR, Rayonmax/ iter);
            vec3_add(nextPos, nextPos, lastPos);


            nbligne += 35;
        } else {
            sp->ligne->AddKey(Ntime);
            sp->ligne->SetKeyPos(nextPos[0], nextPos[1], nextPos[2]);

            sortie = TRUE;   // c'est fini !!!!!
        }
        iter += 1.0f;
    }

    sp->ligne->Confirm(nbligne);
    sp->ligne->SetZtest(TRUE);
    sp->ligne->SetEndColor(0.7f, 1.0f, 0.5f, 1.0f);
    sp->ligne->SetStartColor(0.0f, 0.0f, 0.0f,  1.0f);

    sp->IsFinish = FALSE;
    sp->TimeToGo = sp->TotalTime = TIME_DISAPPEAR;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void NeuroLines::AffParticul(CoordU3D& pos, Ufloat alpha) {
    Ufloat size;

    pos.frame_process = -1;

    U3D3Monde3D->GetActualCamera()->ProjetUni(&pos, 0);

    if (!(pos.ClipInfo&ClipProche)) {
        size = PART_SIZE * pos.ecran[3] * .5f;

        partMat->SetAlpha(alpha*.5f);

        partBmp->SetCoord(0, pos.ecran[0] - size, pos.ecran[1] - size, pos.ecran[2]);
        partBmp->SetCoord(1, pos.ecran[0] + size, pos.ecran[1] - size, pos.ecran[2]);
        partBmp->SetCoord(2, pos.ecran[0] + size, pos.ecran[1] + size, pos.ecran[2]);
        partBmp->SetCoord(3, pos.ecran[0] - size, pos.ecran[1] + size, pos.ecran[2]);

        partBmp->Affiche();
    }

}
//----------------------------------------------------------------------------------------------------------------------------------------
void NeuroLines::UpdateFrame(Ufloat laptime) {
    CoordU3D    tmpc;
    Ufloat*      tmppartPos = partPos;
    U32         nbpart = 0;


    //------------------------- Affiche les lignes
    NeuroOneLine* ligne = AllLines;
    for (U32 a=0; a<NB_LINE; a++,ligne++) {
        if (ligne->IsFinish) {
            ligne->TimeToGo -= laptime;
            if (ligne->TimeToGo <= 0.0f) {
                GenereLine(ligne);
            } else {
                Ufloat alpha = ligne->TimeToGo / ligne->TotalTime;
                ligne->ligne->SetEndAlpha(alpha);
                ligne->ligne->SetStartAlpha(alpha);

                ligne->ligne->Affiche(laptime);

                ligne->ligne->GetLastPos(tmppartPos);
                tmppartPos[3] = alpha;
                tmppartPos+=4;
                nbpart++;
            }
        } else {
            ligne->ligne->Affiche(laptime);
            if (ligne->ligne->IsFinish())
                ligne->IsFinish = TRUE;

            ligne->ligne->GetLastPos(tmppartPos);
            tmppartPos[3] = 1.0f;
            tmppartPos+=4;
            nbpart++;
        }
    }



    //------------------------- Affiche les particuels au bout des lignes

    tmppartPos = partPos;
    U3D3Pipeline->Begin(partMat, FALSE, TRUE);

    for (U32 a=0; a<nbpart; a++,tmppartPos+=4) {
        vec3_eg(tmpc.trans, tmppartPos);
        AffParticul(tmpc, tmppartPos[3]);
    }

    U3D3Pipeline->End(partMat);

}
//----------------------------------------------------------------------------------------------------------------------------------------
