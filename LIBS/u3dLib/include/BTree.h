    				 /*-----------------------------------------------------+
					  |				         BTree.h						|
					  |														|
					  |    C'est un arbre de boundings box ki permettra 	|
					  |		des tests de collision rapides sur les meshes	|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/

#ifndef _BTREE_U3D3_H
#define _BTREE_U3D3_H

////////////////////////////////////////
// une feuille de l'arbre en fait
class BTreeElem{

	PolygonU3D*		poly	;	// si non feuille terminale poly = NULL
	AABoundingBox	bounds	;	// boundings box de la feuille

public:

	BTreeElem*	suiv	;
	BTreeElem*	gauche	;
	BTreeElem*	droit	;

	BTreeElem(PolygonU3D *p=NULL)	;
	~BTreeElem()					;

	void SetSize( BTreeElem *liste )	;	// devient la boite englobante de tout les elements passés dans la liste

	bool IsTerminal()	{ return poly;}

	bool WicheXSide( Ufloat x )	;	// renvoie TRUE si majoritairement au dessus
	bool WicheYSide( Ufloat y )	;	// renvoie TRUE si majoritairement au dessus
	bool WicheZSide( Ufloat z )	;	// renvoie TRUE si majoritairement au dessus

	Ufloat GetMiddleX()	{return (bounds.GetMaxX()+bounds.GetMinX())*.5f;}
	Ufloat GetMiddleY()	{return (bounds.GetMaxY()+bounds.GetMinY())*.5f;}
	Ufloat GetMiddleZ()	{return (bounds.GetMaxZ()+bounds.GetMinZ())*.5f;}

	bool SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3] )						;	// renvoie TRUE si segment coupe un poly
	bool SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3], PolygonU3D* &pres )	;	// renvoie TRUE si le segment coupe un poly du monde et renvoie l'intersection dans fin et le poly touché dans pres !

	void Affiche( Mat3x4& m, Ufloat RVBA[4] )	;	// affiche TOUT le contenu de l'element

};
////////////////////////////////////////



////////////////////////////////////////
// l'arbre lui meme
class BTree{

//----------- Datas
private:

	BTreeElem*	allFeuille	;
	BTreeElem*	tree		;

//----------- Fonctions
private:

	void		KillTree( BTreeElem* liste )			;
	BTreeElem*	SubDivise( BTreeElem* liste, U32 axe )	;

public:

	BTree()		;
	~BTree()	;

	void AddFeuille( BTreeElem *elem )			;
	void Construct( PolygonU3D *tab, U32 nb )	;	// construit l'arbre

	bool SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3] )						;	// renvoie TRUE si segment coupe un poly
	bool SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3], PolygonU3D* &pres )	;	// renvoie TRUE si le segment coupe un poly du monde et renvoie l'intersection dans fin et le poly touché dans pres !

	void Affiche( Mat3x4& m )	;	// affiche en fonction de la matrice m l'arbre de boundings box

};
////////////////////////////////////////

#endif
