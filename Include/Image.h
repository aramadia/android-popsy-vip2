    				 /*-----------------------------------------------------------------+
					  |							Image.h									|
					  |																	|
					  |	une classe pour gérer l'affiche d'images ( c'est tout con koa )	|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/

#ifndef _IMAGE_VIP2_H
#define _IMAGE_VIP2_H


//////////////////////////////////////////
class UImage{

//-------------------- flags
private:

	enum{
		flagNone			= 0x000,
		flagDisable			= 0x001,
		flagNodetroyTex		= 0x002,
		interpoAlpha		= 0x004,
		interpoZx			= 0x008,
		interpoZy			= 0x010,
		interpoXpos			= 0x020,
		interpoYpos			= 0x040,
		interpoAngle		= 0x080,
		flagNoPipeset		= 0x100
	};

//-------------------- Datas
private:

	U32		FlagStatus			;	// status de l'image
	bool	ProportionRotation	;	// concerve les proportion pendnat la rotation

	TextureU3D	*imTex		;	// texture de l'image ( syen a une )
	Ufloat NaturalWidth		;	// veritable largeur de la texture ( car on peu utiliser une version low def de la texture pour les suckings 3dfx ! )
	Ufloat NaturalHeight	;	// veritable hauteur de la texture ( car on peu utiliser une version low def de la texture pour les suckings 3dfx ! )

	MaterialU3D	*imMat	;	// material de l'image
	BitmapU3D	*imBmp	;	// bitmap de l'image ( structure k'utilise le pipelin en fait )

	Ufloat	xpos,ypos			;	// position de l'image en coord unitaire
	Ufloat	destxpos,destypos	;
	Ufloat	incxpos,incypos		;

	Ufloat	dx,dy		;	// taille de l'image en coord unitaire

	Ufloat	zx,zy			;	// zoom de l'image
	Ufloat	destzx,destzy	;
	Ufloat	inczx,inczy		;

	Ufloat	alpha		;	// valeur alpha de l'image
	Ufloat	destalpha	;	// valeur de destination alpha
	Ufloat	incalpha	;	// incrément de l'alpha

	Ufloat angle		;	// angle de rotation de l'image
	Ufloat destangle	;
	Ufloat incangle		;

//-------------------- Fonctions
private:

	void Ini()																			;
	void CalcInterpo( Ufloat &data, Ufloat inc, Ufloat dest, U32 flag, Ufloat laptime )	;

public:

	UImage()																										;
	UImage( LPCSTR nomTex, U32 flagopenTex=BILINEAR_TEXTURE, Ufloat natWidth=0.0f, Ufloat natHeight = 0.0f )	;
	~UImage()																										;

	UImage&	operator=( UImage& ima )	;

	void OpenMask( LPCSTR dir )	;

	void Disable()	{FlagStatus|=flagDisable;}
	void Enable()	{FlagStatus&=~flagDisable;}

	void ConsProRota( bool b)	{ProportionRotation=b;}
	void PipeSet( bool b )		{b? FlagStatus&=~flagNoPipeset : FlagStatus|=flagNoPipeset;}

	Ufloat GetXpos()		{return xpos;}
	Ufloat GetYpos()		{return ypos;}
	Ufloat GetXSize()		{return dx*zx;}
	Ufloat GetYSize()		{return dy*zy;}
	Ufloat GetXZoom()		{return zx;}
	Ufloat GetYZoom()		{return zy;}

	MaterialU3D*	GetMaterial()	{return imMat;}
	BitmapU3D*		GetBitmap()		{return imBmp;}
	Ufloat			GetAlpha()		{return alpha;}

	void SetTexture( TextureU3D* tex, Ufloat natWidth=0.0f, Ufloat natHeight = 0.0f )	;	// set une texture et spécifie la taille kelel devrai avoir ( mais kelle a pas forcement a cause des 3dfx surtout )
	void SetPos( Ufloat x, Ufloat y )													{xpos=x;ypos=y;}
	void SetUpLeftPos( Ufloat x, Ufloat y )												;
	void SetZoom( Ufloat z )															{zx=z;zy=z;}
	void SetXZoom( Ufloat z )															{zx=z;}
	void SetYZoom( Ufloat z )															{zx=z;}
	void SetSize( Ufloat size )															;
	void SetXSize( Ufloat size )														;
	void SetYSize( Ufloat size )														;
	void SetTransp( U32 tr )															{imMat->SetTransp(tr);imMat->Confirm();}
	void SetAlpha( Ufloat a )															{alpha=a;}
	void SetAngle( Ufloat a )															{angle=a;}
	void SetColor( Ufloat r, Ufloat v, Ufloat b, Ufloat a=1.0f )						{imMat->SetColor(r,v,b,a);}
	void SetCoordRV( Ufloat u1, Ufloat u2, Ufloat v1, Ufloat v2 )						;	// donne les coord de la texture ( mais en valeur réel non unitaire )
	void SetCoord( Ufloat u1, Ufloat u2, Ufloat v1, Ufloat v2 )							;	// donne les coord de la texture ( ici valeur unitaire )

	void InterpoAlpha( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoXPos( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoYPos( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoXZoom( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoYZoom( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoZoom( Ufloat debut, Ufloat fin, Ufloat time )	;
	void InterpoAngle( Ufloat debut, Ufloat fin, Ufloat time )	;

	bool IsEnable()			{return FlagStatus&flagDisable ? FALSE : TRUE;}
	bool IsInterpoAlpha()	{return FlagStatus&interpoAlpha ? TRUE : FALSE;}
	bool IsInterpoXPos()	{return FlagStatus&interpoXpos ? TRUE : FALSE;}
	bool IsInterpoYPos()	{return FlagStatus&interpoYpos ? TRUE : FALSE;}
	bool IsInterpoAngle()	{return FlagStatus&interpoAngle ? TRUE : FALSE;}

	void Affiche( Ufloat laptime=1.0f )	;
};
//////////////////////////////////////////




//----------------------------------------------------------------------------------------------------------
//									+------------------------+
//									|  Les Fonctions Inline  |
//									+------------------------+
//----------------------------------------------------------------------------------------------------------
void inline UImage::CalcInterpo( Ufloat &data, Ufloat inc, Ufloat dest, U32 flag, Ufloat laptime )
{
	data += laptime*inc	;

	if( inc < 0.0f )
	{
		if( data < dest )
		{
			data = dest			;
			FlagStatus &= ~flag	;
		}
	}
	else
	{
		if( data > dest )
		{
			data = dest			;
			FlagStatus &= ~flag	;
		}
	}
}
//----------------------------------------------------------------------------------------------------------

#endif
