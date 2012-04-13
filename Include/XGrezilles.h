	   			 /*---------------------------------------------------------------------+
				  |				         XGrezilles.h									|
				  |																		|
				  |			affichage d'un léger fond  pour la scene des neuronnes		|
				  |																		|
				  | 	U2^PoPsy TeAm 2000												|
				  +---------------------------------------------------------------------*/

#ifndef _XGREZILLE_VIP2_H
#define _XGREZILLE_VIP2_H


/////////////////////////////////////////
// un pic du grezillement
class XGrezillePic{

//--------------- Datas
private:

	Ufloat	pos		;	// position en Y en fait
	Ufloat	speed	;	// vitesse du mouvement
	Ufloat	size	;
	Ufloat	alpha	;	// niveau de transparence du bidule
	bool	left	;	// est a gauche ou a droite

//--------------- Fonctions
public:

	void set( Ufloat pos, Ufloat speed, Ufloat size, Ufloat alpha, bool left )	;

	void Affiche( Ufloat laptime, BitmapU3D *bmp, MaterialU3D *mat, Ufloat dx, Ufloat dy, Ufloat Galpha )	;

};
/////////////////////////////////////////




/////////////////////////////////////////
class XGrezille{

//--------------- Datas
private:

	static TextureU3D	*TexPic1	;	// image de la particule de base
	static MaterialU3D	*MatPic1	;	// material de la particule de base

	static BitmapU3D	*GBmp		;	// bitmap utilisée de partout

	Ufloat	Alpha		;	// transparence du tout
	Ufloat	IncAlpha	;
	Ufloat	GotoAlpha	;

	U32				nbPic1	;
	XGrezillePic*	Pic1	;

//--------------- Fonctions
public:

	static void Initialise()	;
	static void Release()		;

	XGrezille( U32 nbpic )	;
	~XGrezille()			;

	void SetAlpha( Ufloat a )										{Alpha = a;GotoAlpha=a;IncAlpha=0.0f;}
	void Interposalpha( Ufloat debut, Ufloat fin, Ufloat time )	;

	void UpdateFrame( Ufloat laptime )	;
};
/////////////////////////////////////////

#endif
