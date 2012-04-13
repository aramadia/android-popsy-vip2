	   				 /*---------------------------------------------------------------------+
					  |							PlansClock.h								|
					  |																		|
					  |	C'est l'animation de toute cette jolie horloge "I Tek" (se prononce	|
					  |	 aille tèque ) Hard codé ke l'on retouve  dans cette partie			|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#ifndef _CLOCK_VIP2_H
#define _CLOCK_VIP2_H


/////////////////////////////////////////
// encore une structure intérmediaire
struct PlansClockElem{

	ObjetU3D*	obj			;
	Ufloat		speedRotate	;

	PlansClockElem*	suiv	;

	PlansClockElem()	{obj=NULL;speedRotate=0.0f;suiv=NULL;}
};
/////////////////////////////////////////



/////////////////////////////////////////
class PlansClockAnim{

//--------------- Datas
private:

	MondeU3D *Monde ;

	PlansClockElem*	AllElem	;

	Ufloat intensBouge		;

//--------------- Fonctions
private:

	void AddObj( ObjetU3D* obj, bool posAngle, Ufloat speed )	;

public:

	PlansClockAnim( MondeU3D* monde )	;
	~PlansClockAnim()					;

	void SetBouge( Ufloat val )	{intensBouge=val;}

	void Anim( Ufloat time )	;

};
/////////////////////////////////////////

#endif

