/*-----------------------------------------------------------------+
 |                          GhostEtincelle.h                        |
 |                                                                  |
 |  en plus des eclaires, je rajoute des etincelles aux points      |
 |      d'impacts pour donner encore plus de vie a l'ensemble       |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#ifndef _GHOSTETINCELLE_VIP2_H_
#define _GHOSTETINCELLE_VIP2_H_



//////////////////////////////////////////////
// une etincelle
class GhostEtincelle {

//------------------ Datas
private:

    Ufloat Vitesse[3];
    Ufloat LastPos[3];
    Ufloat Pos[3];

    Ufloat life;
    Ufloat Tlife;

    bool gereCollision;   // ne gere les collision k1 frame sur 2 héhé

public:

    GhostEtincelle* suiv;

//------------------ Fonctions
public:

    GhostEtincelle();

    void Ini(Ufloat pos[3], Ufloat vitesse[3], Ufloat tlife);
    void CalcPos(Ufloat laptime, MondeU3D* monde);   // calcul position en gerant les collisions si besoin

    bool    IsFinish()  {
        if (life<=0.0f) return TRUE;
        else return FALSE;
    }
    Ufloat  GetAlpha()  {
        return life/Tlife;
    }
    Ufloat* GetPos()    {
        return Pos;
    }
    Ufloat* GetLastPos() {
        return LastPos;
    }

};
//////////////////////////////////////////////



//////////////////////////////////////////////
// une etincelle
class GhostAllEtincelle {

//------------------ Datas
private:

    TextureU3D*  partTex;   // image de la particule de base
    MaterialU3D* partMat;   // material de la particule de base
    BitmapU3D*   partBmp;   // bitmpa de la particule

    GhostEtincelle* AllActive;
    GhostEtincelle* AllInActive;


//------------------ Fonctions
private:

    void AffOneParti(Ufloat pos[3], Ufloat alpha);

public:

    GhostAllEtincelle(U32 nbmax);
    ~GhostAllEtincelle();

    void GenereEtincelle(Ufloat pos[3], Ufloat dir[3], U32 nb);
    void UpdateFrame(Ufloat laptime, MondeU3D* monde);
    void Affiche();

};
//////////////////////////////////////////////


#endif
