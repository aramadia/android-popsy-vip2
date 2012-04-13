	   				 /*---------------------------------------------------------------------+
					  |							PlansAnim.h									|
					  |																		|
					  |		va faire bouger en hard coding tout ces merveilleux plans  		|
					  |		( PS : j'aime kan un plan se deroule sans accros ( Annibal ) )	|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#ifndef _PLANSANIM_VIP2_H
#define _PLANSANIM_VIP2_H

/////////////////////////////////////////
// encore une structure intérmediaire
struct PlansElem{

	ObjetU3D*	obj			;
	Ufloat		speedAvance	;
	Ufloat		zpos		;

	PlansElem*	suiv	;

	PlansElem()	{obj=NULL;speedAvance=0.0f;zpos=0.0f;suiv=NULL;}
};
/////////////////////////////////////////



/////////////////////////////////////////
class PlansAnim{

//--------------- Datas
private:

	PlansElem*	AllElem	;

//--------------- Fonctions
private:

	void AddObj( ObjetU3D* obj, bool posDir, Ufloat speed )	;

public:

	PlansAnim( MondeU3D* monde )	;
	~PlansAnim()					;

	void Anim( Ufloat laptime )	;

};
/////////////////////////////////////////

#endif
