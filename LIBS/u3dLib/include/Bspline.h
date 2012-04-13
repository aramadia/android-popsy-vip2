    				 /*-----------------------------------------------------+
					  |				         BSpline.h						|
					  |														|
					  |	 c'est a peut pres comme spline.h a ceci prés		|
					  |	 ke se sont des Bezier Spline ki sont utilisées ici	|
					  | ( la spline ne passe pas forcément par tout les pts	|
					  |						de controls ).					|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/

#ifndef _BSpline_U3D3_H
#define _BSpline_U3D3_H

//////////////////////////////////
class BSplineU3D{

//-------------------- Datas
private:

	U32		nbControls	;
	U32		nbDimension	;
	Ufloat*	tab			;

//-------------------- Fonctions
public:

	BSplineU3D( U32 nbC, U32 nbD )	;	// donne le nombre de control points et la dimension de la spline
	~BSplineU3D()					;

	Ufloat*	GetControlPt( U32 nb)				;
	void	GetVal( Ufloat*	dest, Ufloat pos )	;	// position dans la spline ( 0 < pos < 1 )
};
//////////////////////////////////

#endif
