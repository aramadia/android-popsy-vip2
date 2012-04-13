					 /*---------------------------------------------------------------------+
					  |						CoupleTextes.h									|
					  |																		|
					  |	affichera des textes remplient d'une puissance philosophik encore	|
					  |						méconnue par l'homme .							|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#ifndef _COUPLETEXTES_VIP2_H_
#define _COUPLETEXTES_VIP2_H_


//////////////////////////////////////////////
class CoupleTextes{

//------------------ Datas
private:

	UImage* Sub		;
	UImage* Nihil	;
	UImage* Corres	;

	UImage* Revive	;
	UImage* Revive1	;

	UImage* Again						;
	Ufloat	timeAgain,totaltimeAgain	;
	bool	DisaAgain					;

//------------------ Fonctions
private:

	void GereXImage( UImage* ima, Ufloat laptime, bool right  )	;

public:

	CoupleTextes()	;
	~CoupleTextes()	;

	void GoCorres()				;
	void GoAgain( Ufloat time )	;

	void UpdateFrameBack( Ufloat time )			;
	void UpdateFrameFore( Ufloat time )			;
	void UpdateFRameFore1( Ufloat laptime )		;
	void UpdateFrameForeAll( Ufloat laptime )	;

};
//////////////////////////////////////////////

#endif
