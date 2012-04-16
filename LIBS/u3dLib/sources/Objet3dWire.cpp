    				 /*---------------------------------------------------------+
					  |				     Objet3dWire.cpp						|
					  |															|
					  |		va permettre de gérer les arretes affichables		|
					  |						d'un poly							|
					  | On pourra aussi se servir de ces données pour faire		|
					  | des rendus cartoon ou de la lumière pseudo volumiques	|
					  |															|
					  | 	U2^PoPsy TeAm 2000									|
					  +---------------------------------------------------------*/

#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								+---------------------------+
//								|    Struct CoordObjWire	|
//								----------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
CoordObjWire::~CoordObjWire()
{
	PolyObjWireList*	tmp	;

	while( allpoly )
	{
		tmp = allpoly->suiv	;
		delete allpoly		;
		allpoly = tmp		;
	}
}
//----------------------------------------------------------------------------------------------------------
void CoordObjWire::AddPloly( PolygonU3D* p )
{
	PolyObjWireList*	tmp	;

	tmp = new PolyObjWireList	;
	tmp->poly = p				;
	tmp->suiv = allpoly			;
	allpoly = tmp				;
}
//----------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								+---------------------------+
//								|    Struct PolyObjWire	|
//								----------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
PolyObjWire::~PolyObjWire()
{
	delete [] allcoord	;
}
//----------------------------------------------------------------------------------------------------------
void PolyObjWire::Set( PolygonU3D* p )
{
	poly = p									;
	allcoord = new CoordObjWire*[ p->nbVertex ]	;
}
//----------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								+---------------------------+
//								|    Class LigneObjWire		|
//								----------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
LigneObjWire::LigneObjWire()
{
	pt1 = NULL	;
	pt2 = NULL	;

	gauche = NULL	;
	droit = NULL	;

	vec4_set( RVBA[0], 1.0f, 1.0f, 1.0f, 1.0f )		;
	vec4_set( RVBA[1], 1.0f, 1.0f, 1.0f, 1.0f )		;
}
//----------------------------------------------------------------------------------------------------------
//void LigneObjWire::Set( CoordU3D* p1, CoordU3D* p2, PolygonU3D* g )
void LigneObjWire::Set( CoordObjWire* p1, CoordObjWire* p2, PolygonU3D* g )
{
	pt1Tmp = p1	;
	pt2Tmp = p2	;
	gauche = g	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si c'est une outline 2D de l'objet
bool LigneObjWire::IsOutline()
{
	if( !droit ) return TRUE	;	// c'est toujours une outline dans ces cas la !

	U32 res = 0	;

	if( !gauche->IsHide() ) res++	;
	if( !droit->IsHide() )	res++	;

	if( res==1 )	return TRUE		;	// kan l'une des 2 face n'a pas le meme etat alors c'est une outline
	else			return FALSE	;
}
//----------------------------------------------------------------------------------------------------------
void LigneObjWire::AfficheVolume( MaterialU3D* mat, Ufloat pos[3], Ufloat taille )
{
	PolygonU3D* poly								;
	Ufloat		pos1[3],pos2[3]						;
	U32			frameNum = U3D3Monde3D->GetTick()	;


	//-------------- calcul coordonnée des new points
	vec3_sub( pos1, pt1->trans, pos )	;
	vec3_normalize( pos1 )				;
	vec3_mul( pos1, pos1, taille )		;
	vec3_add( pos1, pos1, pt1->trans )	;

	vec3_sub( pos2, pt2->trans, pos )	;
	vec3_normalize( pos2 )				;
	vec3_mul( pos2, pos2, taille )		;
	vec3_add( pos2, pos2, pt2->trans )	;



	//------------- alloue les vertex, coord et poly

	poly = U3D3Manager->NewPoly( 4 )				;
	poly->flag = PolyFLAG_TEMP						;
	vec4_set( poly->RVBA, 1.0f, 1.0f, 1.0f, 1.0f )	;
	poly->m = mat									;

	poly->Vtab[0] = U3D3Manager->NewVertex()		;
	poly->Vtab[0]->utilisateurs++					;
	poly->Vtab[0]->c = U3D3Manager->NewCoord()		;
	poly->Vtab[0]->c->utilisateurs++				;
	poly->Vtab[0]->U = 0.0f							;
	poly->Vtab[0]->V = 0.0f							;
	vec3_eg( poly->Vtab[0]->c->trans, pt1->trans )	;

	poly->Vtab[1] = U3D3Manager->NewVertex()		;
	poly->Vtab[1]->utilisateurs++					;
	poly->Vtab[1]->c = U3D3Manager->NewCoord()		;
	poly->Vtab[1]->c->utilisateurs++				;
	poly->Vtab[1]->U = 0.0f							;
	poly->Vtab[1]->V = 0.95f						;
	vec3_eg( poly->Vtab[1]->c->trans, pos1 )		;

	poly->Vtab[2] = U3D3Manager->NewVertex()		;
	poly->Vtab[2]->utilisateurs++					;
	poly->Vtab[2]->c = U3D3Manager->NewCoord()		;
	poly->Vtab[2]->c->utilisateurs++				;
	poly->Vtab[2]->U = 1.0f							;
	poly->Vtab[2]->V = 0.95f						;
	vec3_eg( poly->Vtab[2]->c->trans, pos2 )		;

	poly->Vtab[3] = U3D3Manager->NewVertex()		;
	poly->Vtab[3]->utilisateurs++					;
	poly->Vtab[3]->c = U3D3Manager->NewCoord()		;
	poly->Vtab[3]->c->utilisateurs++				;
	poly->Vtab[3]->U = 1.0f							;
	poly->Vtab[3]->V = 0.0f							;
	vec3_eg( poly->Vtab[3]->c->trans, pt2->trans )	;

	//--------- projette
	VertexU3D	**V = poly->Vtab	;
	for(U32 a=0; a<poly->nbVertex; a++,V++)
		U3D3Monde3D->GetActualCamera()->Projet( *V, frameNum )	;		// calcul coords 2D

	U3D3Pipeline->AddPolyDynaTemporary( poly )	;
}
//----------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								+---------------------------+
//								|    Class Objet3dWire		|
//								----------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
Objet3dWire::Objet3dWire()
{
	listeCoord = NULL	;
	tabPoly = NULL	;
	tabWire = NULL	;
	NbLigne = 0	;

	vec3_set( PosVolumeLight, 0.0f, 0.0f, 0.0f )	;
	TailleVolumeLight = 0.0f						;
}
//----------------------------------------------------------------------------------------------------------
Objet3dWire::~Objet3dWire()
{
	CoordObjWire*	tmpc	;

	while( listeCoord )
	{
		tmpc = listeCoord->suiv	;
		delete listeCoord		;
		listeCoord = tmpc		;
	}

	if( tabPoly ) delete [] tabPoly		;
	if( tabWire ) delete [] tabWire		;
}
//----------------------------------------------------------------------------------------------------------
CoordObjWire* Objet3dWire::AddPolyToCoord( CoordU3D* c, PolygonU3D* p )
{
	CoordObjWire*	tmpc	;

	tmpc = listeCoord	;
	while( tmpc )
	{
		if( tmpc->coord == c )
		{
			tmpc->AddPloly( p )	;
			return tmpc			;
		}
		tmpc = tmpc->suiv	;
	}

	//------------ pas trouvé dans la liste alors en créé un nouveau
	tmpc = new CoordObjWire( c )	;
	tmpc->AddPloly( p )				;
	tmpc->suiv = listeCoord			;
	listeCoord = tmpc				;

	return tmpc	;
}
//----------------------------------------------------------------------------------------------------------
void Objet3dWire::CreateTabs( ObjetU3D *obj )
{
	U32 a			;
	PolygonU3D*	tmpp;

	tabPoly = new PolyObjWire[ obj->nbpolys ]		;

	tmpp=obj->Ptab	;
	for( a=0; a<obj->nbpolys; a++,tmpp++ )
	{
		tabPoly[a].Set( tmpp )	;
		for( U32 b=0; b<tmpp->nbVertex; b++ )
			tabPoly[a].allcoord[b] =  AddPolyToCoord( tmpp->Vtab[b]->c, tmpp )	;
	}
}
//----------------------------------------------------------------------------------------------------------
void Objet3dWire::CleanTabs()
{
	LigneObjWire*	ligne	;
	CoordObjWire*	tmpc	;


	//--------- va ssigner les coordU3D au ligne au lie d'utiliser les CoordObjWire
	ligne = tabWire	;
	for( U32 a=0; a<NbLigne; a++,ligne++ )
	{
		ligne->SetPt1( ligne->GetPt1Tmp()->coord )	;
		ligne->SetPt2( ligne->GetPt2Tmp()->coord )	;
	}

	delete [] tabPoly	;	tabPoly = NULL	;

	//--------- detruit ensuite la liste des coord
	while( listeCoord )
	{
		tmpc = listeCoord->suiv	;
		delete listeCoord		;
		listeCoord = tmpc		;
	}
}
//----------------------------------------------------------------------------------------------------------
// TODO : optimiser la recherche du 2eme ki jouxte chaque ligne paskeu c'est lennnnnnnt !
//		  mieux vaut construire une table de chak poly ki touche chak sommet !!!!!!!!!
void Objet3dWire::Construct( ObjetU3D *obj )
{
	U32				a,b		;
	U32				count	;
//	PolygonU3D*		poly	;
	PolyObjWire*	poly	;
	LigneObjWire*	ligne	;


	objet = obj	;

	NbLigne = obj->GetNbArretes()			;
	tabWire = new LigneObjWire[ NbLigne ]	;

	CreateTabs( obj )	;

	//------------------------------ créé toutes les lignes
	poly = tabPoly	;
	count = 0		;
	for( a=0; a<obj->nbpolys; a++,poly++ )
	{
		for( b=0; b<poly->poly->nbVertex; b++ )
		{
			U32 n = b+1					;
			if( n==poly->poly->nbVertex ) n=0	;

			if( (poly->poly->arrete>>b)&0x1 )
			{
//				tabWire[ count ].Set( poly->Vtab[b]->c, poly->Vtab[n]->c, poly )	;
				tabWire[ count ].Set( poly->allcoord[b], poly->allcoord[n], poly->poly )	;
				count++																		;
			}
		}
	}


	//---------------------------- recherche le 2eme poly ki touche chak ligne
	ligne= tabWire	;
	for( a=0; a<NbLigne; a++,ligne++ )
	{
		PolyObjWireList *listp	;
		listp = ligne->GetPt1Tmp()->allpoly	;
		while( listp )
		{
			if( listp->poly != ligne->GetGauche() )
			{
				if( listp->poly->IsElement( ligne->GetPt1Tmp()->coord, ligne->GetPt2Tmp()->coord ) )
				{
					ligne->SetPolyDroit( listp->poly )	;
					break								;
				}
			}
			listp= listp->suiv	;
		}
/*		poly = obj->Ptab	;
		for( b=0; b<obj->nbpolys; b++,poly++ )
			if( poly!=ligne->GetGauche() )
			{
				if( poly->IsElement( ligne->GetPt1(), ligne->GetPt2() )	)
				{
					ligne->SetPolyDroit( poly )	;
					break						;
				}
			}*/
	}

	CleanTabs()	;
}
//----------------------------------------------------------------------------------------------------------
void Objet3dWire::SetLineColor( Ufloat rvba[4] )
{
	LigneObjWire*	ligne = tabWire	;

	for( U32 a=0; a<NbLigne; a++, ligne++ )
		ligne->SetColor( rvba )	;
}
//----------------------------------------------------------------------------------------------------------
// TODO : ben à finirrrrrrrr
void Objet3dWire::Affiche( U32 type, MaterialU3D *mat )
{
	U32				a				;
	LigneObjWire*	ligne = tabWire	;


	switch( type )
	{
		//--------------- affiche tout betement chak ligne
		case normal :
			for( a=0; a<NbLigne; a++, ligne++ )
				ligne->Affiche()	;
			break;

		//--------------- affiche seulement les outlines
		case cartoon :
			for( a=0; a<NbLigne; a++, ligne++ )
				if( ligne->IsOutline() )
					ligne->Affiche()	;
			break;

		//--------------- affiche des polys en outline en fonctions de la camera ( TMP !!! )
		case volumelight :
//			objet->MakeCulling( tmpP )	;
			for( U32 a=0; a<NbLigne; a++, ligne++ )
//				if( ligne->IsOutline() )
				if( !ligne->GetGauche()->IsHide() || !ligne->GetDroit()->IsHide() )
					ligne->AfficheVolume( mat,  PosVolumeLight, TailleVolumeLight )	;
			break;
	}
}
//----------------------------------------------------------------------------------------------------------
