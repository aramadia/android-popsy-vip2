/*-------------------------------------------------------------------------+
 |                          Multi.h                                         |
 |                                                                          |
 |   ca va permettre de sceduler plein de petites scenette en fait ki       |
 |                  juste avant le final de la demo                         |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _MULTI_VIP2_H
#define _MULTI_VIP2_H

/////////////////////////////////////////
class MultiPart {

//--------------- Datas
private:

    static UScheduler*  Scheduler;

    static bool ColonneScene;   // est-ce ke c'est le tour de la scene de la colonne ?

    static bool     FirstTime;
    static Udouble  LapTime;
    static Udouble  LastTime;

//--------------- Fonctions
private:

    static void         IniSynchros();
    static void WINAPI  SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user);

public:

    static void         PreLoad();
    static void WINAPI  Initialise(UScheduler* sche, U32 user);
    static void WINAPI  Release(UScheduler* sche, U32 user);
    static void WINAPI  MainLoop(UScheduler* sche, U32 user);
    static void         LettreFinish();   // signale ka la lettre en cour a finit d'etre affichée

};
/////////////////////////////////////////

#endif
