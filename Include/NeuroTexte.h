    					 /*-----------------------------------------------------+
						  |				       NeroTexte.h						|
						  |														|
						  |	Affichage des giga paroles du chanteur de la chanson|
						  |														|
						  | 	U2^PoPsy TeAm 2000								|
						  +-----------------------------------------------------*/

#ifndef _NEUROTEXTE_VIP2_H
#define _NEUROTEXTE_VIP2_H

/////////////////////////////////////////
class NeuroTxt{

//--------------- Datas
private:

	TextureU3D*	texture1	;

	UImage*	There	;
	UImage*	Was		;
	UImage*	No		;
	UImage*	God		;
	UImage*	For		;
	UImage*	Me		;

	UImage* Allthat	;
	UImage* Shatters;
	UImage* Absense	;

	U32 SloganPos	;


//-------------- Fonctions
private:

	void Disappear( UImage* ima, Ufloat time )	;

	void OnShatters( Ufloat time )	;
	void OnAllthat( Ufloat time )	;
	void OnAbsense( Ufloat time )	;

public:

	NeuroTxt()	;
	~NeuroTxt()	;

	void OnThere()	;
	void OnWas()	;
	void OnNo()		;
	void OnGod()	;
	void OnFor()	;
	void OnMe()		;

	void NextSlogan()	;

	void Affiche( Ufloat laptime )	;
};
/////////////////////////////////////////

#endif
