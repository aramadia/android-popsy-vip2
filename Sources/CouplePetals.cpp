					 /*-----------------------------------------------------------------+
					  |						CouplePetals.h								|
					  |																	|
					  |	je vasi essayer de faire des petals de fleurs sur la 2eme scene	|
					  |				du couple, avec un chti doigt de physik				|
					  |																	|
					  | 	U2^PoPsy TeAm 2000											|
					  +-----------------------------------------------------------------*/

#include "stdafx.h"

#define FACT_SCENE_CM	5.f	// facteur de taille entre la scene et les unités utilisées

#define GRAVITE 25.0f	// en cm/s²

#ifdef _DEBUG
	#define NB_PETALS	200	// nombre de petals
#else
	#define NB_PETALS	200	// nombre de petals
#endif

#define PETALS_SIZE 10.f	// taille en cm

#define NB_VENT 10

#define NB_PETALS_3D 1000

#define NB_SHADOWPETALS  500 // doit etre inférieur ou egal a NB_PETALS_3D


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+---------------------------+
//										|  Class CoupleOnePetals	|
//										+---------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
CoupleOnePetals::CoupleOnePetals()
{
	Mass = 1.0f + 1.5f*Random1()			;
	vec3_set( Vitesse, 0.0f, 0.0f, 0.0f )	;
	vec3_set( Pos, 0.0f, 0.0f, 0.0f )		;
	vec3_set( Accel, 0.0f, 0.0f, 0.0f )		;

	Tappear	= 0.0f			;
	totalTappear = 0.0f		;
	Treste = 0.0f			;
	Tdisappear = 0.0f		;
	totalTdisappear = 0.0f	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleOnePetals::Ini( Ufloat tapp, Ufloat tres, Ufloat tdisa, Ufloat pos[3], Ufloat vitesse[3] )
{
	vec3_eg( Pos, pos )			;
	vec3_eg( Vitesse, vitesse )	;

	totalTappear = Tappear	= tapp			;
	Treste = tres							;
	totalTdisappear = Tdisappear = tdisa	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleOnePetals::IniAccel()
{
	vec3_set( Accel, 0.0f, -GRAVITE, 0.0f )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleOnePetals::AddForce( Ufloat force[3] )
{
	Ufloat tmp[3]	;

	vec3_div( tmp, force, Mass )	;
	vec3_add( Accel, Accel, tmp )	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleOnePetals::CalcPos( Ufloat laptime )
{
	Ufloat tmp[3] ;

	vec3_mul( Accel, Accel, laptime )	;
	vec3_add( Vitesse, Accel, Vitesse )	;	// calcul de la vitesse

	vec3_mul( tmp, Vitesse, laptime )	;
	vec3_add( Pos, Pos, tmp )			;	// calcul de la new position

	if( Tappear >=0.0f )
		Tappear -= laptime	;
	else if( Treste >=0.0f )
		Treste -= laptime	;
	else 
		Tdisappear -= laptime	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
Ufloat CoupleOnePetals::GetAlpha()
{
	if( Tappear>= 0.0f )		return 1.0f-Tappear/totalTappear	;
	if( Treste >= 0.0f )		return 1.0f							;
	return Tdisappear/totalTdisappear	;
}
//----------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+---------------------------+
//										|  Class CoupleSphereVent	|
//										+---------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
CoupleSphereVent::CoupleSphereVent( Ufloat inner, Ufloat outer, Ufloat puiss, Ufloat dir[3], Ufloat p[3] )
{
	outer2 = outer*outer	;
	inner2 = inner*inner	;
	vec3_eg( vec, dir )		;
	vec3_normalize( vec )	;
	puissance = puiss		;
	vec3_eg( pos, p )		;

	suiv = NULL	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleSphereVent::CalcInfluence( CoupleOnePetals* peta, U32 nb )
{
	Ufloat ppuiss[3]	;

	vec3_mul( ppuiss, vec, puissance )	;
	for( U32 a=0; a<nb; a++,peta++ )
	{
		Ufloat dist	;

		dist = vec3_distFake( pos, peta->GetPos() )	;

		if( dist>outer2 )	
			continue	;

		Ufloat val[3]	;
		vec3_sub( val, peta->GetPos(), pos )	;
		vec3_normalize( val )					;

		if( dist<inner2 )
		{
			vec3_mul( val, val, puissance )	;
		}
		else
		{
			vec3_mul( val, val,  puissance*(1.0f - (outer2 - dist) / ( outer2 - inner2 )) )	;
		}
		
		peta->AddForce( val )			;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CoupleSphereVent::CalcInfluence( CoupleOnePetals3D* peta, U32 nb )
{
	Ufloat ppuiss[3]	;

	vec3_mul( ppuiss, vec, puissance )	;
	for( U32 a=0; a<nb; a++,peta++ )
	{
		Ufloat dist	;

		dist = vec3_distFake( pos, peta->GetPos() )	;

		if( dist>outer2 )	
			continue	;

		Ufloat val[3]	;
		vec3_sub( val, peta->GetPos(), pos )	;
		vec3_normalize( val )					;

		if( dist<inner2 )
		{
			vec3_mul( val, val, puissance )	;
		}
		else
		{
			vec3_mul( val, val,  puissance*(1.0f - (outer2 - dist) / ( outer2 - inner2 )) )	;
		}
		
		peta->AddForce( val )			;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------+
//										|  Class CouplePetals	|
//										+-----------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
CouplePetals::CouplePetals( MondeU3D* monde )
{

	//-------------------- ini et load l'image d'une particul !
	TextureManager::SetRepertoire( "" )																				;
	partTex = TextureManager::OpenJpg( "Datas/meshes/Couple/petal.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE  )	;
	partTex->IncUser()																								;

	partMat	= new MaterialU3D( "partMat" )	;
	partMat->SetTexture1( partTex )			;
	partMat->SetTransp( MatTranspAddAlpha )	;
	partMat->SetShading( MatShadingFlat )	;
	partMat->Confirm()						;

	partBmp = new BitmapU3D( partMat )	;

	Tex3D = TextureManager::OpenJpg( "Datas/meshes/Couple/petal3D.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE|ALPHA_TEXTURE  )	;
	TextureManager::OpenMask( Tex3D, "Datas/meshes/Couple/petal3D.raw" )															;
	Tex3D->IncUser()																												;

	Mat3D = new MaterialU3D( "Tex3D" )	;
	Mat3D->SetTexture1( Tex3D )			;
	Mat3D->SetTransp( MatTranspAlpha )	;
	Mat3D->SetShading( MatShadingFlat )	;
	Mat3D->Side( TRUE )					;
	Mat3D->Confirm()					;

	TexShadow3D = TextureManager::OpenJpg( "Datas/meshes/Couple/shadow.jpg", BILINEAR_TEXTURE|TRILINEAR_TEXTURE  )	;
	TexShadow3D->IncUser()																								;

	MatShadow3D = new MaterialU3D( "TexShadow3D" )	;
	MatShadow3D->SetTexture1( TexShadow3D )			;
	MatShadow3D->SetTransp( MatTranspMulAlpha )		;
	MatShadow3D->SetShading( MatShadingFlat )		;
	MatShadow3D->Side( TRUE )						;
	MatShadow3D->Confirm()							;



	AllPetals = new	CoupleOnePetals[NB_PETALS];

	//------------------ genere le vent !!!!!!!!
	Ufloat dir[3] ={ 1.0f, 0.0f, .5f}	;
	Ufloat pos[3] ={ 0.0f, 0.0f, 0.0f}	;
	AllVents = NULL						;
//	AllVents = new CoupleSphereVent( 200.f, 500.f, 50.f, dir, pos ) ;	// rajoute le vent principal
	for( U32 a=0; a<NB_VENT; a++ )
	{
		vec3_set( dir, 2.f*Random1() - 1.f, 1.5f*Random1() - 0.5f, 2.f*Random1() - 1.f )	;
		vec3_set( pos, 300.f*Random1() - 150.f, 300.f*Random1(), 300.f*Random1() - 150.f )	;

		CoupleSphereVent* tmp = new CoupleSphereVent( 50.f + 20.f*Random1(), 200.f + 50.f*Random1(), 20.f + 10.f*Random1(), dir, pos )	;
		tmp->suiv = AllVents	;
		AllVents = tmp			;
	}


	//--------------- ini l'objet3D pour rendre les petals en 3d justement !
	ObjetU3D* obj = new ObjetU3D( "Petals", NB_PETALS_3D*6, NB_PETALS_3D*6, NB_PETALS_3D*2 )	;
	monde->AddElem( obj )																		;
	obj->SetRecalcFlag( 0 )																		;

	//-------- ini vertex et coordonnées de l'objet
	for( U32 a=0; a<obj->nbcoords; a++ )
	{
		obj->Ctab[a].utilisateurs = 1			;
		obj->Ctab[a].frame_process = (U32)-1	;

		obj->Vtab[a].c =  &obj->Ctab[a]	;	// assigne Coodonnée au vertex
		obj->Vtab[a].utilisateurs = 0	;
	}

	
	//--------------- ini l'objet3D pour rendre les pitites shadows
	ObjetU3D* obj1 = new ObjetU3D( "Shadows", NB_SHADOWPETALS*4, NB_SHADOWPETALS*4, NB_SHADOWPETALS )	;
	monde->AddElem( obj1 )																				;
	obj1->SetRecalcFlag( 0 )																			;

	//-------- ini vertex et coordonnées de l'objet
	for( U32 a=0; a<obj1->nbcoords; a++ )
	{
		obj1->Ctab[a].utilisateurs = 1			;
		obj1->Ctab[a].frame_process = (U32)-1	;

		obj1->Vtab[a].c =  &obj1->Ctab[a]	;	// assigne Coodonnée au vertex
		obj1->Vtab[a].utilisateurs = 0		;
	}



	//-------- va alloue et initialiser les petals 3D
	AllPetals3D	= new CoupleOnePetals3D[ NB_PETALS_3D ]	;

	PolygonU3D* poly = obj->Ptab	;
	VertexU3D* vert = obj->Vtab		;
	for( U32 a=0; a<NB_PETALS_3D; a++,poly+=2,vert+=6 )
		AllPetals3D[a].Set( poly, vert, Mat3D )	;



	//-------- va alloue et initialiser les shadows des petals 3D
	AllShadows = new CoupleShadowP3D[ NB_SHADOWPETALS ]	;

	poly = obj1->Ptab	;
	vert = obj1->Vtab	;
	for( U32 a=0; a<NB_SHADOWPETALS; a++,poly++,vert+=4 )
		AllShadows[a].Set( poly, vert, MatShadow3D, &AllPetals3D[a] )	;


}
//----------------------------------------------------------------------------------------------------------------------------------------
CouplePetals::~CouplePetals()
{
	CoupleSphereVent* tmpv	;

	//------------------ KIll image de la particul
	delete partBmp							;
	delete partMat							;
	delete Mat3D							;
	delete MatShadow3D						;
	TextureManager::SubUse( partTex )		;
	TextureManager::SubUse( Tex3D )			;
	TextureManager::SubUse( TexShadow3D )	;

	delete [] AllPetals		;
	delete [] AllPetals3D	;
	delete [] AllShadows	;

	//----------------- Kill le vent
	while( AllVents )
	{
		tmpv = AllVents->suiv	;
		delete AllVents			;
		AllVents = tmpv			;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CouplePetals::AffOneParti( Ufloat pos[3], Ufloat Size, Ufloat alpha )
{
	Camera*		cam = U3D3Monde3D->GetActualCamera()	;
	CoordU3D	tmpc									;
	Ufloat		size									;

	vec3_eg( tmpc.trans, pos )	;
	cam->ProjetUni( &tmpc, 0 )	;

	if( !(tmpc.ClipInfo&ClipProche) )
	{
		size = Size * tmpc.ecran[3] * .5f	;

		partMat->SetAlpha( alpha )	;

		partBmp->SetCoord( 0, tmpc.ecran[0] - size, tmpc.ecran[1] - size, tmpc.ecran[2] )	;
		partBmp->SetCoord( 1, tmpc.ecran[0] + size, tmpc.ecran[1] - size, tmpc.ecran[2] )	;
		partBmp->SetCoord( 2, tmpc.ecran[0] + size, tmpc.ecran[1] + size, tmpc.ecran[2] )	;
		partBmp->SetCoord( 3, tmpc.ecran[0] - size, tmpc.ecran[1] + size, tmpc.ecran[2] )	;

		partBmp->Affiche()	;
	}	
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CouplePetals::IncAnim3D( Ufloat time, Ufloat laptime )
{
	Ufloat	vent[3]	={ 1.0f, 0.0f, .5f}	;


	//-------------- ini les forces sur chak petal
	vec3_normalize( vent )					;
	vec3_mul( vent, vent, 60.f )			;
	CoupleOnePetals3D* tmpp = AllPetals3D	;
	for( U32 a=0; a<NB_PETALS_3D; a++,tmpp++ )
	{
		tmpp->IniAccel()		;
		tmpp->AddForce( vent )	;
	}

	//------------- calcul les forces ki s'appliquent sur chak petals
	CoupleSphereVent* tmpv = AllVents	;
	while( tmpv )
	{
		tmpv->CalcInfluence( AllPetals3D, NB_PETALS_3D )	;
		tmpv = tmpv->suiv									;
	}

	tmpp = AllPetals3D	;
	for( U32 a=0; a<NB_PETALS_3D; a++,tmpp++ )
	{
		if( tmpp->IsFinish() )
		{
			Ufloat	genpos[3]		;
			Ufloat	vecspeed[3]	;

			vec3_set( genpos, 300.f*Random1() - 150.f, 30.f*Random1(), 300.f*Random1() - 150.f )			;
			vec3_set( vecspeed, sin( time*5.f +a ), 1.f, cos( time*4.f +.2f +a  ) )						;
			vec3_normalize( vecspeed )																		;
			vec3_mul( vecspeed, vecspeed, 100.f + 30.f*cos(time*8.f +a ) )								;	// lache le truc a une vitesse voisine de 200 cm/s*/

			tmpp->Ini( .2f+ Random1()*.5f, 1.f + 3.f*Random1(), .5f + .5f*Random1(), genpos, vecspeed )	;
		}
		else
		{
			tmpp->IncAnim( laptime )	;
		}
	}

	//------------- anim shadows des petals
	CoupleShadowP3D* tmpShad = AllShadows	;
	for( U32 a=0; a<NB_SHADOWPETALS; a++,tmpShad++ )
		tmpShad->IncAnim( laptime )	;
	
}
//----------------------------------------------------------------------------------------------------------------------------------------
void CouplePetals::UpdateFrame( Ufloat time, Ufloat laptime )
{
	U32		a							;
	Ufloat	genpos[3]					;
	Ufloat	vecspeed[3]					;
	Ufloat	vent[3]	={ 1.0f, 0.0f, .5f}	;

	vec3_set( genpos, 300.f*Random1() - 150.f, 30.f*Random1(), 300.f*Random1() - 150.f )	;
	vec3_set( vecspeed, sin( time*5.f ), 1.f, cos( time*4.f +.2f  ) )						;
	vec3_normalize( vecspeed )																;
	vec3_mul( vecspeed, vecspeed, 100.f + 30.f*cos(time*8.f ) )							;


	//-------------- ini les forces sur chak petal
	vec3_normalize( vent )				;
	vec3_mul( vent, vent, 60.f )		;
	CoupleOnePetals* tmpp = AllPetals	;
	for( a=0; a<NB_PETALS; a++,tmpp++ )
	{
		tmpp->IniAccel()		;
		tmpp->AddForce( vent )	;
	}


	//------------- calcul les forces ki s'appliquent sur chak petals
	CoupleSphereVent* tmpv = AllVents	;
	while( tmpv )
	{
		tmpv->CalcInfluence( AllPetals, NB_PETALS )	;
		tmpv = tmpv->suiv							;
	}


	//--------------affichent les petals
	U3D3Pipeline->Begin( partMat, FALSE, TRUE )	;

		tmpp = AllPetals	;
		for( a=0; a<NB_PETALS; a++,tmpp++ )
		{
			Ufloat pos[3]	;

			if( tmpp->IsFinish() )
			{
				tmpp->Ini( .2f+ Random1()*.5f, 1.f + 3.f*Random1(), .5f + .5f*Random1(), genpos, vecspeed )	;

				vec3_set( genpos, 300.f*Random1() - 150.f, 30.f*Random1(), 300.f*Random1() - 150.f )		;
//				vec3_set( genpos, 10.f*cos( time+a ), 170.f + 20.f*sin( time*3.5f+.5+a), 10.f*sin(time+a) )	;
				vec3_set( vecspeed, sin( time*5.f +a ), 1.f, cos( time*4.f +.2f +a  ) )					;
				vec3_normalize( vecspeed )																	;
				vec3_mul( vecspeed, vecspeed, 100.f + 30.f*cos(time*8.f +a ) )							;	// lache le truc a une vitesse voisine de 200 cm/s*/
			}

/*			tmpp->IniAccel()								;
			tmpp->AddForce( vent )							;*/
			tmpp->CalcPos( laptime )						;

			vec3_mul( pos, tmpp->GetPos(), FACT_SCENE_CM )	;

			AffOneParti( pos, PETALS_SIZE, tmpp->GetAlpha() )	;
//			AffOneParti( pos, PETALS_SIZE, 1.0f )	;
		}

	U3D3Pipeline->End( NULL )	;

}
//----------------------------------------------------------------------------------------------------------------------------------------
