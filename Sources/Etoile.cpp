/*-------------------------------------------------------------------------+
 |                          Etoile.cpp                                      |
 |                                                                          |
 |  Cete partie s'appelle etoile non pas paskeu ya des myriades d'objets    |
 |  Celestes, mais plutot paskeu l'objet ki se deforme au 1er plan ressemble|
 |          a une salle etoile de mer toute moisie vala.                    |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#include "stdafx.h"

#define TIME_FONDU 0.5f

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +-----------------------+
//                                  |   Variables Locales   |
//                                  +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*     EtoilePart::Scheduler = NULL;
MondeU3D*       EtoilePart::Monde = NULL;
ObjDeformation* EtoilePart::EtoileDef = NULL;
EtoileScale*    EtoilePart::Scale = NULL;
EtoileGreets*   EtoilePart::Greets = NULL;
UImage*         EtoilePart::Fondu = NULL;
U32             EtoilePart::TextePos = 0;
bool            EtoilePart::FirstTime = TRUE;
Udouble         EtoilePart::LapTime = 0.0f;
Udouble         EtoilePart::LastTime = 0.0f;




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class EtoilePart |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void EtoilePart::PreLoad() {
    Monde = new MondeU3D;
    Monde->LoadU3D("Datas/meshes/etoile/scene.u3d");
    Monde->GetStates().SortPolyTr = FALSE;  // ne trie pas les poly transparents
    Monde->AddAFDCallback("Scales", AffScale);
    Monde->LoadTXM("Datas/meshes/etoile/scene.txm");
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI EtoilePart::Initialise(UScheduler* sche, U32 user) {


    for (Monde->IniObjIterator(); Monde->GetObjFromIterator(); Monde->IncObjIterator()) {
        if (strcmp(Monde->GetObjFromIterator()->GetNom(), "zarb")==0) {
            Monde->GetObjFromIterator()->SetUVcalc(ObjetU3D::envStage0);
            EtoileDef = new ObjDeformation(Monde->GetObjFromIterator(), 20.f);
            EtoileDef->Confirm();
            break;
        }
    }

    Scale = new EtoileScale(Monde);

    Greets = new EtoileGreets(6.f);

    Fondu = new UImage;
    Fondu->SetColor(0.0f, 0.0f, 0.0f);
    Fondu->SetSize(1.0f);
    Fondu->SetTransp(MatTranspAlpha);
    Fondu->Disable();

    TextureManager::LoadAll();  // load toute les textures


    IniSynchros();

    //----------- Ini le scheduler interne
    Scheduler = new UScheduler;

    //----------- Ini des flags divers et variés ( poil d'épervier )
    TextePos = 0;
    FirstTime = TRUE;

    //----------- lance une autre tache !
    sche->KillActuaTask();  // s'arrete soit meme !
    sche->AddTask("EtoileMainLoop", MainLoop);   // lance la main loop
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI EtoilePart::Release(UScheduler* sche, U32 user) {
    //------------ Kill le scheduler
    DeleteBlinde(Scheduler);

    //------------ Kill les images
    DeleteBlinde(Greets);

    //------------ Kill MondeU3D
    DeleteBlinde(EtoileDef);
    DeleteBlinde(Scale);
    DeleteBlinde(Monde);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void EtoilePart::IniSynchros() {
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI EtoilePart::SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user) {
}

//----------------------------------------------------------------------------------------------------------------------------------------

void EtoilePart::AffScale(Ufloat step, U32 user) {
    Scale->UpdateFrame(LapTime);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void EtoilePart::NextCamera() {
    Greets->GoScreen(TextePos);
    TextePos++;

    Fondu->Enable();
    Fondu->InterpoAlpha(1.0f, 0.0f, TIME_FONDU);     // fait un fondu en ouverture a chak changement

    FirstTime = TRUE;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI EtoilePart::MainLoop(UScheduler* sche, U32 user) {
    if (FirstTime) {
        LastTime = ClockU3D::GetTime();
        FirstTime = FALSE;
    }


    //--------------- Calcul le LapTime
    LapTime = ClockU3D::GetTime() - LastTime;
    LastTime = ClockU3D::GetTime();


    ClearOpenGLBuffers();


    EtoileDef->Anim(LastTime*3.f);
    Monde->UpdateFrame((float)(FRAMEPERSECOND*LapTime*.6f));

    Fondu->Affiche(LapTime);
    if (!Fondu->IsInterpoAlpha())
        Fondu->Disable();

    Greets->AfficheFore(LapTime);


    FlipOpenGL();
}

//----------------------------------------------------------------------------------------------------------------------------------------
