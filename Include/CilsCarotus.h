	   				 /*-------------------------------------------------------------------------+
					  |								CilsCarotus.h								|
					  |																			|
					  |	Pourkoa carotus me direz-vous ? ben en fait paskeu c'est l'image de la  |
					  |				molecule de du pigment de la carote vala !					|
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#ifndef _CILSCAROTUS_VIP2_H
#define _CILSCAROTUS_VIP2_H

/////////////////////////////////////////
// 1 seule boule 
class CilsCarotus{

//--------------- Datas
private:

	UImage* carotus1	;	bool Bcarotus1	;
	UImage* carotus2	;	bool Bcarotus2	;
	UImage* carotus3	;	bool Bcarotus3	;
	UImage* carotus4	;	bool Bcarotus4	;

//--------------- Focntions
public:


	CilsCarotus()	;
	~CilsCarotus()	;

	void Go()	;

	void UpdateFrame( Ufloat laptime )	;
};
/////////////////////////////////////////

#endif
