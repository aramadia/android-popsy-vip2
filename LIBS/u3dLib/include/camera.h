/*-----------------------------------------------------+
 |                       camera.h                       |
 |                                                      |
 |    Une camera c'est indispensable pour un moteur     |
 |    3D digne de se nom c'est pourquoa ya a ici        |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _camera_U3D3_H
#define _camera_U3D3_H

extern U32  Xres,Yres;  // from video.cpp

//==================
// definition d'un ViewPort


class Viewport {

    U32     Dx;
    U32     Dy;
    Ufloat  midDx;   // = DX*.5f
    Ufloat  midDy;   // = DY*.5f
    Ufloat  centreX;
    Ufloat  centreY;

public:

    Viewport();
    Viewport(U32 x1, U32 x2, U32 y1, U32 y2);

    void Set(U32 x1, U32 x2, U32 y1, U32 y2);
    void SetCentre(Ufloat x, Ufloat y);
    void SetUni(Ufloat x1, Ufloat x2, Ufloat y1, Ufloat y2);      // a partir de coordonnées unitaires
    void SetCentreUni(Ufloat x, Ufloat y);     // a partir de coordonnées unitaires

    U32     GetDx()         {
        return Dx;
    }
    U32     GetDy()         {
        return Dy;
    }
    Ufloat  GetDxSur2()     {
        return midDx;
    }
    Ufloat  GetDySur2()     {
        return midDy;
    }
    Ufloat  GetCentreX()    {
        return centreX;
    }
    Ufloat  GetCentreY()    {
        return centreY;
    }
    Ufloat  GetMinX()       {
        return centreX-midDx;
    }
    Ufloat  GetMaxX()       {
        return centreX+midDx;
    }
    Ufloat  GetMinY()       {
        return centreY-midDy;
    }
    Ufloat  GetMaxY()       {
        return centreY+midDy;
    }

};





//================================
// definition d'une Camera

class Camera;

#define ProcheZClipping    2.f
#define LointainZClipping  65536.f
#define ClipPlaneEpsilon   .01f

typedef void (*PROCCAMERAANIMCALLBACK)(Camera* cam, Ufloat step, U32 user);


class Camera {

    CString nom;

    Ufloat pos[3];
    Ufloat targ[3];
    Ufloat vue[3];
    Ufloat roll;
    Ufloat fov;

    Ufloat Trans[4][4];
    Ufloat Project[4][4];
    Ufloat Final[4][4];

    Plan gauche,droite,haut,bas,proche;

    Viewport viewport;   // viewport de la camera

//  Ufloat screenscalex,screenscaley;   // info pour le clipping

    SplineU3D*              Spos;   // mouvement dans le temps
    SplineU3D*              Starg;   // mouvement target dans le temps
    SplineU3D*              Sroll;   // mouvement roll dans le temps
    SplineU3D*              Sfov;   // mouvement fov dans le temps
    PROCCAMERAANIMCALLBACK  callbackAnim;   // fonction appelée pour l'animation

public:

    Camera(LPCSTR Nom);
//  Camera(S8 *Nom, Ufloat sx, Ufloat sy );
//  Camera( FILE *f, Ufloat sx, Ufloat sy );
    ~Camera();

    bool Serialize(CArchive& ar);

    Camera* operator=(Camera* cam);

    LPCSTR Nom()    {
        return (LPCSTR)nom;
    }

    void    SetPos(Ufloat p[3])                     {
        vec3_eg(pos, p);
    }
    void    SetPos(Ufloat x, Ufloat y, Ufloat z)    {
        vec3_set(pos, x, y, z);
    }
    Ufloat* GetPos()                                {
        return &pos[0];
    }

    void    SetTarg(Ufloat t[3])                    {
        vec3_eg(targ, t);
    }
    void    SetTarg(Ufloat x, Ufloat y, Ufloat z)   {
        vec3_set(targ, x, y, z);
    }
    Ufloat* GetTarg()                               {
        return &targ[0];
    }

    Ufloat* GetBaseX()  {
        return Trans[0];
    }
    Ufloat* GetBaseY()  {
        return Trans[1];
    }
    Ufloat* GetBaseZ()  {
        return Trans[2];
    }

    void    CalcVue()   {
        vec3_sub(vue, targ, pos);
        vec3_normalize(vue);
    }
    Ufloat* GetVue()    {
        return vue;
    }

    void    SetRoll(Ufloat r)   {
        roll=r;
    }
    Ufloat  GetRoll()           {
        return roll;
    }
    void    SetFov(Ufloat f)    {
        if (f<.1) f=(Ufloat).1;
        if (f>179) f=179;
        fov=f;
    }
    Ufloat  GetFov()            {
        return fov;
    }


    void        Set(Viewport& vi);
    Viewport&   GetViewport()       {
        return viewport;
    }
    Ufloat      GetGauche2DClip()   {
        return viewport.GetMinX();
    }
    Ufloat      GetDroite2DClip()   {
        return viewport.GetMaxX();
    }
    Ufloat      GetBas2DClip()      {
        return viewport.GetMaxY();
    }
    Ufloat      GetHaut2DClip()     {
        return viewport.GetMinY();
    }
/// void SetClipFenetre( Ufloat Xlarg, Ufloat Yhauteur );

    Plan* GetPlanDroit()    {
        return &droite;
    }
    Plan* GetPlanGauche()   {
        return &gauche;
    }
    Plan* GetPlanBas()      {
        return &bas;
    }
    Plan* GetPlanHaut()     {
        return &haut;
    }
    Plan* GetPlanProche()   {
        return &proche;
    }

    void UpdatePlans();
    void UpdateTransMatrix();
    void UpdateProjectMatrix();

    //  renvoie le code de clipping que subit la bounding box ( ki possede comme matrice de transfo mat )
    //  pourêtre affichée sur l'écran
    U32 ClipInfo(BoundingBox* b, Mat3x4& mat, U32 flag=ClipAll);

    // transforme les coords d'un vecteur dans le monde de la camera
    // vers des coordonnées dans le modne générale
    void Cam2Monde(Ufloat Vdest[3], const Ufloat Vsrce[3]);
    void Cam2MondePos(Ufloat Vdest[3], const Ufloat Vsrce[3]);     // donne des coordonnées en X,Y,Z camera pour les transformer dans les X,Y,Z monde

    void Projet(CoordU3D* v, U32 frame);     // calcul les coordonnées 2D de la coordonnée
    void Projet(VertexU3D* v, U32 frame);     // calcul les coordonnées 2D du vertex
    void ProjetUni(CoordU3D* v, U32 frame);     // calcul les coordonnées 2D de la coordonnée ( mais en valeur unitaire )
    bool ProjetUni(Ufloat vdest[4], Ufloat vsrc[3]);     // calcul les coordonnées 2D de la coordonnée ( mais en valeur unitaire ) ( renvoie FALSE si point derrière la camera )

    void IniAnim();
    void IncAnim(Ufloat nb);

    void SetAnimCallBack(PROCCAMERAANIMCALLBACK proc)  {
        callbackAnim=proc;
    }
};




//----------------------------------------------------------------------------------------------------------
//                                  +------------------------+
//                                  |    Fonctions Inlines   |
//                                  +------------------------+
//----------------------------------------------------------------------------------------------------------
//////////////////// Class ViewPort
//----------------------------------------------------------------------------------------------------------
inline Viewport::Viewport() {
    Dx = Xres-1;
    Dy = Yres-1;
    centreX = midDx = Dx*.5f;
    centreY = midDy = Dy*.5f;
}
//----------------------------------------------------------------------------------------------------------
inline Viewport::Viewport(U32 x1, U32 x2, U32 y1, U32 y2) {
    Dx = x2 - x1 + 1;
    Dy = y2 - y1 + 1;
    midDx = Dx*.5f;
    midDy = Dy*.5f;
    centreX = (x2 + x1 + 1)*.5f;
    centreY = (y2 + y1 + 1)*.5f;
}
//----------------------------------------------------------------------------------------------------------
void inline Viewport::Set(U32 x1, U32 x2, U32 y1, U32 y2) {
    Dx = x2 - x1;
    Dy = y2 - y1;
    midDx = Dx*.5f;
    midDy = Dy*.5f;
    centreX = (x2 + x1)*.5f;
    centreY = (y2 + y1)*.5f;
}
//----------------------------------------------------------------------------------------------------------
void inline Viewport::SetCentre(float x, float y) {
    centreX = x;
    centreY = y;
}
//----------------------------------------------------------------------------------------------------------
// a partir de coordonnées unitaires
void inline Viewport::SetUni(Ufloat x1, Ufloat x2, Ufloat y1, Ufloat y2) {
    Dx = (int)((x2 - x1)*Xres-1);
    Dy = (int)((y2 - y1)*Yres-1);
    midDx = Dx*.5f;
    midDy = Dy*.5f;
    centreX = ((x2 + x1)*Xres-1)*.5f;
    centreY = ((y2 + y1)*Yres-1)*.5f;
}
//----------------------------------------------------------------------------------------------------------
// a partir de coordonnées unitaires
void inline Viewport::SetCentreUni(Ufloat x, Ufloat y) {
    centreX = x*Xres;
    centreY = y*Yres;
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
//////////////////// Class Camera
//----------------------------------------------------------------------------------------------------------
void inline Camera::Projet(CoordU3D* c, U32 frame) {
    Ufloat x,y,z,tmp[3];

    // la coordonnée n'a pas déja été calculée
    if (frame != c->frame_process) {
        vec3_sub(tmp, c->trans, pos);
        x = vec3_dot(tmp, Final[0]);
        y = vec3_dot(tmp, Final[1]);
        z = vec3_dot(tmp, Final[3]);
        if (z>(ProcheZClipping-ClipPlaneEpsilon)) {
            c->ecran[2] = z;
            c->ecran[3] = z = 1.f/z;
            c->ecran[0] = z * x * viewport.GetDxSur2() + viewport.GetCentreX();
            c->ecran[1] = z * y * viewport.GetDySur2() + viewport.GetCentreY();
            c->ClipInfo = ClipNo;
            if (c->ecran[0] > viewport.GetMaxX()) c->ClipInfo |= ClipDroit;
            else if (c->ecran[0] < viewport.GetMinX()) c->ClipInfo |= ClipGauche;
            if (c->ecran[1] > viewport.GetMaxY()) c->ClipInfo |= ClipBas;
            else if (c->ecran[1] < viewport.GetMinY()) c->ClipInfo |= ClipHaut;
        } else {
            c->ClipInfo = ClipProche;
        }
        c->frame_process = frame;
    }
}
//----------------------------------------------------------------------------------------------------------
void inline Camera::ProjetUni(CoordU3D* c, U32 frame) {
    Ufloat x,y,z,tmp[3];

    // la coordonnée n'a pas déja été calculée
    if (frame != c->frame_process) {
        vec3_sub(tmp, c->trans, pos);
        x = vec3_dot(tmp, Final[0]);
        y = vec3_dot(tmp, Final[1]);
        z = vec3_dot(tmp, Final[3]);
        if (z>(ProcheZClipping-ClipPlaneEpsilon)) {
            c->ecran[2] = z;
            c->ecran[3] = z = 1.f/z;
            c->ecran[0] = z * x *.5f + .5f;
            c->ecran[1] =  - z * y *.5f + .5f;
            c->ClipInfo = ClipNo;
            if (c->ecran[0] > 1.0f) c->ClipInfo |= ClipDroit;
            else if (c->ecran[0] < 0.0f) c->ClipInfo |= ClipGauche;
            if (c->ecran[1] > 1.0f) c->ClipInfo |= ClipBas;
            else if (c->ecran[1] < 0.0f) c->ClipInfo |= ClipHaut;
        } else {
            c->ClipInfo = ClipProche;
        }
        c->frame_process = frame;
    }
}
//----------------------------------------------------------------------------------------------------------
// calcul les coordonnées 2D de la coordonnée ( mais en valeur unitaire )
bool inline Camera::ProjetUni(Ufloat vdest[4], Ufloat vsrc[3]) {
    Ufloat x,y,z,tmp[3];

    vec3_sub(tmp, vsrc, pos);
    x = vec3_dot(tmp, Final[0]);
    y = vec3_dot(tmp, Final[1]);
    z = vec3_dot(tmp, Final[3]);

    if (z>(ProcheZClipping-ClipPlaneEpsilon)) {
        vdest[2] = z;
        vdest[3] = z = 1.f/z;
        vdest[0] = z * x *.5f + .5f;
        vdest[1] =  - z * y *.5f + .5f;
    } else {
        return FALSE;
    }

    return TRUE;
}
//----------------------------------------------------------------------------------------------------------
void inline Camera::Projet(VertexU3D* v, U32 frame) {
    Projet(v->c, frame);
}
//----------------------------------------------------------------------------------------------------------

#endif
