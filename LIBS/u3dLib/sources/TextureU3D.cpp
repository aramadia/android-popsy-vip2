/*-----------------------------------------------------+
 |                    TextureU3D.cpp                    |
 |                                                      |
 |  La classe ki va s'occuper de tritoyer et des texture|
 |              comme il faut poil au dos               |
 |                                                      |
 |  U2^PoPsy TeAm 2000                              |
 +-----------------------------------------------------*/


#include "U3D3.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +-------------------------+
//                                  |    Class TextureU3D     |
//                                  +-------------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
TextureU3D::TextureU3D(U32 flag) {
    BindNum = (U32)-1;
    data    = NULL;
    mipmap = NULL;
    largeur = 0;
    hauteur = 0;
    utilisateurs = 0;
    FlagCapa    = flag;
    nbLevelOfMipmap = 0;
    load = FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
TextureU3D::~TextureU3D() {
    if (data)           delete [] data;
    if (IsLoaded()) glDeleteTextures(1, &BindNum);

    if (mipmap) {
        for (U32 a=0; a<nbLevelOfMipmap; a++)
            if (mipmap[a])  delete [] mipmap[a];

        delete [] mipmap;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
// alloue espace nécessaire pour la resolution de texture donnée
void TextureU3D::Create(U32 larg, U32 haut) {
    U32     a,latmp,hatmp,nbbit;
    bool    sortie;

    largeur = larg;
    hauteur = haut;

    /*  if( IsLowTexture() )    nbbit = 2;
        else*/
    if (IsAlphaTexture() || IsLowTexture()) nbbit = 4;
    else                                        nbbit = 3;

    data = new U8[largeur*hauteur*nbbit];


    //----------------------- Trouve et alloue les mipmaps nécessaires

    if (IsMipMapTexture()) {
        nbLevelOfMipmap = 0;
        latmp = largeur;
        hatmp = hauteur;
        sortie = FALSE;

        //------------------- tourve le nombre de mipmap nécessaires
        while (!sortie) {
            latmp>>= 1;
            if (latmp<1) latmp = 1;
            hatmp>>= 1;
            if (hatmp<1) hatmp = 1;

            if ((latmp==1) && (hatmp==1)) sortie = TRUE;

            nbLevelOfMipmap++;
        }

        //------------------ Alloue le tabeau et les images de mipmaps
        latmp = largeur;
        hatmp = hauteur;
        mipmap = new U8*[nbLevelOfMipmap];
        for (a=0; a<nbLevelOfMipmap; a++) {
            latmp>>= 1;
            if (latmp<1) latmp = 1;
            hatmp>>= 1;
            if (hatmp<1) hatmp = 1;

            mipmap[a] = new U8[latmp*hatmp*nbbit];
        }
    }

    //-----------------------
}
//----------------------------------------------------------------------------------------------------------------------------------------
static U32 nbMIP = 0;
// va créer les textures de mipmaping automatiquement !
void TextureU3D::CreatDefaultMipmaps() {
    U32 a,latmp,hatmp,nbbit;
    U32 latmpP,hatmpP;
    U8*  srctmp;

    if (!IsMipMapTexture()) return;

    if (IsAlphaTexture() || IsLowTexture()) nbbit = 4;
    else                                     nbbit = 3;

    latmp = latmpP = largeur;
    hatmp = hatmpP = hauteur;

    srctmp = data;
    for (a=0; a<nbLevelOfMipmap; a++) {
        latmp>>= 1;
        if (latmp<1) latmp = 1;
        hatmp>>= 1;
        if (hatmp<1) hatmp = 1;

        if (IsAlphaTexture() || IsLowTexture()) Make32AlphaBitMimap(mipmap[a], latmp, hatmp, srctmp, latmpP, hatmpP);
        else {
            Make24BitMimap(mipmap[a], latmp, hatmp, srctmp, latmpP, hatmpP);

            /*          CString sctmp;
                        sctmp.Format( "D:/sources/3D/U3D3/GFX/mipmap %d.raw", nbMIP++ );
                        CFile file;
                        file.Open( sctmp, CFile::modeCreate|CFile::modeWrite );
                        CArchive ar( &file, CArchive::store );
                        ar.Write( mipmap[a], latmp*hatmp*3 );
                        ar.Close();
                        file.Close();*/
        }

        srctmp = mipmap[a];
        latmpP = latmp;
        hatmpP = hatmp;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
// load texture dans la carte 3D
void TextureU3D::Load() {
    U32 a,latmp,hatmp,bitComponent,bitFormat;

    if (IsLoaded()) return;
    if (!data)          return;

    glGenTextures(1, &BindNum);   // Génère un ID pour la tetxure
    glBindTexture(GL_TEXTURE_2D, BindNum);   // La fonction pour dire que la texture courante c'est celle référencée par "TextureID"

    //-------------------- Determine format interne de stockage de la texture
    if (IsLowTexture()) {
        bitComponent = GL_RGB5_A1;
        bitFormat = GL_RGBA;
    } else {
        if (IsAlphaTexture()) {
            bitComponent = GL_RGBA8;
            bitFormat = GL_RGBA;
        } else {
            bitComponent = GL_RGB8;
            bitFormat = GL_RGB;
        }
    }

    //------------------ envoie image de base !
    glTexImage2D(GL_TEXTURE_2D, 0, bitComponent, largeur, hauteur, 0, bitFormat, GL_UNSIGNED_BYTE, data);

    //----------------- envoie les mipmaps si yen a
    if (IsMipMapTexture()) {
        CreatDefaultMipmaps();

        latmp = largeur;
        hatmp = hauteur;

        for (a=0; a<nbLevelOfMipmap; a++) {
            latmp>>= 1;
            if (latmp<1) latmp = 1;
            hatmp>>= 1;
            if (hatmp<1) hatmp = 1;

            glTexImage2D(GL_TEXTURE_2D, a+1, bitComponent, latmp, hatmp, 0, bitFormat,
                         GL_UNSIGNED_BYTE, mipmap[a]);
        }
    }


    if (FlagCapa&BILINEAR_TEXTURE) {
        if (IsMipMapTexture()) {
            if (FlagCapa&TRILINEAR_TEXTURE) {
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            } else {
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            }
        } else {
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    } else {
        if (IsMipMapTexture()) {
            if (FlagCapa&TRILINEAR_TEXTURE) {
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            } else {
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            }
        } else {
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
    }

    if (FlagCapa&TILE_TEXTURE) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }

    delete [] data;
    data = NULL;

    for (a=0; a<nbLevelOfMipmap; a++) {
        delete [] mipmap[a];
        mipmap[a] = NULL;
    }
    delete [] mipmap;
    mipmap = NULL;

    load = TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
bool TextureU3D::Kill() {
    utilisateurs--;

    if (utilisateurs<1) {
        if (data)           delete [] data;
        data = NULL;
        if (IsLoaded()) glDeleteTextures(1, &BindNum);

        if (mipmap) {
            for (U32 a=0; a<nbLevelOfMipmap; a++) {
                if (mipmap[a])  delete [] mipmap[a];
                mipmap[a] = NULL;
            }
            delete [] mipmap;
            mipmap = NULL;
        }

        return TRUE;
    }

    return FALSE;
}
//----------------------------------------------------------------------------------------------------------------------------------------
// retourne pointeur sur mipmap num, largeur mipmap et hauteur mipmap
U8*  TextureU3D::GetTexMipMap(U32 num, U32& width, U32& height) {
    U32 a,latmp,hatmp;

    if (num>nbLevelOfMipmap) return NULL;

    latmp = largeur;
    hatmp = hauteur;

    for (a=0; a<num; a++) {
        latmp>>= 1;
        if (latmp<1) latmp = 1;
        hatmp>>= 1;
        if (hatmp<1) hatmp = 1;
    }

    width = latmp;
    height = hatmp;

    return mipmap[ a-1 ];
}
//----------------------------------------------------------------------------------------------------------------------------------------
void TextureU3D::SetAlpha(bool b) {
    U32 a = 0;
    U8* tmpData;


    if (IsAlphaTexture() == b)  return;    // pas de changements donc rien a faire

    if (IsAlphaTexture() && !b) {
        FlagCapa&=~ALPHA_TEXTURE;   // la texture n'a plus de composante alpha

        tmpData = new U8[ largeur*hauteur*3 ];

        U32 dep1 = 0;
        U32 dep2 = 0;
        for (a=0; a<largeur*hauteur; a++,dep1+=4,dep2+=3) {
            tmpData[ dep2 + 0 ] = data[ dep1 + 0 ];
            tmpData[ dep2 + 1 ] = data[ dep1 + 1 ];
            tmpData[ dep2 + 2 ] = data[ dep1 + 2 ];
        }

        delete [] data;
        data = tmpData;
    } else {
        FlagCapa|=ALPHA_TEXTURE;   // op on met une composante alpha a la texture

        tmpData = new U8[ largeur*hauteur*4  ];

        U32 dep1 = 0;
        U32 dep2 = 0;
        for (a=0; a<largeur*hauteur; a++,dep1+=3,dep2+=4) {
            tmpData[ dep2 + 0 ] = data[ dep1 + 0 ];
            tmpData[ dep2 + 1 ] = data[ dep1 + 1 ];
            tmpData[ dep2 + 2 ] = data[ dep1 + 2 ];
            tmpData[ dep2 + 3 ] = 0;
        }

        delete [] data;
        data = tmpData;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
