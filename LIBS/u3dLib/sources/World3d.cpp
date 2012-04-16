    				 /*-----------------------------------------------------+
					  |				         World3D.cpp				    |
					  |														|
					  |	  Ici ce sont les définitions principales pour 	    |
					  |	  gérer un monde 3D grace a mon moteur BSP tout		|
					  |							neuf !!						|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/


#include "U3D3.h"



//----------------------------------------------------------------------------------------------------------
//									+---------------------+
//									|    Les Fonctions    |
//									+---------------------+
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
//									ListeCameraU3D
//----------------------------------------------------------------------------------------------------------
ListeCameraU3D::ListeCameraU3D(Camera *c,ListeCameraU3D *cl)
{
	cam = c;
	suiv = cl;
}
//----------------------------------------------------------------------------------------------------------
ListeCameraU3D::~ListeCameraU3D()
{
	delete cam;
}
//----------------------------------------------------------------------------------------------------------
//									ListeLightU3D
//----------------------------------------------------------------------------------------------------------
ListeLightU3D::ListeLightU3D(LightU3D *l,ListeLightU3D *ll)
{
	light = l;
	suiv = ll;
}
//----------------------------------------------------------------------------------------------------------
ListeLightU3D::~ListeLightU3D()
{
	delete light;
}
//----------------------------------------------------------------------------------------------------------
//									ListeObjetU3D
//----------------------------------------------------------------------------------------------------------
ListeObjetU3D::ListeObjetU3D(ObjetU3D *o,ListeObjetU3D *ol)
{
	obj = o;
	suiv = ol;
}
//----------------------------------------------------------------------------------------------------------
ListeObjetU3D::~ListeObjetU3D()
{
	delete obj;
}
//----------------------------------------------------------------------------------------------------------
//									ListeMaterialU3D
//----------------------------------------------------------------------------------------------------------
ListeMaterialU3D::ListeMaterialU3D(MaterialU3D *m,ListeMaterialU3D *l)
{
	mat = m;
	suiv = l;
}
//----------------------------------------------------------------------------------------------------------
ListeMaterialU3D::~ListeMaterialU3D()
{
	delete mat;
}
//----------------------------------------------------------------------------------------------------------





//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									+------------------+
//									|  Class MondeU3D  |
//									+------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
MondeU3D::MondeU3D()
{
	U3D3Monde3D = this;	// c'est le monde 3D actif now

	ListeMat = NULL	;
	ItMat	 = NULL	;

	ListeCam = NULL;

	ListeObj = NULL	;
	ItObj = NULL	;

	ListeLight = NULL	;
	ItLight = NULL		;

	vec3_set( AmbientRVB, 0.0f, 0.0f, 0.0f )	;

	PolyPipeLine::GetDefaultState( PStates )	;

	NumFrame = 0.0f	;
	SetNbFrame( 0 )	;
	Tick = 0		;

	// une camera par defaut
	ActualC = new Camera("Default")		;
	AddElem( ActualC )					;

	// un material par defaut
	MaterialU3D *tmp					;
	tmp = new MaterialU3D("Default")	;
	AddElem( tmp )						;

	ListeAFDcallback = NULL	;
}
//----------------------------------------------------------------------------------------------------------
MondeU3D::~MondeU3D()
{
	ListeMaterialU3D	*Mtmp	;
	ListeCameraU3D		*Ctmp	;
	ListeObjetU3D		*Otmp	;
	ListeLightU3D		*Ltmp	;
	AfterDrawCallBack	*AFDtmp	;

	// detruit les cameras
	while( ListeCam )
	{
		Ctmp = ListeCam->suiv	;
		delete ListeCam			;
		ListeCam = Ctmp			;
	}
	// detruit les faces
	while( ListeObj )
	{
		Otmp = ListeObj->suiv	;
		delete ListeObj			;
		ListeObj = Otmp			;
	}

	// detruit les materials
	while( ListeMat )
	{
		Mtmp = ListeMat->suiv	;
		delete ListeMat			;
		ListeMat = Mtmp			;
	}
	// détruit les lights
	while( ListeLight )
	{
		Ltmp = ListeLight->suiv	;
		delete ListeLight		;
		ListeLight = Ltmp		;
	}
	// détruit les AFD callback
	while( ListeAFDcallback )
	{
		AFDtmp = ListeAFDcallback->suiv	;
		delete ListeAFDcallback			;
		ListeAFDcallback = AFDtmp		;
	}

}
//----------------------------------------------------------------------------------------------------------
bool MondeU3D::LoadU3D( LPCSTR nom )
{
	std::vector<char>*	array	;
	FILE *file = fopen(nom, "r");

	if( file ) {
		try
		{
			CArchive ar(file)	;

			if( !Serialize( ar ) )
			{
				fprintf(stderr, "<U3D> Error loading %s U3D file\n", nom );
				exit(-1);
			}

			ar.Close()		;
			fclose(file);
		}
		catch (...)
		{
			fprintf(stderr, "<U3D> Error loading %s U3D file\n", nom )	;
			exit(-1);
		}
	} else {
		#ifdef _DEBUG
			DebugErrorMess( "<U3D> Error loading %s file\n", nom )	;
		#endif
			return FALSE	;
	}

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------
// renvoie FALSE si pas reussi a ouvrir
bool MondeU3D::Serialize( CArchive &ar )
{
	U16			Chunk	;
	ObjetU3D	*Obj	;
	Camera		*Cam	;
	LightU3D	*Light	;

	if( !ar.IsStoring() )
	{

		//---------------- Test validité du fichier
		ar >> Chunk									;
		if( Chunk != CHUNK_WOLRD )	return FALSE	;

		//---------------- Recupere nbframe
		ar >> NbFrame	;

		//---------------- Recupere couleur ambient
		ar >> AmbientRVB[0]	;
		ar >> AmbientRVB[1]	;
		ar >> AmbientRVB[2]	;

		//---------------- Bcl de Chunk ki va charger les differents elements
		ar >> Chunk	;
		while( Chunk != CHUNK_WOLRD_END )
		{
			switch( Chunk )
			{
				case CHUNK_OBJ:
					Obj = new ObjetU3D							;
					if( !Obj->Serialize( ar ) )	return FALSE	;
					AddElem( Obj )								;
					break;

				case CHUNK_CAMERA:
					Cam = new Camera( "Default" )				;
					AddElem( Cam )								;
					if( !Cam->Serialize( ar ) )	return FALSE	;
					break;

				case CHUNK_LIGHT:
					Light = new LightU3D()						;
					AddElem( Light )							;
					if( !Light->Serialize( ar ) ) return FALSE	;
					break;

				default:
					return FALSE	;
					break;
			}

			ar >> Chunk	;
		}

	}
	else	return FALSE	;

	IniAfterAllLoaded()	;

	return TRUE	;
}
//----------------------------------------------------------------------------------------------------------
// fait des inies une fois tout chagé en mémoire
void MondeU3D::IniAfterAllLoaded()
{

	//------------------------ appel la meme fonction mais pour chak objets
	for( IniObjIterator(); GetObjFromIterator();  IncObjIterator() )	GetObjFromIterator()->IniAfterAllLoaded()	;

	//------------------------ met une camera par defaut ( la derniere entrée )
	SetActualCamera( GetCamera( (U32)0 ) )	;

	//------------------------- Ini Animations
	IniAnim()	;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::CallAllAFDcallback( Ufloat step )
{
	AfterDrawCallBack *tmp = ListeAFDcallback	;

	while( tmp )
	{
		(*tmp->callback)( step, 0 )	;
		tmp = tmp->suiv				;
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::AddElem(MaterialU3D *mat)
{
	ListeMaterialU3D *m;

	m = new ListeMaterialU3D( mat, ListeMat );
	TestPtr( m );
	ListeMat = m;
}
//----------------------------------------------------------------------------------------------------------
MaterialU3D *MondeU3D::GetMat( LPCSTR nom )
{
	ListeMaterialU3D *m;

	m = ListeMat;
	while( m )
	{
		if( m->mat->Cmp(nom) )
			return m->mat;
		m = m->suiv;
	}
	return NULL;
}
//----------------------------------------------------------------------------------------------------------
MaterialU3D *MondeU3D::GetOrCreateMat( LPCSTR nom )
{
	ListeMaterialU3D *m;
	MaterialU3D *tmp;

	m = ListeMat;
	while( m )
	{
		if( m->mat->Cmp(nom) )
			return m->mat;
		m = m->suiv;
	}

	// pas trouve dans la liste alors on cree
	tmp = new MaterialU3D( nom );
	AddElem( tmp );
	return tmp;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::SetAllSpecular( bool b )
{
	ListeMaterialU3D *tmp	;

	tmp = ListeMat	;
	while( tmp )
	{
		tmp->mat->SetSpecular( b )	;
		tmp = tmp->suiv				;
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::AddElem(Camera *cam)
{
	ListeCameraU3D *c;

	c= new ListeCameraU3D( cam, ListeCam );
	ListeCam = c;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::AddElem(ObjetU3D *obj)
{
	ListeObjetU3D *o;

	if( obj->IsBaseObject() )	// c'est un objet de base alors on le rajoute comme tout les autres
	{
		o = new ListeObjetU3D( obj, ListeObj );
		ListeObj = o;
	}
	else	// c'est un objet fils donc on le rajoute a son pere !
	{
		ObjetU3D	*Otmp = GetObjet( obj->GetParent() )					;
		if( Otmp )	Otmp->AddFils( obj )									;
		else FatalError( "1 objet na pas trouve son parent ( ho nan!!)" )	;
	}
}
//----------------------------------------------------------------------------------------------------------
ObjetU3D *MondeU3D::GetObjet( LPCSTR nom )
{
	ListeObjetU3D	*o		;
	ObjetU3D		*Otmp	;

	o = ListeObj	;
	while( o )
	{
		Otmp = o->obj->GetObj( nom )	;
		if( Otmp ) return Otmp			;
		o = o->suiv						;
	}

	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::ApplyMat()
{
	ListeObjetU3D *tmp=ListeObj;

	while( tmp )
	{
		tmp->obj->ApplyMat();
		tmp = tmp->suiv;
	}
}
//----------------------------------------------------------------------------------------------------------
Camera	*MondeU3D::GetCamera(LPCSTR nom)
{
	ListeCameraU3D*	cam = ListeCam	;
	U32				a=0				;

	while( cam )
	{
		if( strcmp( cam->cam->Nom(), nom )==0 )
			return cam->cam;
		cam = cam->suiv;
		a++;
	}
	return NULL;
}
//----------------------------------------------------------------------------------------------------------
Camera *MondeU3D::GetCamera(U32 nb)
{
	ListeCameraU3D *cam = ListeCam	;
	U32				a=0				;

	while( cam )
	{
		if( a== nb )
			return cam->cam;
		cam = cam->suiv;
		a++;
	}
	return NULL;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::SetActualCamera(U32 nb)
{
	SetActualCamera( GetCamera(nb) )	;
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::SetActualCamera(LPCSTR nom)
{
	SetActualCamera( GetCamera(nom) )	;
}
//----------------------------------------------------------------------------------------------------------
// va calculer la new image du monde3D non d'un petit bonhomme
void MondeU3D::UpdateFrame( float nb )
{
	MondeU3D *Save = SetActualMondeU3D( this )	;	// met ce world comme actuel et sauve le précédent

	IncAnim( nb )	;

	ActualC->UpdateProjectMatrix()				;
	ActualC->UpdateTransMatrix()				;
	ActualC->UpdatePlans()						;
	PStates.viewport = ActualC->GetViewport()	;

	U3D3Pipeline->PushState()			;	// sauve les etats du pipeline
	U3D3Pipeline->SetState( PStates )	;	// set les etats pour la scene

	TraiteObj()	;

	U3D3Pipeline->Affiche()	;		// affiches tout les polys !
	U3D3Pipeline->Release()	;		// detruits tout les polys temporaires

	CallAllAFDcallback( nb )	;	// appels toute les callback apres affichage

	Tick++									;
	if( NumFrame >= NbFrame )	IniAnim()	;	// op on recommence dans ce cas de figure

	U3D3Pipeline->PopState()	;	// restore les etats du pipeline

	SetActualMondeU3D( Save )	;	// remete l'ancien monde !
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::AddAFDCallback( LPCSTR n, PROCAFTERDRAWU3D3 func )
{
	AfterDrawCallBack *tmp	;

	tmp = new AfterDrawCallBack		;

	tmp->nom = n					;
	tmp->callback = func			;
	tmp->suiv = ListeAFDcallback	;
	ListeAFDcallback = tmp			;
}
//----------------------------------------------------------------------------------------------------------
// Fait tout se quya à faire sur les objets pour une frame
void MondeU3D::TraiteObj()
{
	ListeObjetU3D *tmp = ListeObj	;

	while( tmp )
	{
		if( tmp->obj->IsVisible() )
		{
			tmp->obj->MakeCulling()		;
			tmp->obj->CalcShading()		;
			tmp->obj->CalcUVAnimation()	;
			tmp->obj->Affiche()			;
		}
		else
			tmp = tmp	;
		tmp = tmp->suiv;
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::SetAllUVcalc(U32 t)
{
	ListeObjetU3D *tmp = ListeObj	;

	while( tmp )
	{
		tmp->obj->SetUVcalc( t )	;
		tmp = tmp->suiv				;
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::SetAllObjTypeDraw(U32 t)
{
	ListeObjetU3D *tmp = ListeObj	;

	while( tmp )
	{
		tmp->obj->SetTypeDraw( t )	;
		tmp = tmp->suiv				;
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::ConstrAllObjWireDatas()
{
	ListeObjetU3D *tmp = ListeObj	;

	while( tmp )
	{
		tmp->obj->ConstructWireDatas()	;
		tmp = tmp->suiv					;
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::ConstrAllObjColTree()
{
	ListeObjetU3D *tmp = ListeObj	;

	while( tmp )
	{
		tmp->obj->ConstructColTree()	;
		tmp = tmp->suiv					;
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::ConstructAllObjStaticLighting()
{
	ListeObjetU3D *tmp = ListeObj	;

	while( tmp )
	{
		if( !tmp->obj->IsAnimated() )
			tmp->obj->ConstructStaticLighting()	;
		tmp = tmp->suiv					;
	}
}
//----------------------------------------------------------------------------------------------------------
void MondeU3D::AddElem(LightU3D *light)
{
	ListeLightU3D *l	;

	l = new ListeLightU3D( light, ListeLight )	;
	ListeLight = l								;
}
//----------------------------------------------------------------------------------------------------------
LightU3D* MondeU3D::GetLight(U32 nb)
{
	ListeLightU3D *light = ListeLight;
	U32 a=0;

	while( light )
	{
		if( a==nb )
			return light->light;
		light = light->suiv;
		a++;
	}
	return NULL;
}
//----------------------------------------------------------------------------------------------------------
LightU3D* MondeU3D::GetLight(LPCSTR nom)
{
	ListeLightU3D	*l		;
	CString			nl		;

	l = ListeLight	;
	while( l )
	{
		if( strcmp( l->light->GetNom(), nom )==0 )
			return l->light	;
		l = l->suiv	;
	}

	return NULL	;
}
//----------------------------------------------------------------------------------------------------------
// remet toutes les animations a zero
void MondeU3D::IniAnim()
{
	ListeCameraU3D	*Ctmp	= ListeCam		;
	ListeLightU3D	*Ltmp	= ListeLight	;
	ListeObjetU3D	*Otmp	= ListeObj		;

	NumFrame =	0.0f	;

	//----------------- Ini Anim cameras
	while( Ctmp )
	{
		Ctmp->cam->IniAnim()	;
		Ctmp = Ctmp->suiv		;
	}

	//----------------- Ini Anim de lights
	while( Ltmp	)
	{
		Ltmp->light->IniAnim()	;
		Ltmp = Ltmp->suiv		;
	}


	//----------------- Ini Objets
	while( Otmp )
	{
		if( Otmp->obj->IsAnimated() )
			Otmp->obj->IniAnim()	;
		Otmp = Otmp->suiv	;
	}

}
//----------------------------------------------------------------------------------------------------------
// avance de v keyframe
void MondeU3D::IncAnim( float v )
{
	ListeCameraU3D	*Ctmp	= ListeCam		;
	ListeLightU3D	*Ltmp	= ListeLight	;
	ListeObjetU3D	*Otmp	= ListeObj		;

	NumFrame +=	v	;

	//----------------- Incrémente Animation cameras
	while( Ctmp )
	{
		Ctmp->cam->IncAnim( v )	;
		Ctmp = Ctmp->suiv		;
	}

	//----------------- Incrémente Animaton de light
	while( Ltmp	)
	{
		Ltmp->light->IncAnim( v )	;
		Ltmp = Ltmp->suiv			;
	}

	//----------------- Inicrémente Animation Objets
	while( Otmp )
	{
		if( Otmp->obj->IsAnimated() )
			Otmp->obj->IncAnim( v )	;
		Otmp = Otmp->suiv	;
	}
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si le segment coupe un poly du monde
bool MondeU3D::SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3] )
{
	U32				a = 0			;
	ListeObjetU3D*	tmp = ListeObj	;

	while( tmp  )
	{
		if( tmp->obj->SegmentIntersecPoly( debut, fin ) )
			return TRUE;
		tmp = tmp->suiv;
		a++;
	}
	return FALSE;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si le segment coupe un poly du monde et renvoie l'intersection dans fin et le poly touché dans pres !
bool MondeU3D::SegmentIntersecPoly( Ufloat debut[3], Ufloat fin[3], PolygonU3D* &pres )
{
	U32				a = 0			;
	ListeObjetU3D*	tmp = ListeObj	;
	bool			ret = FALSE		;

	while( tmp  )
	{
		if( tmp->obj->SegmentIntersecPoly( debut, fin, pres ) )
			ret =  TRUE;
		tmp = tmp->suiv;
		a++;
	}

	return ret	;
}
//----------------------------------------------------------------------------------------------------------
#ifdef _LMAPS
//----------------------------------------------------------------------------
// calcul toutes les lightmaps du monde
void MondeU3D::CalculLightMaps()
{
	ListeObjetU3D *obj = ListeObj;

	while( obj )
	{
		obj->obj->CreateLightMaps();
		obj = obj->suiv;
	}
}
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------
