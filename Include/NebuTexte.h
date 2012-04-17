/*-----------------------------------------------------+
 |                     NebuTexte.h                      |
 |                                                      |
 |  Affichage des giga paroles du chanteur de la chanson|
 |                                                      |
 |  U2^PoPsy TeAm 2000                              |
 +-----------------------------------------------------*/

#ifndef _NEBUTEXTE_VIP2_H
#define _NEBUTEXTE_VIP2_H

/////////////////////////////////////////
class NebuTxt {

//--------------- Datas
private:

    TextureU3D* texture1;
    TextureU3D* texture2;
    TextureU3D* texture3;

    UImage* Forsaken1;
    UImage* Forsaken2;
    UImage* own;
    UImage* destiny;
    UImage* remove;
    UImage* in;
    UImage* mind;
    UImage* eyes;
    UImage* see;
    UImage* belief;
    UImage* skin;
    UImage* your;
    UImage* to;
    UImage* I;
    UImage* by1;
    UImage* by2;
    UImage* my1;
    UImage* my2;
    UImage* must;


//-------------- Fonctions
private:

    void Disappear(UImage* ima, Ufloat time);

public:

    NebuTxt();
    ~NebuTxt();

    void OnForsaken1();
    void OnBy1();
    void OnDestiny();
    void OnForsaken2();
    void OnBy2();
    void OnMy1();
    void OnOwn();
    void OnMind();
    void OnI();
    void OnMust();
    void OnRemove();
    void OnMy2();
    void OnSkin();
    void OnTo();
    void OnSee();
    void OnBelief();
    void OnIn();
    void OnYour();
    void OnEyes();

    void Affiche(Ufloat laptime);
};
/////////////////////////////////////////

#endif

