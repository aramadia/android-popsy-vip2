/*-----------------------------------------------------+
 |                       objet3d.h                      |
 |                                                      |
 |    Que serait une monde 3D sans ses objets alors     |
 |      vala leur définitions a tout ces petits         |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _objet3d_U3D3_H
#define _objet3d_U3D3_H


class Objet3dWire;   // from Objet3dWire.h
class ObjStaticLighting;   // from ObjStaticLighting.h
class ObjBones;   // from ObjetBones.h
class OneObjBones;   // from ObjetBones.h

//==================
// les différents types d'objets!

/*#define TypeObjetShading_Rien     0x0
#define TypeObjetShading_Flat       0x1
#define TypeObjetShading_Gouraud    0x2*/

/*#define UVCalcNone        0x0
#define UVCalcEnv       0x1
#define UVCalcEnv1      0x2
#define UVCalcCallBack  0x3 // !!!!!! TODO !!!!!!*/


#define TypeObjetDraw_Solid 0x1
#define TypeObjetDraw_Line  0x2

//////////////////////////////////////////////////////////////
// defintion d'un objet 3D
// TODO : cleaner un peu tout ce bordel !!!!!!!!!!
class ObjetU3D {


//------------------- datas
private:
    CString nom;
    CString parent;   // nom du parent de l'objet
    U32 IDnum;   // numero d'identification unique !

    Mat3x4  LocalMatrix;
    Mat3x4  WorldMatrix;

    U32 typeShading;   // type de shading qu'il fo appliquer a l'objet
    U32 typeUVcalc;   // type de modifier de coordonnée UV de l'objet
    U32 typeDraw;   // type de drawing de l'objet !
    U32 Recalc;   // flag indiquant tout se ke l'on doit recalculer pour un objet

    ObjetU3D* enfant;   // les objets dépendants de celui-ci
    ObjetU3D* suiv;   // suivant dans le méme niveau hiérarchique

    BoundingBox     Blocal;
    BoundingBox     Bworld;
    AABoundingBox   Box;

    Objet3dWire*        Wire;
    BTree*              ColTree;   // arbre pour accélérer les collisions ( mais couteux en mémoire )
    ObjStaticLighting*  StaticLighting;   // si objet static alors on a pu calculer les lumières static

    SplineU3D*       Spos;   // mouvement dans le temps
    SplineRotU3D*    Srot;   // rotations de l'objets

    SplineMorphU3D*  Smorph;   // morphing de l'objets
    U16             nbMorphKey;
    MorphKeyFileU3D* TmpMorph;   // va sauver temporairement les clé de morphing avant de les transformer en spline

    HideU3D* Hides;   // hide key si il y en a de l'objet

    ObjBones*   Bones;   // bones de l'objet si il en a !

public:

    U32         nbcoords;
    CoordU3D*    Ctab;   // tableau de coordonnées

    U32         nbvertices;
    VertexU3D*   Vtab;   // table des vertex de l'objet

    U32         nbpolys;
    PolygonU3D*  Ptab;   // table des polys de l'objet


//----------------- flags
public:

    // Type de shading
    enum {
        snone = 0x0,
        flat = 0x1,
        gouraud = 0x2
    };

    // Type de calcul sur les coords de texture
    enum {
        cnone = 0x0,
        envStage0 = 0x1,
        envStage1 = 0x2,
        callback = 0x3
    };

    // Type de drawing
    enum {
        dnone = 0x0,
        solid = 0x1,
        line = 0x2
    };

    // Type de recalcul
    enum {
        bbox            = 0x01,
        normalsPoly     = 0x02,
        normalsVertex   = 0x04,
        shading         = 0x08,
        culling2d       = 0x10, // kan actif calcul le backface culling en 2D et non en 3D
        collision       = 0x20  // gere les collisions si demandé
    };

//----------------- fonctions
private:

    void LoadCoord(CArchive& ar);         // load tableau de coordonée depuis l'archive
    void LoadVertices(CArchive& ar);         // load tableau de vertex depuis l'archive
    void LoadPolys(CArchive& ar);         // load tableau de polygons depuis l'archive
    void LoadMats(CArchive& ar);         // load tableau de material depuis l'archive

public:

    ObjetU3D();
    ObjetU3D(LPCSTR n, U32 nbc, U32 nbv, U32 nbp);
    void Reset();
    ~ObjetU3D();

    bool Serialize(CArchive& ar);

    void Ini();   // fait toute les ini nécessaires a l'objet ( doit etre appelé au - 1 fois apres le serialize! )
    void IniAfterAllLoaded();   // des ini ki dépendent d'autre objets dans la base -> donc fo attendre ke tout soit loadé !


    void            ConstructWireDatas();
    void            KillWireData();
    Objet3dWire&    GetWire()               {
        return *Wire;
    }

    void    ConstructColTree();
    void    KillColTree();
    BTree&  GetColTree()        {
        return *ColTree;
    }

    void ConstructStaticLighting();

    void AddBones(OneObjBones* bone);
    void ConfirmBones();

    LPCSTR      GetNom()            {
        return (LPCSTR)nom;
    }
    LPCSTR      GetParent()         {
        return (LPCSTR)parent;
    }
    bool        Cmp(LPCSTR n);
    ObjetU3D*    GetObj(LPCSTR n);     // renvoie l'objet ki porte le nom n
    Mat3x4&      GetObjToWorld()    {
        return WorldMatrix;
    }
    Mat3x4&      GetObjMatrix()     {
        return LocalMatrix;
    }
    U32         GetUVcalculation()  {
        return typeUVcalc;
    }
    U32         GetTypeDraw()       {
        return typeDraw;
    }
    U32         GetNbArretes();   // renvoie nombre d'arretes de l'objet


    bool        IsVisible();   // renvoie TRUE si objet visible
    bool        IsAnimated();   // renvoie TRUE si objet animé
    bool        IsBaseObject();   // renvoie TRUE si objet ne possede pas de parent
    bool        IsMorph()           {
        return Smorph?TRUE:FALSE;   // renvoie TRUE si l'objet a des étapes de morphing
    }
    bool        IsFlatShading()     {
        return (typeShading&flat)?TRUE:FALSE;
    }
    bool        IsGouraudShading()  {
        return (typeShading&gouraud)?TRUE:FALSE;
    }

    void CalcCoordNormales(bool realtime=FALSE);     // calcul les normales au vertex
    void CalcPolyNormales(bool realtime=FALSE);     // calcul les normales aux polys
    void CalcNormales(bool realtime=FALSE);     // calcul toutes les normales de l'objet
    void CalcBBox();   // calcul bounding box de l'objet

    void    Transform(Mat3x4& mat);     // transforme toute les coordonné par cette matrice

    void AddFils(ObjetU3D* obj);     // ajoute un fils a cet objet

//  Unoeud_BSP* GetBSP(){return arbre;}

    void ApplyMat();   // calcul les coords au vertex et fait de settings en fct des materials de l'objet

    void SetShadingFlat()       {
        typeShading|=flat;
    }
    void SetShadingGouraud()    {
        typeShading|=gouraud;
    }
    void SetUVcalc(U32 t)       {
        typeUVcalc=t;
    }
    void SetTypeDraw(U32 t)     {
        typeDraw=t;
    }
    void SetRecalcFlag(U32 f)   {
        Recalc=f;
    }


    void MakeCulling();   // essaye d'enlever un max de face pour cette frame ( en fonction de la position de la camera )
    void MakeCulling(Ufloat pos[3], bool test2side=FALSE);     // essaye d'enlever un max de face pour cette frame ( ici en fonction d'un point quelconque )
    void Make2DCulling(bool test2side=FALSE);     // pareil ke makeculling sauf k'ici on fait ca apres projecton des coords des points ( donc pas en 3D )


    bool SegmentIntersecPoly(Ufloat debut[3], Ufloat fin[3]);     // renvoie TRUE si le segment coupe un poly du monde
    bool SegmentIntersecPoly(Ufloat debut[3], Ufloat fin[3], PolygonU3D*& pres);    // renvoie TRUE si le segment coupe un poly du monde et renvoie l'intersection dans fin et le poly touché dans pres !
    bool SphereIntersecObj(Ufloat pos[3], Ufloat rayon);     // renvoie TRUE si la sphere coupe la bounding box de l'objet. ( attention pos dans espace objet )


    void CalcMatrixFromAnim();   // calcul la matrice local depuis l'animation de l'objet
    void IniAnim(Mat3x4* mat=NULL);     // ini animation de l'objet ! ( et des objets dépendants )
    void IncAnim(Ufloat v, Mat3x4* mat=NULL);    // avance l'objet de v keyframe

    void CalcShading();   // calcul le shading de l'objet si il en a
    void CalcUVAnimation();   // calcul efet spéciaux sur les UV ( comme env map )

    void Affiche();     // Zou affiche l'objet

};
//////////////////////////////////////////////////////////////


#endif
