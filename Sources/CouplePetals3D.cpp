/*-----------------------------------------------------------------+
 |                      CouplePetals3D.cpp                          |
 |                                                                  |
 |  je vasi essayer de faire des petals de fleurs sur la 2eme scene |
 |              du couple, avec un chti doigt de physik             |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#include "stdafx.h"

#define FACT_SCENE_CM   5.f // facteur de taille entre la scene et les unités utilisées

#define DEFAULT_SIZEX   10.f
#define DEFAULT_SIZEY   20.f

#define DEFAULT_SHADOW 25.f


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +---------------------------+
//                                      |  Class CoupleOnePetals3D  |
//                                      +---------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
CoupleOnePetals3D::CoupleOnePetals3D() : CoupleOnePetals() {
    p1 = NULL;
    p2 = NULL;

    for (U32 a=0; a<6; a++)
        coord[a] = NULL;

}
//----------------------------------------------------------------------------------------------------------------------------------------
// les 2 poly et les 6 vertx doivent etre a la suite
void CoupleOnePetals3D::Set(PolygonU3D* poly, VertexU3D* vertex, MaterialU3D* mat) {
    p1 = poly;
    p1->SetNbVertex(4);
    p1->m = mat;

    vertex[0].utilisateurs++;
    p1->Vtab[0] = &vertex[0];
    vertex[1].utilisateurs++;
    p1->Vtab[1] = &vertex[1];
    vertex[3].utilisateurs++;
    p1->Vtab[2] = &vertex[3];
    vertex[2].utilisateurs++;
    p1->Vtab[3] = &vertex[2];

    poly++;
    p2 = poly;
    p2->SetNbVertex(4);
    p2->m = mat;

    vertex[2].utilisateurs++;
    p2->Vtab[0] = &vertex[2];
    vertex[3].utilisateurs++;
    p2->Vtab[1] = &vertex[3];
    vertex[5].utilisateurs++;
    p2->Vtab[2] = &vertex[5];
    vertex[4].utilisateurs++;
    p2->Vtab[3] = &vertex[4];

    vertex[0].U = 0.0f;
    vertex[0].V = 0.0f;
    vertex[1].U = 1.0f;
    vertex[1].V = 0.0f;
    vertex[2].U = 0.0f;
    vertex[2].V = 0.5f;
    vertex[3].U = 1.0f;
    vertex[3].V = 0.5f;
    vertex[4].U = 0.0f;
    vertex[4].V = 1.0f;
    vertex[5].U = 1.0f;
    vertex[5].V = 1.0f;

    Ufloat sizex = DEFAULT_SIZEX*GetMass();
    Ufloat sizey = DEFAULT_SIZEY*GetMass();

    coord[0] = vertex[0].c;
    vec3_set(coord[0]->origine, -sizex, -sizey, 0.0f);
    coord[1] = vertex[1].c;
    vec3_set(coord[1]->origine, sizex, -sizey, 0.0f);
    coord[2] = vertex[2].c;
    vec3_set(coord[2]->origine, -sizex, 0.0f, sizex*.5f*Random1() - .25f*sizey);
    coord[3] = vertex[3].c;
    vec3_set(coord[3]->origine, sizex, 0.0f, sizex*.5f*Random1() - .25f*sizey);
    coord[4] = vertex[4].c;
    vec3_set(coord[4]->origine, -sizex, sizey, 0.0f);
    coord[5] = vertex[5].c;
    vec3_set(coord[5]->origine, sizex, sizey, 0.0f);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleOnePetals3D::Ini(Ufloat tapp, Ufloat tres, Ufloat tdisa, Ufloat pos[3], Ufloat vitesse[3]) {
    CoupleOnePetals::Ini(tapp, tres, tdisa, pos, vitesse);

    rot.SetFromAxis(Random1()*2.f - 1.f, Random1()*2.f - 1.f, Random1()*2.f - 1.f, 2.f*M_PI*Random1());
    incrot.SetFromAxis(Random1()*2.f - 1.f, Random1()*2.f - 1.f, Random1()*2.f - 1.f, .5f*M_PI*Random1() -.25f*M_PI);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleOnePetals3D::IncAnim(Ufloat laptime) {
    Mat3x4  mat;
    Ufloat  tmp[3];

    CalcPos(laptime);

    Ufloat alpha = GetAlpha();

    vec4_set(p1->RVBA, 1.0f, 1.0f, 1.0f, alpha);
    vec4_set(p2->RVBA, 1.0f, 1.0f, 1.0f, alpha);

    rot *= incrot * laptime;
    rot.matrix(mat);
    vec3_mul(tmp, GetPos(), FACT_SCENE_CM);
    mat.SetPos(tmp[0], tmp[1], tmp[2]);

    for (U32 a=0; a<6; a++)
        mat.GetTransfo(coord[a]->trans, coord[a]->origine);
}
//----------------------------------------------------------------------------------------------------------------------------------------







//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +---------------------------+
//                                      |  Class CoupleShadowP3D    |
//                                      +---------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
CoupleShadowP3D::CoupleShadowP3D() {
    p1 = NULL;
    petal = NULL;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// les 2 poly et les 6 vertx doivent etre a la suite
void CoupleShadowP3D::Set(PolygonU3D* poly, VertexU3D* vertex, MaterialU3D* mat, CoupleOnePetals3D* peta) {
    p1 = poly;
    p1->SetNbVertex(4);
    p1->m = mat;

    vertex[0].utilisateurs++;
    p1->Vtab[0] = &vertex[0];
    vertex[1].utilisateurs++;
    p1->Vtab[1] = &vertex[1];
    vertex[3].utilisateurs++;
    p1->Vtab[2] = &vertex[3];
    vertex[2].utilisateurs++;
    p1->Vtab[3] = &vertex[2];

    vertex[0].U = 0.0f;
    vertex[0].V = 0.0f;
    vertex[1].U = 1.0f;
    vertex[1].V = 0.0f;
    vertex[2].U = 0.0f;
    vertex[2].V = 1.0f;
    vertex[3].U = 1.0f;
    vertex[3].V = 1.0f;

    Ufloat sizex = DEFAULT_SHADOW*.5f;
    Ufloat sizey = DEFAULT_SHADOW*.5f;

    vec3_set(vertex[0].c->origine, -sizex, 0.0f, -sizey);
    vec3_set(vertex[1].c->origine, sizex, 0.0f, -sizey);
    vec3_set(vertex[2].c->origine, -sizex, 0.0f, sizey);
    vec3_set(vertex[3].c->origine, sizex, 0.0f, sizey);

    petal = peta;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleShadowP3D::IncAnim(Ufloat laptime) {
    Mat3x4  mat;
    Ufloat  tmp[3]= {0.0f,1.0f,0.0f};
    Ufloat  alpha = petal->GetAlpha();

    vec4_set(p1->RVBA, 1.0f, 1.0f, 1.0f, alpha);

    petal->GetQuat().matrix(mat);
    vec3_mul(tmp, petal->GetPos(), FACT_SCENE_CM);
    mat.SetPos(tmp[0], tmp[1], tmp[2]);
    mat *= petal->GetMass() + tmp[1]*.001f;


    for (U32 a=0; a<4; a++) {
        mat.GetTransfo(p1->Vtab[a]->c->trans, p1->Vtab[a]->c->origine);
        p1->Vtab[a]->c->trans[1] = 0.0f;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
