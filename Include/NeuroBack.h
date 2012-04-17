/*---------------------------------------------------------------------+
 |                       NeuroBack.h                                    |
 |                                                                      |
 |          affichage d'un léger fond  pour la scene des neuronnes      |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#ifndef _NEUROBACK_VIP2_H
#define _NEUROBACK_VIP2_H

/////////////////////////////////////////
class NeuroBackground {

//--------------- Datas
private:

    UImage* DestroyBar;
    UImage* CleanTexte;
    UImage* DestroyTexte;
    UImage* Nenfants;

    Ufloat totaltime;

//--------------- Fonctions
public:

    NeuroBackground();
    ~NeuroBackground();

    void Appear(Ufloat time);
    void Disappear(Ufloat time);

    void UpdateFrame(Ufloat laptime);

};
/////////////////////////////////////////


#endif

