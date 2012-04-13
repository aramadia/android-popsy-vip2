    					 /*-----------------------------------------------------+
						  |				ObjStaticLighting.h						|
						  |														|
						  |  Une class pour stocker le lighting static d'un  	|
						  |				objet 3d poil de pied					|
						  |														|
						  | 	U2^PoPsy TeAm 2000								|
						  +-----------------------------------------------------*/

#ifndef _OBJSTATICLIGHTING_U3D3_H
#define _OBJSTATICLIGHTING_U3D3_H

////////////////////////////////////////
// une feuille de l'arbre en fait
class ObjStaticLighting{

//----------- Datas
private:

	Ufloat	*GouraudData	;	// tableau RVB !

//----------- Fonctions
public:

	ObjStaticLighting( ObjetU3D *obj )	;
	~ObjStaticLighting()				;

	void GetGouraud( CoordU3D *tab, U32 nb )	;
};
////////////////////////////////////////

#endif

