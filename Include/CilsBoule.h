/*-------------------------------------------------------------------------+
 |                              CilsBoule.h                                 |
 |                                                                          |
 |  une simple sphere remplit de gnolie peits cils tout velus et gigotants  |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _CILSBOULE_VIP2_H
#define _CILSBOULE_VIP2_H

#define CILS_NBCTRLPTS  10

/////////////////////////////////////////
// un pitit poil en fait
struct CilsPoil {

    Ufloat          origine[CILS_NBCTRLPTS][3];
    BSplinaeAff*    poil;
    Ufloat          partiPos;

    CilsPoil()  {
        poil=new BSplinaeAff(CILS_NBCTRLPTS,16);
    }
    ~CilsPoil() {
        delete poil;
    }
};
/////////////////////////////////////////



/////////////////////////////////////////
// 1 seule boule
class CilsBoule {

//--------------- Datas
private:

    U32         NbPoils;
    CilsPoil*   Poils;
    CilsPoil*   NextPoil;

    Mat3x4 Mats[CILS_NBCTRLPTS];   // matrice de transfos de chak niveau de controls pts

    CilsBoule* fils;

public:

    CilsBoule*  suiv;

//--------------- Fonctions
public:


    CilsBoule(Ufloat xp, Ufloat yp, Ufloat zp, U32 nbpoils, Ufloat dmin, Ufloat delta, U32 enfant);
    ~CilsBoule();

    Ufloat  GetNbPoils()                                                        {
        return NbPoils;
    }
    void    ResetPoilsParcours()                                                {
        NextPoil=Poils;
    }
    void    GetPoilsPartiPos(Ufloat& x, Ufloat& y, Ufloat& z, Ufloat& alpha);

    void Affiche(Mat3x4& mat, Ufloat time, Ufloat laptime, Ufloat alpha);

};
/////////////////////////////////////////




/////////////////////////////////////////
// toute les spheres !
class CilsSpheres {

//--------------- Datas
private:

    TextureU3D*  Tex;   // texture de la particule
    MaterialU3D* Mat;   // material de la particule de base
    BitmapU3D*   Bmp;   // bitmap utilisée de partout

    CilsBoule* AllBoule;

    bool    interpoAlpha;
    Ufloat  alpha;   // valeur alpha de l'image
    Ufloat  destalpha;   // valeur de destination alpha
    Ufloat  incalpha;   // incrément de l'alpha

    bool enable;

//--------------- Fonctions
private:

    void AffParti(Ufloat x, Ufloat y, Ufloat z, Ufloat zoom);

public:

    CilsSpheres();
    ~CilsSpheres();

    void SetAlpha(Ufloat a)                                    {
        alpha=a;
    }
    void InterpoAlpha(Ufloat debut, Ufloat fin, Ufloat time);

    void SetEnable(bool b) {
        enable=b;
    }

    void Affiche(Ufloat time, Ufloat laptime);

};
/////////////////////////////////////////

#endif
