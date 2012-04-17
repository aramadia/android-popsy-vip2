/*-------------------------------------------------------------------------+
 |                              CilsBack.cpp                                |
 |                                                                          |
 |  affiche un fantastik background ki bouge et tout comme il fo waaaaaaaa  |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#include "stdafx.h"


#define NB_XRAY 40
#define NB_YRAY 40

#define RAYON_TUNEL 800.f

//-----------------------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |   Class CilsBack      |
//                                      +-----------------------+
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------------------------------
CilsBack::CilsBack() {
    Tex = TextureManager::OpenJpg("Datas/meshes/Cils/Fond.jpg", BILINEAR_TEXTURE);
    Tex1 = TextureManager::OpenJpg("Datas/meshes/Cils/mapmul.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE|TILE_TEXTURE);
    Tex->IncUser();
    Tex1->IncUser();

    Mat = new MaterialU3D("partMat");
    Mat->SetTexture1(Tex);
    Mat->SetTexture2(Tex1);
    Mat->SetTranspTex2(MatTranspMul);
    Mat->SetShading(MatShadingRien);
    Mat->Confirm();

    Bmp = new BitmapU3D(Mat);

    cam = new Camera("ploum");

    tab = new CilsRaycast[(NB_XRAY+1) * (NB_YRAY+1) ];     // les rayons

    CilsRaycast* tmp = tab;
    for (U32 a=0; a<NB_YRAY+1; a++)
        for (U32 b=0; b<NB_XRAY+1; b++,tmp++) {
            tmp->U = tmp->posx = (Ufloat)b/NB_XRAY;
            tmp->V = tmp->posy = (Ufloat)a/NB_YRAY;
        }

    moveTun = 0.0f;


    DisappearMode = FALSE;
    Tdisappear = 0.0f;
    ActuaTdisappear = 0.0f;

}
//-----------------------------------------------------------------------------------------------------------------------------------------
CilsBack::~CilsBack() {
    delete Mat;
    delete Bmp;

    TextureManager::SubUse(Tex);
    TextureManager::SubUse(Tex1);

    delete cam;

    delete [] tab;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void CilsBack::CalcRays() {
    Ufloat          vec[3];
    Ufloat          vec1[3];
    Ufloat          xpos,ypos;
    Ufloat          incxpos,incypos;
    CilsRaycast*    tmp;

    xpos = 0.0f;
    ypos = 0.0f;
    incxpos = 1.f/NB_XRAY;
    incypos = 1.f/NB_YRAY;

    tmp = tab;
    for (U32 a=0; a<NB_YRAY+1; a++,ypos+=incypos) {
        xpos= 0.0f;
        for (U32 b=0; b<NB_XRAY+1; b++,tmp++,xpos+=incxpos) {
            vec3_set(vec, xpos, ypos, 100.f);
            cam->Cam2MondePos(vec, vec);

            vec3_sub(vec1, vec, cam->GetPos());
            vec3_eg(vec, cam->GetPos());

            Ufloat parama  = vec1[0]*vec1[0] + vec1[1]*vec1[1];
            Ufloat paramb =  vec[0]*vec1[0] + vec[1]*vec1[1];
            Ufloat paramc = vec[0]*vec[0] + vec[1]*vec[1] - RAYON_TUNEL*RAYON_TUNEL;
            Ufloat delta = paramb*paramb - 4*parama*paramc;
            if (delta<0) delta=0;
            Ufloat t = (-paramb + (float)sqrt(delta)) / (2*parama);

            Ufloat x = vec[0] + vec1[0]*t;
            Ufloat y = vec[1] + vec1[1]*t;
            Ufloat z = vec[2] + vec1[2]*t;

            tmp->U1 = (Ufloat)fabs((atan2(y,x)/M_PI));
            tmp->V1 = z * .00015f + moveTun;
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void CilsBack::AffRays() {
    CilsRaycast* tmp;

    tmp = tab;
    for (U32 a=0; a<NB_YRAY; a++) {
        for (U32 b=0; b<NB_XRAY; b++,tmp++) {
            Bmp->SetCoord(0, tmp->posx, tmp->posy);
            Bmp->SetMapCoord(0, tmp->U, tmp->V);
            Bmp->SetMapCoord(0, tmp->U1, tmp->V1, 1);

            Bmp->SetCoord(1, (tmp+1)->posx, (tmp+1)->posy);
            Bmp->SetMapCoord(1, (tmp+1)->U, (tmp+1)->V);
            Bmp->SetMapCoord(1, (tmp+1)->U1, (tmp+1)->V1, 1);

            Bmp->SetCoord(2, (tmp+NB_XRAY+2)->posx, (tmp+NB_XRAY+2)->posy);
            Bmp->SetMapCoord(2, (tmp+NB_XRAY+2)->U, (tmp+NB_XRAY+2)->V);
            Bmp->SetMapCoord(2, (tmp+NB_XRAY+2)->U1, (tmp+NB_XRAY+2)->V1, 1);

            Bmp->SetCoord(3, (tmp+NB_XRAY+1)->posx, (tmp+NB_XRAY+1)->posy);
            Bmp->SetMapCoord(3, (tmp+NB_XRAY+1)->U, (tmp+NB_XRAY+1)->V);
            Bmp->SetMapCoord(3, (tmp+NB_XRAY+1)->U1, (tmp+NB_XRAY+1)->V1, 1);

            Bmp->Affiche();
        }
        tmp++;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// va faire dispparaitre ce joli fond !
void CilsBack::Disappear(Ufloat time) {
    Tdisappear = time;
    ActuaTdisappear = time;
    DisappearMode = TRUE;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void CilsBack::UpdateFrame(Ufloat time, Ufloat laptime) {
    moveTun -= laptime*.2f;
    ActuaTdisappear -= laptime;

    if (!(DisappearMode && (ActuaTdisappear<0.0f))) {
        cam->UpdateProjectMatrix();
        cam->UpdateTransMatrix();
        cam->UpdatePlans();

        if (DisappearMode) {
            Ufloat c = ActuaTdisappear / Tdisappear;
            Mat->SetColor(c, c, c);
        }

        CalcRays();

        U3D3Pipeline->Begin(Mat);

        AffRays();

        U3D3Pipeline->End(Mat);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
