	   				 /*---------------------------------------------------------------------+
					  |							Oeil.cpp									|
					  |																		|
					  |	Une sacrée partie ou l'on est en giga gros plan sur un oeil humain 	|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/


#include "stdafx.h"



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|	Variables Locales	|
//									+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*	OeilPart::Scheduler = NULL		;
MondeU3D*	OeilPart::Monde = NULL			;
OeilLigne*	OeilPart::Lignes = NULL			;
OeilDefor*	OeilPart::Deformation = NULL	;
OeilTextes*	OeilPart::Textes = NULL			;
UImage*		OeilPart::Flash = NULL			;
UImage*		OeilPart::Fondu = NULL			;
bool		OeilPart::affCilAdd = FALSE		;
U32			OeilPart::IncDef = 0			;
bool		OeilPart::FlipScreen = TRUE		;
bool		OeilPart::FirstTime = NULL		;
Udouble		OeilPart::LapTime = 0.0			;
Udouble		OeilPart::LastTime = 0.0f		;



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|  Class OeilPart	|
//										+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void OeilPart::PreLoad()
{
	//------------ Load MondeU3D
	Monde = new MondeU3D								;
	Monde->LoadU3D( "Datas/meshes/Oeil/morph.u3d" )	;
	Monde->LoadTXM( "Datas/meshes/Oeil/morph.txm" )	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	OeilPart::Initialise(UScheduler *sche, U32 user)
{
//	Monde->LoadTXM( "Datas/meshes/Oeil/Oeil.txm" )	;
	Monde->SetAllUVcalc( ObjetU3D::envStage1 )			;

	Lignes = new OeilLigne		;

	Textes = new OeilTextes	;

	Flash = new UImage						;
	Flash->SetTransp( MatTranspAddAlpha )	;
	Flash->SetSize( 1.0f )					;

	Fondu = new UImage					;
	Fondu->SetTransp( MatTranspAlpha )	;
	Fondu->SetSize( 1.0f )				;
	Fondu->SetColor( 0.0f, 0.0f, 0.0f )	;
	Fondu->Disable()					;


	TextureManager::LoadAll()	;	// load toute les textures

//	mp3.Poll()	;	// le loading peu prendre + de temps ke prevu...


	IniSynchros()	;

	//----------- Ini le scheduler interne
	Scheduler = new UScheduler	;

	//----------- Ini des flags divers et variés ( poil d'épervier )
	FirstTime = TRUE	;
	IncDef = 0			;

	//----------- lance une autre tache !
	sche->KillActuaTask()						;	// s'arrete soit meme !
	sche->AddTask( "OeilMainLoop", MainLoop )	;	// lance la main loop
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	OeilPart::Release(UScheduler *sche, U32 user)
{
	//------------ Kill le scheduler
	DeleteBlinde( Scheduler )	;

	//------------ Kill les images
	DeleteBlinde( Flash )	;
	DeleteBlinde( Fondu )	;

	//------------ Kill effet2D
	DeleteBlinde( Lignes )		;
	DeleteBlinde( Deformation )	;
	DeleteBlinde( Textes )		;

	//------------ Kill MondeU3D
	DeleteBlinde( Monde )	;

	//------------ lance les éventuelles taches suivantes
	if( sche )
	{
		sche->KillActuaTask()										;
		sche->AddTask( "CilsMainLoop", CilsPart::MainLoop )			;
		CilsPart::SetFlip( TRUE )									;
		CilsPart::GetCilsSphere()->InterpoAlpha( 1.0f, 1.0f, 1.0f )	;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void OeilPart::IniSynchros()
{
	SynchroPhase* phase	;

	phase = AllSynchro->AddPhase( "OeilHC", OEIL_TIME_BEGIN, OEIL_TIME_END )	;

	phase->AddTick( 0, 15.352f,  SynchrosMess )	;	// synchros roulement de batterie a la fin

	for( U32 a=0; a<10; a++ )
		phase->AddTick( (a&0x1)+1, 15.392f+a*.2f,  SynchrosMess )	;	// synchros roulement de batterie a la fin

	phase->AddTick( 3, 17.392f,  SynchrosMess )	;	// dernière deformation
	phase->AddTick( 4, 17.9f,  SynchrosMess )	;	// balance le fondu

	//--------------- synchros pour les petite images ki switchent
	for( U32 a=0; a<10; a++ )
	{
		phase->AddTick( 6, 0.792f + a*0.912f, SynchrosMess  )			;
		phase->AddTick( 5, 0.792f + .46f + a*0.912f, SynchrosMess  )	;
	}
	for( U32 a=0; a<10; a++ )
	{
		phase->AddTick( 6, 0.792f + 10*0.912f + a*.46f, SynchrosMess  )			;
		phase->AddTick( 5, 0.792f + .46f + 10*0.912f + a*.46f, SynchrosMess  )	;
	}
	for( U32 a=0; a<15; a++ )
	{
		phase->AddTick( 6, 0.792f + 15*0.912f + a*.23f, SynchrosMess  )			;
		phase->AddTick( 5, 0.792f + .46f + 15*0.912f + a*.23f, SynchrosMess  )	;
	}

	//--------------- synchros pour apparition du revive
	phase->AddTick( 7, 6.4f, SynchrosMess )		;
	phase->AddTick( 8, 7.77f, SynchrosMess )	;
	phase->AddTick( 9, 8.22f, SynchrosMess )	;
	phase->AddTick( 10, 9.7f, SynchrosMess )	;

	//--------------- synchros pour apparition du VIP II ( yeahhh )
	phase->AddTick( 11, 9.95f, SynchrosMess )	;
	phase->AddTick( 12, 10.35f, SynchrosMess )	;
	phase->AddTick( 13, 10.75f, SynchrosMess )	;
	phase->AddTick( 14, 11.15f, SynchrosMess )	;
	phase->AddTick( 15, 12.15f, SynchrosMess )	;

	//--------------- synchros pour la deformation des lignes
	phase->AddTick( 16, 7.77f, SynchrosMess  )	;
	phase->AddTick( 16, 7.87f, SynchrosMess  )	;
	phase->AddTick( 16, 7.97f, SynchrosMess  )	;
	phase->AddTick( 17, 9.95f, SynchrosMess  )	;
	phase->AddTick( 17, 10.35f, SynchrosMess  )	;
	phase->AddTick( 17, 10.75f, SynchrosMess  )	;
	phase->AddTick( 17, 11.15f, SynchrosMess  )	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	OeilPart::SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)
{
	switch( syncNum )
	{
		//----------------- roulement de batterie a la fin
		case 0:
			if( !Deformation )	// cas ou on serai deja passé par une allocation par synchro 1
			{
				Deformation = new OeilDefor	;
				TextureManager::LoadAll()	;	// load toute les textures

				Deformation->SetEnable( TRUE )			;
				CilsPart::Initialise( Scheduler,  0 )	;	// ini la partie des cils
				CilsPart::SetFlip( FALSE )				;
				Scheduler->SuspendAllTask()				;	// ne lance pas tout de suite la tache d'affichage des cils
			}
			break;

		case 1:
			if( !Deformation )	// pour prevenir du cas ou on louperai la synchro 0
			{
				Deformation = new OeilDefor	;
				TextureManager::LoadAll()	;	// load toute les textures

				Deformation->SetEnable( TRUE )			;
				CilsPart::Initialise( Scheduler,  0 )	;	// ini la partie des cils
				CilsPart::SetFlip( FALSE )				;
				Scheduler->SuspendAllTask()				;	// ne lance pas tout de suite la tache d'affichage des cils
			}

			if( IncDef >= 5 )	Deformation->OnDefoX( Random1()*Pi() )	;
			else				Deformation->OnDefoY( Random1()*Pi() )	;
			Flash->InterpoAlpha( .5f, 0.0f, .15f )	;
			affCilAdd = FALSE						;
			IncDef++								;
			break;
		case 2:
			Deformation->OnDefoZoom( Random1()*Pi() )							;
			Flash->InterpoAlpha( .3f, 0.0f, .15f )								;
			CilsPart::GetCilsSphere()->SetEnable( TRUE )						;
			CilsPart::GetCilsSphere()->InterpoAlpha( IncDef*.1f, .0f, .15f )	;
			affCilAdd = TRUE													;
			IncDef++															;
			break;
		case 3:
			CilsPart::GetCilsSphere()->InterpoAlpha( 1.0f, 1.0f, 1.0f )		;
			Deformation->OnCalcTunel( 0.9f )								;
			affCilAdd = FALSE												;
			Scheduler->ReScheduleAllTask()									;	// op on passe maintenant par la vrai MainLoop des cils
			break;
		case 4:
			Deformation->Goalpha()	;
			break;

		//---------------- fait defiler des petits textes
		case 5:
			Textes->OnAppearHori()	;
			break;
		case 6:
			Textes->OnAppearVerti()	;
			break;

		//--------------- synchros pour apparition du revive
		case 7:
			Textes->OnAppear1Revive( 1.4f )		;
			Textes->OnAttenuateTexte( 4.0f )	;
			break;
		case 8:
			Fondu->InterpoAlpha( 1.0f, 0.0f, .45f )	;
			Fondu->Enable()							;	// balance un fondu
			Textes->OnAppear2Revive( 0.45f )		;
			break;
		case 9:
			Textes->OnAppear3Revive( 0.8f )		;
			break;
		case 10:
			Textes->OnDisAppearRevive( 0.4f )	;
			break;

		//--------------- synchros pour apparition du VIP II ( yeahhh )
		case 11:
			Fondu->InterpoAlpha( 1.0f, 0.0f, 1.36f )	;
			Fondu->Enable()								;
			Textes->OnAppear1Vip( .4f )					;
			Textes->OnAttenuateTexte( 5.2f )			;
			break;
		case 12:
			Textes->OnAppear2Vip( .4f )			;
			break;
		case 13:
			Textes->OnAppear3Vip( .4f )			;
			break;
		case 14:
			Textes->OnAppear4Vip( 1.0f )		;
			break;
		case 15:
			Textes->OnDisAppearVip( 3.0f )		;
			break;

		//--------------- synchros pour la deformation des lignes
		case 16:
			Lignes->Deform( .5f )	;
			break;
		case 17:
			Lignes->Deform( .5f )	;
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void OeilPart::AffCilsAdd()
{
	MondeU3D* save	;

	save = SetActualMondeU3D( CilsPart::GetCilsMonde() )	;
	CilsPart::GetCilsSphere()->Affiche( LastTime, LapTime )	;
	SetActualMondeU3D( save )								;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	OeilPart::MainLoop(UScheduler *sche, U32 user)
{
	if( FirstTime )
	{
		LastTime = ClockU3D::GetTime()	;
		FirstTime = FALSE				;
	}


	//--------------- Calcul le LapTime
	LapTime = ClockU3D::GetTime() - LastTime	;
	LastTime = ClockU3D::GetTime()				;


	if( FlipScreen )
		ClearOpenGLBuffers()	;

	//----- partie normale
	if( !Deformation )
	{
		Monde->UpdateFrame( (float)(FRAMEPERSECOND*LapTime) )	;
		Lignes->Affiche( LapTime )								;

		Fondu->Affiche( LapTime )	;
		if( !Fondu->IsInterpoAlpha() )
			Fondu->Disable()	;

		Textes->UpdateFrame( LapTime )	;
	}
	//----- partie normale avec les roulements de batteries
	else
	{
		Scheduler->Run()					;
		Deformation->UpdateFrame( LapTime )	;
		if( affCilAdd ) AffCilsAdd()		;	// affiche les cils en additif kan enablé
		Flash->Affiche( LapTime	)			;
	}


	if( FlipScreen )
		FlipOpenGL()	;
	else
		ClearOpenGLZbuffer()	;

	//------------------- condition de sortie
	if(  (ClockU3D::GetTime() - LaunchTime) >= OEIL_TIME_END )
	{
		sche->KillActuaTask()					;	// s'arrete soit meme
		sche->AddTask( "OeilRelease", Release )	;	// lance le release
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
