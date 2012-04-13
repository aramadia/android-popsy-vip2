					 /*-----------------------------------------------------------------+
					  |							Lettres.cpp								|
					  |																	|
					  |	 va afficher PoPsY TeAm VIP 2 en plusieirus fois avec pleins	|
					  |		d'effets distroy a souhaits ( genre pub Fila )				|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/


#include "stdafx.h"

#define DUREE .45f

#define TIME_FONDU	.5f

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|	Variables Locales	|
//									+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

UScheduler*		LettresPart::Scheduler = NULL			;
MondeU3D*		LettresPart::Lettres[NB_LETTRES_LPART]	;
U32				LettresPart::ActuaLettre = 0			;
MondeU3D*		LettresPart::FScene	= NULL				;
bool			LettresPart::ActuaFScene = FALSE		;
UImage*			LettresPart::Fondu = NULL				;
UImage*			LettresPart::Url = NULL					;
LettresBack*	LettresPart::Back = NULL				;
BPeloche*		LettresPart::Bruit = NULL				;
XGrezille*		LettresPart::grezille = NULL			;
TextureU3D*		LettresPart::TexGeneral = NULL			;
Ufloat			LettresPart::DureeScene	= DUREE			;
bool			LettresPart::FirstTime = TRUE			;
Udouble			LettresPart::LapTime = 0.0f				;
Udouble			LettresPart::LastTime = 0.0f			;




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|  Class GhostPart	|
//										+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

void LettresPart::PreLoad()
{
	Lettres[0] = new MondeU3D								;
	Lettres[0]->LoadU3D( "Datas/meshes/Lettres/V.u3d" )	;

	Lettres[1] = new MondeU3D								;
	Lettres[1]->LoadU3D( "Datas/meshes/Lettres/I.u3d" )	;

	Lettres[2] = new MondeU3D								;
	Lettres[2]->LoadU3D( "Datas/meshes/Lettres/P.u3d" )	;

	Lettres[3] = new MondeU3D								;
	Lettres[3]->LoadU3D( "Datas/meshes/Lettres/2.u3d" )	;

	Lettres[4] = new MondeU3D								;
	Lettres[4]->LoadU3D( "Datas/meshes/Lettres/P2.u3d" )	;

	Lettres[5] = new MondeU3D								;
	Lettres[5]->LoadU3D( "Datas/meshes/Lettres/O.u3d" )	;

	Lettres[6] = new MondeU3D								;
	Lettres[6]->LoadU3D( "Datas/meshes/Lettres/P1.u3d" )	;

	Lettres[7] = new MondeU3D								;
	Lettres[7]->LoadU3D( "Datas/meshes/Lettres/S.u3d" )	;

	Lettres[8] = new MondeU3D								;
	Lettres[8]->LoadU3D( "Datas/meshes/Lettres/Y.u3d" )	;

	Lettres[9] = new MondeU3D								;
	Lettres[9]->LoadU3D( "Datas/meshes/Lettres/T.u3d" )	;

	Lettres[10] = new MondeU3D								;
	Lettres[10]->LoadU3D( "Datas/meshes/Lettres/E.u3d" )	;

	Lettres[11] = new MondeU3D								;
	Lettres[11]->LoadU3D( "Datas/meshes/Lettres/A.u3d" )	;

	Lettres[12] = new MondeU3D								;
	Lettres[12]->LoadU3D( "Datas/meshes/Lettres/M.u3d" )	;

	FScene = new MondeU3D									;
	FScene->LoadU3D( "Datas/meshes/Lettres/All.u3d" )	;

	Back = new LettresBack			;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	LettresPart::Initialise(UScheduler *sche, U32 user)
{
	TexGeneral = TextureManager::OpenJpg( "Datas/meshes/Lettres/metal.jpg", ENGINE_TEXTURE )	;	// texture pour toutes les lettres
	TextureManager::AddUse( TexGeneral	)														;

	ActuaLettre = 0	;
	for( U32 a=0; a<NB_LETTRES_LPART; a++)
	{
		Lettres[a]->LoadTXM( "Datas/meshes/Lettres/vip.txm" )	;

		Lettres[a]->GetStates().SortPolyTr = FALSE		;

		Lettres[a]->SetAmbientColor( .2f, .2f, .5f )					;
		Lettres[a]->IniMatIterator()									;
//		Lettres[a]->GetMatFromIterator()->SetTexture1( TexGeneral )		;
		Lettres[a]->GetMatFromIterator()->SetTransp( MatTranspAlpha )	;
		Lettres[a]->GetMatFromIterator()->SetAlpha( 1.f/8.f )			;
		Lettres[a]->GetMatFromIterator()->Confirm()						;
	}

	FScene->LoadTXM( "Datas/meshes/Lettres/vip.txm" )		;
	FScene->GetStates().SortPolyTr = FALSE						;
	FScene->IniMatIterator()									;
	FScene->GetMatFromIterator()->SetTransp( MatTranspAlpha )	;
	FScene->GetMatFromIterator()->SetAlpha( 1.f/8.f )			;
	FScene->GetMatFromIterator()->Confirm()						;

	Fondu = new UImage					;
	Fondu->SetSize( 1.0f )				;
	Fondu->SetTransp( MatTranspAlpha )	;
	Fondu->SetColor( 0.0f, 0.0f, 0.0f )	;
	Fondu->Disable()					;

	Url = new UImage( "Datas/meshes/Lettres/url.jpg" )	;
	Url->SetPos( 0.0f, .5f )								;
	Url->SetTransp( MatTranspAddAlpha )						;
	Url->Disable()											;
//	Back = new LettresBack			;

	Bruit = new BPeloche			;
	grezille = new XGrezille( 250 )	;

	TextureManager::LoadAll()	;	// load toute les textures


	IniSynchros()	;

	//----------- Ini le scheduler interne
	Scheduler = new UScheduler	;

	//----------- Ini des flags divers et variés ( poil d'épervier )
	FirstTime = TRUE	;
	ActuaFScene = FALSE	;
	DureeScene = DUREE	;

	//----------- lance une autre tache !
	sche->KillActuaTask()							;	// s'arrete soit meme !
	sche->AddTask( "LettresMainLoop", MainLoop )	;	// lance la main loop

}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI	LettresPart::Release(UScheduler *sche, U32 user)
{
	//------------ Kill le scheduler
	DeleteBlinde( Scheduler )	;

	//------------ Kill les nimages
	DeleteBlinde( Fondu )		;
	DeleteBlinde( Url )			;
	DeleteBlinde( Bruit )		;
	DeleteBlinde( grezille )	;
	DeleteBlinde( Back )		;

	//------------ Kill MondeU3D
	for( U32 a=0; a<NB_LETTRES_LPART; a++)
		DeleteBlinde( Lettres[a] )		;

	DeleteBlinde( FScene )	;

	//------------ delaisse la texture
	TextureManager::SubUse( TexGeneral )	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void LettresPart::IniSynchros()
{
	SynchroPhase* phase	;

	phase = AllSynchro->AddPhase( "LettresHC", MULTI_TIME_BEGIN, MULTI_TIME_END )	;

#ifndef BMULTI_PART
	//----------------- !!!!! TMP !!!!!!!!!!!
	for( U32 a=0; a<20; a++ )
		phase->AddTick( 0, a*1.0f,  SynchrosMess )	;
#endif

	phase->AddTick( 1, 25.356f, SynchrosMess )	;	// lance le fondu
	phase->AddTick( 2, 24.f, SynchrosMess )		;	// lance l'url
}

//----------------------------------------------------------------------------------------------------------------------------------------

void WINAPI LettresPart::SynchrosMess(SynchroTick *sync, Ufloat relativeTime, U32 syncNum, U32 user)
{
	switch( syncNum )
	{
#ifndef BMULTI_PART
		//----------------- balance des viewports
		case 0:
			Back->Interpo( 1.0f, 0.0f, DureeScene)	;
			break;
#endif
		case 1:
			Fondu->Enable()									;
			Fondu->InterpoAlpha( 0.0f, 1.0f, TIME_FONDU )	;
			break;
		case 2:
			Url->Enable()														;
			Url->InterpoXPos( -Url->GetXSize()*.5f, Url->GetXSize()*.5f, 0.5f )	;
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------

void LettresPart::NextLettre()
{
	ActuaLettre++	;	ActuaLettre = ActuaLettre%NB_LETTRES_LPART	;
	FirstTime = TRUE												;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void LettresPart::GoFScene()		
{
	ActuaFScene = TRUE	;
	FirstTime = TRUE	;
	DureeScene = 1.5f	;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void LettresPart::AffBlur( MondeU3D* monde, Ufloat intens )
{
	Ufloat saveFov										;
	Ufloat camF = monde->GetActualCamera()->GetFov()	;

	saveFov = camF	;
	for( U32 a=0; a<8; a++,camF+=intens )
	{
		if( camF>120.f)	camF=120.f	;
		monde->GetActualCamera()->SetFov( camF )						;
		monde->UpdateFrame( (float)(FRAMEPERSECOND*2.f*LapTime/8.f) )	;
	}
	monde->GetActualCamera()->SetFov( saveFov )	;
}

//----------------------------------------------------------------------------------------------------------------------------------------
void WINAPI	LettresPart::MainLoop(UScheduler *sche, U32 user)
{
	if( FirstTime )
	{
		LastTime = ClockU3D::GetTime()			;
		FirstTime = FALSE						;
		Back->Interpo( 1.0f, 0.0f, DureeScene )	;
	}


	//--------------- Calcul le LapTime
	LapTime = ClockU3D::GetTime() - LastTime	;
	LastTime = ClockU3D::GetTime()				;

	ClearOpenGLBuffers()	;

	Back->UpdateFRame( LapTime )		;
	grezille->UpdateFrame( LapTime )	;

	if( !ActuaFScene )
	{
		AffBlur( Lettres[ActuaLettre], 4.0f*Back->GetAlpha() )	;

	#ifndef BLETTRES_PART
		if( Back->GetAlpha() <=0.0f )	// on a fini alors on passe a la tache suivante
		{
			MultiPart::LettreFinish()	;
		}
	#endif

	}
	else	//--------------------- scene final pour les lettres
	{
		AffBlur( FScene, 2.0f*Back->GetAlpha() )	;
		Url->Affiche( LapTime )						;
		Fondu->Affiche( LapTime )					;
	}

	Bruit->UpdateFrame( LapTime )	;
	
	FlipOpenGL()	;
}

//----------------------------------------------------------------------------------------------------------------------------------------
	
