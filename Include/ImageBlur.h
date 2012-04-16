    				 /*---------------------------------------------------------------------+
					  |							ImageBlur.h									|
					  |																		|
					  | Affiche plusieurs fois la meme image mais avec decalage pour faire	|
					  |					un giga effet de blur ki tue !						|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#ifndef _IMAGEBLUR_VIP2_H
#define _IMAGEBLUR_VIP2_H


//////////////////////////////////////////
class UImageBlur{

//-------------------- Datas
private:

	UImage**	tab			;	// toute les image
	U32			nbBlur		;	// nombre d'images de blurs

	Ufloat	factAlpha		;	// facteur alpha
	Ufloat	facttimeAlpha	;	// facteur de temps ( tempsBase * facttime = temps total d'interpolation pour le + petit niveau de blur )
	Ufloat	facttimeXPos	;	// facteur de temps ( tempsBase * facttime = temps total d'interpolation pour le + petit niveau de blur )
	Ufloat	facttimeYPos	;	// facteur de temps ( tempsBase * facttime = temps total d'interpolation pour le + petit niveau de blur )
	Ufloat	facttimeXZoom	;	// facteur de temps ( tempsBase * facttime = temps total d'interpolation pour le + petit niveau de blur )
	Ufloat	facttimeYZoom	;	// facteur de temps ( tempsBase * facttime = temps total d'interpolation pour le + petit niveau de blur )
	Ufloat	facttimeAngle	;	// facteur de temps ( tempsBase * facttime = temps total d'interpolation pour le + petit niveau de blur )

//-------------------- Fonctions
public:


	UImageBlur( U32 nbBlur, LPCSTR nomTex, U32 flagopenTex=BILINEAR_TEXTURE, Ufloat natWidth=0.0f, Ufloat natHeight = 0.0f  )	;
	~UImageBlur()																													;

	void Disable()	;
	void Enable()	;

	void ConsProRota( bool b)	;

	Ufloat GetXpos()		{return tab[0]->GetXpos();}
	Ufloat GetYpos()		{return tab[0]->GetYpos();}
	Ufloat GetXSize()		{return tab[0]->GetXSize();}
	Ufloat GetYSize()		{return tab[0]->GetYSize();}
	Ufloat GetXZoom()		{return tab[0]->GetXZoom();}
	Ufloat GetYZoom()		{return tab[0]->GetYZoom();}

	void SetFactAlpha( Ufloat a )		{factAlpha=a;}
	void SetFacttimeAlpha( Ufloat a )	{facttimeAlpha=a;}
	void SetFactXpos( Ufloat a )		{facttimeXPos=a;}
	void SetFactYpos( Ufloat a )		{facttimeYPos=a;}
	void SetFactXZoom( Ufloat a )		{facttimeXZoom=a;}
	void SetFactYZoom( Ufloat a )		{facttimeYZoom=a;}
	void SetFactAngle( Ufloat a )		{facttimeAngle=a;}

	void SetPos( Ufloat x, Ufloat y )													;
	void SetUpLeftPos( Ufloat x, Ufloat y )												;
	void SetZoom( Ufloat z )															;
	void SetXZoom( Ufloat z )															;
	void SetYZoom( Ufloat z )															;
	void SetSize( Ufloat size )															;
	void SetXSize( Ufloat size )														;
	void SetYSize( Ufloat size )														;
	void SetTransp( U32 tr )															;
	void SetAlpha( Ufloat a )															;
	void SetAngle( Ufloat a )															;
	void SetColor( Ufloat r, Ufloat v, Ufloat b, Ufloat a=1.0f )						;
	void SetCoordRV( Ufloat u1, Ufloat u2, Ufloat v1, Ufloat v2 )						;	// donne les coord de la texture ( mais en valeur réel non unitaire )
	void SetCoord( Ufloat u1, Ufloat u2, Ufloat v1, Ufloat v2 )							;	// donne les coord de la texture ( ici valeur unitaire )

	void InterpoAlpha( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoXPos( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoYPos( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoXZoom( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoYZoom( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoZoom( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoAngle( Ufloat debut, Ufloat fin, Ufloat time )	;

	bool IsEnable()			{return tab[0]->IsEnable();}
	bool IsInterpoAlpha()	;
	bool IsInterpoXPos()	;
	bool IsInterpoYPos()	;
	bool IsInterpoAngle()	;

	void Affiche( Ufloat laptime=1.0f )	;
};
//////////////////////////////////////////


#endif

