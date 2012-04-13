	   				 /*-------------------------------------------------------------------------+
					  |								CilsBack.h									|
					  |																			|
					  |	affiche un fantastik background ki bouge et tout comme il fo waaaaaaaa  |
					  |																			|
					  | 	U2^PoPsy TeAm 2000													|
					  +-------------------------------------------------------------------------*/

#ifndef _CILSBACK_VIP2_H
#define _CILSBACK_VIP2_H

/////////////////////////////////////////
struct CilsRaycast{

	Ufloat posx,posy	;

	Ufloat	U,V		;
	Ufloat	U1,V1	;
};
/////////////////////////////////////////


/////////////////////////////////////////
class CilsBack{

//--------------- Datas
private:

	TextureU3D	*Tex	;	// texture du fond du raycasting
	TextureU3D	*Tex1	;	// texture du devant "			"
	MaterialU3D	*Mat	;	// material du raycast
	BitmapU3D	*Bmp	;	// bitmap utilisée de partout

	Camera*	cam ;

	CilsRaycast*	tab		;	// les rayons
	Ufloat			moveTun	;	// déplacement dans le tunnel

	bool	DisappearMode	;	// TRUE kan on est en mode de disparition
	Ufloat	Tdisappear		;	// temps de disparition
	Ufloat	ActuaTdisappear	;	// temps actuel pour la disparition


//--------------- Fonctions
private:

	void CalcRays()	;
	void AffRays()	;

public:

	CilsBack()	;
	~CilsBack()	;

	void Disappear( Ufloat time )	;	// va faire dispparaitre ce joli fond !

	void UpdateFrame( Ufloat time, Ufloat laptime )	;

};
/////////////////////////////////////////

#endif
