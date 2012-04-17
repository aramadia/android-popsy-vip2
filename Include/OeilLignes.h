/*-------------------------------------------------------------------------+
 |                              OeilLignes.h                                |
 |                                                                          |
 |  C'est une classe ki va afficher des elements "designs" par dessus l'oeil|
 |                                                                          |
 |  U2^PoPsy TeAm 2000                                                  |
 +-------------------------------------------------------------------------*/

#ifndef _OEILLIGNES_VIP2_H
#define _OEILLIGNES_VIP2_H


/////////////////////////////////////////
struct OeilLigneList {

    BSplinaeAff*    spline;
    Ufloat*         posDest;
    Ufloat*         pos;

    OeilLigneList*  suiv;

    OeilLigneList()     {
        spline=NULL;
        posDest=NULL;
        pos=NULL;
        suiv=NULL;
    }
    ~OeilLigneList()    {
        delete spline;
        delete []posDest;
        delete []pos;
    }
};
/////////////////////////////////////////



/////////////////////////////////////////
// 1 seule boule
class OeilLigne {

//--------------- Datas
private:

    OeilLigneList* XLigne;
    OeilLigneList* YLigne;


//--------------- Fonctions
private:

    void AddXLigne(Ufloat xdebut, Ufloat xdelta, Ufloat y);
    void AddYLigne(Ufloat ydebut, Ufloat ydelta, Ufloat x);

    void GereLigne(OeilLigneList* ligne);

public:

    OeilLigne();
    ~OeilLigne();

    void Deform(Ufloat intens);

    void Affiche(Ufloat laptime);
};
/////////////////////////////////////////

#endif

