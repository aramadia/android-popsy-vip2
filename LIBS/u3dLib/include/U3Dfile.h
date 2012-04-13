    				 /*-----------------------------------------------------+
					  |				         U3Dfile.h					    |
					  |														|
					  |  Ici on va décrire touts les éléments qui peuvent   |
					  |		intervenir dans la composition du format		|
					  |					 de fichier U3D						|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/


#ifndef _U3Dfile_U3D3_H
#define _U3Dfile_U3D3_H

//#define OK_valide 2000


//===================
// definitions des Chunks !
//	NOTES : les CHUNK sont en 16 bits

#define CHUNK_WOLRD		(U16)0x0006

	#define CHUNK_CAMERA	(U16)0x1000

	#define CHUNK_OBJ		(U16)0x2000
	#define CHUNK_OBJ_END	(U16)0x2fff

	#define CHUNK_LIGHT		(U16)0x3000
	#define CHUNK_LIGHT_END	(U16)0x3fff

#define CHUNK_WOLRD_END (U16)0x0fff


#endif
