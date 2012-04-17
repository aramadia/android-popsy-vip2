/*---------------------------------------------------------------------+
 |                          PlansCache.h                                |
 |                                                                      |
 |          une jolie partie cache ki regorge de fiéfés faciesses       |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#ifndef _PLANSCACHE_VIP2_H
#define _PLANSCACHE_VIP2_H

/////////////////////////////////////////
class PlansCache {

//--------------- Datas
private:

    UImage* Tuo;
    bool BTuo;
    UImage* U2;
    bool BU2;
    UImage* Ghell;
    bool BGhell;
    UImage* Madman;
    bool BMadman;
    UImage* Taggy;
    bool BTaggy;
    UImage* FFactory;
    bool BFFactory;

//--------------- Fonctions
public:

    PlansCache(Ufloat time);
    ~PlansCache();


    void OnAppearTuo(Ufloat time);
    void OnDisappearTuo(Ufloat time);

    void OnAppearU2(Ufloat time);
    void OnDisappearU2(Ufloat time);

    void OnAppearMadman(Ufloat time);
    void OnDisappearMadman(Ufloat time);

    void OnAppearGhell(Ufloat time);
    void OnDisappearGhell(Ufloat time);

    void OnAppearTaggy(Ufloat time);
    void OnDisappearTaggy(Ufloat time);

    void OnAppearFFactory(Ufloat time);
    void OnDisappearFFactory(Ufloat time);

    void UpdateFrame(Ufloat laptime);
};
/////////////////////////////////////////


#endif

