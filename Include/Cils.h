/*-------------------------------------------------------------------------+
 |                              Cils.h                                      |
 |                                                                          |
 |  Encre une partie deja vue dans d'autre demos, mais j'adoooore alors     |
 |          bon je pouvais pas m'empécher de la foutre                      |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _CILS_VIP2_H
#define _CILS_VIP2_H

/////////////////////////////////////////
class CilsPart {

//--------------- Datas
private:

    static UScheduler*  Scheduler;

    static MondeU3D*        Monde;
    static CilsSpheres*     Cils;

    static CilsBack*    Back;
    static CilsCarotus* Carotus;
    static CilsTextes*  Textes;
    static UImage*      Fondu;

    static bool    FirstTime;
    static Udouble LapTime;
    static Udouble LastTime;

    static bool FlipScreen;

//--------------- Fonctions
private:

    static void         IniSynchros();
    static void WINAPI  SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user);
    static void WINAPI  AffCils(Ufloat step, U32 user);

public:

    static MondeU3D*    GetCilsMonde()  {
        return Monde;
    }
    static CilsSpheres* GetCilsSphere() {
        return Cils;
    }

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

