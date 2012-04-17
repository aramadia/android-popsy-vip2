/*-----------------------------------------------------+
 |                       Fpipe.cpp                      |
 |                                                      |
 |  ne voyez aucun jeu de mot avec le nom de ce fichier |
 |      c'est simplement la gestion du Pipeline de      |
 |    l'affichage des faces dans un ordre correct ! |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/


#include "U3D3.h"


#define DEFAULT_ALPHAREF 0.1f

//----------------------------------------------------------------------------------------------------------
//                                  +---------------------+
//                                  |    Les Fonctions    |
//                                  +---------------------+
//----------------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
// Class MatPipeline
//----------------------------------------------------------------------------------------------------------
// créé un pipeline compatible avec ce polygone
MatPipeline::MatPipeline(MaterialU3D* m) {
    Tex1            = m->GetTex1();
    Tex2            = m->GetTex2();
    Transparence1   = m->GetTrTex1();
    Transparence2   = m->GetTrTex2();

    liste = NULL;
    DynaAdd = FALSE;

    utilisateurs = 1;

    suiv        = NULL;
    preced      = NULL;
    PipeSuiv    = NULL;
}
//----------------------------------------------------------------------------------------------------------
// SetOpenGL pour rendre le material comme il fo
// ATTENTION :on suppose ke le GL_BLEND est enablé
void MatPipeline::SetRenderState() {
    if (!IsNoTexture()) {
        if (Tex1) {
            glActiveTextureARB(GL_TEXTURE0_ARB);   //  Enable texturing !
            glBindTexture(GL_TEXTURE_2D, Tex1->GetBindNum());
            glEnable(GL_TEXTURE_2D);
        }
        if (Tex2) {
            glActiveTextureARB(GL_TEXTURE1_ARB);   //  Enable texturing !
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, Tex2->GetBindNum());
            SetTranspOn2ndStage();
        } else {
            glActiveTextureARB(GL_TEXTURE1_ARB);
            glDisable(GL_TEXTURE_2D);     //  Enleve texturing sur 2eme stage
        }

        glActiveTextureARB(GL_TEXTURE0_ARB);   //  remet texture 0 comme courante
    } else {
        glActiveTextureARB(GL_TEXTURE0_ARB);   // Disable texturing !
        glDisable(GL_TEXTURE_2D);
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glDisable(GL_TEXTURE_2D);
    }

    switch (Transparence1) {
    case MatTranspAdd :
        glBlendFunc(GL_ONE, GL_ONE);
        break;

    case MatTranspAddAlpha :
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        break;

    case MatTranspMul :
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        break;

    case MatTranspMulAlpha :
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
        break;

    case MatTranspAlpha :
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;

    case MatTranspIncrust:
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR, GL_ONE);
        break;
    }

}
//----------------------------------------------------------------------------------------------------------
// SetOpenGL pour rendre le material comme il fo ( mais en tenant compte des state deja setté par le precedent ! )
// TODO : gérer le multitexturing !!!!!!!!!!!
void MatPipeline::SetRenderState(MatPipeline* mp) {
    glEnd();   // arret les precedents render states

    //------------------------------- Traitement de la 1ere texture

//  if( Tex1 != mp->Tex1 )  // la texture des 2 materials est différente
    {
        if (Tex1 && mp->Tex1) {  // les 2 ont au moins 1 texture
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glBindTexture(GL_TEXTURE_2D, Tex1->GetBindNum());
        } else if (Tex1 && !mp->Tex1) {  // le nouveau a une texture et pas l'ancien
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, Tex1->GetBindNum());
        } else if (Tex1 && !mp->Tex1) {  // le nouveau n'a pas de texture mais l'ancien en avait
            glActiveTextureARB(GL_TEXTURE0_ARB);
            glDisable(GL_TEXTURE_2D);
        }
    }

    //-------------------------------



    //------------------------------- Traitement de la 1ere transparence

//  if( Transparence1 != mp->Transparence1 )
    {
        switch (Transparence1) {
        case MatTranspAdd :
            glBlendFunc(GL_ONE, GL_ONE);
            break;

        case MatTranspAddAlpha :
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;

        case MatTranspMul :
            glBlendFunc(GL_ZERO, GL_SRC_COLOR);
            break;

        case MatTranspMulAlpha :
            glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
            break;

        case MatTranspAlpha :
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        }
    }

    //-------------------------------


    glBegin(GL_TRIANGLES);     // op c'est reparti !!!!!!!
}
//----------------------------------------------------------------------------------------------------------
// met les good render states pour la combinaison entre les 2 texture en multitexturing
// ATTENTION : la 2eme texture doit etre active
void MatPipeline::SetTranspOn2ndStage() {
    switch (Transparence2) {
    default                 :
    case MatTranspAddAlpha  :
    case MatTranspAdd       :
///         glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
//          glTexEnvf( GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD );
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
        break;


    case MatTranspMulAlpha  :
    case MatTranspMul       :
    case MatTranspAlpha     :
        /*          glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
                    glTexEnvf( GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MULT );*/
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        break;
    }
}
//----------------------------------------------------------------------------------------------------------
bool MatPipeline::IsGood(MaterialU3D* mat) {
    if (mat->GetTex1()!=Tex1)               return FALSE;
    if (mat->GetTex2()!=Tex2)               return FALSE;
    if (mat->GetTrTex1()!=Transparence1)    return FALSE;
    if (mat->GetTrTex2()!=Transparence2)    return FALSE;
    return TRUE;
}
//----------------------------------------------------------------------------------------------------------
bool MatPipeline::IsNoTexture() {
    if (!Tex1 && !Tex2) return TRUE;
    return FALSE;
}
//----------------------------------------------------------------------------------------------------------
bool MatPipeline::Is1Texture() {
    if (Tex1 && !Tex2) return TRUE;
    return FALSE;
}
//----------------------------------------------------------------------------------------------------------
bool MatPipeline::Is2Texture() {
    if (Tex1 && Tex2) return TRUE;
    return FALSE;
}
//----------------------------------------------------------------------------------------------------------
bool MatPipeline::IsTransparent() {
    if ((Transparence1!=MatTranspRien) && (Transparence1!=MatTranspTrou)) return TRUE;
    return FALSE;
}
//----------------------------------------------------------------------------------------------------------
bool MatPipeline::IsOpaque() {
    if (Transparence1==MatTranspTrou) return TRUE;
    return FALSE;
}
//----------------------------------------------------------------------------------------------------------
// renvoie TRUE si les 2 material on les render state compatible
bool MatPipeline::IsCompatible(MatPipeline* mp) {
    if (mp == this)                         return TRUE;    // evite tout les test si c'est le cas
    if (Tex1 != mp->Tex1)                       return FALSE;
    if (Transparence1 != mp->Transparence1) return FALSE;
    if (Tex2 != mp->Tex2)                       return FALSE;
    if (Transparence2 != mp->Transparence2) return FALSE;

    return TRUE;
}
//----------------------------------------------------------------------------------------------------------
// NB : Fonction ne marchant k'avec des polys non tranparents !
void MatPipeline::Affiche() {
//  Camera      *cam = U3D3Monde3D->GetActualCamera();
//  U32         frameNum = U3D3Monde3D->GetTick();
    PolygonU3D*  p1 = liste;

    //------------------------ fait les settings OpenGL
    if (Tex1) {
        glActiveTextureARB(GL_TEXTURE0_ARB);   //  Enable texturing !
        glBindTexture(GL_TEXTURE_2D, Tex1->GetBindNum());
    }
    if (Tex2) {
        glActiveTextureARB(GL_TEXTURE1_ARB);   //  Enable texturing sur 2eme stage
        glBindTexture(GL_TEXTURE_2D, Tex2->GetBindNum());
        SetTranspOn2ndStage();   // definit le type de mélange entre les couches
    }

    glBegin(GL_TRIANGLES);

    //------------------------ aller zouuuuu on affiche !
    if (Tex1 && Tex2) {
        while (p1) {
            RasterizMultiTexture(p1);
            p1 = p1->suiv;
        }
    } else if (Tex1) {
        while (p1) {
            RasterizTexture(p1);
            p1 = p1->suiv;
        }
    } else {
        while (p1) {
            RasterizNoTexture(p1);
            p1 = p1->suiv;
        }
    }

    glEnd();
}
//----------------------------------------------------------------------------------------------------------
void MatPipeline::Release() {
    PolygonU3D* tmp;

    while (liste) {
        tmp = liste;
        liste = liste->suiv;
        if (tmp->flag&PolyFLAG_TEMP)
            U3D3Manager->FreePoly(tmp);
    }

    DynaAdd = FALSE;
}
//----------------------------------------------------------------------------------------------------------






//----------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////
// Class PolyPipeline
//----------------------------------------------------------------------------------------------------------
// Créé un nouveau pipeline !
PolyPipeLine::PolyPipeLine(UManager* m, U32 NbMaxSortedPoly) {
    U32 a;

    AllMatPipeline = NULL;       // tout les MatPipeline sont ici

    //--------------------- les differents type de pipeline trié par materials lors du rendu
    NoMapPipeLine       = NULL;
    UneMapPipeLine      = NULL;
    UneMapOpaPipeLine   = NULL;
    MultiMapPipeLine    = NULL;

    Speculars = NULL;

    PLinesRien = NULL;
    PLinesAdditif = NULL;
    PLinesMultipli = NULL;

    MaxPolySort = NbMaxSortedPoly;
    listeTr = new PolySort[ NbMaxSortedPoly ];
    nbTrPoly = 0;


    //-------------------- Ini le poly static
    MStatic = NULL;
    PStatic = new PolygonU3D(4);
    for (a=0; a<4; a++) {
        PStatic->Vtab[a] = &VStatic[a];
        VStatic[a].IncUser();
        VStatic[a].c = &CStatic[a];
        CStatic[a].IncUser();
    }

    stackState = NULL;

    ActuaViewport.Set(0, Xres-1, 0, Yres-1);

    AlphaRef = DEFAULT_ALPHAREF;   // référence Alpha pour les Alpha tests
    SortPolyTr = TRUE;

#if defined(_DEBUG) || defined(TOOLU3D)
    NBspeculars = 0;
#endif
}
//----------------------------------------------------------------------------------------------------------
PolyPipeLine::~PolyPipeLine() {
    MatPipeline*    tmp;
    PipelineState*  tmps;

    Release();
    delete [] listeTr;

    //---------------------------- Kill Tout les MatPipeline
    while (AllMatPipeline) {
        tmp = AllMatPipeline;
        AllMatPipeline = AllMatPipeline->suiv;
        delete tmp;
    }

    //-------------------- Kill tout se ki a dans la pile de states
    while (stackState) {
        tmps = stackState->suiv;
        delete stackState;
        stackState = tmps;
    }

    //-------------------- Kill le poly static
    delete PStatic;
}
//----------------------------------------------------------------------------------------------------------
MatPipeline* PolyPipeLine::GetOrCreate(MaterialU3D* mat) {
    MatPipeline* tmp = AllMatPipeline;

    while (tmp) {
        if (tmp->IsGood(mat))
            return tmp;
        tmp = tmp->suiv;
    }

    tmp = new MatPipeline(mat);
    tmp->suiv = AllMatPipeline;
    if (AllMatPipeline) AllMatPipeline->preced = tmp;
    AllMatPipeline = tmp;

    return tmp;
}
//----------------------------------------------------------------------------------------------------------
void PolyPipeLine::Release(MatPipeline* mp) {
    mp->DecUser();
    if (mp->GetUse() > 1) return;

    //--------------- cas ou lon libere vraiment la mémoire
    if (mp->preced) {
        mp->preced->suiv = mp->suiv;
        if (mp->suiv) mp->suiv->preced = mp->preced;
    } else {
        AllMatPipeline = mp->suiv;
        if (AllMatPipeline) AllMatPipeline->preced = NULL;
    }
    delete mp;
}
//----------------------------------------------------------------------------------------------------------
// met les renderstate par defaut
void PolyPipeLine::DefaultRenderState() {
    glActiveTextureARB(GL_TEXTURE0_ARB);   // Disable texturing !
    glDisable(GL_TEXTURE_2D);     // "                 "
    glActiveTextureARB(GL_TEXTURE1_ARB);   // "                 "
    glDisable(GL_TEXTURE_2D);     // "                 "

    glDisable(GL_BLEND);     // Disable Blending
    glDepthMask(GL_TRUE);     // permet d'ecrire dans le Zbuff
    glEnable(GL_DEPTH_TEST);   // effectue le test de Zbuff

    glAlphaFunc(GL_GEQUAL, AlphaRef);     // met le type de test alpha correct
    glDisable(GL_ALPHA_TEST);     // Disable alpha test
}
//----------------------------------------------------------------------------------------------------------
// met les render state en accord avec le material
void PolyPipeLine::Begin(MaterialU3D* mat, bool Zwrite, bool Ztest) {
    MatPipeline* mp = (MatPipeline*)mat->GetPipe();

    if (Zwrite)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);

    if (Ztest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    if (mp->IsTransparent())
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    mp->SetRenderState();
    MStatic = mat;

    glBegin(GL_TRIANGLES);
}
//----------------------------------------------------------------------------------------------------------
// met en place les render state ( mais pour une ligne )
void PolyPipeLine::Begin(U32 transp, bool Zwrite, bool Ztest) {
    if (Zwrite)
        glDepthMask(GL_TRUE);
    else
        glDepthMask(GL_FALSE);

    if (Ztest)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    switch (transp) {
    case MatTranspAdd :
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        break;

    case MatTranspAddAlpha :
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        break;

    case MatTranspMul :
        glEnable(GL_BLEND);
        glBlendFunc(GL_ZERO, GL_SRC_COLOR);
        break;

    case MatTranspMulAlpha :
        glEnable(GL_BLEND);
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
        break;

    case MatTranspAlpha :
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;

    case MatTranspIncrust:
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR, GL_ONE);
        break;
    }

    glBegin(GL_LINES);
}
//----------------------------------------------------------------------------------------------------------
// clear render state
void PolyPipeLine::End(MaterialU3D* mat) {
    glEnd();
    DefaultRenderState();
}
//----------------------------------------------------------------------------------------------------------
void PolyPipeLine::AddDynaMatPipe(MatPipeline* mp) {
    mp->SetDynaAdd();

    if (mp->IsOpaque()) {
        mp->PipeSuiv = UneMapOpaPipeLine;
        UneMapOpaPipeLine = mp;
    } else if (mp->IsNoTexture()) {
        mp->PipeSuiv = NoMapPipeLine;
        NoMapPipeLine = mp;
    } else if (mp->Is1Texture()) {
        mp->PipeSuiv = UneMapPipeLine;
        UneMapPipeLine = mp;
    } else if (mp->Is2Texture()) {
        mp->PipeSuiv = MultiMapPipeLine;
        MultiMapPipeLine = mp;
    }
}
//----------------------------------------------------------------------------------------------------------
// fait Qsort sur les polys tranparents !
void PolyPipeLine::Sort(S32 deb, S32 fin) {
    S32         i=deb;
    S32         j=fin;
    Ufloat      milieu=listeTr[(deb+fin)/2].z;
    PolySort    tmp;

    while (i<=j) {
        while (listeTr[i].z > milieu) i++;
        while (listeTr[j].z < milieu) j--;

        if (i<=j) {
            tmp.z=listeTr[i].z;
            tmp.p=listeTr[i].p;

            listeTr[i].z=listeTr[j].z;
            listeTr[i].p=listeTr[j].p;

            listeTr[j].z=tmp.z;
            listeTr[j].p=tmp.p;
            i++;
            j--;
        }
    }
    if (i<fin) Sort(i,fin);
    if (deb<j) Sort(deb,j);
}
//----------------------------------------------------------------------------------------------------------
// retourne poly réellement ajouté au pipeline ( clipé et tout comme il fo )
PolygonU3D* PolyPipeLine::AddPolyDyna(PolygonU3D* p) {
//  VertexU3D   **V;
    MatPipeline* mp = (MatPipeline*)p->m->GetPipe();
    PolygonU3D*  paff = p;
//  U32         frameNum = U3D3Monde3D->GetTick();
//  Camera      *cam = U3D3Monde3D->GetActualCamera();


    //--------------------------- Calcul le veritable poly si besoin

    /*  V = p->Vtab;
        for(U32 a=0; a<p->nbVertex; a++,V++)
            cam->Projet( *V, frameNum );       // calcul coords 2D*/

    if (p->BesoinClip()) {
        paff = p->GetClippedPolyByCamera();
        if (!paff) return NULL;
    }

    //---------------------------


    //-------------------------- rajoute un poly specular si besoin
    if (p->m->IsSpecular())
        AddPolySpecular(paff);


    if (mp->IsTransparent()) {
        AddPolyTr(paff);         // met dans la liste des polys a trier
    } else {
        if (!mp->IsDynaAdd())
            AddDynaMatPipe(mp);

        mp->AddPoly(paff);
    }

    return paff;
}
//----------------------------------------------------------------------------------------------------------
// ajoute un polygone ki est forcément detruit apres affichage !
// ( contrairement a AddPolyDyna ou l'on y met un poly ki "restera en vie" ).
void PolyPipeLine::AddPolyDynaTemporary(PolygonU3D*  p) {
    PolygonU3D* tmp;

    tmp = AddPolyDyna(p);
    if (tmp != p)   U3D3Manager->FreePoly(p);
}
//----------------------------------------------------------------------------------------------------------
void PolyPipeLine::AddPolyLines(PolygonU3D* p) {
//  VertexU3D   **V;
//  MatPipeline *mp = (MatPipeline*)p->m->GetPipe();
    PolygonU3D*  paff = p;
//  U32         frameNum = U3D3Monde3D->GetTick();
//  Camera      *cam = U3D3Monde3D->GetActualCamera();

    //--------------------------- Calcul le veritable poly si besoin

    /*  V = p->Vtab;
        for(U32 a=0; a<p->nbVertex; a++,V++)
            cam->Projet( *V, frameNum );       // calcul coords 2D*/

    if (p->BesoinClip()) {
        paff = p->GetClippedPolyByCamera();
        if (!paff) return;
    }

    //---------------------------

    if (!paff->arrete) {     // ya pas de ligne à afficher alors ô revoir
        if (paff!=p) U3D3Manager->FreePoly(paff);
        return;
    }

    AddPolyLinesNoClip(paff);
}
//----------------------------------------------------------------------------------------------------------
// sauve tout les etats du pipeline
void PolyPipeLine::PushState() {
    PipelineState* tmp;

    tmp = U3D3Manager->NewPipeState();
    tmp->AlphaRef = AlphaRef;
    tmp->SortPolyTr = SortPolyTr;
    tmp->viewport = ActuaViewport;

    tmp->suiv = stackState;
    stackState = tmp;
}
//----------------------------------------------------------------------------------------------------------
// retouve les etats
void PolyPipeLine::PopState() {
    PipelineState* tmp;

    if (!stackState)    return;

    SetState(*stackState);

    tmp = stackState;
    stackState = stackState->suiv;
    U3D3Manager->FreePipeState(tmp);
}
//----------------------------------------------------------------------------------------------------------
void PolyPipeLine::DefaultState() {
    AlphaRef = DEFAULT_ALPHAREF;
    SortPolyTr = TRUE;
    ActuaViewport.Set(0, Xres-1, 0, Yres-1);
}
//----------------------------------------------------------------------------------------------------------
// renvoie dans state les etats par defaut
void PolyPipeLine::GetDefaultState(PipelineState& state) {
    state.AlphaRef = DEFAULT_ALPHAREF;
    state.SortPolyTr = TRUE;
    state.viewport.Set(0, Xres-1, 0, Yres-1);
}
//----------------------------------------------------------------------------------------------------------
void PolyPipeLine::SetState(PipelineState& state) {
    AlphaRef = state.AlphaRef;
    SortPolyTr = state.SortPolyTr;
    ActuaViewport = state.viewport;
}
//----------------------------------------------------------------------------------------------------------
// affiche le contenu du pipeline en fonctions des tout les états prédéfinits
// par les polygones camera etc....
void PolyPipeLine::Affiche() {
    U32         a;
    PolySort*    PS;
    MatPipeline* MPpreced,*MPactua;
    MatPipeline* tmp;
    PolygonU3D*  Ptmp;


    DefaultRenderState();

    //----------------------------- Affiche dabord les polys sans texture

    glDisable(GL_BLEND);     // Disable Blending

    tmp = NoMapPipeLine;
    while (tmp) {
        tmp->Affiche();
        tmp = tmp->PipeSuiv;
    }



    //---------------------------- Affiche ensuite les polys avec 1 seule texture

    glActiveTextureARB(GL_TEXTURE0_ARB);   //  Enable texturing !
    glEnable(GL_TEXTURE_2D);     //  "               "

    tmp = UneMapPipeLine;
    while (tmp) {
        tmp->Affiche();
        tmp = tmp->PipeSuiv;
    }



    //--------------------------- Affiche les polys en avec 1 seule texture mais avec un Tests Alpha

    glEnable(GL_ALPHA_TEST);
    tmp = UneMapOpaPipeLine;
    while (tmp) {
        tmp->Affiche();
        tmp = tmp->PipeSuiv;
    }
    glDisable(GL_ALPHA_TEST);



    //--------------------------- Affiche les polys en Multitexturing

    glActiveTextureARB(GL_TEXTURE1_ARB);   //
    glEnable(GL_TEXTURE_2D);     //  Enable 2eme texture stage

    tmp = MultiMapPipeLine;
    while (tmp) {
        tmp->Affiche();
        tmp = tmp->PipeSuiv;
    }


    //--------------------------- Affiche les polys Transparents

    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);     // n'ecrit pas dans le Zbuff

    PS = listeTr;

    if (nbTrPoly) {
        if (SortPolyTr) Sort(0, nbTrPoly-1);      // Trie les polys par les Z

        PS = listeTr;
        MPpreced = (MatPipeline*)PS->p->m->GetPipe();
        MPpreced->SetRenderState();
        glBegin(GL_TRIANGLES);
        for (a=0; a<nbTrPoly; a++,PS++) {
            MPactua = (MatPipeline*)PS->p->m->GetPipe();

            if (!MPactua->IsCompatible(MPpreced)) {       // si render states differents alors on va les changer
                MPactua->SetRenderState(MPpreced);
                MPpreced = MPactua;
            }

            // TODO : Gérer le multitexturing !!!!!!!!!!!!!!
            if (MPactua->Is1Texture())
                RasterizTexture(PS->p);
            else
                RasterizNoTexture(PS->p);
        }

        glEnd();
    }


    //---------------------------



    //-------------------------- Affiche les polys Speculars

    if (Speculars) {
        glActiveTextureARB(GL_TEXTURE0_ARB);   // Disable texturing !
        glDisable(GL_TEXTURE_2D);     // "                 "
        glActiveTextureARB(GL_TEXTURE1_ARB);   // "                 "
        glDisable(GL_TEXTURE_2D);     // "                 "
        glEnable(GL_BLEND);     // Enable Blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);     // toujours en additif

        glBegin(GL_TRIANGLES);

        Ptmp = Speculars;
        while (Ptmp) {
            RasterizNoTexture(Ptmp);
            Ptmp = Ptmp->suiv;
        }

        glEnd();
    }

    //--------------------------



    //------------------------- Affiche les polys lignes

    glActiveTextureARB(GL_TEXTURE0_ARB);   // Disable texturing !
    glDisable(GL_TEXTURE_2D);     // "                 "
    glActiveTextureARB(GL_TEXTURE1_ARB);   // "                 "
    glDisable(GL_TEXTURE_2D);     // "                 "

    if (PLinesRien) {
        glDisable(GL_BLEND);
        glBegin(GL_LINES);

        Ptmp = PLinesRien;
        while (Ptmp) {
            Rasteriz3DLines(Ptmp);
            Ptmp = Ptmp->suiv;
        }

        glEnd();
    }
    if (PLinesAdditif) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glBegin(GL_LINES);

        Ptmp = PLinesAdditif;
        while (Ptmp) {
            Rasteriz3DLines(Ptmp);
            Ptmp = Ptmp->suiv;
        }

        glEnd();
    }
    if (PLinesMultipli) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
        glBegin(GL_LINES);

        Ptmp = PLinesMultipli;
        while (Ptmp) {
            Rasteriz3DLines(Ptmp);
            Ptmp = Ptmp->suiv;
        }

        glEnd();
    }

    //-------------------------


    glDepthMask(GL_TRUE);     // permet ecriture dans le Zbuf
}
//----------------------------------------------------------------------------------------------------------------------------------------
// détruits tout les polygones temporaires si nécessaire
void PolyPipeLine::Release() {
    PolygonU3D*  tmp;
    U32         a;

    //--------------------------- Release tout les pipeline de material
    while (NoMapPipeLine) {
        NoMapPipeLine->Release();
        NoMapPipeLine = NoMapPipeLine->PipeSuiv;
    }
    while (UneMapPipeLine) {
        UneMapPipeLine->Release();
        UneMapPipeLine = UneMapPipeLine->PipeSuiv;
    }
    while (UneMapOpaPipeLine) {
        UneMapOpaPipeLine->Release();
        UneMapOpaPipeLine = UneMapOpaPipeLine->PipeSuiv;
    }
    while (MultiMapPipeLine) {
        MultiMapPipeLine->Release();
        MultiMapPipeLine = MultiMapPipeLine->PipeSuiv;
    }


    //------------------------- Libere tout les polys transparents
    for (a=0; a<nbTrPoly; a++) {
        if (listeTr[a].p->flag&PolyFLAG_TEMP)
            U3D3Manager->FreePoly(listeTr[a].p);
    }


    //------------------------- Libere tout les polys speculars
    while (Speculars) {
        tmp = Speculars;
        Speculars = Speculars->suiv;
        if (tmp->flag&PolyFLAG_TEMP) {
            U3D3Manager->FreePoly(tmp);

#if defined(_DEBUG) || defined(TOOLU3D)
            NBspeculars--;
#endif
        }
    }

    //------------------------- Libere tout les polys avec rendu line
    while (PLinesRien) {
        tmp = PLinesRien;
        PLinesRien  = PLinesRien->suiv;
        if (tmp->flag&PolyFLAG_TEMP)
            U3D3Manager->FreePoly(tmp);
    }
    while (PLinesAdditif) {
        tmp = PLinesAdditif;
        PLinesAdditif  = PLinesAdditif->suiv;
        if (tmp->flag&PolyFLAG_TEMP)
            U3D3Manager->FreePoly(tmp);
    }
    while (PLinesMultipli) {
        tmp = PLinesMultipli;
        PLinesMultipli = PLinesMultipli->suiv;
        if (tmp->flag&PolyFLAG_TEMP)
            U3D3Manager->FreePoly(tmp);
    }

    nbTrPoly = 0;
}
//----------------------------------------------------------------------------------------------------------
// affiche directement le poly static du pipeline
// TODO : a optimiser dasn certains ca de figuures
void PolyPipeLine::AfficheQuad(Ufloat XYZ[4][3], Ufloat RVBA[4][4], Ufloat UV[4][4]) {
    U32             a;
    CoordU3D*       c = CStatic;
    VertexU3D*      v = VStatic;
    PolygonU3D*     paff;
    MatPipeline*    matp;


    for (a=0; a<4; a++,c++,v++) {
        v->U = UV[a][0];
        v->V = UV[a][1];
        v->U1 = UV[a][2];
        v->V1 = UV[a][3];

        vec4_eg(c->RVBA, RVBA[a]);

        c->ecran[0] = XYZ[a][0] * (ActuaViewport.GetDx()) + ActuaViewport.GetMinX();
        c->ecran[1] = (1.f - XYZ[a][1]) * (ActuaViewport.GetDy()) + ActuaViewport.GetMinY();     // inverse axe des Y pour etre compatible avec pensée du coder :)
        c->ecran[2] = XYZ[a][2];

        c->ClipInfo = ClipNo;
        if (c->ecran[0] > ActuaViewport.GetMaxX()) c->ClipInfo |= ClipDroit;
        else if (c->ecran[0] < ActuaViewport.GetMinX()) c->ClipInfo |= ClipGauche;
        if (c->ecran[1] > ActuaViewport.GetMaxY()) c->ClipInfo |= ClipBas;
        else if (c->ecran[1] < ActuaViewport.GetMinY()) c->ClipInfo |= ClipHaut;
    }


    PStatic->m = MStatic;
    paff = PStatic;

    if (paff->BesoinClip()) {
        paff = PStatic->GetClippedPolyByViewport(ActuaViewport);
        if (!paff) return;    // rien a faire car le poly est hors viewport
    }

    matp = (MatPipeline*)MStatic->GetPipe();

    //------------------ affiche
    if (matp->Is2Texture())
        RasterizMultiTexture(paff);
    else if (matp->Is1Texture())
        RasterizTexture(paff);
    else {
        for (a=0; a<paff->nbVertex; a++)
            paff->Vtab[a]->c->ecran[3] = 1.f /paff->Vtab[a]->c->ecran[2];
        RasterizNoTexture(paff);
    }

    //------------------ libere le poly temporaire si besoin
    if (paff!=PStatic) U3D3Manager->FreePoly(paff);

}
//----------------------------------------------------------------------------------------------------------------------------------------
// affiche directement une ligne ( coord depuis engine 3D )
void PolyPipeLine::AfficheLigneFromEngine(CoordU3D* pt1, CoordU3D* pt2, Ufloat RVBA[2][4]) {
    Camera* cam = U3D3Monde3D->GetActualCamera();
    U32     frameNum = U3D3Monde3D->GetTick();

    cam->Projet(pt1, frameNum);
    cam->Projet(pt2, frameNum);

    if (pt1->ClipInfo || pt2->ClipInfo) {
        CoordU3D    tmp1,tmp2;

        vec4_eg(tmp1.RVBA, RVBA[0]);
        vec4_eg(tmp1.ecran, pt1->ecran);
        vec3_eg(tmp1.trans, pt1->trans);
        tmp1.ClipInfo = pt1->ClipInfo;

        vec4_eg(tmp2.RVBA, RVBA[1]);
        vec4_eg(tmp2.ecran, pt2->ecran);
        vec3_eg(tmp2.trans, pt2->trans);
        tmp2.ClipInfo = pt2->ClipInfo;

        if (!GetClippedLineByCamera(tmp1, tmp2)) return;      // on return kan ligne hors ecran

        glColor4fv(tmp1.RVBA);
        glVertex3f(tmp1.ecran[0], tmp1.ecran[1], -tmp1.ecran[3] /*- 2.f/65536.f*/);

        glColor4fv(tmp2.RVBA);
        glVertex3f(tmp2.ecran[0], tmp2.ecran[1], -tmp2.ecran[3] /*- 2.f/65536.f*/);
    } else {
        glColor4fv(RVBA[0]);
        glVertex3f(pt1->ecran[0], pt1->ecran[1], -pt1->ecran[3] /*- 2.f/65536.f*/);

        glColor4fv(RVBA[1]);
        glVertex3f(pt2->ecran[0], pt2->ecran[1], -pt2->ecran[3] /*- 2.f/65536.f*/);
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
// affiche une ligne en 2D ( ne projette pas les coordonnées
void PolyPipeLine::AfficheLigne(Ufloat pt1[3], Ufloat pt2[3], Ufloat RVBA[2][4]) {
    CoordU3D coord1,coord2;

    coord1.ecran[0] = pt1[0] * (ActuaViewport.GetDx()) + ActuaViewport.GetMinX();
    coord1.ecran[1] = (1.f - pt1[1]) * (ActuaViewport.GetDy()) + ActuaViewport.GetMinY();     // inverse axe des Y pour etre compatible avec pensée du coder :)
    coord1.ecran[3] = 3.f/65536.f;
    if (coord1.ecran[0] > ActuaViewport.GetMaxX()) coord1.ClipInfo |= ClipDroit;
    else if (coord1.ecran[0] < ActuaViewport.GetMinX()) coord1.ClipInfo |= ClipGauche;
    if (coord1.ecran[1] > ActuaViewport.GetMaxY()) coord1.ClipInfo |= ClipBas;
    else if (coord1.ecran[1] < ActuaViewport.GetMinY()) coord1.ClipInfo |= ClipHaut;

    coord2.ecran[0] = pt2[0] * (ActuaViewport.GetDx()) + ActuaViewport.GetMinX();
    coord2.ecran[1] = (1.f - pt2[1]) * (ActuaViewport.GetDy()) + ActuaViewport.GetMinY();     // inverse axe des Y pour etre compatible avec pensée du coder :)
    coord2.ecran[3] = 3.f/65536.f;
    if (coord2.ecran[0] > ActuaViewport.GetMaxX()) coord2.ClipInfo |= ClipDroit;
    else if (coord2.ecran[0] < ActuaViewport.GetMinX()) coord2.ClipInfo |= ClipGauche;
    if (coord2.ecran[1] > ActuaViewport.GetMaxY()) coord2.ClipInfo |= ClipBas;
    else if (coord2.ecran[1] < ActuaViewport.GetMinY()) coord2.ClipInfo |= ClipHaut;

    if (coord1.ClipInfo || coord2.ClipInfo) {
        vec4_eg(coord1.RVBA, RVBA[0]);
        vec4_eg(coord2.RVBA, RVBA[1]);

        if (!GetClippedLineByViewport(coord1, coord2, ActuaViewport)) return;      // on return kan ligne hors ecran

        glColor4fv(coord1.RVBA);
        glVertex3f(coord1.ecran[0], coord1.ecran[1], -coord1.ecran[3] /*- 2.f/65536.f*/);

        glColor4fv(coord2.RVBA);
        glVertex3f(coord2.ecran[0], coord2.ecran[1], -coord2.ecran[3] /*- 2.f/65536.f*/);
    } else {
        glColor4fv(RVBA[0]);
        glVertex3f(coord1.ecran[0], coord1.ecran[1], -coord1.ecran[3] /*- 2.f/65536.f*/);

        glColor4fv(RVBA[1]);
        glVertex3f(coord2.ecran[0], coord2.ecran[1], -coord2.ecran[3] /*- 2.f/65536.f*/);
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
