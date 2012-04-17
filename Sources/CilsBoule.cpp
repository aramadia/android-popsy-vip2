/*-------------------------------------------------------------------------+
 |                              CilsBoule.cpp                               |
 |                                                                          |
 |  une simple sphere remplit de gnolie peits cils tout velus et gigotants  |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class CilsBoule  |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
CilsBoule::CilsBoule(Ufloat xp, Ufloat yp, Ufloat zp, U32 nbpoils, Ufloat dmin, Ufloat delta, U32 enfant) {
    U32         a;
    Ufloat      avance;
    Ufloat      vec[3];

    suiv = NULL;
    fils = NULL;

    NbPoils = nbpoils;
    Poils = new CilsPoil[ nbpoils ];

    //------------ set position des matrices
    for (a=0; a<CILS_NBCTRLPTS; a++)
        Mats[a].SetPos(xp, yp, zp);

    //------------ va calculer positions d'origines des poils
    avance = 0.0f;
    for (a=0; a<nbpoils; a++,avance+=(Ufloat)1.f/nbpoils) {
        vec[1] = 2.f* avance - 1.f;
        vec[0] = (1.f-fabs(vec[1]))*cos(avance*M_PI*18.f);
        vec[2] = (1.f-fabs(vec[1]))*sin(avance*M_PI*18.f);
        vec3_normalize(vec);

        for (U32 b=0; b<CILS_NBCTRLPTS; b++) {
            vec3_mul(Poils[a].origine[b], vec, dmin + delta * (Ufloat)b / (CILS_NBCTRLPTS-1));
        }

        Poils[a].poil->SetStartColor(.2f, .4f, .8f);
        Poils[a].poil->SetEndColor(.05f, .1f, .25f);
        Poils[a].partiPos = Random1();
    }

    /*  //---------------- va créer les enfants
        if( enfant )
        {
            tmp = new CilsBoule( dmin+delta, 0.0f, 0.0f, nbpoils>>1, dmin*.5f, delta*.5f, enfant-1 );
            tmp->suiv = fils;
            fils = tmp;

            tmp = new CilsBoule( -(dmin+delta), 0.0f, 0.0f, nbpoils>>1, dmin*.5f, delta*.5f, enfant-1 );
            tmp->suiv = fils;
            fils = tmp;

            tmp = new CilsBoule( 0.0f, dmin+delta, 0.0f, nbpoils>>1, dmin*.5f, delta*.5f, enfant-1 );
            tmp->suiv = fils;
            fils = tmp;

            tmp = new CilsBoule( 0.0f, -(dmin+delta), 0.0f, nbpoils>>1, dmin*.5f, delta*.5f, enfant-1 );
            tmp->suiv = fils;
            fils = tmp;
        }*/
}
//----------------------------------------------------------------------------------------------------------------------------------------
CilsBoule::~CilsBoule() {
    CilsBoule* tmp;

    delete [] Poils;

    while (fils) {
        tmp = fils->suiv;
        delete fils;
        fils = tmp;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CilsBoule::Affiche(Mat3x4& mat, Ufloat time, Ufloat laptime, Ufloat alpha) {
    CilsBoule*  tmp;
    Ufloat      vec[3];
    Mat3x4      MatTransfo[CILS_NBCTRLPTS];

    //-------------- calcul animation des matrices
    for (U32 a=0; a<CILS_NBCTRLPTS; a++) {
        vec3_set(vec, cos(time - a*.18f), sin(time - a*.11f), 0.0f);
        vec3_normalize(vec);
        Quat q(vec, time*2.f - a*.17f);

        q.matrix(Mats[a]);

        MatTransfo[a] =     Mats[a];
        MatTransfo[a] *=    mat;
    }

    //------------- Transform tout les controls points et affiche la spline
    for (U32 a=0; a<NbPoils; a++) {
        for (U32 b=0; b<CILS_NBCTRLPTS; b++) {
            MatTransfo[b].GetTransfo(vec, Poils[a].origine[b]);
            Poils[a].poil->SetKeyPos(b, vec[0], vec[1], vec[2]);
        }

        Poils[a].poil->SetStartAlpha(alpha);
        Poils[a].poil->SetEndAlpha(alpha);
        Poils[a].poil->Affiche();
        Poils[a].partiPos += laptime;
        if (Poils[a].partiPos > 1.0f)
            Poils[a].partiPos = 0.0f;
    }

    //----------- affiche les fils
    tmp = fils;
    while (tmp) {
        tmp->Affiche(Mats[CILS_NBCTRLPTS-1], time, laptime, alpha);
        tmp = tmp->suiv;
    }

}
//----------------------------------------------------------------------------------------------------------------------------------------
void CilsBoule::GetPoilsPartiPos(Ufloat& x, Ufloat& y, Ufloat& z, Ufloat& alpha) {
    Ufloat tmp[7];

    alpha = 1.f - NextPoil->partiPos;
    NextPoil->poil->GetVal(tmp, NextPoil->partiPos);

    x = tmp[0];
    y = tmp[1];
    z = tmp[2];

    NextPoil ++;
}
//----------------------------------------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      | Class CilsSpheres |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
CilsSpheres::CilsSpheres() {
    Bmp = new BitmapU3D(NULL);

    TextureManager::SetRepertoire("");
    Tex = TextureManager::OpenJpg("Datas/meshes/Cils/parti.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE);
    Tex->IncUser();

    Mat = new MaterialU3D("Mat");
    Mat->SetTexture1(Tex);
    Mat->SetTransp(MatTranspAddAlpha);
    Mat->SetShading(MatShadingRien);
    Mat->Confirm();

    AllBoule = new CilsBoule(0.0f, 0.0f, 0.0f, 150, 00.f, 100.f, 1);

    interpoAlpha = FALSE;
    alpha = 1.0f;
    destalpha = 1.0f;
    incalpha = 0.0f;

    enable = TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
CilsSpheres::~CilsSpheres() {
    delete Mat;
    delete Bmp;
    delete AllBoule;

    TextureManager::SubUse(Tex);
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CilsSpheres::AffParti(Ufloat x, Ufloat y, Ufloat z, Ufloat zoom) {
    CoordU3D            tmpc;
    Ufloat              size;
    Camera*             cam = U3D3Monde3D->GetActualCamera();

    tmpc.frame_process = -1;
    vec3_set(tmpc.trans, x, y, z);
    cam->ProjetUni(&tmpc, 0);

    if (!(tmpc.ClipInfo&ClipProche)) {
        size = zoom * tmpc.ecran[3] * .5f;

        Bmp->SetCoord(0, tmpc.ecran[0] - size, tmpc.ecran[1] - 1.33f*size);
        Bmp->SetCoord(1, tmpc.ecran[0] + size, tmpc.ecran[1] - 1.33f*size);
        Bmp->SetCoord(2, tmpc.ecran[0] + size, tmpc.ecran[1] + 1.33f*size);
        Bmp->SetCoord(3, tmpc.ecran[0] - size, tmpc.ecran[1] + 1.33f*size);

        Bmp->Affiche();
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CilsSpheres::InterpoAlpha(Ufloat debut, Ufloat fin, Ufloat time) {
    alpha = debut;
    destalpha = fin;
    incalpha = (fin - debut) / time;
    interpoAlpha = TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CilsSpheres::Affiche(Ufloat time, Ufloat laptime) {
    Mat3x4  mdefault;

    if (!enable) return;

    if (interpoAlpha) {
        alpha += laptime*incalpha;
        if (incalpha > 0.0f)    {
            if (alpha > destalpha) {
                alpha=destalpha;
                interpoAlpha=FALSE;
            }
        } else                  {
            if (alpha < destalpha) {
                alpha=destalpha;
                interpoAlpha=FALSE;
            }
        }
    }

    AllBoule->Affiche(mdefault, time*.5f, laptime*.5f, alpha);


    U3D3Pipeline->Begin(Mat);

    Mat->SetAlpha(alpha);
    AffParti(0.0f, 0.0f, 0.0f, 35.f);

    AllBoule->ResetPoilsParcours();
    for (U32 i=0; i<AllBoule->GetNbPoils(); i++) {
        Ufloat x,y,z,a;

        AllBoule->GetPoilsPartiPos(x, y, z, a);
        Mat->SetAlpha(a*alpha*.9f);
        AffParti(x, y, z, 5.f);
    }

    U3D3Pipeline->End(Mat);

}
//----------------------------------------------------------------------------------------------------------------------------------------
