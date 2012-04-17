/*---------------------------------------------------------------------+
 |                       Neurone.cpp                                    |
 |                                                                      |
 |  une joliep partie ou la camera se ballade dans un champ de neurones |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +-----------------------+
//                                  |   Variables Locales   |
//                                  +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*         NeuronePart::Scheduler = NULL;
MondeU3D*           NeuronePart::Monde = NULL;
NeuroBackground*    NeuronePart::Background = NULL;
NeuroLines*         NeuronePart::Lines = NULL;
NeuroLights*        NeuronePart::Lights = NULL;
UImage*             NeuronePart::FlashBlanc = NULL;
UImage*             NeuronePart::FondNoir = NULL;
UImage*             NeuronePart::RotoTRuc = NULL;
XGrezille*          NeuronePart::Grezille = NULL;
NeuroTxt*           NeuronePart::Textes = NULL;
Ufloat              NeuronePart::CamTrembleIntensX = 0.0f;
Ufloat              NeuronePart::CamTrembleIntensY = 0.0f;
Viewport            NeuronePart::SCviewport;
bool                NeuronePart::FirstTime = TRUE;
Udouble             NeuronePart::LapTime;
Udouble             NeuronePart::LastTime;




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-----------------------+
//                                      |   Class NeuronePart   |
//                                      +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

// load des trucs avant la commencement de la séquence
void NeuronePart::PreLoad() {
    //------------ Load MondeU3D
    Monde = new MondeU3D;
    Monde->LoadU3D("Datas/meshes/neurones/Neur_dem.u3d");

    //----------- fait des inis sur le monde
    Monde->ConstrAllObjColTree();   // speeed collisions !
    Monde->ConstructAllObjStaticLighting(); // pre calc lumières
    Monde->GetActualCamera()->SetAnimCallBack(CameraTremblement);

    Monde->LoadTXM("Datas/meshes/neurones/neuro.txm");
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI NeuronePart::Initialise(UScheduler* sche, U32 user) {
    Background = new NeuroBackground;   // ini le background
    Background->Appear(1.35f);

    Lights = new NeuroLights(Monde);     // ini les lumière 2D

    Lines = new NeuroLines(Monde, Lights->GetBig());     // ini ligne ki rebondissent

    Textes = new NeuroTxt;  // ini les textes de la partie

    //----------- Ini diverse images
    FlashBlanc = new UImage;
    FlashBlanc->SetTransp(MatTranspAddAlpha);
    FlashBlanc->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    FlashBlanc->SetSize(1.0f);
    FlashBlanc->InterpoAlpha(1.0f, 0.0f, 2.5f);

    FondNoir = new UImage;
    FondNoir->SetColor(0.0f, 0.0f, 0.0f);
    FondNoir->SetSize(1.0f);
    FondNoir->Disable();

    RotoTRuc = new UImage("Datas/meshes/neurones/roto_truc.jpg");
    RotoTRuc->SetTransp(MatTranspAddAlpha);
    RotoTRuc->ConsProRota(FALSE);
    RotoTRuc->SetPos(RotoTRuc->GetXSize()*.5f, 1.f - RotoTRuc->GetYSize()*.5f);
    RotoTRuc->InterpoAngle(0.0f, -45.f, NEURO_TIME_END - NEURO_TIME_BEGIN);
    RotoTRuc->SetAlpha(0.0f);

    TextureManager::LoadAll();  // load toute les textures

//  mp3.Poll(); // le loading peu prendre + de temps ke prevu...

    Grezille = new XGrezille(400);   // alloue grezillements
    Grezille->SetAlpha(0.0f);


    //----------- Ini variable pour le tremblement de la camera
    CamTrembleIntensX = 0.0f;
    CamTrembleIntensY = 0.0f;


    //----------- Ini le scheduler interne
    Scheduler = new UScheduler;

    //----------- Ini les synchros
    IniSynchros();

#ifndef BNEURO_PART         // skip transition si on commence direct aux neuronnes
    /*  Scheduler->AddTask( "FFondu", Transi1Part::MainLoop );
        Transi1Part::Disappear( 1.35f );    // fait disparaitre la transition*/
#endif

    SCviewport.Set(0, Xres-1, 0, Yres-1);

    FirstTime = TRUE;

    //----------- lance une autre tache !
    sche->KillActuaTask();  // s'arrete soit meme !
    sche->AddTask("NeuroMainLoop", MainLoop);    // lance la main loop

}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI NeuronePart::Release(UScheduler* sche, U32 user) {

    //------------ Kill le scheduler
    DeleteBlinde(Scheduler);

    //----------- Kill diverse images
    DeleteBlinde(FlashBlanc);
    DeleteBlinde(FondNoir);
    DeleteBlinde(Grezille);

    //------------ Kill MondeU3D
    DeleteBlinde(Monde);
    DeleteBlinde(Background);
    DeleteBlinde(Lights);
    DeleteBlinde(Lines);
    DeleteBlinde(Textes);

    //------------ lance les éventuelles taches suivantes
    if (sche) {
        sche->KillActuaTask();
        sche->AddTask("PlansMainLoop", PlansPart::MainLoop);
        PlansPart::SetFlip(TRUE);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

void NeuronePart::IniSynchros() {
    SynchroPhase* phase;

    phase = AllSynchro->AddPhase("NeuroHC", NEURO_TIME_BEGIN, NEURO_TIME_END);

    for (U32 a=0; a<7; a++)
        phase->AddTick(a&0x1, 2.8f + a*2.888f, SynchrosMess);

    for (U32 a=0; a<7; a++)
        phase->AddTick(14, 2.8f+1.444f + a*2.888f, SynchrosMess);    // simplement des moindres grezillements

    /*  phase->AddTick( 0, 1.3f,  SynchrosMess );   // fait trembler la camera en X
        phase->AddTick( 1, 2.8f,  SynchrosMess );   // fait trembler la camera en Y
        phase->AddTick( 0, 5.6f,  SynchrosMess );   // fait trembler la camera en X
        phase->AddTick( 0, 8.6f,  SynchrosMess );   // fait trembler la camera en X
        phase->AddTick( 1, 11.5f,  SynchrosMess );  // fait trembler la camera en Y
        phase->AddTick( 0, 14.1f,  SynchrosMess );  // fait trembler la camera en X
        phase->AddTick( 1, 17.1f,  SynchrosMess );  // fait trembler la camera en Y
        phase->AddTick( 1, 20.0f,  SynchrosMess );  // fait trembler la camera en Y*/

    phase->AddTick(2, 5.2f,  SynchrosMess);    // There
    phase->AddTick(3, 6.0f,  SynchrosMess);    // Was
    phase->AddTick(4, 6.6f,  SynchrosMess);    // No
    phase->AddTick(5, 7.1f,  SynchrosMess);    // God
    phase->AddTick(6, 8.1f,  SynchrosMess);    // For
    phase->AddTick(7, 8.2f,  SynchrosMess);    // Me

    phase->AddTick(8, 21.9f,  SynchrosMess);     // setViewport
    phase->AddTick(9, 22.04f,  SynchrosMess);    // setViewport
    phase->AddTick(10, 22.1933f,  SynchrosMess);     // setViewport
    phase->AddTick(11, 22.34f,  SynchrosMess);       // setViewport
    phase->AddTick(12, 22.48f,  SynchrosMess);       // setViewport
    phase->AddTick(13, 22.63f,  SynchrosMess);       // setViewport
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI NeuronePart::SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user) {
    switch (syncNum) {
        //----------------- tremblement de camera
    case 0:
        CamTrembleIntensX = 1.0f;
        RotoTRuc->InterpoAlpha(1.0f, 0.0f, 1.f);
        Grezille->Interposalpha(1.0f, 0.0f, 1.5f);
        Textes->NextSlogan();
        break;

    case 1:
        CamTrembleIntensY = -1.0f;
        RotoTRuc->InterpoAlpha(1.0f, 0.0f, 1.f);
        Grezille->Interposalpha(1.0f, 0.0f, 1.5f);
        Textes->NextSlogan();
        break;

    case 14:
        RotoTRuc->InterpoAlpha(1.0f, 0.0f, 1.f);
        Grezille->Interposalpha(0.5f, 0.0f, 1.5f);
        break;

        //---------------- Synchros de Textes
    case 2:
        Textes->OnThere();
        break;
    case 3:
        Textes->OnWas();
        break;
    case 4:
        Textes->OnNo();
        break;
    case 5:
        Textes->OnGod();
        break;
    case 6:
        Textes->OnFor();
        break;
    case 7:
        Textes->OnMe();
        break;

        //--------------- Modifi taille du viewport
    case 8:
        SCviewport.SetUni(.1f, .9f, 0.0f, 1.0f);
        Monde->GetActualCamera()->GetViewport() = SCviewport;
        /*          Monde->GetActualCamera()->GetViewport().SetUni( .1f, .9f, 0.0f, 1.0f );
                    U3D3Pipeline->SetActuaViewport( Monde->GetActualCamera()->GetViewport() );*/

        //----- profite de cette synchro pour scheduler la partie des plans
        Scheduler->AddTask("PlansInitialise", PlansPart::Initialise);    // lance la main loop des plans
        PlansPart::SetFlip(FALSE);

        //----- affiche un fond noir pour eviter de voir derriere la scene
        FondNoir->Enable();

        RotoTRuc->InterpoAlpha(1.0f, 0.0f, 0.5f);
        break;

    case 9:
        SCviewport.SetUni(.1f, .9f, 0.0f, 1.0f);
        Monde->GetActualCamera()->GetViewport() = SCviewport;
        /*          Monde->GetActualCamera()->GetViewport().SetUni( .05f, 0.8f, 0.05f, .8f );
                    U3D3Pipeline->SetActuaViewport( Monde->GetActualCamera()->GetViewport() );*/
        RotoTRuc->InterpoAlpha(1.0f, 0.0f, 0.2f);
        break;
    case 10:
        SCviewport.SetUni(.3f, .95f, 0.3f, 0.95f);
        Monde->GetActualCamera()->GetViewport() = SCviewport;
        /*          Monde->GetActualCamera()->GetViewport().SetUni( .3f, 0.95f, 0.3f, 0.95f  );
                    U3D3Pipeline->SetActuaViewport( Monde->GetActualCamera()->GetViewport() );*/
        RotoTRuc->InterpoAlpha(1.0f, 0.0f, 0.2f);
        break;
    case 11:
        SCviewport.SetUni(.05f, .7f, 0.3f, 0.95f);
        Monde->GetActualCamera()->GetViewport() = SCviewport;
        /*          Monde->GetActualCamera()->GetViewport().SetUni( .05f, 0.7f, 0.3f, .95f );
                    U3D3Pipeline->SetActuaViewport( Monde->GetActualCamera()->GetViewport() );*/
        RotoTRuc->InterpoAlpha(1.0f, 0.0f, 0.2f);
        break;
    case 12:
        SCviewport.SetUni(.3f, .95f, 0.05f, 0.7f);
        Monde->GetActualCamera()->GetViewport() = SCviewport;
        /*          Monde->GetActualCamera()->GetViewport().SetUni(  .3f, 0.95f, 0.05f, .7f  );
                    U3D3Pipeline->SetActuaViewport( Monde->GetActualCamera()->GetViewport() );*/
        RotoTRuc->InterpoAlpha(1.0f, 0.0f, 0.2f);
        break;
    case 13:
        SCviewport.SetUni(.3f, .7f, 0.3f, 0.7f);
        Monde->GetActualCamera()->GetViewport() = SCviewport;
        /*          Monde->GetActualCamera()->GetViewport().SetUni( .3f, 0.7f, 0.3f, .7f );
                    U3D3Pipeline->SetActuaViewport( Monde->GetActualCamera()->GetViewport() );*/
        RotoTRuc->InterpoAlpha(1.0f, 0.0f, 0.2f);
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

// appellé a chak fois ke l'on calcul l'anim de la camera
void NeuronePart::CameraTremblement(Camera* cam, Ufloat inc, U32 user) {
    Ufloat vec1[3],vec2[3];

    vec3_mul(vec1, cam->GetBaseX(), CamTrembleIntensX*500.f/*(250.f + 250.f*Sin( LastTime*20.f ) )*/);
    vec3_mul(vec2, cam->GetBaseY(), CamTrembleIntensY*500.f/*(250.f + 250.f*Cos( LastTime*20.f ) )*/);
    vec3_add(vec1, vec2, vec1);

    vec3_add(cam->GetPos(), cam->GetPos(), vec1);
    vec3_add(cam->GetTarg(), cam->GetTarg(), vec1);

    CamTrembleIntensX += - CamTrembleIntensX / 8.f;
    CamTrembleIntensY += - CamTrembleIntensY / 8.f;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void NeuronePart::AffFlashBlanc(Ufloat laptime) {
    FlashBlanc->Affiche(laptime);
    if (!FlashBlanc->IsInterpoAlpha())
        FlashBlanc->Disable();
}

//----------------------------------------------------------------------------------------------------------------------------------------


void WINAPI NeuronePart::MainLoop(UScheduler* sche, U32 user) {
    if (FirstTime) {
        LastTime = ClockU3D::GetTime();
        FirstTime = FALSE;
    }

    SetActualMondeU3D(Monde);

    //--------------- Calcul le LapTime
    LapTime = ClockU3D::GetTime() - LastTime;
    LastTime = ClockU3D::GetTime();

    ClearOpenGLBuffers();

    Scheduler->Run();

    Viewport saveView = U3D3Pipeline->GetActuaViewport();   // sauve le viewport
    U3D3Pipeline->SetActuaViewport(SCviewport);

    FondNoir->Affiche();    // affiche un fond noir kan il faut

    //----------- affiche background
    Background->UpdateFrame(LapTime);

    //----------- calcul la translation de la texture !
    Monde->IniMatIterator();
    Monde->GetMatFromIterator()->SetTex1Pos(0.0f, LastTime*.5f, 1.0f, 1.0f);
    Monde->UpdateFrame((float)(FRAMEPERSECOND*LapTime));

    Lines->UpdateFrame(LapTime);
    Lights->Affiche(LapTime);
    Textes->Affiche(LapTime);
    Grezille->UpdateFrame(LapTime);
    AffFlashBlanc(LapTime);


    U3D3Pipeline->SetActuaViewport(saveView);    // remet l'ancien viewport en place

    //-------- affiche la sorte de rond en bas a gauche
    RotoTRuc->Affiche(LapTime);

    FlipOpenGL();

    //------------------- condition de sortie
    if ((ClockU3D::GetTime() - LaunchTime) >= NEURO_TIME_END) {
        sche->KillActuaTask();  // s'arrete soit meme
        sche->AddTask("NeuroRelease", Release);      // lance le release
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------
