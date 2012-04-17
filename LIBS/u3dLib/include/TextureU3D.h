/*-----------------------------------------------------+
 |                   TextureU3D.h                       |
 |                                                      |
 |  La classe ki va s'occuper de tritoyer et des texture|
 |              comme il faut poil au dos               |
 |                                                      |
 |  U2^PoPsy TeAm 2000                              |
 +-----------------------------------------------------*/


#ifndef _Texture_U3D3_H
#define _Texture_U3D3_H


//==================
// definition d'une texture

#define TILE_TEXTURE        0x01    // est-ce kon permet le tiling ?
#define BILINEAR_TEXTURE    0x02
#define MIPMAP_TEXTURE      0x04    // est-ce kon va créer des mimaps normale ?
#define ALPHA_TEXTURE       0x08    // est-ce ke la texture contient une composante alpha ?
#define LOW_TEXTURE         0x10    // Flag positionné a low kan on utilise un rendu 16 bit !
#define TRILINEAR_TEXTURE   0x20    // est-ce kon va créer des mimaps ave du trilinear

#define ENGINE_TEXTURE      BILINEAR_TEXTURE|TRILINEAR_TEXTURE

class TextureU3D {

    U32     BindNum;   // numéro de texture pour OpenGL
    U8*     data;   // pointeur sur texture en RAM
    U8**     mipmap;   // pointeurs sur tout les niveaux de mipmaping en RAM
    U32     largeur,hauteur;
    U32     utilisateurs;   // nb utilisateurs de cette texture
    U32     FlagCapa;   // capacité de la texture
    U32     nbLevelOfMipmap;   // nombre de mimpa pour cette texture
    bool    load;   // est-ce que texturel loadée ?

public:

    TextureU3D(U32 flag=TILE_TEXTURE|BILINEAR_TEXTURE);
    ~TextureU3D();

    void    Create(U32 largeur, U32 hauteur);     // alloue espace nécessaire pour la resolution de texture donnée
    void    CreatDefaultMipmaps();   // va créer les textures de mipmaping automatiquement !
    void    Load();   // load texture dans la carte 3D
    bool    IsLoaded()                          {
        return load;
    }
    bool    Kill();   // renvoie TRUE si vraiment delete
    void    IncUser()                           {
        utilisateurs++;
    }

    U8*  GetTexture()                                       {
        return data;   // renvoie pointeur mémoire de la texture
    }
    U8*  GetTexMipMap(U32 num, U32& width, U32& height);     // retourne pointeur sur mipmap num, largeur mipmap et hauteur mipmap
    U32 Width()                                             {
        return largeur;
    }
    U32 Height()                                            {
        return hauteur;
    }
    U32 GetBindNum()                                        {
        return BindNum;
    }

    void SetTiling(bool b) {
        b? FlagCapa|=TILE_TEXTURE : FlagCapa&=~TILE_TEXTURE;
    }
    void SetAlpha(bool b);

    bool IsTileTexture()        {
        return (FlagCapa&TILE_TEXTURE) ? TRUE:FALSE;
    }
    bool IsBilinearTexture()    {
        return (FlagCapa&BILINEAR_TEXTURE) ? TRUE:FALSE;
    }
    bool IsMipMapTexture()      {
        return ((FlagCapa&MIPMAP_TEXTURE)||(FlagCapa&TRILINEAR_TEXTURE)) ? TRUE:FALSE;
    }
    bool IsLowTexture()         {
        return (FlagCapa&LOW_TEXTURE) ? TRUE:FALSE;
    }
    bool IsAlphaTexture()       {
        return (FlagCapa&ALPHA_TEXTURE) ? TRUE:FALSE;
    }

};


#endif
