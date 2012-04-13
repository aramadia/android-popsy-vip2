    					 /*-----------------------------------------------------+
						  |				         BSP.h							|
						  |														|
						  |	    la définition de l'arbre BSP mais ici pour le   |
						  |					moteur temps réel!					|
						  |														|
						  | 	U2^PoPsy TeAm 1999								|
						  +-----------------------------------------------------*/

#ifndef _BSP_U3D3_H
#define _BSP_U3D3_H


/*//-------
// flags de clipping

#define ClipNo		0x00
#define ClipDroit	0x01
#define ClipGauche	0x02
#define ClipHaut	0x04
#define ClipBas		0x08
#define ClipProche	0x10
#define ClipAll		0x1f
#define ClipCache	0x20*/


//==================
// definition d'une bouding box
/*
struct BoundingBox{
	
	// Avec PT0 = MinX MaxZ MaxY
	//		PT1 = MaxX MaxZ MaxY
	//		PT2 = MinX MinZ MaxY
	//		PT3 = MaxX MinZ MaxY
	// puis PT4 = "       " MinY
	// etc.....
	Ufloat pts[8][3];

	BoundingBox();
	void TestEtActua( Ufloat *pt );		// actualise la Bounding en fct du point si necessaire
	bool Contenu( BoundingBox &b );
};



//==================
// definition d'une bouding sphere

struct BoundingSphere{

	Ufloat centre[3];
	Ufloat rayon;
	Ufloat rayonFake;		// rayon sans la racine carré !

	BoundingSphere();

};
*/



//==================
// definition d'un plan
/*
typedef enum{DEVANT,DERRIERE,ENTRE_DEUX}BoundPlace;

struct Uplan_BSP{

	Ufloat normal[3];	// normale
	Ufloat dist;		// distance sur la normale

	Uplan_BSP(){vec3_set( normal, 0.f, 0.f, 0.f);dist=0.f;}
	inline Ufloat distance(const Ufloat v[3]){ return vec3_dot( v, normal) - dist; }

	// recalcul le plan à partie des 3 points 
	void Calc( const Ufloat pt0[3], const Ufloat pt1[3], const Ufloat pt2[3] );	

	// renvoie la matrice de transformation associée au plan 
	void GetTransMatrix( Ufloat m[4][4] );

	//renvoie position d'une Bounding box par rapport au plan
	BoundPlace GetPos( BoundingBox &b );
};*/



/*
//==================
//definition d'une coordonnée 3D

struct CoordU3D{

	// coordonnées du point..
	Ufloat origine[3];			// d'origine
	Ufloat trans[3];			// après transformation
	Ufloat ecran[3];			// 2D

	Ufloat normale[3];			// normale du vertex

	Ufloat RVB[3];				// couleur ( si Gouraud! )
	Ufloat A;					// alpha

	U32 ClipInfo;				// info de clipping

	U32 utilisateurs;			// nombre de vertex qui pointe sur lui
	union{
		U32 frame_process;		// numéro de la dernière frame qui l'a modifié
		CoordU3D *suiv;			// Coordonnée suivante
	};

	CoordU3D(){vec3_set( origine, 0.f, 0.f, 0.f);
			   vec3_set( trans,   0.f, 0.f, 0.f);	
			   vec3_set( ecran,   0.f, 0.f, 0.f);
			   vec3_set( normale, 0.f, 0.f, 0.f);
			   vec3_set( RVB,	  1.f, 1.f, 1.f);
			   A = 1.f;
			   ClipInfo = ClipNo;
			   utilisateurs = 0;
			   frame_process = (U32)-1;
	}

};



//==================
// definition d'un vertex 

struct VertexU3D{

	CoordU3D *c;				// coordonnée du vertex

	Ufloat U,V;					// coordonnées de texture1
	Ufloat U1,V1;				// coordonnées de texture2
#ifdef _LMAPS
	char LU,LV;					// coordonnées dans la lightmap
#endif

	union{
		U32 utilisateurs;		// nombre de polygone qui point sur lui
		VertexU3D *suiv;		// vertex suivant
	};

	//==========
	// va interpoler les données entre pdebut et pfin en fct de scale ( 0<scale<1 )
	// et foutre le resultat dans this
	void inline Interpole3D( VertexU3D *pdebut, VertexU3D *pfin, Ufloat scale )
	{
		Ufloat temp[3];
		U = pdebut->U + ( pfin->U - pdebut->U )*scale;
		V = pdebut->V + ( pfin->V - pdebut->V )*scale;
		U1 = pdebut->U1 + ( pfin->U1 - pdebut->U1 )*scale;
		V1 = pdebut->V1 + ( pfin->V1 - pdebut->V1 )*scale;
		vec3_sub( temp, pfin->c->RVB, pdebut->c->RVB );
		vec3_mul( temp, temp, scale );
		vec3_add( c->RVB, temp, pdebut->c->RVB );
		vec3_sub( temp, pfin->c->trans, pdebut->c->trans );
		vec3_mul( temp, temp, scale );
		vec3_add( c->trans, temp, pdebut->c->trans );
	}

	//==========
	// meme chose que pour interpole3D sauf qu'on boss sur les 
	// donnée de l'espace ecran. Attention gere la correct de perspective
	void inline Interpole2D( VertexU3D *pdebut, VertexU3D *pfin, Ufloat scale )
	{
		Ufloat temp[3];
		Ufloat UnSurZdebut = 1.f / pdebut->c->ecran[2];
		Ufloat UnSurZfin   = 1.f / pfin->c->ecran[2];
		c->ecran[2] = 1.f / ( ( UnSurZfin - UnSurZdebut )*scale + UnSurZdebut );
		scale = ( c->ecran[2] - pdebut->c->ecran[2] ) / ( pfin->c->ecran[2] - pdebut->c->ecran[2] );
		U = pdebut->U + ( pfin->U - pdebut->U )*scale;
		V = pdebut->V + ( pfin->V - pdebut->V )*scale;
		U1 = pdebut->U1 + ( pfin->U1 - pdebut->U1 )*scale;
		V1 = pdebut->V1 + ( pfin->V1 - pdebut->V1 )*scale;
		vec3_sub( temp, pfin->c->RVB, pdebut->c->RVB );
		vec3_mul( temp, temp, scale );
		vec3_add( c->RVB, temp, pdebut->c->RVB );
	}

};

//==================
// defintion d'un polygone
#define PolyFLAG_NORM		0x0
#define PolyFLAG_HIDE		0x1
#define PolyFLAG_TEMP		0x2

struct PolygonU3D{

	U32 nbVertex;			// nombre de vertex
	VertexU3D **Vtab;		// tabelau de pointeurs sur vertex

	U32 arrete;				// flag décrivant les arrètes visibles
	U32 flag;				// flag utile pour le pipeline de rendu

	MaterialU3D *m;			// op un chti material pour le poly
	TextureU3D *lmapTex;	// Texture de lightmap si yen a !


	PolygonU3D *suiv;		// polygone suivant

	Uplan_BSP plan;			// plan dans lequel est inscrit le poly

	Ufloat RVB[3];			// couleur de la face

#ifdef STREAM
	U32 IDnum;				// numero d'identification
	bool HidePreced;		// TRUE si face cachée précédement
	bool Hide;				// TRUE si face cachée actuellement
#endif

#ifdef _LMAPS
	Lightmap *lmap;
#endif

	PolygonU3D();
	~PolygonU3D();
	void CalcPlan();
	PolygonU3D* ClipToPlan( Uplan_BSP *plan );	
	PolygonU3D* GetClipped2DPoly(U32 flag,Ufloat val);
	PolygonU3D* GetClipped2DPoly();

	// renvoie TRUE si le segment coupe le poly
	bool Intersection( Ufloat debut[3], Ufloat fin[3] );


	//==========
	bool inline BackFaceCull( Ufloat *pt )
	{	return plan.distance( pt ) > 0; }

	//==========
	// renvoie valeur Z d'un poly dans l'espace de camera
	Ufloat inline GetZ()			
	{
		Ufloat z=0.f;
		for(U32 a=0; a<nbVertex; a++)
			z += Vtab[a]->c->ecran[2];
		return z;
	}

	//==========
	// renvoie le poly si il peut être affiché
	PolygonU3D* GetToDraw()	
	{
		if( (flag&PolyFLAG_HIDE) != 0 ) return NULL;
		return this;
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

};*/

//==================
// definition d'un noeud BSP


class Unoeud_BSP{

// c'est la bounding box que recouvre l'espace représenté par ce noeud
	BoundingBox B;

	Unoeud_BSP	 *devant;			// Noeud de devant
	Unoeud_BSP	 *derriere;			// Noeud de derriere

	Plan plan;					// plan de coupure du noeud

	U32		nbfaces;				// nombre de polys dans le noeud
	PolygonU3D **Ptab;				// tableau de pointeurs sur les polys


public:

	Unoeud_BSP(U32 nbf);
	~Unoeud_BSP();

	void AddDevant(Unoeud_BSP *n){ devant=n; }
	void AddDerriere(Unoeud_BSP *n){ derriere=n; }

	void CalculPlan();

	void CalculBounding();
	void GetBounding(BoundingBox &box);

	void Affiche(U32 Flagc=ClipAll);
	void AfficheDyna(U32 Flagc=ClipAll);	// affiche mais des arbres d'objets dynamiques
};

//==================

#endif
