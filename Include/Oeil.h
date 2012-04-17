/*---------------------------------------------------------------------+
 |                          Oeil.h                                      |
 |                                                                      |
 |  Une sacrée partie ou l'on est en giga gros plan sur un oeil humain  |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#ifndef _OEIL_VIP2_H
#define _OEIL_VIP2_H


/////////////////////////////////////////
class OeilPart {

//--------------- Datas
private:

    static UScheduler*  Scheduler;

    static MondeU3D*        Monde;

    static OeilLigne*   Lignes;
    static OeilDefor*   Deformation;
    static OeilTextes*  Textes;

    static UImage*  Flash;
    static UImage*  Fondu;

    static bool affCilAdd;
    static U32  IncDef;

    static bool FlipScreen;

    static bool    FirstTime;
    static Udouble LapTime;
    static Udouble LastTime;

//--------------- Fonctions
private:

    static void         IniSynchros();
    static void WINAPI  SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user);
    static void         AffCilsAdd();

public:

    static void SetFlip(bool b)    {
        FlipScreen=b;
    }

    static void         PreLoad();
    static void WINAPI  Initialise(UScheduler* sche, U32 user);
    static void WINAPI  Release(UScheduler* sche, U32 user);
    static void WINAPI  MainLoop(UScheduler* sche, U32 user);

};
/////////////////////////////////////////

#endif
