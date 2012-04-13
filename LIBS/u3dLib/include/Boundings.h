    			 /*-----------------------------------------------------+
				  |				         Boundings.h					|
				  |														|
				  |		les machins ki entourent les des trucs 			|
				  |														|
				  | 	U2^PoPsy TeAm 1999								|
				  +-----------------------------------------------------*/

#ifndef _BOUNDS_U3D3_H
#define _BOUNDS_U3D3_H


////////////////////////////////////////
// Definition d'une boundingbox alignée sur les axes
class AABoundingBox{

	Ufloat Min[3]	;	// valeur min en X,Y, et Z
	Ufloat Max[3]	;	// valeur max en X,Y, et Z

//------------------ flags
public:

	// position relative a la box
	enum{
		coupe = 0x0,
		inside = 0x1,
		outside = 0x2
	};
	
public:

	AABoundingBox()	;
	void Reset()	;

	AABoundingBox& operator+=( AABoundingBox& b)	;

	void PointIn( const Ufloat pt[3] )	;	// indic ke le point est dans la bounding ( donc va agrandir la boundig si besoin )
	void PointInX( Ufloat pt )			;	// indic ke la valeur pt est dans la bounding en X ( donc va agrandir la boundig si besoin )
	void PointInY( Ufloat pt )			;	// indic ke la valeur pt est dans la bounding en Y ( donc va agrandir la boundig si besoin )
	void PointInZ( Ufloat pt )			;	// indic ke la valeur pt est dans la bounding en Z ( donc va agrandir la boundig si besoin )

	Ufloat GetMinX() {return Min[0];}
	Ufloat GetMinY() {return Min[1];}
	Ufloat GetMinZ() {return Min[2];}
	Ufloat GetMaxX() {return Max[0];}
	Ufloat GetMaxY() {return Max[1];}
	Ufloat GetMaxZ() {return Max[2];}

	bool IsInside( const Ufloat pos[3] )							;	// renvoie TRUE si point a l'interieur de la bounding
	bool IsInside( const Ufloat pos[3], Ufloat rayon )				;	// renvoie TRUE si la bounding box est completement contenue dans la sphere
	bool SegmentIntersec( const Ufloat a[3], const Ufloat b[3] )	;	// renvoie TRUE si le segment coupe la boundig box
	bool SphereIntersec( const Ufloat pos[3], const Ufloat rayon )	;	// renvoie TRUE si la sphere coupe la bounding box de l'objet. ( attention pos dans espace objet )  ( NB rayon = vrai rayo*vrai rayon )

	U32 SpherePos( const Ufloat pos[3], const Ufloat rayonFake )	;	// renvoie position de la sphere par rapport a la bounding
};
////////////////////////////////////////





////////////////////////////////////////
// definition d'une bouding box
class BoundingBox{
	
public:
	// Avec PT0 = MinX MaxZ MaxY
	//		PT1 = MaxX MaxZ MaxY
	//		PT2 = MinX MinZ MaxY
	//		PT3 = MaxX MinZ MaxY
	// puis PT4 = "       " MinY
	// etc.....
	Ufloat pts[8][3];


	BoundingBox()	;


	U32 GetType()					{return 0;}		// renvoie type de bounding
	void TestEtActua( Ufloat *pt )	;	// actualise la Bounding en fct du point si necessaire
	bool Contenu( BoundingBox &b )	;

	void Set( AABoundingBox& b, Mat3x4& m )	;

	void Affiche( Ufloat RVBA[4] )	;	// affiche les lignes de la bounding box ( attentions le pipelein doit etre deja en mode begin!! )
};
////////////////////////////////////////




////////////////////////////////////////
// definition d'une bouding sphere
class BoundingSphere/* : public Bound*/{

	Ufloat centre[3]	;
	Ufloat rayon		;
	Ufloat rayonFake	;	// rayon sans la racine carré !

public:

	BoundingSphere()	;

	U32 GetType()	{return 0;}		// renvoie type de bounding
};
////////////////////////////////////////





#endif
