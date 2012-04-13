	   			 /*---------------------------------------------------------------------+
				  |							EtoileGreets.h								|
				  |																		|
				  |	 les greets c'est bien design et ca permet de faire une jolie scene	|
				  |							voiliiiiii									|
				  |																		|
				  | 	U2^PoPsy TeAm 2000												|
				  +---------------------------------------------------------------------*/

#ifndef _ETOILEGREETS_VIP2_H
#define _ETOILEGREETS_VIP2_H

//////////////////////////////////////////
class EtoileGreets{

//-------------------- Datas
private:

/*	UImage* GreetsInter		;
	UImage* GreetsFrench	;*/
	UImage* Cote1			;
	UImage* Cote2			;

	UImage* Greets[24]	;

	Ufloat TDefil	;
	Ufloat Temps	;

//-------------------- fonctions
private:

	void AddImage( U32 num, LPCSTR nom, Ufloat posx, Ufloat posy )	;
	void DisableAll()												;

public:

	EtoileGreets( Ufloat timedefil )	;
	~EtoileGreets()						;

	void GoScreen( U32 num )	;	// balance un screen de greets

	void AfficheFore( Ufloat laptime )		;
};
//////////////////////////////////////////


#endif

