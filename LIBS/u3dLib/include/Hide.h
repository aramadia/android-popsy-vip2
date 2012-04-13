    				 /*-----------------------------------------------------+
					  |				         Hide.h							|
					  |														|
					  |	une classe toute simple ki va gérer les hide keys	|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/


#ifndef _Hide_U3D3_H
#define _Hide_U3D3_H


/////////////////////////////////////////////
class HideU3D{

//-------------- Datas
private:

	bool	HideStatus		;
	U32		nbKey			;	// nombre de clé
	U32		actualNumKey	;	// numero clé actuel
	Ufloat	timePos			;	// position dans le temps
	Ufloat	*Keys			;	// toutes les clés

//-------------- Fonctions
public:

	HideU3D( HideKeyFileU3D *h, U32 nbh )	;
	~HideU3D()								;

	void Ini()				;	// remet la parcour de la spline au debut
	void SetPos( Ufloat p )	;	// met la courbe a une position donnée
	void IncPos( Ufloat n )	;	// incrémente position dans la spline de n valeur

	bool IsHide()	{return HideStatus;}

};
/////////////////////////////////////////////


#endif
