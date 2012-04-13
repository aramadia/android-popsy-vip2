    				 /*-----------------------------------------------------+
					  |				         BTree.cpp						|
					  |														|
					  |    C'est un arbre de boundings box ki permettra 	|
					  |		des tests de collision rapides sur les meshes	|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------+
//									|    Class BTreeElem  |
//									+---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
BTreeElem::BTreeElem(PolygonU3D *p)
{
	poly = p	;

	if( p )	// on calcul la bounding box dans ce cas
	{
		for( U32 a=0; a<p->nbVertex; a++ )
			bounds.PointIn( p->Vtab[a]->c->origine )	;
	}

	suiv = NULL		;
	gauche = NULL	;
	droit = NULL	;
}
//----------------------------------------------------------------------------------------------------------
BTreeElem::~BTreeElem()
{
	if( gauche ) delete gauche	;
	if( droit ) delete droit	;
}
//----------------------------------------------------------------------------------------------------------
// devient la boite englobante de tout les elements passés dans la liste
void BTreeElem::SetSize( BTreeElem *liste )
{
	bounds.Reset()	;

	while( liste )
	{
		bounds += liste->bounds	;
		liste= liste->suiv		;
	}
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si majoritairement au dessus
bool BTreeElem::WicheXSide( Ufloat x )
{
	Ufloat cote = (bounds.GetMaxX() - x) + (bounds.GetMinX() - x)	;

	if( cote > 0.0f )	return TRUE		;
	else				return FALSE	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si majoritairement au dessus
bool BTreeElem::WicheYSide( Ufloat y )
{
	Ufloat cote = (bounds.GetMaxY() - y) + (bounds.GetMinY() - y)	;

	if( cote > 0.0f )	return TRUE		;
	else				return FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// renvoie TRUE si majoritairement au dessus
bool BTreeElem::WicheZSide( Ufloat z )
{
	Ufloat cote = (bounds.GetMaxZ() - z) + (bounds.GetMinZ() - z)	;

	if( cote > 0.0f )	return TRUE		;
	else				return FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// renvoie TRUE si segment coupe un poly
bool BTreeElem::SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3] )
{
	if( bounds.SegmentIntersec( debut, fin ) )
	{
		if( gauche )	if( gauche->SegmentIntersecPoly( debut, fin ) ) return TRUE	;
		if( droit )		if( droit->SegmentIntersecPoly( debut, fin ) )	return TRUE	;
		if( poly )		if( poly->Intersection( debut, fin ) )			return TRUE	;
	}

	return FALSE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// renvoie TRUE si le segment coupe un poly du monde et renvoie l'intersection dans fin et le poly touché dans pres !
bool BTreeElem::SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3], PolygonU3D* &pres )
{
	bool ret = FALSE	;

	if( bounds.SegmentIntersec( debut, fin ) )
	{
		if( gauche )	if( gauche->SegmentIntersecPoly( debut, fin, pres ) )	ret = TRUE	;
		if( droit )		if( droit->SegmentIntersecPoly( debut, fin, pres ) )	ret = TRUE	;
		if( poly )		
		{
			if( poly->IntersectionMod( debut, fin ) )
			{
				pres = poly	;
				ret = TRUE	;
			}
		}
	}

	return ret	;	
}
//----------------------------------------------------------------------------------------------------------------------------------------
// affiche TOUT le contenu de l'element
void BTreeElem::Affiche( Mat3x4& m, Ufloat RVBA[4] )
{
	Ufloat		tmprvbaG[4]	;
	Ufloat		tmprvbaD[4]	;
	BoundingBox	boxaff		;

	vec4_mul( tmprvbaG, RVBA, .9f )	;
	tmprvbaG[1] *= .9f				;
	tmprvbaG[2] *= .9f				;

	vec4_mul( tmprvbaD, RVBA, .9f )	;
	tmprvbaG[0] *= .9f				;
	tmprvbaG[1] *= .9f				;


	if( gauche ) gauche->Affiche( m, tmprvbaG )	;
	if( droit ) droit->Affiche( m, tmprvbaD )	;

	if( poly )	// n'affiche ke les boundings finales
	{
		boxaff.Set( bounds, m )	;
		boxaff.Affiche( RVBA )	;
	}
}
//----------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------+
//									|    Class BTree      |
//									+---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
BTree::BTree()
{
	allFeuille = NULL	;
	tree = NULL			;
}
//----------------------------------------------------------------------------------------------------------
BTree::~BTree()
{
	BTreeElem *tmp	;

	if( tree )
	{
		delete tree	;
//		KillTree( tree )	;
	}
	else
	{
		while( allFeuille )
		{
			tmp = allFeuille				;
			allFeuille = allFeuille->suiv	;
			delete tmp						;
		}
	}
}
//----------------------------------------------------------------------------------------------------------
void BTree::KillTree( BTreeElem* liste )
{
	if( !liste ) return	;

	KillTree( liste->gauche )	;
	KillTree( liste->droit )	;

	delete liste	;

}
//----------------------------------------------------------------------------------------------------------
void BTree::AddFeuille( BTreeElem *elem )
{
	elem->suiv = allFeuille	;
	allFeuille = elem		;
}
//----------------------------------------------------------------------------------------------------------
BTreeElem*	BTree::SubDivise( BTreeElem* liste, U32 axe )
{
	BTreeElem*	tmp,*tmpw		;
	Ufloat		val				;
	bool		gauche=FALSE	;
	bool		right=FALSE		;
	bool		sortie= FALSE	;
	U32			parcours=0		;


	if( !liste->suiv )	return liste	;	// on est arrivé au bout


	tmp = new BTreeElem		;
	tmp->SetSize( liste )	;

	if( !liste->suiv->suiv )	// cas particulier ou il ne reste plus ke 2 elements
	{
		tmp->gauche = liste			;
		tmp->droit = liste->suiv	;
		return tmp					;
	}

	while( !sortie )
	{
		switch( axe%3 )
		{
			case 0:
				val = tmp->GetMiddleX()	;
				while( liste )
				{
					tmpw = liste->suiv	;

					if( liste->WicheXSide( val ) )
					{
						liste->suiv = tmp->gauche	;
						tmp->gauche = liste			;
						gauche = TRUE				;
					}
					else
					{
						liste->suiv = tmp->droit	;
						tmp->droit = liste			;
						right = TRUE				;
					}
					liste = tmpw	;
				}
				break;

			case 1:
				val = tmp->GetMiddleY()	;
				while( liste )
				{
					tmpw = liste->suiv	;

					if( liste->WicheYSide( val ) )
					{
						liste->suiv = tmp->gauche	;
						tmp->gauche = liste			;
						gauche = TRUE				;
					}
					else
					{
						liste->suiv = tmp->droit	;
						tmp->droit = liste			;
						right = TRUE				;
					}

					liste = tmpw	;
				}
				break;

			default:
				val = tmp->GetMiddleZ()	;
				while( liste )
				{
					tmpw = liste->suiv	;

					if( liste->WicheZSide( val ) )
					{
						liste->suiv = tmp->gauche	;
						tmp->gauche = liste			;
						gauche = TRUE				;
					}
					else
					{
						liste->suiv = tmp->droit	;
						tmp->droit = liste			;
						right = TRUE				;
					}

					liste = tmpw	;
				}
				break;
		}

		// test pour voar si on a répartie la liste au moins 1 fois
		if( gauche && right )	
		{
			sortie = TRUE	;
		}
		// sinon on recommence mais en changant d'axe 
		else
		{	
			if( gauche )	liste = tmp->gauche	;
			else			liste = tmp->droit	;

			gauche = FALSE		;
			right = FALSE		;
			tmp->gauche = NULL	;
			tmp->droit = NULL	;

			parcours++		;
			if( parcours==3 ) // on a fait les 3 cas de figure on réparti donc le truc en 2 maintenant
			{
				sortie = TRUE	;

				while( liste )
				{
					tmpw = liste->suiv	;

					if( parcours&0x1 )
					{
						liste->suiv = tmp->gauche	;
						tmp->gauche = liste			;
					}
					else
					{
						liste->suiv = tmp->droit	;
						tmp->droit = liste			;
					}

					liste = tmpw	;
					parcours++		;
				}
			}
		}

		axe++			;	// change d'axe a chak nouvelle iteration
	}

	if( tmp->gauche ) tmp->gauche = SubDivise( tmp->gauche, axe )	;
	if( tmp->droit ) tmp->droit = SubDivise( tmp->droit, axe )		;

	return tmp	;
}
//----------------------------------------------------------------------------------------------------------
// construit l'arbre
void BTree::Construct( PolygonU3D *tab, U32 nb )
{
	U32			a	;
	PolygonU3D*	p	;

	//--------------- créé un nouvel élément a partir de chak poly
	p = tab	;
	for( a=0; a<nb; a++,p++ )
		AddFeuille( new BTreeElem( p ) )	;

	//-------------- construit l'arbre
	tree = SubDivise( allFeuille, 0 )	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si segment coupe un poly
bool BTree::SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3] )
{
	return tree->SegmentIntersecPoly( debut, fin )	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si le segment coupe un poly du monde et renvoie l'intersection dans fin et le poly touché dans pres !
bool BTree::SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3], PolygonU3D* &pres )
{
	return tree->SegmentIntersecPoly( debut, fin, pres )	;
}
//----------------------------------------------------------------------------------------------------------
// affiche en fonction de la matrice m l'arbre de boundings box
void BTree::Affiche( Mat3x4& m )
{
	Ufloat RVBA[4]	;

	vec4_set( RVBA, 1.0f, 1.0f, 1.0f, 1.0f )	;

	if( tree )
	{
		U3D3Pipeline->Begin( (U32)MatTranspRien )	;

			tree->Affiche( m, RVBA )	;

		U3D3Pipeline->End( MatTranspRien )	;
	}
}
//----------------------------------------------------------------------------------------------------------
