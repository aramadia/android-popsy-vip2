	   			 /*---------------------------------------------------------------------+
				  |							NeuroLines.h								|
				  |																		|
				  |	classe ki consitera a afficher des lignes ki ricocherons sur la 	|
				  |					mesh pour faire un wiiiz effect						|
				  |																		|
				  | 	U2^PoPsy TeAm 2000												|
				  +---------------------------------------------------------------------*/

#ifndef _NEUROLINES_VIP2_H
#define _NEUROLINES_VIP2_H


//////////////////////////////////////////
// une structure ou je stock mes lignes et qq infos
struct NeuroOneLine{

	HSplinaeAff*	ligne		;
	bool			IsFinish	;
	Ufloat			TimeToGo	;
	Ufloat			TotalTime	;

	NeuroOneLine()	;
	~NeuroOneLine()	;
};
//////////////////////////////////////////




//////////////////////////////////////////
class NeuroLines{

//-------------------- Datas
private:

	NeuroOneLine*	AllLines	;

	MondeU3D*	monde	;

	MaterialU3D*	partMat	;
	BitmapU3D*		partBmp	;
	Ufloat*			partPos	;	// tableau de position des particules et de l'alpha

	Ufloat PosDep[3]	;
	Ufloat Rayonmax		;

//-------------------- fonctions
private:

	void AffParticul( CoordU3D &pos, Ufloat alpha=1.0f )	;

public:

	NeuroLines( MondeU3D *world, UImage* particul )	;
	~NeuroLines()										;

	void GenereLine( NeuroOneLine* sp )	;

	void UpdateFrame( Ufloat laptime )	;
};
//////////////////////////////////////////

#endif
