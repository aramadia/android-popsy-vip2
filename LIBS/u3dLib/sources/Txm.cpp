		    				 /*-----------------------------------------------------+
							  |				         TXM.h						    |
							  |														|
							  | Je sais c'est une methode de Monde3D mais bon elle  |
							  |   est tellement imposante que j'ai décidé de la 	|
							  |		foutre dans un fichier indépendant vala	        |
							  |														|
							  | 	U2^PoPsy TeAm 1999								|
							  +-----------------------------------------------------*/

#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//										+---------------------+
//										|    Les Fonctions    |
//										+---------------------+
//----------------------------------------------------------------------------------------------------------------------------------------
bool MondeU3D::LoadTXM( LPCSTR nom, U32 flag )
{
	FILE *file = fopen(nom, "r");

	if( file ) {
		try
		{
			CArchive ar(file)	;

			LoadTXM( ar, flag )	;

			ar.Close()		;
			fclose(file);
		} catch (...) {
		#ifdef _DEBUG
			DebugErrorMess( "<U3D> Error loading %s TXM file\n", nom )	;
		#endif
			return FALSE	;
		}
	} else {
		#ifdef _DEBUG
			DebugErrorMess( "<U3D> Error loading %s TXM file\n", nom )	;
		#endif
			return FALSE	;
	}

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
void MondeU3D::LoadTXM( CArchive &ar, U32 flag )
{
	char		bufligne[500]		;
	char		mname[256]			;
	char		*chaine				;
	char		buf[256]			;
	bool		good=FALSE			;
	bool		fin=FALSE			;
	MaterialU3D *mat=NULL			;
	CString		MaskOpen			;
	U32			flagOpenTex = flag	;


	MondeU3D *Save = SetActualMondeU3D( this )	;	// met ce world comme actuel et sauve le précédent


	while( !fin )
	{
		if( !ar.ReadString( bufligne,500 )	)	fin = TRUE	;
		chaine = bufligne									;

		//--------- cherche un caractere valide
		U32 a;
		for(a=0; a<500; a++)
		{
			char C = chaine[a]	; 
			if( (C=='\r') || (C=='\n') || (C==';') || (C=='/') )
			{
				good = FALSE	;
				break			;
			}
			else if( (C==' ') || (C=='\t') )
			{
				continue	;
			}
			good = TRUE	;
			break		;
		}


		//--------- analyse la ligne et la traite

		if( !good ) continue	;	// passe a la ligne suivante

		if( strncasecmp(&bufligne[a],"material",8) == 0)
		{
			if( mat )	mat->Confirm()			;
			sscanf(&bufligne[a+8],"%s",&mname)      ;
			mat = GetOrCreateMat( mname )		;
		}
		else if( strncasecmp(&bufligne[a],"2side",5) ==0)
		{
			if( mat )	mat->Side()	;
		}
		else if( strncasecmp(&bufligne[a],"Couleur Solid",13) == 0)
		{
			U32 r=0,v=0,b=0,A=255	;
			S32 i=a+13				;
			while(chaine[i]!='R') i++	;
			i+=2;
			sscanf(&bufligne[i],"%d",&r)	;
			while(chaine[i]!='V') i++	;
			i+=2;
			sscanf(&bufligne[i],"%d",&v)	;
			while(chaine[i]!='B') i++	;
			i+=2;
			sscanf(&bufligne[i],"%d",&b)	;
			while( (chaine[i]!='A') && (chaine[i]!='\r') && (chaine[i]!='\n') && (chaine[i]!='/') && (chaine[i]!=';') )
				i++	;
			if( (chaine[i]!='\r') && (chaine[i]!='\n') && (chaine[i]!='/') && (chaine[i]!=';') )
			{
				i+=2						;
				sscanf(&bufligne[i],"%d",&A)	;
			}

			if( mat )	mat->SetColorFromTXM( r, v, b, A )	;
			continue	;
		}
		else if( strncasecmp(&bufligne[a],"Shading",7) == 0)
		{
			U32 dest=0	;
			S32 i=a+7	;
			while(chaine[i]==' ') i++	;
			if( strncasecmp(&chaine[i],"Fil",3)==0 )
			{
				dest=1						;
				while(chaine[i]!=' ') i++	;
			}
			if( mat )
			{
				while(chaine[i]==' ') i++	;
				if( strncasecmp(&chaine[i],"Flat",4) == 0)
					mat->SetShading( MatShadingFlat	)	;
				else if( strncasecmp(&chaine[i],"Gouraud",7) == 0)
					mat->SetShading( MatShadingGouraud	)	;
				else if( strncasecmp(&chaine[i],"Rien",4) == 0)
					mat->SetShading( MatShadingRien	)	;
				else if( strncasecmp(&chaine[i],"Phong",5) == 0)
					mat->SetShading( MatShadingPhong	)	;
			}
			continue	;
		}
		else if( strncasecmp(&bufligne[a],"Transp",6) == 0)
		{
			S32 i=a+6	;
			while(chaine[i]==' ') i++	;
			if( mat )
			{
				while(chaine[i]==' ') i++	;
				if( strncasecmp(&chaine[i],"Trou",4) == 0)
					mat->SetTransp( MatTranspTrou )	;
				else if( strncasecmp(&chaine[i],"Add",3) == 0)
					mat->SetTransp( MatTranspAdd )	;
				else if( strncasecmp(&chaine[i],"Mul",3) == 0)
					mat->SetTransp( MatTranspMul )	;
				else if( strncasecmp(&chaine[i],"AAdd",4) == 0)
					mat->SetTransp( MatTranspAddAlpha )	;
				else if( strncasecmp(&chaine[i],"AMul",4) == 0)
					mat->SetTransp( MatTranspMulAlpha )	;
				else if( strncasecmp(&chaine[i],"Alpha",3) == 0)
					mat->SetTransp( MatTranspAlpha )	;
			}
			continue	;
		}
		else if( strncasecmp(&bufligne[a],"TexTransp2",10) == 0 )
		{
			S32 i=a+10	;
			while(chaine[i]==' ') i++	;
			if( mat )
			{
				while(chaine[i]==' ') i++	;
				if( strncasecmp(&chaine[i],"Trou",4) == 0)
					mat->SetTranspTex2( MatTranspTrou )	;
				else if( strncasecmp(&chaine[i],"Add",3) == 0)
					mat->SetTranspTex2( MatTranspAdd )	;
				else if( strncasecmp(&chaine[i],"Mul",3) == 0)
					mat->SetTranspTex2( MatTranspMul )	;
				else if( strncasecmp(&chaine[i],"AAdd",4) == 0)
					mat->SetTranspTex2( MatTranspAddAlpha )	;
				else if( strncasecmp(&chaine[i],"AMul",4) == 0)
					mat->SetTranspTex2( MatTranspMulAlpha )	;
				else if( strncasecmp(&chaine[i],"Alpha",3) == 0)
					mat->SetTranspTex2( MatTranspAlpha )	;
			}
			continue	;
		}
		else if( strncasecmp(&bufligne[a],"Tex1",4) == 0)
		{
			if( mat )
			{
				S32 i=a+4	;
				while(chaine[i]==' ') i++	;
				sscanf(&chaine[i],"%s",&buf)							;
				mat->SetTexture1( TextureManager::GetForUse( buf ) )	;
			}
		}
		else if( strncasecmp(&bufligne[a],"Tex2",4) == 0)
		{
			if( mat )
			{
				S32 i=a+4	;
				while(chaine[i]==' ') i++	;
				sscanf(&chaine[i],"%s",&buf)							;
				mat->SetTexture2( TextureManager::GetForUse( buf ) )	;
			}
		}
		else if( strncasecmp(&bufligne[a],"PosTex1",7) ==0)
		{
			if( mat )
			{
				S32 i=a+7	;
				Ufloat u,v	;
				while(chaine[i]!='U') i++	;
				i+=2	;
				sscanf(&bufligne[i],"%f",&u)	;
				while(chaine[i]!='V') i++	;
				i+=2	;
				sscanf(&bufligne[i],"%f",&v)	;

				mat->SetTex1PosFromTXM( u, v )	;
			}
			continue	;
		}
		else if( strncasecmp(&bufligne[a],"PosTex2",7) ==0)
		{
			if( mat )
			{
				S32 i=a+7	;
				Ufloat u,v	;
				while(chaine[i]!='U') i++		;
				i+=2							;
				sscanf(&bufligne[i],"%f",&u)	;
				while(chaine[i]!='V') i++		;
				i+=2							;
				sscanf(&bufligne[i],"%f",&v)	;

				mat->SetTex2PosFromTXM( u, v )	;
			}
			continue	;
		}
		else if( strncasecmp(&bufligne[a],"DeltaTex1",9) ==0)
		{
			if( mat )
			{
				S32 i=a+9	;
				Ufloat u,v	;
				while(chaine[i]!='U') i++		;
				i+=2							;
				sscanf(&bufligne[i],"%f",&u)	;
				while(chaine[i]!='V') i++		;
				i+=2							;
				sscanf(&bufligne[i],"%f",&v)	;

				mat->SetTex1DeltaFromTXM( u, v )	;
			}
			continue	;
		}
		else if( strncasecmp(&bufligne[a],"DeltaTex2",9) ==0)
		{
			if( mat )
			{
				S32 i=a+9	;
				Ufloat u,v	;
				while(chaine[i]!='U') i++		;
				i+=2							;
				sscanf(&bufligne[i],"%f",&u)	;
				while(chaine[i]!='V') i++		;
				i+=2							;
				sscanf(&bufligne[i],"%f",&v)	;

				mat->SetTex2DeltaFromTXM( u, v )	;
			}
			continue	;
		}
		else if( strncasecmp(&bufligne[a],"Repertoire", 10) ==0)
		{
			sscanf(&bufligne[a+10],"%s",&buf)		;
			TextureManager::SetRepertoire( buf )	;
		}
		else if( strncasecmp(&bufligne[a],"TileTex", 7) ==0 )
		{
			flagOpenTex |= TILE_TEXTURE	;
		}
		else if( strncasecmp(&bufligne[a],"MaskOpen", 8) ==0 )
		{
			sscanf(&bufligne[a+8],"%s",&buf)		;
			MaskOpen = buf							;
			flagOpenTex |= ALPHA_TEXTURE			;
		}
		else if( strncasecmp(&bufligne[a],"Open", 4) ==0)
		{
			TextureU3D	*textmp	;

			sscanf(&bufligne[a+4],"%s",&buf)						;
			textmp = TextureManager::OpenJpg( buf, flagOpenTex )	;

			if( textmp->IsAlphaTexture() )
				TextureManager::OpenMask( textmp, (LPCSTR)MaskOpen )	;

			flagOpenTex = flag		;	// remet flag par defaut
		}
		else if( strncasecmp(&bufligne[a],"EndTXM", 6) ==0)
		{
			fin = TRUE	;	// c'est la fin du fichier !
		}

	}
	if( mat )	mat->Confirm()	;


	ApplyMat()	;	// !!!!!!!!!!! TMPPPPPP !!!!!! un truc ki partira kon j'aurai amélioré le TOOLS !


	SetActualMondeU3D( Save )	;	// remete l'ancien monde !
}
//----------------------------------------------------------------------------------------------------------------------------------------
