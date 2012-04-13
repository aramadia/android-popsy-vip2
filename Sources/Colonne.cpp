	   				 /*-------------------------------------------------------------------------+
					  |								Colonne.cpp									|
					  |																			|
					  |	une des piece maitresse de la demo ou l'on voit une giga colonne	 	|
					  |							organik ki va bien !							|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#include "stdafx.h"

#define TIME_FONDU 0.5f

#define NB_CAMERA	3

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|	Variables Locales	|
//									+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*		ColonnePart::Scheduler = NULL	;
MondeU3D*		ColonnePart::Monde = NULL		;
ColonneAnim*	ColonnePart::BAnim = NULL		;
UImage*			ColonnePart::Fondu = NULL		;
U32				ColonnePart::CamPos = 0			;
bool			ColonnePart::FirstTime = TRUE	;
Udouble			ColonnePart::LapTime = 0.0f		;
Udouble			ColonnePart::LastTime = 0.0f	;



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class ColonnePart	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void ColonnePart::PreLoad()
{
	Monde = new MondeU3D									;
	Monde->LoadU3D( "Datas/meshes/Colonne/coldem.u3d" )	;
	Monde->ConstructAllObjStaticLighting()					;
	Monde->SetActualCamera( Monde->GetCamera("Camera03") )	;
	Monde->SetAllSpecular( FALSE )							;	// enleve le specular ki prend moult temps en plus dans cette scene !
	Monde->LoadTXM( "Datas/meshes/Colonne/colonne.txm" )	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	ColonnePart::Initialise(UScheduler *sche, U32 user)
{

	BAnim = new ColonneAnim( Monde )	;

	Fondu = new UImage					;
	Fondu->SetColor( 0.0f, 0.0f, 0.0f )	;
	Fondu->SetSize( 1.0f )				;
	Fondu->SetTransp( MatTranspAlpha )	;
	Fondu->Disable()					;
	

	TextureManager::LoadAll()	;	// load toute les textures

	CamPos = 0	;

	IniSynchros()	;

	//----------- Ini le scheduler interne
	Scheduler = new UScheduler	;

	//----------- Ini des flags divers et variés ( poil d'épervier )
	FirstTime = TRUE	;

	//----------- lance une autre tache !
	sche->KillActuaTask()							;	// s'arrete soit meme !
	sche->AddTask( "ColonneMainLoop", MainLoop )	;	// lance la main loop
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	ColonnePart::Release(UScheduler *sche, U32 user)
{
	//------------ Kill le scheduler
	DeleteBlinde( Scheduler )	;

	//------------ Kill MondeU3D
	DeleteBlinde( Monde )		;
	DeleteBlinde( BAnim )		;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void ColonnePart::IniSynchros()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	ColonnePart::SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------

void ColonnePart::NextCam()
{
	char camname[256];

	CamPos++	;	CamPos = CamPos%NB_CAMERA	;

	sprintf(camname, "Camera0%d", CamPos+1);
	Camera* cam = Monde->GetCamera(camname)	;
	Monde->SetActualCamera( cam )					;

	Fondu->Enable()									;
	Fondu->InterpoAlpha( 1.0f, 0.0f, TIME_FONDU )	;	// fait un fondu en ouverture a chak changement

	FirstTime = TRUE	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	ColonnePart::MainLoop(UScheduler *sche, U32 user)
{
	if( FirstTime )
	{
		LastTime = ClockU3D::GetTime()	;
		FirstTime = FALSE				;
	}


	//--------------- Calcul le LapTime
	LapTime = ClockU3D::GetTime() - LastTime	;
	LastTime = ClockU3D::GetTime()				;


	ClearOpenGLBuffers()	;

	BAnim->Anim( LastTime, LapTime )															;
	Monde->IniMatIterator()																		;
	Monde->GetMatFromIterator()->SetTex1Pos( 0.0f, 0.0f, 2.0f, 2.0f )							;
	Monde->GetMatFromIterator()->SetTex2Pos( .1f * sin( LastTime*1.91f ), 0.0f, 1.0f, 1.0f )	;
	Monde->UpdateFrame( (float)(FRAMEPERSECOND*LapTime) )										;

	Fondu->Affiche( LapTime )	;
	if( !Fondu->IsInterpoAlpha() )
		Fondu->Disable()	;

//	AffFps()	;

	FlipOpenGL()	;
}

//----------------------------------------------------------------------------------------------------------------------------------------
