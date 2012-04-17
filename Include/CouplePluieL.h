/*-----------------------------------------------------------------+
 |                      CouplePluieL.h                              |
 |                                                                  |
 |  la classe ki affiche de la pluie en ligne sur la scene en avant |
 |                      et en arrière plan.                         |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#ifndef _COUPLEPLUIEL_VIP2_H_
#define _COUPLEPLUIEL_VIP2_H_


//////////////////////////////////////////////
class GoutteL {

//------------------ Datas
private:

    Ufloat dir[3];
    Ufloat pos[3];
    Ufloat size;
    Ufloat vitesse;
    Ufloat alpha;

//------------------ Fonctions
public:

    GoutteL();

    void Genere(Ufloat dir[3]);

    void Move(Ufloat step) {
        Ufloat tmp[3];
        vec3_mul(tmp, dir, vitesse*step);
        vec3_add(pos,tmp,pos);
    }

    bool IsFinish()                 {
        if (alpha<=0.0f) return TRUE;
        else return FALSE;
    }
    void GetPos1(Ufloat dest[3])    {
        vec3_eg(dest, pos);
    }
    void GetPos2(Ufloat dest[3])    {
        vec3_mul(dest, dir, -size);
        vec3_add(dest,dest,pos);
    }

    Ufloat  GetAlpha()  {
        return alpha;
    }
};
//////////////////////////////////////////////



//////////////////////////////////////////////
// une classe ki géré une particul de vent
class VentParticul {

//------------------ Datas
private:

    Ufloat dir[3];
    Ufloat pos[3];
    Ufloat size;
    Ufloat vitesse;
    Ufloat alpha;

//------------------ Fonctions
public:

    VentParticul();

    void Genere(Ufloat dir[3]);

    void Move(Ufloat step) {
        Ufloat tmp[3];
        vec3_mul(tmp, dir, vitesse*step);
        vec3_add(pos,tmp,pos);
    }

    void    GetPos(Ufloat dest[3])  {
        vec3_eg(dest, pos);
    }
    Ufloat  GetSize()               {
        return size;
    }
    Ufloat  GetAlpha()              {
        return alpha;
    }
};
//////////////////////////////////////////////



//////////////////////////////////////////////
class PluieL {

//------------------ Datas
private:

    TextureU3D*  partTex;   // image de la particule de base
    MaterialU3D* partMat;   // material de la particule de base
    BitmapU3D*   partBmp;   // bitmpa de la particule

    GoutteL* Gouttes;

    VentParticul* VentDerriere;   // vent en avant plan
    VentParticul* VentDevant;   // vent en arriere plan

    Ufloat dirGeneral[3];

//------------------ Fonctions
private:

    void    AffOneParti(Ufloat pos[3], Ufloat Size, Ufloat alpha);
    Ufloat  CalcAlpha(Ufloat y);
    void    AffVent(VentParticul* tmpv, Ufloat laptime, U32 nbPart);

public:

    PluieL();
    ~PluieL();

    void UpdateFrameBackGround(Ufloat laptime);
    void UpdateFrameForeGround(Ufloat laptime);
};
//////////////////////////////////////////////

#endif
