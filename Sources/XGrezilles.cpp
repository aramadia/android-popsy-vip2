/*---------------------------------------------------------------------+
 |                       XGrezilles.cpp                                 |
 |                                                                      |
 |          affichage d'un léger fond  pour la scene des neuronnes      |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +-----------------------+
//                                  |   Variables Locales   |
//                                  +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

TextureU3D*     XGrezille::TexPic1 = NULL;
MaterialU3D*    XGrezille::MatPic1 = NULL;
BitmapU3D*      XGrezille::GBmp =  NULL;


//-----------------------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |  Class XGrezillePic   |
//                                      +-----------------------+
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------------------------------
void XGrezillePic::set(Ufloat p, Ufloat spd, Ufloat s, Ufloat alph, bool l) {
    pos = p;
    speed = spd;
    size = s;
    alpha = alph;
    left = l;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void XGrezillePic::Affiche(Ufloat laptime, BitmapU3D* bmp, MaterialU3D* mat, Ufloat dx, Ufloat dy, Ufloat Galpha) {
    Ufloat dx2,dy2;
    Ufloat xpos;
    Ufloat inv;

    dx2 = dx*size*.5f;
    dy2 = dy*size*.5f;

    pos += speed*laptime;

    if (speed>0.0f) if (pos > dx2+1.0f) pos = -dx2;
        else                if (pos < -dx2)  pos = dx2+1.0f;

    if (left) {
        xpos = dx2;
        inv = 1.0f;
    } else {
        xpos = 1.f - dx2;
        inv = -1.0f;
    }

    mat->SetColor(1.0f, 1.0f, 1.0f, Galpha*alpha);

    bmp->SetCoord(0, xpos - inv*dx2, pos - dy2);
    bmp->SetCoord(1, xpos + inv*dx2, pos - dy2);
    bmp->SetCoord(2, xpos + inv*dx2, pos + dy2);
    bmp->SetCoord(3, xpos - inv*dx2, pos + dy2);

    bmp->Affiche();
}
//-----------------------------------------------------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |    Class XGrezille    |
//                                      +-----------------------+
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------------------------------
void XGrezille::Initialise() {
    GBmp = new BitmapU3D(NULL);

    TextureManager::SetRepertoire("");
    TexPic1 = TextureManager::OpenJpg("Datas/2d/GPic1.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    TexPic1->IncUser();

    MatPic1 = new MaterialU3D("MatPic1");
    MatPic1->SetTexture1(TexPic1);
    MatPic1->SetTransp(MatTranspAddAlpha);
    MatPic1->SetShading(MatShadingRien);
    MatPic1->Confirm();

}
//-----------------------------------------------------------------------------------------------------------------------------------------
void XGrezille::Release() {
    DeleteBlinde(MatPic1);
    DeleteBlinde(GBmp);

    TextureManager::SubUse(TexPic1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
XGrezille::XGrezille(U32 nbpic1) {
    Alpha = 1.0f;
    IncAlpha = 0.0f;
    GotoAlpha = 1.0f;


    nbPic1 = nbpic1;
    Pic1 = new  XGrezillePic[ nbpic1 ];

    for (U32 a=0; a<nbPic1; a++)
        Pic1[a].set(Random1(), Random1()*4.f - 2.f, Random1()*.7f + .3f, Random1()*.7f + .3f, a&0x1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
XGrezille::~XGrezille() {
    delete [] Pic1;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void XGrezille::Interposalpha(Ufloat debut, Ufloat fin, Ufloat time) {
    Alpha = debut;
    GotoAlpha = fin;

    IncAlpha = (fin - debut) / time;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void XGrezille::UpdateFrame(Ufloat laptime) {
    U32     a;
    Ufloat  dxp1,dyp1;

    Alpha += laptime*IncAlpha;
    if (IncAlpha>0.0f) {
        if (Alpha > GotoAlpha) Alpha = GotoAlpha;
    } else {
        if (Alpha < GotoAlpha) Alpha = GotoAlpha;
    }

    if (Alpha < 0.01f)   return;

    dxp1 = (Ufloat)TexPic1->Width() / DEFAULT_SCREEN_X;
    dyp1 = (Ufloat)TexPic1->Height() / DEFAULT_SCREEN_Y;

    U3D3Pipeline->Begin(MatPic1);

    for (a=0; a<nbPic1; a++)
        Pic1[a].Affiche(laptime, GBmp, MatPic1, dxp1, dyp1, Alpha);

    U3D3Pipeline->End(MatPic1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
