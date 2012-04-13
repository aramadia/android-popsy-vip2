    	 /*-----------------------------------------------------+
		  |				       LmapsManager.cpp					|
		  |														|
		  |	 Bon ca va gérer sec les lightmaps ici alors faites |
		  |					   attention !!!					|
		  |														|	
		  | 	U2^PoPsy TeAm 1999								|	
		  +-----------------------------------------------------*/

#include "U3D3.h"

#define FACTEUR_DIV 8	

#ifdef _LMAPS
//-------------------------------------------------------------------------
//						+---------------------+
//						|    Les Fonctions    |
//						+---------------------+
//-------------------------------------------------------------------------


//----------------------------------------------------------------------------
//								Lumel
//----------------------------------------------------------------------------
void Lumel::Conv( U8 dest[3] )
{
	for( U32 a=0; a<3; a++ )
	{
		if( RVB[a]<0.f ) dest[a] = 0; 
		else if( 255.f*RVB[a]>255.f ) dest[a] = 255;
		else dest[a] = ceil( 255.f*RVB[a] );
	}
}
//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
//								Lightmap
//----------------------------------------------------------------------------
Lightmap::Lightmap( U32 larg, U32 haut )
{
	hauteur = haut;
	largeur = larg;
	suiv = NULL;
	data = new Lumel[ hauteur*largeur ];
}
//-------------------------------------------------------------------------
Lightmap::~Lightmap()
{
	delete [] data;
}
//-------------------------------------------------------------------------
void inline Lightmap::Add( U32 u, U32 v, Ufloat rvb[3] )
{
	data[ v*hauteur +u ].Add( rvb );
}
//-------------------------------------------------------------------------
// copie lightmap dans une surface RVB normale
// ATTENTION dest doit etre alloué comme il fo 
void Lightmap::ToSurface( U8 *dest )
{
	for( U32 a=0; a<hauteur*largeur; a++,dest+=3 )
		data[ a ].Conv( dest );
}
//-------------------------------------------------------------------------
// va lisser la lightmap ( util can ya des ombres portées dessus ! )
void Lightmap::Smooth()
{
	Lumel *tmp = new Lumel[ hauteur*largeur ];

	for( S32 v=0; v<(S32)hauteur; v++ )
		for( S32 u=0; u<(S32)largeur; u++ )
		{
			Ufloat nbfact = 0.f;
			Ufloat coul[3];
			vec3_eg( coul, data[ v*hauteur +u].RVB );
			nbfact++;
			if( v-1 > 0 )
			{
				vec3_add( coul, coul, data[ (v-1)*hauteur +u].RVB );
				nbfact++;
			}
			if( v+1 < (S32)hauteur )
			{
				vec3_add( coul, coul, data[ (v+1)*hauteur +u].RVB );
				nbfact++;
			}
			if( u-1 > 0 )
			{
				vec3_add( coul, coul, data[ v*hauteur +(u-1)].RVB );
				nbfact++;
			}
			if( u+1 < (S32)largeur )
			{
				vec3_add( coul, coul, data[ v*hauteur +(u+1)].RVB );
				nbfact++;
			}

			vec3_eg( tmp[ v*hauteur +u ].RVB, coul );
		}

	// copie lightmap blurrée
	for( U32 a=0; a<hauteur*largeur; a++)
		vec3_eg( data[a].RVB,tmp[a].RVB );

	delete tmp;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//							LightmapManager
//----------------------------------------------------------------------------
LightmapManager::LightmapManager()
{
	liste = NULL;
	nbElem = 0;
}
//----------------------------------------------------------------------------
LightmapManager::~LightmapManager()
{
}
//----------------------------------------------------------------------------
void LightmapManager::Add( Lightmap *l )
{
	l->suiv = liste;
	liste = l;
	nbElem++;
}
//----------------------------------------------------------------------------
// renvoie une nouvelle lightmap pour le poly donné
Lightmap *LightmapManager::GetNew( PolygonU3D &p )
{
	Lightmap *ret;
	Ufloat mat[4][4];
	Ufloat maxU=-1e+19f,minU=1e+19f;
	Ufloat maxV=-1e+19f,minV=1e+19f;
	U32 a;

	// pas de texture alors tchao !
	if( p.m->Texflag == MatTexture0 ) return NULL;

	p.lmapTex = NULL;

	ret = new Lightmap( 32, 32 );
	Add( ret );

	// va voir si on partage pas deja des vertex avec d'autre polygone
	// et si oui ben on en créé un nouvo car il aura pas les memes 
	// coords de mapping ( pour la lightmap )
	for( a=0; a<p.nbVertex; a++)
	{
		if( p.Vtab[a]->utilisateurs> 1 )
		{
			VertexU3D *tmp;
			tmp = U3D3Manager->NewVertex();
			tmp->c = p.Vtab[a]->c;
			tmp->V = p.Vtab[a]->V;
			tmp->U = p.Vtab[a]->U;
			tmp->utilisateurs = 1;
			p.Vtab[a]->utilisateurs--;
			p.Vtab[a] = tmp;
		}
	}


	// va mettre en place les coord dans l'espace de la texture
	for( a=0; a<p.nbVertex; a++)
	{
		p.plan.GetTransMatrix( mat );
		// U1 et V1 = coord 2D dans le plan en fait
		p.Vtab[a]->U1 = vec3_dot( mat[0], p.Vtab[a]->c->origine );
		p.Vtab[a]->V1 = vec3_dot( mat[1], p.Vtab[a]->c->origine );
	}

	// va chercher les bounding des coords
	for( a=0; a<p.nbVertex; a++)
	{
		if( p.Vtab[a]->U1 > maxU ) maxU = p.Vtab[a]->U1;
		if( p.Vtab[a]->U1 < minU ) minU = p.Vtab[a]->U1;
		if( p.Vtab[a]->V1 > maxV ) maxV = p.Vtab[a]->V1;
		if( p.Vtab[a]->V1 < minV ) minV = p.Vtab[a]->V1;
	}

	// va clamper les coords comme il fo !
	for( a=0; a<p.nbVertex; a++)
	{
		p.Vtab[a]->U1 = .95f*(p.Vtab[a]->U1-minU) / ( maxU-minU ) +.025f;
		p.Vtab[a]->V1 = .95f*(p.Vtab[a]->V1-minV) / ( maxV-minV ) +.025f;
		p.Vtab[a]->LU = ceil( (p.Vtab[a]->U1)*32.f );
		p.Vtab[a]->LV = ceil( (p.Vtab[a]->V1)*32.f );
	}

	p.m->SetTexture( MatTexture2 );	// enclanche multi texturing 

	return ret;
}
//----------------------------------------------------------------------------
// va calculer le lighting sur la lightmap du poly !
void LightmapManager::CalculLightMap( PolygonU3D &p )
{
	Ufloat DnormalDu[3];
	Ufloat DnormalDv[3];
	Ufloat DposDu[3];
	Ufloat DposDv[3];
	CoordU3D *c1,*c2,*c3;

	// pas de texture alors tchao !
	if( p.m->Texflag == MatTexture0 ) return;

	c1 = p.Vtab[0]->c;
	c2 = p.Vtab[1]->c;
	c3 = p.Vtab[2]->c;

	Ufloat denom = ((Ufloat)p.Vtab[1]->LU-(Ufloat)p.Vtab[0]->LU)*((Ufloat)p.Vtab[2]->LV-(Ufloat)p.Vtab[1]->LV) -
				   ((Ufloat)p.Vtab[2]->LU-(Ufloat)p.Vtab[1]->LU)*((Ufloat)p.Vtab[1]->LV-(Ufloat)p.Vtab[0]->LV);
	denom = 1.f / denom;

	// calcul les gradients dans l'espace texture ( plan quoa )
	for( U32 a=0; a<3; a++)
	{
		Ufloat tmpT;
		if( p.Vtab[0]->LU - p.Vtab[2]->LU != 0 )
			tmpT = (c1->origine[a]-c3->origine[a]) / ( (Ufloat)p.Vtab[0]->LU - (Ufloat)p.Vtab[2]->LU );
		else
			tmpT = (c1->origine[a]-c2->origine[a]) / ( (Ufloat)p.Vtab[0]->LU - (Ufloat)p.Vtab[1]->LU );
		DposDu[a] = ( (c2->origine[a]-c1->origine[a])*((Ufloat)p.Vtab[2]->LV-(Ufloat)p.Vtab[1]->LV) -
					  (c3->origine[a]-c2->origine[a])*((Ufloat)p.Vtab[1]->LV-(Ufloat)p.Vtab[0]->LV) )*denom;
		if( (tmpT<0.f && DposDu[a]>0.f) || (tmpT>0.f && DposDu[a]<0.f) )
			DposDu[a] = -DposDu[a];


		if( p.Vtab[0]->LV - p.Vtab[2]->LV != 0 )
			tmpT = (c1->origine[a]-c3->origine[a]) / ( (Ufloat)p.Vtab[0]->LV - (Ufloat)p.Vtab[2]->LV );
		else
			tmpT = (c1->origine[a]-c2->origine[a]) / ( (Ufloat)p.Vtab[0]->LV - (Ufloat)p.Vtab[1]->LV );
		DposDv[a] = ( (c2->origine[a]-c1->origine[a])*((Ufloat)p.Vtab[2]->LU-(Ufloat)p.Vtab[1]->LU) -
					  (c3->origine[a]-c2->origine[a])*((Ufloat)p.Vtab[1]->LU-(Ufloat)p.Vtab[0]->LU) )*denom;
		if( (tmpT<0.f && DposDv[a]>0.f) || (tmpT>0.f && DposDv[a]<0.f) )
			DposDv[a] = -DposDv[a];

		if( p.Vtab[0]->LU - p.Vtab[2]->LU != 0 )
			tmpT = (c1->normale[a]-c3->normale[a]) / ( (Ufloat)p.Vtab[0]->LU - (Ufloat)p.Vtab[2]->LU );
		else
			tmpT = (c1->normale[a]-c2->normale[a]) / ( (Ufloat)p.Vtab[0]->LU - (Ufloat)p.Vtab[1]->LU );
		DnormalDu[a] = ( (c2->normale[a]-c1->normale[a])*((Ufloat)p.Vtab[2]->LV-(Ufloat)p.Vtab[1]->LV) -
					  (c3->normale[a]-c2->normale[a])*((Ufloat)p.Vtab[1]->LV-(Ufloat)p.Vtab[0]->LV) )*denom;
		if( (tmpT<0.f && DnormalDu[a]>0.f) || (tmpT>0.f && DnormalDu[a]<0.f) )
			DnormalDu[a] = -DnormalDu[a];
/*		DnormalDu[a] = ( (c1->normale[a]-c3->normale[a])*((Ufloat)p.Vtab[1]->LV-(Ufloat)p.Vtab[2]->LV) -
						 (c2->normale[a]-c3->normale[a])*((Ufloat)p.Vtab[0]->LV-(Ufloat)p.Vtab[2]->LV) )*denom;*/

		if( p.Vtab[0]->LV - p.Vtab[2]->LV != 0 )
			tmpT = (c1->normale[a]-c3->normale[a]) / ( (Ufloat)p.Vtab[0]->LV - (Ufloat)p.Vtab[2]->LV );
		else
			tmpT = (c1->normale[a]-c2->normale[a]) / ( (Ufloat)p.Vtab[0]->LV - (Ufloat)p.Vtab[1]->LV );
		DnormalDv[a] = ( (c2->normale[a]-c1->normale[a])*((Ufloat)p.Vtab[2]->LU-(Ufloat)p.Vtab[1]->LU) -
					  (c3->normale[a]-c2->normale[a])*((Ufloat)p.Vtab[1]->LU-(Ufloat)p.Vtab[0]->LU) )*denom;
		if( (tmpT<0.f && DnormalDv[a]>0.f) || (tmpT>0.f && DnormalDv[a]<0.f) )
			DnormalDv[a] = -DnormalDv[a];
/*		DnormalDv[a] = ( (c1->normale[a]-c3->normale[a])*((Ufloat)p.Vtab[1]->LU-(Ufloat)p.Vtab[2]->LU) -
						 (c2->normale[a]-c3->normale[a])*((Ufloat)p.Vtab[0]->LU-(Ufloat)p.Vtab[2]->LU) )*denom;*/
	}

	// allé zou calcul la lightmap en fct des lights des occlusions etc...
	Ufloat posDeb[3];
	Ufloat normalDeb[3];

	// fait un preset pour se positionné au haut a gauche de la lightmap
	vec3_eg( posDeb, c1->origine );
	vec3_eg( normalDeb, c1->normale );
	for( a=0; a<3; a++)
	{
		posDeb[a] += (DposDv[a])*(-(0.49f+(Ufloat)p.Vtab[0]->LV)) + DposDu[a]*(-(0.49f+(Ufloat)p.Vtab[0]->LU));
		normalDeb[a] += DnormalDv[a]*(-(0.49f+(Ufloat)p.Vtab[0]->LV)) + DnormalDu[a]*(-(0.49f+(Ufloat)p.Vtab[0]->LU));
	}

	// Aller zou on calcul !!!!!!!!!!
	for( a=0; a<U3D3Monde3D->GetNbLight(); a++)
//	for( a=0; a<1; a++)
	{
		LightU3D *li = U3D3Monde3D->GetLight(a);
		Ufloat posV[3];
		Ufloat normalV[3];

		if( p.BackFaceCull(li->GetPos()) )
			continue;

		vec3_eg( posV, posDeb );
		vec3_eg( normalV, normalDeb );

		for( U32 v=0; v<p.lmap->GetHauteur(); v++)
		{
			Ufloat pos[3];
			Ufloat normal[3];
			vec3_eg( pos, posV );
			vec3_eg( normal, normalV );
			for(U32 u=0; u<p.lmap->GetLargeur(); u++)
			{
				if( !U3D3Monde3D->Intersec( li->GetPos(), pos ) )
				{
					vec3_normalize( normal );
					Ufloat rvb[3],vec[3],i;
					
					vec3_sub( vec, pos, li->GetPos() );
					vec3_normalize( vec );
					li->GetRVB( rvb );
					i = fabs( vec3_dot( p.plan.normal, vec ) ) * li->ComputeIntensity( pos );

					vec3_mul( rvb, rvb, i );
					p.lmap->Add( u, v, rvb );
				}
				vec3_add( pos, pos, DposDu );
				vec3_add( normal, normal, DnormalDu );
			}
			vec3_add( posV, posV, DposDv );
			vec3_add( normalV, normalV, DnormalDv );
		}
	}

	// la texture pour la lightmap n'a pas encore été allouée
	if( !p.lmapTex )
	{
		char buf[20];
		TextureU3D *nouvo;

		sprintf( buf, "%sL", U3D3TexManager->GetNom( p.m->Tex1 ) );
		nouvo = new TextureU3D(BILINEAR_TEXTURE);
		U3D3TexManager->Add( nouvo, buf );		// rajoute la texture dans le manager
		U3D3TexManager->AddUse( nouvo );		// dit qu'on l'utilise 
		nouvo->Create( 32, 32 );
		p.lmap->Smooth();
		p.lmap->ToSurface( nouvo->data );
		p.lmapTex = nouvo;
		nouvo->Load();							// load texture dans la carte
	}
}
//----------------------------------------------------------------------------
#endif