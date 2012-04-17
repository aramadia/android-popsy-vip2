/*-------------------------------------------------------------------------+
 |                          Smile.h                                         |
 |                                                                          |
 |   la prés scene final ou une gonzesse en morph nous fait un smiley       |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _SMILE_VIP2_H
#define _SMILE_VIP2_H

/////////////////////////////////////////
class SmilePart {

//--------------- Datas
private:

    static UScheduler*  Scheduler;

    static MondeU3D* Monde;

    static bool     FirstTime;
    static Udouble  LapTime;
    static Udouble  LastTime;

//--------------- Datas
private:

    static void         IniSynchros();
    static void WINAPI  SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user);

public:

    static void         PreLoad();
    static void WINAPI  Initialise(UScheduler* sche, U32 user);
    static void WINAPI  Release(UScheduler* sche, U32 user);
    static void WINAPI  MainLoop(UScheduler* sche, U32 user);

};
/////////////////////////////////////////


#endif
