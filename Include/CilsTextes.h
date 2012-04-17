/*-------------------------------------------------------------------------+
 |                              CilsTextes.h                                |
 |                                                                          |
 |  Affichage d'une grosse quantité de d'enormes textes a l'ecran poil de   |
 |                                  vent                                    |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _CILSTEXTES_VIP2_H
#define _CILSTEXTES_VIP2_H

/////////////////////////////////////////
class CilsTextes {

//--------------- Datas
private:

    UImage** Salvation;
    UImage** Temptation;
    UImage** Disinfo;
    UImage** Testi;
    UImage** Freedom;

    UImage* dates;

//--------------- fonctions
private:

    void GereXImage(UImage* ima, Ufloat laptime, bool right);
    void GereYImage(UImage* ima, Ufloat laptime, bool haut);

public:

    CilsTextes();
    ~CilsTextes();

    void Disappear(Ufloat time);

    void UpdateFrame(Ufloat laptime);
};
/////////////////////////////////////////

#endif

