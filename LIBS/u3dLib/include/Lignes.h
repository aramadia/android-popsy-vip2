    				 /*-----------------------------------------------------+
					  |				         Lignes.h						|
					  |														|
					  |	 Toutes les chtites routines d'affichages de lignes |
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#ifndef _Lignes_U3D3_H
#define _Lignes_U3D3_H

// renvoie une ligne clippée 3D par la camera courante ( renvoie FALSE si ligne non affichable )
bool GetClippedLineByCamera( CoordU3D &ptdest1, CoordU3D &ptdest2 )	;

// renvoie une ligne clippée 2D par le viewport ( renvoie FALSE si ligne non affichable )
bool GetClippedLineByViewport( CoordU3D &ptdest1, CoordU3D &ptdest2, Viewport &view )	;

#endif
