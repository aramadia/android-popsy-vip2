/*---------------------------------------------------------+
 |                   Objet3dWire.h                          |
 |                                                          |
 |      va permettre de gérer les arretes affichables       |
 |                      d'un poly                           |
 | On pourra aussi se servir de ces données pour faire      |
 | des rendus cartoon ou de la lumière pseudo volumiques    |
 |                                                          |
 |  U2^PoPsy TeAm 2000                                  |
 +---------------------------------------------------------*/

#ifndef _OBJWIRE_U3D3_H
#define _OBJWIRE_U3D3_H


struct PolyObjWire;
////////////////////////////////////////
// une liste de PolygonU3D en fait
struct PolyObjWireList {

    PolygonU3D*         poly;
    PolyObjWireList*    suiv;

    PolyObjWireList() {
        poly = NULL;
        suiv=NULL;
    }
};
////////////////////////////////////////


////////////////////////////////////////
// structure de coordonnées ki stock tout les poly ki le touche
struct CoordObjWire {

    CoordU3D*           coord;
    PolyObjWireList*    allpoly;
    CoordObjWire*       suiv;

    CoordObjWire(CoordU3D* c)   {
        coord=c;
        suiv=NULL;
        allpoly=NULL;
    }
    ~CoordObjWire();

    void Set(CoordU3D* c)          {
        coord = c;
    }
    void AddPloly(PolygonU3D* p);
};
////////////////////////////////////////


////////////////////////////////////////
// une liste de PolygonU3D en fait
struct PolyObjWire {

    CoordObjWire**  allcoord;   // un tableau de coordonnée
    PolygonU3D*     poly;

    PolyObjWire()   {
        allcoord=NULL;
        poly=NULL;
    }
    ~PolyObjWire();

    void Set(PolygonU3D* p);
};
////////////////////////////////////////





////////////////////////////////////////
// définition d'une ligne d'un objet wire
class LigneObjWire {

    union {
        CoordObjWire*   pt1Tmp;
        CoordU3D*       pt1;
    };

    union {
        CoordObjWire*   pt2Tmp;
        CoordU3D*       pt2;
    };

    PolygonU3D* gauche;
    PolygonU3D* droit;
    Ufloat      RVBA[2][4];   // couleur a chak sommet

public:

    LigneObjWire();

//  void Set( CoordU3D* pt1, CoordU3D* pt2, PolygonU3D* gauche );
    void Set(CoordObjWire* pt1, CoordObjWire* pt2, PolygonU3D* gauche);
    void SetPolyGauche(PolygonU3D* p)                                      {
        gauche=p;
    }
    void SetPolyDroit(PolygonU3D* p)                                       {
        droit=p;
    }

    void SetPt1(CoordU3D* c)       {
        pt1=c;
    }
    void SetPt2(CoordU3D* c)       {
        pt2=c;
    }
    void SetColor(Ufloat rvba[4])  {
        vec4_eg(RVBA[0],rvba);
        vec4_eg(RVBA[1],rvba);
    }

    CoordObjWire*   GetPt1Tmp() {
        return pt1Tmp;
    }
    CoordObjWire*   GetPt2Tmp() {
        return pt2Tmp;
    }
    PolygonU3D*     GetGauche() {
        return gauche;
    }
    PolygonU3D*     GetDroit()  {
        return droit;
    }

    bool IsOutline();   // renvoie TRUE si c'est une outline 2D de l'objet

    void Affiche()                                                          {
        U3D3Pipeline->AfficheLigneFromEngine(pt1, pt2, RVBA);
    }
    void AfficheVolume(MaterialU3D* mat, Ufloat pos[3], Ufloat taille);

};
////////////////////////////////////////



////////////////////////////////////////
class Objet3dWire {

    ObjetU3D* objet;

    CoordObjWire*   listeCoord;   // une liste ki stock tout les poly attachés a une coordonnée
    PolyObjWire*    tabPoly;   // un tableau de poly temporaire
    LigneObjWire*   tabWire;
    U32             NbLigne;

    U32 TypeAffichage;

    Ufloat          PosVolumeLight[3];
    Ufloat          TailleVolumeLight;   // taille des poly a extruder
    MaterialU3D*    MatVolumelight;   // material assignée aux face créées pour la lumière volumik

//------- flags
public:

    enum {
        normal = 0x0,
        cartoon = 0x1,
        volumelight = 0x2,
        disable = 0x3
    };

//------- fonctions
private:

    CoordObjWire*   AddPolyToCoord(CoordU3D* c, PolygonU3D* p);
    void            CreateTabs(ObjetU3D* obj);
    void            CleanTabs();

public:

    Objet3dWire();
    ~Objet3dWire();

    void Construct(ObjetU3D* obj);

    void SetLineColor(Ufloat rvba[4]);
    void SetPosVolumeL(Ufloat pos[3])  {
        vec3_eg(PosVolumeLight, pos);
    }
    void SeTailleVolumeL(Ufloat v) {
        TailleVolumeLight=v;
    }

    void Affiche(U32 type = normal, MaterialU3D* mat=NULL);     // attention le pipeline doit etre setté comme il fo pour chak type !! ( doit setter un material si on est en volumelight )
};
////////////////////////////////////////

#endif
