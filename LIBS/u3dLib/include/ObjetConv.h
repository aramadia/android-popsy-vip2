    				 /*-----------------------------------------------------+
					  |				        ObjetConv.h						|
					  |														|
					  |	  cette classe me permet de traiter + facilement    |
					  |	    les données a convertir entre des datas 3D 		|
					  |		 quelconques et le formet spécifik U3D		    |
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#ifdef TOOLU3D
#ifndef _OBJETCONV_U3D3_H
#define _OBJETCONV_U3D3_H

//==================
// nom du material de l'objet

struct MaterialU3DConv{

	CString			nom			;
	Ufloat			tilex,tiley	;
	U8				num			;
	MaterialU3DConv	*suiv		;

};

//==================

struct Coord3DConv{

	Ufloat		coord[3]	;
	U16			num			;
	Coord3DConv	*suiv		;	
	Coord3DConv	*preced		;

};

//==================

struct VertexU3DConv{

	Ufloat			u,v		;	// info Texture
	U16				num		;
	Coord3DConv		*c		;
	VertexU3DConv	*suiv	;
	VertexU3DConv	*preced	;

};

//==================

struct PolygonU3DConv{

	VertexU3DConv	*tab[3]	;	// tableau qui donne les pointeurs sur les vertex
	U8				flag	;	// flag décrivant les arretes visiblent
	U8				wrap	;	// indic si il fo faire boucler le tiling du poly : 1 pour U et 2 pour V
	U16				num		;	// numero de la face

	MaterialU3DConv	*mat	;		

	PolygonU3DConv *suiv	;	// face suivante pour l'objet


};

//==================
// objet à traiter durant la transition des formats quelconque -> U3d

class ObjetU3DConv{

	CString			nom			;
	Ufloat			Pivot[3]	;
	Mat3x4			locaMatrix	;
	ObjetU3DConv	*Childs		;
	CString			csParent	;	// nom du parent
	bool			BaseObject	;	// est-ce ke cet objet n'a pas de parents

	U16			nbcoord		;
	Coord3DConv	*Ctab		;
	Coord3DConv	*CtabFin	;

	U16				nbvertices	;
	VertexU3DConv	*Vtab		;
	VertexU3DConv	*VtabFin	;

	U16				nbfaces	;
	PolygonU3DConv	*Ftab	;

	U8				nbmats	;
	MaterialU3DConv *mat	;

	U16				npkeys	;	// nb pos key
	PosKeyFileU3D	*posK	;

	U16				nrkeys	;	// nb rot key
	RotKeyFileU3D	*rotK	;

	U16				nmkeys	;	// nb morph key
	MorphKeyFileU3D	*morphK	;

	U16				nhkeys	;	// nb hide key
	HideKeyFileU3D	*hideK	;


public:

	ObjetU3DConv *suiv		;
	ObjetU3DConv *Childsuiv	;	// kan hiérarchie on stock les enfants la dedans !

	ObjetU3DConv()	;
	~ObjetU3DConv()	;

	LPCSTR GetNom()					{return (LPCSTR)nom;}
	void Renomme( LPCSTR n )		{nom = n;}
	void Conv3DS( mesh3ds *m )		;
	void Conv3DS( kfmesh3ds *m )	;

	void GetLocaMatrixFrom3DS( Ufloat mat3ds[12] )	;

	void	AddCoord( Coord3DConv *c )		;
	void	AddCoord( point3ds *pt )		;
	U16		GetNBCoord()					{return nbcoord;}
	U16		GetCoordIndex( Coord3DConv *c)	{return c->num;}
	void	WolrdToLoca()					;	// transform les coords dans le system local de l'objet
	void	WriteCoords( CArchive &ar )		;

	void			AddVertex( VertexU3DConv *v )		;
	void			AddVertex( textvert3ds *v )			;
	VertexU3DConv	*GetVertex(U16 pos)					;
	U16				GetNBVertex()						{return nbvertices;}
	U16				GetVertexIndex( VertexU3DConv *v)	{return v->num;}
	void			WriteVertex( CArchive &ar )			;

	void			AddFace( face3ds *f )			;
	void			AddFace( PolygonU3DConv *f )	;
	PolygonU3DConv* GetFace(U16 pos)				;
	U32				GetNBFace()						{return nbfaces;}
	U16				GetFaceIndex(PolygonU3DConv *f)	{return f->num;}
	void			DoWrapping()					;
	void			WriteFaces( CArchive &ar )		;

	void		AddMat( MaterialU3DConv *m )		;
	void		AddMat( LPCSTR nom )				;
	U8			GetmatIndex( MaterialU3DConv *m )	{return m->num;}
	void		WriteMats( CArchive &ar )			;

	void			AddChild( ObjetU3DConv *child )	;
	ObjetU3DConv	*FindChild( LPCSTR nom )		;
	bool			IsBase()						{return BaseObject;}

	void Optimiz()	;

	void Serialize( CArchive &ar )	;

};

//==================


#endif
#endif
