/*-----------------------------------------------------------------+
 |                          Ghost.cpp                               |
 |                                                                  |
 |   C'est scene s'appelle ghost paskeu tout simplement on s'est    |
 | inspiré de de la gonzesse de ghost in the shell remplit de cable|
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/


#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +-----------------------+
//                                  |   Variables Locales   |
//                                  +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*     GhostPart::Scheduler = NULL;
MondeU3D*       GhostPart::Monde = NULL;
GhostEclaire*   GhostPart::Eclairs = NULL;
GhostVPort*     GhostPart::Viewsp = NULL;
Ghost2D*        GhostPart::Effet2D = NULL;
bool            GhostPart::FirstTime = TRUE;
Udouble         GhostPart::LapTime = 0.0f;
Udouble         GhostPart::LastTime = 0.0f;
bool            GhostPart::FlipScreen = TRUE;



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class GhostPart  |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void GhostPart::PreLoad() {
    Monde = new MondeU3D;
    Monde->LoadU3D("Datas/meshes/Ghost/Gis13.u3d");
    Monde->ConstructAllObjStaticLighting();
    Monde->SetActualCamera(Monde->GetCamera("Camera01"));
    Monde->GetStates().SortPolyTr = FALSE;

    //---------- construi arbre de collision sauf pour la gonzesse et la flotte
    for (Monde->IniObjIterator(); Monde->GetObjFromIterator(); Monde->IncObjIterator()) {
        if ((strcmp(Monde->GetObjFromIterator()->GetNom(), "corps")==0)) {
            Monde->GetObjFromIterator()->ConstructWireDatas();
        } else if (!(strcmp(Monde->GetObjFromIterator()->GetNom(), "eau")==0)) {
            Monde->GetObjFromIterator()->ConstructColTree();
        }
    }

    Monde->LoadTXM("Datas/meshes/Ghost/gis02.txm");

}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI GhostPart::Initialise(UScheduler* sche, U32 user) {
    Monde->AddAFDCallback("Eclairs", AffEclairs);

    Eclairs = new GhostEclaire(Monde);

    Viewsp = new GhostVPort;

    Effet2D = new Ghost2D;

    TextureManager::LoadAll();   // load toute les textures


    IniSynchros();

    //----------- Ini le scheduler interne
    Scheduler = new UScheduler;

    //----------- Ini des flags divers et variés ( poil d'épervier )
    FirstTime = TRUE;

    //----------- lance la mainloop
    sche->KillActuaTask();   // s'arrete soit meme !
    sche->AddTask("GhostMainLoop", MainLoop);     // lance la main loop
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI GhostPart::Release(UScheduler* sche, U32 user) {
    //------------ Kill le scheduler
    DeleteBlinde(Scheduler);

    //------------ Kill les effets 2d
    DeleteBlinde(Viewsp);
    DeleteBlinde(Effet2D);

    //------------ Kill MondeU3D
    DeleteBlinde(Monde);
    DeleteBlinde(Eclairs);

    //------------ lance les éventuelles taches suivantes
    if (sche) {
        sche->KillActuaTask();
        sche->AddTask("CoupleMainLoop", CouplePart::Initialise);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

void GhostPart::IniSynchros() {
    SynchroPhase* phase;

    phase = AllSynchro->AddPhase("GhostHC", GHOST_TIME_BEGIN, GHOST_TIME_END);

    //----------------- balance des viewports
    phase->AddTick(0, 4.8f,  SynchrosMess);
    phase->AddTick(1, 11.9f,  SynchrosMess);
    phase->AddTick(2, 16.9f,  SynchrosMess);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI GhostPart::SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user) {
    switch (syncNum) {
        //----------------- balance des viewports
    case 0:
        Monde->GetCamera("Camera02")->GetViewport().SetUni(GetUniXpos(360.f), GetUniXpos(600.f), GetUniYpos(284.f), GetUniYpos(438.f));
        Viewsp->Add(Monde, Monde->GetCamera("Camera02"), 1.0f, 3.0f, 1.0f);

        Effet2D->Go();
        break;
    case 1:
        Monde->GetCamera("Camera02")->GetViewport().SetUni(GetUniXpos(360.f), GetUniXpos(600.f), GetUniYpos(90.f), GetUniYpos(253.f));
        Viewsp->Add(Monde, Monde->GetCamera("Camera02"), 1.0f, 3.0f, 1.0f);
        break;

    case 2:
        Monde->GetCamera("Camera02")->GetViewport().SetUni(GetUniXpos(360.f), GetUniXpos(600.f), GetUniYpos(128.f), GetUniYpos(278.f));
        Viewsp->Add(Monde, Monde->GetCamera("Camera02"), 1.0f, 3.0f, 1.0f);
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI GhostPart::AffEclairs(Ufloat step, U32 user) {
    Ufloat rvba[4];

    Eclairs->AffAllPart(LapTime);

    //----------- affiche outline de la gonzesse dans la flotte
    ObjetU3D* obj = Monde->GetObjet("corps");
    Ufloat alpha = sin(LastTime*3.f);
    vec4_set(rvba, .3f, .3f, .7f, alpha);
    if (alpha> 0.0f) {
        alpha = 1.0f - alpha*.5f;
        obj->Ptab[0].m->SetColor(alpha, alpha, alpha);     // modifi couleur du mesh
        obj->GetWire().SetLineColor(rvba);
        U3D3Pipeline->Begin((U32)MatTranspAddAlpha, FALSE, TRUE);
        obj->GetWire().Affiche(Objet3dWire::cartoon);
        U3D3Pipeline->End(NULL);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI GhostPart::MainLoop(UScheduler* sche, U32 user) {
    if (FirstTime) {
        LastTime = ClockU3D::GetTime();
        FirstTime = FALSE;
    }


    //--------------- Calcul le LapTime
    LapTime = ClockU3D::GetTime() - LastTime;
    LastTime = ClockU3D::GetTime();


    if (FlipScreen)
        ClearOpenGLBuffers();

    Eclairs->IncAnim(LastTime, LapTime);
    Monde->UpdateFrame((float)(FRAMEPERSECOND*LapTime));

    Viewsp->Affiche(LapTime);

    Effet2D->UpdateFrame(LapTime);

    if (FlipScreen)
        FlipOpenGL();
    else
        ClearOpenGLZbuffer();

    //------------------- condition de sortie
    if ((ClockU3D::GetTime() - LaunchTime) >= GHOST_TIME_END) {
        sche->KillActuaTask();   // s'arrete soit meme
        sche->AddTask("GhostRelease", Release);     // lance le release
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

