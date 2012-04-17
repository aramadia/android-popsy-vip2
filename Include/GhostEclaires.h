/*-----------------------------------------------------------------+
 |                          GhostEclaire.h                          |
 |                                                                  |
 |   je vasi faire jaillir des petits eclaires des tuyaux de cette  |
 |                  fabuleuse scene trop oufement kiffante          |
 |                                                                  |
 |  U2^PoPsy TeAm 2000                                          |
 +-----------------------------------------------------------------*/

#ifndef _GHOSTECLAIRE_VIP2_H_
#define _GHOSTECLAIRE_VIP2_H_



//////////////////////////////////////////////
// une liste d'objet générateur d'ecrlaires
struct GhostEclaireGenerator {

    ObjetU3D*               obj;
    GhostEclaireGenerator*  suiv;

};
//////////////////////////////////////////////





//////////////////////////////////////////////
// un eclaire
class GhostOneEclaire {

//------------------ Datas
private:

    ObjetU3D*       obj;
    HSplinaeAff*    spline;   // servira seulement a calculer le déplacement
    Ufloat*         interPos;

    Ufloat Galpha;
    Ufloat alpha;
    Ufloat incAlpha;

public:

    GhostOneEclaire* suiv;

//------------------ Fonctions
private:

    void CalcCoord(Ufloat dir[3], Ufloat totaltime);      // Calcul la position des coordonées
    void AssignCoord(Ufloat time);

public:

    GhostOneEclaire(MondeU3D* m, MaterialU3D* mat);
    ~GhostOneEclaire();

    Ufloat* GetLastPos();
    Ufloat* GetFirstPos();
    Ufloat GetAlpha()       {
        return alpha*Galpha;
    }

    void CalcNew(MondeU3D* m, Ufloat posdep[3], Ufloat vdir[3]);

    bool IsFinish() {
        if (alpha==0) return TRUE;
        else return FALSE;
    }

    void IncAnim(Ufloat time, Ufloat laptime);
};
//////////////////////////////////////////////





//////////////////////////////////////////////
// ze classe ki va gérer les eclaires
class GhostEclaire {

//------------------ Datas
private:

    TextureU3D*  partTex;   // image de la particule de base
    MaterialU3D* partMat;   // material de la particule de base
    BitmapU3D*   partBmp;   // bitmpa de la particule

    MondeU3D*       Monde;
    MaterialU3D*    Mat;   // le material pour les eclaires

    GhostEclaireGenerator*  generators;
    GhostOneEclaire*        eclairs;

    GhostEclaireGenerator* next; // prochain objet générateur


    GhostAllEtincelle* Etincelles;   // des pitites etincelles ki jaillissent vlou vlou

//------------------ Fonctions
private:

    void Genere(GhostOneEclaire* ec);
    void AffOneParti(Ufloat pos[3], Ufloat alpha);

public:

    GhostEclaire(MondeU3D* monde);
    ~GhostEclaire();


    void IncAnim(Ufloat time, Ufloat laptime);
    void AffAllPart(Ufloat laptime);
};
//////////////////////////////////////////////

#endif

