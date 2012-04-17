/*-----------------------------------------------------------------+
 |                          Ghost2D.h                               |
 |                                                                  |
 |  pour gérer l'affichage de tout se ki est 2D sur cette scene     |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#ifndef _GHOST2D_VIP2_H_
#define _GHOST2D_VIP2_H_



//////////////////////////////////////////////
// la class ki gere tout ca
class Ghost2D {

//------------------ Datas
private:

    UImage* Rond[3];
    UImage* BarreV[4];
    UImage* BarreH[3];

    bool    disamode;
    Ufloat  time;

//------------------ Fonctions
private:

    void DisappearAll();

public:

    Ghost2D();
    ~Ghost2D();

    void Go();

    void UpdateFrame(Ufloat laptime);

};
//////////////////////////////////////////////

#endif
