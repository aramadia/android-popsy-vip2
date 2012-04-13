    				 /*-----------------------------------------------------+
					  |				        MotionKeys.h					|
					  |														|
					  |	  se sont les différentes clés d'animation utilisé  |
					  |					dans le fichier U3D 				|
					  |														|
					  | 	U2^PoPsy TeAm 1999								|
					  +-----------------------------------------------------*/

#ifndef _MOTIONKEYS_U3D3_H
#define _MOTIONKEYS_U3D3_H

//==================

struct KeyHeaderU3D{

	Ufloat	time		;
	Ufloat	tension		;
	Ufloat	continuity	;
	Ufloat	bias		;
	Ufloat	easeto		;
	Ufloat	easefrom	;

	void Serialize( CArchive &ar )
	{
		if( ar.IsStoring() )
		{
			ar << time			;
			ar << tension		;
			ar << continuity	;
			ar << bias			;
			ar << easeto		;
			ar << easefrom		;
		}
		else
		{
			ar >> time			;
			ar >> tension		;
			ar >> continuity	;
			ar >> bias			;
			ar >> easeto		;
			ar >> easefrom		;
		}
	}

	void Ini()
	{
		time = 0			;
		tension = 0.0f		;
		continuity = 0.0f	;
		bias = 0.0f			;
		easeto = 0.0f		;
		easefrom = 0.0f		;
	}

};

//==================

struct PosKeyFileU3D{

	Ufloat			p[3]	;
	KeyHeaderU3D	k		;

	void Serialize( CArchive &ar )
	{
		if( ar.IsStoring() )
		{
			ar << p[0]			;
			ar << p[1]			;
			ar << p[2]			;
			k.Serialize( ar )	;
		}
		else
		{
			ar >> p[0]			;
			ar >> p[1]			;
			ar >> p[2]			;
			k.Serialize( ar )	;
		}
	}

};

//==================

struct RotKeyFileU3D{

	Ufloat			q[4]	;
	KeyHeaderU3D	k		;

	void Serialize( CArchive &ar )
	{
		if( ar.IsStoring() )
		{
			ar << q[0]			;
			ar << q[1]			;
			ar << q[2]			;
			ar << q[3]			;
			k.Serialize( ar )	;
		}
		else
		{
			ar >> q[0]			;
			ar >> q[1]			;
			ar >> q[2]			;
			ar >> q[3]			;
			k.Serialize( ar )	;
		}
	}

};

//==================

struct UniKeyFileU3D{

	Ufloat			u	;
	KeyHeaderU3D	k	;

	void Serialize( CArchive &ar )
	{
		if( ar.IsStoring() )
		{
			ar << u				;
			k.Serialize( ar )	;
		}
		else
		{
			ar >> u				;
			k.Serialize( ar )	;
		}
	}

};



//==================

struct MorphKeyFileU3D{

	char *nom	;
	KeyHeaderU3D	k	;

	void Serialize( CArchive &ar )
	{
		if( ar.IsStoring() )
		{
			ar << nom			;
			k.Serialize( ar )	;
		}
		else
		{
			ar >> nom			;
			k.Serialize( ar )	;
		}
	}

};

//==================

struct HideKeyFileU3D{
	U32 pos	;

	void Serialize( CArchive &ar )	
	{
		if( ar.IsStoring() )
			ar << pos	;
		else
			ar >> pos	;
	}
};

//==================

#endif
