    				 /*---------------------------------------------------------+
					  |				     Objet3dBones.cpp						|
					  |															|
					  |		une implémentation des bones dans l'engine.			|
					  |				avec ces Bones il pourra					|
					  | y avoir N matrices ki influenceront N vertex vala.		|
					  |															|
					  | 	U2^PoPsy TeAm 2000									|
					  +---------------------------------------------------------*/

#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//								+-----------------------+
//								|   Class OneObjBones	|
//								------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
OneObjBones::OneObjBones(  Ufloat RayonInfMin, Ufloat RayonInfMax, bool invInflu )
{
	suiv = NULL		;
	enfant = NULL	;

	RayonInfluenceMax = RayonInfMax	;
	RayonInfluenceMin = RayonInfMin	;
	InvInfluence = invInflu			;

	Spos = NULL	;
	Srot = NULL	;

	nbcoord	= 0			;
	actuacoord = 0		;
	AllCoords = NULL	;
}
//----------------------------------------------------------------------------------------------------------
OneObjBones::~OneObjBones()
{
	if( AllCoords ) delete [] AllCoords	;
	if( Spos )		delete [] Spos		;
	if( Srot )		delete [] Srot		;
}
//----------------------------------------------------------------------------------------------------------
// va calculer la matrice de transof relativement a l'objet
void OneObjBones::SendObjMatrix( const Mat3x4 &objMat )
{
	OneObjBones *tmp	;

	ObjMatrix = LocalMatrix	;
	ObjMatrix *= objMat		;

	tmp = enfant	;
	while( tmp )
	{
		tmp->SendObjMatrix( ObjMatrix )	;
		tmp = tmp->suiv					;
	}
}
//----------------------------------------------------------------------------------------------------------
void OneObjBones::CalcInfluence( BoneAfectCoord* coord )
{
	Ufloat			dist,inf	;
	Ufloat			pos[3]		;
	OneObjBones*	tmpB		;

	ObjMatrix.GetPos( pos )	;

	dist = vec3_dist( coord->coord->origine, pos )	;

	if( dist <= RayonInfluenceMin )	// on est dans la sphere d'influence du bone
	{
		if( dist <= RayonInfluenceMax )	// influence totale du bone
			coord->TotInfluence += inf = 1.0f	;
		else
			coord->TotInfluence += inf = (RayonInfluenceMin - dist) / ( RayonInfluenceMin - RayonInfluenceMax )	;

		coord->AddBone( this, inf )	;
		nbcoord	++					;
	}

	//----------- detremine l'influence des fils
	tmpB = enfant	;
	while( tmpB )
	{
		tmpB->CalcInfluence( coord )	;
		tmpB = tmpB->suiv				;
	}
}
//----------------------------------------------------------------------------------------------------------
// rajoute coord au tableau AllCoords a la position actuacoord
void OneObjBones::AddCoord( CoordU3D* coord, Ufloat influence )
{
	if( !actuacoord ) AllCoords = new BonesCoord[ nbcoord ]	;	// alloue le tableau la 1er fois

	AllCoords[ actuacoord ].coord = coord			;
	AllCoords[ actuacoord ].Influence = influence	;

	ObjMatrix.GetTranspose( AllCoords[ actuacoord ].origine, coord->origine )	;

	actuacoord++	;
}
//----------------------------------------------------------------------------------------------------------
void OneObjBones::CalcMatrixFromAnim()
{
	Ufloat pos[3]	;

	//------------------ Prend la rotation
	if( Srot )
	{
		Quat	qtmp	;

		Srot->GetVal( qtmp )		;
		qtmp.matrix( LocalMatrix )	;
	}

	//------------------ Prend la translation
	if( Spos )
	{
		Spos->GetVal( pos )								;
		LocalMatrix.SetPos( pos[0], pos[1], pos[2] )	;
	}
}
//----------------------------------------------------------------------------------------------------------
void OneObjBones::IniAnim( const Mat3x4 &Mat )
{
	OneObjBones*	tmpB	;

	if( Spos )	Spos->Ini()		;
	if( Srot )	Srot->Ini()		;

	CalcMatrixFromAnim()		;
	WorldMatrix = LocalMatrix	;
	WorldMatrix *= Mat			;

	tmpB = enfant	;
	while( tmpB )
	{
		tmpB->IniAnim( WorldMatrix )	;
		tmpB = tmpB->suiv				;
	}
}
//----------------------------------------------------------------------------------------------------------
void OneObjBones::IncAnim( const Mat3x4 &objMat, float v)
{
	OneObjBones*	tmpB	;

	if( Spos ) Spos->IncPos( v )	;
	if( Srot ) Srot->IncPos( v )	;

	CalcMatrixFromAnim()		;
	WorldMatrix = LocalMatrix	;
	WorldMatrix *= objMat		;

	tmpB = enfant	;
	while( tmpB )
	{
		tmpB->IncAnim( WorldMatrix, v )	;
		tmpB = tmpB->suiv				;
	}
}
//----------------------------------------------------------------------------------------------------------
// calcul new position des coord du bone
void OneObjBones::Transform()
{
	BonesCoord*	tmp = AllCoords	;
	Ufloat		vtmp[3]			;

	for( U32 a=0; a<nbcoord; a++,tmp++ )
	{
		WorldMatrix.GetTransfo( vtmp, tmp->origine )				;
		vec3_mul( vtmp, vtmp, tmp->Influence )						;
//		vec3_add( tmp->coord->trans, vtmp, tmp->coord->trans )		;
		vec3_add( tmp->coord->origine, vtmp, tmp->coord->origine )	;
	}
}
//----------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+-------------------+
//									|   Class ObjBones	|
//									+-------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
ObjBones::ObjBones()
{
	AllBones = NULL		;
	ListeCoord = NULL	;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
ObjBones::~ObjBones()
{
	OneObjBones*		tmp		;
	BonesCoordListe*	ctmp	;

	while( AllBones )
	{
		tmp = AllBones->suiv	;
		delete AllBones			;
		AllBones = tmp			;
	}

	while( ListeCoord )
	{
		ctmp = ListeCoord->suiv	;
		delete ListeCoord		;
		ListeCoord = ctmp		;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void ObjBones::AddBone( OneObjBones* bone )
{
	bone->suiv = AllBones	;
	AllBones = bone			;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// Calcul les influence et place les bones
void ObjBones::Confirm( ObjetU3D *obj )
{
	U32				a				;
	BoneAfectCoord*	tmpCalcCoord	;
	OneObjBones*	tmpB			;
	BonesList*		tmpLB			;
	Mat3x4			DefaultMat		;

	tmpCalcCoord = new BoneAfectCoord[ obj->nbcoords ]	;

	for( a=0; a<obj->nbcoords; a++ )
		tmpCalcCoord[a].coord = &obj->Ctab[a]	;

	//------------------- Ini matrice de transfo des bones 
	tmpB = AllBones	;
	while( tmpB ) 
	{
		tmpB->SendObjMatrix( DefaultMat )	;
		tmpB = tmpB->suiv					;
	}

	//------------------- va calculer le degré d'influence de tout les bones sur les coord
	for( a=0; a<obj->nbcoords; a++ )
	{
		tmpCalcCoord[a].TotInfluence = 1.0f	;	// ya au moins une influence de base

		tmpB = AllBones	;
		while( tmpB )
		{
			tmpB->CalcInfluence( &tmpCalcCoord[a] )	;
			tmpB = tmpB->suiv							;
		}
	}

	//------------------- va calculer comuniquer aux bones les pts kils influences
	for( a=0; a<obj->nbcoords; a++ )
	{
		tmpLB = tmpCalcCoord[a].Bones	;

		//---------- rajoute dans la liste des coods touchés par au moins 1 bone
		if( tmpLB )
		{
			BonesCoordListe *ctmp = new BonesCoordListe					;
			ctmp->coord = tmpCalcCoord[a].coord						;
			vec3_eg( ctmp->origine, tmpCalcCoord[a].coord->origine )	;
			ctmp->Influence = 1.0f / tmpCalcCoord[a].TotInfluence		;	// toujours une influence de 1.0f
			ctmp->suiv = ListeCoord										;
			ListeCoord = ctmp											;
		}

		while( tmpLB )
		{
			tmpLB->Bone->AddCoord( tmpCalcCoord[a].coord, tmpLB->influence / tmpCalcCoord[a].TotInfluence )	;
			tmpLB = tmpLB->suiv																				;
		}
	}


	delete [] tmpCalcCoord	; // desalloue le tableau car il est inutile maintenant
}
//----------------------------------------------------------------------------------------------------------
// ini animation des bones	( avec objMat = matrice de transfo objetToWorld )
void ObjBones::IniAnim( const Mat3x4 &objMat )
{
	OneObjBones*	tmpB	;

	tmpB = AllBones	;
	while( tmpB ) 
	{
		tmpB->IniAnim( objMat )	;
		tmpB = tmpB->suiv		;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// avance dans l'anim des bones de v keyframe ( avec objMat = matrice de transfo de objetToWorld )
void ObjBones::IncAnim( const Mat3x4 &objMat, Ufloat v )
{
	OneObjBones*	tmpB		;
	Mat3x4			Mdefault	;

	tmpB = AllBones	;
	while( tmpB )
	{
//		tmpB->IncAnim( objMat, v )		;
		tmpB->IncAnim( Mdefault, v )	;
		tmpB = tmpB->suiv				;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
// calcul la position dans le monde des coordonnées de l'objet
void ObjBones::Transform( ObjetU3D *obj )
{
	CoordU3D*			ctmp = obj->Ctab			;
	OneObjBones*		tmpB						;
	BonesCoordListe*	cbtmp						;
	Mat3x4				mat = obj->GetObjToWorld()	;

/*	for( U32 a=0; a<obj->nbcoords; a++,ctmp++ )
		vec3_set( ctmp->trans, 0.0f, 0.0f, 0.0f )	;
	for( U32 a=0; a<obj->nbcoords; a++,ctmp++ )
		vec3_set( ctmp->origine, 0.0f, 0.0f, 0.0f )	;*/

	//------------- ini toutes les coords touchées par au moins 1 bone
	cbtmp = ListeCoord	;
	while( cbtmp )
	{
		vec3_mul( cbtmp->coord->origine, cbtmp->origine, cbtmp->Influence )	;
		cbtmp = cbtmp->suiv													;
	}

	tmpB = AllBones	;
	while( tmpB ) 
	{
		tmpB->Transform()	;
		tmpB = tmpB->suiv	;
	}

	//------------- Transform ensuite toute les coordonnées ki on été modifiées
	cbtmp = ListeCoord	;
	while( cbtmp )
	{
		cbtmp->coord->trans[0] = mat.GetXTransfo( cbtmp->coord->origine )	;
		cbtmp->coord->trans[1] = mat.GetYTransfo( cbtmp->coord->origine )	;
		cbtmp->coord->trans[2] = mat.GetZTransfo( cbtmp->coord->origine )	;

		cbtmp = cbtmp->suiv	;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------
