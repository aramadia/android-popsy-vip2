    				 /*---------------------------------------------------------+
					  |				      ColonneAnim.h							|
					  |															|
					  |	   va gérer l'animation de cette grandiose colonne		|
					  |															|
					  | 	U2^PoPsy TeAm 2000									|
					  +--------------------------------------------------------*/

#ifndef _COLONNEANIM_VIP2_H
#define _COLONNEANIM_VIP2_H

/////////////////////////////////////////
class ColonneAnim{

//--------------- Datas
private:

	OneObjBones* bone[8]	;

//--------------- Fonctions
public:

	ColonneAnim( MondeU3D* monde )	;
	~ColonneAnim()					;

	void Anim( Ufloat time, Ufloat laptime )	;
};
/////////////////////////////////////////

#endif
