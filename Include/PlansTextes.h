	   				 /*---------------------------------------------------------------------+
					  |							PlansTextes.h								|
					  |																		|
					  |			affichage de textes dans la partie des pllans voiliii		|
					  |																		|
					  | 	U2^PoPsy TeAm 2000												|
					  +---------------------------------------------------------------------*/

#ifndef _PLANSTEXTES_VIP2_H
#define _PLANSTEXTES_VIP2_H

/////////////////////////////////////////
class PlansTextes{

//--------------- Datas
private:

	UImage*	DStaff		;	bool BDStaff		;
	UImage*	Programing	;	bool BPrograming	;
	UImage*	Gfx2d		;	bool BGfx2d			;
	UImage*	Gfx3d		;	bool BGfx3d			;
	UImage*	Music		;	bool BMusic			;
	UImage*	Tuo			;	bool BTuo			;
	UImage*	U2			;	bool BU2			;
	UImage*	Ghell		;	bool BGhell			;
	UImage*	Madman		;	bool BMadman		;
	UImage*	Taggy		;	bool BTaggy			;
	UImage*	FFactory	;	bool BFFactory		;

//--------------- Fonctions
public:

	PlansTextes( Ufloat time )	;
	~PlansTextes()				;

	void OnDisappear( Ufloat time )	;

	void OnAppearPrograming( Ufloat time )		;
	void OnDisappearPrograming( Ufloat time )	;

	void OnAppearGfx2d( Ufloat time )		;
	void OnDisappearGfx2d( Ufloat time )	;

	void OnAppearGfx3d( Ufloat time )		;
	void OnDisappearGfx3d( Ufloat time )	;

	void OnAppearMusic( Ufloat time )		;
	void OnDisappearMusic( Ufloat time )	;

	void OnAppearTuo( Ufloat time )			;
	void OnDisappearTuo( Ufloat time )		;

	void OnAppearU2( Ufloat time )			;
	void OnDisappearU2( Ufloat time )		;

	void OnAppearMadman( Ufloat time )		;
	void OnDisappearMadman( Ufloat time )	;

	void OnAppearGhell( Ufloat time )		;
	void OnDisappearGhell( Ufloat time )	;

	void OnAppearTaggy( Ufloat time )		;
	void OnDisappearTaggy( Ufloat time )	;

	void OnAppearFFactory( Ufloat time )	;
	void OnDisappearFFactory( Ufloat time )	;

	void UpdateFrame( Ufloat laptime )	;
};
/////////////////////////////////////////


#endif

