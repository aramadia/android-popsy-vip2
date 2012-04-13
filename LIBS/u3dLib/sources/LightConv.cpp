    				 /*-----------------------------------------------------+
					  |				        LightConv.cpp					|
					  |														|
					  |	  cette classe me permet de traiter + facilement    |
					  |	    les données a convertir entre des datas 3D 		|
					  |		 quelconques et le formet spécifik U3D		    |
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#include "U3D3.h"

#ifdef TOOLU3D
//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|    Class LightConv    |
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
LightU3DConv::LightU3DConv()
{
	Nom = "Default"			;
	Type = TYPE_LIGHT_OMNI	;
	suiv = NULL				;

	vec3_set( Pos, 0.0f, 0.0f, 0.0f )	;

	vec3_set( RVB, 1.0f, 1.0f, 1.0f )	;
	Intensity = 1.0f					;

	Attenuation = FALSE	;	// est-ce que l'attenuation est gérée ?
	Inner = 100.f		;
	Outer = 1000.f		;

	//---------- donnée relative au spot !
	vec3_set( Targ, 0.0f, 0.0f, 100.f )	;
	Hotspot = .7853981634				;	// Hotspot a 45 degré par defaut 
	Falloff = 1.570796327				;	// Falloff a 90 degré par defaut 

	npkeys = 0	;
	posK = NULL	;

	ntkeys = 0		;
	targK = NULL	;

	nckeys = 0		;
	colorK = NULL	;
}
//----------------------------------------------------------------------------------------------------------
LightU3DConv::~LightU3DConv()
{
	if( posK ) delete [] posK		;
	if( targK ) delete [] targK		;
	if( colorK ) delete [] colorK	;
}
//----------------------------------------------------------------------------------------------------------
void LightU3DConv::Conv3DS( light3ds *li )
{
	Nom = li->name	;

	vec3_set( Pos, li->pos.x, li->pos.z, li->pos.y )	;


	vec3_set( RVB, li->color.r, li->color.g, li->color.b )	;
	Intensity = li->multiplier								;
	Attenuation = li->attenuation.on						;
	Inner = li->attenuation.inner							;
	Outer = li->attenuation.outer							;

	if( li->spot )
	{
		Type = TYPE_LIGHT_SPOT	;
		
		vec3_set( Targ, li->spot->target.x, li->spot->target.z, li->spot->target.y )	;

		Hotspot = li->spot->hotspot	;
		Falloff = li->spot->falloff	;
	}
	else
	{
		Type = TYPE_LIGHT_OMNI	;
	}

}
//----------------------------------------------------------------------------------------------------------
void LightU3DConv::Conv3DS( kfomni3ds *li )
{
	U32 a	;

	npkeys = (U16)li->npkeys	;	// nb pos key
	nckeys = (U16)li->nckeys	;	// nb color key

	posK = new PosKeyFileU3D[ npkeys ]		;
	colorK = new PosKeyFileU3D[ nckeys ]	;

	//------- copie toutes les positions
	for(a=0; a<npkeys; a++)
	{
		vec3_set( posK[a].p, li->pos[a].x, li->pos[a].z, li->pos[a].y )	;
		KHeadereg( posK[a].k, li->pkeys[a] )							;
	}

	//------- copie toutes les couleurs
	for(a=0; a<nckeys; a++)
	{
		vec3_set( colorK[a].p, li->color[a].r, li->color[a].g, li->color[a].b )	;
		KHeadereg( colorK[a].k, li->ckeys[a] )									;
	}
}
//----------------------------------------------------------------------------------------------------------
void LightU3DConv::Conv3DS( kfspot3ds *li )
{
	U32 a	;

	npkeys = (U16)li->npkeys	;	// nb pos key
	ntkeys = (U16)li->ntkeys	;	// nb target key
	nckeys = (U16)li->nckeys	;	// nb color key

	posK = new PosKeyFileU3D[ npkeys ]		;
	targK = new PosKeyFileU3D[ ntkeys ]		;
	colorK = new PosKeyFileU3D[ nckeys ]	;

	//------- copie toutes les positions
	for(a=0; a<npkeys; a++)
	{
		vec3_set( posK[a].p, li->pos[a].x, li->pos[a].z, li->pos[a].y )	;
		KHeadereg( posK[a].k, li->pkeys[a] )							;
	}

	//------- copie toutes les targets
	for(a=0; a<ntkeys; a++)
	{
		vec3_set( targK[a].p, li->tpos[a].x, li->tpos[a].z, li->tpos[a].y )	;
		KHeadereg( targK[a].k, li->tkeys[a] )								;
	}

	//------- copie toutes les couleurs
	for(a=0; a<nckeys; a++)
	{
		vec3_set( colorK[a].p, li->color[a].r, li->color[a].g, li->color[a].b )	;
		KHeadereg( colorK[a].k, li->ckeys[a] )									;
	}
}
//----------------------------------------------------------------------------------------------------------
void LightU3DConv::Serialize( CArchive &ar )
{
	U32	a ;

	if( ar.IsStoring() )
	{
		ar << Nom	;
		ar << Type	;

		ar << Pos[0]	;	ar << Pos[1]	;	ar << Pos[2]	;

		ar << RVB[0]	;	ar << RVB[1]	;	ar << RVB[2]	;
		ar << Intensity	;

		ar << Attenuation	;
		ar << Inner			;
		ar << Outer			;

		ar << Targ[0]	;	ar << Targ[1]	;	ar << Targ[2]	;
		ar << Hotspot	;
		ar << Falloff	;

		ar << npkeys	;
		for( a=0; a<npkeys;	a++	)	posK[a].Serialize( ar )	;

		ar << ntkeys	;
		for( a=0; a<ntkeys;	a++	)	targK[a].Serialize( ar )	;

		ar << nckeys	;
		for( a=0; a<nckeys;	a++	)	colorK[a].Serialize( ar )	;


		ar << CHUNK_LIGHT_END	;
	}
}
//----------------------------------------------------------------------------------------------------------
#endif
