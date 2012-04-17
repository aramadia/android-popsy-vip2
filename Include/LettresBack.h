/*-----------------------------------------------------------------+
 |                          LettresBack.h                           |
 |                                                                  |
 |   un fake blur sur une jolie image fournit par taggy !!!!!       |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#ifndef _LETTRESBACK_VIP2_H_
#define _LETTRESBACK_VIP2_H_


//////////////////////////////////////////////
// défintion de la class statik
class LettresBack {

//------------------ Datas
private:

    UImage* image1;
    UImage* image2;

    Ufloat alpha;   // imaDest = image1*alpha + image2*(1-alpha)
    Ufloat alphadest;
    Ufloat incalpha;

//------------------ Fonctions
public:

    LettresBack();
    ~LettresBack();

    void Interpo(Ufloat dep, Ufloat ariv, Ufloat time);

    Ufloat GetAlpha()   {
        return alpha;
    }

    void UpdateFRame(Ufloat laptime);
};
//////////////////////////////////////////////


#endif
