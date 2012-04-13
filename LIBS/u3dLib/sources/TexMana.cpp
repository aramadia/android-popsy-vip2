		    				 /*-----------------------------------------------------+
							  |				         TexMana.h						|
							  |														|
							  |	  C'est un Manager de texture ou plutot d'images en |
							  |	 en tout genre qui permet de renvoyer les nimages   |
							  |			que l'on veut avoir en indiquant leur nom.	|
							  |														|
							  | 	U2^PoPsy TeAm 1999								|
							  +-----------------------------------------------------*/


#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+------------------------+
//									|    Variables Static    |
//									+------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------

TextureListe*	TextureManager::Liste = NULL		;
TextureListe*	TextureManager::IteratorPos = NULL	;
U32		TextureManager::ForceFlag = 0		;
char *          TextureManager::Repertoir = NULL;



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+---------------------+
//									|    Les Fonctions    |
//									+---------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------


void my_error_exit(j_common_ptr cinfo) {
	char buf[JMSG_LENGTH_MAX];
	(*cinfo->err->format_message)(cinfo, buf);
	exit(-1);
}
void noop(j_decompress_ptr cinfo) {}
int fill(j_decompress_ptr cinfo)
{
        if (cinfo->src->bytes_in_buffer >= 0) {
                return TRUE;
        } else {
                return FALSE;
        }
}
void skip (j_decompress_ptr cinfo, long num_bytes)
{
        cinfo->src->bytes_in_buffer -= num_bytes;
        cinfo->src->next_input_byte += num_bytes;
}

void jpeg_stdio_srcMem(j_decompress_ptr cinfo, char *buf, int len)
{
        cinfo->src = (struct jpeg_source_mgr *)
        (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT,
                sizeof(struct jpeg_source_mgr));

        cinfo->src->init_source = noop;
        cinfo->src->fill_input_buffer = fill;
        cinfo->src->skip_input_data = skip;
        cinfo->src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
        cinfo->src->term_source = noop;
        cinfo->src->bytes_in_buffer = len; /* forces fill_input_buffer on first read */
	cinfo->src->next_input_byte = (const JOCTET *)buf; /* until buffer loaded */
}


//----------------------------------------------------------------------------------------------------------------------------------------
//									TextureListe
//----------------------------------------------------------------------------------------------------------------------------------------
TextureListe::TextureListe()
{
	tex = NULL;
	suiv = NULL;
}
//----------------------------------------------------------------------------------------------------------------------------------------
TextureListe::~TextureListe()
{
	nom.Empty()				;
	nomMask.Empty()			;
	if( tex ) delete tex	;
}
//----------------------------------------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										+-----------------------------+
//										|    Class TextureManager	  |
//										+-----------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
//TextureManager::TextureManager()
void TextureManager::Initialise()
{
	Liste	    = NULL		;
	IteratorPos = NULL		;
	ForceFlag	= 0			;
}
//----------------------------------------------------------------------------------------------------------------------------------------
//TextureManager::~TextureManager()
void TextureManager::Release()
{
	TextureListe *tmp;

	while( Liste )
	{
		tmp = Liste->suiv	;
		delete Liste		;
		Liste = tmp			;
	}

	IteratorPos = NULL		;
}
//----------------------------------------------------------------------------------------------------------
void TextureManager::DeleteListe( TextureListe *actuel, TextureListe *preced )
{
	if( preced )
	{
		preced->suiv = actuel->suiv	;
		delete actuel				;
	}
	else
	{
		Liste = actuel->suiv	;
		delete actuel			;
	}
}
//----------------------------------------------------------------------------------------------------------
TextureListe *TextureManager::Getl( LPCSTR nom )
{
	TextureListe *tmp = Liste	;

	while( tmp )
	{
		if( tmp->nom==nom )
			return tmp	;
		tmp = tmp->suiv	;
	}
	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
TextureListe *TextureManager::Getl( TextureU3D *tex )
{
	TextureListe *tmp=Liste	;

	while( tmp )
	{
		if( tmp->tex == tex )
			return tmp	;	
		tmp = tmp->suiv	;
	}
	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
void TextureManager::Add( TextureU3D *tex, LPCSTR nom  )
{
	TextureListe *tmp	;

	tmp = new TextureListe	;
	tmp->nom = nom			;
	tmp->tex = tex			;
	tmp->suiv = Liste		;
	Liste = tmp				;
}
//----------------------------------------------------------------------------------------------------------
void TextureManager::AddUse( TextureU3D *tex )
{
	tex->IncUser()	;
}
//----------------------------------------------------------------------------------------------------------
void TextureManager::AddUse( LPCSTR nom )
{
	TextureU3D *tmp	;

	tmp = Get( nom )	;
	if( tmp )
		tmp->IncUser()	;
}
//----------------------------------------------------------------------------------------------------------
void TextureManager::SubUse( TextureU3D *tex )
{
	TextureListe *tmp=Liste,*preced=NULL,*suiv	;

	while( tmp )
	{
		suiv = tmp->suiv	;
		if( tmp->tex == tex )
			if( tex->Kill() )
				DeleteListe( tmp, preced )	;
			else return	;
		preced = tmp	;
		tmp = suiv		;
	}
}
//----------------------------------------------------------------------------------------------------------
void TextureManager::SubUse( LPCSTR nom )
{
	TextureListe *tmp=Liste,*preced=NULL	;

	while( tmp )
	{
		if( tmp->nom==nom )
			if( tmp->tex->Kill() )
				DeleteListe( tmp, preced )	;
			else return	;
		preced = tmp	;
		tmp = tmp->suiv	;
	}
}
//----------------------------------------------------------------------------------------------------------
void TextureManager::KILL( TextureU3D *tex )
{
	TextureListe *tmp=Liste,*preced=NULL	;

	while( tmp )
	{
		if( tmp->tex == tex )
		{
			DeleteListe( tmp, preced )	;
			return;
		}
		preced = tmp	;
		tmp = tmp->suiv	;
	}	
}
//----------------------------------------------------------------------------------------------------------
void TextureManager::KILL( LPCSTR nom )
{
	TextureListe *tmp=Liste,*preced=NULL	;

	while( tmp )
	{
		if( tmp->nom == nom )
		{
			DeleteListe( tmp, preced )	;
			return;
		}
		preced = tmp	;
		tmp = tmp->suiv	;
	}
}
//----------------------------------------------------------------------------------------------------------
TextureU3D* TextureManager::Get( LPCSTR nom )
{
	TextureListe *tmp=Liste	;

	while( tmp )
	{
		if( tmp->nom == nom )
			return tmp->tex	;
		tmp = tmp->suiv	;
	}
	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
LPCSTR	TextureManager::GetNom( TextureU3D *tex )
{
	TextureListe *tmp=Liste;

	while( tmp )
	{
		if( tmp->tex == tex )
			return (LPCSTR)tmp->nom	;
		tmp = tmp->suiv	;
	}
	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
LPCSTR	TextureManager::GetNomMask( TextureU3D *tex )
{
	return (LPCSTR)Getl( tex )->nomMask	;
}
//----------------------------------------------------------------------------------------------------------
TextureU3D* TextureManager::GetForUse( LPCSTR nom )
{
	TextureListe *tmp=Liste,*preced	;

	while( tmp )
	{
		if( tmp->nom == nom )
		{
			tmp->tex->IncUser()	;
			return tmp->tex	;
		}
		preced = tmp	;
		tmp = tmp->suiv	;
	}
	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE sur texture presente
bool TextureManager::Exist( TextureU3D *tex  )
{
	TextureListe *tmp=Liste	;

	while( tmp )
	{
		if( tmp->tex == tex )
			return TRUE	;
		tmp = tmp->suiv	;
	}
	return FALSE	;
}
//----------------------------------------------------------------------------------------------------------
// Note : pour TexFlag se référé a la class TextureU3D
TextureU3D* TextureManager::OpenRaw( CArchive &ar, LPCSTR nom, U32 x, U32 y, U32 flag )
{
	U32	a	;

	flag |= ForceFlag	;	// force certains flag settés

	if( ar.IsStoring() )	return	NULL;

	TextureU3D *ret = new TextureU3D( flag )	;
	ret->Create( x, y )							;

	if( ret->IsLowTexture() )
	{
		U8 *buf = new U8[x*y*3]				;
		U16 *dest = (U16*)ret->GetTexture()	;

		ar.Read( buf, x*y*3 )	;

		for( a=0; a<x*y; a++,dest++,buf+=3 )	*dest = BitConv24To16( buf )	;

		delete [] buf	;
	}
	else
	{
		if( ret->IsAlphaTexture() )
		{
			U8 *buf = new U8[x*y*3]				;
			U32 *dest = (U32*)ret->GetTexture()	;

			ar.Read( buf, x*y*3 )	;

			for( a=0; a<x*y; a++,dest++,buf+=3 )	*dest = BitConv24To32( buf )	;

			delete [] buf	;
		}
		else
		{
			ar.Read( ret->GetTexture(), x*y*3 )	;
		}
	}

	Add( ret, nom )	;

	return ret	;
}
//----------------------------------------------------------------------------------------------------------
// Fonction de sortie si mauvais decompactage de Jpeg !
//----------------------------------------------------------------------------------------------------------
TextureU3D* TextureManager::OpenJpg( CArchive &ar, LPCSTR nom, U32 lenghtdata, U32 flag )
{
	JSAMPARRAY	buffer							;
	struct		jpeg_decompress_struct cinfo	;
	struct		jpeg_error_mgr jerr				;
	U8*			CompressStream					;

	flag |= ForceFlag	;

	CompressStream = new U8[ lenghtdata ]	;
	ar.Read( CompressStream, lenghtdata )	;

	cinfo.err = jpeg_std_error( &jerr )	;
	jpeg_create_decompress( &cinfo )		;

	jerr.error_exit = my_error_exit									;
	jpeg_stdio_srcMem(&cinfo, (S8*)CompressStream, lenghtdata )	;
	jpeg_read_header(&cinfo, TRUE)									;

	// Prend pas les truc en Black and Withe
	if (cinfo.out_color_space==JCS_GRAYSCALE) 
	{
		FatalError( " JE veux pas de JPeg en Noire et Blanc maman " )	;
		jpeg_destroy_decompress(&cinfo)									;
	}

	jpeg_start_decompress(&cinfo)	;

	U32 blah = cinfo.output_width*cinfo.output_components			;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo,
					JPOOL_IMAGE, blah, 1)							;


	
	//------------------ Alloue texture
	TextureU3D *ret = new TextureU3D( flag )				;
	ret->Create( cinfo.image_width, cinfo.image_height )	;


	if( ret->IsAlphaTexture() || ret->IsLowTexture() )
	{
		for (U32 a=0; a < cinfo.output_height; a++)
		{
			jpeg_read_scanlines(&cinfo, buffer, 1)	;
			U8 *src = (U8*)buffer[0]				;
			U32 *ptr = (U32*)ret->GetTexture()		;
			ptr +=  a*cinfo.image_width			;

			for (U32 j=0; j < cinfo.output_width; j++,ptr++,src+=3)
				*ptr = BitConv24To32( src )	;
		}
	}
	else
	{
		for (U32 a=0; a < cinfo.output_height; a++)
		{
			jpeg_read_scanlines(&cinfo, buffer, 1)	;
			U8 *src = (U8*)buffer[0]				;
			U8 *ptr = (U8*)ret->GetTexture()		;
			ptr +=  a*cinfo.image_width*3			;

			memcpy( ptr, src, cinfo.image_width*3 )	;
		}
	}

	jpeg_finish_decompress( &cinfo )	;
	jpeg_destroy_decompress( &cinfo )	;

	delete [] CompressStream	;

	Add( ret, nom )	;

	return ret	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// ouvre un .raw et renvoie l'adresse de l'image
U8*	 TextureManager::NewRaw( CArchive &ar, U32 x, U32 y, U32 nbOctet )
{
	if( ar.IsStoring() )	return	NULL;

	U8 *buf = new U8[x*y*nbOctet]		;

	ar.Read( buf, x*y*nbOctet )	;

	return buf	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// ouvre un .jpg et renvoie l'adresse de en 32 bit et la taille x et y de l'image
U32* TextureManager::NewJpg( CArchive &ar, U32 &x, U32 &y, U32 lenghtdata  )
{
	JSAMPARRAY	buffer							;
	struct		jpeg_decompress_struct cinfo	;
	struct		jpeg_error_mgr jerr				;
	U8*			CompressStream					;
	U32*		retBuf							;

	CompressStream = new U8[ lenghtdata ]	;
	ar.Read( CompressStream, lenghtdata )	;

	cinfo.err = jpeg_std_error( &jerr )	;
	jpeg_create_decompress( &cinfo )	;

	jerr.error_exit = my_error_exit									;
	jpeg_stdio_srcMem(&cinfo, (S8*)CompressStream, lenghtdata )	;
	jpeg_read_header(&cinfo, TRUE)									;

	// Prend pas les truc en Black and Withe
	if (cinfo.out_color_space==JCS_GRAYSCALE) 
	{
		FatalError( " JE veux pas de JPeg en Noire et Blanc maman " )	;
		jpeg_destroy_decompress(&cinfo)									;
	}

	jpeg_start_decompress(&cinfo)	;

	U32 blah = cinfo.output_width*cinfo.output_components			;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo,
					JPOOL_IMAGE, blah, 1)							;


	//------------------ Alloue texture
	x = cinfo.output_width										;
	y = cinfo.output_height										;
	retBuf = new U32[cinfo.output_height*cinfo.output_width ]	;

	for (U32 a=0; a < cinfo.output_height; a++)
	{
		jpeg_read_scanlines(&cinfo, buffer, 1)	;
		U8 *src = (U8*)buffer[0]				;
		U32 *ptr = retBuf						;
		ptr +=  a*cinfo.image_width				;

		for (U32 j=0; j < cinfo.output_width; j++,ptr++,src+=3)
			*ptr = BitConv24To32( src )	;
	}

	jpeg_finish_decompress( &cinfo )	;
	jpeg_destroy_decompress( &cinfo )	;

	delete [] CompressStream	;

	return retBuf	;	
}
//----------------------------------------------------------------------------------------------------------------------------------------
bool TextureManager::OpenMask( TextureU3D* tex, CArchive &ar )
{
	U32	a,taille	;
	U8*	buf			;
	U8*	dest		;

	if( ar.IsStoring() )	return	FALSE;

	taille = tex->Width() * tex->Height()	;

	buf = new U8[ taille ]	;	// alloue buffer temporaire

	dest = (U8*)tex->GetTexture()	;

	ar.Read( buf, taille )	; // lit les données poil de nez

	for( a=0; a<taille; a++,dest+=4 )	
		dest[3] = buf[a]	;	// op store la composate alpha

	delete buf ;	// on a plus besoin de ce vilain buffer

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// Note : pour TexFlag se référé a la class TextureU3D
TextureU3D* TextureManager::OpenRaw( LPCSTR nom,U32 x, U32 y, U32 flag )
{
	FILE            *file		;
	TextureU3D	*ret = NULL	;
	char buf[256];

	sprintf(buf, "%s%s", Repertoir, nom);
	file = fopen(buf, "r");

	if( file ) {
		try {
			CArchive ar(file)	;

			ret = OpenRaw( ar, buf, x, y, flag )	;
	
			ar.Close()		;
			fclose(file);
		} catch (...) { 
			perror(buf);
			exit(-1);
		}
	} else {
		perror(buf);
	}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// ouvre un .raw et renvoie l'adresse de l'image
U8*	TextureManager::NewRaw( LPCSTR nom, U32 x, U32 y, U32 nbOctet )
{
	FILE            *file		;
	U8        	*ret = NULL	;
	char buf[256];
	
	sprintf(buf, "%s%s", Repertoir, nom);
	file = fopen(buf, "r");

	if( file ) {
		try {
			CArchive ar(file)	;

			ret = NewRaw( ar, x, y, nbOctet )       ;
	
			ar.Close()		;
			fclose(file);
		} catch (...) { 
			perror(buf);
			exit(-1);
		}
	} else {
		perror(buf);
	}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
TextureU3D* TextureManager::OpenJpg( LPCSTR nom, U32 flag )
{
	FILE            *file		;
	TextureU3D	*ret = NULL	;
	char buf[256];
	
	sprintf(buf, "%s%s", Repertoir, nom);
	file = fopen(buf, "r");
	
	if( file ) {
		try {
			fseek(file, 0, SEEK_END);
			int len = ftell(file);
			fseek(file, 0, SEEK_SET);

			CArchive ar(file)	;
			ret = OpenJpg( ar, nom, len, flag );
	
			ar.Close()		;
			fclose(file);
		} catch (...) { 
			perror(buf);
			exit(-1);
		}
	} else {
		perror(buf);
	}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// ouvre un .jpg et renvoie l'adresse de en 32 bit et la taille x et y de l'image
U32* TextureManager::NewJpg( LPCSTR nom, U32 &x, U32 &y  )
{
	FILE            *file		;
	U32    	        *ret = NULL	;
	char buf[256];
	
	sprintf(buf, "%s%s", Repertoir, nom);
	file = fopen(buf, "r");

	if( file ) {
		try {
			CArchive ar(file)	;
			fseek(file, 0, SEEK_END);
			int len = ftell(file);
			fseek(file, 0, SEEK_SET);

			ret = NewJpg( ar, x, y, len);
	
			ar.Close()		;
			fclose(file);
		} catch (...) { 
			perror(buf);
			exit(-1);
		}
	} else {
		perror(buf);
	}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// ouvre un mask pour cette texture
bool TextureManager::OpenMask( TextureU3D* tex, LPCSTR nomMask )
{
	FILE			*file		;
	bool			ret = FALSE	;
	TextureListe	*TexL		;
	char buf[256];
	
	sprintf(buf, "%s%s", Repertoir, nomMask);

	if( !tex->IsAlphaTexture() )	return FALSE	;

	TexL = Getl( tex )			;
	if( !TexL )	return FALSE	;

	TexL->nomMask = nomMask	;	// sauve le nom du mask

	file = fopen(buf, "r");
	if( file )
	{
		CArchive ar(file)	;

#if 0
			if( file.GetLength() != tex->Width()*tex->Height() )
			{
				ar.Close()		;
				file.Close()	;
				return FALSE	;
			}
#endif

			ret = OpenMask( tex, ar )	;

			ar.Close()		;
			fclose(file);
		}
	else
		{
			perror(buf);
			exit(-1);
		}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// Load toutes les textures dans la carte 3D
void TextureManager::LoadAll()
{
	TextureListe *tmp = Liste;

	while( tmp )
	{
		tmp->tex->Load();
		tmp = tmp->suiv;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------
// detruit toute les texture du manager
void TextureManager::DeleteAll()
{
	TextureListe *tmp;

	while( Liste )
	{
		tmp = Liste->suiv	;
		delete Liste		;
		Liste = tmp			;
	}

	Liste = NULL		;
	IteratorPos = NULL	;
}
//----------------------------------------------------------------------------------------------------------------------------------------
//
#if 0
// sauve info sous format texte !
void TextureManager::SerializeTxt( CArchive &ar)
{
	CString			tmpCS			;
	TextureListe*	tmpTex = Liste	;


	ar.WriteString("//--------------------------------------\15\n" )	;
	ar.WriteString("//			Texture List\15\n" )					;
	ar.WriteString("//--------------------------------------\15\n" )	;
	ar.WriteString("\15\n" )											;


	tmpCS.Format( "Repertoir %s\15\n", (LPCSTR)Repertoir	)	;
	ar.WriteString( (LPCSTR)tmpCS )								;
	ar.WriteString("\15\n" )									;


	while( tmpTex )
	{
		if( tmpTex->tex->IsTileTexture() )
			ar.WriteString( "TileTex\15\n" )	;

		if( tmpTex->tex->IsAlphaTexture() )
		{
			tmpCS.Format( "MaskOpen %s\15\n", (LPCSTR)tmpTex->nomMask )	;
			ar.WriteString( (LPCSTR)tmpCS )									;
		}

		tmpCS.Format( "Open %s\15\n", (LPCSTR)tmpTex->nom )	;
		ar.WriteString( (LPCSTR)tmpCS )							;

		tmpTex = tmpTex->suiv		;
		ar.WriteString("\15\n" )	;
	}

	ar.WriteString("\15\n" )	;
}
#endif
//----------------------------------------------------------------------------------------------------------------------------------------
