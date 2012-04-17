/*-----------------------------------------------------------------+
 |                       Nebuleuse.h                                |
 |                                                                  |
 |  C'est le debut de la demo en fait avec une jolie né...          |
 |( je vous laisse deviner la suite je sais ke vous etes joueurs )  |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#ifndef _NEBULEUSE_VIP2_H_
#define _NEBULEUSE_VIP2_H_


//////////////////////////////////////////////
// definit une chtite classe ki affichera le fondu au debut de la demo
class FonduNebPart {

//------------------ Datas
private:

    UImage* image;
    Ufloat  totaltime;
    Ufloat  timetogo;

//------------------ Fonctions
public:

    FonduNebPart(Ufloat duree);
    ~FonduNebPart();

    bool Affiche(Ufloat step);     // renvoie TRUE tant kil y a qq chose a afficher
};
//////////////////////////////////////////////



//////////////////////////////////////////////
// défintion de la class statik
class NebuleusePart {

//------------------ Datas
private:

    static UScheduler*   Scheduler;

    static MondeU3D*        Monde;
    static Nebuparticule*   Nebuleuse;
    static NebuRayon*       RayonsExp;
    static FonduNebPart*    FirstFondu;
    static NebuTxt*         TexteAdd;

    static UImage*  FonduBlanc;

    static Udouble LapTime;
    static Udouble LastTime;

//------------------ Fonctions
private:

    static void IniSynchros();
    static void SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user);
    static void DoFirstFond(UScheduler* sche, U32 user);
    static void AffNebuleuse(Ufloat step, U32 user);
    static void AffRayons(Ufloat step, U32 user);

public:

    static void Initialise(UScheduler* sche, U32 user);
    static void Release(UScheduler* sche, U32 user);
    static void MainLoop(UScheduler* sche, U32 user);

};
//////////////////////////////////////////////

#endif
