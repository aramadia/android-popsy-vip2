    				 /*-----------------------------------------------------+
					  |				        World3DConv.h					|
					  |														|
					  |	  cette classe me permet de traiter + facilement    |
					  |	    les données a convertir entre des datas 3D 		|
					  |		 quelconques et le formet spécifik U3D		    |
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#ifdef TOOLU3D
#ifndef _WORLDCONV_U3D3_H
#define _WORLDCONV_U3D3_H


class MondeU3DConv{

	U32				nbobj		;
	ObjetU3DConv	*ObjList	;

	U32				nbcam		;
	CamU3DConv		*CamList	;

	U32				nblight		;
	LightU3DConv	*LightList	;

	U32	nbframe	;

	Ufloat AmbientColor[3]	;

public:

	MondeU3DConv()	;
	~MondeU3DConv()	;

	static MondeU3DConv *ActualMonde	;

	void AddElement(ObjetU3DConv *obj)					;
	void AddElement(mesh3ds *mesh)						;
	void AddElement(CamU3DConv *cam)					;
	void AddElement(camera3ds *cam, kfcamera3ds *kfcam)	;
	void AddElement(LightU3DConv *li)					;
	void AddElement(light3ds *li)						;

	void RemoveElement(ObjetU3DConv *obj)	;	// enleve element de la liste

	void ConvAnim( kfmesh3ds *anim )	;
	void ConvAnim( kfomni3ds *li )		;
	void ConvAnim( kfspot3ds *li )		;

	void SetNbFrame(U32 nb)									{nbframe=nb;}
	void SetAmbientColor( Ufloat r, Ufloat v, Ufloat b )	{vec3_set( AmbientColor, r, v, b );}

	ObjetU3DConv	*Get( LPCSTR cs )		;
	LightU3DConv	*GetLight( LPCSTR cs )	;
	U32				GetNBPolygon()			;	// renvoie le nombre de poly en du monde
	U32				GetNBVertices()			;	// renvoie le nombre de vertex en du monde
	U32				GetNBCoord()			;	// renvoie le nombre de coordonnées en du monde

	void Serialize( CArchive &ar )	;

};

#endif
#endif
