    					/*------------------------------------------------------+
						  |				     DataFile.cpp						|
						  |														|
						  |	 class ki permet de récupérer facilement se kil y   |
						  |		dans un GTZ ( une surcouche koa en fait )	    |
						  |														|
						  | 	U2^PoPsy TeAm 2000								|
						  +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-----------------------+
//									|	 Class DataFile		|
//									+-----------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
DataManager::DataManager()
{
}
//----------------------------------------------------------------------------------------------------------
DataManager::~DataManager()
{
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si succes sinon FALSE ( pf on l'aurai pas deviné )
void DataManager::LoadGTZ( CArchive &ar )
{
}
//----------------------------------------------------------------------------------------------------------
bool DataManager::LoadGTZ( LPCSTR nom )
{
#if 0
	CFile file	;

	if( file.Open( nom, CFile::modeRead ) )
	TRY
	{
		CArchive ar(&file, CArchive::load )	;

		/*pack = new GTZpack(NULL)	;
		pack->Serialize( ar )		;*/
		LoadGTZ( ar )	;

		ar.Close()		;
		file.Close()	;
	}
	CATCH_ALL(e)
	{
		return FALSE	;
	}
	END_CATCH_ALL
	else
	{
		return FALSE	;
	}

	return TRUE	;	
#endif
	return FALSE;
}
//----------------------------------------------------------------------------------------------------------
// voir fonctions danbs pack.h (de la  GTZLib )
std::vector<char>* DataManager::GetData( LPCSTR nom,bool destruc )
{
#if 0
	if( ! pack )	return NULL	;

	CByteArray* ret = pack->GetData( nom, destruc )	;

#ifdef _DEBUG
	if( !ret )
		DebugErrorMess( "<U3D> GTZ file missing : %s\n", nom )	;
#endif

	return ret	;
#endif
}
//----------------------------------------------------------------------------------------------------------
// voir fonctions danbs pack.h (de la  GTZLib )
std::vector<char>* DataManager::GetDataFolder( LPCSTR nom,bool destruc )
{
#if 0
	if( !pack )	return NULL	;

	CByteArray* ret = pack->GetDataFolder( nom, destruc )	;

#ifdef _DEBUG
	if( !ret )
		DebugErrorMess( "<U3D> GTZ file missing : %s\n", nom )	;
#endif

	return ret	;
#endif
}
//----------------------------------------------------------------------------------------------------------
