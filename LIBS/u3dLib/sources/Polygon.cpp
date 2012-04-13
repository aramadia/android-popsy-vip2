    				 /*-----------------------------------------------------+
					  |				        Polygon.cpp						|
					  |														|
					  |	   Le polygon est l'element de base a tout moteur   |
					  |		    3D ki se respecte vous en conviendrez		|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------+
//									|    Class CoordU3D	  |
//									+---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
CoordU3D::CoordU3D()
{
	Ini()	;
}
//----------------------------------------------------------------------------------------------------------
// met valeurs par defaut 
void CoordU3D::Ini()
{
	vec3_set( origine, 0.f, 0.f, 0.f)		;
	vec3_set( trans,   0.f, 0.f, 0.f)		;
	vec3_set( ecran,   0.f, 0.f, 0.f)		;
	vec3_set( normale, 0.f, 0.f, 0.f)		;
	vec4_set( RVBA,	  1.f, 1.f, 1.f, 1.f)	;
	ClipInfo = ClipNo						;
	utilisateurs = 0						;
	frame_process = (U32)-1					;
}
//----------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------+
//									|   Class VertexU3D   |
//									+---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
VertexU3D::VertexU3D()
{
	c = NULL				;
	U = V = U1 = V1 = 0.0f	;
	suiv = NULL				;
}
//----------------------------------------------------------------------------------------------------------
// attach coordonnée a ca vertex
CoordU3D *VertexU3D::operator=(CoordU3D *coord)
{
	c = coord		;
	c->IncUser()	;

	return c	;
}
//----------------------------------------------------------------------------------------------------------




//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------+
//									|    Class Polygon	  |
//									+---------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
PolygonU3D::PolygonU3D( int nb )
{
	nbVertex = nb						;
	if( nb ) Vtab = new VertexU3D*[nb]	;

	arrete = 0				;
	flag = PolyFLAG_NORM	;

	m = NULL		;
	suiv = NULL		;

	vec4_set( RVBA, 1.f, 1.f, 1.f, 1.f )	;
}
//----------------------------------------------------------------------------------------------------------
PolygonU3D::~PolygonU3D()
{
	if( Vtab ) delete [] Vtab	;
}
//----------------------------------------------------------------------------------------------------------
// definit le nombre de vertex du poly
void PolygonU3D::SetNbVertex( U32 nb )
{
	nbVertex = nb						;
	if( nb ) Vtab = new VertexU3D*[nb]	;
}
//----------------------------------------------------------------------------------------------------------
// calcul normale d'origine
void PolygonU3D::CalcPlan()
{
	plan.Calc( Vtab[0]->c->origine, Vtab[2]->c->origine, Vtab[1]->c->origine )	;
}
//----------------------------------------------------------------------------------------------------------
// créé un new poly si nécessaire en fonction du clipping !
PolygonU3D *PolygonU3D::ClipToPlan(Plan *plan)
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
//		ret->lmapTex = lmapTex;
		vec4_eg( ret->RVBA, RVBA );
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
//----------------------------------------------------------------------------------------------------------
PolygonU3D* PolygonU3D::GetClipped2DPoly( U32 flag, Ufloat val, Viewport &view )
{
	U32			a,nbV=0					;
	bool		Fnew=FALSE				;
	VertexU3D*	dst[20],**V1,**Vtab1	;
	PolygonU3D*	ret						;

	for( a=0; a<nbVertex; a++ )
	{
		CoordU3D *src1 = Vtab[a]->c					;
		CoordU3D *src2 = Vtab[(a+1)%nbVertex]->c	;
		if( (src1->ClipInfo&flag) == 0 )
		{
			dst[nbV++] = Vtab[a]	;
			if( (src2->ClipInfo&flag) ==0 )
				continue	;
		}
		else
		{
			if( src2->ClipInfo&flag )
				continue	;
		}

		Fnew = TRUE	;

		//------------------ alloue new vertex
		VertexU3D*	nouvo;

		nouvo = U3D3Manager->NewVertex()	;
		nouvo->c = U3D3Manager->NewCoord()	;

		//----------------- met flag de clipping a jour
		if( flag==ClipGauche || flag==ClipDroit )
		{
			Ufloat scale = (val - src1->ecran[0]) / (src2->ecran[0] - src1->ecran[0])	;

			//---- interpole les données attachées au vertex !
			nouvo->Interpole2D( Vtab[a], Vtab[(a+1)%nbVertex], scale )							;
			nouvo->c->ecran[0] = val															;
			nouvo->c->ecran[1] = scale*( src2->ecran[1] - src1->ecran[1] ) + src1->ecran[1]		;
			nouvo->c->ecran[2] = scale*( src2->ecran[2] - src1->ecran[2] ) + src1->ecran[2]		;

			// remet flag de clipping a jour pour ce vertex
			if( nouvo->c->ecran[1] < view.GetMinY() )
				nouvo->c->ClipInfo = ClipHaut	;
			else if(nouvo->c->ecran[1] > view.GetMaxY() )
				nouvo->c->ClipInfo = ClipBas	;
		}
		else
		{
			Ufloat scale = (val - src1->ecran[1]) / (src2->ecran[1] - src1->ecran[1])	;

			//---- interpole les données attachées au vertex !
			nouvo->Interpole2D( Vtab[a], Vtab[(a+1)%nbVertex], scale )							;
			nouvo->c->ecran[1] = val															;
			nouvo->c->ecran[0] = scale*( src2->ecran[0] - src1->ecran[0] ) + src1->ecran[0]		;
			nouvo->c->ecran[2] = scale*( src2->ecran[2] - src1->ecran[2] ) + src1->ecran[2]		;

			// remet flag de clipping a jour pour ce vertex
			if( nouvo->c->ecran[0] < view.GetMinX() )
				nouvo->c->ClipInfo = ClipGauche	;
			else if(nouvo->c->ecran[0] > view.GetMaxX() )
				nouvo->c->ClipInfo = ClipDroit	;
		}

		dst[nbV++] = nouvo	;
	}

	if( nbV<3 ) return NULL	;	// moins de 3 sommet alors on se casse
	if( !Fnew ) return this	;	// pas de clipping alors rien de nouvo
	else
	{
		ret = U3D3Manager->NewPoly( nbV )		;	// créé un poly
		ret->flag = this->flag|PolyFLAG_TEMP	;	// on indique que c'est un poly temporaire
		vec4_eg( ret->RVBA, RVBA )				;
		ret->m = this->m						;
		V1 = dst								;
		Vtab1 = ret->Vtab						;
		for(U32 i=0; i<nbV; i++,V1++,Vtab1++)
		{
			*Vtab1 = *V1				;
			(*V1)->utilisateurs++		;	// indique utilisation
			(*V1)->c->utilisateurs++	;	// indique utilisation
		}
	}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------
PolygonU3D* PolygonU3D::GetClippedPolyByCamera()
{
	PolygonU3D*	draw1,*draw2						;
//	VertexU3D*	V[20],*V1[20]						;
/*	VertexU3D** src = V								;
	VertexU3D** dst = V1							;*/
	U32			a									;
	bool		BesGauche,BesDroit,BesHaut,BesBas	;


	Camera *cam = U3D3Monde3D->GetActualCamera()	;
	draw1 = this									;

	//-------------------------> realise le clipping proche si besoin est
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

		BesGauche = draw1->BesoinClip( ClipGauche )	;
		BesDroit  = draw1->BesoinClip( ClipDroit )	;
		BesHaut   = draw1->BesoinClip( ClipHaut )	;
		BesBas    = draw1->BesoinClip( ClipBas )	;
	}
	else
	{
		BesGauche = BesoinClip( ClipGauche )	;
		BesDroit  = BesoinClip( ClipDroit )		;
		BesHaut   = BesoinClip( ClipHaut )		;
		BesBas    = BesoinClip( ClipBas )		;
	}

	if( BesGauche )
	{
		draw2 = draw1->ClipToPlan( cam->GetPlanGauche() )	;
		if( !draw2 ) 
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 )	;
			return NULL											;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 )	;
		draw1 = draw2	;
	}
	if( BesDroit )
	{
		draw2 = draw1->ClipToPlan( cam->GetPlanDroit() )	;
		if( !draw2 )
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 )	;
			return NULL											;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 )	;
		draw1 = draw2	;
	}
	if( BesBas )
	{			
		draw2 = draw1->ClipToPlan( cam->GetPlanHaut() )	;
		if( !draw2 ) 
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 )	;
			return NULL											;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 )	;
		draw1 = draw2	;
	}
	if( BesHaut )
	{
		draw2 = draw1->ClipToPlan( cam->GetPlanBas() )	;
		if( !draw2 )
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 )	;
			return NULL											;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 )	;
		draw1 = draw2	;
	}

	// calcul coords 2D si il le fo
	U32 num = U3D3Monde3D->GetTick()	;
	for( a=0; a<draw1->nbVertex; a++)
	{
		cam->Projet( draw1->Vtab[a], num )	;
	}

	return draw1;
}
//----------------------------------------------------------------------------------------------------------
// TODO : Ben a faire koa !
PolygonU3D* PolygonU3D::GetClippedPolyByViewport( Viewport &view )
{
	PolygonU3D	*draw1,*draw2						;
	bool		BesGauche,BesDroit,BesHaut,BesBas	;

	BesGauche = BesoinClip( ClipGauche )	;
	BesDroit  = BesoinClip( ClipDroit )		;
	BesHaut   = BesoinClip( ClipHaut )		;
	BesBas    = BesoinClip( ClipBas )		;

	draw1 = this	;

	if( BesGauche )
	{
		draw2 = draw1->GetClipped2DPoly( ClipGauche, view.GetMinX(), view )	;
		if( !draw2 ) 
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 )	;
			return NULL											;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 )	;
		draw1 = draw2	;
	}
	if( BesDroit )
	{
		draw2 = draw1->GetClipped2DPoly( ClipDroit, view.GetMaxX(), view )	;
		if( !draw2 )
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 )	;
			return NULL											;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 )	;
		draw1 = draw2	;
	}
	if( BesBas )
	{			
		draw2 = draw1->GetClipped2DPoly( ClipBas, view.GetMaxY(), view )	;
		if( !draw2 ) 
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 )	;
			return NULL											;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 )	;
		draw1 = draw2	;
	}
	if( BesHaut )
	{
		draw2 = draw1->GetClipped2DPoly( ClipHaut, view.GetMinY(), view )	;
		if( !draw2 )
		{
			if( draw1!=this ) U3D3Manager->FreePoly( draw1 )	;
			return NULL											;
		}
		if( (draw2!=draw1) && (draw1!=this) )
			U3D3Manager->FreePoly( draw1 )	;
		draw1 = draw2	;
	}

	return draw1	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie une copie conforme du poly
// TODO : optimiserrrrrrrrrrrrrrrr celaaaaaaaaaaaaa
PolygonU3D* PolygonU3D::GetCopyPoly()
{
	PolygonU3D *ret	;

	ret = U3D3Manager->NewPoly( nbVertex )	;	// créé un poly
	ret->flag = flag|PolyFLAG_TEMP			;	// on indique que c'est un poly temporaire
	ret->arrete = arrete					;
	ret->plan = plan						;
	vec4_eg( ret->RVBA, RVBA )				;
	ret->m = m								;

	for( U32 a=0; a<nbVertex; a++ )
	{
		ret->Vtab[a] = U3D3Manager->NewVertex()						;
		ret->Vtab[a]->utilisateurs = 1								;
		ret->Vtab[a]->c = U3D3Manager->NewCoord()					;
		memcpy( ret->Vtab[a]->c, Vtab[a]->c, sizeof( CoordU3D ) )	;
		ret->Vtab[a]->c->utilisateurs = 1							;
	}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si le segment coupe le poly
#define EPSILON 0.01f
bool PolygonU3D::Intersection( Ufloat debut[3], Ufloat fin[3] )
{
	Ufloat	dist		;
	bool	res1, res2	;
	Plan	planCote	;

	// fait dabord backface culling!
	if( BackFaceCull( debut ) )
		return FALSE	;

	// si les 2 pts sont du même coté du plan de la face alors
	// cette que le segment ne coupe pas cette face !
	dist = plan.distance(debut)			;
	if(  dist < -EPSILON ) res1 = 0		;
	else if( dist > EPSILON ) res1 = 1	;
	else return FALSE					;	// cas ou pt dans le plan du poly -> yaura forcement pas de coupe !

	dist = plan.distance(fin)				;
	if(  dist < -EPSILON ) res2 = FALSE		;
	else if( dist > EPSILON ) res2 = TRUE	;
	else return FALSE						;	// pareil qu'en haut

	if( res1 == res2 )	return FALSE		;

	planCote.Calc( debut, Vtab[nbVertex-1]->c->origine, Vtab[0]->c->origine )	;

	bool preced = TRUE,actua;

	if( planCote.distance( fin ) < -EPSILON ) preced = FALSE	;
	for( U32 a=0; a<nbVertex-1; a++ )
	{
		planCote.Calc( debut, Vtab[a]->c->origine, Vtab[a+1]->c->origine )	;
		if( planCote.distance( fin ) < -EPSILON ) actua = FALSE	;
		else actua = TRUE										;

		if( actua != preced ) return FALSE	;

		preced = actua	;
	}

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si le segment coupe le poly et modifie fin ki sera pil poil a l'endroit de l'intersection !
bool PolygonU3D::IntersectionMod( Ufloat debut[3], Ufloat fin[3] )
{
	Ufloat	dist,dist1	;
	Ufloat	temp[3]		;
	bool	res1, res2	;
	Plan	planCote	;

	// fait dabord backface culling!
	if( BackFaceCull( debut ) )
		return FALSE	;

	// si les 2 pts sont du même coté du plan de la face alors
	// cette que le segment ne coupe pas cette face !
	dist = plan.distance(debut)			;
	if(  dist < -EPSILON ) res1 = 0		;
	else if( dist > EPSILON ) res1 = 1	;
	else return FALSE					;	// cas ou pt dans le plan du poly -> yaura forcement pas de coupe !

	dist1 = plan.distance(fin)				;
	if(  dist1 < -EPSILON ) res2 = FALSE	;
	else if( dist1 > EPSILON ) res2 = TRUE	;
	else return FALSE						;	// pareil qu'en haut

	if( res1 == res2 )	return FALSE		;

	planCote.Calc( debut, Vtab[nbVertex-1]->c->origine, Vtab[0]->c->origine )	;

	bool preced = TRUE,actua;

	if( planCote.distance( fin ) < -EPSILON ) preced = FALSE	;
	for( U32 a=0; a<nbVertex-1; a++ )
	{
		planCote.Calc( debut, Vtab[a]->c->origine, Vtab[a+1]->c->origine )	;
		if( planCote.distance( fin ) < -EPSILON ) actua = FALSE	;
		else actua = TRUE										;

		if( actua != preced ) return FALSE	;

		preced = actua	;
	}

	//------------- interpole sur la droite a l'endroit de l'intersection
	Ufloat scale = -dist / ( dist1 - dist )	;

	vec3_sub( temp, fin, debut )	;
	vec3_mul( temp, temp, scale )	;
	vec3_add( fin, temp, debut )	;	// op la fin est sur le poly now !

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si les 2 point font partie du poly
bool PolygonU3D::IsElement( CoordU3D *pt1, CoordU3D *pt2 )
{
	bool f1=FALSE,f2=FALSE	;

	for( U32 a=0; a<nbVertex; a++ )
	{
		if( Vtab[a]->c==pt1 )		f1=TRUE	;
		else if( Vtab[a]->c==pt2 )	f2=TRUE	;
	}

	if( f1 && f2 )	return TRUE		;
	else			return FALSE	;
}
//----------------------------------------------------------------------------------------------------------
