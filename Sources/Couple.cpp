					 /*-----------------------------------------------------------------+
					  |							Couple.cpp								|
					  |																	|
					  |	 normalement une magnifik scene avec un couple sous la pluie	|
					  |			battante ki s'envol vers un monde meilleur				|
					  |					( snif c'est émouvant... )						|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/

#include "stdafx.h"

#define NB_MAX_CAMERASC1	5	// donne le nombre de camera ki y a dans les scenes
#define NB_MAX_CAMERASC2	5	// donne le nombre de camera ki y a dans les scenes

#define TIME_FLASH 1.0f		// temps d'un flash

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|	Variables Locales	|
//									+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*		CouplePart::Scheduler = NULL	;
MondeU3D*		CouplePart::Monde = NULL		;
MondeU3D*		CouplePart::Monde1 = NULL		;
MondeU3D*		CouplePart::ActuaMonde = NULL	;
bool			CouplePart::ScenePluie = TRUE	;
U32				CouplePart::ActuaCamSc1 = -1	;
U32				CouplePart::ActuaCamSc2 = -1	;
CouplePetals*	CouplePart::Petals = NULL		;
CoupleRPluie*	CouplePart::Bruine = NULL		;
PluieL*			CouplePart::pluieLigne = NULL	;
PluieP*			CouplePart::pluiePoly = NULL	;
CoupleTextes*	CouplePart::Textes = NULL		;
UImage*			CouplePart::Flash = NULL		;
UImage*			CouplePart::Fondu = NULL		;
bool			CouplePart::FirstTime = TRUE	;
Udouble			CouplePart::LapTime = 0.0f		;
Udouble			CouplePart::LastTime = 0.0f		;



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|  Class CouplePart	|
//										+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void CouplePart::PreLoad()
{
	Monde = new MondeU3D										;
	Monde->LoadU3D( "Datas/meshes/Couple/Couple03.u3d" )		;
	Monde->ConstructAllObjStaticLighting()						;
	Monde->SetActualCamera( Monde->GetCamera("Camera01") )		;
	Monde->LoadTXM( "Datas/meshes/Couple/couple.txm" )		;

	Monde1 = new MondeU3D										;
	Monde1->LoadU3D( "Datas/meshes/Couple/Cpl_fnt.u3d" )		;
	Monde1->ConstructAllObjStaticLighting()						;
	Monde1->SetActualCamera( Monde1->GetCamera("Camera01") )	;
	Monde1->LoadTXM( "Datas/meshes/Couple/couple_fnt.txm" )	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	CouplePart::Initialise(UScheduler *sche, U32 user)
{
	ActuaMonde = Monde											;
	ScenePluie = TRUE											;
/*	ActuaMonde = Monde1											;
	ScenePluie = FALSE											;*/

	Petals = new CouplePetals( Monde1 )	;
	Bruine = new CoupleRPluie( Monde )	;
	pluieLigne = new PluieL				;
	pluiePoly = new PluieP( Monde )		;


	Textes = new CoupleTextes	;

	Flash = new UImage( "Datas/meshes/Couple/Flash.jpg"  )	;
	Flash->SetSize( 1.0f )										;
	Flash->SetPos( 0.5f, 0.5f )									;
	Flash->SetTransp( MatTranspAddAlpha )						;
	Flash->InterpoAlpha( 1.0f, 0.0f, TIME_FLASH )				;	// fait un flash des le debut

	Fondu = new UImage					;
	Fondu->SetColor( 0.0f, 0.0f, 0.0f )	;
	Fondu->SetSize( 1.0f )				;
	Fondu->SetTransp( MatTranspAlpha )	;
	Fondu->Disable()					;

	TextureManager::LoadAll()	;	// load toute les textures


	IniSynchros()	;

	//----------- Ini le scheduler interne
	Scheduler = new UScheduler	;

	//----------- Ini des flags divers et variés ( poil d'épervier )
	FirstTime = TRUE	;
	ActuaCamSc1 = -1	;
	ActuaCamSc2 = -1	;

	//----------- lance une autre tache !
	sche->KillActuaTask()						;	// s'arrete soit meme !
	sche->AddTask( "CoupleMainLoop", MainLoop )	;	// lance la main loop

}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	CouplePart::Release(UScheduler *sche, U32 user)
{
	//------------ Kill le scheduler
	DeleteBlinde( Scheduler )	;

	//------------ Kill les effets 2d
	DeleteBlinde( Flash )	;
	DeleteBlinde( Textes )	;
	DeleteBlinde( Fondu )	;

	//------------ Kill MondeU3D
	DeleteBlinde( Monde )		;
	DeleteBlinde( Monde1 )		;
	DeleteBlinde( Petals )		;
	DeleteBlinde( Bruine )		;
	DeleteBlinde( pluieLigne )	;
	DeleteBlinde( pluiePoly )	;

	//------------ lance les éventuelles taches suivantes
	if( sche )
	{
		sche->KillActuaTask()										;
		sche->AddTask( "MultiInitialise", MultiPart::Initialise )	;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void CouplePart::IniSynchros()
{
	SynchroPhase* phase	;

	phase = AllSynchro->AddPhase( "CoupleHC", COUPLE_TIME_BEGIN, COUPLE_TIME_END )	;

	//----------------- balance les switch de scene
	phase->AddTick( 0, 6.56f,  SynchrosMess )	;	// pase sur la scene 2
	phase->AddTick( 1, 13.184f,  SynchrosMess )	;	// pase sur la scene 1
	phase->AddTick( 0, 16.700f,  SynchrosMess )	;	// pase sur la scene 2
	phase->AddTick( 1, 20.503f,  SynchrosMess )	;	// pase sur la scene 1

	//---------------- balance les switch de camera
	for( U32 a=0; a<12; a++)
		phase->AddTick( a&0x1, .423f*a + 20.926f,  SynchrosMess )	;

	//---------------- balance les giga switch de scene, camera et fondu sur la fin
	for( U32 a=0; a<12; a++)
		phase->AddTick( 2+(a&0x1), .15f*a + 25.576f,  SynchrosMess )	;

	//---------------- balance le Again 2D
	phase->AddTick( 4, 13.184f,  SynchrosMess )	;	// pase sur la scene 1
}

//----------------------------------------------------------------------------------------------------------------------------------------
void WINAPI CouplePart::SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)
{
	switch( syncNum )
	{
		case 0:
			NextCamScene2()		;
			ScenePluie = FALSE	;
			ActuaMonde = Monde1	;

			Flash->Enable()									;
			Flash->InterpoAlpha( 1.0f, 0.0f, TIME_FLASH )	;
			break;

		case 1:
			NextCamScene1()		;
			ScenePluie = TRUE	;
			ActuaMonde = Monde	;

			Flash->Enable()									;
			Flash->InterpoAlpha( 1.0f, 0.0f, TIME_FLASH )	;
			break;

		//---------------- balance les giga switch de scene, camera et fondu sur la fin
		case 2:
			NextCamScene2()		;
			ScenePluie = FALSE	;
			ActuaMonde = Monde1	;

			Fondu->Enable()							;
			Fondu->InterpoAlpha( 0.0f, 1.0f, .18f )	;
			break;

		case 3:
			NextCamScene1()		;
			ScenePluie = TRUE	;
			ActuaMonde = Monde	;
			
			Fondu->Enable()							;
			Fondu->InterpoAlpha( 0.0f, 1.0f, .18f )	;
			break;

		case 4:
			Textes->GoAgain( 6.56f )	;
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void CouplePart::NextCamScene1()
{
	char camname[256];
	ActuaCamSc1 ++	;

	if( ActuaCamSc1 < NB_MAX_CAMERASC1 )
	{
		sprintf(camname, "Camera0%d", ActuaCamSc1+2 )	;
		Monde->SetActualCamera( camname )		;
		Monde->GetActualCamera()->IniAnim()				;	// la fait partir du debut de son anim
	}
	else
	{
		sprintf(camname, "Camera0%d", ActuaCamSc1%(NB_MAX_CAMERASC1-1) + 3 )     ;
		Monde->SetActualCamera( (LPCSTR)camname )							;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void CouplePart::NextCamScene2()
{
	char camname[256];
	ActuaCamSc2 ++	;

	if( ActuaCamSc2 < NB_MAX_CAMERASC2 )
	{
		sprintf(camname, "Camera0%d", ActuaCamSc2+1 );
		Monde1->SetActualCamera( camname )		;
		Monde1->GetActualCamera()->IniAnim()			;	// la fait partir du debut de son anim
	}
	else
	{
		sprintf(camname, "Camera0%d", ActuaCamSc2%(NB_MAX_CAMERASC1-2) + 3 )	;
		Monde1->SetActualCamera( (LPCSTR)camname )							;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	CouplePart::MainLoop(UScheduler *sche, U32 user)
{
	if( FirstTime )
	{
		LastTime = ClockU3D::GetTime()	;
		FirstTime = FALSE				;

/*		Monde->IniObjIterator()						;
		ObjetU3D* tmp = Monde->GetObjFromIterator()	;
		CoordU3D* tmpc = tmp->Ctab					;
		for( U32 a=0; a<tmp->nbcoords; a++,tmpc++ )
			vec3_set( tmpc->ecran, 0.0f, 0.0f, 0.0f)	;*/

		Textes->GoCorres()	;
	}


	//--------------- Calcul le LapTime
	LapTime = ClockU3D::GetTime() - LastTime	;
	LastTime = ClockU3D::GetTime()				;

	MondeU3D* save = SetActualMondeU3D( ActuaMonde )	;

	ClearOpenGLBuffers()	;


	if( ScenePluie )
	{
		Textes->UpdateFrameBack( LapTime )	;

		pluiePoly->IncAnim(	LapTime )							;
	//	pluieLigne->UpdateFrameBackGround( LapTime )			;
		Monde->UpdateFrame( (float)(FRAMEPERSECOND*LapTime) )	;
		Bruine->UpdateFrame( LapTime )							;
	//	pluieLigne->UpdateFrameForeGround( LapTime )			;

		Textes->UpdateFrameFore( LapTime )	;
	}
	else
	{
		Petals->IncAnim3D( LastTime, LapTime )					;
		Monde1->UpdateFrame( (float)(FRAMEPERSECOND*LapTime) )	;
//		Petals->UpdateFrame( LastTime, LapTime )				;

		Textes->UpdateFRameFore1( LapTime )	;
	}

	//--------- gestion du flash
	Flash->Affiche( LapTime )	;
	if( !Flash->IsInterpoAlpha() )
		Flash->Disable()	;

	Textes->UpdateFrameForeAll(	LapTime )	;
	
	//--------- gestion du fondu
	Fondu->Affiche( LapTime )	;
	if( !Fondu->IsInterpoAlpha() )
		Fondu->Disable()	;

	FlipOpenGL()	;

	SetActualMondeU3D( save )	;

	//------------------- condition de sortie
	if(  (ClockU3D::GetTime() - LaunchTime) >= COUPLE_TIME_END )
	{
		sche->KillActuaTask()						;	// s'arrete soit meme
		sche->AddTask( "CoupleRelease", Release )	;	// lance le release
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------
