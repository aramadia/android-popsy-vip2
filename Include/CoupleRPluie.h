					 /*-----------------------------------------------------------------+
					  |						CoupleRPluie.h								|
					  |																	|
					  |	 une classe pour gérer les rebond de la pluie sur le couple		|
					  |	( enfin un truc ki simule :) inspiré de Metal Gear Solid2 )		|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/

#ifndef _COUPLERPLUIE_VIP2_H_
#define _COUPLERPLUIE_VIP2_H_


//////////////////////////////////////////////
// une structure ki definit un émeteur de bruine
struct BruineEmet{

	Ufloat pos[3]	;
	Ufloat dir[3]	;

	BruineEmet* suiv	;

	BruineEmet() {suiv=NULL;}
};
//////////////////////////////////////////////


//////////////////////////////////////////////
class BruineParticul{

//------------------ Datas
private:

	Ufloat	Pos[3]		;
	Ufloat	vitessDep	;

	Ufloat	size		;
	Ufloat  incsize		;

	Ufloat alpha	;
	Ufloat incalpha	;

	BruineEmet*	parent	;

//------------------ Fonctions
public:

	BruineParticul()	;

	void Genere( BruineEmet* emet )	;

	void Update( Ufloat laptime )	;

	bool		IsFinish()	{if( alpha==0.0f ) return TRUE; else return FALSE;}
	Ufloat		GetSize()	{return size;}
	Ufloat		GetAlpha()	{return alpha;}
	Ufloat*		GetPos()	{return Pos;}	
	BruineEmet*	GetParent()	{return parent;}
};
//////////////////////////////////////////////




//////////////////////////////////////////////
// la class ki gere le tout
class CoupleRPluie{

//------------------ Datas
private:

	TextureU3D	*partTex	;	// image de la particule de base
	MaterialU3D	*partMat	;	// material de la particule de base
	BitmapU3D	*partBmp	;	// bitmpa de la particule

	MondeU3D* Monde ;

	U32				NbGenerators	;
	BruineEmet*		AllEmet			;
	BruineParticul*	AllBruinePart	;

	BruineEmet* nextEmet	;

//------------------ Fonctions
private:

	void AddGeneratorIfGood( ObjetU3D* obj, CoordU3D* coord )	;
	void Genere( BruineParticul* bpart )						;
	void AffOnePart( BruineParticul* bpart )					;

public:

	CoupleRPluie( MondeU3D *monde )	;
	~CoupleRPluie()					;

	void UpdateFrame( Ufloat laptime )	;
};
//////////////////////////////////////////////


#endif

