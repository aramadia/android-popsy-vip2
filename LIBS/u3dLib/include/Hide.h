    				 /*-----------------------------------------------------+
					  |				         Hide.h							|
					  |														|
					  |	une classe toute simple ki va g�rer les hide keys	|
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
	U32		nbKey			;	// nombre de cl�
	U32		actualNumKey	;	// numero cl� actuel
	Ufloat	timePos			;	// position dans le temps
	Ufloat	*Keys			;	// toutes les cl�s

//-------------- Fonctions
public:

	HideU3D( HideKeyFileU3D *h, U32 nbh )	;
	~HideU3D()								;

	void Ini()				;	// remet la parcour de la spline au debut
	void SetPos( Ufloat p )	;	// met la courbe a une position donn�e
	void IncPos( Ufloat n )	;	// incr�mente position dans la spline de n valeur

	bool IsHide()	{return HideStatus;}

};
/////////////////////////////////////////////


#endif
