	   			 /*---------------------------------------------------------------------+
				  |							ObjDeformation.cpp							|
				  |																		|
				  |	une classe pour faire un ki deformera un objet dans tout les sens	|
				  |																		|
				  | 	U2^PoPsy TeAm 2000												|
				  +---------------------------------------------------------------------*/

#include "stdafx.h"

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+---------------------------+
//										|    Class ObjDeformation	|
//										+---------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
ObjDeformation::ObjDeformation( ObjetU3D* objet, Ufloat ampl, Ufloat freq )
{
	obj = objet	;

	AllVertex = new DefObjVertex[ obj->nbcoords ]	;

	amplitude = ampl	;
	frequence = freq	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
ObjDeformation::~ObjDeformation()
{
	delete [] AllVertex	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void ObjDeformation::Sort(S32 deb, S32 fin)
{
	S32				i=deb								;
	S32				j=fin								;
	Ufloat			milieu=AllVertex[(deb+fin)/2].dist	;
	DefObjVertex	tmp									;

	while(i<=j)
	{
		while(AllVertex[i].dist > milieu) i++	;
		while(AllVertex[j].dist < milieu) j--	;

		if(i<=j)
		{
			tmp.dist=AllVertex[i].dist		;
			tmp.coord=AllVertex[i].coord	;

			AllVertex[i].dist=AllVertex[j].dist		;
			AllVertex[i].coord=AllVertex[j].coord	;

			AllVertex[j].dist=tmp.dist		;
			AllVertex[j].coord=tmp.coord	;
			i++; j--						;
		}
	}
	if(i<fin) Sort(i,fin)	;
	if(deb<j) Sort(deb,j)	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void ObjDeformation::Confirm()
{
	U32		a=0		;

	//--------- assigne coordonnée et caclul distance
	for( a=0; a<obj->nbcoords; a++ )
	{
		AllVertex[a].coord = &obj->Ctab[a]						;
		AllVertex[a].dist = vec3_sqrt( obj->Ctab[a].origine )	;
	}

	//-------- trie les vertex par rapport a leur distance au centre
	Sort( 0, obj->nbcoords-1 )	;

	//-------- calcul vecteur depuis centre de deformation
	for( a=0; a<obj->nbcoords; a++ )
	{
		vec3_eg( AllVertex[a].cbase, AllVertex[a].coord->origine )	;
		vec3_eg( AllVertex[a].Vdef, AllVertex[a].cbase )			;
		vec3_normalize( AllVertex[a].Vdef )							;

		if( AllVertex[a].dist < 2.f* amplitude )
			AllVertex[a].fact = 1.0f - ((2.f* amplitude) - AllVertex[a].dist) / (2.f* amplitude) ;
		else
			AllVertex[a].fact = 1.0f	;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void ObjDeformation::Anim( Ufloat time )
{
	DefObjVertex*	tmp = AllVertex		;
	Ufloat			tmpv[3]				;
	Ufloat			Pos = 0.0f,incpos	;
	U32				a					;
	
	incpos = (Ufloat)1.f/obj->nbcoords	;
	for( a=0; a<obj->nbcoords; a++,tmp++,Pos+=incpos )
	{
		Ufloat ampl = tmp->fact * amplitude * Cos( Pos*frequence + time )	;
		vec3_mul( tmpv, tmp->Vdef, ampl	 )									;
		vec3_add( tmp->coord->origine, tmpv, tmp->cbase )					;
	}

	obj->CalcNormales()						;
	obj->Transform( obj->GetObjToWorld() )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
