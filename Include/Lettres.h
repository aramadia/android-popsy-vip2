/*-----------------------------------------------------------------+
 |                          Lettres.h                               |
 |                                                                  |
 |   va afficher PoPsY TeAm VIP 2 en plusieirus fois avec pleins    |
 |      d'effets distroy a souhaits ( genre pub Fila )              |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#ifndef _LETTRES_VIP2_H_
#define _LETTRES_VIP2_H_


#define NB_LETTRES_LPART    13

//////////////////////////////////////////////
// défintion de la class statik
class LettresPart {

//------------------ Datas
private:

    static UScheduler*   Scheduler;

    static MondeU3D*    Lettres[NB_LETTRES_LPART];
    static U32          ActuaLettre;

    static MondeU3D*    FScene;
    static bool         ActuaFScene;   // on est sur la scene de fin
    static Ufloat       DureeRestante;   // duree restante avant le fondu au noir

    static UImage* Fondu;
    static UImage* Url;

    static LettresBack* Back;
    static BPeloche*    Bruit;
    static XGrezille*   grezille;
    static TextureU3D*  TexGeneral;   // texture pour toutes les lettres

    static Ufloat DureeScene;   // duree d'une pitite scene

    static bool    FirstTime;
    static Udouble LapTime;
    static Udouble LastTime;

//------------------ Fonctions
private:

    static void         IniSynchros();
    static void WINAPI  SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user);
    static void         AffBlur(MondeU3D* monde, Ufloat intens);

public:

    static void         PreLoad();
    static void WINAPI  Initialise(UScheduler* sche, U32 user);
    static void WINAPI  Release(UScheduler* sche, U32 user);
    static void WINAPI  MainLoop(UScheduler* sche, U32 user);

    static void NextLettre();
    static void GoFScene();
};
//////////////////////////////////////////////

#endif
