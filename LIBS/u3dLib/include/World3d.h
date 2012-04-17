/*-----------------------------------------------------+
 |                       World3D.h                      |
 |                                                      |
 |    Ici ce sont les définitions principales pour      |
 |    gérer un monde 3D grace a mon moteur BSP tout     |
 |                          neuf !!                     |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/


#ifndef _World3D_U3D3_H
#define _World3D_U3D3_H


//==================
// definition d'une liste de camera

struct ListeCameraU3D {

    Camera*          cam;
    ListeCameraU3D*  suiv;

    ListeCameraU3D(Camera* c,ListeCameraU3D* cl);
    ~ListeCameraU3D();
};


//==================
// definition d'une liste de camera

struct ListeLightU3D {

    LightU3D*        light;
    ListeLightU3D*   suiv;

    ListeLightU3D(LightU3D* c,ListeLightU3D* ll);
    ~ListeLightU3D();
};



//==================
// definition d'une liste d'objet

struct ListeObjetU3D {

    ObjetU3D*        obj;
    ListeObjetU3D*   suiv;

    ListeObjetU3D(ObjetU3D* o,ListeObjetU3D* ol);
    ~ListeObjetU3D();
    bool Cmp(S8* nom);
};



//==================
// definition d'une liste da material

struct ListeMaterialU3D {

    MaterialU3D*         mat;
    ListeMaterialU3D*    suiv;

    ListeMaterialU3D(MaterialU3D* m,ListeMaterialU3D* l);
    ~ListeMaterialU3D();
};



/////////////////////////////////////////
// defintion d'un callback ki sera appelé apres l'affichage de tout le monde U3D3

typedef void (*PROCAFTERDRAWU3D3)(Ufloat step, U32 user);

struct AfterDrawCallBack {

    CString             nom;
    PROCAFTERDRAWU3D3   callback;
    AfterDrawCallBack*  suiv;

};
////////////////////////////////////////



//==================
// definition de la grôsseu Klass Monde3D poil au nez

class MondeU3D {


//------------------- datas
private:

    ListeMaterialU3D*    ListeMat;   // liste de tout les material du monde
    ListeMaterialU3D*    ItMat;

    ListeCameraU3D*  ListeCam;   // liste de toutes les cameras du monde

    ListeLightU3D*   ListeLight;   // liste de toutes les lights du monde
    ListeLightU3D*   ItLight;

    ListeObjetU3D*   ListeObj;   // liste de tout les objets du monde
    ListeObjetU3D*   ItObj;

    Ufloat  AmbientRVB[3];   // lumière ambiente !

    Camera* ActualC;       // camera actuel

    PipelineState   PStates;   // les etats nécessaires a la scene

    float   NumFrame;   // Numero de la frame actuelle
    U32     NbFrame;   // nombre de frame en tout
    U32     Tick;   // un nombre UNIQUE pour chaque frame ( meme si
    // l'anim boucle! )

    AfterDrawCallBack*   ListeAFDcallback;   // toute les callback apres affichage


//------------------- fonctions
private:

    void CallAllAFDcallback(Ufloat step);

public:

    MondeU3D();
    ~MondeU3D();

    bool LoadU3D(LPCSTR nom);
    bool LoadTXM(LPCSTR nom, U32 flag=ENGINE_TEXTURE);     // NOTE : pour le flag se référer aux créations de textures dans la classe TextureU3D
    void LoadTXM(CArchive& ar, U32 flag=ENGINE_TEXTURE);     // NOTE : pour le flag se référer aux créations de textures dans la classe TextureU3D
    bool Serialize(CArchive& ar);     // renvoie FALSE si pas reussi a ouvrir
    void IniAfterAllLoaded();   // fait des inies une fois tout chagé en mémoire

    void GetArbreBSP();


    //------------------------- Action sur les material du monde

    void            AddElem(MaterialU3D* mat);
    MaterialU3D*     GetMat(LPCSTR nom);
    MaterialU3D*     GetOrCreateMat(LPCSTR nom);
    void            IniMatIterator()                {
        ItMat=ListeMat;
    }
    void            IncMatIterator()                {
        ItMat=ItMat->suiv;
    }
    MaterialU3D*    GetMatFromIterator()            {
        return ItMat?ItMat->mat:NULL;
    }
    void            SetAllSpecular(bool b);


    //------------------------- Camera Fonction

    void    AddElem(Camera* cam);
    Camera*  GetCamera(LPCSTR nom);
    Camera*  GetCamera(U32 nb);
    Camera*  GetActualCamera()              {
        return ActualC;
    }
    void    SetActualCamera(Camera* cam)    {
        ActualC=cam;
    }
    void    SetActualCamera(U32 nb);
    void    SetActualCamera(LPCSTR nom);


    //------------------------- Objet Fonction

    void        AddElem(ObjetU3D* obj);
    ObjetU3D*   GetObjet(LPCSTR nom);
    void        IniObjIterator()                {
        ItObj=ListeObj;
    }
    void        IncObjIterator()                {
        ItObj=ItObj->suiv;
    }
    ObjetU3D*   GetObjFromIterator()            {
        return ItObj?ItObj->obj:NULL;
    }
    void        ApplyMat();   // applique les changements du au chargement des materials
    void        TraiteObj();   // Fait tout se quya à faire sur les objets pour une frame
    void        SetAllUVcalc(U32 t);
    void        SetAllObjTypeDraw(U32 t);
    void        ConstrAllObjWireDatas();
    void        ConstrAllObjColTree();
    void        ConstructAllObjStaticLighting();


    //------------------------- Light Fonction

    void        AddElem(LightU3D* light);
    LightU3D*   GetLight(U32 nb);
    LightU3D*   GetLight(LPCSTR nom);
    void        IniLightIterator()          {
        ItLight=ListeLight;
    }
    void        IncLightIterator()          {
        ItLight=ItLight->suiv;
    }
    LightU3D*    GetLightFromIterator()     {
        return ItLight?ItLight->light:NULL;
    }

    void SetAmbientColor(Ufloat R, Ufloat V, Ufloat B) {
        vec3_set(AmbientRVB,R,V,B);
    }
    void GetAmbientColor(Ufloat dest[3])                   {
        vec3_eg(dest,AmbientRVB);
    }


    //------------------------ PipeLine Fonction

    PipelineState& GetStates()  {
        return PStates;
    }
//  PolyPipeLine *GetActualPipe(){ return ActualP; }*/


    //----------------------- Time Fonction

    float   GetNumFrame()       {
        return NumFrame;
    }
    void    SetNbFrame(U32 nb)  {
        if (nb==0)nb=(U32)-1;
        NbFrame=nb;
    }
    void    IncTick()           {
        Tick++;
    }
    U32     GetTick()           {
        return Tick;
    }


    //----------------------- Anim Fonction

    void IniAnim();   // remet toutes les animations a zero
    void IncAnim(float v);     // avance de v keyframe


    //----------------------- Geometrie Fonction

    bool SegmentIntersecPoly(Ufloat debut[3], Ufloat fin[3]);     // renvoie TRUE si le segment coupe un poly du monde
    bool SegmentIntersecPoly(Ufloat debut[3], Ufloat fin[3], PolygonU3D*& pres);    // renvoie TRUE si le segment coupe un poly du monde et renvoie l'intersection dans fin et le poly touché dans pres !


    //----------------------- Affichage Fonction

    void UpdateFrame(float nb=1.f);       // Avance de NB frame dans le temps et affiche le tout


    //----------------------- Callback Fontions

    void AddAFDCallback(LPCSTR nom, PROCAFTERDRAWU3D3 func);


    //--------------------- Section ki concerne les tools

#ifdef TOOLU3D

    void AddElem(mesh3ds* mesh);
    void AddElem(camera3ds* cam, kfcamera3ds* kcam);
    void AddElem(light3ds* light);

    void CalculLightMaps();         // calcul toutes les lightmaps du monde

#endif

};

//==================

#endif
