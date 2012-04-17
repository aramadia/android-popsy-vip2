/*---------------------------------------------------------+
 |                   Objet3dBones.h                         |
 |                                                          |
 |      une implémentation des bones dans l'engine.         |
 |              avec ces Bones il pourra                    |
 | y avoir N matrices ki influenceront N vertex vala.       |
 |                                                          |
 |  U2^PoPsy TeAm 2000                                  |
 +---------------------------------------------------------*/

#ifndef _OBJBONES_U3D3_H
#define _OBJBONES_U3D3_H



////////////////////////////////////////
// coordonnées d'un point par rapport au bones
struct BonesCoord {

    Ufloat      Influence;
    Ufloat      origine[3];
    CoordU3D*   coord;

    BonesCoord()    {
        Influence=0.0f;
        vec3_set(origine, 0.0f, 0.0f, 0.0f);
        coord=NULL;
    }
};
////////////////////////////////////////



struct BoneAfectCoord;

////////////////////////////////////////
// un seul et unik bone
class OneObjBones {

//------------------- datas
private:

    Ufloat  RayonInfluenceMax;   // sphere a l'interieur de lakelle les vertex son pleinement influencés
    Ufloat  RayonInfluenceMin;   // sphere au dela de lakelle le bone n'agit plus
    bool    InvInfluence;   // inverse la sphere d'influence du bone

    Mat3x4  LocalMatrix;
    Mat3x4  ObjMatrix;
    Mat3x4  WorldMatrix;

    SplineU3D*       Spos;   // mouvement dans le temps
    SplineRotU3D*    Srot;   // rotations de l'objets

    U32         nbcoord;
    U32         actuacoord;
    BonesCoord* AllCoords;

public:

    OneObjBones* suiv;   // bones suivant ( au meme niveau hiérarchik )
    OneObjBones* enfant;   // bones dependants de celui-ci

//------------------- fonctions
public:

    OneObjBones(Ufloat RayonInfluenceMin, Ufloat RayonInfluenceMax = 0.0f, bool invInflu=FALSE);
    ~OneObjBones();

    void SetPos(Ufloat x, Ufloat y, Ufloat z)  {
        LocalMatrix.SetPos(x, y, z);
    }

    void    SendObjMatrix(const Mat3x4& objMat);     // va calculer la matrice de transof relativement a l'objet
    Mat3x4& GetLocaMatrix()                         {
        return LocalMatrix;
    }

    void CalcInfluence(BoneAfectCoord* coord);

    void AddCoord(CoordU3D* coord, Ufloat influence);     // rajoute coord au tableau AllCoords a la position actuacoord et incrémente actuacoord

    void CalcMatrixFromAnim();
    void IniAnim(const Mat3x4& Mat);
    void IncAnim(const Mat3x4& objMat, float v);

    void Transform();   // calcul new position des coord du bone
};
////////////////////////////////////////



////////////////////////////////////////
// structure intermédiare
struct BonesList {

    Ufloat      influence;
    OneObjBones* Bone;
    BonesList*   suiv;

    BonesList() {
        Bone=NULL;
        suiv=NULL;
        influence=0.0f;
    }
};
////////////////////////////////////////





////////////////////////////////////////
// structure intermédiare
struct BoneAfectCoord {

    BonesList*  Bones;
    Ufloat      TotInfluence;
    CoordU3D*   coord;

    BoneAfectCoord()    {
        Bones=NULL;
        TotInfluence=0.0f;
        coord=NULL;
    }
    inline ~BoneAfectCoord();

    void inline AddBone(OneObjBones* bone, Ufloat influence);
};
////////////////////////////////////////



////////////////////////////////////////
// liste des coordonées touchées par les bones
struct BonesCoordListe {

    CoordU3D*           coord;
    Ufloat              Influence;
    Ufloat              origine[3];
    BonesCoordListe*    suiv;

    BonesCoordListe()   {
        coord=NULL;
        suiv=NULL;
    }
};
////////////////////////////////////////



////////////////////////////////////////
// définition d'une ligne d'un objet wire
class ObjBones {

//------------------- datas
private:

    OneObjBones*        AllBones;   // tout les bones de l'objet
    BonesCoordListe*    ListeCoord;   // liste de coordonnées affectées par les bones

//------------------- fonctions
public:

    ObjBones();
    ~ObjBones();

    void AddBone(OneObjBones* bone);

    void Confirm(ObjetU3D* obj);     // Calcul les influence et place les bones

    void IniAnim(const Mat3x4& objMat);     // ini animation des bones  ( avec objMat = matrice de transfo objetToWorld )
    void IncAnim(const Mat3x4& objMat, float v);    // avance dans l'anim des bones de v keyframe ( avec objMat = matrice de transfo de objetToWorld )

    void Transform(ObjetU3D* obj);     // calcul la position dans le monde des coordonnées de l'objet
};
////////////////////////////////////////



//----------------------------------------------------------------------------------------------------------
//                                  +------------------------+
//                                  |  Les Fonctions Inline  |
//                                  +------------------------+
//----------------------------------------------------------------------------------------------------------
inline BoneAfectCoord::~BoneAfectCoord() {
    BonesList* tmp;

    while (Bones) {
        tmp = Bones->suiv;
        delete Bones;
        Bones = tmp;
    }
}
//----------------------------------------------------------------------------------------------------------
void inline BoneAfectCoord::AddBone(OneObjBones* bone, Ufloat influence) {
    BonesList* tmp;

    tmp = new BonesList;
    tmp->Bone = bone;
    tmp->influence = influence;
    tmp->suiv = Bones;
    Bones = tmp;
}
//----------------------------------------------------------------------------------------------------------

#endif
