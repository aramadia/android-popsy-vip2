		   			 /*---------------------------------------------------------------------+
					  |				         NeuroBack.cpp									|
					  |																		|
					  |			affichage d'un léger fond  pour la scene des neuronnes		|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/


#include "stdafx.h"



#define DBAR_SPEED		.5f
#define DTEXTE_SPEED	1.5f

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|  Class NeuroBack	|
//										+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
NeuroBackground::NeuroBackground()
{
	DestroyBar = new UImage( "Datas/2d/barre_destroy.jpg", BILINEAR_TEXTURE|TILE_TEXTURE, 64.f, 512.f )	;
	DestroyBar->SetTransp( MatTranspAddAlpha )																;
	DestroyBar->SetCoord( 0.0f, 1.0f, 0.0f, 2.0f )															;
	DestroyBar->SetUpLeftPos( GetUniXpos( 48.0f ), GetUniYpos( 0.0f ) )										;


	CleanTexte = new UImage( "Datas/2d/Texte_clean.jpg", BILINEAR_TEXTURE, 64.f, 512.f )	;
	CleanTexte->SetTransp( MatTranspAddAlpha )												;
	CleanTexte->SetUpLeftPos( GetUniXpos( 500.0f ), -CleanTexte->GetYSize()*.5f )			;
	CleanTexte->InterpoYPos( CleanTexte->GetYpos(), 1.f+CleanTexte->GetYSize(), 2.0f)		;


	DestroyTexte = new UImage( "Datas/2d/Texte_dstr.jpg", BILINEAR_TEXTURE|TILE_TEXTURE, 64.f, 512.f )	;
	DestroyTexte->SetTransp( MatTranspAddAlpha )															;
	DestroyTexte->SetYSize( 1.f )																			;
	DestroyTexte->SetUpLeftPos( GetUniXpos( 136.0f ), GetUniYpos( 0.0f ) )									;


	Nenfants = new UImage( "Datas/2d/nenfants.jpg", BILINEAR_TEXTURE )	;
	Nenfants->SetTransp( MatTranspAddAlpha )								;
	Nenfants->SetUpLeftPos( GetUniXpos( 260.0f ), 1.0f )					;
	Nenfants->InterpoYPos( Nenfants->GetYpos(), -Nenfants->GetYpos(), 3.0f)	;

	totaltime = 0.0f	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
NeuroBackground::~NeuroBackground()
{
	delete DestroyBar	;
	delete CleanTexte	;
	delete DestroyTexte	;
	delete Nenfants		;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void NeuroBackground::Appear( Ufloat time )
{
	DestroyBar->InterpoAlpha( 0.0f, 1.0f, time )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void NeuroBackground::Disappear( Ufloat time )
{
	DestroyBar->InterpoAlpha( 1.0f, 0.0f, time )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void NeuroBackground::UpdateFrame( Ufloat laptime )	
{
	totaltime += laptime	;

	DestroyBar->Affiche( laptime )													;
	DestroyBar->GetMaterial()->SetTex1Pos( 0.0f, totaltime*DBAR_SPEED, 1.0f, 2.0f )	;

	DestroyTexte->Affiche( laptime )													;
	DestroyTexte->SetAlpha( fabs(sin( totaltime*3.f ))  )								;
	DestroyTexte->GetMaterial()->SetTex1Pos( 0.0f, totaltime*DTEXTE_SPEED, 1.0f, 1.0f )	;

	Nenfants->Affiche( laptime )	;
	if( !Nenfants->IsInterpoYPos() )
	{
		Nenfants->SetUpLeftPos( GetUniXpos( 260.0f ), 1.0f )					;
		Nenfants->InterpoYPos( Nenfants->GetYpos(), -Nenfants->GetYpos(), 4.0f)	;	
	}

	CleanTexte->Affiche( laptime )	;
	if( !CleanTexte->IsInterpoYPos() )
	{
		DestroyTexte->SetAlpha( .2f + .8f*fabs(cos( totaltime*5.f ))  )													;
		CleanTexte->SetUpLeftPos( GetUniXpos( 400.0f )+GetUniXpos( 200.0f )*Random1(), -CleanTexte->GetYSize()*.5f )	;
		CleanTexte->InterpoYPos( CleanTexte->GetYpos(), 1.f+CleanTexte->GetYSize(), 2.0f)								;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
