		 /*-----------------------------------------------------+
		  |				         Video.h						|
		  |														|
		  | on s'occupe ici de la gestion des modes videos		|
		  | c'est a dire du passage dans tel ou tel mode etc..  |
		  |														|
		  | 	U2^PoPsy TeAm 1999								|
		  +-----------------------------------------------------*/

#ifndef _Video_U3D3_H
#define _Video_U3D3_H

void SetVideoW(U32 X, U32 Y);				// mode fenetre
// pour le mode fentre seulement!
// void SetWindowDC(HDC Hdc);

// Quit le mode video desire
void KillVideo();

extern U32  Xres,Yres;				// resolution X et Y demandee
extern U32  ABpp;					// nombre de bits par pixels

void AfficheFrameU3D()	;

#if 0
bool ChangeScreenResolution( U32 x, U32 y, U32 bitperpixel=0 )	;	// essaye de passer en resolution x:y avec le + de couleur possible
void RestoreScreenResolution()									;	// remet la resolution comme avant
#endif

#endif
