	   				 /*-------------------------------------------------------------------------+
					  |							FinalSpline.cpp									|
					  |																			|
					  |		je vasi tracer des Bspline les long des lettres de VIP2 na ! 		|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#include "stdafx.h"

#define NB_SPLINE_PARLETTRE	35

#define NB_CTRL_POINT 5


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class FinalOneSpline	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
FinalOneSpline::FinalOneSpline( ObjetU3D* objet )
{
	obj = objet ;

	IndexDepart	= rand()%obj->nbcoords	;
	IncrementIndex = rand()%7 + 1		;

	TimeReste = Random1()*.2f + .0f	;
	ActuaTime = TimeReste			;

	spline = new BSplinaeAff( NB_CTRL_POINT, IncrementIndex*5 )	;
	spline->SetStartColor( .2f, .1f, .1f )						;
	spline->SetEndColor( .3f, .3f, .7f )						;

	AssignKey()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
FinalOneSpline::~FinalOneSpline()
{
	delete spline	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void FinalOneSpline::AssignKey()
{
	U32 index	;

	IndexDepart += IncrementIndex			;
	IndexDepart = IndexDepart%obj->nbcoords	;

	index = IndexDepart	;
	for( U32 a=0; a<NB_CTRL_POINT; a++ )
	{
		spline->SetKeyPos( a, obj->Ctab[index].trans )	;

		index += IncrementIndex		;
		index = index%obj->nbcoords	;
	}

	ActuaTime = TimeReste	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void FinalOneSpline::SeAlpha( Ufloat a )
{
	spline->SetEndAlpha( a )	;
	spline->SetStartAlpha( a )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void FinalOneSpline::UpdateFrame( Ufloat laptime )
{
	ActuaTime -= laptime				;
	if( ActuaTime <= 0.0f )	AssignKey()	;

	spline->Affiche()	;
}
//----------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class FinalSpline	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
FinalSpline::FinalSpline( MondeU3D* monde )
{
	FinalOneSpline* tmpsp	;

	AllSpline = NULL	;

	for( monde->IniObjIterator(); monde->GetObjFromIterator(); monde->IncObjIterator() )
	{
		for( U32 a=0; a<NB_SPLINE_PARLETTRE; a++ )
		{
			tmpsp = new FinalOneSpline( monde->GetObjFromIterator() )	;
			tmpsp->suiv = AllSpline										;
			AllSpline = tmpsp											;
		}
	}

	TAppear	= 1.0f		;
	actuatime = 0.0f	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
FinalSpline::~FinalSpline()
{
	FinalOneSpline* tmpsp	;

	while( AllSpline )
	{
		tmpsp = AllSpline->suiv	;
		delete AllSpline		;
		AllSpline = tmpsp		;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void FinalSpline::Appear( Ufloat time )
{
	TAppear	= time		;
	actuatime = time	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void FinalSpline::UpdateFrame( Ufloat laptime )
{
	Ufloat			alpha	;
	FinalOneSpline* tmpsp	;

	actuatime -= laptime					;
	if( actuatime<=0.0f ) actuatime = 0.0f	;

	alpha = 1.0f - actuatime / TAppear	;

	tmpsp	= AllSpline	;
	while( tmpsp )
	{
		tmpsp->SeAlpha( alpha )			;
		tmpsp->UpdateFrame( laptime )	;
		tmpsp	= tmpsp->suiv			;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------

