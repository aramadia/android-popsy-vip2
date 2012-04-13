	   				 /*-----------------------------------------------------------------+
					  |				         Nebuleuse.cpp								|
					  |																	|
					  |	C'est le debut de la demo en fait avec une jolie né... 			|
					  |( je vous laisse deviner la suite je sais ke vous etes joueurs )	|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/

#include "stdafx.h"


//#define TIME_SORTIE	30.790f	// temps ou se fini la séquence
#define TIME_SORTIE	30.590f	// temps ou se fini la séquence


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|	Variables Locales	|
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

UScheduler*		NebuleusePart::Scheduler = NULL		;
MondeU3D*		NebuleusePart::Monde = NULL			;
Nebuparticule*	NebuleusePart::Nebuleuse = NULL		;
NebuRayon*		NebuleusePart::RayonsExp = NULL		;
FonduNebPart*	NebuleusePart::FirstFondu = NULL	;
NebuTxt*		NebuleusePart::TexteAdd = NULL		;
UImage*			NebuleusePart::FonduBlanc			;
Udouble			NebuleusePart::LapTime = 0.0		;
Udouble			NebuleusePart::LastTime = 0.0		;




//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|  Class FonduNebPart	|
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

FonduNebPart::FonduNebPart( Ufloat duree )
{
	image = new UImage										;
	image->SetXSize( 1.0f )									;
	image->SetYSize( .5f )									;
	image->GetBitmap()->SetColor( 0.0f, 0.0f, 0.0f, 1.0f )	;
	image->SetTransp( MatTranspAlpha )						;
	image->GetMaterial()->SetShading( MatShadingGouraud )	;

	totaltime = timetogo = duree	;
}

//----------------------------------------------------------------------------------------------------------

FonduNebPart::~FonduNebPart()
{
	delete image	;
}

//----------------------------------------------------------------------------------------------------------
// renvoie TRUE tant kil y a qq chose a afficher

bool FonduNebPart::Affiche( Ufloat step )
{
	Ufloat alphamin = timetogo / totaltime	;
	Ufloat alphamax = alphamin * alphamin	;

	//------------------- affiche partie du haut
	image->SetPos( 0.5f, .25f )						;
	image->GetBitmap()->SetAlpha( 0, alphamin )	;
	image->GetBitmap()->SetAlpha( 1, alphamin )	;
	image->GetBitmap()->SetAlpha( 2, alphamax )	;
	image->GetBitmap()->SetAlpha( 3, alphamax )	;
	image->Affiche()								;

	//------------------- affiche partie du bas
	image->SetPos( 0.5f, .75f )						;
	image->GetBitmap()->SetAlpha( 0, alphamax )	;
	image->GetBitmap()->SetAlpha( 1, alphamax )	;
	image->GetBitmap()->SetAlpha( 2, alphamin )	;
	image->GetBitmap()->SetAlpha( 3, alphamin )	;
	image->Affiche()								;

	timetogo -= step						;
	if( timetogo < 0.0f )	return FALSE	;
	else					return TRUE		;
}

//----------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|  Class NebuleusePart	|
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------

void WINAPI	NebuleusePart::Initialise(UScheduler *sche, U32 user)
{

	//------------ Ini MondeU3D
	Monde = new MondeU3D									;
	Monde->LoadU3D( "Datas/meshes/nebuleuse/sc1.u3d" )	;
	Monde->LoadTXM( "Datas/meshes/nebuleuse/sc1.txm" )	;


	//----------- Ini Champ de particules
	Nebuleuse = new Nebuparticule( "Datas/meshes/nebuleuse/particul.jpg",  
								   "Datas/meshes/nebuleuse/Exparticul.jpg" )	;
	Monde->AddAFDCallback( "Nebuleuse", AffNebuleuse )							;
	RayonsExp = new NebuRayon( 2.5f )											;

	//----------- Ini les textes
	TexteAdd = new NebuTxt		;

	//----------- Ini premier fondu
	FirstFondu = new FonduNebPart( 8.f )	;

	//----------- Ini diverse images
	FonduBlanc = new UImage		;
	FonduBlanc->Disable()		;


	TextureManager::LoadAll()	;	// load toute les textures ( temporaire !!!!! )



	//----------- Ini les synchros
	IniSynchros()	;

	//----------- Ini le scheduler interne
	Scheduler = new UScheduler					;
	Scheduler->AddTask( "Fondu", DoFirstFond )	;


	//----------- lance une autre tache !
	sche->KillActuaTask()						;	// s'arrete soit meme !
	sche->AddTask( "NebMainLoop", MainLoop )	;	// lance la main loop


	//----------- Ini internal timer 
	LastTime = ClockU3D::GetTime()	;

}

//----------------------------------------------------------------------------------------------------------

void WINAPI	NebuleusePart::Release(UScheduler *sche, U32 user)
{

	//------------ Kill le scheduler
	DeleteBlinde( Scheduler )	;

	//------------ Kill les images
	DeleteBlinde( FonduBlanc )	;

	//------------ Kill les textes
	DeleteBlinde( TexteAdd )	;

	//------------ Kill le fondu
	DeleteBlinde( FirstFondu )	;

	//------------ Kill MondeU3D
	DeleteBlinde( RayonsExp )	;
	DeleteBlinde( Nebuleuse )	;
	DeleteBlinde( Monde )		;

	//------------ lance les éventuelles taches suivantes
	if( sche )
	{
		sche->KillActuaTask()										;
//		PostMessage( hWndMain, WM_CLOSE, 0, 0 )						;
		sche->AddTask( "NeurInitialise", NeuronePart::Initialise )	;
	}

}

//----------------------------------------------------------------------------------------------------------

void NebuleusePart::IniSynchros()
{
	SynchroPhase* phase	;

	phase = AllSynchro->AddPhase( "NebuHC", 0.0f, 32.f )	;
	phase->AddTick( 0, 27.0f, SynchrosMess )				;	// lancement des rayons
	phase->AddTick( 1, 29.7f, SynchrosMess )				;	// lancement du fondu blanc

	phase->AddTick( 2, 28.0f, SynchrosMess )				;	// lancement de l'explosion de la nebuleuse

	phase->AddTick( 3, 7.9f, SynchrosMess )					;	// lancement de forsaken1
	phase->AddTick( 4, 9.26f, SynchrosMess )				;	// lancement de by1
	phase->AddTick( 5, 9.7f, SynchrosMess )					;	// lancement de destiny
	phase->AddTick( 6, 13.558f, SynchrosMess )				;	// lancement de forsaken2
	phase->AddTick( 7, 15.0f, SynchrosMess )				;	// lancement de by
	phase->AddTick( 8, 16.0f, SynchrosMess )				;	// lancement de my
	phase->AddTick( 9, 16.446f, SynchrosMess )				;	// lancement de own
	phase->AddTick( 10, 17.7f, SynchrosMess )				;	// lancement de mind
	phase->AddTick( 11, 19.2f, SynchrosMess )				;	// lancement de I
	phase->AddTick( 12, 19.7f, SynchrosMess )				;	// lancement de must
	phase->AddTick( 13, 20.3f, SynchrosMess )				;	// lancement de remove
	phase->AddTick( 14, 21.6f, SynchrosMess )				;	// lancement de my2
	phase->AddTick( 15, 22.0f, SynchrosMess )				;	// lancement de skin
	phase->AddTick( 16, 25.0f, SynchrosMess )				;	// lancement de to
	phase->AddTick( 17, 25.25f, SynchrosMess )				;	// lancement de see
	phase->AddTick( 18, 26.0f, SynchrosMess )				;	// lancement de belief
	phase->AddTick( 19, 27.0f, SynchrosMess )				;	// lancement de in
	phase->AddTick( 20, 27.26f, SynchrosMess )				;	// lancement de your
	phase->AddTick( 21, 27.8f, SynchrosMess )				;	// lancement de eyes

	phase->AddTick( 22, 29.0f, SynchrosMess )	;	// lancement de la derniere transition
}

//----------------------------------------------------------------------------------------------------------

void WINAPI NebuleusePart::SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)
{
	switch( syncNum )
	{
		//----------- on rajoute l'affichage des rayons au monde 3D
		case 0:
			Monde->AddAFDCallback( "Rayon", AffRayons )	;	
			break;


		//----------- on balance le fondu au blanc
		case 1:
			FonduBlanc->Enable()							;
			FonduBlanc->SetSize( 1.0f )						;
			FonduBlanc->SetColor( 1.0f, 1.0f, 1.0f, 0.0f )	;
			FonduBlanc->SetTransp( MatTranspAlpha )			;
			FonduBlanc->InterpoAlpha( 0.0f, 1.0f, 1.0f )	;
			break;

		//---------- balance l'explosion de la nebuleuse
		case 2:
			Nebuleuse->Explode( 3.0f )	;
			break;

		//--------- lancement des textes
		case 3:
			TexteAdd->OnForsaken1()	;
			break;
		case 4:
			TexteAdd->OnBy1()		;
			break;
		case 5:
			TexteAdd->OnDestiny()	;
			break;
		case 6:
			TexteAdd->OnForsaken2()	;
			break;
		case 7:
			TexteAdd->OnBy2()	;
			break;
		case 8:
			TexteAdd->OnMy1()	;
			break;
		case 9:
			TexteAdd->OnOwn()	;
			break;
		case 10:
			TexteAdd->OnMind()	;
			break;
		case 11:
			TexteAdd->OnI()	;
			break;
		case 12:
			TexteAdd->OnMust()	;
			break;
		case 13:
			TexteAdd->OnRemove()	;
			break;
		case 14:
			TexteAdd->OnMy2()	;
			break;
		case 15:
			TexteAdd->OnSkin()	;
			break;
		case 16:
			TexteAdd->OnTo()	;
			break;
		case 17:
			TexteAdd->OnSee()	;
			break;
		case 18:
			TexteAdd->OnBelief()	;
			break;
		case 19:
			TexteAdd->OnIn()	;
			break;
		case 20:
			TexteAdd->OnYour()	;
			break;
		case 21:
			TexteAdd->OnEyes()	;
			break;

		//--------- schedule la derniere transition
/*		case 22:
			Scheduler->AddTask( "Ltransi", Transi1Part::MainLoop )	;
			break;*/
	}
}

//----------------------------------------------------------------------------------------------------------

void WINAPI NebuleusePart::DoFirstFond(UScheduler *sche, U32 user)
{

	if( !FirstFondu->Affiche( LapTime ) )
		sche->KillActuaTask()	;	// on a finit le fondu on stop donc

}

//----------------------------------------------------------------------------------------------------------

void WINAPI NebuleusePart::AffNebuleuse(Ufloat step, U32 user)
{

	Nebuleuse->UpdateFrame( step )	;

}

//----------------------------------------------------------------------------------------------------------

void WINAPI NebuleusePart::AffRayons(Ufloat step, U32 user)
{

	RayonsExp->UpdateFrame( step )	;

}

//----------------------------------------------------------------------------------------------------------

void WINAPI	NebuleusePart::MainLoop(UScheduler *sche, U32 user)
{
	//--------------- Calcul le LapTime 
	LapTime = ClockU3D::GetTime() - LastTime	;
	LastTime = ClockU3D::GetTime()				;

	ClearOpenGLBuffers()	;

	Monde->UpdateFrame( (Ufloat)(FRAMEPERSECOND*LapTime) )	;
	TexteAdd->Affiche( (Ufloat)LapTime )					;
	FonduBlanc->Affiche( (Ufloat)LapTime )					;
	Scheduler->Run()										;

	FlipOpenGL()	;

	//------------------- condition de sortie
	if(  (ClockU3D::GetTime() - LaunchTime) >= TIME_SORTIE )
	{
		sche->KillActuaTask()					;	// s'arrete soit meme !
		sche->AddTask( "NebRelease", Release )	;	// lance le release
	}
}

//----------------------------------------------------------------------------------------------------------
