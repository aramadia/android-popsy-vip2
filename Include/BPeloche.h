/*---------------------------------------------------------------------+
 |                          BPeloche.h                                  |
 |                                                                      |
 |   une sorte de gros layer avec plein de defaut ( comme sur un vieux  |
 |                      film super 8 koa ).                             |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#ifndef _BPELOCHE_VIP2_H
#define _BPELOCHE_VIP2_H

/////////////////////////////////////////
class BPeloche {

//--------------- Datas
private:

    static TextureU3D*   TexPic1;   // image de la particule de base
    static MaterialU3D*  MatPic1;   // material de la particule de base

    static BitmapU3D*    GBmp;   // bitmap utilisée de partout

    Ufloat  time;

    Ufloat  Alpha;   // transparence du tout
    Ufloat  IncAlpha;
    Ufloat  GotoAlpha;

//--------------- Fonctions
public:

    static void Initialise();
    static void Release();

    BPeloche();

    void SetAlpha(Ufloat a)                                    {
        Alpha = a;
        GotoAlpha=a;
        IncAlpha=0.0f;
    }
    void Interposalpha(Ufloat debut, Ufloat fin, Ufloat time);

    void UpdateFrame(Ufloat laptime);
};
/////////////////////////////////////////

#endif
