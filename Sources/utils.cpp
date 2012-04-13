    				 /*-----------------------------------------------------+
					  |				        Utils.cpp						|
					  |														|
					  |		des pitites fonctions utiles dans divers		|
					  |						cas de figures					|
					  |														|
					  | 	U2^PoPsy TeAm 2000								|
					  +-----------------------------------------------------*/


#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
void GrabScreen( U32 x, U32 y, U32 num )
{
#if 0
	U32*	buff		;
	U8*		bufffina	;
	CFile	file		;
	CString nomOpen		;

	buff = new U32[ x*y ]		;
	bufffina = new U8[ x*y*3 ]	;

	glReadPixels( 0, 0, x, y, GL_RGBA, GL_UNSIGNED_BYTE, buff )	;

	U8* src = (U8*)&buff[x*(y-1)]	;
	U8* dest = (U8*)bufffina	;
	for( U32 a=0; a<y; a++,src-=4*x )
	{
		U8* src1 = src	;
		for( U32 b=0; b<x; b++,dest+=3,src1+=4)
		{
			dest[0] = src1[0]	;
			dest[1] = src1[1]	;
			dest[2] = src1[2]	;
		}
	}

	nomOpen.Format( "image%d.raw", num )			;

	if( file.Open( nomOpen, CFile::modeCreate | CFile::modeWrite ) )
	{
		CArchive ar(&file, CArchive::store )	;

		ar.Write( bufffina, x*y*3 )	;

		ar.Close()		;
		file.Close()	;
	}

	delete [] buff		;
	delete [] bufffina	;
#endif
}
//----------------------------------------------------------------------------------------------------------------------------------------
