/*-------------------------------------------------------------------------+
 |                              OeilTextes.cpp                              |
 |                                                                          |
 |  affichage de wiiiz textes par dessus la retine de l'oeil voiliiiii      |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#include "stdafx.h"


#define NB_BLURPI 8

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |    Class OeilTextes   |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
OeilTextes::OeilTextes() {
    Revive = new UImageBlur(5, "Datas/meshes/Oeil/revive.jpg", TRILINEAR_TEXTURE|BILINEAR_TEXTURE);
    Revive->SetTransp(MatTranspAddAlpha);
    Revive->Disable();
    disaRevive = FALSE;

    Vip  = new UImageBlur(5, "Datas/meshes/Oeil/VIP2trash.jpg", TRILINEAR_TEXTURE|BILINEAR_TEXTURE);
    Vip->SetTransp(MatTranspAddAlpha);
    Vip->Disable();
    disaVip = FALSE;

    Vpos = 0;
    Vertical[0] =   new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/h_com.jpg", BILINEAR_TEXTURE);
    Vertical[1] =   new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/h_evil.jpg", BILINEAR_TEXTURE);
    Vertical[2] =   new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/h_eyes.jpg", BILINEAR_TEXTURE);
    Vertical[3] =   new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/h_hurt.jpg", BILINEAR_TEXTURE);
    Vertical[4] =   new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/h_roots.jpg", BILINEAR_TEXTURE);
    Vertical[5] =   new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/h_seek.jpg", BILINEAR_TEXTURE);
    Vertical[6] =   new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/h_waste.jpg", BILINEAR_TEXTURE);
    Vertical[7] =   new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/h_weed.jpg", BILINEAR_TEXTURE);

    Hpos = 0;
    Horizontal[0] = new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/t_back.jpg", BILINEAR_TEXTURE);
    Horizontal[1] = new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/t_clear.jpg", BILINEAR_TEXTURE);
    Horizontal[2] = new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/t_fear.jpg", BILINEAR_TEXTURE);
    Horizontal[3] = new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/t_http.jpg", BILINEAR_TEXTURE);
    Horizontal[4] = new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/t_tear.jpg", BILINEAR_TEXTURE);
    Horizontal[5] = new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/t_today.jpg", BILINEAR_TEXTURE);
    Horizontal[6] = new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/t_void.jpg", BILINEAR_TEXTURE);
    Horizontal[7] = new UImageBlur(NB_BLURPI, "Datas/meshes/Oeil/t_waste.jpg", BILINEAR_TEXTURE);

    for (U32 a=0; a<8; a++) {
        Vertical[a]->Disable();
        Vertical[a]->SetTransp(MatTranspAddAlpha);
        Vertical[a]->SetFactYpos(1.3f);
        Vertical[a]->SetPos(.5f, -Vertical[a]->GetYSize()*.5f);
        Vertical[a]->SetZoom(2.0f);

        Horizontal[a]->Disable();
        Horizontal[a]->SetTransp(MatTranspAddAlpha);
        Horizontal[a]->SetFactXpos(1.3f);
        Horizontal[a]->SetPos(-Horizontal[a]->GetXSize()*.5f, .5f);
        Horizontal[a]->SetZoom(2.0f);
    }


    TexteArabe = new UImage("Datas/meshes/Oeil/Text.jpg", BILINEAR_TEXTURE, 512.f, 512.f);
    TexteArabe->SetTransp(MatTranspAddAlpha);
    TexteArabe->SetYSize(1.0f);
    TexteArabe->SetAlpha(.7f);
//  TexteArabe->InterpoYPos( TexteArabe->GetYpos(), -.1f, 16.f );   // 20 secondes de scroll

    Rond[0] = new UImage("Datas/meshes/Oeil/Rond.jpg");
    Rond[0]->SetTransp(MatTranspAddAlpha);
    Rond[0]->Disable();

    Rond[1] = new UImage;
    (*Rond[1]) = (*Rond[0]);

    Rond[2] = new UImage;
    (*Rond[2]) = (*Rond[0]);

    Rond[3] = new UImage;
    (*Rond[3]) = (*Rond[0]);

}
//----------------------------------------------------------------------------------------------------------------------------------------
OeilTextes::~OeilTextes() {
    delete Revive;
    delete Vip;
    delete TexteArabe;

    for (U32 a=0; a<8; a++) {
        delete Vertical[a];
        delete Horizontal[a];
    }
    for (U32 a=0; a<4; a++)
        delete Rond[a];
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::AppearHori(UImageBlur* ima, Ufloat ypos) {
    ima->SetPos(ima->GetXSize()*.5f, ypos);
    ima->InterpoAlpha(0.0f, 1.0f, 0.1f);
    ima->InterpoXPos(-ima->GetXSize()*.5f, .2f, 0.15f);
    ima->Enable();
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::DisAppearHori(UImageBlur* ima) {
    ima->InterpoAlpha(1.0f, 0.0f, 0.1f);
    ima->InterpoXPos(ima->GetXpos(), -ima->GetXSize()*.5f, 0.15f);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::AppearRond(Ufloat ypos) {
    for (U32 a=0; a<4; a++) {
        Rond[a]->SetPos(0.0f, ypos -.05f + .1f*Random1());
        Rond[a]->InterpoAlpha(1.0f, 0.0f, 0.3f);
        Rond[a]->InterpoXPos(-Rond[a]->GetXSize()*.5f, .4f+.55f*Random1(), 0.25f+.1f*Random1());
        Rond[a]->Enable();
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::AppearVerti(UImageBlur* ima, Ufloat xpos) {
    ima->SetPos(xpos, ima->GetYSize()*.5f);
    ima->InterpoAlpha(0.0f, 1.0f, 0.1f);
    ima->InterpoYPos(-ima->GetYSize()*.5f, .2f, 0.15f);
    ima->Enable();
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::DisAppearVerti(UImageBlur* ima) {
    ima->InterpoAlpha(1.0f, 0.0f, 0.1f);
    ima->InterpoYPos(ima->GetYpos(), -ima->GetYSize()*.5f, 0.15f);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAppearHori() {
    DisAppearHori(Horizontal[Hpos]);

    Hpos += 3;
    Hpos = Hpos%8;

    AppearHori(Horizontal[Hpos], GetUniYpos(415.f));
    AppearRond(GetUniYpos(415.f));
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAppearVerti() {
    DisAppearVerti(Vertical[Vpos]);

    Vpos += 3;
    Vpos = Vpos%8;

    AppearVerti(Vertical[Vpos], GetUniXpos(560.f));
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAppear1Revive(Ufloat time) {
    Revive->Enable();

    Revive->SetFactXZoom(3.5f);
    Revive->SetPos(.5f, .5f);
    Revive->SetYZoom(.5f);
    Revive->InterpoXZoom(10.f, 2.0f, time);
    Revive->InterpoAlpha(0.0f, 1.0f, time);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAppear2Revive(Ufloat time) {
    Revive->SetFactYZoom(3.5f);
    Revive->SetXZoom(1.0f);
    Revive->InterpoYZoom(2.f, 5.0f, time);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAppear3Revive(Ufloat time) {
    Revive->SetFactYZoom(2.0f);
    Revive->SetFactXZoom(2.0f);

    Revive->InterpoAlpha(0.0f, 1.0f, time*.5f);
    Revive->InterpoXZoom(2.f, 1.0f, time);
    Revive->InterpoYZoom(0.5f, 1.0f, time);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnDisAppearRevive(Ufloat time) {
    Revive->InterpoAlpha(1.0f, 0.0f, time);
    Revive->InterpoXZoom(1.f, 4.0f, time);
    Revive->InterpoYZoom(1.0f, 0.2f, time);

    disaRevive = TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAppear1Vip(Ufloat time) {
    Vip->Enable();

    Vip->SetFactXZoom(2.5f);
    Vip->SetFactYZoom(2.5f);

    Vip->SetPos(.75f, .2f);
    Vip->SetXZoom(.5f);
    Vip->InterpoYZoom(0.4f, 3.f, time);
    Vip->InterpoAlpha(1.0f, 0.8f, time);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAppear2Vip(Ufloat time) {
    Vip->SetPos(.15f, .75f);
    Vip->SetXZoom(1.0f);
    Vip->InterpoYZoom(0.4f, 3.f, time);
    Vip->InterpoAlpha(1.0f, 0.8f, time);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAppear3Vip(Ufloat time) {
    Vip->SetPos(.25f, .25f);
    Vip->SetYZoom(0.7f);
    Vip->InterpoXZoom(1.0f, 4.f, time);
    Vip->InterpoAlpha(1.0f, 0.8f, time);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAppear4Vip(Ufloat time) {
    Vip->SetFactXZoom(1.5f);
    Vip->SetFactYZoom(1.5f);

    Vip->SetPos(.5f, .5f);
    Vip->InterpoXZoom(1.0f, 1.0f, 0.001f);
    Vip->InterpoYZoom(3.0f, 1.0f, time);
    Vip->InterpoAlpha(0.0f, 1.0f, time*.8f);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnDisAppearVip(Ufloat time) {
    Vip->InterpoXZoom(1.0f, 0.5f, time);
    Vip->InterpoYZoom(1.0f, 0.25f, time);
    Vip->InterpoAlpha(1.0f, 0.0f, time);

    disaVip = TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::OnAttenuateTexte(Ufloat time) {
    TexteArabe->InterpoAlpha(0.0f, .7f, time);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void OeilTextes::UpdateFrame(Ufloat laptime) {
    Revive->Affiche(laptime);
    if (disaRevive && !Revive->IsInterpoAlpha())
        Revive->Disable();

    Vip->Affiche(laptime);
    if (disaRevive && !Vip->IsInterpoAlpha())
        Vip->Disable();

    //-------------- affiche les petites images
    for (U32 a=0; a<8; a++) {
        Horizontal[a]->Affiche(laptime);

        Vertical[a]->Affiche(laptime);
    }

    //------------- affiche les petits ronds
    for (U32 a=0; a<4; a++) {
        Rond[a]->Affiche(laptime);
        if (!Rond[a]->IsInterpoAlpha())
            Rond[a]->Disable();
    }

    TexteArabe->Affiche(laptime);
}
//----------------------------------------------------------------------------------------------------------------------------------------
