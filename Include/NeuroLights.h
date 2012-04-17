/*---------------------------------------------------------------------+
 |                       NeuroLights.h                                  |
 |                                                                      |
 |      c'est ici ke l'on vag�rer l'affichage 2D des gnolies lights     |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#ifndef _NEUROLIGHTS_VIP2_H
#define _NEUROLIGHTS_VIP2_H

/////////////////////////////////////////
class NeuroLights {

//--------------- Datas
private:

    MondeU3D* m_monde;   // monde ou agit tout cela
    LightU3D* BigLight;   // la grosse lumi�re

    UImage* Big;   // image de la grosse lumi�re
    UImage* BigRay;   // un rayon de la grosse lumi�re
    UImage* Lens[5];   // les lensflare

    Ufloat Bigxpos;
    Ufloat Bigypos;

    Ufloat BigAlpha;   // alpha da la lumi�re
    Ufloat BigAlphaToGo;   // valeur de destination alpha

    Ufloat BigRayAlpha[8];
    Ufloat BigRayAlphaToGo[8];

//--------------- Fonctions
private:

    void AffBigLight(Ufloat laptime);
    void AffLensFlare(Ufloat laptime);

public:

    NeuroLights(MondeU3D* monde);
    ~NeuroLights();

    UImage* GetBig()    {
        return Big;
    }

    void Affiche(Ufloat laptime);
};
/////////////////////////////////////////

#endif
