/*-------------------------------------------------------------------------+
 |                          Final.cpp                                       |
 |                                                                          |
 |          la toute derniere scene de la demo poil au mal de dos           |
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/



#include "stdafx.h"

#define TAPPEAR_OUTLINE 5.f
#define TAPPEAR_FLAT    8.f
#define TAPPEAR_VOLUME  4.f
#define TCLIGN_STOP     4.f

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +-----------------------+
//                                  |   Variables Locales   |
//                                  +-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*     FinalPart::Scheduler = NULL;
MondeU3D*       FinalPart::Monde = NULL;
MondeU3D*       FinalPart::Back = NULL;
FinalSpline*    FinalPart::Spline = NULL;
BPeloche*       FinalPart::Bruit = NULL;
FinalTexte*     FinalPart::Textes = NULL;
MaterialU3D*    FinalPart::MatVolume = NULL;
Ufloat          FinalPart::VolumeLightIntens = 0.0f;
Ufloat          FinalPart::VolumeLightIncIntens = 0.0f;
Ufloat          FinalPart::Flat3DIntens = 0.0f;
Ufloat          FinalPart::Flat3DIncIntens = 0.0f;
Ufloat          FinalPart::Flat3DClign = 0.0f;
Ufloat          FinalPart::Flat3DIncClign = 0.0f;
Ufloat          FinalPart::Cartoon3DIntens = 0.0f;
Ufloat          FinalPart::Cartoon3DIncIntens = 0.0f;
bool            FinalPart::FirstTime = TRUE;
bool            FinalPart::FlipScreen = TRUE;
Udouble         FinalPart::LapTime = 0.0f;
Udouble         FinalPart::LastTime = 0.0f;


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class FinalPart  |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void FinalPart::PreLoad() {
    Monde = new MondeU3D;
    Monde->LoadU3D("Datas/meshes/Final/titre.u3d");
    Monde->ConstrAllObjWireDatas();
    Monde->AddAFDCallback("cartoon", AffCartoon);

    Back = new MondeU3D;
    Back->LoadU3D("Datas/meshes/Final/boules.u3d");
    Back->ConstrAllObjWireDatas();
    Back->AddAFDCallback("volumel", AffVolumeLight);
    Back->SetAllObjTypeDraw(ObjetU3D::dnone);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI FinalPart::Initialise(UScheduler* sche, U32 user) {
    Monde->LoadTXM("Datas/meshes/Final/titre.txm");
    Back->LoadTXM("Datas/meshes/Final/boules.txm");

    Spline = new FinalSpline(Monde);
    Spline->Appear(5.0f);

    TextureManager::SetRepertoire("");
    MatVolume = new MaterialU3D("Plam");
    MatVolume->SetTransp(MatTranspAddAlpha);
    MatVolume->SetShading(MatShadingFlat);
    MatVolume->SetTexture1(TextureManager::OpenJpg("Datas/meshes/Final/degrad.jpg", BILINEAR_TEXTURE|TILE_TEXTURE));
    MatVolume->Confirm();

    Bruit = new BPeloche;
    Textes = new FinalTexte;

    TextureManager::LoadAll();   // load toute les textures

    IniSynchros();

    //----------- Ini le scheduler interne
    Scheduler = new UScheduler;

    //----------- Ini des flags divers et variés ( poil d'épervier )
    VolumeLightIntens = 0.0f;
    VolumeLightIncIntens = 0.0f;
    Flat3DIntens = 0.0f;
    Flat3DIncIntens = 0.0f;
    Flat3DClign = 0.0f;
    Flat3DIncClign = 0.0f;
    Cartoon3DIntens = 0.0f;
    Cartoon3DIncIntens = 0.0f;
    FirstTime = TRUE;

    //----------- lance une autre tache !
    sche->KillActuaTask();   // s'arrete soit meme !
    sche->AddTask("FinalMainLoop", MainLoop);     // lance la main loop
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI FinalPart::Release(UScheduler* sche, U32 user) {
    //------------ Kill le scheduler
    DeleteBlinde(Scheduler);

    //------------ Kill effet 2d
    DeleteBlinde(MatVolume);
    DeleteBlinde(Bruit);
    DeleteBlinde(Textes);

    //------------ Kill MondeU3D
    DeleteBlinde(Monde);
    DeleteBlinde(Back);
    DeleteBlinde(Spline);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void FinalPart::IniSynchros() {
    SynchroPhase* phase;

    phase = AllSynchro->AddPhase("FinalHC", FINAL_TIME_BEGIN, FINAL_TIME_BEGIN+30.f);         // !!!! TMP !!!!

    phase->AddTick(0, 5.f,  SynchrosMess);     // balance l'affichage des outlines
    phase->AddTick(1, 7.f,  SynchrosMess);     // balance l'affichage en flat
    phase->AddTick(2, 5.f,  SynchrosMess);     // balance l'affichage des lumières volumiks
    phase->AddTick(3, 13.f,  SynchrosMess);     // stop le clignotement du flat
    phase->AddTick(4, 16.f,  SynchrosMess);     // balance url

    phase->AddTick(5, 17.f, SynchrosMess);     // balance le scroll texte
    phase->AddTick(6, 27.f, SynchrosMess);     // balance le fondu
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI FinalPart::SynchrosMess(SynchroTick* sync, Ufloat relativeTime, U32 syncNum, U32 user) {
    switch (syncNum) {
    case 0:
        Cartoon3DIntens = 0.0f;
        Cartoon3DIncIntens = 1.f / (TAPPEAR_OUTLINE);
        break;
    case 1:
        Flat3DIntens = 0.0f;
        Flat3DIncIntens = 1.f / (TAPPEAR_FLAT);
        break;
    case 2:
        VolumeLightIntens = 0.0f;
        VolumeLightIncIntens = 1.f / (TAPPEAR_VOLUME);
        break;
    case 3:
        Flat3DClign = 0.0f;
        Flat3DIncClign = 1.f / (TCLIGN_STOP);
        break;
    case 4:
        Textes->GoUrl();
        break;
    case 5:
        Textes->GoScroll();
        break;
    case 6:
        Textes->GoFondu();
        break;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI FinalPart::AffCartoon(Ufloat step, U32 user) {
    if (Cartoon3DIntens> 0.0f) {
        Ufloat coul[4];

        vec4_set(coul, .3f, .3f, .7f, fabs(Cartoon3DIntens*cos(LastTime*2.f+Random1())));

        U3D3Pipeline->Begin((U32)MatTranspAddAlpha);

        for (U3D3Monde3D->IniObjIterator(); U3D3Monde3D->GetObjFromIterator(); U3D3Monde3D->IncObjIterator()) {
            U3D3Monde3D->GetObjFromIterator()->GetWire().SetLineColor(coul);
            U3D3Monde3D->GetObjFromIterator()->GetWire().Affiche(Objet3dWire::cartoon);
        }

        U3D3Pipeline->End(NULL);
    }

    Spline->UpdateFrame(LapTime);
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI FinalPart::AffVolumeLight(Ufloat step, U32 user) {
    Ufloat pos[3] = {0.5f+.2f*cos(LastTime*.6f), 0.5f+.2f*sin(LastTime*.5f+.123f), 1500.0f/* + 1000.f*sin(LastTime*4.4f)*/ };

    U3D3Monde3D->GetActualCamera()->Cam2MondePos(pos, pos);

    /*  U3D3Pipeline->Begin( (U32)MatTranspAddAlpha );

        for( U3D3Monde3D->IniObjIterator(); U3D3Monde3D->GetObjFromIterator(); U3D3Monde3D->IncObjIterator() )
            U3D3Monde3D->GetObjFromIterator()->GetWire().Affiche( Objet3dWire::cartoon );

        U3D3Pipeline->End( NULL );*/

//  MatVolume->SetTex1Pos( LastTime, 0.0f, 1.0f, 1.0f );

    if (VolumeLightIntens>0.0f) {
        MatVolume->SetAlpha(VolumeLightIntens * .5f*(.6f + .3f*sin(LastTime*.9f)));

        for (U3D3Monde3D->IniObjIterator(); U3D3Monde3D->GetObjFromIterator(); U3D3Monde3D->IncObjIterator()) {
            U3D3Monde3D->GetObjFromIterator()->GetWire().SetPosVolumeL(pos);
            U3D3Monde3D->GetObjFromIterator()->GetWire().SeTailleVolumeL(500.f);
            U3D3Monde3D->GetObjFromIterator()->GetWire().Affiche(Objet3dWire::volumelight, MatVolume);
        }

        U3D3Pipeline->Affiche();       // affiches tout les polys !
        U3D3Pipeline->Release();       // detruits tout les polys temporaires  */
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------

void FinalPart::IncVars() {
    VolumeLightIntens += LapTime*VolumeLightIncIntens;
    if (VolumeLightIntens>1.0f) VolumeLightIntens = 1.0f;
    Flat3DIntens += LapTime*Flat3DIncIntens;
    if (Flat3DIntens>1.0f) Flat3DIntens = 1.0f;
    Flat3DClign += LapTime*Flat3DIncClign;
    if (Flat3DClign>1.0f) Flat3DClign = 1.0f;
    Cartoon3DIntens += LapTime*Cartoon3DIncIntens;
    if (Cartoon3DIntens>1.0f) Cartoon3DIntens = 1.0f;
    /*  VolumeLightIntens = 1.0f;
        Flat3DIntens = 1.0f;
        Flat3DClign = 1.0f;
        Cartoon3DIntens = 1.0f;*/
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI FinalPart::MainLoop(UScheduler* sche, U32 user) {
    if (FirstTime) {
        LastTime = ClockU3D::GetTime();
        FirstTime = FALSE;
    }


    //--------------- Calcul le LapTime
    LapTime = ClockU3D::GetTime() - LastTime;
    LastTime = ClockU3D::GetTime();

    if (FlipScreen)
        ClearOpenGLBuffers();

    IncVars();

    //--------- modifie alpha du material flat des lettres du titre
    Ufloat alpha = Flat3DIntens*.5f + Flat3DIntens*.5f*sin(LastTime*2.0f*2.f*M_PI);
    alpha = alpha*(1.0f-Flat3DClign) + Flat3DClign;
    Monde->IniMatIterator();
    Monde->GetMatFromIterator()->SetAlpha(alpha*.7f);

    Back->UpdateFrame((Ufloat)(FRAMEPERSECOND*LapTime*.2f));
    Monde->UpdateFrame((Ufloat)(FRAMEPERSECOND*LapTime));

    Bruit->UpdateFrame(LapTime);

    Textes->UpdateFrame(LapTime);

    if (FlipScreen)
        FlipOpenGL();
    else
        ClearOpenGLZbuffer();

    //------------------- condition de sortie
    if ((ClockU3D::GetTime() - LaunchTime) >= FINAL_TIME_END) {
//      CVip2Dlg::m_Active = FALSE;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------
