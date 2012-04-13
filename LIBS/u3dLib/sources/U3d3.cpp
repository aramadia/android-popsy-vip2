					 /*-----------------------------------------------------+
					  |				          U3D3.h					    |
					  |														|
					  |	 C'est le Header de ma plateforme de developpement  |
					  |		  de U3D3 ( moteur 3d et gestion de moult		|
					  |           Truc pour faire de la bonne demo)		    |
					  |											            |
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------
//								+------------------------------+
//								|    Les Variables globales    |
//								+------------------------------+
//----------------------------------------------------------------------------------------------------------


UManager		*U3D3Manager=NULL			;
MondeU3D		*U3D3Monde3D=NULL			;
//TextureManager	*U3D3TexManager=NULL		;
DataManager		*U3D3DataManager=NULL		;
PolyPipeLine	*U3D3Pipeline=NULL			;
#ifdef _LMAPS
LightmapManager *U3D3LightMapManager=NULL	;
#endif



//----------------------------------------------------------------------------------------------------------
//									+---------------------+
//									|    Les Fonctions    |
//									+---------------------+
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void IniEnvironmentU3D3( Ufloat x, Ufloat y )
{
	Xres = x;
	Yres = y;

	U3D3Monde3D = NULL								;
	U3D3Manager = new UManager						;
	TextureManager::Initialise()					;
	U3D3DataManager = new DataManager				;
	U3D3Pipeline = new PolyPipeLine( U3D3Manager )	;

#ifdef _LMAPS
	U3D3LightMapManager = new LightmapManager; 
#endif

	Build_OneOnSqrtTab()	;	// from MathMacro.h
}
//----------------------------------------------------------------------------------------------------------
void KillEnvironmentU3D3()
{
	TextureManager::Release()	;
	if( U3D3Manager ) {delete U3D3Manager; U3D3Manager=NULL;}
	if( U3D3DataManager ) {delete U3D3DataManager; U3D3DataManager=NULL;}
	if( U3D3Pipeline ) {delete U3D3Pipeline; U3D3Pipeline=NULL;} 

#ifdef _LMAPS
	delete U3D3LightMapManager; 
#endif

}
//----------------------------------------------------------------------------------------------------------
// renvoie l'ancien monde3d
MondeU3D *SetActualMondeU3D( MondeU3D *monde )
{
	MondeU3D *tmp	;

	tmp = U3D3Monde3D	;
	U3D3Monde3D = monde ;

	return tmp	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie l'ancien manager
UManager *SetActualManagerU3D( UManager *manager )
{
	UManager *tmp	;

	tmp = U3D3Manager		;
	U3D3Manager = manager	;

	return tmp	;
}
//----------------------------------------------------------------------------------------------------------
