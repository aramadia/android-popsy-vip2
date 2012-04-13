	   				 /*-------------------------------------------------------------------------+
					  |							FinalTexte.h									|
					  |																			|
					  |		affichage des quelques derniers graphs de textes de la demo ho ho	|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#ifndef _FINALTEXTE_VIP2_H
#define _FINALTEXTE_VIP2_H

/////////////////////////////////////////
class FinalTexte{
	
//--------------- Datas
private:

	UImage* Url		;
	UImage* UrlAdd	;
	UImage* FScroll	;
	UImage*	Fondu	;

//--------------- Fonctions
public:

	FinalTexte()	;
	~FinalTexte()	;

	void GoUrl()	;
	void GoScroll()	;
	void GoFondu()	;

	void UpdateFrame( Ufloat laptime )	;
};
/////////////////////////////////////////

#endif

