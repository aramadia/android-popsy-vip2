	   				 /*-------------------------------------------------------------------------+
					  |							FinalTexte.cpp									|
					  |																			|
					  |		affichage des quelques derniers graphs de textes de la demo ho ho	|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/


#include "stdafx.h"


#define TIME_SCROLL 15.f

#define TIME_FONDU 5.2f

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|  Class FinalTexte	|
//										+-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
FinalTexte::FinalTexte()
{
	Url	 = new UImage( "Datas/meshes/Final/url.jpg", ALPHA_TEXTURE|BILINEAR_TEXTURE )	;
	Url->OpenMask( "Datas/meshes/Final/url.raw" )										;
	Url->SetTransp( MatTranspAlpha )														;
	Url->SetUpLeftPos( GetUniXpos(128.f), GetUniYpos(368.f) )								;
	Url->Disable()																			;

	UrlAdd = new UImage( "Datas/meshes/Final/urlflash.jpg" )	;
	UrlAdd->SetTransp( MatTranspAddAlpha )						;
	UrlAdd->SetPos( Url->GetXpos(), Url->GetYpos() )			;
	UrlAdd->Disable()											;

	FScroll	= new UImage( "Datas/meshes/Final/finalscroll.jpg", BILINEAR_TEXTURE, 256.f, 512.f )		;
	FScroll->SetTransp( MatTranspAddAlpha )																;
	FScroll->SetPos( 1.0f- FScroll->GetXpos()*.5f, .5f )												;
	FScroll->Disable()																					;

	Fondu = new UImage					;
	Fondu->SetSize( 1.0f )				;
	Fondu->SetColor( 0.0f, 0.0f, 0.0f )	;
	Fondu->SetTransp( MatTranspAlpha )	;
	Fondu->Disable()					;
}
//----------------------------------------------------------------------------------------------------------------------------------------
FinalTexte::~FinalTexte()
{
	delete Url		;
	delete UrlAdd	;
	delete FScroll	;
	delete Fondu	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void FinalTexte::GoUrl()
{
	Url->InterpoAlpha( 0.0f, 1.0f, 1.0f )	;
	Url->Enable()							;
	UrlAdd->InterpoAlpha( 1.0f, 0.0f, 1.0f) ;
	UrlAdd->Enable()						;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void FinalTexte::GoScroll()
{
	FScroll->InterpoYPos( FScroll->GetYSize()*.5f +1.f, -FScroll->GetYSize()*.5f, TIME_SCROLL )	;
	FScroll->Enable()																			;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void FinalTexte::GoFondu()
{
	Fondu->InterpoAlpha( 0.0f, 1.0f, TIME_FONDU	)	;
	Fondu->Enable()									;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void FinalTexte::UpdateFrame( Ufloat laptime )
{
	Url->Affiche( laptime )	;

	UrlAdd->Affiche( laptime )	;
	if( !UrlAdd->IsInterpoAlpha() )
		UrlAdd->Disable()	;

	FScroll->Affiche( laptime )	;

	Fondu->Affiche( laptime )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
