/*-----------------------------------------------------------------+
 |                          GhostEclaire.cpp                        |
 |                                                                  |
 |   je vasi faire jaillir des petits eclaires des tuyaux de cette  |
 |                  fabuleuse scene trop oufement kiffante          |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#include "stdafx.h"

#define NB_ECLAIRS  20

#define ECLAIRE_SIZE 10.f

#define NB_SEGMENT_ECLAIR   30

#define PARTI_SIZE 20.f

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +---------------------------+
//                                      |  Class GhostOneEclaire    |
//                                      +---------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
GhostOneEclaire::GhostOneEclaire(MondeU3D* m, MaterialU3D* mat) {
    suiv = NULL;

    alpha = 1.0f;
    incAlpha = 0.0f;

    spline = NULL;

    interPos = new Ufloat[ 3*(NB_SEGMENT_ECLAIR+1) ];   // alloue tableau de position intermédiaires

    //------------- ini l'objet
    obj = new ObjetU3D("GEclaire", (NB_SEGMENT_ECLAIR+1)*4, (NB_SEGMENT_ECLAIR+1)*4, NB_SEGMENT_ECLAIR*2);
    m->AddElem(obj);     // rajoute objet dans le monde
    obj->SetRecalcFlag(0);     // ne recalcul rien sur l'objet

    //-------- ini vertex et coordonnées de l'objet
    for (U32 a=0; a<obj->nbcoords; a++) {
        Ufloat vpos = (Ufloat)(a>>2) / NB_SEGMENT_ECLAIR;   // position dans la map en V

        vec4_set(obj->Ctab[a].RVBA, 1.f, 1.f, 1.f, 1.f);
        vec4_set(obj->Ctab[a].ecran, 0.f, 0.f, 0.f, 0.f);
        obj->Ctab[a].utilisateurs = 1;
        obj->Ctab[a].frame_process = (U32)-1;

        obj->Vtab[a].c =  &obj->Ctab[a];   // assigne Coodonnée au vertex
        obj->Vtab[a].V = vpos;
        obj->Vtab[a].U = (a&0x1);
        obj->Vtab[a].utilisateurs = 0;
    }

    //-------- ini polygones de l'objet
    PolygonU3D* poly = obj->Ptab;
    for (U32 a=0; a<obj->nbpolys; a++,poly++) {
        U32 pos = ((a>>1)<<2) + ((a&0x1)<<1);

        poly->SetNbVertex(4);
        poly->m = mat;

        obj->Vtab[ pos ].utilisateurs++;
        poly->Vtab[ 0 ] = &obj->Vtab[ pos ];

        obj->Vtab[ pos+1 ].utilisateurs++;
        poly->Vtab[ 1 ] = &obj->Vtab[ pos+1 ];

        obj->Vtab[ pos+5 ].utilisateurs++;
        poly->Vtab[ 2 ] = &obj->Vtab[ pos+5 ];

        obj->Vtab[ pos+4 ].utilisateurs++;
        poly->Vtab[ 3 ] = &obj->Vtab[ pos+4 ];
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
GhostOneEclaire::~GhostOneEclaire() {
    if (spline) delete spline;
    delete [] interPos;
}
//----------------------------------------------------------------------------------------------------------------------------------------
Ufloat* GhostOneEclaire::GetLastPos() {
    return &interPos[NB_SEGMENT_ECLAIR*3];
}
//----------------------------------------------------------------------------------------------------------------------------------------
Ufloat* GhostOneEclaire::GetFirstPos() {
    return &interPos[0];
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostOneEclaire::CalcNew(MondeU3D* monde, Ufloat posdep[3], Ufloat vdir[3]) {
    Ufloat      totaltime = 0.0f;
    bool        sortie=FALSE;
    Ufloat      lastPos[3];
    Ufloat      nextPos[3];
    Ufloat      hasard[3];
    PolygonU3D* pIntersec=NULL;


    //------------------ Calcul la spline
    if (spline) delete spline;
    spline = new HSplinaeAff;

    spline->AddKey(0.0f);
    spline->SetKeyPos(posdep[0], posdep[1], posdep[2]);

    vec3_eg(lastPos, posdep);
    vec3_mul(nextPos, vdir, 60.f+ Random1()*50.f);
    vec3_add(nextPos, nextPos, lastPos);

    while (!sortie) {
        totaltime += 0.8f + .5f*Random1();

        if (monde->SegmentIntersecPoly(lastPos, nextPos, pIntersec)) {
            spline->AddKey(totaltime);
            spline->SetKeyPos(nextPos[0], nextPos[1], nextPos[2]);

            sortie = TRUE;   // c'est fini !!!!!
        } else {
            spline->AddKey(totaltime);
            spline->SetKeyPos(nextPos[0], nextPos[1], nextPos[2]);

            for (U32 a=0; a<3; a++) {
                if (nextPos[a] > 1600.f)    sortie = TRUE;
                if (nextPos[a] < -1600.f)   sortie = TRUE;
            }
        }

        vec3_eg(lastPos, nextPos);
        vec3_mul(nextPos, vdir, 60.f + Random1()*40.f);
        vec3_add(nextPos, nextPos, lastPos);

        vec3_set(hasard, Random1()*2.f-1.f, Random1()*2.f-1.f, Random1()*2.f-1.f);
        vec3_mul(hasard, hasard, 20.f + Random1()*40.f);
        vec3_add(nextPos, nextPos, hasard);
    }

    spline->Confirm(1);


    CalcCoord(vdir, totaltime);

    Galpha = 0.5f + 0.5f*Random1();
    alpha = 1.0f;
    incAlpha = -1.0f / (.5f + .5f*Random1());

}
//----------------------------------------------------------------------------------------------------------------------------------------
//Calcul la position des coordonées
void GhostOneEclaire::CalcCoord(Ufloat vdir[3], Ufloat totaltime) {
    Ufloat      incanim = totaltime / NB_SEGMENT_ECLAIR;
    Ufloat      pos0[3] = {0.0f, ECLAIRE_SIZE, 0.0f};
    Ufloat      pos1[3] = {0.0f, -ECLAIRE_SIZE, 0.0f};
    Ufloat      pos2[3] = {-ECLAIRE_SIZE, 0.0f, 0.0f};
    Ufloat      pos3[3] = {ECLAIRE_SIZE, 0.0f, 0.0f};
    Ufloat      actuapos[3];
    Ufloat      lastpos[3];
    Ufloat      vec[3];
    Ufloat*      IPos = interPos;
    Mat3x4      mat;
    CoordU3D*   coord;

    spline->IniAnim();
    spline->GetLastPos(actuapos);
    vec3_eg(vec, vdir);

    coord = obj->Ctab;
    for (U32 a=0; a<NB_SEGMENT_ECLAIR+1; a++,coord+=4,IPos+=3) {
        Plan plan(vec, actuapos);
        plan.GetTransMatrix(mat);
        mat.SetPos(actuapos[0], actuapos[1], actuapos[2]);

        vec3_set(IPos, actuapos[0], actuapos[1], actuapos[2]);

        mat.GetTransfoR(coord->origine, pos0);

        mat.GetTransfoR(coord[1].origine, pos1);

        mat.GetTransfoR(coord[2].origine, pos2);

        mat.GetTransfoR(coord[3].origine, pos3);

        spline->IncAnim(incanim);
        vec3_eg(lastpos, actuapos);
        spline->GetLastPos(actuapos);


        vec3_sub(vec, actuapos, lastpos);
    }

    obj->CalcBBox();   // recalcul la bounding box
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostOneEclaire::AssignCoord(Ufloat time) {
    CoordU3D*   coord;
    Ufloat*     IPos = &interPos[3];
    Ufloat      dep[3];

    coord = obj->Ctab;

    vec3_add(coord->trans, coord->origine, interPos);
    vec3_add(coord[1].trans, coord[1].origine, interPos);
    vec3_add(coord[2].trans, coord[2].origine, interPos);
    vec3_add(coord[3].trans, coord[3].origine, interPos);

    coord+=4;

    for (U32 a=1; a<NB_SEGMENT_ECLAIR; a++,coord+=4,IPos+=3) {
        Ufloat intens = .5f + .5f * cos(time + a*.48f);
        vec3_set(dep, Random1()*20.f + 10.f, Random1()*20.f + 10.f, Random1()*20.f + 10.f);
        vec3_mul(dep, dep, intens*alpha);
        vec3_add(dep, dep, IPos);

        vec3_add(coord->trans, coord->origine, dep);
        vec3_add(coord[1].trans, coord[1].origine, dep);
        vec3_add(coord[2].trans, coord[2].origine, dep);
        vec3_add(coord[3].trans, coord[3].origine, dep);
    }

    vec3_add(coord->trans, coord->origine, IPos);
    vec3_add(coord[1].trans, coord[1].origine, IPos);
    vec3_add(coord[2].trans, coord[2].origine, IPos);
    vec3_add(coord[3].trans, coord[3].origine, IPos);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostOneEclaire::IncAnim(Ufloat time, Ufloat laptime) {
    alpha += incAlpha*laptime;
    if (alpha<0.0f) alpha = 0.0f;

    AssignCoord(time);

    Ufloat alph = alpha*Galpha;
    PolygonU3D* poly = obj->Ptab;
    for (U32 a=0; a<obj->nbpolys; a++,poly++)
        poly->RVBA[3] = alph;
}
//----------------------------------------------------------------------------------------------------------------------------------------






//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |  Class GhostEclaire   |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
GhostEclaire::GhostEclaire(MondeU3D* monde) {
    Monde = monde;
    Mat = NULL;
    next = NULL;


    //-------------------- ini et load l'image d'une particul !
    TextureManager::SetRepertoire("");
    partTex = TextureManager::OpenJpg("Datas/meshes/Ghost/particul.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    partTex->IncUser();

    partMat = new MaterialU3D("partMat");
    partMat->SetTexture1(partTex);
    partMat->SetTransp(MatTranspAddAlpha);
    partMat->SetShading(MatShadingFlat);
    partMat->Confirm();

    partBmp = new BitmapU3D(partMat);


    //------------------- alloue etincelles
    Etincelles = new GhostAllEtincelle(100);


#if 0
    //-------------------- recherche le material des eclairs
    for (Monde->IniMatIterator(); Monde->GetMatFromIterator(); Monde->IncMatIterator()) {
        if (strcasecmp(Monde->GetMatFromIterator()->GetNom(), "Eclair")==0) {
            Mat = Monde->GetMatFromIterator();
            Mat->SetTex1Pos(0.0f, 0.0f, 0.9f, 5.f);
            break;
        }
    }
#endif
    Mat = Monde->GetMat("Eclair");
    Mat->SetTex1Pos(0.0f, 0.0f, 0.9f, 5.f);


    //-------------------- recherche les objets ki s'appellent tuyauXX
    generators = NULL;
    for (Monde->IniObjIterator(); Monde->GetObjFromIterator(); Monde->IncObjIterator()) {
        if (strncmp(Monde->GetObjFromIterator()->GetNom(), "tuyau", 5) ==0) {
            Monde->GetObjFromIterator()->SetUVcalc(ObjetU3D::envStage0);

            GhostEclaireGenerator* tmp = new GhostEclaireGenerator;
            tmp->obj = Monde->GetObjFromIterator();
            tmp->suiv = generators;
            generators = tmp;
        }
    }


    //-------------------- créé des eclairs
    eclairs = NULL;
    for (U32 a=0; a<NB_ECLAIRS; a++) {
        GhostOneEclaire* tmp = new GhostOneEclaire(Monde, Mat);

        Genere(tmp);
        tmp->suiv = eclairs;
        eclairs = tmp;
    }


}
//----------------------------------------------------------------------------------------------------------------------------------------
GhostEclaire::~GhostEclaire() {
    GhostEclaireGenerator*  tmpg;
    GhostOneEclaire*        tmpe;


    //------------------ KIll image de la particul
    delete partBmp;
    delete partMat;
    TextureManager::SubUse(partTex);


    while (generators) {
        tmpg = generators->suiv;
        delete generators;
        generators = tmpg;
    }

    while (eclairs) {
        tmpe = eclairs->suiv;
        delete eclairs;
        eclairs = tmpe;
    }

    //------------ Kill etincelles
    delete Etincelles;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostEclaire::Genere(GhostOneEclaire* ec) {
    Ufloat pos3d[3];
    Ufloat vec[3];

    if (!next) next = generators;

    U32 pos = rand()%next->obj->nbcoords;

    next->obj->GetObjToWorld().GetTransfo(pos3d, next->obj->Ctab[pos].origine);
    next->obj->GetObjToWorld().GetTransfoR(vec,  next->obj->Ctab[pos].normale);
    vec3_eg(vec, pos3d);
    vec[1] += 300.f;
    vec3_normalize(vec);

    ec->CalcNew(Monde, pos3d, vec);

    Etincelles->GenereEtincelle(pos3d, vec, rand()%40 + 15);

    next = next->suiv;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostEclaire::AffOneParti(Ufloat pos[3], Ufloat alpha) {
    Camera*             cam = Monde->GetActualCamera();

//  if( !Monde->SegmentIntersecPoly( cam->GetPos(), pos) )
    {
        CoordU3D    tmpc;
        Ufloat      size;

        vec3_eg(tmpc.trans, pos);
        cam->ProjetUni(&tmpc, 0);

        if (!(tmpc.ClipInfo&ClipProche)) {
            size = PARTI_SIZE * tmpc.ecran[3] * .5f;

            partMat->SetAlpha(alpha);

            partBmp->SetCoord(0, tmpc.ecran[0] - size, tmpc.ecran[1] - size);
            partBmp->SetCoord(1, tmpc.ecran[0] + size, tmpc.ecran[1] - size);
            partBmp->SetCoord(2, tmpc.ecran[0] + size, tmpc.ecran[1] + size);
            partBmp->SetCoord(3, tmpc.ecran[0] - size, tmpc.ecran[1] + size);

            partBmp->Affiche();
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostEclaire::AffAllPart(Ufloat laptime) {
    GhostOneEclaire*    tmpe;
    Ufloat              pos[3];

    //----------------- affiche particules
    U3D3Pipeline->Begin(partMat);

    tmpe = eclairs;
    while (tmpe) {
        Ufloat alpha = tmpe->GetAlpha();

        vec3_eg(pos, tmpe->GetFirstPos());
        AffOneParti(pos, alpha);

        vec3_eg(pos, tmpe->GetLastPos());
        AffOneParti(pos, alpha);

        tmpe = tmpe->suiv;
    }

    U3D3Pipeline->End(partMat);

    //----------------- affiche etincelles
    Etincelles->Affiche();
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostEclaire::IncAnim(Ufloat time, Ufloat laptime) {
    GhostOneEclaire*        tmpe;

    Mat->SetTex1Pos(0.0f, -time*2.f, 0.9f, 5.f);


    tmpe = eclairs;
    while (tmpe) {
        if (tmpe->IsFinish())
            Genere(tmpe);
        else
            tmpe->IncAnim(time, laptime);

        tmpe = tmpe->suiv;
    }

    //----------------- anim etincelles
    Etincelles->UpdateFrame(laptime, Monde);
}
//----------------------------------------------------------------------------------------------------------------------------------------
