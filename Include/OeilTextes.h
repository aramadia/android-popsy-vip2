/*-------------------------------------------------------------------------+
 |                              OeilTextes.h                                |
 |                                                                          |
 |  affichage de wiiiz textes par dessus la retine de l'oeil voiliiiii      |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _OEILTEXTES_VIP2_H
#define _OEILTEXTES_VIP2_H


/////////////////////////////////////////
class OeilTextes {

//-------------------- Datas
private:

    UImageBlur* Revive;
    bool disaRevive;
    UImageBlur* Vip;
    bool disaVip;

    UImageBlur* Vertical[8];   // petite image Verticales
    U32         Vpos;   // prochaine image horizontale
    UImageBlur* Horizontal[8];   // petites image horizontales
    U32         Hpos;   // prochaine image horizontale

    UImage* TexteArabe;   // le texte fournis par madaman ke je sais meme pas se ki est ecrit na !

    UImage* Rond[4];

//-------------------- Fonctions
private:

    void AppearHori(UImageBlur* ima, Ufloat ypos);
    void DisAppearHori(UImageBlur* ima);
    void AppearRond(Ufloat ypos);
    void AppearVerti(UImageBlur* ima, Ufloat xpos);
    void DisAppearVerti(UImageBlur* ima);

public:

    OeilTextes();
    ~OeilTextes();

    void OnAppear1Revive(Ufloat time);
    void OnAppear2Revive(Ufloat time);
    void OnAppear3Revive(Ufloat time);
    void OnDisAppearRevive(Ufloat time);

    void OnAppear1Vip(Ufloat time);
    void OnAppear2Vip(Ufloat time);
    void OnAppear3Vip(Ufloat time);
    void OnAppear4Vip(Ufloat time);
    void OnDisAppearVip(Ufloat time);

    void OnAppearHori();
    void OnAppearVerti();

    void OnAttenuateTexte(Ufloat time);

    void UpdateFrame(Ufloat laptime);
};
/////////////////////////////////////////

#endif

