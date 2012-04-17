//
//              +-------------------------------------------------------------------+
//              |                       ElemAff.h                                   |
//              |                                                                   |
//              | Defininit tout les elements ki devront etre affichés poil au nez  |
//              |                                                                   |
//              |  David-U2/GameSquad 1999(c)                                       |
//              +-------------------------------------------------------------------+


//=============================
// definition d'ne bitmap de la skin
class BitmapSkin {

    int     IDBit;
    int     dx,dy;
    U32*    Bit;
    bool    Mask;   // est-ce ke l'on gere couleur de mask ?

public:

    BitmapSkin*  suiv;
    BitmapSkin*  suivList;

    BitmapSkin(int ID, LPCSTR nomOpen, bool msk=FALSE);
    ~BitmapSkin();

    int GetXSize()  {
        return dx;
    }
    int GetYSize()  {
        return dy;
    }

    bool Cmp(int ID)   {
        if (ID==IDBit)return TRUE;
        else return FALSE;
    }

    void Affiche(int x, int y);     // affiche la bitmap
    void Affiche(int debx, int deltax, int x, int y);     // affiche la bitmap

};



//=============================
// definition d'un bouton de la skin
class BoutonSkin {

    int     ID;   // ID du bouton
    int     xpos,ypos;
    int     colormask;   // couleur dans le mask
    bool    Etat;   // si Etat==TRUE alors la souris est sur le bouton

    BitmapSkin*  HighLight;   // bitmap kan souris sur le bouton  !

public:

    BoutonSkin*  suiv;

    BoutonSkin(int ID, int xp, int yp, int colormask, BitmapSkin* b);

    int     GetColorMask()  {
        return colormask;
    }
    int     GetID()         {
        return ID;
    }
    bool    GetEtat()       {
        return Etat;
    }

    void SetHighLight(bool b)  {
        Etat=b;
    }

    bool    IsInside(int color)    {
        if (color==colormask) return TRUE;
        else return FALSE;
    }
    bool    Cmp(int id)        {
        if (ID==id)return TRUE;
        else return FALSE;
    }

    void Affiche();

};


//=============================
// d'une zone avec plusieurs images affichables !

class MultiAff {

    int         ID;
    int         xpos,ypos;

    BitmapSkin* All;   // toute les bitmap
    BitmapSkin* Actua;   // bitmap actuellement selectionnée

public:

    MultiAff*    suiv;

    MultiAff(int id, int xp, int yp);

    int GetID()         {
        return ID;
    }
    bool Cmp(int id)   {
        if (ID==id)return TRUE;
        else return FALSE;
    }

    void AddBitmap(BitmapSkin* ptr);
    void SetActuaBitmap(int id);

    void Affiche();

};
