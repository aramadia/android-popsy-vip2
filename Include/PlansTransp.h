	   				 /*---------------------------------------------------------------------+
					  |							PlansTransp.h								|
					  |																		|
					  |	Affichage de la scene principale en additif par dessus tout le reste|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#ifndef _PLANSTRANSP_VIP2_H
#define _PLANSTRANSP_VIP2_H


/////////////////////////////////////////
class PlansTransp{

//--------------- Datas
private:

	MondeU3D* Monde	;

	Camera*	cam		;

	Ufloat	Transp		;
	Ufloat	GotoTransp	;
	Ufloat	IncTransp	;

//--------------- Fonctions
public:

	PlansTransp( MondeU3D* monde )	;

	void SetTransp( Ufloat tr, Ufloat time )	;

	void Affiche( Ufloat laptime )	;

};
/////////////////////////////////////////

#endif
