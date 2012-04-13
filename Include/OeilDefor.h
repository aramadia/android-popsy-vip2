	   			 /*---------------------------------------------------------------------+
				  |							OeilDefor.h									|
				  |																		|
				  |	 c'est une fake deformation de l'oeil 3D ( on va lui faire subir	|
				  |						les pires sévisses ).							|
				  |																		|
				  | 	U2^PoPsy TeAm 2000												|
				  +---------------------------------------------------------------------*/

#ifndef _OEILDEFOR_VIP2_H
#define _OEILDEFOR_VIP2_H

/////////////////////////////////////////
struct OeilDeforVertex{

	Ufloat	dest[3]		;
	Ufloat	origine[3]	;
	Ufloat	trans[3]	;
	Ufloat	alpha		;
	Ufloat	U,V			;

};
/////////////////////////////////////////



/////////////////////////////////////////
class OeilDefor{

//--------------- Datas
private:

	TextureU3D	*Tex	;	// texture de la fake image
	MaterialU3D	*Mat	;	// material de la fake image
	BitmapU3D	*Bmp	;	// bitmap utilisée de partout

	Camera*	cam	;	// un camera fictive ki nous permet de calculer une projection

	OeilDeforVertex*	AllVertex	;
	Ufloat				DivFact		;

	Ufloat Galpha		;
	Ufloat incAlpha		;
	Ufloat zmin,zmax	;

	bool calcAlpha	;
	bool enable		;

//--------------- Fonctions
private:

	void Projet()						;
	void AffAll()						;
	void CalcAlpha( Ufloat laptime )	;

public:

	OeilDefor()		;
	~OeilDefor()	;

	void OnDefoX(  Ufloat dec )			;
	void OnDefoY(  Ufloat dec )			;
	void OnDefoZoom(  Ufloat dec )		;	
	void OnCalcTunel( Ufloat time )		;	// time = temps de disparition

	void SetEnable( bool b )	{enable=b;}
	bool IsEnable()				{return enable;}

	void Goalpha()	{calcAlpha=TRUE;}

	void UpdateFrame( Ufloat laptime )	;
};
/////////////////////////////////////////

#endif
