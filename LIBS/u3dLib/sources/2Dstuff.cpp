		    				 /*-----------------------------------------------------+
							  |				         2Dstuff.h						|
							  |														|
							  |		des chtis utilitaures 2d ki vont bien			|
							  |														|
							  | 	U2^PoPsy TeAm 1999								|
							  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------------------------------------
//										+---------------------+
//										|    Les Fonctions    |
//										+---------------------+
//----------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------
void AffTexture( U32 *ptr, U32 largeur, U32 hauteur, TextureU3D *tex, 
				 Ufloat debutX, Ufloat finX, Ufloat debutY, Ufloat finY )
{
	Ufloat	dx,dy		;
	U8		*ptrDest	;
	U32		dep			;

	if( !tex ) return				;
	if( !tex->GetTexture() ) return	;

	ptrDest = (U8*)ptr																		;
	dx = (Ufloat)(tex->Width()-1)*(finX-debutX) / largeur									;
	dy = (Ufloat)(tex->Height()-1)*(finY-debutY) / hauteur								;
	dep = (U32)((tex->Width()-1)*debutX) + tex->Width()*(U32)((tex->Height()-1)*debutY)	;

	if( tex->IsAlphaTexture() )
	{
		dep *= 4	;

		for(U32 y=0; y<largeur; y++)
			for(U32 x=0; x<hauteur; x++,ptrDest+=4)
			{
				U32 ypos = dy*y										;
				U32 xpos = dx*x										;
				U32 pos  = dep+((ypos*tex->Width() + xpos)<<2)	;

				ptrDest[0] = tex->GetTexture()[ pos+2 ]	;
				ptrDest[1] = tex->GetTexture()[ pos+1 ]	;
				ptrDest[2] = tex->GetTexture()[ pos+0 ]	;
				ptrDest[3] = 0								;
			}
	}
	else
	{
		dep *= 3	;

		for(U32 y=0; y<largeur; y++)
			for(U32 x=0; x<hauteur; x++,ptrDest+=4)
			{
				U32 ypos = dy*y									;
				U32 xpos = dx*x									;
				U32 pos  = dep+(ypos*tex->Width() + xpos)*3	;

				ptrDest[0] = tex->GetTexture()[ pos+2 ]	;
				ptrDest[1] = tex->GetTexture()[ pos+1 ]	;
				ptrDest[2] = tex->GetTexture()[ pos+0 ]	;
				ptrDest[3] = 0								;
			}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
// remplit ptr avec la composante alpha de tex
void AffAlphaTexture( U32 *ptr, U32 largeur, U32 hauteur, TextureU3D *tex, Ufloat debutX, 
					  Ufloat finX, Ufloat debutY, Ufloat finY )
{
	Ufloat	dx,dy		;
	U8		*ptrDest	;
	U32		dep			;

	if( !tex ) return				;
	if( !tex->GetTexture() ) return	;

	ptrDest = (U8*)ptr																		;
	dx = (Ufloat)(tex->Width()-1)*(finX-debutX) / largeur									;
	dy = (Ufloat)(tex->Height()-1)*(finY-debutY) / hauteur								;
	dep = (U32)((tex->Width()-1)*debutX) + tex->Width()*(U32)((tex->Height()-1)*debutY)	;

	if( tex->IsAlphaTexture() )
	{
		dep *= 4	;
		for(U32 y=0; y<largeur; y++)
			for(U32 x=0; x<hauteur; x++,ptrDest+=4)
			{
				U32 ypos = dy*y										;
				U32 xpos = dx*x										;
				U32 pos  = dep+((ypos*tex->Width() + xpos)<<2)	;

				ptrDest[0] = tex->GetTexture()[ pos+3 ]	;
				ptrDest[1] = tex->GetTexture()[ pos+3 ]	;
				ptrDest[2] = tex->GetTexture()[ pos+3 ]	;
				ptrDest[3] = 0								;
			}
	}
	else
	{
		dep *= 3	;
		for(U32 y=0; y<largeur; y++)
			for(U32 x=0; x<hauteur; x++,ptrDest+=4)
			{
				U32 ypos = dy*y									;
				U32 xpos = dx*x									;
				U32 pos  = dep+(ypos*tex->Width() + xpos)*3	;

				ptrDest[0] = tex->GetTexture()[ pos+3 ]	;
				ptrDest[1] = tex->GetTexture()[ pos+3 ]	;
				ptrDest[2] = tex->GetTexture()[ pos+3 ]	;
				ptrDest[3] = 0								;
			}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void Make24BitMimap( U8 *dest, U32 xdest, U32 ydest, U8 *src, U32 xsrc, U32 ysrc )
{
	U32		R,V,B,memp								;
	U32		factX = xsrc / xdest					;
	U32		factY = ysrc / ydest					;
	Ufloat	MulFact = (Ufloat)1.f / (factX*factY)	;

	for( U32 a=0; a<ydest; a++ )
	{
		for( U32 b=0; b<xdest; b++,dest+=3,src+=factX*3 )
		{
			R = 0	;	V = 0	;	B = 0	;
			for( U32 j=0; j<factY; j++ )
				for( U32 i=0; i<factX; i++ )
				{
					memp = (j*xsrc + i)*3	;
					R += src[ memp + 0 ]	;
					V += src[ memp + 1 ]	;
					B += src[ memp + 2 ]	;
				}

			dest[0] = R*MulFact	;
			dest[1] = V*MulFact	;
			dest[2] = B*MulFact	;
		}
		if( factY> 1 )	src += xsrc*(factY-1)*3	;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void Make32AlphaBitMimap( U8 *dest, U32 xdest, U32 ydest, U8 *src, U32 xsrc, U32 ysrc )
{
	U32		R,V,B,A,memp							;
	U32		factX = xsrc / xdest					;
	U32		factY = ysrc / ydest					;
	Ufloat	MulFact = (Ufloat)1.f / (factX*factY)	;

	for( U32 a=0; a<ydest; a++ )
	{
		for( U32 b=0; b<xdest; b++,dest+=4,src+=factX*4 )
		{
			R = 0	;	V = 0	;	B = 0	;	A = 0	;
			for( U32 j=0; j<factY; j++ )
				for( U32 i=0; i<factX; i++ )
				{
					memp = ((j*xsrc + i)<<2)	;
					R += src[ memp + 0 ]		;
					V += src[ memp + 1 ]		;
					B += src[ memp + 2 ]		;
					A += src[ memp + 3 ]		;
				}

			dest[0] = R*MulFact	;
			dest[1] = V*MulFact	;
			dest[2] = B*MulFact	;
			dest[3] = A*MulFact	;
		}
		if( factY> 1 )	src += xsrc*(factY-1)*4	;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------

