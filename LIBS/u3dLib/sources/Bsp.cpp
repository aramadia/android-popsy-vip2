    	 /*-----------------------------------------------------+
		  |				         BSP.cpp						|
		  |														|
		  |	    la définition de l'arbre BSP mais ici pour le   |
		  |					moteur temps réel!					|
		  |														|	
		  | 	U2^PoPsy TeAm 1999								|	
		  +-----------------------------------------------------*/

#include "U3D3.h"

//-------------------------------------------------------------------------
//						+---------------------+
//						|    Les Fonctions    |
//						+---------------------+
//-------------------------------------------------------------------------


//----------------------------------------------------------------------------
//							BoundingBox
//----------------------------------------------------------------------------
/*BoundingBox::BoundingBox()
{
	vec3_set( pts[0], +1e+19f, -1e+19f, -1e+19f );
	vec3_set( pts[1], -1e+19f, -1e+19f, -1e+19f );
	vec3_set( pts[2], +1e+19f, -1e+19f, +1e+19f );
	vec3_set( pts[3], -1e+19f, -1e+19f, +1e+19f );
	vec3_set( pts[4], +1e+19f, +1e+19f, -1e+19f );
	vec3_set( pts[5], -1e+19f, +1e+19f, -1e+19f );
	vec3_set( pts[6], +1e+19f, +1e+19f, +1e+19f );
	vec3_set( pts[7], -1e+19f, +1e+19f, +1e+19f );
}
//----------------------------------------------------------------------------
// actualise la Bounding en fct du point si necessaire
void BoundingBox::TestEtActua( Ufloat *pt )
{
	if( pt[0] > pts[1][0] )
	{
		pts[1][0] = pt[0]; 
		pts[3][0] = pt[0]; 
		pts[5][0] = pt[0]; 
		pts[7][0] = pt[0]; 
	}
	if( pt[0] < pts[0][0] )
	{
		pts[0][0] = pt[0]; 
		pts[2][0] = pt[0]; 
		pts[4][0] = pt[0]; 
		pts[6][0] = pt[0]; 
	}
	if( pt[1] > pts[0][1] )
	{
		pts[0][1] = pt[1]; 
		pts[1][1] = pt[1]; 
		pts[2][1] = pt[1]; 
		pts[3][1] = pt[1]; 
	}
	if( pt[1] < pts[4][1] )
	{
		pts[4][1] = pt[1]; 
		pts[5][1] = pt[1]; 
		pts[6][1] = pt[1]; 
		pts[7][1] = pt[1]; 
	}
	if( pt[2] > pts[0][2] )
	{
		pts[0][2] = pt[2]; 
		pts[1][2] = pt[2]; 
		pts[4][2] = pt[2]; 
		pts[5][2] = pt[2]; 
	}
	if( pt[2] < pts[2][2] )
	{
		pts[2][2] = pt[2]; 
		pts[3][2] = pt[2]; 
		pts[6][2] = pt[2]; 
		pts[7][2] = pt[2]; 
	}
}
//----------------------------------------------------------------------------
// renvoie TRUE si &b est contune dans la bounding box !
bool BoundingBox::Contenu( BoundingBox &b )
{
	if( pts[1][0] < b.pts[1][0] ) return FALSE;
	if( pts[0][0] > b.pts[0][0] ) return FALSE;

	if( pts[0][1] < b.pts[0][1] ) return FALSE;
	if( pts[4][1] > b.pts[1][1] ) return FALSE;

	if( pts[0][2] < b.pts[0][2] ) return FALSE;
	if( pts[2][2] > b.pts[2][2] ) return FALSE;

	return FALSE;
}*/
//----------------------------------------------------------------------------
//							BoundingSphere
//----------------------------------------------------------------------------
/*BoundingSphere::BoundingSphere()
{
	vec3_set( centre, 0.f, 0.f, 0.f );
	rayon = 0.f;
	rayonFake = 0.f;
}*/
//----------------------------------------------------------------------------
//							Uplan_BSP
//----------------------------------------------------------------------------
// recalcul le plan à partie des 3 points 
/*void Uplan_BSP::Calc( const Ufloat pt0[3], const Ufloat pt1[3], const Ufloat pt2[3] )
{
	Ufloat v1[3],v2[3];

	vec3_sub( v1, pt1, pt0 );
	vec3_sub( v2, pt2, pt0 );
	vec3_xprd( normal, v1, v2 );
	vec3_normalize( normal );
	dist = vec3_dot( pt0, normal );
}
//----------------------------------------------------------------------------
//renvoie position d'une Bounding box par rapport au plan
BoundPlace Uplan_BSP::GetPos( BoundingBox &b )
{
	bool derriere=FALSE,devant=FALSE;

	for(U32 a=0; a<8; a++)
	{
		if( distance( b.pts[a] )< 0 )	derriere = TRUE;
		else devant = TRUE;
	}
	if( derriere && devant ) return ENTRE_DEUX;
	if( derriere ) return DERRIERE;
	else return DEVANT;
}
//----------------------------------------------------------------------------
// renvoie la matrice de transformation associée au plan 
void Uplan_BSP::GetTransMatrix( Ufloat m[4][4] )
{
	Ufloat r,rx,ry;
	Ufloat t[4][4];
	Ufloat v1[3];

	r = sqrt( normal[2]*normal[2] + normal[0]*normal[0] );
	if( r==0.0f )	// cas particulier
	{
		m44_identite( m );
		vec3_set( m[1], 0.f, 0.f, 1.f );
		vec3_set( m[2], 0.f, 1.f, 0.f );
		return;
	}
	ry= acos( normal[2]/r );
	if( normal[0]<0 ) ry=-ry;
	m44_identite( t );
	t[0][0]=cos(ry);	t[0][2]=-sin(ry);
	t[2][0]=sin(ry);	t[2][2]=cos(ry);	

	v1[0] = vec3_dot( normal, t[0] );
	v1[1] = vec3_dot( normal, t[1] );
	v1[2] = vec3_dot( normal, t[2] );

	r = sqrt( v1[2]*v1[2] + v1[1]*v1[1] );
	rx= acos( v1[2]/r );
	if( normal[1]>0 ) rx=-rx;
	m44_identite( m );
	m[1][1]=cos(rx);		m[1][2]=sin(rx);
	m[2][1]=-sin(rx);		m[2][2]=cos(rx);

	m44_multiply(m,t,m);
}*/
//----------------------------------------------------------------------------
//							PolygonU3D
//----------------------------------------------------------------------------
/*PolygonU3D::PolygonU3D()
{
	nbVertex = 0;
	Vtab = NULL;
	arrete = 0;
	flag = PolyFLAG_NORM;
	m = NULL;
	suiv = NULL;
	lmapTex = NULL;

	vec3_set( RVB, 1.f, 1.f, 1.f );
}
//----------------------------------------------------------------------------
PolygonU3D::~PolygonU3D()
{
	if( Vtab ) delete [] Vtab;
}
//----------------------------------------------------------------------------
// calcul normale d'origine
void PolygonU3D::CalcPlan()
{
	plan.Calc( Vtab[0]->c->origine, Vtab[1]->c->origine, Vtab[2]->c->origine );
}
//----------------------------------------------------------------------------
// créé un new poly si nécessaire en fonction du clipping !
PolygonU3D *PolygonU3D::ClipToPlan(Uplan_BSP *plan)
{
	PolygonU3D *ret;
	VertexU3D *V[20],**V1;
	VertexU3D **Vtab1;
	VertexU3D *debut, *fin;
	Ufloat dist1,dist2,distp;
	U32 i,nbV,arreteF=0;
	bool Fnew=FALSE;
	
	nbV = 0;
	V1 = V;
	Vtab1 = Vtab;
	distp = dist2 = plan->distance( Vtab[0]->c->trans );
	for(i=0; i<nbVertex-1; i++,Vtab1++)
	{
		debut = *Vtab1;
		fin =   Vtab1[1];

		dist1 = dist2;
		dist2 = plan->distance( fin->c->trans );
		if( (dist1>=0) && (dist2>=0) )
		{
			*V1 = debut;
			V1++;
			arreteF |= ( (arrete>>i)&0x1 )<<nbV;
			nbV++;
		}
		// cas ou les 2 points sont hors de champ
		else if( (dist1<0) && (dist2<0) )
		{
			Fnew = 1;
			continue;
		}
		// ca où l'on créé un nouveau vertex car le segment est 
		// coupé
		else
		{
			Ufloat scale;
			VertexU3D *nouvo;
			CoordU3D *co;

			if( dist1>=0 )
			{
				*V1 = debut;
				V1++;
				arreteF |= ( (arrete>>i)&0x1 )<<nbV;
				nbV++;
			}
			else
				arreteF |= ( (arrete>>i)&0x1 )<<nbV;

			scale =  -dist1 / (dist2 - dist1 );

			nouvo = U3D3Manager->NewVertex();
			nouvo->c = U3D3Manager->NewCoord();
			co = nouvo->c;

			// ---> interpole les données attachées au vertex !
			nouvo->Interpole3D( debut, fin, scale );
			// insere new point
			*V1 = nouvo;
			V1++;

			nbV++;
			Fnew = 1;
		}
	}

	{
		debut = *Vtab1;
		fin =  Vtab[0];

		dist1 = dist2;
		dist2 = distp;
		if( (dist1>=0) && (dist2>=0) )
		{
			*V1 = debut;
			V1++;
			arreteF |= ( (arrete>>i)&0x1 )<<nbV;
			nbV++;
		}
		// cas ou les 2 points sont hors de champ
		else if( (dist1<0) && (dist2<0) )
		{
			Fnew = 1;
		}
		// ca où l'on créé un nouveau vertex car le segment est 
		// coupé
		else
		{
			Ufloat scale;
			VertexU3D *nouvo;
			CoordU3D *co;

			if( dist1>=0 )
			{
				*V1 = debut;
				V1++;
				arreteF |= ( (arrete>>i)&0x1 )<<nbV;
				nbV++;
			}
			else
				arreteF |= ( (arrete>>i)&0x1 )<<nbV;

			scale =  -dist1 / (dist2 - dist1 );

			nouvo = U3D3Manager->NewVertex();
			nouvo->c = U3D3Manager->NewCoord();
			co = nouvo->c;

			// ---> interpole les données attachées au vertex !
			nouvo->Interpole3D( debut, fin, scale );
			// insere new point
			*V1 = nouvo;
			V1++;

			nbV++;
			Fnew = 1;
		}
	}

	if( !Fnew ) return this;	// pas de clipping alors rien de nouvo
	else
	{
		if( nbV<3 ) return NULL; // moins de 3 sommets alors zou rien

		ret = U3D3Manager->NewPoly( nbV );		// créé un poly
		ret->flag = this->flag|PolyFLAG_TEMP;	// on indique que c'est un poly temporaire
		ret->arrete = arreteF;
		ret->lmapTex = lmapTex;
		vec3_eg( ret->RVB, RVB );
		ret->m = this->m;
		V1 = V;
		Vtab1 = ret->Vtab;
		for(i=0; i<nbV; i++,V1++,Vtab1++)
		{
			*Vtab1 = *V1;
			(*V1)->utilisateurs++;		// indique utilisation
			(*V1)->c->utilisateurs++;	// indique utilisation
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
PolygonU3D* PolygonU3D::GetClipped2DPoly(U32 flag,Ufloat val)
{
	U32 a,nbV=0;
	bool Fnew=FALSE;
	VertexU3D *dst[20],**V1,**Vtab1;
	PolygonU3D *ret;

	for( a=0; a<nbVertex; a++ )
	{
		CoordU3D *src1 = Vtab[a]->c;
		CoordU3D *src2 = Vtab[(a+1)%nbVertex]->c;
		if( (src1->ClipInfo&flag) == 0 )
		{
			dst[nbV++] = Vtab[a];
			if( (src2->ClipInfo&flag) ==0 )
				continue;
		}
		else
		{
			if( src2->ClipInfo&flag )
				continue;
		}

		Fnew = TRUE;
		// alloue new vertex
		VertexU3D *nouvo;
		nouvo = U3D3Manager->NewVertex();
		nouvo->c = U3D3Manager->NewCoord();
		// met flag de clipping a jour
		Camera *cam = U3D3Monde3D->GetActualCamera();
		if( flag==ClipGauche || flag==ClipDroit )
		{
			Ufloat scale = (val - src1->ecran[0]) / (src2->ecran[0] - src1->ecran[0]);

			// ---> interpole les données attachées au vertex !
			nouvo->Interpole2D( Vtab[a], Vtab[(a+1)%nbVertex], scale );
			nouvo->c->ecran[0] = val;
			nouvo->c->ecran[1] = scale*( src2->ecran[1] - src1->ecran[1] ) + src1->ecran[1];

			// remet flag de clipping a jour pour ce vertex
			if( nouvo->c->ecran[1] < cam->GetHaut2DClip() )
				nouvo->c->ClipInfo = ClipHaut;
			else if(nouvo->c->ecran[1] > cam->GetBas2DClip() )
				nouvo->c->ClipInfo = ClipBas;
		}
		else
		{
			Ufloat scale = (val - src1->ecran[1]) / (src2->ecran[1] - src1->ecran[1]);

			// ---> interpole les données attachées au vertex !
			nouvo->Interpole2D( Vtab[a], Vtab[(a+1)%nbVertex], scale );
			nouvo->c->ecran[1] = val;
			nouvo->c->ecran[0] = scale*( src2->ecran[0] - src1->ecran[0] ) + src1->ecran[0];

			// remet flag de clipping a jour pour ce vertex
			if( nouvo->c->ecran[0] < cam->GetGauche2DClip() )
				nouvo->c->ClipInfo = ClipGauche;
			else if(nouvo->c->ecran[0] > cam->GetDroite2DClip() )
				nouvo->c->ClipInfo = ClipDroit;
		}

		dst[nbV++] = nouvo;
	}

	if( nbV<3 ) return NULL;	// moins de 3 sommet alors on se casse
	if( !Fnew ) return this;	// pas de clipping alors rien de nouvo
	else
	{
		ret = U3D3Manager->NewPoly( nbV );		// créé un poly
		ret->flag = this->flag|PolyFLAG_TEMP;	// on indique que c'est un poly temporaire
//		ret->arrete = arreteF;				// PAS Géré POUR l'INSTANT !
		ret->lmapTex = lmapTex;
		vec3_eg( ret->RVB, RVB );
		ret->m = this->m;
		V1 = dst;
		Vtab1 = ret->Vtab;
		for(U32 i=0; i<nbV; i++,V1++,Vtab1++)
		{
			*Vtab1 = *V1;
			(*V1)->utilisateurs++;		// indique utilisation
			(*V1)->c->utilisateurs++;	// indique utilisation
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
PolygonU3D* PolygonU3D::GetClipped2DPoly()
{
	PolygonU3D *draw1,*draw2;
	VertexU3D *V[20],*V1[20];
	VertexU3D ** src = V;
	VertexU3D ** dst = V1;
	U32 a;
	bool BesGauche,BesDroit,BesHaut,BesBas;

	Camera *cam = U3D3Monde3D->GetActualCamera();
	draw1 = this;

	//-------------------------> realise e clipping proche si besoin est
	if( BesoinClip(ClipProche) )
	{
		bool ret=TRUE;
		for( a=0; a<nbVertex; a++)
		{
			if( (Vtab[a]->c->ClipInfo&ClipProche) == 0 )
			{
				ret = FALSE;
				break;
			}
		}
		if( ret ) return NULL;

		draw2 = draw1->ClipToPlan( cam->GetPlanProche() );
		if( !draw2 ) return NULL;
		draw1 = draw2;

		U32 num = U3D3Monde3D->GetTick();
		for( a=0; a<draw1->nbVertex; a++)
			cam->Projet( draw1->Vtab[a], num );	// calcul coords 2D si il le fo

		BesGauche = draw1->BesoinClip( ClipGauche );
		BesDroit  = draw1->BesoinClip( ClipDroit );
		BesHaut   = draw1->BesoinClip( ClipHaut );
		BesBas    = draw1->BesoinClip( ClipBas );;
	}
	else
	{
		BesGauche = BesoinClip( ClipGauche );
		BesDroit  = BesoinClip( ClipDroit );
		BesHaut   = BesoinClip( ClipHaut );
		BesBas    = BesoinClip( ClipBas );;
	}

	if( BesGauche )
	{
//		draw2 = draw1->GetClipped2DPoly( ClipGauche, cam->GetGauche2DClip() );
		draw2 = draw1->ClipToPlan( cam->GetPlanGauche() );
		if( !draw2 ) 
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 );
			return NULL;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 );
		draw1 = draw2;
	}
	if( BesDroit )
	{
//		draw2 = draw1->GetClipped2DPoly( ClipDroit, cam->GetDroite2DClip() );
		draw2 = draw1->ClipToPlan( cam->GetPlanDroit() );
		if( !draw2 )
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 );
			return NULL;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 );
		draw1 = draw2;
	}
	if( BesBas )
	{			
//		draw2 = draw1->GetClipped2DPoly( ClipBas, cam->GetBas2DClip() );
		draw2 = draw1->ClipToPlan( cam->GetPlanHaut() );
		if( !draw2 ) 
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 );
			return NULL;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 );
		draw1 = draw2;
	}
	if( BesHaut )
	{
//		draw2 = draw1->GetClipped2DPoly( ClipHaut, cam->GetHaut2DClip() );
		draw2 = draw1->ClipToPlan( cam->GetPlanBas() );
		if( !draw2 )
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 );
			return NULL;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 );
		draw1 = draw2;
	}

	// calcul coords 2D si il le fo
	U32 num = U3D3Monde3D->GetTick();
	for( a=0; a<draw1->nbVertex; a++)
	{
		cam->Projet( draw1->Vtab[a], num );	
		if( draw1->Vtab[a]->c->ClipInfo != ClipNo ) 
			a=a;
	}

	return draw1;
}
//----------------------------------------------------------------------------
// renvoie TRUE si le segment coupe le poly
#define EPSILON 0.01f
bool PolygonU3D::Intersection( Ufloat debut[3], Ufloat fin[3] )
{
	Uplan_BSP planCote;

	// fait dabord backface culling!
	if( BackFaceCull( debut ) )
		return FALSE;

	// si les 2 pts sont du même coté du plan de la face alors
	// cette que le segment ne coupe pas cette face !
	bool res1, res2;
	Ufloat dist = plan.distance(debut);
	if(  dist < -EPSILON ) res1 = 0;
	else if( dist > EPSILON ) res1 = 1;
	else return FALSE;					// cas ou pt dans le plan du poly -> yaura forcement pas de coupe !
	dist = plan.distance(fin);
	if(  dist < -EPSILON ) res2 = FALSE;
	else if( dist > EPSILON ) res2 = TRUE;
	else return FALSE;					// pareil qu'en haut
	if( res1 == res2 )	return FALSE;

	planCote.Calc( debut, Vtab[nbVertex-1]->c->origine, Vtab[0]->c->origine );
	bool preced = TRUE,actua;
	if( planCote.distance( fin ) < -EPSILON ) preced = FALSE;

	for( U32 a=0; a<nbVertex-1; a++ )
	{
		planCote.Calc( debut, Vtab[a]->c->origine, Vtab[a+1]->c->origine );
		if( planCote.distance( fin ) < -EPSILON ) actua = FALSE;
		else actua = TRUE;
		if( actua != preced ) return FALSE;
		preced = actua;
	}
	return TRUE;
}*/
//----------------------------------------------------------------------------
//							Unoeud_BSP
//----------------------------------------------------------------------------
/*Unoeud_BSP::Unoeud_BSP(U32 nbf)
{
	devant = NULL;
	derriere = NULL;

	nbfaces = nbf;
	Ptab = new PolygonU3D*[nbf];
	if( nbf ) TestPtr(Ptab);
}
//----------------------------------------------------------------------------
Unoeud_BSP::~Unoeud_BSP()
{
	if( Ptab ) delete [] Ptab;
	if( devant ) delete  devant;
	if( derriere ) delete derriere;
}
//----------------------------------------------------------------------------
void Unoeud_BSP::CalculPlan()
{
	Ufloat v1[3],v2[3],v[3];

	vec3_sub( v1, Ptab[0]->Vtab[1]->c->origine, Ptab[0]->Vtab[0]->c->origine );
	vec3_sub( v2, Ptab[0]->Vtab[2]->c->origine, Ptab[0]->Vtab[0]->c->origine );
	vec3_xprd( v, v1, v2 );
	vec3_normalize( v );
	vec3_eg( plan.normal, v );
	plan.dist = vec3_dot( v, Ptab[0]->Vtab[0]->c->origine );
	if( devant ) devant->CalculPlan();
	if( derriere ) derriere->CalculPlan();
}
//----------------------------------------------------------------------------
// Calcul la bounding d'un Noeud ( en fonction des enfants et de lui meme )
void Unoeud_BSP::CalculBounding()
{
	U32 a,b;
	BoundingBox B1;

	for(a=0; a<nbfaces; a++)
		for(b=0; b<Ptab[a]->nbVertex; b++)
			B.TestEtActua( Ptab[a]->Vtab[b]->c->origine );

	if( devant )
	{
		devant->CalculBounding();
		devant->GetBounding( B1 );
		for( a=0; a<8; a++)
			B.TestEtActua( B1.pts[a] );
	}

	if( derriere )
	{
		derriere->CalculBounding();
		derriere->GetBounding( B1 );
		for( a=0; a<8; a++)
			B.TestEtActua( B1.pts[a] );
	}
}
//----------------------------------------------------------------------------
// Renvoie la bounding du noeud
void Unoeud_BSP::GetBounding(BoundingBox &box)
{
	memcpy( &box, &B, sizeof(BoundingBox) );
}
//----------------------------------------------------------------------------
// on va afficher le contenue de TOUT se qu'englobe le noeud en fonction
// de la camera dans le PipeLine de faces 
void Unoeud_BSP::Affiche(U32 flagc)
{
	U32 a;
	Camera *cam = U3D3Monde3D->GetActualCamera();
	Ufloat *vue = cam->GetVue();
	PolyPipeLine *pipe = U3D3Monde3D->GetActualPipe();

	// réactualise les flags de clipping de l'espace délimité par 
	// la bounding Box
//	flagc = cam->ClipInfo( &B, flagc );
	flagc = ClipAll;
	// c'est en dehors de l'ecran  alors on laisse tomber
	if( flagc==ClipCache ) return;	

	// cas de figure où la camera est devant le plan
	if( plan.distance( cam->GetPos() ) >= 0 )
	{
		// on affiche dabord tout se qu'il ya derrière
		if( derriere )
			derriere->Affiche( flagc );

		for(a=0; a<nbfaces; a++)
			pipe->AddPoly( Ptab[a]->GetToDraw() );

		if( devant )
			devant->Affiche( flagc );
	}
	// cas de figure où la camera est derriere le plan
	else
	{
		// on affiche dabord tout se qu'il ya devant
		if( devant )
			devant->Affiche( flagc );

		for(a=0; a<nbfaces; a++)
			pipe->AddPoly( Ptab[a]->GetToDraw() );

		if( derriere )
			derriere->Affiche( flagc );	
	}
}
//----------------------------------------------------------------------------
// meme fonction qu'en haut sauf que la on traite des objets dynamiques ( qui
// bougent en fait )
void Unoeud_BSP::AfficheDyna(U32 flagc)
{
	U32 a;
	Camera *cam = U3D3Monde3D->GetActualCamera();
	Ufloat *vue = cam->GetVue();
	PolyPipeLine *pipe = U3D3Monde3D->GetActualPipe();

	// réactualise les flags de clipping de l'espace délimité par
	// la bounding Box
//	flagc = cam->ClipInfo( &B, flagc );
	flagc = ClipAll;
	// c'est en dehors de l'ecran  alors on laisse tomber
	if( flagc==ClipCache ) return;

	// cas de figure où la camera est devant le plan
	if( plan.distance( cam->GetPos() ) >= 0 )
	{
		// on affiche dabord tout se qu'il ya derrière
		if( derriere )
			derriere->AfficheDyna( flagc );

		for(a=0; a<nbfaces; a++)
			pipe->AddPolyDyna( Ptab[a]->GetToDraw() );

		if( devant )
			devant->AfficheDyna( flagc );
	}
	// cas de figure où la camera est derriere le plan
	else
	{
		// on affiche dabord tout se qu'il ya devant
		if( devant )
			devant->AfficheDyna( flagc );

		for(a=0; a<nbfaces; a++)
			pipe->AddPolyDyna( Ptab[a]->GetToDraw() );

		if( derriere )
			derriere->AfficheDyna( flagc );	
	}
}*/
//----------------------------------------------------------------------------
