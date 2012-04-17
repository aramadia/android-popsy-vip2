/*-----------------------------------------------------+
 |                      Bitmap.h                        |
 |                                                      |
 |  des classes pour gérer efficacement et facilement   |
 |          l'affichage d'image 2D avec OpenGL          |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _BITMAP_U3D3_H
#define _BITMAP_U3D3_H


extern PolyPipeLine* U3D3Pipeline;   // from U3D3.h

/////////////////////////////////////////////////////////////
// une bitmap 2D
class BitmapU3D {

    MaterialU3D* mat;

    Ufloat  RVBA[4][4];   // couleur gouraud aux 4 sommets
    Ufloat  XYZ[4][3];   // coordonnées des 4 sommets ( valeurs unitaires )
    Ufloat  UV[4][4];   // coordonnées textures aux 4 sommets

    U32 flag;

//---------------- enum pour le flagage
public:

    enum {
        normal = 0x0,
        zwrite = 0x01,
        ztest = 0x02
    };

public:

    BitmapU3D(MaterialU3D* mat, U32 flag=normal);

    BitmapU3D& operator=(BitmapU3D& bmp);

    void            SetMaterial(MaterialU3D* m)    {
        mat=m;
    }
    MaterialU3D*    GetMaterial()                   {
        return mat;
    }

    void SetColor(Ufloat R, Ufloat V, Ufloat B, Ufloat A=1.0f);     // met une couleur pour toute l'image
    void SetColor(Ufloat col[4]);     // met une couleur pour toute l'image
    void SetColor(U32 sommet, Ufloat col[4]);     // met une couleur pour le sommet n
    void SetColor(U32 sommet, Ufloat R, Ufloat V, Ufloat B, Ufloat A=1.0f);     // met une couleur pour le sommet n
    void SetAlpha(U32 sommet, Ufloat A)                                        {
        RVBA[sommet][3] = A;
    }

    void SetMapCoord(U32 sommet, Ufloat u, Ufloat v, U32 stage=0);     // met une coord de mapping pour le sommet n

    void SetCoord(U32 sommet, Ufloat X, Ufloat Y, Ufloat Z=ProcheZClipping+0.1f);
    void SetCoord(U32 sommet, Ufloat xyz[3]);

    void Affiche()  {
        U3D3Pipeline->AfficheQuad(XYZ, RVBA, UV);
    }
};
/////////////////////////////////////////////////////////////

#endif
