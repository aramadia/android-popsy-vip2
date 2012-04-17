/*---------------------------------------------------------------------+
 |                          Plans.h                                     |
 |                                                                      |
 |  une partie un peu High tech assez deja vu, mais je voulais TROP en  |
 |          faire une paskeu j'aime TROP ca hahahaahahah                |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#ifndef _PLANS_VIP2_H
#define _PLANS_VIP2_H


/////////////////////////////////////////
class PlansPart {

//--------------- Datas
private:

    static UScheduler*  Scheduler;

    static MondeU3D*        Monde;
    static PlansAnim*       PAnim;
    static PlansTransp*     PTransp;

    static MondeU3D*        MondeClock;
    static PlansClockAnim*  ClockAnim;

    static PlansTextes* AllTextes;
    static PlansCache*  PartCache;

    static UImage* LigneHaut;
    static UImage* LigneBas;
    static UImage* LignePetite;

    static Ufloat GAlpha;   // alpha globale de la scene
    static Ufloat IncGAlpha;

    static bool FlipScreen;   // est-ce ke l'on fait le flip et le clear ?

    static bool HidenPart;   // est-ce ke l'on va avoir droit a la partie cachee ?

    static bool    FirstTime;
    static Udouble LapTime;
    static Udouble LastTime;

//--------------- Fonctions
private:

    static void         IniSynchros();
    static void SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user);
    static void         GereGAlpha(Ufloat laptime);

public:

    static void SetFlip(bool b)    {
        FlipScreen=b;
    }

    static void ActiveHidenPart() {
        HidenPart = TRUE;
    }

    static void         PreLoad();   // load des trucs avant la commencement de la séquence
    static void Initialise(UScheduler* sche, U32 user);
    static void Release(UScheduler* sche, U32 user);
    static void MainLoop(UScheduler* sche, U32 user);

};
/////////////////////////////////////////


#endif

