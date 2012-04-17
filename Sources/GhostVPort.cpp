/*-----------------------------------------------------------------+
 |                          GhostVPort.cpp                          |
 |                                                                  |
 |   un truc ki va gérer l'affichage de gnolie viewport de camera   |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/



#include "stdafx.h"


#define NB_CARRE    18

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class GhostVPort |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
GhostCarre::GhostCarre() {
    angle = 0.0f;
    incangle = 0.0f;

    sizex = 0.0f;
    incsizex = 0.0f;

    sizey = 0.0f;
    incsizey = 0.0f;

    alpha = 1.0f;
    incalpha = 0.0f;
    InterpoAlpha = FALSE;

    posx = 0.0f;
    posy = 0.0f;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostCarre::Set(Ufloat sizx, Ufloat destsizex, Ufloat sizy, Ufloat destsizey,
                     Ufloat angl, Ufloat destangle, Ufloat time, bool invalpha) {
    angle = angl;
    Destangle = destangle;
    incangle = (destangle - angl) / time;

    sizex = sizx;
    DestSizex = destsizex;
    incsizex = (destsizex - sizex) / time;

    sizey = sizy;
    DestSizey = destsizey;
    incsizey = (destsizey - sizey) / time;

    if (invalpha) {
        alpha = 1.0f;
        incalpha = -1.0f / time;
        DestAlpha = 0.0f;
    } else {
        alpha = 0.0f;
        incalpha = 1.0f / time;
        DestAlpha = 1.0f;
    }
    InterpoAlpha = TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostCarre::Goto(Ufloat destsizex, Ufloat destsizey, Ufloat destsizeangle, Ufloat destalpha, Ufloat time) {
    DestSizex = destsizex;
    incsizex = (DestSizex - sizex) / time;

    DestSizey = destsizey;
    incsizey = (DestSizey - sizey) / time;

    Destangle = destsizeangle;
    incangle = (Destangle - angle) / time;

    DestAlpha = destalpha;
    incalpha = (DestAlpha - alpha) / time;
    InterpoAlpha = TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostCarre::IncVal(Ufloat& val, Ufloat inc, Ufloat laptime, Ufloat destmax) {
    val += inc* laptime;

    if (inc > 0.0f) {
        if (val > destmax)
            val = destmax;
    } else {
        if (val < destmax)
            val = destmax;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostCarre::Affiche(Ufloat laptime) {
    Ufloat  dx2;
    Ufloat  dy2;
    Ufloat  p1x,p1y;
    Ufloat  p2x,p2y;
    Ufloat  p3x,p3y;
    Ufloat  p4x,p4y;
    Ufloat  RVBA[2][4];
    Ufloat  pos1[3],pos2[3];

//  if( (alpha>1.0f) || (alpha<0.0f) )  return;

    dx2 = sizex * .5f;
    dy2 = sizey * .5f;

    Ufloat COS = cos(angle);
    Ufloat SIN = sin(angle);

    p1x = -dx2 * COS + -dy2 * SIN   + posx;
    p1y = -dx2 * -SIN + -dy2 * COS  + posy;

    p2x = dx2 * COS + -dy2 * SIN    + posx;
    p2y = dx2 * -SIN + -dy2 * COS   + posy;

    p3x = dx2 * COS + dy2 * SIN + posx;
    p3y = dx2 * -SIN + dy2 * COS    + posy;

    p4x = -dx2 * COS + dy2 * SIN    + posx;
    p4y = -dx2 * -SIN + dy2 * COS   + posy;

    vec4_set(RVBA[0], 1.0f, 1.0f, 1.0f, alpha);
    vec4_set(RVBA[1], 1.0f, 1.0f, 1.0f, alpha);

    vec3_set(pos1, p1x, p1y, 0.0f);
    vec3_set(pos2, p2x, p2y, 0.0f);
    U3D3Pipeline->AfficheLigne(pos1, pos2, RVBA);

    vec3_set(pos1, p2x, p2y, 0.0f);
    vec3_set(pos2, p3x, p3y, 0.0f);
    U3D3Pipeline->AfficheLigne(pos1, pos2, RVBA);

    vec3_set(pos1, p3x, p3y, 0.0f);
    vec3_set(pos2, p4x, p4y, 0.0f);
    U3D3Pipeline->AfficheLigne(pos1, pos2, RVBA);

    vec3_set(pos1, p4x, p4y, 0.0f);
    vec3_set(pos2, p1x, p1y, 0.0f);
    U3D3Pipeline->AfficheLigne(pos1, pos2, RVBA);


    IncVal(sizex, incsizex, laptime, DestSizex);
    IncVal(sizey, incsizey, laptime, DestSizey);
    IncVal(angle, incangle, laptime, Destangle);
    IncVal(alpha, incalpha, laptime, DestAlpha);

//  if( alpha = DestAlpha ) InterpoAlpha = FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |  Class GhostOneVPort  |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
GhostOneVPort::GhostOneVPort(MondeU3D* monde, Camera* cam, Ufloat tappear, Ufloat timeReste, Ufloat timeDisa) {
    suiv = NULL;
    Monde = monde;
    Cam = cam;

    blanc = new UImage;
    blanc->SetTransp(MatTranspAddAlpha);
    blanc->SetSize(1.0f);
    blanc->InterpoAlpha(0.0f, 1.0f, tappear);

    Appear = TRUE;
    Tappear  = tappear;
    Treste = timeReste;

    Disappear = FALSE;
    TDisappear = timeDisa;

    carres = new GhostCarre[ NB_CARRE ];

    Ufloat sizex = (Ufloat)cam->GetViewport().GetDx() / Xres;
    Ufloat sizey = (Ufloat)cam->GetViewport().GetDy() / Yres;
    for (U32 a=0; a<NB_CARRE; a++) {
        carres[a].SetPos((Ufloat)cam->GetViewport().GetCentreX() / Xres, 1.f-(Ufloat)cam->GetViewport().GetCentreY()/ Yres);

        Ufloat inc = (Ufloat)a/NB_CARRE + .1f;
        Ufloat tfact = (.2f + .8f*(Ufloat)a/NB_CARRE) * tappear;
        carres[a].Set(sizex + sizex*inc*1.0f, sizex, sizey + sizey*1.0f, sizey, 1.0f*(Ufloat)a/NB_CARRE, 0.0f, tfact);
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
GhostOneVPort::~GhostOneVPort() {
    delete blanc;
    delete [] carres;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostOneVPort::DisaCarres(Ufloat time) {
    Ufloat sizex = (Ufloat)Cam->GetViewport().GetDx() / Xres;
    Ufloat sizey = (Ufloat)Cam->GetViewport().GetDy() / Yres;
    for (U32 a=0; a<NB_CARRE; a++) {
        Ufloat inc = (Ufloat)a/NB_CARRE + .1f;
        Ufloat tfact = (.2f + .8f*(Ufloat)a/NB_CARRE) * time;
        carres[a].Set(sizex,  sizex + sizex*inc*1.0f, sizey, sizey + inc*sizey*1.0f, 0.0f, 1.0f*(Ufloat)a/NB_CARRE, tfact, TRUE);
//      carres[a].Set( sizex,  sizex , sizey, sizey , 0.0f, 0.0f, 10.f, TRUE );
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostOneVPort::Affiche(Ufloat laptime) {
    U32 a=0;


    if (Appear) {

        Tappear -= laptime;

        if (Tappear <=0.0f) {
            Appear  = FALSE;
            blanc->InterpoAlpha(1.0f, 0.0f, Treste*.1f);
        }
    } else if (Disappear) {
        /*      U3D3Pipeline->Begin( MatTranspAddAlpha );
                    for( a=0; a<NB_CARRE; a++ )
                        carres[a].Affiche( laptime );
                U3D3Pipeline->End( NULL );*/

        U3D3Pipeline->PushState();
        U3D3Pipeline->SetActuaViewport(Cam->GetViewport());     // pour afficher l'image dans le meme viewport ke la scene
        blanc->Affiche(laptime);
        U3D3Pipeline->PopState();
    } else {
        Camera* save = Monde->GetActualCamera();
        Monde->SetActualCamera(Cam);

        ClearOpenGLZbuffer();   // efface kan meme le Zbuff pour eviter toute interference

        Monde->UpdateFrame(0.0f);

        U3D3Pipeline->PushState();
        U3D3Pipeline->SetActuaViewport(Cam->GetViewport());     // pour afficher l'image dans le meme viewport ke la scene
        blanc->Affiche(laptime);
        U3D3Pipeline->PopState();

        if (!blanc->IsInterpoAlpha())
            blanc->Disable();

        Treste -= laptime;
        if (Treste<=0.0f) {
            blanc->InterpoAlpha(1.0f, 0.0f, TDisappear);
            blanc->InterpoXZoom(blanc->GetXZoom(), 0.0f, TDisappear);
            blanc->InterpoYZoom(blanc->GetYZoom(), 5.f*blanc->GetYZoom(), TDisappear);
            blanc->Enable();

            DisaCarres(TDisappear);

            Disappear=TRUE;
        }

        Monde->SetActualCamera(save);
    }

    /*  Ufloat sizex = GetUniXpos( Cam->GetViewport().GetDx() );
        Ufloat sizey = GetUniYpos( Cam->GetViewport().GetDy() );*/
    U3D3Pipeline->Begin(MatTranspAddAlpha);
    for (a=0; a<NB_CARRE; a++) {
        carres[a].Affiche(laptime);
        /*          if( !carres[a].IsInterpo() )
                    {
                        Ufloat inc = (Ufloat)a/NB_CARRE + .1f;
                        Ufloat tfact = ( .2f + .8f*(Ufloat)a/NB_CARRE) * .5f;
                        carres[a].Goto( sizex, sizey, .2f*(Ufloat)a/NB_CARRE*cos( ClockU3D::GetTime() ), .6f + .4f*Random1(), tfact );
                    }*/
    }
    U3D3Pipeline->End(NULL);
}
//----------------------------------------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class GhostVPort |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
GhostVPort::GhostVPort() {
    allport = NULL;
}
//----------------------------------------------------------------------------------------------------------------------------------------
GhostVPort::~GhostVPort() {
    GhostOneVPort* tmp;

    while (allport) {
        tmp = allport->suiv;
        delete allport;
        allport = tmp;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostVPort::Add(MondeU3D* monde, Camera* cam, Ufloat tappear, Ufloat timeReste, Ufloat timeDisa) {
    GhostOneVPort* tmp = new GhostOneVPort(monde, cam, tappear, timeReste, timeDisa);

    tmp->suiv = allport;
    allport = tmp;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void GhostVPort::Affiche(Ufloat laptime) {
    GhostOneVPort* tmp;
    GhostOneVPort* preced=NULL;

    tmp = allport;
    while (tmp) {
        tmp->Affiche(laptime);
        if (tmp->IsFinish()) {
            if (preced) {
                preced->suiv = tmp->suiv;
                delete tmp;
                tmp = preced->suiv;
            } else {
                allport = tmp->suiv;
                delete tmp;
                tmp = allport;
            }

        } else {
            preced = tmp;
            tmp = tmp->suiv;
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
