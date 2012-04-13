	   				 /*---------------------------------------------------------------------+
					  |							Plans.cpp									|
					  |																		|
					  |	une partie un peu High tech assez deja vu, mais je voulais TROP en 	|
					  |			faire une paskeu j'aime TROP ca hahahaahahah				|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

//---------------- Scene infos :

#include "stdafx.h"



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|	Variables Locales	|
//									+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*		PlansPart::Scheduler=NULL		;
MondeU3D*		PlansPart::Monde=NULL			;
PlansAnim*		PlansPart::PAnim=NULL			;
PlansTransp*	PlansPart::PTransp=NULL			;
MondeU3D*		PlansPart::MondeClock = NULL	;
PlansClockAnim*	PlansPart::ClockAnim = NULL		;
PlansTextes*	PlansPart::AllTextes = NULL		;
PlansCache*		PlansPart::PartCache = NULL		;
UImage*			PlansPart::LigneHaut = NULL		;
UImage*			PlansPart::LigneBas = NULL		;
UImage*			PlansPart::LignePetite = NULL	;
Ufloat			PlansPart::GAlpha = 1.0f		;
Ufloat			PlansPart::IncGAlpha = 0.0f		;
bool			PlansPart::FlipScreen = TRUE	;
bool			PlansPart::HidenPart = FALSE	;
//bool			PlansPart::HidenPart = TRUE		;
bool			PlansPart::FirstTime=FALSE		;
Udouble			PlansPart::LapTime=0.0f			;
Udouble			PlansPart::LastTime=0.0f		;



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|  Class PlansPart	|
//										+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

// load des trucs avant la commencement de la séquence
void PlansPart::PreLoad()
{
	//------------ Load MondeU3D
	Monde = new MondeU3D									;
	Monde->LoadU3D( "Datas/meshes/Plans/Plans1.u3d" )	;
	Monde->GetStates().SortPolyTr = FALSE					;	// enleve trie des poly transparents

	MondeClock = new MondeU3D									;
	MondeClock->LoadU3D( "Datas/meshes/Plans/clock.u3d" )	;
	MondeClock->GetStates().SortPolyTr = FALSE					;	// enleve trie des poly transparents
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	PlansPart::Initialise(UScheduler *sche, U32 user)
{
	Monde->LoadTXM( "Datas/meshes/Plans/Plans.txm" )	;
	PAnim = new PlansAnim( Monde )						;
	PTransp	 = new PlansTransp( Monde )					;

	MondeClock->LoadTXM( "Datas/meshes/Plans/clock.txm" )	;
	ClockAnim = new PlansClockAnim( MondeClock )				;

	LigneHaut = new UImage( "Datas/meshes/Plans/bluefond.jpg", BILINEAR_TEXTURE, 512.f, 128.f )	;
	LigneHaut->SetTransp( MatTranspMulAlpha )														;
	LigneHaut->SetXSize( 1.0f )																		;
	LigneHaut->SetYSize( 1.f/3.f )																	;
	LigneHaut->SetUpLeftPos( 0.0f, 0.0f )															;

	LigneBas = new UImage										;
	(*LigneBas) = (*LigneHaut)									;
	LigneBas->SetCoord( 0.0f, 1.0f, 1.0f, 0.0f )				;
	LigneBas->SetPos( .5f, 1.0f - LigneBas->GetYSize()*.5f )	;

	LignePetite = new UImage( "Datas/meshes/Plans/ligne.jpg", BILINEAR_TEXTURE|TILE_TEXTURE)	;
	LignePetite->SetTransp( MatTranspAddAlpha )													;
	LignePetite->SetCoord( 0.0f, 100.f, 0.0f, 1.0f )											;
	LignePetite->SetXSize( 1.0f )																;
	LignePetite->SetPos( 0.5f, 0.5f )															;

	TextureManager::LoadAll()	;	// load toute les textures

	IniSynchros()	;

	//----------- Ini le scheduler interne
	Scheduler = new UScheduler	;

	//----------- Ini des flags divers et variés ( poil d'épervier )
	GAlpha = 1.0f		;
	IncGAlpha = 0.0f	;
	FirstTime = TRUE	;

	//----------- lance une autre tache !
	sche->KillActuaTask()						;	// s'arrete soit meme !
	sche->AddTask( "PlansMainLoop", MainLoop )	;	// lance la main loop
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	PlansPart::Release(UScheduler *sche, U32 user)
{
	//------------ Kill le scheduler
	DeleteBlinde( Scheduler )	;

	//------------ Kill les images
	DeleteBlinde( LigneHaut )	;
	DeleteBlinde( LigneBas )	;
	DeleteBlinde( LignePetite )	;

	//------------ Kill les textes
	DeleteBlinde( AllTextes )	;
	DeleteBlinde( PartCache )	;

	//------------ Kill MondeU3D
	DeleteBlinde( Monde )			;
	DeleteBlinde( PTransp )			;
	DeleteBlinde( PAnim )			;
	DeleteBlinde( MondeClock )		;
	DeleteBlinde( ClockAnim )		;

	//------------ lance les éventuelles taches suivantes
	if( sche )
	{
		sche->KillActuaTask()								;
		sche->AddTask( "OeilMainLoop", OeilPart::MainLoop )	;
		OeilPart::SetFlip( TRUE )							;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void PlansPart::IniSynchros()
{
	SynchroPhase* phase	;

	phase = AllSynchro->AddPhase( "PlansHC", PLANS_TIME_BEGIN, PLANS_TIME_END )	;

	phase->AddTick( 0, 1.8f,  SynchrosMess )	;	// modifie transparence de la 2émé scène
	phase->AddTick( 1, 4.4f,  SynchrosMess )	;	// modifie transparence de la 2émé scène
	phase->AddTick( 0, 8.0f,  SynchrosMess )	;	// modifie transparence de la 2émé scène
	phase->AddTick( 8, 12.0f,  SynchrosMess )	;	// modifie transparence de la 2émé scène

	// PS: interval entre chak coup de batterie : .86s !!
	phase->AddTick( 2, 1.404f,  SynchrosMess )		;	// apparaitre DStaff 
	phase->AddTick( 3, 3.124f,  SynchrosMess )		;	// apparaitre madman
	phase->AddTick( 4, 4.844f,  SynchrosMess )		;	// apparaitre Ghell
	phase->AddTick(13, 6.646f,  SynchrosMess )		;	// apparaitre Taggy
	phase->AddTick( 5, 8.284f,  SynchrosMess )		;	// apparaitre Tuo
	phase->AddTick( 6,10.004f,  SynchrosMess )		;	// apparaitre u2+code
	phase->AddTick( 7, 11.724f,  SynchrosMess )		;	// apparaitre u2+Gfx3d
	phase->AddTick( 10, 12.584f,  SynchrosMess )	;	// apparaitre music+FearFactory
	phase->AddTick( 14, 14.304f,  SynchrosMess )	;	// fait tout disparraitre

	for( U32 a=0; a<20; a++ )
		phase->AddTick( 11+(a&0x1), 1.404f + a*.86f,  SynchrosMess )		;	// fait sursauter la clock


	phase->AddTick( 9, 15.29f,  SynchrosMess )	;	// schedule l'oeil et commence a faire le fondu general
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	PlansPart::SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)
{
	switch( syncNum )
	{
		//----------------- modifie TRansparence 2eme scene de camera
		case 0:
			PTransp->SetTransp( 1.0f, 2.5f )	;
			break;
		case 1:
			PTransp->SetTransp( 0.5f, 0.5f )	;
			break;
		case 8:
			PTransp->SetTransp( 0.0f, 2.5f )	;
			break;

		//----------------- modifie les textes
		case 2:
			AllTextes = new PlansTextes( 0.5f )					;
			if( HidenPart ) PartCache = new PlansCache( 0.5f )	;

			TextureManager::LoadAll()			;	// load toute les textures
			break;
		case 3:
			AllTextes->OnAppearGfx3d( 0.5f )	;
			AllTextes->OnAppearMadman( 0.5f )	;
			if( PartCache )
				PartCache->OnAppearMadman( 0.5f )	;
			break;
		case 4:
			AllTextes->OnDisappearGfx3d( 0.5f )		;
			AllTextes->OnDisappearMadman( 0.5f )	;
			if( PartCache )
				PartCache->OnDisappearMadman( .5f )	;

			AllTextes->OnAppearGfx2d( 0.5f )	;
			AllTextes->OnAppearGhell( 0.5f )	;
			if( PartCache )
				PartCache->OnAppearGhell( .5f )	;
			break;
		case 13:
			AllTextes->OnDisappearGhell( 0.5f )	;
			if( PartCache )
				PartCache->OnDisappearGhell( .5f )	;

			AllTextes->OnAppearTaggy( 0.5f )	;
			if( PartCache )
				PartCache->OnAppearTaggy( .5f )	;
			break;
		case 5:
			AllTextes->OnDisappearGfx2d( 0.5f )	;
			AllTextes->OnDisappearTaggy( 0.5f )	;
			if( PartCache )
				PartCache->OnDisappearTaggy( .5f )	;

			AllTextes->OnAppearPrograming( 0.5f )	;
			AllTextes->OnAppearTuo( 0.5f )			;
			if( PartCache )
				PartCache->OnAppearTuo( .5f )	;
			break;
		case 6:
			AllTextes->OnDisappearTuo( 0.5f )	;
			if( PartCache )
				PartCache->OnDisappearTuo( .5f )	;

			AllTextes->OnAppearU2( 0.5f )		;
			if( PartCache )
				PartCache->OnAppearU2( .5f )	;
			break;
		case 7:
			AllTextes->OnDisappearPrograming( 0.5f )	;

			AllTextes->OnAppearGfx3d( 0.5f )	;
			break;
		case 10:
			//AllTextes->OnDisappear( 0.5f )		;
			AllTextes->OnDisappearGfx3d( 0.5f )	;
			AllTextes->OnDisappearU2( 0.5f )	;
			if( PartCache )
				PartCache->OnDisappearU2( .5f )	;

			AllTextes->OnAppearMusic( .5f )		;
			AllTextes->OnAppearFFactory( .5f )	;
			if( PartCache )
				PartCache->OnAppearFFactory( .5f )	;
			break;
		case 14:
			AllTextes->OnDisappear( 0.5f )			;
			AllTextes->OnDisappearFFactory( .5f )	;
			AllTextes->OnDisappearMusic( .5f )		;

			if( PartCache )
				PartCache->OnDisappearFFactory( .5f )	;
			break;

		//----------------- fait sursauter les engrenages
		case 11:
			ClockAnim->SetBouge( 1.0f )	;
			break;
		case 12:
			ClockAnim->SetBouge( -1.0f )	;
			break;

		//----------------- schedule l'oeil et commence a faire le fondu general
		case 9:

			IncGAlpha = -1.0f / (PLANS_TIME_END - OEIL_TIME_BEGIN)	;

			//------ ini les materials de la scene
			for( Monde->IniMatIterator(); Monde->GetMatFromIterator(); Monde->IncMatIterator() )
			{
				if( Monde->GetMatFromIterator()->GetTrTex1() == MatTranspRien )
				{
					Monde->GetMatFromIterator()->SetTransp( MatTranspAddAlpha )	;
					Monde->GetMatFromIterator()->Confirm()						;
				}
			}
			for( MondeClock->IniMatIterator(); MondeClock->GetMatFromIterator(); MondeClock->IncMatIterator() )
			{
				if( strcmp( MondeClock->GetMatFromIterator()->GetNom(), "APE" )==0 )
				{
					MondeClock->GetMatFromIterator()->SetTransp( MatTranspAlpha )	;
					MondeClock->GetMatFromIterator()->Confirm()						;
				}
			}

			Scheduler->AddTask( "oeilInitialise", OeilPart::Initialise )	;	// lance la main loop de l'oeil
			OeilPart::SetFlip( FALSE )										;

			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void PlansPart::GereGAlpha( Ufloat laptime )
{
	GAlpha += IncGAlpha*laptime			;
	if( GAlpha <0.0f ) GAlpha = 0.0f	;

	for( Monde->IniMatIterator(); Monde->GetMatFromIterator(); Monde->IncMatIterator() )
		Monde->GetMatFromIterator()->SetAlpha( GAlpha )	;

	for( MondeClock->IniMatIterator(); MondeClock->GetMatFromIterator(); MondeClock->IncMatIterator() )
		if( MondeClock->GetMatFromIterator()->GetTrTex1() == MatTranspMulAlpha )
		{
			MondeClock->GetMatFromIterator()->SetRouge( MondeClock->GetMatFromIterator()->GetRouge()*GAlpha )	;
			MondeClock->GetMatFromIterator()->SetVert( MondeClock->GetMatFromIterator()->GetVert()*GAlpha )		;
			MondeClock->GetMatFromIterator()->SetBleu( MondeClock->GetMatFromIterator()->GetBleu()*GAlpha )		;
		}
		else
		{
			MondeClock->GetMatFromIterator()->SetAlpha( GAlpha )	;
		}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	PlansPart::MainLoop(UScheduler *sche, U32 user)
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

	Scheduler->Run()	;

	GereGAlpha( LapTime )	;

	PAnim->Anim( LapTime )										;
	Monde->UpdateFrame( (float)(FRAMEPERSECOND*LapTime*2.f) )	;

	//----------- affiche divers
	LignePetite->SetPos( .5f, .5f + .8f*Sin( LastTime*2.f ) )	;
	LignePetite->SetAlpha( GAlpha*fabs( Cos( LastTime ) ) )		;
	LignePetite->Affiche()										;

	LignePetite->SetPos( .5f, .5f + .8f*Cos( LastTime*2.f ) )		;
	LignePetite->SetAlpha( GAlpha*fabs( Sin( LastTime*.79f ) ) )	;
	LignePetite->Affiche()											;

	LigneHaut->Affiche()			;
	LigneHaut->SetAlpha( GAlpha )	;
	LigneBas->Affiche()				;
	LigneBas->SetAlpha( GAlpha )	;

	//----------- fin affiche divers

	ClockAnim->Anim( LastTime )									;	// calcul animation de la clock
	MondeClock->UpdateFrame( (float)(FRAMEPERSECOND*LapTime) )	;

	PTransp->Affiche( LapTime )	;

	if( AllTextes )	AllTextes->UpdateFrame( LapTime )	;
	if( PartCache ) PartCache->UpdateFrame( LapTime )	;

	if( FlipScreen )
		FlipOpenGL()	;
	else				
		ClearOpenGLZbuffer()	;


	//------------------- condition de sortie
	if(  (ClockU3D::GetTime() - LaunchTime) >= PLANS_TIME_END )
	{
		sche->KillActuaTask()						;	// s'arrete soit meme
		sche->AddTask( "PlansRelease", Release )	;	// lance le release
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
