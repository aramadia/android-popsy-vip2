    				 /*-----------------------------------------------------------------+
					  |							Image.cpp								|
					  |																	|
					  |	une classe pour gérer l'affiche d'images ( c'est tout con koa )	|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/

#include "stdafx.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-------------------+
//										|    Class UImage	|
//										+-------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
UImage::UImage()
{
	Ini()	;
}
//----------------------------------------------------------------------------------------------------------
UImage::UImage( LPCSTR nomTex, U32 flagopenTex, Ufloat natWidth, Ufloat natHeight )
{
	Ini()	;

	TextureManager::SetRepertoire( "" )						;
	imTex = TextureManager::OpenJpg( nomTex, flagopenTex  )	;
	imTex->IncUser()										;
	imMat->SetTexture1( imTex )								;

	if( natWidth != 0.0f )	NaturalWidth = natWidth			;
	else					NaturalWidth = imTex->Width()	;
	if( natHeight != 0.0f )	NaturalHeight = natHeight		;
	else					NaturalHeight = imTex->Height()	;

	dx = (Ufloat)NaturalWidth / DEFAULT_SCREEN_X	;
	dy = (Ufloat)NaturalHeight / DEFAULT_SCREEN_Y	;
}
//----------------------------------------------------------------------------------------------------------
UImage::~UImage()
{
	delete imMat	;
	delete imBmp	;

	if( imTex )	if( !(FlagStatus&flagNodetroyTex) )	TextureManager::SubUse( imTex )	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::Ini()
{
	FlagStatus = flagNone		;
	ProportionRotation = TRUE	;
	
	xpos = .5f		;	ypos = .5f		;
	destxpos = .5f	;	destypos = .5f	;
	incxpos = 0.0f	;	incypos = 0.0f	;

	dx = 256.f / DEFAULT_SCREEN_X	;
	dy = 256.f / DEFAULT_SCREEN_Y	;

	zx = 1.0f		;	zy = 1.0f		;
	destzx = 1.0f	;	destzy = 1.0f	;
	inczx = 0.0f	;	inczy = 0.0f	;

	alpha = 1.0f		;
	destalpha = 1.0f	;
	incalpha = 0.0f		;

	angle = 0.0f		;
	destangle = 0.0f	;
	incangle = 0.0f		;

	imTex = NULL			;
	NaturalWidth = 0.0f		;
	NaturalHeight = 0.0f	;

	imMat = new MaterialU3D( "Dima" )	;
	imBmp = new BitmapU3D( imMat )		;
}
//----------------------------------------------------------------------------------------------------------
UImage&	UImage::operator=( UImage& ima )
{
	FlagStatus = ima.FlagStatus	;

	*imMat = *ima.imMat												;
	*imBmp = *ima.imBmp												;
	SetTexture( ima.imTex, ima.NaturalWidth, ima.NaturalHeight )	;

	xpos = ima.xpos			;	ypos = ima.ypos			;
	destxpos = ima.destxpos	;	destypos = ima.destypos	;
	incxpos = ima.incxpos	;	incypos = ima.incypos	;

	dx = ima.dx	;	dy = ima.dy	;

	zx = ima.zx			;	zy = ima.zy			;
	destzx = ima.destzx	;	destzy = ima.destzy	;
	inczx= ima.inczx	;	inczy = ima.inczy	;

	alpha = ima.alpha			;
	destalpha = ima.destalpha	;
	incalpha = ima.incalpha		;

	return *this	;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void UImage::OpenMask( LPCSTR dir )
{
	if( imTex )	
	{
		if( !TextureManager::OpenMask( imTex, dir ) )
		{
			ErrorMess( "Impossible to Open mask %s !! Fucking Fucker.", dir)	;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// set une texture et spécifie la taille kelel devrai avoir ( mais kelle a pas forcement a cause des 3dfx surtout )
void UImage::SetTexture( TextureU3D* tex, Ufloat natWidth, Ufloat natHeight )
{
	if( imTex )	if( !(FlagStatus&flagNodetroyTex) )	TextureManager::SubUse( imTex )	;

	imTex = tex						;
	imMat->SetTexture1( imTex )		;

	if( natWidth != 0.0f )	NaturalWidth = natWidth			;
	else					NaturalWidth = imTex->Width()	;
	if( natHeight != 0.0f )	NaturalHeight = natHeight		;
	else					NaturalHeight = imTex->Height()	;

	dx = (Ufloat)NaturalWidth / DEFAULT_SCREEN_X	;
	dy = (Ufloat)NaturalHeight / DEFAULT_SCREEN_Y	;

	FlagStatus |= flagNodetroyTex	;	// dit ke l'on ne détruira pas la texture ( puisk'on ne la pas allouée )
}
//----------------------------------------------------------------------------------------------------------
void UImage::SetUpLeftPos( Ufloat x, Ufloat y )
{
	xpos = x + GetXSize()*.5f	;
	ypos = y + GetYSize()*.5f	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::SetSize( Ufloat size )
{
	zx = size / dx	;
	zy = size / dy	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::SetXSize( Ufloat size )
{
	zx = size / dx	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::SetYSize( Ufloat size )
{
	zy = size / dy	;
}
//----------------------------------------------------------------------------------------------------------
// donne les coord de la texture ( mais en valeur réel pas unitaire )
void UImage::SetCoordRV( Ufloat u1, Ufloat u2, Ufloat v1, Ufloat v2 )
{
	Ufloat	u1Uni,u2Uni	;
	Ufloat	v1Uni,v2Uni	;

	dx = ( (Ufloat)fabs( u2 - u1 )+1.0f ) / DEFAULT_SCREEN_X	;
	dy = ( (Ufloat)fabs( v2 - v1 )+1.0f	) / DEFAULT_SCREEN_Y	;

	u1Uni = u1 / ( NaturalWidth - 1.0f )	;
	u2Uni = u2 / ( NaturalWidth - 1.0f )	;
	v1Uni = v1 / ( NaturalHeight - 1.0f )	;
	v2Uni = v2 / ( NaturalHeight - 1.0f )	;

	imBmp->SetMapCoord( 0, u1Uni, v1Uni )	;
	imBmp->SetMapCoord( 1, u2Uni, v1Uni )	;
	imBmp->SetMapCoord( 2, u2Uni, v2Uni )	;
	imBmp->SetMapCoord( 3, u1Uni, v2Uni )	;
}
//----------------------------------------------------------------------------------------------------------
// donne les coord de la texture ( ici valeur unitaire )
void UImage::SetCoord( Ufloat u1, Ufloat u2, Ufloat v1, Ufloat v2 )
{
	dx = ( (Ufloat)fabs( u2 - u1 )*NaturalWidth ) / DEFAULT_SCREEN_X	;
	dy = ( (Ufloat)fabs( v2 - v1 )*NaturalHeight ) / DEFAULT_SCREEN_Y	;

	imBmp->SetMapCoord( 0, u1, v1 )	;
	imBmp->SetMapCoord( 1, u2, v1 )	;
	imBmp->SetMapCoord( 2, u2, v2 )	;
	imBmp->SetMapCoord( 3, u1, v2 )	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::InterpoAlpha( Ufloat debut, Ufloat fin, Ufloat time )
{
	alpha = debut						;
	destalpha = fin						;
	incalpha = ( fin - debut ) / time	;

	FlagStatus |= interpoAlpha	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::InterpoXPos( Ufloat debut, Ufloat fin, Ufloat time )
{
	xpos = debut						;
	destxpos = fin						;
	incxpos = ( fin - debut ) / time	;

	FlagStatus |= interpoXpos	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::InterpoYPos( Ufloat debut, Ufloat fin, Ufloat time )
{
	ypos = debut						;
	destypos = fin						;
	incypos = ( fin - debut ) / time	;

	FlagStatus |= interpoYpos	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::InterpoXZoom( Ufloat debut, Ufloat fin, Ufloat time )
{
	zx = debut						;
	destzx = fin					;
	inczx = ( fin - debut ) / time	;

	FlagStatus |= interpoZx	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::InterpoYZoom( Ufloat debut, Ufloat fin, Ufloat time )
{
	zy = debut						;
	destzy = fin					;
	inczy = ( fin - debut ) / time	;

	FlagStatus |= interpoZy	;
}
//----------------------------------------------------------------------------------------------------------
void UImage::InterpoZoom( Ufloat debut, Ufloat fin, Ufloat time )
{
	InterpoXZoom( debut, fin, time )	;
	InterpoYZoom( debut, fin, time )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImage::InterpoAngle( Ufloat debut, Ufloat fin, Ufloat time )
{
	angle = debut						;
	destangle = fin						;
	incangle = ( fin - debut ) / time	;

	FlagStatus |= interpoAngle	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void UImage::Affiche( Ufloat laptime )
{
	Ufloat dx2	;
	Ufloat dy2	;

	if( FlagStatus&flagDisable ) return	;

	if( FlagStatus&interpoAlpha )	CalcInterpo( alpha, incalpha, destalpha, interpoAlpha, laptime )	;
	if( FlagStatus&interpoXpos )	CalcInterpo( xpos, incxpos, destxpos, interpoXpos, laptime )		;
	if( FlagStatus&interpoYpos )	CalcInterpo( ypos, incypos, destypos, interpoYpos, laptime )		;
	if( FlagStatus&interpoZx )		CalcInterpo( zx, inczx, destzx, interpoZx, laptime )				;
	if( FlagStatus&interpoZy )		CalcInterpo( zy, inczy, destzy, interpoZy, laptime )				;
	if( FlagStatus&interpoAngle )	CalcInterpo( angle, incangle, destangle, interpoAngle, laptime )	;

	dx2 = dx * zx * .5f	;
	dy2 = dy * zy * .5f	;

	if( (imMat->GetTrTex1()==MatTranspMulAlpha) ||
		(imMat->GetTrTex1()==MatTranspIncrust) )
	{
		imMat->SetColor( alpha, alpha, alpha, alpha )	;
	}
	else
	{
		imMat->SetAlpha( alpha )	;
	}

	if( !(FlagStatus&flagNoPipeset) )	U3D3Pipeline->Begin( imMat )	;

		if( angle==0.0f )
		{
			imBmp->SetCoord( 0, xpos - dx2, ypos - dy2 )	;
			imBmp->SetCoord( 1, xpos + dx2, ypos - dy2 )	;
			imBmp->SetCoord( 2, xpos + dx2, ypos + dy2 )	;
			imBmp->SetCoord( 3, xpos - dx2, ypos + dy2 )	;
		}
		else
		{
			Ufloat p1x,p1y	;
			Ufloat p2x,p2y	;
			Ufloat p3x,p3y	;
			Ufloat p4x,p4y	;

			if( ProportionRotation )
			{
				Ufloat COS = cos( angle )		;
				Ufloat SIN = sin( angle )		;

				p1x = -dx2 * COS + -dy2 * SIN	;
				p1y = -dx2 * -SIN + -dy2 * COS	;

				p2x = dx2 * COS + -dy2 * SIN	;
				p2y = dx2 * -SIN + -dy2 * COS	;

				p3x = dx2 * COS + dy2 * SIN	;
				p3y = dx2 * -SIN + dy2 * COS	;

				p4x = -dx2 * COS + dy2 * SIN	;
				p4y = -dx2 * -SIN + dy2 * COS	;
			}
			else
			{
				Ufloat longueur = (float)sqrt( dx2*dx2 + dy2*dy2 )	;

				p1x = longueur*(float)cos( angle+M_PI*.75f )	;
				p1y = longueur*(float)sin( angle+M_PI*.75f )	;

				p2x = longueur*(float)cos( angle+M_PI*.25f )	;
				p2y = longueur*(float)sin( angle+M_PI*.25f )	;

				p3x = longueur*(float)cos( angle-M_PI*.25f )	;
				p3y = longueur*(float)sin( angle-M_PI*.25f )	;

				p4x = longueur*(float)cos( angle-M_PI*.75f )	;
				p4y = longueur*(float)sin( angle-M_PI*.75f )	;
			}

			imBmp->SetCoord( 0, xpos + p1x, ypos + p1y )	;
			imBmp->SetCoord( 1, xpos + p2x, ypos + p2y )	;
			imBmp->SetCoord( 2, xpos + p3x, ypos + p3y )	;
			imBmp->SetCoord( 3, xpos + p4x, ypos + p4y )	;
		}

		imBmp->Affiche()	;

	if( !(FlagStatus&flagNoPipeset) )	U3D3Pipeline->End( imMat )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
