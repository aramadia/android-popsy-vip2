    				 /*---------------------------------------------------------------------+
					  |							ImageBlur.cpp								|
					  |																		|
					  | Affiche plusieurs fois la meme image mais avec decalage pour faire	|
					  |					un giga effet de blur ki tue !						|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|    Class UImageBlur	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
UImageBlur::UImageBlur( U32 nbB, LPCSTR nomTex, U32 flagopenTex, Ufloat natWidth, Ufloat natHeight  )
{
	nbBlur = nbB				;
	tab = new UImage*[nbBlur]	;

	tab[0] = new UImage( nomTex, flagopenTex, natWidth, natHeight )	;
	tab[0]->PipeSet( FALSE )											;

	for( U32 a=0; a<nbBlur-1; a++ )
	{
		tab[a+1] = new UImage	;
		(*tab[a+1]) = (*tab[0])	;
	}

	factAlpha = 1.0f		;
	facttimeAlpha = 2.0f	;
	facttimeXPos = 2.0f	;
	facttimeYPos = 2.0f	;
	facttimeXZoom = 2.0f	;
	facttimeYZoom = 2.0f	;
	facttimeAngle = 2.0f	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
UImageBlur::~UImageBlur()
{
	for( U32 a=0; a<nbBlur; a++ )	delete tab[a]	;

	delete [] tab	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::Disable()
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->Disable()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::Enable()
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->Enable()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::ConsProRota( bool b)
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->ConsProRota( b )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetPos( Ufloat x, Ufloat y )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetPos( x, y )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetUpLeftPos( Ufloat x, Ufloat y )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetUpLeftPos( x, y )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetZoom( Ufloat z )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetZoom( z )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetXZoom( Ufloat z )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetXZoom( z )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetYZoom( Ufloat z )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetYZoom( z )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetSize( Ufloat size )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetSize( size )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetXSize( Ufloat size )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetXSize( size )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetYSize( Ufloat size )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetYSize( size )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetTransp( U32 tr )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetTransp( tr )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetAlpha( Ufloat alph )
{
	Ufloat alpha = factAlpha*alph / nbBlur	;
	if( alpha > 1.0f ) alpha = 1.0f			;

	for( U32 a=0; a<nbBlur; a++ )
	{
		tab[a]->SetAlpha( alpha )	;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetAngle( Ufloat angl )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetAngle( angl )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetColor( Ufloat r, Ufloat v, Ufloat b, Ufloat alph )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetColor( r, v, b, alph )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetCoordRV( Ufloat u1, Ufloat u2, Ufloat v1, Ufloat v2 )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetCoordRV( u1, u2, v1, v2 )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::SetCoord( Ufloat u1, Ufloat u2, Ufloat v1, Ufloat v2 )
{
	for( U32 a=0; a<nbBlur; a++ )	tab[a]->SetCoord( u1, u2, v1, v2 )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::InterpoAlpha( Ufloat debut, Ufloat fin, Ufloat time )
{
	Ufloat alphadebut = factAlpha*debut / nbBlur		;
	Ufloat alphafin = factAlpha*fin / nbBlur			;
	Ufloat ftime = (facttimeAlpha-1.0f )*time / nbBlur	;

	for( U32 a=0; a<nbBlur; a++ )	tab[a]->InterpoAlpha( alphadebut, alphafin, time + a*ftime )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::InterpoXPos( Ufloat debut, Ufloat fin, Ufloat time )
{
	Ufloat ftime = (facttimeXPos-1.0f )*time / nbBlur		;

	for( U32 a=0; a<nbBlur; a++ )	tab[a]->InterpoXPos( debut, fin, time + a*ftime )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::InterpoYPos( Ufloat debut, Ufloat fin, Ufloat time )
{
	Ufloat ftime = (facttimeYPos-1.0f )*time / nbBlur		;

	for( U32 a=0; a<nbBlur; a++ )	tab[a]->InterpoYPos( debut, fin, time + a*ftime )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::InterpoXZoom( Ufloat debut, Ufloat fin, Ufloat time )
{
	Ufloat ftime = (facttimeXZoom-1.0f )*time / nbBlur		;

	for( U32 a=0; a<nbBlur; a++ )	tab[a]->InterpoXZoom( debut, fin, time + a*ftime )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::InterpoYZoom( Ufloat debut, Ufloat fin, Ufloat time )
{
	Ufloat ftime = (facttimeYZoom-1.0f )*time / nbBlur		;

	for( U32 a=0; a<nbBlur; a++ )	tab[a]->InterpoYZoom( debut, fin, time + a*ftime )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::InterpoZoom( Ufloat debut, Ufloat fin, Ufloat time )
{
	Ufloat ftime = ((facttimeXZoom+facttimeYZoom)*.5f-1.0f )*time / nbBlur		;

	for( U32 a=0; a<nbBlur; a++ )	tab[a]->InterpoZoom( debut, fin, time + a*ftime )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::InterpoAngle( Ufloat debut, Ufloat fin, Ufloat time )
{
	Ufloat ftime = (facttimeAngle-1.0f )*time / nbBlur		;

	for( U32 a=0; a<nbBlur; a++ )	tab[a]->InterpoZoom( debut, fin, time + a*ftime )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
bool UImageBlur::IsInterpoAlpha()
{
	for( U32 a=0; a<nbBlur; a++ )
	{
		if( tab[a]->IsInterpoAlpha() )
			return TRUE	;
	}
	return FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
bool UImageBlur::IsInterpoXPos()
{
	for( U32 a=0; a<nbBlur; a++ )
	{
		if( tab[a]->IsInterpoXPos() )
			return TRUE	;
	}
	return FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
bool UImageBlur::IsInterpoYPos()
{
	for( U32 a=0; a<nbBlur; a++ )
	{
		if( tab[a]->IsInterpoYPos() )
			return TRUE	;
	}
	return FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
bool UImageBlur::IsInterpoAngle()
{
	for( U32 a=0; a<nbBlur; a++ )
	{
		if( tab[a]->IsInterpoAngle() )
			return TRUE	;
	}
	return FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImageBlur::Affiche( Ufloat laptime )	
{
	U3D3Pipeline->Begin( tab[0]->GetMaterial() )	;

		for( U32 a=0; a<nbBlur; a++) tab[a]->Affiche( laptime )	;

	U3D3Pipeline->End( tab[0]->GetMaterial() )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
