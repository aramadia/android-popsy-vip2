/*-------------------------------------------------------------------------+
 |                              CilsTextes.cpp                              |
 |                                                                          |
 |  Affichage d'une grosse quantité de d'enormes textes a l'ecran poil de   |
 |                                  vent                                    |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#include "stdafx.h"

#define NB_SALVATION    2
#define NB_TEMPTATION   2
#define NB_DISINFO      2
#define NB_TESTI        2
#define NB_FREEDOM      2

//-----------------------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |   Class CilsTextes    |
//                                      +-----------------------+
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------------------------------
CilsTextes::CilsTextes() {
    U32 a=0;

    Salvation = new UImage*[NB_SALVATION];

    Salvation[0] = new UImage("Datas/meshes/Cils/salvation.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    Salvation[0]->SetPos(0.5f, 0.5f);
    Salvation[0]->SetTransp(MatTranspIncrust);

    for (a=0; a<NB_SALVATION-1; a++) {
        Salvation[a+1] = new UImage;
        (*Salvation[a+1]) = (*Salvation[0]);
    }


    Temptation = new UImage*[NB_TEMPTATION];

    Temptation[0] = new UImage("Datas/meshes/Cils/Temptation.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    Temptation[0]->SetPos(0.5f, 0.5f);
    Temptation[0]->SetTransp(MatTranspIncrust);

    for (a=0; a<NB_TEMPTATION-1; a++) {
        Temptation[a+1] = new UImage;
        (*Temptation[a+1]) = (*Temptation[0]);
    }


    Disinfo  = new UImage*[NB_DISINFO];

    Disinfo[0] = new UImage("Datas/meshes/Cils/Disinfo.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    Disinfo[0]->SetPos(0.5f, 0.5f);
    Disinfo[0]->SetTransp(MatTranspIncrust);

    for (a=0; a<NB_DISINFO-1; a++) {
        Disinfo[a+1] = new UImage;
        (*Disinfo[a+1]) = (*Disinfo[0]);
    }


    Testi    = new UImage*[NB_TESTI];

    Testi[0] = new UImage("Datas/meshes/Cils/Testi.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    Testi[0]->SetPos(0.5f, 0.5f);
    Testi[0]->SetTransp(MatTranspIncrust);

    for (a=0; a<NB_TESTI-1; a++) {
        Testi[a+1] = new UImage;
        (*Testi[a+1]) = (*Testi[0]);
    }


    Freedom  = new UImage*[NB_FREEDOM];

    Freedom[0] = new UImage("Datas/meshes/Cils/Testi.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    Freedom[0]->SetPos(0.5f, 0.5f);
    Freedom[0]->SetTransp(MatTranspIncrust);

    for (a=0; a<NB_FREEDOM-1; a++) {
        Freedom[a+1] = new UImage;
        (*Freedom[a+1]) = (*Freedom[0]);
    }


    dates = new UImage("Datas/meshes/Cils/Textebas.jpg", BILINEAR_TEXTURE|ALPHA_TEXTURE);
    dates->OpenMask("Datas/meshes/Cils/Textebas.raw");
    dates->SetTransp(MatTranspAlpha);
    dates->SetPos(dates->GetXSize()*.5f, 1.0f - dates->GetYSize()*.5f);

}
//-----------------------------------------------------------------------------------------------------------------------------------------
CilsTextes::~CilsTextes() {
    U32 a;

    for (a=0; a<NB_SALVATION; a++)
        delete Salvation[a];
    delete []Salvation;

    for (a=0; a<NB_TEMPTATION; a++)
        delete Temptation[a];
    delete [] Temptation;

    for (a=0; a<NB_DISINFO; a++)
        delete Disinfo[a];
    delete [] Disinfo;

    for (a=0; a<NB_TESTI; a++)
        delete Testi[a];
    delete [] Testi;

    for (a=0; a<NB_FREEDOM; a++)
        delete Freedom[a];
    delete [] Freedom;

    delete dates;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void CilsTextes::GereXImage(UImage* ima, Ufloat laptime, bool right) {
    if (!ima->IsInterpoXPos()) {
        ima->SetZoom(.5f + 1.5f*Random1());

        if (right) {
            ima->SetPos(1.0f+ima->GetXSize()*.5f, Random1());
            ima->InterpoXPos(ima->GetXpos(), -ima->GetXSize()*.5f, 1.0f + 2.f*Random1());
        } else {
            ima->SetPos(-ima->GetXSize()*.5f, Random1());
            ima->InterpoXPos(ima->GetXpos(), ima->GetXSize()*.5f + 1.0f, 1.0f + 2.f*Random1());
        }
    }

    ima->Affiche(laptime);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void CilsTextes::GereYImage(UImage* ima, Ufloat laptime, bool haut) {
    if (!ima->IsInterpoYPos()) {
        ima->SetZoom(.5f + 1*Random1());

        if (haut) {
            ima->SetPos(Random1(), 1.0f+ima->GetYSize()*.5f);
            ima->InterpoYPos(ima->GetYpos(), -ima->GetYSize()*.5f, 1.0f + 2.f*Random1());
        } else {
            ima->SetPos(Random1(), -ima->GetXSize()*.5f);
            ima->InterpoXPos(ima->GetYpos(), ima->GetYSize()*.5f + 1.0f, 1.0f + 2.f*Random1());
        }
    }

    ima->Affiche(laptime);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void CilsTextes::Disappear(Ufloat time) {
    U32 a;

    for (a=0; a<NB_SALVATION; a++)
        Salvation[a]->InterpoAlpha(1.0f, 0.0f, time);

    for (a=0; a<NB_TEMPTATION; a++)
        Temptation[a]->InterpoAlpha(1.0f, 0.0f, time);

    for (a=0; a<NB_DISINFO; a++)
        Disinfo[a]->InterpoAlpha(1.0f, 0.0f, time);

    for (a=0; a<NB_TESTI; a++)
        Testi[a]->InterpoAlpha(1.0f, 0.0f, time);

    for (a=0; a<NB_FREEDOM; a++)
        Freedom[a]->InterpoAlpha(1.0f, 0.0f, time);

    dates->InterpoAlpha(1.0f, 0.0f, time);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void CilsTextes::UpdateFrame(Ufloat laptime) {
    U32 a;

    for (a=0; a<NB_SALVATION; a++)
        GereXImage(Salvation[a], laptime, a&0x1);

    for (a=0; a<NB_TEMPTATION; a++)
        GereXImage(Temptation[a], laptime, a&0x1);

    for (a=0; a<NB_DISINFO; a++)
        GereXImage(Disinfo[a], laptime, a&0x1);

    for (a=0; a<NB_TESTI; a++)
        GereYImage(Testi[a], laptime, a&0x1);

    for (a=0; a<NB_FREEDOM; a++)
        GereYImage(Freedom[a], laptime, a&0x1);

    dates->Affiche(laptime);

}
//-----------------------------------------------------------------------------------------------------------------------------------------
