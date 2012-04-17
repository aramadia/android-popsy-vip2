/*-----------------------------------------------------+
 |                       Rasteriz.cpp                   |
 |                                                      |
 |  c'est ici que l'on va prendre chaque poly analyser  |
 |  leur propriété de material et l'afficher a l'ecran  |
 |              en fonction de cela!                    |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#include "U3D3.h"

//----------------------------------------------------------------------------------------------------------
//                                  +---------------------+
//                                  |    Les Fonctions    |
//                                  +---------------------+
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// draw 1 poly sans texture
void RasterizNoTexture(PolygonU3D* paff) {
    MaterialU3D* mat = paff->m;
    CoordU3D*    c1,*c2,*c3;
    VertexU3D**   V;
    U32         a;

    //------------------------------- petites inies de vertex
    V = paff->Vtab;
    c1 = V[0]->c;
    c3 = V[1]->c;
    V += 2;

    switch (mat->GetShading()) {
    case MatShadingGouraud:
        for (a=0; a<paff->nbVertex-2; a++,V++) {
            c2 = c3;
            c3 = V[0]->c;

            glColor4f(c1->RVBA[0]*mat->GetRouge(), c1->RVBA[1]*mat->GetVert(), c1->RVBA[2]*mat->GetBleu(), c1->RVBA[3]*mat->GetAlpha());
            glVertex3f(c1->ecran[0], c1->ecran[1], -c1->ecran[3]);

            glColor4f(c2->RVBA[0]*mat->GetRouge(), c2->RVBA[1]*mat->GetVert(), c2->RVBA[2]*mat->GetBleu(), c2->RVBA[3]*mat->GetAlpha());
            glVertex3f(c2->ecran[0], c2->ecran[1], -c2->ecran[3]);

            glColor4f(c3->RVBA[0]*mat->GetRouge(), c3->RVBA[1]*mat->GetVert(), c3->RVBA[2]*mat->GetBleu(), c3->RVBA[3]*mat->GetAlpha());
            glVertex3f(c3->ecran[0], c3->ecran[1], -c3->ecran[3]);
        }
        break;

    case MatShadingFlat:
        glColor4f(paff->RVBA[0]*mat->GetRouge(), paff->RVBA[1]*mat->GetVert(), paff->RVBA[2]*mat->GetBleu(), paff->RVBA[3]*mat->GetAlpha());
        for (a=0; a<paff->nbVertex-2; a++,V++) {
            c2 = c3;
            c3 = V[0]->c;

            glVertex3f(c1->ecran[0], c1->ecran[1], -c1->ecran[3]);
            glVertex3f(c2->ecran[0], c2->ecran[1], -c2->ecran[3]);
            glVertex3f(c3->ecran[0], c3->ecran[1], -c3->ecran[3]);
        }
        break;

    default:
        glColor4f(mat->GetRouge(), mat->GetVert(), mat->GetBleu(), mat->GetAlpha());
        for (a=0; a<paff->nbVertex-2; a++,V++) {
            c2 = c3;
            c3 = V[0]->c;

            glVertex3f(c1->ecran[0], c1->ecran[1], -c1->ecran[3]);
            glVertex3f(c2->ecran[0], c2->ecran[1], -c2->ecran[3]);
            glVertex3f(c3->ecran[0], c3->ecran[1], -c3->ecran[3]);
        }
        break;
    }

}
//----------------------------------------------------------------------------------------------------------
// draw 1 poly avec texture
void RasterizTexture(PolygonU3D* paff) {
    MaterialU3D*     mat = paff->m;
    VertexU3D**       V;
    Raster2DData    tmpTab[20],*ptrTab;
    U32             a;
    Ufloat          xTexStart,DxTex;
    Ufloat          yTexStart,DyTex;

    V = paff->Vtab;
    mat->GetTex1Pos(xTexStart, yTexStart, DxTex, DyTex);     // prend les coords de la map dans la texture

    switch (mat->GetShading()) {

    case MatShadingGouraud:
        ptrTab = tmpTab;
        for (a=0; a<paff->nbVertex; a++,V++,ptrTab++) {
            ptrTab->RVBA[0] = V[0]->c->RVBA[0]*mat->GetRouge();
            ptrTab->RVBA[1] = V[0]->c->RVBA[1]*mat->GetVert();
            ptrTab->RVBA[2] = V[0]->c->RVBA[2]*mat->GetBleu();
            ptrTab->RVBA[3] = V[0]->c->RVBA[3]*mat->GetAlpha();

            ptrTab->UV[0] = xTexStart + DxTex*V[0]->U;
            ptrTab->UV[1] = yTexStart + DyTex*V[0]->V;

            Ufloat z = V[0]->c->ecran[2];
            ptrTab->XYZW[0] = V[0]->c->ecran[0]*z;
            ptrTab->XYZW[1] = V[0]->c->ecran[1]*z;
            ptrTab->XYZW[2] = -1.0f;
            ptrTab->XYZW[3] = z;
        }
        ptrTab = tmpTab+1;
        for (a=0; a<paff->nbVertex-2; a++,ptrTab++) {
            glColor4fv((const float*)&tmpTab[0].RVBA);
            glTexCoord2fv((const float*)&tmpTab[0].UV);
            glVertex4fv((const float*)&tmpTab[0].XYZW);

            glColor4fv((const float*)&ptrTab->RVBA);
            glTexCoord2fv((const float*)&ptrTab->UV);
            glVertex4fv((const float*)&ptrTab->XYZW);

            glColor4fv((const float*)&(ptrTab+1)->RVBA);
            glTexCoord2fv((const float*)&(ptrTab+1)->UV);
            glVertex4fv((const float*)&(ptrTab+1)->XYZW);
        }
        break;


    case MatShadingFlat:
        ptrTab = tmpTab;
        for (a=0; a<paff->nbVertex; a++,V++,ptrTab++) {
            ptrTab->UV[0] = xTexStart + DxTex*V[0]->U;
            ptrTab->UV[1] = yTexStart + DyTex*V[0]->V;

            Ufloat z = V[0]->c->ecran[2];
            ptrTab->XYZW[0] = V[0]->c->ecran[0]*z;
            ptrTab->XYZW[1] = V[0]->c->ecran[1]*z;
            ptrTab->XYZW[2] = -1.0f;
            ptrTab->XYZW[3] = z;
        }
        glColor4f(paff->RVBA[0]*mat->GetRouge(), paff->RVBA[1]*mat->GetVert(), paff->RVBA[2]*mat->GetBleu(), paff->RVBA[3]*mat->GetAlpha());
        ptrTab = tmpTab+1;
        for (a=0; a<paff->nbVertex-2; a++,ptrTab++) {
            glTexCoord2fv((const float*)&tmpTab[0].UV);
            glVertex4fv((const float*)&tmpTab[0].XYZW);

            glTexCoord2fv((const float*)&ptrTab->UV);
            glVertex4fv((const float*)&ptrTab->XYZW);

            glTexCoord2fv((const float*)&(ptrTab+1)->UV);
            glVertex4fv((const float*)&(ptrTab+1)->XYZW);
        }
        break;


    default:
        ptrTab = tmpTab;
        for (a=0; a<paff->nbVertex; a++,V++,ptrTab++) {
            ptrTab->UV[0] = xTexStart + DxTex*V[0]->U;
            ptrTab->UV[1] = yTexStart + DyTex*V[0]->V;

            Ufloat z = V[0]->c->ecran[2];
            ptrTab->XYZW[0] = V[0]->c->ecran[0]*z;
            ptrTab->XYZW[1] = V[0]->c->ecran[1]*z;
            ptrTab->XYZW[2] = -1.0f;
            ptrTab->XYZW[3] = z;
        }
        glColor4f(mat->GetRouge(), mat->GetVert(), mat->GetBleu(), mat->GetAlpha());
        ptrTab = tmpTab+1;
        for (a=0; a<paff->nbVertex-2; a++,ptrTab++) {
            glTexCoord2fv((const float*)&tmpTab[0].UV);
            glVertex4fv((const float*)&tmpTab[0].XYZW);

            glTexCoord2fv((const float*)&ptrTab->UV);
            glVertex4fv((const float*)&ptrTab->XYZW);

            glTexCoord2fv((const float*)&(ptrTab+1)->UV);
            glVertex4fv((const float*)&(ptrTab+1)->XYZW);
        }
        break;

    }

}
//----------------------------------------------------------------------------------------------------------
// draw 1 poly avec texture
void RasterizMultiTexture(PolygonU3D* paff) {
    MaterialU3D*     mat = paff->m;
    VertexU3D**       V;
    Raster2DData    tmpTab[20],*ptrTab;
    U32             a;
    Ufloat          xTexStart,DxTex;
    Ufloat          yTexStart,DyTex;
    Ufloat          xTexStart1,DxTex1;
    Ufloat          yTexStart1,DyTex1;


    V = paff->Vtab;
    mat->GetTex1Pos(xTexStart, yTexStart, DxTex, DyTex);     // prend les coords de la map dans la texture
    mat->GetTex2Pos(xTexStart1, yTexStart1, DxTex1, DyTex1);     // prend les coords de la map dans la texture ( mais pour la 2eme texture )

    switch (mat->GetShading()) {

    case MatShadingGouraud:
        ptrTab = tmpTab;
        for (a=0; a<paff->nbVertex; a++,V++,ptrTab++) {
            ptrTab->RVBA[0] = V[0]->c->RVBA[0]*mat->GetRouge();
            ptrTab->RVBA[1] = V[0]->c->RVBA[1]*mat->GetVert();
            ptrTab->RVBA[2] = V[0]->c->RVBA[2]*mat->GetBleu();
            ptrTab->RVBA[3] = V[0]->c->RVBA[3]*mat->GetAlpha();

            ptrTab->UV[0] = xTexStart + DxTex*V[0]->U;
            ptrTab->UV[1] = yTexStart + DyTex*V[0]->V;

            ptrTab->UV1[0] = xTexStart1 + DxTex1*V[0]->U1;
            ptrTab->UV1[1] = yTexStart1 + DyTex1*V[0]->V1;

            Ufloat z = V[0]->c->ecran[2];
            ptrTab->XYZW[0] = V[0]->c->ecran[0]*z;
            ptrTab->XYZW[1] = V[0]->c->ecran[1]*z;
            ptrTab->XYZW[2] = -1.0f;
            ptrTab->XYZW[3] = z;
        }
        ptrTab = tmpTab+1;
        for (a=0; a<paff->nbVertex-2; a++,ptrTab++) {
            glColor4fv((const float*)&tmpTab[0].RVBA);
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, tmpTab[0].UV[0], tmpTab[0].UV[1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, tmpTab[0].UV1[0], tmpTab[0].UV1[1]);
            glVertex4fv((const float*)&tmpTab[0].XYZW);

            glColor4fv((const float*)&ptrTab->RVBA);
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, ptrTab->UV[0], ptrTab->UV[1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, ptrTab->UV1[0], ptrTab->UV1[1]);
            glVertex4fv((const float*)&ptrTab->XYZW);

            glColor4fv((const float*)&(ptrTab+1)->RVBA);
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (ptrTab+1)->UV[0], (ptrTab+1)->UV[1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (ptrTab+1)->UV1[0], (ptrTab+1)->UV1[1]);
            glVertex4fv((const float*)&(ptrTab+1)->XYZW);
        }
        break;


    case MatShadingFlat:
        ptrTab = tmpTab;
        for (a=0; a<paff->nbVertex; a++,V++,ptrTab++) {
            ptrTab->UV[0] = xTexStart + DxTex*V[0]->U;
            ptrTab->UV[1] = yTexStart + DyTex*V[0]->V;

            ptrTab->UV1[0] = xTexStart1 + DxTex1*V[0]->U1;
            ptrTab->UV1[1] = yTexStart1 + DyTex1*V[0]->V1;

            Ufloat z = V[0]->c->ecran[2];
            ptrTab->XYZW[0] = V[0]->c->ecran[0]*z;
            ptrTab->XYZW[1] = V[0]->c->ecran[1]*z;
            ptrTab->XYZW[2] = -1.0f;
            ptrTab->XYZW[3] = z;
        }
        glColor4f(paff->RVBA[0]*mat->GetRouge(), paff->RVBA[1]*mat->GetVert(), paff->RVBA[2]*mat->GetBleu(), paff->RVBA[3]*mat->GetAlpha());
        ptrTab = tmpTab+1;
        for (a=0; a<paff->nbVertex-2; a++,ptrTab++) {
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, tmpTab[0].UV[0], tmpTab[0].UV[1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, tmpTab[0].UV1[0], tmpTab[0].UV1[1]);
            glVertex4fv((const float*)&tmpTab[0].XYZW);

            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, ptrTab->UV[0], ptrTab->UV[1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, ptrTab->UV1[0], ptrTab->UV1[1]);
            glVertex4fv((const float*)&ptrTab->XYZW);

            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (ptrTab+1)->UV[0], (ptrTab+1)->UV[1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (ptrTab+1)->UV1[0], (ptrTab+1)->UV1[1]);
            glVertex4fv((const float*)&(ptrTab+1)->XYZW);
        }
        break;


    default:
        ptrTab = tmpTab;
        for (a=0; a<paff->nbVertex; a++,V++,ptrTab++) {
            ptrTab->UV[0] = xTexStart + DxTex*V[0]->U;
            ptrTab->UV[1] = yTexStart + DyTex*V[0]->V;

            ptrTab->UV1[0] = xTexStart1 + DxTex1*V[0]->U1;
            ptrTab->UV1[1] = yTexStart1 + DyTex1*V[0]->V1;

            Ufloat z = V[0]->c->ecran[2];
            ptrTab->XYZW[0] = V[0]->c->ecran[0]*z;
            ptrTab->XYZW[1] = V[0]->c->ecran[1]*z;
            ptrTab->XYZW[2] = -1.0f;
            ptrTab->XYZW[3] = z;
        }
        glColor4f(mat->GetRouge(), mat->GetVert(), mat->GetBleu(), mat->GetAlpha());
        ptrTab = tmpTab+1;
        for (a=0; a<paff->nbVertex-2; a++,ptrTab++) {
            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, tmpTab[0].UV[0], tmpTab[0].UV[1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, tmpTab[0].UV1[0], tmpTab[0].UV1[1]);
            glVertex4fv((const float*)&tmpTab[0].XYZW);

            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, ptrTab->UV[0], ptrTab->UV[1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, ptrTab->UV1[0], ptrTab->UV1[1]);
            glVertex4fv((const float*)&ptrTab->XYZW);

            glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (ptrTab+1)->UV[0], (ptrTab+1)->UV[1]);
            glMultiTexCoord2fARB(GL_TEXTURE1_ARB, (ptrTab+1)->UV1[0], (ptrTab+1)->UV1[1]);
            glVertex4fv((const float*)&(ptrTab+1)->XYZW);
        }
        break;
    }

}
//----------------------------------------------------------------------------------------------------------
// draw les lignes d'un poly
void Rasteriz3DLines(PolygonU3D* paff) {
    MaterialU3D* mat = paff->m;
    CoordU3D*    c1,*c2;
    VertexU3D**   V;
    U32         a;

    V = paff->Vtab;

    switch (mat->GetShading()) {
    case MatShadingGouraud:
        for (a=0; a<paff->nbVertex; a++,V++) {
            U32 n = a+1;
            if (n==paff->nbVertex) n=0;
            if ((paff->arrete>>a)&0x1) {
                c1 = paff->Vtab[a]->c;
                c2 = paff->Vtab[n]->c;

                glColor4f(c1->RVBA[0]*mat->GetRouge(), c1->RVBA[1]*mat->GetVert(), c1->RVBA[2]*mat->GetBleu(), c1->RVBA[3]*mat->GetAlpha());
                glVertex3f(c1->ecran[0], c1->ecran[1], -c1->ecran[3]-2.f/65536.f);

                glColor4f(c2->RVBA[0]*mat->GetRouge(), c2->RVBA[1]*mat->GetVert(), c2->RVBA[2]*mat->GetBleu(), c2->RVBA[3]*mat->GetAlpha());
                glVertex3f(c2->ecran[0], c2->ecran[1], -c2->ecran[3]-2.f/65536.f);
            }

        }
        break;

    case MatShadingFlat:
        glColor4f(paff->RVBA[0]*mat->GetRouge(), paff->RVBA[1]*mat->GetVert(), paff->RVBA[2]*mat->GetBleu(), paff->RVBA[3]*mat->GetAlpha());
        for (a=0; a<paff->nbVertex; a++,V++) {
            U32 n = a+1;
            if (n==paff->nbVertex) n=0;
            if ((paff->arrete>>a)&0x1) {
                c1 = paff->Vtab[a]->c;
                c2 = paff->Vtab[n]->c;

                glVertex3f(c1->ecran[0], c1->ecran[1], -c1->ecran[3]-2.f/65536.f);
                glVertex3f(c2->ecran[0], c2->ecran[1], -c2->ecran[3]-2.f/65536.f);
            }
        }
        break;

    default:
        glColor4f(mat->GetRouge(), mat->GetVert(), mat->GetBleu(), mat->GetAlpha());
        for (a=0; a<paff->nbVertex; a++,V++) {
            U32 n = a+1;
            if (n==paff->nbVertex) n=0;
            if ((paff->arrete>>a)&0x1) {
                c1 = paff->Vtab[a]->c;
                c2 = paff->Vtab[n]->c;

                glVertex3f(c1->ecran[0], c1->ecran[1], -c1->ecran[3]-2.f/65536.f);
                glVertex3f(c2->ecran[0], c2->ecran[1], -c2->ecran[3]-2.f/65536.f);
            }
        }
        break;
    }
}
//----------------------------------------------------------------------------------------------------------
