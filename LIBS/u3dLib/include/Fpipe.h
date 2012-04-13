    				 /*-----------------------------------------------------+
					  |				         Fpipe.h						|
					  |														|
					  |	ne voyez aucun jeu de mot avec le nom de ce fichier |
					  |	    c'est simplement la gestion du Pipeline de 		|
					  |    l'affichage des faces dans un ordre correct !	|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/


#ifndef _Fpipe_U3D3_H
#define _Fpipe_U3D3_H

#define DEFAULT_MAXSORTEDPOLY 3500

//==============================
// definition des materials de pipeline

class MatPipeline{

	TextureU3D	*Tex1			;
	U32			Transparence1	;
	TextureU3D	*Tex2			;
	U32			Transparence2	;

	PolygonU3D *liste	;	// liste des polys de ce type de material lors du rendu
	bool	   DynaAdd	;	// est-ce ke ce material est utilisé dans cette frame ?

	S32	 utilisateurs	;	// nombre d'utilisateurs de l'instance de la classe

public:

	MatPipeline *suiv		;	// suivant dans la liste generale
	MatPipeline *preced		;	// precedent dans la liste generale
	MatPipeline *PipeSuiv	;	// suivant dans le rendu de pipelin actuel

	MatPipeline(MaterialU3D *m)	;	// créé un pipeline compatible avec ce material

	void IncUser()							{utilisateurs++;}
	void DecUser()							{utilisateurs--;}
	void SetDynaAdd()						{DynaAdd=TRUE;}
	void SetRenderState()					;	// SetOpenGL pour rendre le material comme il fo
	void SetRenderState( MatPipeline *mp )	;	// SetOpenGL pour rendre le material comme il fo ( mais en tenant compte des state deja setté par le precedent ! )
	void SetTranspOn2ndStage()				;	// met les good render states pour la combinaison entre les 2 texture en multitexturing

	S32		GetUse()						{return utilisateurs;}
	bool	IsGood( MaterialU3D *mat )		;
	bool	IsDynaAdd()						{return DynaAdd;}
	bool	IsNoTexture()					;
	bool	Is1Texture()					;
	bool	Is2Texture()					;
	bool	IsTransparent()					;
	bool	IsOpaque()						;
	bool	IsCompatible( MatPipeline *mp )	;	// renvoie TRUE si les 2 material on les render state compatible

	void AddPoly( PolygonU3D *p )	{p->suiv = liste; liste=p;}

	void Affiche()	;
	void Release()	;

};




struct PolySort{
	Ufloat		z	;
	PolygonU3D*	p	;
};


/////////////////////////////////////////////
// un ensemble de flags et de données ki influences les pipeline
struct PipelineState{

	Viewport	viewport	;
	Ufloat		AlphaRef	;	// référence Alpha pour les Alpha tests
	bool		SortPolyTr	;	// est-ce ke l'on trie les polygones ?

	PipelineState* suiv	;	// le pipeline est capable de foutre tout ca dans sa pile de flag
};
/////////////////////////////////////////////


/////////////////////////////////////////////
// le pipeline du moteur 1 une piece maitresse ki gere tout l'affichage
class PolyPipeLine{

	MatPipeline	*AllMatPipeline		;	// tout les MatPipeline sont ici

	//--------------------- les differents type de pipeline trié par materials lors du rendu
	MatPipeline *NoMapPipeLine		;
	MatPipeline *UneMapPipeLine		;
	MatPipeline *UneMapOpaPipeLine	;
	MatPipeline *MultiMapPipeLine	;

	//--------------------- Liste de polys Speculars 
	PolygonU3D *Speculars	;

	//--------------------- Listes de polys en fil de fer
	PolygonU3D *PLinesRien		;	// sans shading
	PolygonU3D *PLinesAdditif	;	// avec shading additif
	PolygonU3D *PLinesMultipli	;	// avec shading multiplicatif

	U32			MaxPolySort	;
	PolySort	*listeTr	;	// liste de poly qu'il fo trier
	U32			nbTrPoly	;

	//---------------------- un Poly Static pour affichage direct non trié
	MaterialU3D*	MStatic		;	// seulement un pointeur setté apres appele a un Begin
	PolygonU3D*		PStatic		;
	VertexU3D		VStatic[4]	;
	CoordU3D		CStatic[4]	;

	//---------------------- pile de flag sauvés
	PipelineState	*stackState	;

	//--------------------- Viewport par defaut
	Viewport ActuaViewport	;

	//--------------------- Variables diverses
	Ufloat	AlphaRef	;	// référence Alpha pour les Alpha tests
	bool	SortPolyTr	;	// est-ce ke l'on trie les polygones ?

	//---------------------- Variables de debuggings
#if defined(_DEBUG) || defined(TOOLU3D)
	U32	NBspeculars	;	// nombre de poly speculars rajoutés !
#endif


	void AddDynaMatPipe( MatPipeline *mp )	;	// indique au pipeline que ce material sera utilisé lors du rendu de cette frame

	void AddPolySpecular(PolygonU3D *p)	;
	void AddPolyTr(PolygonU3D *p)		;

	void Sort(S32 deb, S32 fin);

	void DefaultRenderState()	;	// met les renderstate par defaut

public:

	PolyPipeLine(UManager *m, U32 NbMaxSortedPolys=DEFAULT_MAXSORTEDPOLY)	;
	~PolyPipeLine()															;

	MatPipeline*	GetOrCreate( MaterialU3D *p )										;
	void			Release( MatPipeline *mp )											;
	void			Begin( MaterialU3D *mat, bool Zwrite=FALSE, bool Ztest=FALSE )		;	// met les render state en accord avec le material
	void			Begin( U32 transp, bool Zwrite=FALSE, bool Ztest=FALSE )			;	// met en place les render state ( mais pour une ligne )
	void			End( MaterialU3D *mat )												;	// clear render state

	Viewport&	GetActuaViewport()							{return ActuaViewport;}
	void		SetActuaViewport( const Viewport &view )	{ActuaViewport=view;}

	PolygonU3D*	AddPolyDyna(PolygonU3D	*p)				;	// retourne poly réellement ajouté au pipeline ( clipé et tout comme il fo )
	void		AddPolyDynaTemporary(PolygonU3D	*p )	;	// ajoute un polygone ki est forcément detruit apres affichage ! ( contrairement a AddPolyDyna ou l'on y met un poly ki "restera en vie" ).
	void		AddPolyLines(PolygonU3D *p)				;
	void		AddPolyLinesNoClip(PolygonU3D *p)		;

	//-------------------- modification de l'etat du pipeline
	static	void GetDefaultState( PipelineState &state )	;	// renvoie dans state les etats par defaut
			void PushState()								;	// sauve tout les etats du pipeline
			void PopState()									;	// retouve les etats
			void DefaultState()								;
			void SetState( PipelineState &state )			;
			void SetAlphaRef( Ufloat val )					{AlphaRef=val;}	// met une nouvelle valeur dans l'alpha reference
			void SortTransparentPoly( bool b )				{SortPolyTr=b;}

	void Affiche()																	;
	void Release()																	;
	void AfficheQuad( Ufloat XYZ[4][3], Ufloat RVBA[4][4], Ufloat UV[4][4] )		;	// affiche directement le poly static du pipeline
	void AfficheLigneFromEngine( CoordU3D *pt1, CoordU3D *pt2, Ufloat RVBA[2][4] )	;	// affiche directement une ligne ( coord depuis engine 3D )
	void AfficheLigne( Ufloat pt1[2], Ufloat pt2[2], Ufloat RVBA[2][4] )			;	// affiche une ligne en 2D ( ne projette pas les coordonnées



	//-------------------- Fonctions de debugging
#if defined(_DEBUG) || defined(TOOLU3D)
	U32	GetNBspeculars()	{return NBspeculars;}
#endif

};
/////////////////////////////////////////////




//----------------------------------------------------------------------------------------------------------
//									+------------------------+
//									|  Les Fonctions Inline  |
//									+------------------------+
//----------------------------------------------------------------------------------------------------------
void inline PolyPipeLine::AddPolyTr(PolygonU3D *p)
{
	listeTr[ nbTrPoly ].p = p			;
	listeTr[ nbTrPoly ].z = p->GetZ()	;
	nbTrPoly++							;

#if defined(_DEBUG) || defined(TOOLU3D)
	if( nbTrPoly >= MaxPolySort  ) FatalError( "Ya trop de polys a trier dédé!" )	;
#endif

}
//----------------------------------------------------------------------------------------------------------
void inline PolyPipeLine::AddPolySpecular(PolygonU3D *p)
{
	PolygonU3D	*np		;
	U32			a		;
	VertexU3D	**V		;
	bool		pass	;

	switch( p->m->GetShading() )
	{
		case MatShadingFlat:
			if( (p->RVBA[0]>1.0f) || (p->RVBA[1]>1.0f) || (p->RVBA[2]>1.0f) )
			{
				np = p->GetCopyPoly()	;
				#if defined(_DEBUG) || defined(TOOLU3D)
					NBspeculars++		;	
				#endif

				np->RVBA[0] -= 1.0f	;	if( np->RVBA[0]<0.0f ) np->RVBA[0] = 0.0f	;
				np->RVBA[1] -= 1.0f	;	if( np->RVBA[1]<0.0f ) np->RVBA[1] = 0.0f	;
				np->RVBA[2] -= 1.0f	;	if( np->RVBA[2]<0.0f ) np->RVBA[2] = 0.0f	;
			}
			else
			{
				return	;
			}
			break;


		case MatShadingGouraud:
			pass=FALSE	;
			V=p->Vtab	;
			for( a=0; a<p->nbVertex; a++,V++ )
			{
				if( ((*V)->c->RVBA[0]>1.0f) || ((*V)->c->RVBA[1]>1.0f) || ((*V)->c->RVBA[2]>1.0f) )
				{
					pass = TRUE	;
					break		;
				}
			}
			if( !pass ) return	;


			np = p->GetCopyPoly()	;
			#if defined(_DEBUG) || defined(TOOLU3D)
				NBspeculars++		;
			#endif
			V = np->Vtab			;
			for( a=0; a<np->nbVertex; a++,V++ )
			{
				(*V)->c->RVBA[0] -= 1.0f	;	if( (*V)->c->RVBA[0]<0.0f ) (*V)->c->RVBA[0] = 0.0f	;
				(*V)->c->RVBA[1] -= 1.0f	;	if( (*V)->c->RVBA[1]<0.0f ) (*V)->c->RVBA[1] = 0.0f	;
				(*V)->c->RVBA[2] -= 1.0f	;	if( (*V)->c->RVBA[2]<0.0f ) (*V)->c->RVBA[2] = 0.0f	;
			}
			break;


		default:
			return;
			break;
	}

	//------------------------- rajoute un petit decalage en Z pour ne pas avoir de defaut
	V = np->Vtab	;
	for( a=0; a<np->nbVertex; a++,V++ )
	{
		(*V)->c->ecran[3] += 1.f/65536.f	;
	}

	np->suiv = Speculars	;
	Speculars = np			;

}
//----------------------------------------------------------------------------------------------------------
void inline PolyPipeLine::AddPolyLinesNoClip(PolygonU3D *p)
{
	switch( p->m->GetTrTex1() )
	{
		case MatTranspAdd :
		case MatTranspAddAlpha :
			p->suiv = PLinesAdditif	;
			PLinesAdditif = p		;
			break;

		case MatTranspMul :
		case MatTranspMulAlpha :
			p->suiv = PLinesMultipli	;
			PLinesMultipli = p			;
			break;

		default:
			p->suiv = PLinesRien	;
			PLinesRien = p			;
			break;
	}
}
//----------------------------------------------------------------------------------------------------------


#endif
