    			 /*-----------------------------------------------------+
				  |				         Spline.cpp						|
				  |														|
				  |	   Bon ici on calcul les corbes pour avoir des		|
				  |			 mouvements bien tip top fluides			|
				  |														|
				  | 	U2^PoPsy TeAm 1999								|
				  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
static void Deriv(KeyHeaderU3D *hp,KeyHeaderU3D *h,KeyHeaderU3D *hn,
				  Ufloat &ksm, Ufloat &ksp, Ufloat &kdm, Ufloat &kdp)
{
	Ufloat	dt = 1/( (Ufloat)0.5*(Ufloat)( hn->time - hp->time) );
	Ufloat	fp = dt * (Ufloat)( h->time - hp->time );
	Ufloat	fn = dt * (Ufloat)( hn->time -  h->time );
	Ufloat	 c = (Ufloat)fabs( h->continuity );
			fp = fp + c - c * fp;
			fn = fn + c - c * fn;
	Ufloat	cm = 1 - h->continuity;
	Ufloat	tm = (Ufloat)0.5 * ( 1 - h->tension );
	Ufloat	bm = 1 - h->bias;
	Ufloat	bp = 2 - bm;
	Ufloat tmcm = tm * cm;
	Ufloat tmcp = tm * ( 2 - cm );
	ksm  = tmcm*bp*fp;	ksp = tmcp*bm*fp;
	kdm  = tmcp*bp*fn;	kdp = tmcm*bm*fn;

}
//----------------------------------------------------------------------------------------------------------
static void CompElementDeriv(Ufloat pp, Ufloat p, Ufloat pn, Ufloat &ds, Ufloat &dd,
							 Ufloat ksm, Ufloat ksp, Ufloat kdm, Ufloat kdp)
{
	Ufloat delm,delp;

	delm = p - pp			;
	delp = pn - p			;
	ds = ksm*delm + ksp*delp;
	dd = kdm*delm + kdp*delp;
}
//----------------------------------------------------------------------------------------------------------
static void Hermite_coeff( float *h, float t, float dt )
{
	float t2,s,s2;

	t = t/dt	;
	t2 = t*t;	s = 1-t;	s2 = s*s;

	h[1] = t2*(3-2*t)					;
	h[0] = 1-h[1]						;
	h[2] = t*s2							;
	h[3] = -t2*s						;
}
//----------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------+
//									|   Class SplineU3D	  |
//									+---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
SplineU3D::SplineU3D( U32 nbk, U32 nbdim, Ufloat *key, KeyHeaderU3D *kinfos  )
{
	nbKey		= nbk	;
	nbDim		= nbdim	;
	actualKey	= 0		;

	inf		= new SplineInfoKey[ nbKey ]	;
	Keys	= new SplineKey[ nbKey*nbDim ]	;
	actualInf = inf							;
	actualKey = Keys						;
	SplineKey *preced = Keys				;
	SplineKey *actuel = Keys				;
	KeyHeaderU3D *kpreced = kinfos			;

	U32 a;
	
	for(a=0; a<nbk; a++)
	{
		inf[a].frame = 0.0f		;
		for( U32 b=0; b<nbdim; b++)
			Keys[a*nbdim+b].v = key[a*nbdim+b]	;
	}

	for(a=0; a<nbk-1; a++ )
	{
		inf[a].dt = (float)(kinfos[a+1].time - kinfos[a].time)								;
		SetDerivKey( preced, actuel, &actuel[nbDim], kpreced, &kinfos[a], &kinfos[a+1] )	;
		preced = actuel																		;
		actuel+=nbDim																		;
		kpreced = &kinfos[a]																;
	}

	SetDerivKey( preced, actuel, actuel, kpreced, &kinfos[a], &kinfos[a] )	;
}
//----------------------------------------------------------------------------------------------------------
SplineU3D::SplineU3D( PosKeyFileU3D *pos, U32 nbk )
{
	nbKey = nbk			;
	nbDim = 3			;
	actualNumKey = 0	;

	actualInf = inf = new SplineInfoKey[ nbKey ]	;
	actualKey = Keys = new SplineKey[ nbKey*nbDim ]	;
	SplineKey *preced = Keys						;
	SplineKey *actuel = Keys						;
	KeyHeaderU3D *kpreced = &pos[0].k				;

	U32 a;
	
	for(a=0; a<nbk; a++)
	{
		Keys[a*nbDim+0].v = pos[a].p[0]	;
		Keys[a*nbDim+1].v = pos[a].p[1]	;
		Keys[a*nbDim+2].v = pos[a].p[2]	;
		inf[a].frame = 0.0f				;
	}

	for(a=0; a<nbk-1; a++)
	{
		inf[a].dt = (float)(pos[a+1].k.time - pos[a].k.time)							;
		SetDerivKey( preced, actuel, &actuel[nbDim], kpreced, &pos[a].k, &pos[a+1].k )	;
		preced = actuel																	;
		actuel+=nbDim																	;
		kpreced = &pos[a].k																;
	}

	SetDerivKey( preced, actuel, actuel, kpreced, &pos[a].k, &pos[a].k )	;

}
//----------------------------------------------------------------------------------------------------------
SplineU3D::SplineU3D( UniKeyFileU3D *uni, U32 nbk )
{
	nbKey = nbk;
	nbDim = 1;
	actualNumKey = 0;

	actualInf = inf = new SplineInfoKey[ nbKey ];
	TestPtr( inf );
	actualKey = Keys = new SplineKey[ nbKey*nbDim ];
	TestPtr( Keys );
	SplineKey *preced = Keys;
	SplineKey *actuel = Keys;
	KeyHeaderU3D *kpreced = &uni[0].k;

	U32 a;
	
	for(a=0; a<nbk; a++)
	{
		Keys[a].v = uni[a].u;
		inf[a].frame = 0;
	}

	for(a=0; a<nbk-1; a++)
	{
		inf[a].dt = uni[a+1].k.time - uni[a].k.time;
		inf[a].frame = 0;
		SetDerivKey( preced, actuel, &actuel[1],
					 kpreced, &uni[a].k, &uni[a+1].k );
		preced = actuel;
		actuel++;
		kpreced = &uni[a].k;
	}
	SetDerivKey( preced, actuel, actuel,
				 kpreced, &uni[a].k, &uni[a].k );
}
//----------------------------------------------------------------------------------------------------------
SplineU3D::~SplineU3D()
{
	delete [] Keys	;
	delete [] inf	;
}
//----------------------------------------------------------------------------------------------------------
void SplineU3D::SetDerivKey(SplineKey *skp, SplineKey *sk, SplineKey *skn,
			KeyHeaderU3D *hp,KeyHeaderU3D *h,KeyHeaderU3D *hn)
{
	Ufloat ksm,ksp,kdm,kdp;

	Deriv( hp, h, hn, ksm, ksp, kdm, kdp );
	for(U32 a=0; a<nbDim; a++)
		CompElementDeriv( skp[a].v, sk[a].v, skn[a].v, sk[a].ds, sk[a].dd,
						  ksm, ksp, kdm, kdp );
}
//----------------------------------------------------------------------------------------------------------
void SplineU3D::Ini()
{
	actualKey = Keys	;
	actualInf = inf		;
	actualNumKey = 0	;
	inf->frame = 0;
}
//----------------------------------------------------------------------------------------------------------
// met la courbe a une position donnée 
void SplineU3D::SetPos( float p )
{
	Ini()		;
	IncPos( p )	;
}
//----------------------------------------------------------------------------------------------------------
// incrémente position dans la spline de n valeur
void SplineU3D::IncPos( float n )
{
	actualInf->frame+= n	;

	if( actualInf->frame > actualInf->dt )
	{
		n = actualInf->frame - actualInf->dt		;
		actualNumKey++								;
		if( actualNumKey == (nbKey-1) )
		{
			actualNumKey--						;	// c'est la fin on arrete
			actualInf->frame = actualInf->dt	;
		}
		else
		{
			actualKey+=nbDim		;
			actualInf++				;
			actualInf->frame = 0.0f	;
			IncPos( n )				;	// fait ca en recursif pour pouvoir sauter comme il fo les clés !
		}
	}
}
//----------------------------------------------------------------------------------------------------------
// renvoie valeur actuelle sur la spline
void SplineU3D::GetVal( Ufloat *dest )
{
	Ufloat	h[4]	;
	U32		a		;
	
	Hermite_coeff( h, actualInf->frame, actualInf->dt )	;
	for( a=0; a<nbDim; a++ )
	{
		dest[a]  = h[0] * actualKey[a].v			;
		dest[a] += h[1] * actualKey[a+nbDim].v		;
		dest[a] += h[2] * actualKey[a].dd			;
		dest[a] += h[3] * actualKey[a+nbDim].ds		;
	}
}
//----------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------+
//									| Class SplineRotU3D  |
//									+---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
SplineRotU3D::SplineRotU3D( U32 nbk )
{
	nbKey = nbk			;
	actualNumKey = 0	;

	inf = new SplineInfoKey[ nbKey ]		;
	Keys = new SplineRotKey[ nbKey ]		;
	actualInf = inf							;
	actualRotKey = Keys						;
}
//----------------------------------------------------------------------------------------------------------
SplineRotU3D::SplineRotU3D( RotKeyFileU3D *rot, U32 nbk )
{
	nbKey = nbk			;
	actualNumKey = 0	;

	inf = new SplineInfoKey[ nbKey ]	;
	Keys = new SplineRotKey[ nbKey ]	;
	actualInf = inf						;
	actualRotKey = Keys					;

	SplineRotKey *preced = Keys			;
	SplineRotKey *actuel = Keys			;
	KeyHeaderU3D *kpreced = &rot[0].k	;
	
	U32 a;
	
	for(a=0; a<nbk; a++)
	{
		Keys[a].q = rot[a].q	;
		inf[a].frame = 0.0f		;
	}

	for(a=0; a<nbk-1; a++)
	{
		inf[a].dt = (float)(rot[a+1].k.time - rot[a].k.time)					;
		CompAB( preced, actuel, &actuel[1], kpreced, &rot[a].k, &rot[a+1].k )	;
		preced = actuel															;
		actuel ++																;
		kpreced = &rot[a].k														;
	}

	CompAB( preced, actuel, actuel, kpreced, &rot[a].k, &rot[a].k )	;
}
//----------------------------------------------------------------------------------------------------------
SplineRotU3D::~SplineRotU3D()
{
	delete [] inf	;
	delete [] Keys	;
}
//----------------------------------------------------------------------------------------------------------
void SplineRotU3D::CompAB(SplineRotKey *skp, SplineRotKey *sk, SplineRotKey *skn,
						  KeyHeaderU3D *hp,KeyHeaderU3D *h,KeyHeaderU3D *hn )
{
	Quat g1,g2,g3	;


	g1 = sk->q.slerp( skp->q,-(1.0f+h->bias)/3.0f )	;
	g2 = sk->q.slerp( skn->q, (1.0f-h->bias)/3.0f )	;

	g3 = g1.slerp( g2, 0.5f + 0.5f*h->continuity )	;
	sk->quad_a = sk->q.slerp( g3, h->tension-1.0f )	;

	g3 = g1.slerp( g2, 0.5f - 0.5f*h->continuity )		;
	sk->quad_b = sk->q.slerp( g3, -(h->tension-1.0f) )	;
}
//----------------------------------------------------------------------------------------------------------
// remet la parcour de la spline au debut
void SplineRotU3D::Ini()
{
	actualRotKey = Keys	;
	actualInf = inf		;
	actualNumKey = 0	;
	inf->frame = 0.0f	;
}
//----------------------------------------------------------------------------------------------------------
// met la courbe a une position donnée 
void SplineRotU3D::SetPos( float p )
{
	Ini()		;
	IncPos( p )	;
}
//----------------------------------------------------------------------------------------------------------
// incrémente position dans la spline de n valeur
void SplineRotU3D::IncPos( float n )
{
	actualInf->frame+= n	;

	if( actualInf->frame > actualInf->dt )
	{
		n = actualInf->frame - actualInf->dt		;
		actualNumKey++								;
		if( actualNumKey == (nbKey-1) )
		{
			actualNumKey--						;	// c'est la fin on arrete
			actualInf->frame = actualInf->dt	;
		}
		else
		{
			actualRotKey++			;
			actualInf++				;
			actualInf->frame = 0.0f	;
			IncPos( n )				;	// fait ca en recursif pour pouvoir sauter comme il fo les clés !
		}
	}	
}
//----------------------------------------------------------------------------------------------------------
// renvoie valeur actuelle sur la spline
void SplineRotU3D::GetVal( Quat &q )
{	
	Quat	q0,q1,q2	;
	Quat	tmp1,tmp2	;
	Ufloat	t			;

	t = actualInf->frame / actualInf->dt	;

	q0 = actualRotKey->q.slerp( actualRotKey->quad_b,t )			;
	q1 = actualRotKey->quad_b.slerp( actualRotKey[1].quad_a, t )	;
	q2 = actualRotKey[1].quad_a.slerp( actualRotKey[1].q, t )		;

	q0 = q0.slerp( q1, t )	;
	q1 = q1.slerp( q2, t)	;

	q = q0.slerp( q1, t )	;

}
//----------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									| Class MorphRefU3D		|
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
MorphRefU3D::MorphRefU3D( LPCSTR n, U32 nb )
{
	nom = n					;
	nbp = nb				;
	xyz	= new Ufloat[nb*3]	;
}
//----------------------------------------------------------------------------------------------------------
MorphRefU3D::~MorphRefU3D()
{
	if( xyz )	delete [] xyz	;
}
//----------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									| Class SplineMorphU3D  |
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
SplineMorphU3D::SplineMorphU3D( MorphKeyFileU3D *morph, U32 nbk )
{
	nbKey = nbk														;
	actualNumKey = 0												;
	nbPosInMorph = U3D3Monde3D->GetObjet( morph->nom )->nbcoords	;

	AllRef = NULL						;
	inf = new SplineInfoKey[ nbKey ]	;
	Keys = new SplineMorphKey[ nbKey ]	;
	actualInf = inf						;
	actualMorphKey = Keys					;

	SplineMorphKey	*preced = Keys			;
	SplineMorphKey	*actuel = Keys			;
	KeyHeaderU3D	*kpreced = &morph[0].k	;
	
	U32 a;
	
	for(a=0; a<nbk; a++)
	{
		Keys[a].morph = AddOrCreateRef( &morph[a] )	;
		Keys[a].ds = new Ufloat[nbPosInMorph*3]		;
		Keys[a].dd = new Ufloat[nbPosInMorph*3]		;
		inf[a].frame = 0.0f							;
	}

	for(a=0; a<nbk-1; a++)
	{
		inf[a].dt = (float)(morph[a+1].k.time - morph[a].k.time)							;
		SetDerivKey( preced, actuel, &actuel[1], kpreced, &morph[a].k, &morph[a+1].k )	;
		preced = actuel																	;
		actuel ++																		;
		kpreced = &morph[a].k															;
	}

	SetDerivKey( preced, actuel, actuel, kpreced, &morph[a].k, &morph[a].k )	;
}
//----------------------------------------------------------------------------------------------------------
SplineMorphU3D::~SplineMorphU3D()
{
	MorphRefU3D	*tmp	;

	delete [] inf	;
	delete [] Keys	;

	while( AllRef )
	{
		tmp = AllRef			;
		AllRef = AllRef->suiv	;
		delete tmp				;
	}
}
//----------------------------------------------------------------------------------------------------------
MorphRefU3D *SplineMorphU3D::AddOrCreateRef( MorphKeyFileU3D *morph )
{
	MorphRefU3D	*tmp	;
	CoordU3D	*ctmp	;

	tmp = AllRef	;
	while( tmp )
	{
		if( morph->nom == tmp->GetNom() )	return tmp	;
		tmp = tmp->suiv									;
	}
	
	tmp = new MorphRefU3D( (LPCSTR)morph->nom, nbPosInMorph )	;
	tmp->suiv = AllRef											;
	AllRef = tmp												;


	//---------------------- remplit la référence avec les good coordonnées

	ctmp = U3D3Monde3D->GetObjet( morph->nom )->Ctab	;
	Ufloat *ptrF = tmp->GetPtr()						;

	for( U32 a=0; a<nbPosInMorph;	a++,ptrF+=3,ctmp++ )
		vec3_eg( ptrF, ctmp->origine	)	;

	//----------------------


	return tmp	;
}
//----------------------------------------------------------------------------------------------------------
void SplineMorphU3D::SetDerivKey(SplineMorphKey *skp, SplineMorphKey *sk, SplineMorphKey *skn,
				 KeyHeaderU3D *hp,KeyHeaderU3D *h,KeyHeaderU3D *hn)
{
	Ufloat	ksm,ksp,kdm,kdp		;
	Ufloat	*PtrF,*PtrFp,*PtrFn	;
	Ufloat	*Ders,*Derd			;

	Deriv( hp, h, hn, ksm, ksp, kdm, kdp )	;

	PtrF	= sk->morph->GetPtr()	;
	PtrFp	= skp->morph->GetPtr()	;
	PtrFn	= skn->morph->GetPtr()	;
	Ders	= sk->ds				;
	Derd	= sk->dd				;

	for(U32 a=0; a<nbPosInMorph*3; a++,PtrF++,PtrFn++,PtrFp++,Ders++,Derd++)
		CompElementDeriv( *PtrFp, *PtrF, *PtrFn, *Ders, *Derd,
						  ksm, ksp, kdm, kdp );
}
//----------------------------------------------------------------------------------------------------------
void SplineMorphU3D::Ini()
{
	actualMorphKey = Keys	;
	actualInf = inf			;
	actualNumKey = 0		;
	inf->frame = 0.0f		;
}
//----------------------------------------------------------------------------------------------------------
void SplineMorphU3D::SetPos( float p )
{
	Ini()		;
	IncPos( p )	;
}
//----------------------------------------------------------------------------------------------------------
void SplineMorphU3D::IncPos( float n )
{
	actualInf->frame+= n	;

	if( actualInf->frame > actualInf->dt )
	{
		n = actualInf->frame - actualInf->dt	;
		actualNumKey++							;
		if( actualNumKey == (nbKey-1) )
		{
			actualNumKey--						;	// c'est la fin on arrete
			actualInf->frame = actualInf->dt	;
		}
		else
		{
			actualMorphKey++		;
			actualInf++				;
			actualInf->frame = 0.0f	;
			IncPos( n )				;	// fait ca en recursif pour pouvoir sauter comme il fo les clés !
		}
	}	
}
//----------------------------------------------------------------------------------------------------------
// renvoie valeur actuelle sur la spline ( ici remplt le tableau de coord de l'objet ! )
void SplineMorphU3D::GetVal(  ObjetU3D *obj  )
{
	Ufloat		h[4]			;
	U32			a				;
	CoordU3D	*ctmp			;
	Ufloat		*PtrF,*PtrFn	;
	Ufloat		*Derd,*Ders		;

	Hermite_coeff( h, actualInf->frame, actualInf->dt )	;

	ctmp	= obj->Ctab							;
	PtrF	= actualMorphKey->morph->GetPtr()	;
	PtrFn	= actualMorphKey[1].morph->GetPtr()	;
	Derd	= actualMorphKey->dd				;
	Ders	= actualMorphKey[1].ds				;

	for( a=0; a<nbPosInMorph; a++,ctmp++ )
	{
		vec3_eg( ctmp->origine, PtrF )	;
		for( U32 b=0; b<3; b++,PtrF++,PtrFn++,Derd++,Ders++)
		{
			ctmp->origine[b]  = h[0] * (*PtrF)	;
			ctmp->origine[b] += h[1] * (*PtrFn)	;
			ctmp->origine[b] += h[2] * (*Derd)	;
			ctmp->origine[b] += h[3] * (*Ders)	;
		}
	}
}
//----------------------------------------------------------------------------------------------------------
