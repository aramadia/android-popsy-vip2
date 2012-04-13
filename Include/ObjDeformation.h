	   			 /*---------------------------------------------------------------------+
				  |							ObjDeformation.h							|
				  |																		|
				  |	une classe pour faire un ki deformera un objet dans tout les sens	|
				  |																		|
				  | 	U2^PoPsy TeAm 2000												|
				  +---------------------------------------------------------------------*/

#ifndef _DEFOBJET_VIP2_H
#define _DEFOBJET_VIP2_H


//////////////////////////////////////////
// structure ou l'on stock des infos supplémentaire sur les vertex
struct DefObjVertex{

	CoordU3D*	coord		;
	Ufloat		cbase[3]	;	// coordonnée de base
	Ufloat		Vdef[3]		;	// vecteur de deformation
	Ufloat		dist		;	// distance depuis le centre de deformation
	Ufloat		fact		;	// facteur de deformation

};
//////////////////////////////////////////



//////////////////////////////////////////
class ObjDeformation{

//-------------------- Datas
private:

	ObjetU3D*	obj	;

	DefObjVertex*	AllVertex	;

	Ufloat amplitude	;
	Ufloat frequence	;

//-------------------- fonctions
private:
	
	void Sort(S32 deb, S32 fin)	;

public:

	ObjDeformation( ObjetU3D* obj, Ufloat amplitute=100.f, Ufloat freq=6.28f )	;
	~ObjDeformation()															;

	void Confirm()	;

	void Anim( Ufloat time )	;

};
//////////////////////////////////////////

#endif

