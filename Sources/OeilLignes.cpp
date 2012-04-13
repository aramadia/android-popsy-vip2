	   				 /*-------------------------------------------------------------------------+
					  |								OeilLignes.cpp								|
					  |																			|
					  |	C'est une classe ki va afficher des elements "designs" par dessus l'oeil|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#include "stdafx.h"


#define NB_CTRLPTS	12	// nombre de controls points par spline


//-----------------------------------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|   Class OeilLigne		|
//										+-----------------------+
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------------------------------------------------------------------
OeilLigne::OeilLigne()
{
	XLigne = NULL	;
	YLigne = NULL	;

	AddXLigne( 0.0f, GetUniXpos( 302.f ) , GetUniYpos( 409.f ) )	;
	AddXLigne( 0.0f, GetUniXpos( 560.f ) , GetUniYpos( 415.f ) )	;
	AddXLigne( 0.0f, GetUniXpos( 797.f ) , GetUniYpos( 422.f ) )	;
	AddXLigne( 0.0f, GetUniXpos( 830.f ) , GetUniYpos( 435.f ) )	;

	AddYLigne( 0.0f, GetUniYpos( 545.f ) , GetUniXpos( 540.f ) )				;
	AddYLigne( GetUniYpos( 10.0f ), GetUniYpos( 475.f ) , GetUniXpos( 545.f ) )	;
	AddYLigne( GetUniYpos( 13.0f ), GetUniYpos( 202.f ) , GetUniXpos( 550.f ) )	;
	AddYLigne( GetUniYpos( 2.0f ), GetUniYpos( 455.f ) , GetUniXpos( 560.f ) )	;
	AddYLigne( GetUniYpos( 0.0f ), GetUniYpos( 415.f ) , GetUniXpos( 575.f ) )	;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
OeilLigne::~OeilLigne()
{
	OeilLigneList* tmp;	

	while( XLigne )
	{
		tmp = XLigne->suiv	;
		delete XLigne		;
		XLigne = tmp		;
	}

	while( YLigne )
	{
		tmp = YLigne->suiv	;
		delete YLigne		;
		YLigne = tmp		;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilLigne::AddXLigne( Ufloat xdebut, Ufloat xdelta, Ufloat y )
{
	OeilLigneList* tmp;	

	tmp = new OeilLigneList											;
	tmp->spline = new BSplinaeAff( NB_CTRLPTS+1, NB_CTRLPTS*10 )	;
	tmp->posDest = new Ufloat[ 2*(NB_CTRLPTS+1) ]					;
	tmp->pos = new Ufloat[ 2*(NB_CTRLPTS+1) ]						;
	tmp->spline->SetSpline2d( TRUE )								;
	tmp->spline->SetEndColor( 0.81f, 0.78f, 0.37f )					;
	tmp->spline->SetStartColor( 0.81f, 0.58f, 0.17f )				;

	tmp->spline->SetKeyPos( 0, xdebut, y )	;
	tmp->posDest[0] = xdebut				;
	tmp->posDest[1] = y						;
	tmp->pos[0] = xdebut					;
	tmp->pos[1] = y							;

	for( U32 a=0; a<NB_CTRLPTS; a++ )
	{
		Ufloat xpos = xdebut + xdelta*(Ufloat)a/NB_CTRLPTS	;
		tmp->spline->SetKeyPos( a+1, xpos, y )					;
		tmp->posDest[ (a+1)*2 + 0 ] = xpos						;
		tmp->posDest[ (a+1)*2 + 1 ] = y							;
		tmp->pos[ (a+1)*2 + 0 ] = xpos							;
		tmp->pos[ (a+1)*2 + 1 ] = y								;
	}

	tmp->suiv = XLigne	;
	XLigne = tmp		;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilLigne::AddYLigne( Ufloat ydebut, Ufloat ydelta, Ufloat x )
{
	OeilLigneList* tmp;	

	tmp = new OeilLigneList											;
	tmp->spline = new BSplinaeAff( NB_CTRLPTS+1, NB_CTRLPTS*10 )	;
	tmp->posDest = new Ufloat[ 2*(NB_CTRLPTS+1) ]					;
	tmp->pos = new Ufloat[ 2*(NB_CTRLPTS+1) ]						;
	tmp->spline->SetSpline2d( TRUE )								;
	tmp->spline->SetEndColor( 0.81f, 0.78f, 0.37f )					;
	tmp->spline->SetStartColor( 0.81f, 0.58f, 0.17f )				;

	tmp->spline->SetKeyPos( 0, x, ydebut )	;
	tmp->posDest[0] = x						;
	tmp->posDest[1] = ydebut				;
	tmp->pos[0] = x							;
	tmp->pos[1] = ydebut					;

	for( U32 a=0; a<NB_CTRLPTS; a++ )
	{
		Ufloat ypos = ydebut + ydelta*(Ufloat)a/NB_CTRLPTS	;
		tmp->spline->SetKeyPos( a+1, x, ypos )					;
		tmp->posDest[ (a+1)*2 + 0 ] = x							;
		tmp->posDest[ (a+1)*2 + 1 ] = ypos						;
		tmp->pos[ (a+1)*2 + 0 ] = x								;
		tmp->pos[ (a+1)*2 + 1 ] = ypos							;
	}

	tmp->suiv = YLigne	;
	YLigne = tmp		;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilLigne::GereLigne( OeilLigneList* ligne )
{
	while( ligne )
	{
		for( U32 a=0; a<NB_CTRLPTS+1; a++)
		{
			ligne->pos[a*2+0] += ( ligne->posDest[a*2+0] - ligne->pos[a*2+0] ) / 16.f	;
			ligne->pos[a*2+1] += ( ligne->posDest[a*2+1] - ligne->pos[a*2+1] ) / 16.f	;

			ligne->spline->SetKeyPos( a, ligne->pos[a*2], ligne->pos[a*2+1] )	;
		}

		ligne->spline->Affiche()	;

		ligne = ligne->suiv	;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilLigne::Deform( Ufloat intens )	
{
	OeilLigneList* tmp;

	tmp = XLigne	;
	while( tmp )
	{
		for( U32 a=0; a<NB_CTRLPTS+1; a++)
		{
			Ufloat fact = tmp->pos[a*2] / .5f								;
			if( fact > 1.0f ) fact = 1.0f									;
			Ufloat fact1 = (Ufloat)a / NB_CTRLPTS							;
			fact *= intens													;
			Ufloat x = fact * .3f * Random1() *Cos( fact1*Pi()*4.f ) + .5f	;
			Ufloat y = fact * .3f * Random1() *Sin( fact1*Pi()*4.f ) + .5f	;

			tmp->pos[a*2] = tmp->pos[a*2]*(1.0f-fact) + fact*x		;
			tmp->pos[a*2+1] = tmp->pos[a*2+1]*(1.0f-fact) + fact*y	;
		}

		tmp = tmp->suiv	;
	}

	tmp = YLigne	;
	while( tmp )
	{
		for( U32 a=0; a<NB_CTRLPTS+1; a++)
		{
			Ufloat fact = tmp->pos[a*2+1] / .5f							;
			if( fact > 1.0f ) fact = 1.0f									;
			Ufloat fact1 = (Ufloat)a / NB_CTRLPTS							;
			fact *= intens													;
			Ufloat x = fact * .3f * Random1() *Cos( fact1*Pi()*4.f ) + .5f	;
			Ufloat y = fact * .3f * Random1() *Sin( fact1*Pi()*4.f ) + .5f	;

			tmp->pos[a*2] = tmp->pos[a*2]*(1.0f-fact) + fact*x		;
			tmp->pos[a*2+1] = tmp->pos[a*2+1]*(1.0f-fact) + fact*y	;

/*			tmp->pos[a*2+1] += intens * .3f * Sin( a*.1f )											;
			tmp->pos[a*2] += intens * .3f * ( (Ufloat)a/NB_CTRLPTS ) * Cos( a*.3f + Random1() )	;*/
		}

		tmp = tmp->suiv	;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void OeilLigne::Affiche( Ufloat laptime )
{
	GereLigne( XLigne )	;
	GereLigne( YLigne )	;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
