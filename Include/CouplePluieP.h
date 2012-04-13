					 /*---------------------------------------------------------------------+
					  |						CouplePluieP.h									|
					  |																		|
					  |la classe ki affiche de la pluie en polygones sur la scene en avant	|
					  |						et en arrière plan.								|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#ifndef _COUPLEPLUIEP_VIP2_H_
#define _COUPLEPLUIEP_VIP2_H_



//////////////////////////////////////////////
class GoutteP{

//------------------ Datas
private:

	Ufloat dir[3]	;
	Ufloat pos[3]	;
	Ufloat size		;
	Ufloat vitesse	;
	Ufloat alpha	;

	PolygonU3D* poly1 ;
	PolygonU3D* poly2 ;

//------------------ Fonctions
public:

	GoutteP()	;

	void Genere( Ufloat dir[3] )	;

	void Move( Ufloat step )	;

	bool IsFinish()		{if( alpha<=0.0f ) return TRUE; else return FALSE;}

	void	SetPoly( PolygonU3D* p1, PolygonU3D* p2  )	{poly1=p1;poly2=p2;}
	void	SetAlpha( Ufloat alph )						;
	Ufloat	GetAlpha()									{return alpha;}
	Ufloat*	GetPos()									{return pos;}

};
//////////////////////////////////////////////





//////////////////////////////////////////////
class PluieP{

//------------------ Datas
private:

	TextureU3D	*partTex	;	// image de la particule de base
	MaterialU3D	*partMat	;	// material de la particule de base

	GoutteP* Gouttes	;

	Ufloat dirGeneral[3]	;

	MondeU3D* Monde ;
	ObjetU3D* obj	;

//------------------ Fonctions
private:

	Ufloat	CalcAlpha( Ufloat y )										;
	void	AffVent( VentParticul* tmpv, Ufloat laptime, U32 nbPart )	;

public:

	PluieP( MondeU3D* monde )	;
	~PluieP()					;

	void IncAnim( Ufloat laptime )	;
};
//////////////////////////////////////////////


#endif
