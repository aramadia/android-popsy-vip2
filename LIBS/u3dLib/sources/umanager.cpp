    				 /*-----------------------------------------------------+
					  |				         Umanager.cpp					|
					  |														|
					  |	    c'est un manager qui va géré l'allocation des   |
					  |				polygones, des vertex, etc...			|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/


#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
//							+------------------------------+
//							|    Les Variables globales    |
//							+------------------------------+
//----------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
	U32 DBnbPolyAlloue=0;
	U32 DBnbVertexAlloue=0;
	U32 DBnbCoordAlloue=0;
	U32 DBnbPipeAlloue=0;
#endif

//----------------------------------------------------------------------------------------------------------
//								+---------------------+
//								|    Les Fonctions    |
//								+---------------------+
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
UManager::UManager()
{
	Pipeliste = NULL	;
	Cliste = NULL		;
	Vliste = NULL		;
	for(U32 a=0; a<MAX_VERTEX_PAR_POLY; a++)
		Pliste[a] = NULL;
}
//----------------------------------------------------------------------------------------------------------
UManager::~UManager()
{
	CoordU3D*		ctmp	;
	VertexU3D*		vtmp	;
	PolygonU3D*		ptmp	;
	PipelineState*	pstmp	;

	// detruit la liste des etats de pipeline
	while( Pipeliste )
	{
		pstmp = Pipeliste->suiv	;
		delete Pipeliste		;
		Pipeliste = pstmp		;
	#ifdef _DEBUG
		DBnbPipeAlloue--;
	#endif
	}

	// détruit la liste de coordonnées
	while( Cliste )
	{
		ctmp = Cliste;
		Cliste = Cliste->suiv;
		delete ctmp;
	#ifdef _DEBUG
		DBnbCoordAlloue--;
	#endif
	}

	// détruit la liste de vertex
	while( Vliste )
	{
		vtmp = Vliste;
		Vliste = Vliste->suiv;
		delete vtmp;
	#ifdef _DEBUG
		DBnbVertexAlloue--;
	#endif
	}

	// détruit les listes de polygones
	for(U32 a=0; a<MAX_VERTEX_PAR_POLY; a++)
		while( Pliste[a] )
		{
			ptmp = Pliste[a];
			Pliste[a] = Pliste[a]->suiv;
			delete ptmp;
		#ifdef _DEBUG
			DBnbPolyAlloue--;
		#endif
		}

}
//----------------------------------------------------------------------------------------------------------
PipelineState*	UManager::NewPipeState()
{
	PipelineState* ret	;

	if( !Pipeliste )
	{
		ret = new PipelineState	;
		#ifdef _DEBUG
			DBnbPipeAlloue++	;
		#endif
	}
	else
	{
		ret = Pipeliste				;
		Pipeliste = Pipeliste->suiv	;
	}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------
void UManager::FreePipeState(PipelineState *c)
{
	c->suiv = Pipeliste	;
	Pipeliste = c		;
}
//----------------------------------------------------------------------------------------------------------
