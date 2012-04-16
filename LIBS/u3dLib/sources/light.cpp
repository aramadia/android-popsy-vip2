    				 /*-----------------------------------------------------+
					  |				         light.cpp						|
					  |														|
					  |	 ralalal la gestion des lumières dans un moteur 3D  |
					  |	   est qq chose d'important car ca donne tout de 	|
					  |				suite bocoup mieux avec que sans		|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
//								+---------------------+
//								|    Les Fonctions    |
//								+---------------------+
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
LightU3D::LightU3D()
{
	Nom = "Default"			;
	Type = TYPE_LIGHT_OMNI	;

	vec3_set( Pos, 0.0f, 0.0f, 0.0f )	;

	vec3_set( RVB, .8f, .8f, .8f )	;
	Intensity = 1.0f				;

	Attenuation = FALSE	;
	Inner = 100.f		;
	Outer = 1000.f		;

	vec3_set( Targ, 0.0f, 0.0f, 100.f )	;
	Hotspot = .7853981634				;	// Hotspot a 45 degré par defaut
	Falloff = 1.570796327				;	// Falloff a 90 degré par defaut

	Spos = NULL		;
	Starg = NULL	;
	Scolor = NULL	;

	Normalize()	;
}
//----------------------------------------------------------------------------------------------------------
LightU3D::~LightU3D()
{
	if( Spos )	delete Spos		;
	if( Starg ) delete Starg	;
	if( Scolor ) delete Scolor	;
}
//----------------------------------------------------------------------------------------------------------
bool LightU3D::Serialize( CArchive &ar )
{
	U32				a				;
	U16				Chunk			;
	U16				npkeys			;
	U16				ntkeys			;
	U16				nckeys			;
	PosKeyFileU3D	*posK = NULL	;
	PosKeyFileU3D	*targK = NULL	;
	PosKeyFileU3D	*colorK = NULL	;

	if( !ar.IsStoring() )
	{
		ar >> Nom	;
		ar >> Type	;

		ar >> Pos[0]	;	ar >> Pos[1]	;	ar >> Pos[2]	;

		ar >> RVB[0]	;	ar >> RVB[1]	;	ar >> RVB[2]	;
		ar >> Intensity	;

		ar >> Attenuation	;
		ar >> Inner			;
		ar >> Outer			;

		ar >> Targ[0]	;	ar >> Targ[1]	;	ar >> Targ[2]	;
		ar >> Hotspot	;
		ar >> Falloff	;


		//---------------------- Spline de mouvement
		ar >> npkeys	;
		if( npkeys ) posK = new PosKeyFileU3D[ npkeys ]		;
		for( a=0; a<npkeys;	a++	)	posK[a].Serialize( ar )	;
		if( npkeys > 1)
		{
			Spos = new SplineU3D( posK, npkeys )	;
		}
		else
		{
			Spos = NULL	;
		}
		if( posK ) delete [] posK	;


		//---------------------- Spline de la target
		ar >> ntkeys	;
		if( ntkeys ) targK = new PosKeyFileU3D[ ntkeys ]	;
		for( a=0; a<ntkeys;	a++	)	targK[a].Serialize( ar );
		if( ntkeys > 1)
		{
			Starg = new SplineU3D( targK, npkeys )	;
		}
		else
		{
			Starg = NULL	;
		}
		if( targK ) delete [] targK	;


		//---------------------- Spline de couleurs
		ar >> nckeys	;
		if( nckeys ) colorK = new PosKeyFileU3D[ nckeys ]	;
		for( a=0; a<nckeys;	a++	)	colorK[a].Serialize( ar );
		if( nckeys > 1)
		{
			Scolor = new SplineU3D( colorK, nckeys )	;
		}
		else
		{
			Scolor = NULL	;
		}
		if( colorK ) delete [] colorK	;


		ar >> Chunk										;
		if( Chunk != CHUNK_LIGHT_END )	return FALSE	;
	}

	Normalize()	;

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------
// est-ce que la lumière est animée ?
// TODO : a faireeeeeeeeeeeeeeee!
bool LightU3D::IsAnimated()
{
	return FALSE	;
}
//----------------------------------------------------------------------------------------------------------
// remet les données à jour comme il fo
void LightU3D::Normalize()
{
	vec3_sub( Vec, Targ, Pos )	;
	vec3_normalize( Vec )		;
}
//----------------------------------------------------------------------------------------------------------
// TODO : géré les hotspot et fallof des spots !
void LightU3D::CalcGouraudStaticLighting( Ufloat *dest, ObjetU3D *obj  )
{
	Ufloat	TrPos[3]		;
	Ufloat	TrRVB[3]		;
	Ufloat	PosCoord[3]		;
	Ufloat	vLightObj[3]	;
	Ufloat	p[3]			;

	obj->GetObjToWorld().GetTranspose( TrPos, Pos )		;

	vec3_mul( TrRVB, RVB, Intensity )	;	// ini RVB en fct de l'intensité de la light

	for( U32 a=0; a<obj->nbcoords; a++)
	{
		obj->GetObjToWorld().GetTransfo( PosCoord, obj->Ctab[a].origine )	;

		//if( !U3D3Monde3D->SegmentIntersecPoly( Pos, PosCoord ) )
		{
			vec3_sub( vLightObj, obj->Ctab[a].origine, TrPos )	;
			vec3_normalize( vLightObj )							;
			vec3_neg( vLightObj )								;

			Ufloat val = vec3_dot( vLightObj, obj->Ctab[a].normale )	;
			if( val < 0.f )	continue									;

			if( Attenuation )
			{
				Ufloat dist = vec3_dist( TrPos, obj->Ctab[a].origine )			;
				if( dist > Outer )	continue									;
				if( dist > Inner )	val *= 1.f - (dist-Inner) / ( Outer-Inner )	;	// calcul distance atténuation
			}

			vec3_mul( p, TrRVB, val )				;
			vec3_add( &dest[a*3], &dest[a*3], p )	;	// op additionne valeur de lighting !
		}
	}
}
//----------------------------------------------------------------------------------------------------------
// va calculer le lighting pour l'objet
// TODO : - OPTIMISER grave ces fonctions paskeu la c'est TROP TROP pas bien !!!!!!!!!!!
//		  - peut etre gérer le Falloff&Hotspot pour le spot justement
void LightU3D::CalcLightingRealTime( ObjetU3D *obj )
{
	Ufloat	TrPos[3]		;
	Ufloat	TrVec[3]		;
	Ufloat	TrRVB[3]		;
	Ufloat	vLightObj[3]	;
	Ufloat	OuterC,InnerC	;

	OuterC = Outer*Outer	;
	InnerC = Inner*Inner	;

	vec3_mul( TrRVB, RVB, Intensity )	;	// ini RVB en fct de l'intensité de la light

	//----------------------- Calcul Positions, et vecteur de la light dans l'espace objet
	obj->GetObjToWorld().GetTranspose( TrPos, Pos )		;
	obj->GetObjToWorld().GetTransposeR( TrVec, Vec )	;

	//----------------------- Calcul le flat shading de l'objet
	if( obj->IsFlatShading() )
	{
/*		if( Type== TYPE_LIGHT_OMNI )
		{
			vec3_eg( vLightObj, TrPos )	;
			vec3_normalize( vLightObj )	;
		}
		else if( Type== TYPE_LIGHT_SPOT )
		{
			vec3_eg( vLightObj, Vec )	;
			vec3_neg( vLightObj )		;
		}*/

		for( U32 a=0; a<obj->nbpolys; a++)
		{
			Ufloat	p[3]	;

			if( obj->Ptab[a].IsHide() ) continue	;

			vec3_sub( vLightObj, obj->Ptab[a].Vtab[0]->c->origine, TrPos )	;
			vec3_FastNormalize( vLightObj )									;
			vec3_neg( vLightObj )											;

			Ufloat val = vec3_dot( vLightObj, obj->Ptab[a].plan.normal )	;
			if( val < 0.f )	continue										;

			if( Attenuation )
			{
//				Ufloat dist = vec3_distFake( TrPos, obj->Ptab[a].Vtab[0]->c.origine )	;
				Ufloat dist = vec3_dist( TrPos, obj->Ptab[a].Vtab[0]->c->origine )		;
				if( dist > Outer ) continue	;
				if( dist > Inner )
					val *= 1.f - (dist-Inner) / ( Outer-Inner )	;	// calcul dustance atténuation
			}

			vec3_mul( p, TrRVB, val )							;
			vec3_add( obj->Ptab[a].RVBA, obj->Ptab[a].RVBA, p )	;	// op additionne valeur de lighting !
		}
	}

	//----------------------- Calcul le gouraud shading de l'objet
	if( obj->IsGouraudShading() )
	{
		if( Attenuation)
		{
			if( !obj->SphereIntersecObj( TrPos, Outer) )	return ;	// la light ne coupe pas l'objet alors cassos

			for( U32 a=0; a<obj->nbcoords; a++)
			{
				Ufloat	p[3]	;

				Ufloat dist = vec3_distFake( TrPos, obj->Ctab[a].origine )	;
				if( dist > OuterC ) continue	;

				vec3_sub( vLightObj, obj->Ctab[a].origine, TrPos )	;
				vec3_FastNormalize( vLightObj )						;
				vec3_neg( vLightObj )								;

				Ufloat val = vec3_dot( vLightObj, obj->Ctab[a].normale )	;
				if( val < 0.f )	continue									;

				if( dist > InnerC )
					val *= 1.f - (dist-InnerC) / ( OuterC-InnerC )	;	// calcul distance atténuation

				vec3_mul( p, TrRVB, val )							;
				vec3_add( obj->Ctab[a].RVBA, obj->Ctab[a].RVBA, p )	;	// op additionne valeur de lighting !
			}
		}
		else
		{
			for( U32 a=0; a<obj->nbcoords; a++)
			{
				Ufloat	p[3]	;

				vec3_sub( vLightObj, obj->Ctab[a].origine, TrPos )	;
				vec3_FastNormalize( vLightObj )						;
				vec3_neg( vLightObj )								;

				Ufloat val = vec3_dot( vLightObj, obj->Ctab[a].normale )	;
				if( val < 0.f )	continue									;

				vec3_mul( p, TrRVB, val )							;
				vec3_add( obj->Ctab[a].RVBA, obj->Ctab[a].RVBA, p )	;	// op additionne valeur de lighting !
			}
		}
	}
}
//----------------------------------------------------------------------------------------------------------
void LightU3D::IniAnim()
{
	if( Spos )
	{
		Spos->Ini()			;
		Spos->GetVal( Pos )	;
	}
	if( Starg )
	{
		Starg->Ini()			;
		Starg->GetVal( Targ )	;
		Normalize()				;
	}
	if( Scolor )
	{
		Scolor->Ini()			;
		Scolor->GetVal( RVB )	;
	}
}
//----------------------------------------------------------------------------------------------------------
void LightU3D::IncAnim( Ufloat laptime )
{
	if( Spos )
	{
		Spos->IncPos( laptime )		;
		Spos->GetVal( Pos )			;
	}
	if( Starg )
	{
		Starg->IncPos( laptime )	;
		Starg->GetVal( Targ )		;
		Normalize()					;
	}
	if( Scolor )
	{
		Scolor->IncPos( laptime )	;
		Scolor->GetVal( RVB )		;
	}
}
//----------------------------------------------------------------------------------------------------------
