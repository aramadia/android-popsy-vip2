/*-----------------------------------------------------------------+
 |                          GhostVPort.h                            |
 |                                                                  |
 |   un truc ki va gérer l'affichage de gnolie viewport de camera   |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#ifndef _GHOSTVPORT_VIP2_H_
#define _GHOSTVPORT_VIP2_H_



//////////////////////////////////////////////
// un simple carré 2D ( généré avec des lignes )
class GhostCarre {

//------------------ Datas
private:

    Ufloat angle;
    Ufloat Destangle;
    Ufloat incangle;

    Ufloat sizex;
    Ufloat DestSizex;
    Ufloat incsizex;

    Ufloat sizey;
    Ufloat DestSizey;
    Ufloat incsizey;

    Ufloat  alpha;
    Ufloat  DestAlpha;
    Ufloat  incalpha;
    bool    InterpoAlpha;

    Ufloat posx,posy;


//------------------ Fonctions
private:

    void IncVal(Ufloat& val, Ufloat inc, Ufloat laptime, Ufloat destmax);

public:

    GhostCarre();

    void SetPos(Ufloat x, Ufloat y)    {
        posx=x;
        posy=y;
    }
    void Set(Ufloat sizex, Ufloat destsize, Ufloat sizey, Ufloat destsizey,
             Ufloat angle, Ufloat destangle, Ufloat time, bool invalpha=FALSE);

    void Goto(Ufloat destsizex, Ufloat destsizey, Ufloat destsizeangle, Ufloat destalpha, Ufloat time);

    bool IsInterpo()    {
        return InterpoAlpha;
    }

    void Affiche(Ufloat laptime);     // pipeline doit etre ini pour les lignes
};
//////////////////////////////////////////////


//////////////////////////////////////////////
// un ensemble de carré
class GhostOneVPort {

//------------------ Datas
private:

    GhostCarre* carres;

    MondeU3D*   Monde;
    Camera*     Cam;

    UImage* blanc;

    bool    Appear; // est-ce ke l'on est dans le mode d'apparition ?
    Ufloat  Tappear;
    Ufloat  Treste;

    bool    Disappear;
    Ufloat  TDisappear;

public:

    GhostOneVPort* suiv;

//------------------ Fonctions
private:

    void DisaCarres(Ufloat time);

public:

    GhostOneVPort(MondeU3D* monde, Camera* cam, Ufloat tappear, Ufloat timeReste, Ufloat timeDisa);
    ~GhostOneVPort();

    bool IsFinish() {
        if (TDisappear<=0.0f) return TRUE;
        else return FALSE;
    }

    void Affiche(Ufloat laptime);
};
//////////////////////////////////////////////



//////////////////////////////////////////////
// n'est en fait kune liste de GhostOneVPort
class GhostVPort {

//------------------ Datas
private:

    GhostOneVPort* allport;

//------------------ Fonctions
public:

    GhostVPort();
    ~GhostVPort();

    void Add(MondeU3D* monde, Camera* cam, Ufloat tappear, Ufloat timeReste, Ufloat timeDisa);

    void Affiche(Ufloat laptime);
};
//////////////////////////////////////////////

#endif

