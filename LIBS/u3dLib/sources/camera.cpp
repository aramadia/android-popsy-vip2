    			 /*-----------------------------------------------------+
				  |				         camera.cpp					    |
				  |														|
				  |	  Une camera c'est indispensable pour un moteur     |
				  |	  3D digne de se nom c'est pourquoa ya a ici		|
				  |														|
				  | 	U2^PoPsy TeAm 1999								|
				  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
//							+------------------------------+
//							|    Les Variables globales    |
//							+------------------------------+
//----------------------------------------------------------------------------------------------------------

static Ufloat PosParDefaut[3]={.0f,.0f,100.f};
static Ufloat TargParDefaut[3]={.0f,.0f,.0f};



//----------------------------------------------------------------------------------------------------------
//									+---------------------+
//									|    Les Fonctions    |
//									+---------------------+
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//										Camera
//----------------------------------------------------------------------------------------------------------
//Camera::Camera(char *Nom, Ufloat sx, Ufloat sy )
Camera::Camera( LPCSTR Nom )
{
	nom = Nom	;

	SetPos( PosParDefaut );
	SetTarg( TargParDefaut );
	SetRoll( .0f );
	SetFov( 75.f );
	CalcVue();

//	SetClipFenetre( sx, sy );
	Viewport viewport	;	// viewport de la camera
	
	UpdateProjectMatrix();
	UpdateTransMatrix();
	UpdatePlans();

	Spos = NULL			;
	Starg = NULL		;
	Sroll = NULL		;
	Sfov = NULL			;
	callbackAnim = NULL	;
}
//----------------------------------------------------------------------------------------------------------
Camera::~Camera()
{
	if( Sfov)	delete Sfov		;
	if( Sroll ) delete Sroll	;
	if( Starg ) delete Starg	;
	if( Spos )	delete Spos		;
}
//----------------------------------------------------------------------------------------------------------
bool Camera::Serialize( CArchive &ar )
{
	U32				a							;
	U16				npkeys,nfkeys,nrkeys,ntkeys	;
	PosKeyFileU3D	*posK						;
	UniKeyFileU3D	*fovK						;
	UniKeyFileU3D	*rollK						;
	PosKeyFileU3D	*targK						;

	if( !ar.IsStoring() )
	{
		ar >> nom	;

		ar >> pos[0]	;
		ar >> pos[1]	;
		ar >> pos[2]	;

		ar >> targ[0]	;
		ar >> targ[1]	;
		ar >> targ[2]	;

		ar >> roll	;

		ar >> fov	;


		//-------------- Charge les pos keys
		ar >> npkeys										;
		posK = new PosKeyFileU3D[ npkeys ]					;
		TestPtr( posK )										;
		for( a=0; a<npkeys;	a++	)	posK[a].Serialize( ar )	;
		if( npkeys > 1)
			Spos = new SplineU3D( posK, npkeys )			;
		else
			Spos = NULL										;
		delete [] posK										;


		//-------------- sauve les fovs
		ar >> nfkeys										;
		fovK = new UniKeyFileU3D[ nfkeys ]					;
		TestPtr( fovK )										;
		for( a=0; a<nfkeys;	a++	)	fovK[a].Serialize( ar )	;
		if( nfkeys > 1 )
			Sfov = new SplineU3D( fovK, nfkeys )			;
		else
			Sfov = NULL										;
		delete [] fovK										;


		//-------------- sauve les rolls
		ar >> nrkeys											;
		rollK = new UniKeyFileU3D[ nrkeys ]						;
		TestPtr( rollK )										;
		for( a=0; a<nrkeys;	a++	)	rollK[a].Serialize( ar )	;
		if( nrkeys > 1 )
			Sroll = new SplineU3D( rollK, nrkeys )				;
		else 
			Sroll = NULL										;
		delete [] rollK											;


		//-------------- sauve les targets
		ar >> ntkeys											;
		targK = new PosKeyFileU3D[ ntkeys ]						;
		TestPtr( targK )										;
		for( a=0; a<ntkeys;	a++	)	targK[a].Serialize( ar )	;
		if( ntkeys > 1 )
			Starg = new SplineU3D( targK, ntkeys )				;	 
		else
			Starg = NULL										;
		delete [] targK											;

	}
	else return FALSE	;

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------
// TODO : A FAIREEEEEEEE !
Camera* Camera::operator=(Camera *cam)
{
/*	SetTarg( cam->targ );
	SetRoll( cam->roll );
	SetPos( cam->pos );
	SetFov( cam->fov );

	screenscalex = cam->screenscalex;
	screenscaley = cam->screenscaley;

	UpdatePlans();
	UpdateProjectMatrix();
	UpdateTransMatrix();*/

	return this;
}
//----------------------------------------------------------------------------------------------------------
void Camera::Set( Viewport &vi )
{
	viewport = vi	;
}
//----------------------------------------------------------------------------------------------------------
// définit une fenètre d'affichage pour la camera
/*void Camera::SetClipFenetre( Ufloat Xlarg, Ufloat Yhauteur )
{
	screenscalex = Xlarg;
	screenscaley = Yhauteur;
}*/
//----------------------------------------------------------------------------
void Camera::UpdatePlans()
{
	Ufloat angle;
	Ufloat normale[3],s,c;

	angle = (Ufloat)3.141592654f*90.f/360.f;
	s = sin( angle );
	c = cos( angle );
	
	// calcul plan de gauche
	vec3_set( normale, s, 0, c );
	Cam2Monde( gauche.normal, normale );
	gauche.dist = vec3_dot( pos, gauche.normal ) + ClipPlaneEpsilon*.01f;
	// calcul plan de droite
	vec3_set( normale, -s, 0, c );
	Cam2Monde( droite.normal, normale );
	droite.dist = vec3_dot( pos, droite.normal ) + ClipPlaneEpsilon*.01f;

	// calcul plan du bas
	vec3_set( normale, 0, s, c );
	Cam2Monde( bas.normal, normale );
	bas.dist = vec3_dot( pos, bas.normal ) + ClipPlaneEpsilon*.01f;
	// calcul plan du haut
	vec3_set( normale, 0, -s, c );
	Cam2Monde( haut.normal, normale );
	haut.dist = vec3_dot( pos, haut.normal ) + ClipPlaneEpsilon*.01f;

	// calcul plan de coupre proche
	vec3_set( normale, 0, 0, 1 );
	Cam2Monde( proche.normal, normale );
	proche.dist = vec3_dot( pos, proche.normal ) + ProcheZClipping;
}
//----------------------------------------------------------------------------
void Camera::UpdateProjectMatrix()
{
	// met tout à zero!
	memset( &Project[0], 0, sizeof( Project ) );

	Ufloat f = 3.141592654f*fov/360.f;
	f = Project[3][2] = sin(f) / cos(f) ;
	Project[0][0] = 1;
	Project[1][1] = (float) 4 / 3;
//	Project[1][1] = (float)viewport.GetDx() / viewport.GetDy()	;
	Project[2][2] = f;/* * ( LointainZClipping / ( LointainZClipping -
					                            ProcheZClipping ) );;*/
	Project[2][3] = f * ( LointainZClipping * ProcheZClipping /
				        ( LointainZClipping - ProcheZClipping ) );
}
//----------------------------------------------------------------------------
void Camera::UpdateTransMatrix()
{
	Udouble v1[3];
	Ufloat t[4][4],t1[4][4];
	Udouble r,rx,ry;

	vec3_sub( vue, targ, pos );
	vec3_normalize( vue );
	r = sqrt( vue[2]*vue[2] + vue[0]*vue[0] );

	ry= acos( vue[2]/r );
	if( vue[0]<0 ) ry=-ry;
	m44_identite( t );
	t[0][0]=cos(ry);	t[0][2]=-sin(ry);
	t[2][0]=sin(ry);	t[2][2]=cos(ry);	
	

	v1[0] = vec3_dot( vue, t[0] );
	v1[1] = vec3_dot( vue, t[1] );
	v1[2] = vec3_dot( vue, t[2] );

	r = sqrt( v1[2]*v1[2] + v1[1]*v1[1] );
	rx= acos( v1[2]/r );
	if( vue[1]>0 ) rx=-rx;
	m44_identite( t1 );
	t1[1][1]=cos(rx);		t1[1][2]=sin(rx);
	t1[2][1]=-sin(rx);		t1[2][2]=cos(rx);

	m44_multiply(t,t1,t)					;
	Quat q( vue, -roll )					;
	q.matrix( Trans )						;
	m44_multiply( Trans, t, Trans )			;
	m44_multiply( Final, Project, Trans )	;
	CalcVue()								;
}
//----------------------------------------------------------------------------------------------------------
//  renvoie le code de clipping que subit la bounding box pour
//	être affichée sur l'écran
U32 Camera::ClipInfo( BoundingBox *b, Mat3x4 &mat, U32 flag )
{
	U32		retFlag=ClipNo		;
	U32		a,count				;
	Plan	GaucheTr = gauche	;
	Plan	DroiteTr = droite	;
	Plan	HautTr	 = haut		;
	Plan	BasTr	 = bas		;
	Plan	ProcheTr = proche	;
	Mat3x4	Transpo  = mat		;

	Transpo.Transpose()	;

	GaucheTr *= Transpo	;
	DroiteTr *= Transpo	;
	HautTr   *= Transpo	;
	BasTr	 *= Transpo	;
	ProcheTr *= Transpo	;


	// pas besoin de retester le clipping parceque tout
	// est dans le champ de vision
	if( flag == ClipNo ) return ClipNo;


	if( flag&ClipGauche )
	{
		count=0	;
		for(a=0; a<8; a++)
			if( GaucheTr.distance( b->pts[a] )< 0 )
				count++	;
		if( count==8 ) return ClipCache				;
		else if( count!=0 ) retFlag |= ClipGauche	;
	}

	if( flag&ClipDroit )
	{
		count=0;
		for(a=0; a<8; a++)
			if( DroiteTr.distance( b->pts[a] )< 0 )
				count++	;
		if( count==8 ) return ClipCache				;
		else if( count!=0 ) retFlag |= ClipDroit	;
	}

	if( flag&ClipHaut )
	{
		count=0	;
		for(a=0; a<8; a++)
			if( HautTr.distance( b->pts[a] )< 0 )
				count++	;
		if( count==8 ) return ClipCache			;
		else if( count!=0 ) retFlag |= ClipHaut	;
	}

	if( flag&ClipBas )
	{
		count=0	;
		for(a=0; a<8; a++)
			if( BasTr.distance( b->pts[a] )< 0 )
				count++	;
		if( count==8 ) return ClipCache			;
		else if( count!=0 ) retFlag |= ClipBas	;
	}

	if( flag&ClipProche )
	{
		count=0;
		for(a=0; a<8; a++)
			if( ProcheTr.distance( b->pts[a] )< 0 )
				count++;
		if( count==8 ) return ClipCache				;
		else if( count!=0 ) retFlag |= ClipProche	;
	}

	return retFlag;
}
//----------------------------------------------------------------------------------------------------------
void Camera::Cam2Monde( Ufloat Vdest[3], const Ufloat Vsrce[3] )
{
	for(U32 a=0; a<3; a++)
	{
		Vdest[a] = Vsrce[0] * Final[0][a] +
				   Vsrce[1] * Final[1][a] +	
				   Vsrce[2] * Final[3][a];
	}
}
//----------------------------------------------------------------------------------------------------------
// donne des coordonnées en X,Y,Z camera pour les transformer dans les x,y,z monde avec X et Y en coordonnées unitaire ecran
void Camera::Cam2MondePos( Ufloat Vdest[3], const Ufloat Vsrce[3] )
{
	Ufloat tmp[3]					;
	Ufloat inv = 0.5f / Vsrce[2]	;

	tmp[0] = ( Vsrce[0] - .5f ) / inv	;
	tmp[1] = -( Vsrce[1] - .5f ) / inv	;
	tmp[2] = Vsrce[2]					;

	//-------- enleve parametre de fov !
	tmp[1] *= 1.f / Project[1][1]	;
	tmp[2] *= 1.f / Project[3][2]	;

	for(U32 a=0; a<3; a++)
	{
		Vdest[a] = tmp[0] * Trans[0][a] +
				   tmp[1] * Trans[1][a] +
				   tmp[2] * Trans[2][a];
	}

	vec3_add( Vdest, Vdest, pos )	;
}
//----------------------------------------------------------------------------------------------------------
/*inline void Camera::Projet( VertexU3D *v, U32 frame )
{
	CoordU3D *c = v->c;
	Ufloat x,y,z;

	// la coordonnée n'a pas déja été calculée
	if( frame != c->frame_process )
	{
		vec3_sub( c->trans, c->trans, pos );
		x = vec3_dot( c->trans, Final[0] );
		y = vec3_dot( c->trans, Final[1] );
		z = vec3_dot( c->trans, Final[3] );
		z=1/z;
		c->ecran[0] = z * x * screenscalex + centrex;
		c->ecran[1] =-z * y * screenscaley + centrey;
		c->frame_process = frame;
	}
}*/
//----------------------------------------------------------------------------------------------------------
void Camera::IniAnim()
{
	if( Sfov )	Sfov->Ini()		;
	if( Sroll ) Sroll->Ini()	;
	if( Starg ) Starg->Ini()	;
	if( Spos )	Spos->Ini()		;
}
//----------------------------------------------------------------------------------------------------------
void Camera::IncAnim( Ufloat nb )
{
	if( Sfov )
	{
		Sfov->IncPos(  nb )		;
		Sfov->GetVal( &fov )	;
	}

	if( Sroll )
	{
		Sroll->IncPos( nb )		;
		Sroll->GetVal( &roll )	;
	}

	if( Starg )
	{
		Starg->IncPos( nb )		;
		Starg->GetVal( targ )	;
	}

	if( Spos )
	{
		Spos->IncPos( nb )	;
		Spos->GetVal( pos )	;
	}

	if( callbackAnim )	(*callbackAnim)( this, nb, 0 )	;
}
//----------------------------------------------------------------------------------------------------------

