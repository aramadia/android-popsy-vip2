/*-----------------------------------------------------+
 |                       TexMana.h                      |
 |                                                      |
 |   C'est un Manager de texture ou plutot d'images en  |
 |    en tout genre qui permet de renvoyer les nimages  |
 |       que l'on veut avoir en indiquant leur nom.     |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/


#ifndef _TexMana_U3D3_H
#define _TexMana_U3D3_H

////////////////////////////////////////////////////////////
struct TextureListe {

    TextureU3D*  tex;
    CString     nom;
    CString     nomMask;

    TextureListe* suiv;

    TextureListe();
    ~TextureListe();

};
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
#define FlagTexOpen32bit    0x0
#define FlagTexOpenMask     0x4

class TextureManager {

    static TextureListe* Liste;
    static TextureListe* IteratorPos;

    static char* Repertoir;

    static U32 ForceFlag;   // flag de capacité ke l'on force pour chak texture allouée

    static void         DeleteListe(TextureListe* actuel, TextureListe* preced);
    static TextureListe* Getl(LPCSTR nom);
    static TextureListe* Getl(TextureU3D* tex);

public:

    static void Initialise();
    static void Release();

    static void SetForceFlag(U32 flag) {
        ForceFlag=flag;   // voir textureU3D.h pour la liste des flags
    }

    static void Add(TextureU3D* tex, LPCSTR nom);     // ajoute texture dans la liste
    static void AddUse(TextureU3D* tex);     // déclare cette texture comme étant utilisee au moins 1 fois
    static void AddUse(LPCSTR nom);
    static void SubUse(TextureU3D* tex);     // Attention SubUse verifie que la texture n'est plus utilisee
    static void SubUse(LPCSTR nom);     // avant de l'eventuellement la detruire
    static void KILL(TextureU3D* tex);     // tandis que KILL la detruit quoa qu'il arrive
    static void KILL(LPCSTR nom);     // tandis que KILL la detruit quoa qu'il arrive

    static TextureU3D*  Get(LPCSTR nom);
    static LPCSTR       GetNom(TextureU3D* tex);
    static LPCSTR       GetNomMask(TextureU3D* tex);
    static TextureU3D*  GetForUse(LPCSTR nom);
    static bool         Exist(TextureU3D* tex);      // renvoie TRUE sur texture presente

    static void     SetRepertoire(LPCSTR nom)  {
        free(Repertoir);
        Repertoir = strdup(nom);
    }
    static LPCSTR   GetRepertoire()             {
        return Repertoir;
    }

    //----------------- ouvre le fichier depuis le Carchive et l'ajoute dans la liste des textures
    static TextureU3D*  OpenRaw(CArchive& ar, LPCSTR nom, U32 x, U32 y, U32 Texflag=TILE_TEXTURE|BILINEAR_TEXTURE);     // Note : pour TexFlag se référé a la class TextureU3D
    static TextureU3D*  OpenJpg(CArchive& ar, LPCSTR nom, U32 lenghtdata, U32 Texflag=TILE_TEXTURE|BILINEAR_TEXTURE);      // Note lenghtdata = taille du fichier Jpeg !
    static U8*          NewRaw(CArchive& ar, U32 x, U32 y, U32 nbOctet=1);     // ouvre un .raw et renvoie l'adresse de l'image
    static U32*         NewJpg(CArchive& ar, U32& x, U32& y, U32 lenghtdata);      // ouvre un .jpg et renvoie l'adresse de en 32 bit et la taille x et y de l'image
    static bool         OpenMask(TextureU3D* tex, CArchive& ar);

    //---------------- ouvre le fichier depuis un nom de répertoire
    static TextureU3D*  OpenRaw(LPCSTR nom,U32 x, U32 y, U32 Texflag=TILE_TEXTURE|BILINEAR_TEXTURE);     // Note : pour TexFlag se référé a la class TextureU3D
    static TextureU3D*  OpenJpg(LPCSTR nom, U32 Texflag=TILE_TEXTURE|BILINEAR_TEXTURE);
    static U8*          NewRaw(LPCSTR nom,U32 x, U32 y, U32 nbOctet=1);     // ouvre un .raw et renvoie l'adresse de l'image
    static U32*         NewJpg(LPCSTR nom, U32& x, U32& y);      // ouvre un .jpg et renvoie l'adresse de en 32 bit et la taille x et y de l'image
    static bool         OpenMask(TextureU3D* tex, LPCSTR nomMask);     // ouvre un mask pour cette texture

    static void LoadAll();       // Load toutes les textures dans la carte 3D
    static void DeleteAll();       // detruit toute les texture du manager

    //--------- Fonction pour le parcours des texture de la base
    static void         IniIterator()               {
        IteratorPos=Liste;   // ini l'iterateur pour parcourir toutes les texture de la base
    }
    static void         IncIterator()               {
        IteratorPos=IteratorPos->suiv;
    }
    static TextureU3D*  GetFromIterator()           {
        return IteratorPos? IteratorPos->tex : NULL;
    }
    static LPCSTR       GetNomFromIterator()        {
        return IteratorPos? (LPCSTR)IteratorPos->nom : NULL;
    }
    static LPCSTR       GetNomMaskFromIterator()    {
        return IteratorPos? (LPCSTR)IteratorPos->nomMask : NULL;
    }

    static void SerializeTxt(CArchive& ar);    // sauve info sous format texte !

};
////////////////////////////////////////////////////////////



#endif
