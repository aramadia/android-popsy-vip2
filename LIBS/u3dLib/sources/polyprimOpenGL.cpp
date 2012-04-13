    	 /*-----------------------------------------------------+
		  |				     polyprimOpenGL.cpp					|
		  |														|
		  |	 Toutes les primitives d'affichage de polygones     |
		  |					mais pour OpenGL					|
		  |														|	
		  | 	U2^PoPsy TeAm 1999								|	
		  +-----------------------------------------------------*/

#include "U3D3.h"
#include <GL/glu.h>

//-------------------------------------------------------------------------
//						+---------------------+
//						|    Les Fonctions    |
//						+---------------------+
//-------------------------------------------------------------------------
/*
//----------------------------------------------------------------------------
// affiche un poly convex en flat ave la couleur coul
void DrawFlatPolyFlatU3D(PolygonU3D *p, Ufloat R, Ufloat V, Ufloat B )
{
	Ufloat alpha;
	U32 a;


	switch( p->m->Trflag[0] )
	{
		default:
			glColor3f( R, V, B );
			glBegin( GL_POLYGON );
			for(a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			break;

		case MatTranspAdd:
			glColor3f( R, V, B ); 
			glEnable( GL_BLEND );
			glBlendFunc( GL_ONE, GL_ONE );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspMul:
			glColor3f( R, V, B ); 
			glEnable( GL_BLEND );
			glBlendFunc( GL_ZERO, GL_SRC_COLOR );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspAddAlpha:
			glColor4f( R, V, B, p->m->A ); 
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspMulAlpha:
			alpha = p->m->A;
			glColor3f( R*alpha, V*alpha, B*alpha ); 
			glEnable( GL_BLEND );
			glBlendFunc( GL_ZERO, GL_SRC_COLOR );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspAlpha:
			glColor4f( R, V, B, p->m->A );
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;
	}

	if( p->m->Fildefer )
		DrawFlatLigneU3D( p, R, V, B );
}
//----------------------------------------------------------------------------
void DrawFlatPolyGouraudU3D(PolygonU3D *p )
{
	Ufloat alpha;
	Ufloat R,V,B,A;
	U32 a;

	R = p->RVB[0];
	V = p->RVB[1];
	B = p->RVB[2];
	A = p->m->A;
	switch( p->m->Trflag[0] )
	{
		default:
			glBegin( GL_POLYGON );
			for(a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glColor3f( R*c->RVB[0], V*c->RVB[1], B*c->RVB[2] ); 
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			break;

		case MatTranspAdd:
			glEnable( GL_BLEND );
			glBlendFunc( GL_ONE, GL_ONE );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glColor3f( R*c->RVB[0], V*c->RVB[1], B*c->RVB[2] ); 
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspMul:
			glEnable( GL_BLEND );
			glBlendFunc( GL_ZERO, GL_SRC_COLOR );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glColor3f( R*c->RVB[0], V*c->RVB[1], B*c->RVB[2] ); 
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspAddAlpha:
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glColor4f( R*c->RVB[0], V*c->RVB[1], B*c->RVB[2], A*c->A ); 
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspMulAlpha:
			alpha = p->m->A;
			glEnable( GL_BLEND );
			glBlendFunc( GL_ZERO, GL_SRC_COLOR );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glColor4f( R*c->RVB[0], V*c->RVB[1], B*c->RVB[2], A*c->A ); 
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspAlpha:
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glColor4f( R*c->RVB[0], V*c->RVB[1], B*c->RVB[2], A*c->A ); 
				glVertex3f( c->ecran[0], c->ecran[1], -c->ecran[2] );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;
	}

	if( p->m->Fildefer )
		DrawGouraudLigneU3D( p );
}
//----------------------------------------------------------------------------
void DrawTexturePolyFlatU3D( PolygonU3D *p, Ufloat R, Ufloat V, Ufloat B )
{
	Ufloat alpha;
	U32 a;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, p->m->Tex1->BindNum );
	switch( p->m->Trflag[0] )
	{
		default:
			glColor3f( R, V, B );
			glBegin( GL_POLYGON );
			for(a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			break;

		case MatTranspAdd:
			glColor3f( R, V, B );
			glEnable( GL_BLEND );
			glBlendFunc( GL_ONE, GL_ONE );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspMul:
			glColor3f( R, V, B );
			glEnable( GL_BLEND );
			glBlendFunc( GL_ZERO, GL_SRC_COLOR );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspAddAlpha:
			glColor4f( R, V, B, p->m->A );
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspMulAlpha:
			alpha = p->m->A;
			glColor3f( alpha*R, alpha*V, alpha*B );
			glEnable( GL_BLEND );
			glBlendFunc( GL_ZERO, GL_SRC_COLOR );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspAlpha:
			glColor4f( R, V, B, p->m->A );
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;
	}
	glDisable(GL_TEXTURE_2D);		

	if( p->m->Fildefer )
		DrawFlatLigneU3D( p, R, V, B );
}
//----------------------------------------------------------------------------
void DrawTexturePolyGouraudU3D( PolygonU3D *p )
{
	Ufloat alpha;
	U32 a;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, p->m->Tex1->BindNum );
	switch( p->m->Trflag[0] )
	{
		default:
			glBegin( GL_POLYGON );
			for(a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				glColor3f( c->RVB[0], c->RVB[1], c->RVB[2] );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			break;

		case MatTranspAdd:
			glEnable( GL_BLEND );
			glBlendFunc( GL_ONE, GL_ONE );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				glColor3f( c->RVB[0], c->RVB[1], c->RVB[2] );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspMul:
			glEnable( GL_BLEND );
			glBlendFunc( GL_ZERO, GL_SRC_COLOR );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				glColor3f( c->RVB[0], c->RVB[1], c->RVB[2] );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspAddAlpha:
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				glColor4f( c->RVB[0], c->RVB[1], c->RVB[2], c->A );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspMulAlpha:
			alpha = p->m->A;
			glEnable( GL_BLEND );
			glBlendFunc( GL_ZERO, GL_SRC_COLOR );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				glColor4f( c->RVB[0], c->RVB[1], c->RVB[2], c->A );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;

		case MatTranspAlpha:
			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
			glBegin( GL_POLYGON );
			for( a=0; a<p->nbVertex; a++)
			{
				CoordU3D *c = p->Vtab[a]->c;
				glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
				glColor4f( c->RVB[0], c->RVB[1], c->RVB[2], c->A );
				Ufloat z = c->ecran[2];
				glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
			}
			glEnd();
			glDisable( GL_BLEND );
			break;			
	}
	glDisable( GL_TEXTURE_2D );	

	if( p->m->Fildefer )
		DrawFlatLigneU3D( p, 1.f, 1.f, 1.f );
}
//----------------------------------------------------------------------------
void DrawTexturePolyPhongU3D( PolygonU3D *p )
{
}
//----------------------------------------------------------------------------
void Draw2TexturePolyU3D( PolygonU3D *p )
{
	U32 a;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, p->lmapTex->BindNum );
	glBegin( GL_POLYGON );
		for(a=0; a<p->nbVertex; a++)
		{
			CoordU3D *c = p->Vtab[a]->c;
			glTexCoord2f( p->Vtab[a]->U1, p->Vtab[a]->V1 );
			glColor3f( c->RVB[0], c->RVB[1], c->RVB[2] );
			Ufloat z = c->ecran[2];
			glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
		}
	glEnd();

/*	glBindTexture(GL_TEXTURE_2D, p->m->Tex1->BindNum );
	glEnable( GL_BLEND );
	glBlendFunc( GL_ZERO, GL_SRC_COLOR );
	glBegin( GL_POLYGON );
		for(a=0; a<p->nbVertex; a++)
		{
			CoordU3D *c = p->Vtab[a]->c;
			glTexCoord2f( p->Vtab[a]->U, p->Vtab[a]->V );
			glColor3f( c->RVB[0], c->RVB[1], c->RVB[2] );
			Ufloat z = c->ecran[2];
			glVertex4f( c->ecran[0]*z, c->ecran[1]*z, -z*z, z );
		}
	glEnd();
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );*/
//}
//----------------------------------------------------------------------------
