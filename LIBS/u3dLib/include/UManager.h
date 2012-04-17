/*-----------------------------------------------------+
 |                       Umanager.h                     |
 |                                                      |
 |      c'est un manager qui va géré l'allocation des   |
 |              polygones, des vertex, etc...           |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#ifndef _Umanager_U3D3_H
#define _Umanager_U3D3_H


struct PipelineState;   // from Fpipe.h

#define MAX_VERTEX_PAR_POLY 17

#ifdef _DEBUG
extern U32 DBnbPolyAlloue;
extern U32 DBnbVertexAlloue;
extern U32 DBnbCoordAlloue;
#endif

//==================
// defintion du manager

class UManager {

    CoordU3D*       Cliste;
    VertexU3D*      Vliste;
    PolygonU3D*     Pliste[MAX_VERTEX_PAR_POLY];
    PipelineState*  Pipeliste;

public:

    UManager();
    ~UManager();

    PipelineState*  NewPipeState();
    void            FreePipeState(PipelineState* c);

    //===================================
    CoordU3D* NewCoord() {
        CoordU3D* ret;

        if (!Cliste) {
            ret = new CoordU3D;
#ifdef _DEBUG
            TestPtr(ret);
            DBnbCoordAlloue++;
#endif
        } else {
            ret = Cliste;
            Cliste = Cliste->suiv;
#ifdef _DEBUG
            DBnbCoordAlloue++;
#endif
        }
        ret->frame_process = (U32)-1;
        ret->utilisateurs = 0;
        ret->ClipInfo = ClipNo;
        return ret;
    }

    //===================================
    void FreeCoord(CoordU3D* c) {
        c->utilisateurs--;
        if (c->utilisateurs <= 0) {
            c->suiv = Cliste;
            Cliste = c;
#ifdef _DEBUG
            DBnbCoordAlloue--;
#endif
        }
    }

    //===================================
    VertexU3D* NewVertex() {
        VertexU3D* ret;

        if (!Vliste) {
            ret = new VertexU3D;
#ifdef _DEBUG
            TestPtr(ret);
            DBnbVertexAlloue++;
#endif
        } else {
            ret = Vliste;
            Vliste = Vliste->suiv;
#ifdef _DEBUG
            DBnbVertexAlloue++;
#endif
        }
        ret->utilisateurs = 0;
        return ret;
    }

    //===================================
    void FreeVertex(VertexU3D* v) {
        FreeCoord(v->c);
        v->utilisateurs--;
        if (v->utilisateurs <= 0) {
            v->suiv = Vliste;
            Vliste = v;
#ifdef _DEBUG
            DBnbVertexAlloue--;
#endif
        }
    }

    //===================================
    PolygonU3D* NewPoly(U32 nbp) {
        PolygonU3D* ret;

#ifdef _DEBUG
        if ((nbp>=MAX_VERTEX_PAR_POLY+3) || (nbp<3))
            FatalError("Erreur dans allocation de polygones!");
#endif

        nbp-=3;
        if (!Pliste[nbp]) {
            ret = new PolygonU3D;
            ret->Vtab = new VertexU3D*[nbp+3];
#ifdef _DEBUG
            TestPtr(ret);
            TestPtr(ret->Vtab);
            DBnbPolyAlloue++;
#endif
        } else {
            ret = Pliste[nbp];
            Pliste[nbp] = Pliste[nbp]->suiv;
#ifdef _DEBUG
            DBnbPolyAlloue++;
#endif
        }
        ret->nbVertex = nbp + 3;
        return ret;
    }

    //===================================
    void FreePoly(PolygonU3D* p) {
#ifdef _DEBUG
        if ((p->nbVertex>=MAX_VERTEX_PAR_POLY+3) || (p->nbVertex<3))
            FatalError("Erreur de desallocation de polygones!");
#endif

        // désalloue tout les vertex d'abord
        for (U32 a=0; a<p->nbVertex; a++)
            FreeVertex(p->Vtab[a]);

        U32 nbp=p->nbVertex-3;
        p->suiv = Pliste[nbp];
        Pliste[nbp] = p;
#ifdef _DEBUG
        DBnbPolyAlloue--;
#endif
    }

};

//==================

#endif
