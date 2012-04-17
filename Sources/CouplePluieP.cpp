/*---------------------------------------------------------------------+
 |                      CouplePluieP.cpp                                |
 |                                                                      |
 |la classe ki affiche de la pluie en polygones sur la scene en avant   |
 |                      et en arrière plan.                             |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#include "stdafx.h"

#define POSX_MAX    250.f
#define POSX_DELTA  500.f

#define POSY_MAX    200.f
#define POSY_DELTA  500.f

#define POSZ_MAX    250.f
#define POSZ_DELTA  500.f

#define NB_GOUTTES  900

#define SIZE_GOUTE 2.5f


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class GoutteP    |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
GoutteP::GoutteP() {
    vec3_set(dir, 0.0f, 0.0f, 0.0f);
    vec3_set(pos, 0.0f, 0.0f, 0.0f);
    size = 0.0f;
    vitesse  = 0.0f;
    alpha = 0.0f;
    poly1 = NULL;
    poly2 = NULL;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GoutteP::Genere(Ufloat d[3]) {
    /*  Ufloat      pos0[3] = {0.0f, SIZE_GOUTE, 0.0f};
        Ufloat      pos1[3] = {0.0f, -SIZE_GOUTE, 0.0f};
        Ufloat      pos2[3] = {-SIZE_GOUTE, 0.0f, 0.0f};
        Ufloat      pos3[3] = {SIZE_GOUTE, 0.0f, 0.0f};*/
    Ufloat  pos0[3];
    Ufloat  pos1[3];
    Ufloat  pos2[3];
    Ufloat  pos3[3];
    Mat3x4  mat;


    vec3_eg(dir, d);
    vitesse = 350.f + 295.f*Random1();
    alpha = .5f + .5f*Random1();
    size = 55.f + 135.f*Random1();

    vec3_set(pos, POSX_MAX - POSX_DELTA*Random1(), POSY_MAX - POSY_DELTA*.05f*Random1(), POSZ_MAX - POSZ_DELTA*Random1());
    vec3_set(pos0, 0.0f, 0.0f, -SIZE_GOUTE);
    vec3_set(pos1, 0.0f, 0.0f, SIZE_GOUTE);
    vec3_set(pos2, -SIZE_GOUTE, 0.0f, 0.0f);
    vec3_set(pos3, SIZE_GOUTE, 0.0f, 0.0f);

    Plan plan(dir, pos);
    plan.GetTransMatrix(mat);

    mat.SetPos(pos[0]-dir[0]*size, pos[1]-dir[1]*size, pos[2]-dir[2]*size);
    mat.GetTransfo(poly1->Vtab[0]->c->trans, pos0);
    mat.GetTransfo(poly1->Vtab[1]->c->trans, pos1);
    mat.GetTransfo(poly2->Vtab[0]->c->trans, pos2);
    mat.GetTransfo(poly2->Vtab[1]->c->trans, pos3);

    mat.SetPos(pos[0], pos[1], pos[2]);
    mat.GetTransfo(poly1->Vtab[3]->c->trans, pos0);
    mat.GetTransfo(poly1->Vtab[2]->c->trans, pos1);
    mat.GetTransfo(poly2->Vtab[3]->c->trans, pos2);
    mat.GetTransfo(poly2->Vtab[2]->c->trans, pos3);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GoutteP::Move(Ufloat step) {
    Ufloat inc[3];

    vec3_mul(inc, dir, step*vitesse);
    vec3_add(pos, pos, inc);

    for (U32 a=0; a<4; a++) {
        Ufloat* c1 =poly1->Vtab[a]->c->trans;
        Ufloat* c2 =poly2->Vtab[a]->c->trans;
        vec3_add(c1, c1, inc);
        vec3_add(c2, c2, inc);
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GoutteP::SetAlpha(Ufloat alph) {
    poly1->RVBA[3] = alph * alpha;
    poly2->RVBA[3] = alph * alpha;
}
//----------------------------------------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |   Class PluieP    |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
PluieP::PluieP(MondeU3D* monde) {
    Monde = monde;

    //-------------------- ini et load l'image d'une particul !
    TextureManager::SetRepertoire("");
    partTex = TextureManager::OpenJpg("Datas/meshes/Couple/VentP.jpg", BILINEAR_TEXTURE/*|TRILINEAR_TEXTURE*/);
    partTex->IncUser();

    partMat = new MaterialU3D("partMat");
    partMat->SetTexture1(partTex);
    partMat->SetTransp(MatTranspAddAlpha);
    partMat->SetShading(MatShadingFlat);
    partMat->Side(TRUE);
    partMat->Confirm();

    //------------------- créé un nouvel objet
    obj = new ObjetU3D("PluieP", NB_GOUTTES*8, NB_GOUTTES*8, NB_GOUTTES*2);
    Monde->AddElem(obj);     // rajoute objet dans le monde
    obj->SetRecalcFlag(0);     // ne recalcul rien sur l'objet


    //-------- ini vertex et coordonnées de l'objet
    for (U32 a=0; a<obj->nbcoords; a++) {
        Ufloat vpos = a&0x4;   // position dans la map en V
        Ufloat upos = a&0x1;   // position dans la map en U

        vec4_set(obj->Ctab[a].RVBA, 1.f, 1.f, 1.f, 1.f);
        vec4_set(obj->Ctab[a].ecran, 0.f, 0.f, 0.f, 0.f);
        obj->Ctab[a].utilisateurs = 1;
        obj->Ctab[a].frame_process = (U32)-1;

        obj->Vtab[a].c =  &obj->Ctab[a];   // assigne Coodonnée au vertex
        obj->Vtab[a].V = vpos;
        obj->Vtab[a].U = upos;
        obj->Vtab[a].utilisateurs = 0;
    }

    //-------- ini polygones de l'objet
    PolygonU3D* poly = obj->Ptab;
    for (U32 a=0; a<NB_GOUTTES; a++,poly++) {
        U32 pos = a*8;

        poly->SetNbVertex(4);
        poly->m = partMat;

        obj->Vtab[ pos ].utilisateurs++;
        poly->Vtab[ 0 ] = &obj->Vtab[ pos ];
        obj->Vtab[ pos+1 ].utilisateurs++;
        poly->Vtab[ 1 ] = &obj->Vtab[ pos+1 ];
        obj->Vtab[ pos+5 ].utilisateurs++;
        poly->Vtab[ 2 ] = &obj->Vtab[ pos+5 ];
        obj->Vtab[ pos+4 ].utilisateurs++;
        poly->Vtab[ 3 ] = &obj->Vtab[ pos+4 ];

        poly++;
        poly->SetNbVertex(4);
        poly->m = partMat;

        obj->Vtab[ pos+2 ].utilisateurs++;
        poly->Vtab[ 0 ] = &obj->Vtab[ pos+2 ];
        obj->Vtab[ pos+3 ].utilisateurs++;
        poly->Vtab[ 1 ] = &obj->Vtab[ pos+3 ];
        obj->Vtab[ pos+7 ].utilisateurs++;
        poly->Vtab[ 2 ] = &obj->Vtab[ pos+7 ];
        obj->Vtab[ pos+6 ].utilisateurs++;
        poly->Vtab[ 3 ] = &obj->Vtab[ pos+6 ];
    }


    //----------------- donne une direction a la pluie
    vec3_set(dirGeneral, 0.2f, -1.0f, 0.1f);
    vec3_normalize(dirGeneral);


    //------------- ini le tableau de goutes
    Gouttes = new GoutteP[ NB_GOUTTES ];

    for (U32 a=0; a<NB_GOUTTES; a++) {
        Gouttes[a].SetPoly(&obj->Ptab[a*2], &obj->Ptab[a*2+1]);
        Gouttes[a].Genere(dirGeneral);
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
PluieP::~PluieP() {
    //------------------ KIll material et image
    delete partMat;
    TextureManager::SubUse(partTex);

    //------------------ Kill les gouttes
    delete [] Gouttes;
}
//----------------------------------------------------------------------------------------------------------------------------------------
Ufloat PluieP::CalcAlpha(Ufloat y) {
    Ufloat alpha = 1.0f;

    if (y < POSY_MAX - POSY_DELTA*.9f)
        alpha = (y - (POSY_MAX - POSY_DELTA)) / ((POSY_MAX - POSY_DELTA*.9f) - (POSY_MAX - POSY_DELTA));

    if (y > POSY_MAX - POSY_DELTA*.1f)
        alpha = (POSY_MAX - y) / (POSY_MAX - (POSY_MAX - POSY_DELTA*.1f));

    return alpha;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void PluieP::IncAnim(Ufloat laptime) {
    GoutteP*    tmpg;
    Ufloat      pos[3];

    tmpg = Gouttes;
    for (U32 a=0; a<NB_GOUTTES; a++,tmpg++) {
        tmpg->Move(laptime);

        vec3_eg(pos, tmpg->GetPos());

        if (pos[1] < POSY_MAX - POSY_DELTA) {
            tmpg->Genere(dirGeneral);
            vec3_eg(pos, tmpg->GetPos());
        }

        tmpg->SetAlpha(CalcAlpha(pos[1]));
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------

