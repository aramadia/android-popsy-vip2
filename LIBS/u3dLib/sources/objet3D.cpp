    				 /*-----------------------------------------------------+
					  |				         objet3d.h					    |
					  |														|
					  |	  Que serait une monde 3D sans ses objets alors     |
					  |		vala leur définitions a tout ces petits			|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/


#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------
//								+---------------------+
//								|    Les Fonctions    |
//								+---------------------+
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
ObjetU3D::ObjetU3D()
{
	Reset()	;
}
//----------------------------------------------------------------------------------------------------------
ObjetU3D::ObjetU3D( LPCSTR n, U32 nbc, U32 nbv, U32 nbp )
{
	Reset()	;

	nom = n	;

	nbcoords = nbc				;
	Ctab = new CoordU3D[nbc]	;

	nbvertices = nbv			;
	Vtab = new VertexU3D[nbv]	;

	nbpolys = nbp				;
	Ptab = new PolygonU3D[nbp]	;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::Reset()
{
	nom = ""	;
	parent = ""	;

	typeShading = snone												;
	typeUVcalc	= cnone												;
	typeDraw	= solid												;
	Recalc		= bbox|normalsPoly|normalsVertex|shading|collision	;

	enfant = NULL		;
	suiv = NULL			;

	nbcoords	= 0		;
	Ctab		= NULL	;
	nbvertices	= 0		;
	Vtab		= NULL	;
	nbpolys		= 0		;
	Ptab		= NULL	;

	Wire = NULL				;
	ColTree = NULL			;
	StaticLighting = NULL	;

	Spos = NULL		;
	Srot = NULL		;

	Smorph = NULL	;
	nbMorphKey = 0	;
	TmpMorph = NULL	;	// va sauver temporairement les clé de morphing avant de les transformer en spline 

	Hides = NULL	;

	Bones = NULL	;
}
//----------------------------------------------------------------------------------------------------------
ObjetU3D::~ObjetU3D()
{
	ObjetU3D	*Otmp	;

	//--------------------- Detruits tout les enfants de l'objets
	while( enfant )
	{
		Otmp = enfant->suiv	;
		delete enfant		;
		enfant = Otmp		;
	}

	//-------------------- Detruits faces,coords et vertices
	if( Ptab ) delete [] Ptab	;
	if( Vtab ) delete [] Vtab	;
	if( Ctab ) delete [] Ctab	;

	//-------------------- Detruit structures spéciales
	if( Wire ) delete Wire						;
	if( ColTree ) delete ColTree				;
	if( StaticLighting ) delete StaticLighting	;

	//------------------- Detruits les anims
	if( Spos ) delete Spos			;
	if( Srot ) delete Srot			;
	if( Smorph ) delete Smorph		;
	if( TmpMorph ) delete TmpMorph	;
	if( Hides ) delete Hides		;
	if( Bones ) delete Bones		;

}
//----------------------------------------------------------------------------------------------------------
// load tableau de coordonée depuis l'archive
void ObjetU3D::LoadCoord( CArchive &ar )
{
	U16 tmp	;

	ar >> tmp						;
	nbcoords = tmp					;
	Ctab = new CoordU3D[ nbcoords ]	;
	
	for( U32 a=0; a<nbcoords; a++ )
	{
		ar >> Ctab[a].origine[0]	;
		ar >> Ctab[a].origine[1]	;
		ar >> Ctab[a].origine[2]	;

		vec4_set( Ctab[a].RVBA, 1.f, 1.f, 1.f, 1.f )	;
		vec4_set( Ctab[a].ecran, 0.f, 0.f, 0.f, 0.f )	;
		Ctab[a].utilisateurs = 0						;
		Ctab[a].frame_process = (U32)-1					;
	}
}
//----------------------------------------------------------------------------------------------------------
// load tableau de vertex depuis l'archive
void ObjetU3D::LoadVertices( CArchive &ar )
{
	U16 tmp		;
	U16 index	;

	ar >> tmp							;
	nbvertices = tmp					;
	Vtab = new VertexU3D[ nbvertices ]	;

	for( U32 a=0; a<nbvertices;	a++)
	{
		ar >> Vtab[a].U	;
		ar >> Vtab[a].V	;
		ar >> index		;

		Vtab[a].utilisateurs = 0		;
		Ctab[ index ].utilisateurs++	;
		Vtab[a].c = &Ctab[ index ]		;
		Vtab[a].U1 = Vtab[a].U			;
		Vtab[a].V1 = Vtab[a].V			;
	}
}
//----------------------------------------------------------------------------------------------------------
// load tableau de polygons depuis l'archive
void ObjetU3D::LoadPolys( CArchive &ar )
{
	U16 tmp16	;
	U8	tmp8	;

	ar >> tmp16							;
	nbpolys = tmp16						;
	Ptab = new PolygonU3D[ nbpolys ]	;

	for( U32 a=0; a<nbpolys; a++ )
	{
		Ptab[a].SetNbVertex( 3 )			;	//on a toujours des triangles

		//------------------- Load les index des 3 vertex et link les pointeurs

		ar >> tmp16							;
		Vtab[ tmp16 ].utilisateurs ++		;
		Ptab[a].Vtab[0] = &Vtab[ tmp16 ]	;

		ar >> tmp16							;
		Vtab[ tmp16 ].utilisateurs ++		;
		Ptab[a].Vtab[1] = &Vtab[ tmp16 ]	;

		ar >> tmp16							;
		Vtab[ tmp16 ].utilisateurs ++		;
		Ptab[a].Vtab[2] = &Vtab[ tmp16 ]	;

		//-------------------

		ar >> tmp8				;
		Ptab[a].arrete = tmp8	;

		ar >> tmp8						;
		Ptab[a].m = (MaterialU3D*)tmp8	;		// attention astuce de cow boy
	}
}
//----------------------------------------------------------------------------------------------------------
// load tableau de material depuis l'archive
void ObjetU3D::LoadMats( CArchive &ar )
{
	U8			nb,i	;
	CString		tmp		;
	MaterialU3D **mtab	;
	MaterialU3D *tmpM	;

	ar >> nb					;

	if( nb )
	{
		mtab = new MaterialU3D*[nb]	;

		for( U32 a=0; a<nb; a++ )
		{
			ar >> tmp												;
			mtab[a] = U3D3Monde3D->GetOrCreateMat( (LPCSTR)tmp )	;
		}

		//----------------------- assigne GOOD pointeur de material sur chaque poly
		for( U32 a=0; a<nbpolys; a++ )
		{
			i = (U8)Ptab[a].m	;

			if( (i==255) || (i>=nb) )	Ptab[a].m = U3D3Monde3D->GetOrCreateMat( "Default" )	;
			else						Ptab[a].m = mtab[ i ]									;
		}

		delete [] mtab;
	}
	else	// cas ou il ny a pas de material ->> donc on en met 1 par defaut
	{
		tmpM = U3D3Monde3D->GetOrCreateMat( "Default" )	;
		for( U32 a=0; a<nbpolys; a++ ) Ptab[a].m = tmpM	;
	}
}
//----------------------------------------------------------------------------------------------------------
bool ObjetU3D::Serialize( CArchive &ar )
{
	U32				a			;
	U16				Chunk		;
	U16				npkeys		;
	PosKeyFileU3D	*posK=NULL	;
	U16				nrkeys		;
	RotKeyFileU3D	*rotK=NULL	;
	U16				nhkeys		;
	HideKeyFileU3D	*hideK=NULL	;

	if( !ar.IsStoring() )
	{

		ar >> nom		;
		ar >> parent	;	// nom de l'objet parent


		LocalMatrix.Serialize( ar )	;


		LoadCoord( ar )		;
		LoadVertices( ar )	;
		LoadPolys( ar )		;
		LoadMats( ar )		;


		//-------------- Charge les pos keys
		ar >> npkeys										;
		if( npkeys ) posK = new PosKeyFileU3D[ npkeys ]		;
		for( a=0; a<npkeys;	a++	)	posK[a].Serialize( ar )	;
		if( npkeys > 1)
		{
			Spos = new SplineU3D( posK, npkeys )	;
		}
		else
		{
			Spos = NULL														;
			LocalMatrix.SetPos( posK[0].p[0], posK[0].p[1], posK[0].p[2] )	;	// prend kan meme position car peut etre != de celle de la matrice
		}
		if( posK ) delete [] posK	;


		//------------ Charge les rot keys
		ar >> nrkeys										;
		if( nrkeys ) rotK = new RotKeyFileU3D[ nrkeys ]		;
		for( a=0; a<nrkeys; a++ ) rotK[a].Serialize( ar )	;
		if( nrkeys > 1 )
		{
			Srot = new SplineRotU3D( rotK, nrkeys )	;
		}
		else
		{
			Srot = NULL																;
			Quat	qtmp( rotK[0].q[0], rotK[0].q[1], rotK[0].q[2], rotK[0].q[3] )	;
			qtmp.matrix( LocalMatrix )												;
		}
		if( rotK ) delete [] rotK	;


		//------------ Charge les morphs keys
		ar >> nbMorphKey	;
		if( nbMorphKey )
		{
			TmpMorph = new MorphKeyFileU3D[ nbMorphKey ]				;
			for( a=0; a<nbMorphKey; a++ )	TmpMorph[a].Serialize( ar )	;
		}


		//------------ Charge les hides keys
		ar >> nhkeys												;
		if( nhkeys )	hideK = new HideKeyFileU3D[ nhkeys ]		;
		for( a=0; a<nhkeys; a++ ) hideK[a].Serialize( ar )			;
		if( nhkeys > 0 )	Hides = new HideU3D( hideK, nhkeys )	;
		if( hideK ) delete [] hideK									;


		ar >> Chunk										;
		if( Chunk != CHUNK_OBJ_END )	return FALSE	;

	}

	Ini()		;

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------
// fait toute les ini nécessaires a l'objet ( doit etre appelé au - 1 fois apres le serialize! )
void ObjetU3D::Ini()
{
	CalcBBox()		;
	CalcNormales()	;

	//-------------------------- Ini les coords de l'objet
	WorldMatrix = LocalMatrix	;
	Transform( WorldMatrix )	;
}
//----------------------------------------------------------------------------------------------------------
// des ini ki dépendent d'autre objets dans la base -> donc fo attendre ke tout soit loadé !
void ObjetU3D::IniAfterAllLoaded()
{

	//-------------------------- Traite le cas du morphing
	if( nbMorphKey > 1 )
	{
		Smorph = new SplineMorphU3D( TmpMorph, nbMorphKey )	;
	}
	if( TmpMorph )
	{
		delete [] TmpMorph	;
		TmpMorph = NULL		;
	}

}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::ConstructWireDatas()
{
	if( Wire )	delete Wire	;

	Wire = new Objet3dWire	;
	Wire->Construct( this )	;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::KillWireData()
{
	if( Wire )	delete Wire	;
	Wire = NULL				;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::ConstructColTree()
{
	if( ColTree ) delete ColTree	;

	ColTree = new BTree					;
	ColTree->Construct( Ptab, nbpolys )	;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::KillColTree()
{
	if( ColTree ) delete ColTree	;
	ColTree = NULL					;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::ConstructStaticLighting()
{
	StaticLighting = new ObjStaticLighting( this )	;
	StaticLighting->GetGouraud( Ctab, nbcoords )	;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::AddBones(  OneObjBones* bone )
{
	if( !Bones )	Bones = new ObjBones	;

	Bones->AddBone( bone )	;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::ConfirmBones()
{
	if( Bones ) Bones->Confirm( this )	;
}
//----------------------------------------------------------------------------------------------------------
bool ObjetU3D::Cmp(LPCSTR n)
{

	if( nom==n )	return TRUE	;
	return FALSE				;

}
//----------------------------------------------------------------------------------------------------------
// renvoie l'objet ki porte le nom n
ObjetU3D *ObjetU3D::GetObj( LPCSTR n )
{
	ObjetU3D	*Otmp = enfant	;
	ObjetU3D	*Otmp1			;

	if( nom==n )	return this	;

	while( Otmp )
	{
		Otmp1 = Otmp->GetObj( n )	;
		if( Otmp1 ) return Otmp1	;
		Otmp = Otmp->suiv			;
	}

	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie nombre d'arretes de l'objet
U32	ObjetU3D::GetNbArretes()
{
	U32			count = 0	;
	PolygonU3D*	p = Ptab	;

	for( U32 a=0; a<nbpolys; a++,p++ )
	{
		for( U32 b=0; b<p->nbVertex; b++ )
			if( (p->arrete>>b)&0x1 )
				count++	;
	}

	return count	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si objet visible
bool ObjetU3D::IsVisible()
{
	if( Hides ) if( Hides->IsHide() )	return FALSE	;

//	U32 flagc = U3D3Monde3D->GetActualCamera()->ClipInfo( &Bworld, WorldMatrix )	;
	U32 flagc = U3D3Monde3D->GetActualCamera()->ClipInfo( &Blocal, WorldMatrix )	;

	if( flagc==ClipCache )
	{
		ObjetU3D	*Otmp = enfant	;
		while( Otmp )
		{
			if( Otmp->IsVisible() ) return TRUE	;
			Otmp = Otmp->suiv					;
		}

		return FALSE	;
	}
	else
	{
		return TRUE	;
	}
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si objet animé
bool ObjetU3D::IsAnimated()
{
	ObjetU3D	*Otmp = enfant	;

	if( Spos )		return TRUE	;
	if( Srot )		return TRUE	;
	if( Hides )		return TRUE	;
	if( Smorph )	return TRUE	;
	if( Bones )		return TRUE	;

	while( Otmp )
	{
		if( Otmp->IsAnimated() ) return TRUE	;
		else					 return FALSE	;

		Otmp = Otmp->suiv	;
	}

	return FALSE	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si objet ne possede pas de parent
bool ObjetU3D::IsBaseObject()
{
	if( parent == "" )	return TRUE		;
	else				return FALSE	;
}
//----------------------------------------------------------------------------------------------------------
// calcul les normales au vertex
void ObjetU3D::CalcCoordNormales( bool realtime )
{
	U32 a,b;

	if( !(Recalc&normalsVertex) )	return	;	// on ne recalcul pas les normales si flag non activé

	for( a=0; a<nbcoords; a++)
		vec3_set( Ctab[a].normale, 0.f, 0.f, 0.f )	;

	for( a=0; a<nbpolys; a++)
	{
		for( b=0; b<Ptab[a].nbVertex; b++)
			vec3_add( Ptab[a].Vtab[b]->c->normale,
					  Ptab[a].Vtab[b]->c->normale,
					  Ptab[a].plan.normal )	;
	}

	if( realtime )
		for( a=0; a<nbcoords; a++)	vec3_FastNormalize( Ctab[a].normale )	;
	else
		for( a=0; a<nbcoords; a++)	vec3_normalize( Ctab[a].normale )		;
}
//----------------------------------------------------------------------------------------------------------
// calcul les normales aux polys
void ObjetU3D::CalcPolyNormales( bool realtime )
{
	if( !(Recalc&normalsPoly) )	return	;	// on ne recalcul si flag non activé

/*	if( realtime )
	{
		for( U32 a=0; a<nbpolys; a++)
			Ptab[a].CalcPlanRealTime()	;
	}
	else
	{*/
		for( U32 a=0; a<nbpolys; a++)
			Ptab[a].CalcPlan()	;
//	}
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::CalcNormales( bool realtime )
{
	CalcPolyNormales( realtime )	;
	CalcCoordNormales( realtime )	;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::CalcBBox()
{
	if( !(Recalc&bbox) )	return	;	// on ne recalcul si flag non activé

	for(U32 a=0; a<nbcoords; a++)
	{
		Blocal.TestEtActua( Ctab[a].origine )	;
		Box.PointIn( Ctab[a].origine )			;
	}
}
//----------------------------------------------------------------------------------------------------------
// transforme toute les coordonné par cette matrice
void ObjetU3D::Transform( Mat3x4 &mat )
{
	U32			a		;
	CoordU3D	*ctmp	;
	Ufloat		Vec[3]	;

	if( Bones )	// transformation par bones
	{
		Bones->Transform( this )	;
	}
	else	// transformation classik
	{
		ctmp = Ctab	;
		for( a=0; a<nbcoords; a++,ctmp++)
		{
			ctmp->trans[0] = mat.GetXTransfo( ctmp->origine )	;
			ctmp->trans[1] = mat.GetYTransfo( ctmp->origine )	;
			ctmp->trans[2] = mat.GetZTransfo( ctmp->origine )	;
		}
	}

	//-------------------- Update Bounding box
	for( a=0; a<8; a++ )
	{
		vec3_set( Vec, Blocal.pts[a][0], Blocal.pts[a][1], Blocal.pts[a][2] )	;
		Bworld.pts[a][0] = mat.GetXTransfo( Vec )								;
		Bworld.pts[a][1] = mat.GetYTransfo( Vec )								;
		Bworld.pts[a][2] = mat.GetZTransfo( Vec )								;
	}
}
//----------------------------------------------------------------------------------------------------------
// ajoute un fils a cet objet
void ObjetU3D::AddFils( ObjetU3D *obj )
{
	obj->suiv = enfant	;
	enfant = obj		;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::ApplyMat()
{
	U32			a				;
	ObjetU3D	*Otmp = enfant	;

	for( a=0; a<nbpolys; a++)
	{
		MaterialU3D *mat = Ptab[a].m;

		// s'occupe de flager le shading de l'objet
		switch( mat->GetShading() )
		{
			case MatShadingFlat:
				SetShadingFlat();
				break;
			case MatShadingGouraud:
				SetShadingGouraud();
				break;
		}
	}

	while( Otmp )
	{
		Otmp->ApplyMat()	;
		Otmp = Otmp->suiv	;
	}
}
//----------------------------------------------------------------------------------------------------------
// essaye d'enlever un max de face pour cette frame
void ObjetU3D::MakeCulling()
{
/*	ObjetU3D	*Otmp = enfant										;
	PolygonU3D	*tmp = Ptab											;
	Ufloat		*camp = U3D3Monde3D->GetActualCamera()->GetPos()	;
	Ufloat		Cpos[3]												;

	WorldMatrix.GetTranspose( Cpos, camp )	;

	for( U32 a=0; a<nbpolys; a++,tmp++)
	{
		if( tmp->BackFaceCull( Cpos ) && !tmp->m->GetSide() )
				tmp->flag |= PolyFLAG_HIDE	;
		else
			tmp->flag &= !PolyFLAG_HIDE	;
	}

	while( Otmp )
	{
		Otmp->MakeCulling()	;
		Otmp = Otmp->suiv	;
	}*/
	MakeCulling( U3D3Monde3D->GetActualCamera()->GetPos(), TRUE )	;
}
//----------------------------------------------------------------------------------------------------------
// essaye d'enlever un max de face pour cette frame ( ici en fonction d'un point quelconque )
void ObjetU3D::MakeCulling( Ufloat pos[3], bool test2side )
{
	ObjetU3D	*Otmp = enfant		;
	PolygonU3D	*tmp = Ptab			;
	Ufloat		Cpos[3]				;

	if( (Recalc&culling2d) ) return ;	// on calcul le back face culling en 2D alors on sort !

	WorldMatrix.GetTranspose( Cpos, pos )	;

	if( test2side )
	{
		for( U32 a=0; a<nbpolys; a++,tmp++)
		{
			if( tmp->BackFaceCull( Cpos ) && !tmp->m->GetSide() )
				tmp->flag |= PolyFLAG_HIDE	;
			else
				tmp->flag &= !PolyFLAG_HIDE	;
		}
	}
	else
	{
		for( U32 a=0; a<nbpolys; a++,tmp++)
		{
			if( tmp->BackFaceCull( Cpos ) )
				tmp->flag |= PolyFLAG_HIDE	;
			else
				tmp->flag &= !PolyFLAG_HIDE	;
		}
	}

	while( Otmp )
	{
		Otmp->MakeCulling( pos, test2side )	;
		Otmp = Otmp->suiv					;
	}
}
//----------------------------------------------------------------------------------------------------------
// pareil ke makeculling sauf k'ici on fait ca apres projecton des coords des points ( donc pas en 3D )
void ObjetU3D::Make2DCulling( bool test2side )
{
	ObjetU3D	*Otmp = enfant		;
	PolygonU3D	*tmp = Ptab			;

	if( test2side )
	{
		for( U32 a=0; a<nbpolys; a++,tmp++)
		{
			if( tmp->BackFaceCull2D() && !tmp->m->GetSide() )
				tmp->flag |= PolyFLAG_HIDE	;
			else
				tmp->flag &= !PolyFLAG_HIDE	;
		}
	}
	else
	{
		for( U32 a=0; a<nbpolys; a++,tmp++)
		{
			if( tmp->BackFaceCull2D() )
				tmp->flag |= PolyFLAG_HIDE	;
			else
				tmp->flag &= !PolyFLAG_HIDE	;
		}
	}

	while( Otmp )
	{
		Otmp->Make2DCulling()	;
		Otmp = Otmp->suiv		;
	}
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si le segment coupe un poly du monde 
bool ObjetU3D::SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3] )
{
	Ufloat		debutloca[3], finloca[3]	;
	PolygonU3D*	tmpp = Ptab					;

	if( !(Recalc&collision) ) return FALSE	;

	WorldMatrix.GetTranspose( debutloca, debut )	;
	WorldMatrix.GetTranspose( finloca, fin )		;

	if( ColTree )
	{
		return ColTree->SegmentIntersecPoly( debutloca, finloca )	;
	}
	else
	{
		if( Box.SegmentIntersec( debutloca, finloca ) )
		{
			for(U32 a=0; a<nbpolys; a++,tmpp++)
				if( tmpp->Intersection( debutloca, finloca ) )
					return TRUE;
		}
	}

	return FALSE;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si le segment coupe un poly du monde et renvoie l'intersection dans fin et le poly touché dans pres !
bool ObjetU3D::SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3], PolygonU3D* &pres )
{
	Ufloat		debutloca[3], finloca[3]	;
	PolygonU3D*	tmpp = Ptab					;
	bool		passtest = FALSE			;

	if( !(Recalc&collision) ) return FALSE	;

	WorldMatrix.GetTranspose( debutloca, debut )	;
	WorldMatrix.GetTranspose( finloca, fin )		;

	if( ColTree )
	{
		passtest = ColTree->SegmentIntersecPoly( debutloca, finloca, pres )	;
	}
	else
	{
		if( Box.SegmentIntersec( debutloca, finloca ) )
		{
			for(U32 a=0; a<nbpolys; a++,tmpp++)
				if( tmpp->IntersectionMod( debutloca, finloca ) )
				{
					pres = tmpp		;
					passtest = TRUE	;
				}
		}
	}

	if( passtest )	// on a trouvé qq chose
	{
		WorldMatrix.GetTransfo( fin, finloca )	;
		return TRUE								;
	}
	else
	{
		return FALSE	;
	}
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si la sphere coupe la bounding box de l'objet. ( attention pos dans espace objet )
bool ObjetU3D::SphereIntersecObj( Ufloat pos[3], Ufloat rayon )
{
	if( !(Recalc&collision) ) return FALSE	;

	return Box.SphereIntersec( pos, rayon )	;
}
//----------------------------------------------------------------------------------------------------------
// calcul la matrice local depuis l'animation de l'objet
void ObjetU3D::CalcMatrixFromAnim()
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
// ini animation de l'objet ! ( et des objets dépendants )
void ObjetU3D::IniAnim( Mat3x4 *mat )
{
	ObjetU3D *Otmp	;

	if( Spos )	Spos->Ini()			;
	if( Srot )	Srot->Ini()			;
	if( Hides ) Hides->Ini()		;
	if( Smorph )
	{
		Smorph->Ini()			;
		Smorph->GetVal( this )	;
	}

	CalcMatrixFromAnim()			;	// calcul new LocalMatrix en fait
	WorldMatrix = LocalMatrix		;
	if( mat ) WorldMatrix *= (*mat)	;

	if( Bones )	Bones->IniAnim( WorldMatrix )	;

	Otmp = enfant	;
	while( Otmp )
	{
		Otmp->IniAnim( &WorldMatrix )	;
		Otmp = Otmp->suiv				;
	}

	Transform( WorldMatrix )	;

	if( Smorph || Bones )
	{
		CalcBBox()				;	// recalcul bounding box
		CalcNormales( TRUE )	;	// recalcul normales
	}
}
//----------------------------------------------------------------------------------------------------------
// avance l'objet de v keyframe
void ObjetU3D::IncAnim( float v, Mat3x4 *mat )
{
	ObjetU3D *Otmp	;

	if( Spos ) Spos->IncPos( v )	;
	if( Srot ) Srot->IncPos( v )	;
	if( Hides ) Hides->IncPos( v )	;
	if( Smorph ) 
	{
		Smorph->IncPos( v )		;
		Smorph->GetVal( this )	;
	}

	CalcMatrixFromAnim()			;
	WorldMatrix = LocalMatrix		;
	if( mat ) WorldMatrix *= (*mat)	;

	if( Bones )	Bones->IncAnim( WorldMatrix, v )	;

	Otmp = enfant	;
	while( Otmp )
	{
		Otmp->IncAnim( v, &WorldMatrix );
		Otmp = Otmp->suiv				;
	}

	Transform( WorldMatrix )	;

	if( Smorph || Bones )
	{
		CalcBBox()				;	// recalcul bounding box
		CalcNormales( TRUE )	;	// recalcul normales
	}
}
//----------------------------------------------------------------------------------------------------------
// calcul le shading de l'objet
void ObjetU3D::CalcShading()
{
	U32		a			;
	Ufloat	ambient[3]	;

	if( !(Recalc&shading) )	return	;	// on ne recalcul pas les normales si flag non activé


	U3D3Monde3D->GetAmbientColor( ambient )	;

	//--------------------- Ini dabord les couleurs avec la couleur ambiente !
	if( IsFlatShading() )
	{
		for( a=0; a<nbpolys; a++ )
			vec3_eg( Ptab[a].RVBA, ambient )	;
	}
	if( IsGouraudShading() )
	{
		if( StaticLighting )
		{
			StaticLighting->GetGouraud( Ctab, nbcoords )	;

			//--------------------- Calcul lighting en fct des lights dynamiques
			for( U3D3Monde3D->IniLightIterator(); U3D3Monde3D->GetLightFromIterator();  U3D3Monde3D->IncLightIterator() )
				if( U3D3Monde3D->GetLightFromIterator()->IsAnimated() )
					U3D3Monde3D->GetLightFromIterator()->CalcLightingRealTime( this )	;

			return	;
		}
		else
		{
			for( a=0; a<nbcoords; a++ )
				vec3_eg( Ctab[a].RVBA, ambient )	;
		}
	}

	//--------------------- Calcul lighting en fct de toute les lights !
	for( U3D3Monde3D->IniLightIterator(); U3D3Monde3D->GetLightFromIterator();  U3D3Monde3D->IncLightIterator() )
		U3D3Monde3D->GetLightFromIterator()->CalcLightingRealTime( this )	;

}
//----------------------------------------------------------------------------------------------------------
// calcul efet spéciaux sur les UV ( comme env map )
void ObjetU3D::CalcUVAnimation()
{
	U32			a		;
	VertexU3D	*vert	;
	Ufloat		v1[3]	;
	Ufloat		v2[3]	;

	switch( typeUVcalc )
	{
		case envStage0:
			vert = Vtab																	;
			WorldMatrix.GetTransposeR( v1, U3D3Monde3D->GetActualCamera()->GetBaseX() )	;
			WorldMatrix.GetTransposeR( v2, U3D3Monde3D->GetActualCamera()->GetBaseY() )	;
			for( a=0; a<nbvertices; a++,vert++)
			{
				vert->U = .5f*(1.f + vec3_dot( v1, vert->c->normale ))	;
				vert->V = .5f*(1.f + vec3_dot( v2, vert->c->normale ))	;
			}
			break;

		case envStage1:
			vert = Vtab																	;
			WorldMatrix.GetTransposeR( v1, U3D3Monde3D->GetActualCamera()->GetBaseX() )	;
			WorldMatrix.GetTransposeR( v2, U3D3Monde3D->GetActualCamera()->GetBaseY() )	;
			for( a=0; a<nbvertices; a++,vert++)
			{
				vert->U1 = .5f*(1.f + vec3_dot( v1, vert->c->normale ))	;
				vert->V1 = .5f*(1.f + vec3_dot( v2, vert->c->normale ))	;
			}
			break;

		default :
			break;
	}
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3D::Affiche()
{
	ObjetU3D		*Otmp = enfant							;
	PolyPipeLine	*pipe = U3D3Pipeline					;

	Camera			*cam = U3D3Monde3D->GetActualCamera()	;
	CoordU3D		*ctmp									;
	U32				frameNum = U3D3Monde3D->GetTick()		;

	PolygonU3D *poly = Ptab	;

	ctmp = Ctab	;
	for( U32 a=0; a<nbcoords; a++,ctmp++ )
		cam->Projet( ctmp, frameNum )	;		// calcul coords 2D

	//---------- : cas particulier du cllipind de face 2D a mettre au propre impérativement !!!!
	if( Recalc&culling2d )	// affiche seulement des faces normales 
	{
		for( U32 a=0; a<nbpolys; a++,poly++)
		{
			CoordU3D *c1,*c2,*c3 ;
			c1 = poly->Vtab[0]->c	;
			c2 = poly->Vtab[1]->c	;
			c3 = poly->Vtab[2]->c	;

			if( (c1->ClipInfo&ClipProche) || (c2->ClipInfo&ClipProche) || (c3->ClipInfo&ClipProche) )
			{
				PolygonU3D* tmpp =	poly->ClipToPlan( cam->GetPlanProche() )	;	// prend poly clippé par camera
				if( !tmpp ) continue											;	// poly non visible alors ouste au suivant

				for( U32 i=0; i<tmpp->nbVertex; i++)
					cam->Projet( tmpp->Vtab[i], frameNum );	// calcul coords 2D si il le fo

				if( tmpp->BackFaceCull2D()	)
					U3D3Manager->FreePoly( tmpp )	;
				else
					pipe->AddPolyDynaTemporary( tmpp )	;
			}
			else
			{
				if( !poly->BackFaceCull2D() )
					pipe->AddPolyDyna( poly )	;
			}
		}

		return ;
	}

	
	if( typeDraw&solid )
	{
		if( !(typeDraw&line) )	//------------ n'affiche l'objet qu'en Solid
		{
			for( U32 a=0; a<nbpolys; a++,poly++)
				if( !poly->IsHide() )
					pipe->AddPolyDyna( poly )	;
		}
		else								//------------ affiche l'objet en Solid et en filaire
		{
			PolygonU3D	*result	;

			for( U32 a=0; a<nbpolys; a++,poly++)
				if( !poly->IsHide() )
				{
					result = pipe->AddPolyDyna( poly )	;
					if( result )
						if( result->arrete )	// y a-t-il des lignes à afficher
						{
							pipe->AddPolyLinesNoClip( result->GetCopyPoly() )	;
						}
				}
		}
	}
	else if( typeDraw&line )	//------------ n'affiche l'objet qu'en filaire
	{
		for( U32 a=0; a<nbpolys; a++,poly++)
			if( !poly->IsHide() )
				pipe->AddPolyLines( poly )	;
	}

	//----------------- Affiche les objets fils !
	while( Otmp )
	{
		Otmp->Affiche()		;
		Otmp = Otmp->suiv	;
	}
}
//----------------------------------------------------------------------------------------------------------
#ifdef _LMAPS
//----------------------------------------------------------------------------
// va créé les lightmaps pour cette objet
void ObjetU3D::CreateLightMaps()
{
	for(U32 a=0; a<nbpolys; a++)
	{
		Ptab[a].lmap = U3D3LightMapManager->GetNew( Ptab[a] );
		U3D3LightMapManager->CalculLightMap( Ptab[a] );
	}
}
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------
