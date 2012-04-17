/*-------------------------------------------------------------+
 |                       Synchronisater.h                       |
 |                                                              |
 |  C'est lui ki va s'occuper de g�rer les callback de synchros |
 |                                                              |
 |  U2^PoPsy TeAm 2000                                      |
 +-------------------------------------------------------------*/

#ifndef _SYNCHRONISATER_H_
#define _SYNCHRONISATER_H_


class SynchroTick;



//////////////////////////////////////////
// definition d'un synchro callback

typedef void (* PROCSYNCCALLBACK)(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user);

class SynchroCallback {

//-------------- type
public:

    enum {
        onecall,    // appel 1 seule fois la fonction
        multicall   // appel la fonctions kan temps >= temps de la synchro
    };


//-------------- Datas
private:

    PROCSYNCCALLBACK    Procs;
    U32                 Type;
    bool                Call;   // est mis a TRUE kan callback au moins appel� 1 fois

public:

    SynchroCallback* suiv;


//-------------- Fonctions
public:

    SynchroCallback(PROCSYNCCALLBACK procs, U32 type=onecall);

    void Run(SynchroTick* sync, Ufloat rtime, U32 syncNum);
};
//////////////////////////////////////////






//////////////////////////////////////////
// d�finition d'une synchro dans le temps
class SynchroTick {

//---------------- Datas
private:

    SynchroCallback* AllCallback;

public:

    U32     Id; // un numero identificateur
    Ufloat  Tpos;   // une position dans le temps

    SynchroTick* suiv;

//---------------- Fonctions
public:

    SynchroTick(U32 number, Ufloat pos);
    ~SynchroTick();

    void AddCalBack(PROCSYNCCALLBACK procs, U32 type=SynchroCallback::onecall);

    void Run(Ufloat time);

};
//////////////////////////////////////////





//////////////////////////////////////////
// d�finition d'une phase de synchro
class SynchroPhase {

//----------------- Datas
private:

    CString nom;

    Ufloat  TStart; // temps ou la phase d�mare
    Ufloat  TEnd;   // temps ou la phase s'arrete

    SynchroTick* AllTick;   // tout les temps de synchros

public:

    SynchroPhase*   suiv;

//----------------- Fonctions
public:

    SynchroPhase(LPCSTR nom, Ufloat start, Ufloat end);
    ~SynchroPhase();

    Ufloat GetStartTime()   {
        return TStart;
    }
    Ufloat GetEndTime()     {
        return TEnd;
    }

    SynchroTick* AddTick(U32 number, Ufloat tpos, PROCSYNCCALLBACK procs, U32 type=SynchroCallback::onecall);

    void Run(Ufloat time);
};
//////////////////////////////////////////




//////////////////////////////////////////
// d�fintion du synchronisater
class USynchronisater {

//----------------- Datas
private:

    SynchroPhase*   AllPhase;

//----------------- Fonctions
public:

    USynchronisater();
    ~USynchronisater();

    SynchroPhase*   AddPhase(LPCSTR nom, Ufloat start, Ufloat end);

    void Run();

};
//////////////////////////////////////////

#endif
