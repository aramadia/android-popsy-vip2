/*-------------------------------------------------------------------------+
 |                              Colonne.h                                   |
 |                                                                          |
 |  une des piece maitresse de la demo ou l'on voit une giga colonne        |
 |                          organik ki va bien !                            |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _COLONNE_VIP2_H
#define _COLONNE_VIP2_H

/////////////////////////////////////////
class ColonnePart {

//--------------- Datas
private:

    static UScheduler*  Scheduler;

    static MondeU3D*        Monde;
    static ColonneAnim*     BAnim;

    static UImage* Fondu;

    static U32  CamPos;

    static bool    FirstTime;
    static Udouble LapTime;
    static Udouble LastTime;

//--------------- Fonctions
private:

    static void         IniSynchros();
    static void WINAPI  SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user);

public:

    static void         PreLoad();
    static void WINAPI  Initialise(UScheduler* sche, U32 user);
    static void WINAPI  Release(UScheduler* sche, U32 user);
    static void WINAPI  MainLoop(UScheduler* sche, U32 user);

    static void NextCam();

};
/////////////////////////////////////////

#endif
