/*-------------------------------------------------------------------------+
 |                          EtoileScale.h                                   |
 |                                                                          |
 |  affiche l'objet ki se deforme en Scale Additif pour faire une sorte     |
 |                          de vibration 3d                                 |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _ETOILESCALE_VIP2_H
#define _ETOILESCALE_VIP2_H

/////////////////////////////////////////
class EtoileScale {

//--------------- Datas
private:

    MondeU3D* Monde;
    ObjetU3D* obj;

    Ufloat scale;
    Ufloat scale1;

//--------------- Fonctions
public:

    EtoileScale(MondeU3D* monde);

    void UpdateFrame(Ufloat laptime);
};
/////////////////////////////////////////

#endif
