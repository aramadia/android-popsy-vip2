    			 /*-----------------------------------------------------+
				  |				         2Dstuff.h						|
				  |														|
				  | une véritable petite panoplies de fonctions bien	|
				  |			utiles pour la 2d poil de nez				|
				  |														|
				  | 	U2^PoPsy TeAm 1999								|
				  +-----------------------------------------------------*/


#ifndef _2Dstuff_U3D3_H
#define _2Dstuff_U3D3_H

// remplit ptr avec la texture tex
void AffTexture( U32 *ptr, U32 largeur, U32 hauteur, TextureU3D *tex, 
				 Ufloat debutX=0.0f, Ufloat finX=1.0f, Ufloat debutY=0.0f, Ufloat finY=1.0f )	;

// remplit ptr avec la composante alpha de tex
void AffAlphaTexture( U32 *ptr, U32 largeur, U32 hauteur, TextureU3D *tex,
				 Ufloat debutX=0.0f, Ufloat finX=1.0f, Ufloat debutY=0.0f, Ufloat finY=1.0f )	;

// remplit l'image avec une couleur spécifique
void AffCouleur( U32 *ptr, U32 largeur, U32 hauteur, U8 R, U8 V, U8 B )	;

// affiche une bitmap dans le DC
// void AffBitmapToDC( U32 *ptr, U32 largeur, U32 hauteur, HDC dc )		;


//--------------------- Fonction pour la création de mimap !
void Make24BitMimap( U8 *dest, U32 xdest, U32 ydest, U8 *src, U32 xsrc, U32 ysrc )		;
void Make32AlphaBitMimap( U8 *dest, U32 xdest, U32 ydest, U8 *src, U32 xsrc, U32 ysrc )	;


//------------------------ Petits outils de conversion : 

U16 inline BitConv24To16( U8 *b )	{ return 0x8000 | (b[0]>>3) | ((b[1]>>3)<<5) | ((b[2]>>3)<<10); }
U32 inline BitConv24To32( U8 *b )	{ return 0xff000000 | (b[0]) | (b[1]<<8) | (b[2]<<16); }

#endif
