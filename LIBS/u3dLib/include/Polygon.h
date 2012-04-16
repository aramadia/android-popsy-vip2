    				 /*-----------------------------------------------------+
					  |				        Polygon.h						|
					  |														|
					  |	   Le polygon est l'element de base a tout moteur   |
					  |		    3D ki se respecte vous en conviendrez		|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#ifndef _BSP_U3D3_H
#define _BSP_U3D3_H

//-------
// flags de clipping

#define ClipNo		0x00
#define ClipDroit	0x01
#define ClipGauche	0x02
#define ClipHaut	0x04
#define ClipBas		0x08
#define ClipProche	0x10
#define ClipAll		0x1f
#define ClipCache	0x20



//==================
//definition d'une coordonnée 3D

struct CoordU3D{

	// coordonnées du point..
	Ufloat origine[3]	;	// d'origine
	Ufloat trans[3]		;	// après transformation
	Ufloat ecran[4]		;	// X,Y,Z et W

	Ufloat normale[3]	;	// normale du vertex

	Ufloat RVBA[4]	;	// couleur ( si Gouraud! )

	U32 ClipInfo	;	// info de clipping

	U32 utilisateurs	;	// nombre de vertex qui pointe sur lui
	union{
		U32 frame_process	;	// numéro de la dernière frame qui l'a modifié
		CoordU3D *suiv		;	// Coordonnée suivante
	};

	CoordU3D()	;

	void Ini()	;	// met valeurs par defaut 

	void IncUser()	{utilisateurs++;}

};



//==================
// definition d'un vertex 

struct VertexU3D{

	CoordU3D *c	;	// coordonnée du vertex

	Ufloat U,V		;	// coordonnées de texture1
	Ufloat U1,V1	;	// coordonnées de texture2


	union{
		U32 utilisateurs	;	// nombre de polygone qui point sur lui
		VertexU3D *suiv		;	// vertex suivant
	};

	VertexU3D()	;

	void IncUser()	{utilisateurs++;}

	CoordU3D *operator=(CoordU3D *coord)	;	// attach coordonnée a ca vertex

	//==========
	// va interpoler les données entre pdebut et pfin en fct de scale ( 0<scale<1 )
	// et foutre le resultat dans this
	void Interpole3D( VertexU3D *pdebut, VertexU3D *pfin, Ufloat scale )
	{
		Ufloat temp[4]	;

		U = pdebut->U + ( pfin->U - pdebut->U )*scale		;
		V = pdebut->V + ( pfin->V - pdebut->V )*scale		;
		U1 = pdebut->U1 + ( pfin->U1 - pdebut->U1 )*scale	;
		V1 = pdebut->V1 + ( pfin->V1 - pdebut->V1 )*scale	;

		vec4_sub( temp, pfin->c->RVBA, pdebut->c->RVBA )	;
		vec4_mul( temp, temp, scale )						;
		vec4_add( c->RVBA, temp, pdebut->c->RVBA )			;
		vec3_sub( temp, pfin->c->trans, pdebut->c->trans )	;
		vec3_mul( temp, temp, scale )						;
		vec3_add( c->trans, temp, pdebut->c->trans )		;
	}

	//==========
	// meme chose que pour interpole3D sauf qu'on boss sur les 
	// donnée de l'espace ecran.
	void Interpole2D( VertexU3D *pdebut, VertexU3D *pfin, Ufloat scale )
	{
		Ufloat temp[4]	;

		U = pdebut->U + ( pfin->U - pdebut->U )*scale		;
		V = pdebut->V + ( pfin->V - pdebut->V )*scale		;
		U1 = pdebut->U1 + ( pfin->U1 - pdebut->U1 )*scale	;
		V1 = pdebut->V1 + ( pfin->V1 - pdebut->V1 )*scale	;

		vec4_sub( temp, pfin->c->RVBA, pdebut->c->RVBA )	;
		vec4_mul( temp, temp, scale )						;
		vec4_add( c->RVBA, temp, pdebut->c->RVBA )			;
	}


};




//==================
// defintion d'un polygone
#define PolyFLAG_NORM		0x0
#define PolyFLAG_HIDE		0x1
#define PolyFLAG_TEMP		0x2

struct PolygonU3D{

	U32			nbVertex	;	// nombre de vertex
	VertexU3D	**Vtab		;	// tabelau de pointeurs sur vertex

	U32 arrete	;	// flag décrivant les arrètes visibles
	U32 flag	;	// flag utile pour le pipeline de rendu

	U8 index;
	MaterialU3D *m	;	// op un chti material pour le poly

	PolygonU3D *suiv	;	// polygone suivant

	Plan plan	;	// plan dans lequel est inscrit le poly

	Ufloat RVBA[4]	;	// couleur de la face

	PolygonU3D( int nb=0 )	;	// donne le nombre de vertex du poly 
	~PolygonU3D()			;

	void SetNbVertex( U32 nb )	;	// definit le nombre de vertex du poly

	void		CalcPlan()												;
	void		CalcPlanRealTime()										{plan.CalcRT(Vtab[0]->c->origine,Vtab[2]->c->origine,Vtab[1]->c->origine );}
	PolygonU3D* ClipToPlan( Plan *plan )								;
	PolygonU3D* GetClipped2DPoly(U32 flag,Ufloat val, Viewport &view)	;
	PolygonU3D* GetClippedPolyByCamera()								;
	PolygonU3D* GetClippedPolyByViewport( Viewport &view )				;
	PolygonU3D* GetCopyPoly()											;	// renvoie une copie conforme du poly



	bool Intersection( Ufloat debut[3], Ufloat fin[3] )		;	// renvoie TRUE si le segment coupe le poly
	bool IntersectionMod( Ufloat debut[3], Ufloat fin[3] )	;	// renvoie TRUE si le segment coupe le poly et modifie fin ki sera pil poil a l'endroit de l'intersection !

	bool IsElement( CoordU3D *pt1, CoordU3D *pt2 )	;	// renvoie TRUE si les 2 point font partie du poly

	//==========
	// renvoie TRUE si le poly ne fait pas face au point
	bool inline BackFaceCull( Ufloat *pt )
	{	return plan.distance( pt ) <= 0.0f; }

	bool inline BackFaceCull2D()	;	// return TRUE kan poly non visible


	//==========
	// renvoie valeur Z d'un poly dans l'espace de camera
	Ufloat inline GetZ()			
	{
		Ufloat z=1e+19f	;
		for(U32 a=0; a<nbVertex; a++)
			if( z > Vtab[a]->c->ecran[2] )
				z = Vtab[a]->c->ecran[2]	;
		return z;
	}

	//==========
	// renvoie TRUE si le poly a besoin d'etre clippé 
	bool inline BesoinClip()
	{
		U32 ret = ClipNo;
		for(U32 a=0; a<nbVertex; a++)
			ret |= Vtab[a]->c->ClipInfo;
		return ret!=ClipNo;
	}

	//==========
	// renvoie TRUE si le poly a besoin d'etre clippé sur le plan
	// decrit par le flag 
	bool inline BesoinClip(U32 flag)
	{
		for(U32 a=0; a<nbVertex; a++)
			if( Vtab[a]->c->ClipInfo&flag )
				return TRUE;
		return FALSE;
	}

	//==========
	// renvoie TRUE si le poly est flagé Hidé !
	bool inline IsHide()
	{
		if( flag&PolyFLAG_HIDE )	return TRUE		;
		else						return FALSE	;
	}

};



//----------------------------------------------------------------------------------------------------------
//									+------------------------+
//									|  Les Fonctions Inline  |
//									+------------------------+
//----------------------------------------------------------------------------------------------------------
// return TRUE kan poly non visible
bool inline PolygonU3D::BackFaceCull2D()
{
	Ufloat v1 = Vtab[1]->c->ecran[0]	;
	Ufloat v2 = Vtab[1]->c->ecran[1]	;
	Ufloat a= (Vtab[0]->c->ecran[0] - v1) * (Vtab[2]->c->ecran[1] - v2) -
			 (Vtab[2]->c->ecran[0] - v1) * (Vtab[0]->c->ecran[1] - v2) ;

	if(  a < 0.0f )	return FALSE	;
	else			return TRUE		;
}
//----------------------------------------------------------------------------------------------------------


#endif
