/*-------------------------------------------------------------------------+
 |                          Multi.cpp                                       |
 |                                                                          |
 |   ca va permettre de sceduler plein de petites scenette en fait ki       |
 |                  juste avant le final de la demo                         |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +-----------------------+
//                                  |   Variables Locales   |
//                                  +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler* MultiPart::Scheduler = NULL;
bool        MultiPart::ColonneScene = FALSE;
bool        MultiPart::FirstTime = TRUE;
Udouble     MultiPart::LapTime = 0.0f;
Udouble     MultiPart::LastTime = 0.0f;




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class MultiPart  |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void MultiPart::PreLoad() {
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI MultiPart::Initialise(UScheduler* sche, U32 user) {

    IniSynchros();

    //----------- Ini le scheduler interne
    Scheduler = new UScheduler;

    LettresPart::Initialise(Scheduler, 0);
    EtoilePart::Initialise(Scheduler, 0);
    ColonnePart::Initialise(Scheduler, 0);

    ColonneScene = TRUE;

    //----------- Ini des flags divers et variés ( poil d'épervier )
    FirstTime = TRUE;

    //----------- lance une autre tache !
    sche->KillActuaTask();   // s'arrete soit meme !
    sche->AddTask("MultiMainLoop", MainLoop);     // lance la main loop
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI MultiPart::Release(UScheduler* sche, U32 user) {

    //------------ Kill le scheduler
    DeleteBlinde(Scheduler);

    LettresPart::Release(NULL, 0);
    EtoilePart::Release(NULL, 0);
    ColonnePart::Release(NULL, 0);

    //------------ lance les éventuelles taches suivantes
    if (sche) {
        sche->KillActuaTask();
        sche->AddTask("SmileInitialise", SmilePart::Initialise);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

void MultiPart::IniSynchros() {
    SynchroPhase* phase;

    phase = AllSynchro->AddPhase("MultiHC", MULTI_TIME_BEGIN, MULTI_TIME_END);

    //----------------- les changement de lettres et de scenes
    phase->AddTick(0, 1.472f,  SynchrosMess);
    phase->AddTick(0, 3.116f,  SynchrosMess);
    phase->AddTick(0, 4.832f,  SynchrosMess);
    phase->AddTick(0, 6.784f,  SynchrosMess);
    phase->AddTick(0, 8.208f,  SynchrosMess);
    phase->AddTick(0, 9.936f,  SynchrosMess);
    phase->AddTick(0, 13.488f,  SynchrosMess);
    phase->AddTick(0, 15.024f,  SynchrosMess);
    phase->AddTick(0, 16.672f,  SynchrosMess);
    phase->AddTick(0, 18.400f,  SynchrosMess);
    phase->AddTick(0, 20.320f,  SynchrosMess);
    phase->AddTick(0, 21.776f,  SynchrosMess);
    phase->AddTick(1, 23.456f,  SynchrosMess);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI MultiPart::SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user) {
    switch (syncNum) {
        //----------------- balance les scene des lettres
    case 0:
        LettresPart::NextLettre();
        Scheduler->ReScheduleTask("LettresMainLoop");
        Scheduler->SuspendTask("EtoileMainLoop");
        Scheduler->SuspendTask("ColonneMainLoop");
        break;

    case 1:
        LettresPart::GoFScene();
        Scheduler->ReScheduleTask("LettresMainLoop");
        Scheduler->SuspendTask("EtoileMainLoop");
        Scheduler->SuspendTask("ColonneMainLoop");
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

// signale ka la lettre en cour a finit d'etre affichée
void MultiPart::LettreFinish() {
    Scheduler->SuspendTask("LettresMainLoop");

    if (ColonneScene) {
        Scheduler->ReScheduleTask("ColonneMainLoop");
        ColonnePart::NextCam();
    } else {
        Scheduler->ReScheduleTask("EtoileMainLoop");
        EtoilePart::NextCamera();
    }

    ColonneScene = !ColonneScene;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI MultiPart::MainLoop(UScheduler* sche, U32 user) {
    if (FirstTime) {
        LastTime = ClockU3D::GetTime();
        FirstTime = FALSE;
        Scheduler->SuspendTask("EtoileMainLoop");
        Scheduler->SuspendTask("ColonneMainLoop");
    }

    //--------------- Calcul le LapTime
    LapTime = ClockU3D::GetTime() - LastTime;
    LastTime = ClockU3D::GetTime();

    Scheduler->Run();

    //------------------- condition de sortie
    if ((ClockU3D::GetTime() - LaunchTime) >= MULTI_TIME_END) {
        sche->KillActuaTask();   // s'arrete soit meme
        sche->AddTask("MultiRelease", Release);     // lance le release
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------
