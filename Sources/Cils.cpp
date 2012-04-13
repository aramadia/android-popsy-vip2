	   				 /*-------------------------------------------------------------------------+
					  |								Cils.cpp									|
					  |																			|
					  |	Encre une partie deja vue dans d'autre demos, mais j'adoooore alors 	|
					  |			bon je pouvais pas m'empécher de la foutre						|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#include "stdafx.h"



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|	Variables Locales	|
//									+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*		CilsPart::Scheduler = NULL	;
MondeU3D*		CilsPart::Monde = NULL		;
CilsSpheres*	CilsPart::Cils = NULL		;
CilsBack*		CilsPart::Back = NULL		;
CilsCarotus*	CilsPart::Carotus = NULL	;
CilsTextes*		CilsPart::Textes = NULL		;
UImage*			CilsPart::Fondu = NULL		;
bool			CilsPart::FirstTime = TRUE	;
bool			CilsPart::FlipScreen = TRUE	;
Udouble			CilsPart::LapTime = 0.0f	;
Udouble			CilsPart::LastTime = 0.0f	;




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|  Class CilsPart	|
//										+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void CilsPart::PreLoad()
{
	Monde = new MondeU3D	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	CilsPart::Initialise(UScheduler *sche, U32 user)
{

	Monde->AddAFDCallback( "cils", AffCils )	;

	Cils = new CilsSpheres	;

	Back = new CilsBack			;
	Carotus = new CilsCarotus	;
	Textes = new CilsTextes		;

	Fondu = new UImage														;
	Fondu->SetColor( 0.0f, 0.0f, 0.0f )										;
	Fondu->SetTransp( MatTranspAlpha )										;
	Fondu->InterpoAlpha( 1.0f, 0.0f, (CILS_TIME_END-GHOST_TIME_BEGIN)*.5f)	;
	Fondu->Disable()														;

	TextureManager::LoadAll()	;	// load toute les textures

//	mp3.Poll()	;	// le loading peu prendre + de temps ke prevu...
	
	IniSynchros()	;

	//----------- Ini le scheduler interne
	Scheduler = new UScheduler	;

	//----------- Ini des flags divers et variés ( poil d'épervier )
	FirstTime = TRUE	;

	//----------- lance une autre tache !
	sche->KillActuaTask()						;	// s'arrete soit meme !
	sche->AddTask( "CilsMainLoop", MainLoop )	;	// lance la main loop
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	CilsPart::Release(UScheduler *sche, U32 user)
{
	//------------ Kill le scheduler
	DeleteBlinde( Scheduler )	;

	//------------ Kill les images
	DeleteBlinde( Back )		;
	DeleteBlinde( Carotus )		;
	DeleteBlinde( Textes )		;
	DeleteBlinde( Fondu )		;

	//------------ Kill MondeU3D
	DeleteBlinde( Cils )		;
	DeleteBlinde( Monde )		;

	//------------ lance les éventuelles taches suivantes
	if( sche )
	{
		sche->KillActuaTask()									;
		sche->AddTask( "GhostMainLoop", GhostPart::MainLoop )	;
		GhostPart::SetFlip( TRUE )								;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void CilsPart::IniSynchros()
{
	SynchroPhase* phase	;

	phase = AllSynchro->AddPhase( "CilsHC", CILS_TIME_BEGIN, CILS_TIME_END )	;

	//----------------- va balancer la scene de ghost en arriere plan
	phase->AddTick( 0, 7.97f, SynchrosMess )	;

	//----------------- va lancerl a dispparition du background
	phase->AddTick( 1, 6.97f, SynchrosMess )	;

	//----------------- balance des bidules carotus
	phase->AddTick( 2, 4.0f, SynchrosMess )	;
	phase->AddTick( 2, 6.0f, SynchrosMess )	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	CilsPart::SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)
{

	switch( syncNum )
	{
		//----------------- va balancer la scen de ghost en arriere plan
		case 0:
			Scheduler->AddTask( "GhostInitialise", GhostPart::Initialise )	;	// lance la main loop de ghost
			GhostPart::SetFlip( FALSE )										;

			Fondu->Enable()														;	// reactive le fondu
			Textes->Disappear( CILS_TIME_END-GHOST_TIME_BEGIN )					;	// fait disparaitre le texte
			Cils->InterpoAlpha( 1.0f, 0.0f, CILS_TIME_END-GHOST_TIME_BEGIN )	;
			break;

		//----------------- va lancerl a dispparition du background
		case 1:
			Back->Disappear( 1.0f )	;
			break;

		//----------------- balance des bidules carotus
		case 2:
			Carotus->Go()	;
			break;
	}

}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	CilsPart::AffCils(Ufloat step, U32 user)
{
	Cils->Affiche( LastTime, LapTime )	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	CilsPart::MainLoop(UScheduler *sche, U32 user)
{
	if( FirstTime )
	{
		LastTime = ClockU3D::GetTime()	;
		FirstTime = FALSE				;

		Carotus->Go()	;
	}


	//--------------- Calcul le LapTime
	LapTime = ClockU3D::GetTime() - LastTime	;
	LastTime = ClockU3D::GetTime()				;


	if( FlipScreen )
		ClearOpenGLBuffers()	;

	Scheduler->Run()	;

	Fondu->Affiche( LapTime )	;
	if( !Fondu->IsInterpoAlpha() )
		Fondu->Disable()	;

	Back->UpdateFrame( LastTime, LapTime )	;

	Monde->UpdateFrame( (float)(FRAMEPERSECOND*LapTime) )	;

	Carotus->UpdateFrame( LapTime )	;
	Textes->UpdateFrame( LapTime )	;


	if( FlipScreen )
		FlipOpenGL()	;
	else
		ClearOpenGLZbuffer()	;

	//------------------- condition de sortie
	if(  (ClockU3D::GetTime() - LaunchTime) >= CILS_TIME_END )
	{
		sche->KillActuaTask()					;	// s'arrete soit meme
		sche->AddTask( "CilsRelease", Release )	;	// lance le release
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
