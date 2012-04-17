/*-------------------------------------------------------------------------+
 |                          Smile.cpp                                       |
 |                                                                          |
 |   la prés scene final ou une gonzesse en morph nous fait un smiley       |
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

UScheduler* SmilePart::Scheduler= NULL;
MondeU3D*   SmilePart::Monde = NULL;
bool        SmilePart::FirstTime=TRUE;
Udouble     SmilePart::LapTime = 0.0f;
Udouble     SmilePart::LastTime = 0.0f;




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class SmilePart  |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void SmilePart::PreLoad() {
    Monde = new MondeU3D;
    Monde->LoadU3D("Datas/meshes/Smile/morph.u3d");
    Monde->SetAllUVcalc(ObjetU3D::envStage0);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI SmilePart::Initialise(UScheduler* sche, U32 user) {
    Monde->LoadTXM("Datas/meshes/Smile/smile.txm");

    TextureManager::LoadAll();   // load toute les textures

    IniSynchros();

    //----------- Ini le scheduler interne
    Scheduler = new UScheduler;
    Scheduler->AddTask("FinalInitialise", FinalPart::Initialise);     // lance la main loop du final
    FinalPart::SetFlip(FALSE);

    //----------- Ini des flags divers et variés ( poil d'épervier )
    FirstTime = TRUE;

    //----------- lance une autre tache !
    sche->KillActuaTask();   // s'arrete soit meme !
    sche->AddTask("SmileMainLoop", MainLoop);     // lance la main loop
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI SmilePart::Release(UScheduler* sche, U32 user) {
    //------------ Kill le scheduler
    DeleteBlinde(Scheduler);

    //------------ Kill MondeU3D
    DeleteBlinde(Monde);
    //------------ lance les éventuelles taches suivantes
    if (sche) {
        sche->KillActuaTask();
        sche->AddTask("FinalMainLoop", FinalPart::MainLoop);
        FinalPart::SetFlip(TRUE);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

void SmilePart::IniSynchros() {
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI SmilePart::SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user) {
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI SmilePart::MainLoop(UScheduler* sche, U32 user) {
    if (FirstTime) {
        LastTime = ClockU3D::GetTime();
        FirstTime = FALSE;
    }


    //--------------- Calcul le LapTime
    LapTime = ClockU3D::GetTime() - LastTime;
    LastTime = ClockU3D::GetTime();

    ClearOpenGLBuffers();

    Monde->UpdateFrame((float)(FRAMEPERSECOND*LapTime));

    Scheduler->Run();

    FlipOpenGL();

    //------------------- condition de sortie
    if ((ClockU3D::GetTime() - LaunchTime) >= SMILE_TIME_END) {
        sche->KillActuaTask();   // s'arrete soit meme
        sche->AddTask("SmileRelease", Release);     // lance le release
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------


