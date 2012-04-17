/*---------------------------------------------------------------------+
 |                          OeilDefor.cpp                               |
 |                                                                      |
 |   c'est une fake deformation de l'oeil 3D ( on va lui faire subir    |
 |                      les pires sévisses ).                           |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#include "stdafx.h"


#define NB_POLY_X   30
#define NB_POLY_Y   30

//-----------------------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |   Class OeilDefor     |
//                                      +-----------------------+
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------------------------------
OeilDefor::OeilDefor() {
    OeilDeforVertex* tmpv;

    Galpha = 1.0f;
    incAlpha = 0.0f;
    zmin = 1e+19f;
    zmax = -1e+19f;
    calcAlpha = FALSE;
    enable = TRUE;
    DivFact = 8.f;

    //------------ ini bitmap
    Bmp = new BitmapU3D(NULL);

    TextureManager::SetRepertoire("");
    Tex = TextureManager::OpenJpg("Datas/meshes/Oeil/fakeoeil.jpg");
    Tex->IncUser();

    Mat = new MaterialU3D("Mat");
    Mat->SetTexture1(Tex);
    Mat->SetTransp(MatTranspAlpha);
    Mat->SetShading(MatShadingGouraud);
    Mat->Confirm();


    //------------ ini camera
    cam = new Camera("tmp");


    //------------ ini les vertex
    AllVertex = new OeilDeforVertex[(NB_POLY_X+1) * (NB_POLY_Y+1) ];

    tmpv = AllVertex;
    for (U32 a=0; a<NB_POLY_Y+1; a++)
        for (U32 b=0; b<NB_POLY_X+1; b++,tmpv++) {
            tmpv->alpha = 1.0f;
            tmpv->U = (Ufloat)b / NB_POLY_X;
            tmpv->V = (Ufloat)a / NB_POLY_Y;
            vec3_set(tmpv->trans, tmpv->U, tmpv->V, 100.f);
            cam->Cam2MondePos(tmpv->origine, tmpv->trans);
            vec3_eg(tmpv->dest, tmpv->origine);
        }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
OeilDefor::~OeilDefor() {
    delete Bmp;
    delete Mat;
    TextureManager::SubUse(Tex);

    delete cam;
    delete [] AllVertex;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilDefor::Projet() {
    OeilDeforVertex*    tmpv;
    Ufloat              tmp[4];

    tmpv = AllVertex;
    for (U32 a=0; a<(NB_POLY_X+1) * (NB_POLY_Y+1); a++,tmpv++) {
        cam->ProjetUni(tmp, tmpv->origine);
        vec3_eg(tmpv->trans, tmp);

        vec3_sub(tmp, tmpv->dest, tmpv->origine);
        vec3_div(tmp, tmp, DivFact);
        vec3_add(tmpv->origine, tmp, tmpv->origine);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilDefor::AffAll() {
    OeilDeforVertex* tmpv,*tmpv1,*tmpv2,*tmpv3;

    U3D3Pipeline->Begin(Mat);

    tmpv = AllVertex;
    for (U32 a=0; a<NB_POLY_Y; a++) {
        for (U32 b=0; b<NB_POLY_X; b++,tmpv++) {
            tmpv1 = tmpv+1;
            tmpv2 = tmpv+ NB_POLY_X+1;
            tmpv3 = tmpv+ NB_POLY_X+2;

            Bmp->SetCoord(0, tmpv->trans);
            Bmp->SetMapCoord(0, tmpv->U, tmpv->V);
            Bmp->SetAlpha(0, tmpv->alpha);
            Bmp->SetCoord(1, tmpv1->trans);
            Bmp->SetMapCoord(1, tmpv1->U, tmpv1->V);
            Bmp->SetAlpha(1, tmpv1->alpha);
            Bmp->SetCoord(3, tmpv2->trans);
            Bmp->SetMapCoord(3, tmpv2->U, tmpv2->V);
            Bmp->SetAlpha(3, tmpv2->alpha);
            Bmp->SetCoord(2, tmpv3->trans);
            Bmp->SetMapCoord(2, tmpv3->U, tmpv3->V);
            Bmp->SetAlpha(2, tmpv3->alpha);

            Bmp->Affiche();
        }

        tmpv++;
    }

    U3D3Pipeline->End(Mat);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilDefor::OnDefoX(Ufloat dec) {
    OeilDeforVertex*    tmpv;

    tmpv = AllVertex;

    if (dec > 1.5f) {
        for (U32 a=0; a<NB_POLY_Y+1; a++)
            for (U32 b=0; b<NB_POLY_X+1; b++,tmpv++) {
                Ufloat intens = (Ufloat)b/NB_POLY_Y;
                intens *= intens;

                vec3_set(tmpv->origine, -intens * (50.f + 20.f * Sin(a*.3f + dec)), 0.0f, intens * 10.f * Cos(a*.4f - dec));
                vec3_add(tmpv->origine, tmpv->origine, tmpv->dest);
            }
    } else {
        for (U32 a=0; a<NB_POLY_Y+1; a++)
            for (U32 b=0; b<NB_POLY_X+1; b++,tmpv++) {
                Ufloat intens = 1.0f - (Ufloat)b/NB_POLY_Y;
                intens *= intens;

                vec3_set(tmpv->origine, intens * (50.f + 20.f * Sin(a*.3f + dec)), 0.0f, intens * 10.f * Cos(a*.4f - dec));
                vec3_add(tmpv->origine, tmpv->origine, tmpv->dest);
            }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilDefor::OnDefoY(Ufloat dec) {
    OeilDeforVertex*    tmpv;

    tmpv = AllVertex;

    if (dec > 1.5f) {
        for (U32 a=0; a<NB_POLY_Y+1; a++)
            for (U32 b=0; b<NB_POLY_X+1; b++,tmpv++) {
                Ufloat intens = (Ufloat)a/NB_POLY_Y;
                intens *= intens;

                vec3_set(tmpv->origine, 0.0f, -intens * (0.0f, 50.f + 20.f * Sin(a*.3f + dec)), intens * 10.f * Cos(a*.4f - dec));
                vec3_add(tmpv->origine, tmpv->origine, tmpv->dest);
            }
    } else {
        for (U32 a=0; a<NB_POLY_Y+1; a++)
            for (U32 b=0; b<NB_POLY_X+1; b++,tmpv++) {
                Ufloat intens = 1.0f - (Ufloat)a/NB_POLY_Y;
                intens *= intens;

                vec3_set(tmpv->origine, 0.0f, intens * (50.f + 20.f * Sin(a*.3f + dec)), intens * 10.f * Cos(a*.4f - dec));
                vec3_add(tmpv->origine, tmpv->origine, tmpv->dest);
            }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilDefor::OnDefoZoom(Ufloat dec) {
    OeilDeforVertex*    tmpv;
    Ufloat              distmax;

    Ufloat x = (NB_POLY_X+1) *.5f;
    Ufloat y = (NB_POLY_Y+1) *.5f;

    distmax = sqrt(x*x + y*y)*.8f;

    tmpv = AllVertex;
    for (U32 a=0; a<NB_POLY_Y+1; a++)
        for (U32 b=0; b<NB_POLY_X+1; b++,tmpv++) {
            Ufloat dist = 1.0f - sqrt((a-y)*(a-y) + (b-x)*(b-x))   / distmax;
            if (dist < 0.0f) dist = 0.0f;

            vec3_set(tmpv->origine, 0.0f, dist * 25.f * Cos(b*.3f+ dec), dist * 90.f);
            vec3_add(tmpv->origine, tmpv->origine, tmpv->dest);
        }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilDefor::OnCalcTunel(Ufloat time) {
    OeilDeforVertex*    tmpv;
    Ufloat              distmax;

    Ufloat x = (NB_POLY_X+1) *.5f;
    Ufloat y = (NB_POLY_Y+1) *.5f;

    distmax = sqrt(x*x + y*y);

    tmpv = AllVertex;
    for (U32 a=0; a<NB_POLY_Y+1; a++)
        for (U32 b=0; b<NB_POLY_X+1; b++,tmpv++) {
            Ufloat dist = 1.0f - sqrt((a-y)*(a-y) + (b-x)*(b-x))   / distmax;
            dist *= dist;

            vec3_set(tmpv->dest, tmpv->dest[0], tmpv->dest[1], tmpv->dest[2] - (dist * 700.f - 90.f));
            if (tmpv->dest[2] > zmax) zmax = tmpv->dest[2];
            else if (tmpv->dest[2] < zmin) zmin = tmpv->dest[2];
        }

    DivFact = 16.f; // ralenti interpolation

    incAlpha = -1.f / time;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilDefor::CalcAlpha(Ufloat laptime) {
    OeilDeforVertex*    tmpv;

    Galpha += laptime*incAlpha;
    if (Galpha <0.0f) Galpha = 0.0f;

    Ufloat  alphac = Galpha*Galpha*Galpha*Galpha;

    tmpv = AllVertex;
    for (U32 a=0; a<(NB_POLY_X+1) * (NB_POLY_Y+1); a++,tmpv++) {
        Ufloat pos = (tmpv->origine[2] - zmin) / (zmax - zmin);
        tmpv->alpha = sqrt(pos) * alphac;
    }

}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilDefor::UpdateFrame(Ufloat laptime) {
    if (!enable) return;

    Projet();

    if (calcAlpha) CalcAlpha(laptime);

    AffAll();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
