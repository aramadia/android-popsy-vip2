/*-----------------------------------------------------+
 |                      ObjetConv.cpp                   |
 |                                                      |
 |    cette classe me permet de traiter + facilement    |
 |      les données a convertir entre des datas 3D      |
 |       quelconques et le formet spécifik U3D          |
 |                                                      |
 |  U2^PoPsy TeAm 1999                              |
 +-----------------------------------------------------*/

#include "U3D3.h"

#ifdef TOOLU3D
//----------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                  +-------------------------+
//                                  |    Class ObjetU3DConv   |
//                                  +-------------------------+
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------
ObjetU3DConv::ObjetU3DConv() {
    nom = "";
    csParent = "";
    vec3_set(Pivot, 0.0f, 0.0f, 0.0f);
    BaseObject = TRUE;

    nbcoord = 0;
    Ctab    = NULL;
    CtabFin = NULL;

    nbvertices  = 0;
    Vtab        = NULL;
    VtabFin     = NULL;

    nbfaces = 0;
    Ftab    = NULL;

    nbmats  = 0;
    mat     = NULL;

    npkeys = 0;
    posK = NULL;

    nrkeys = 0;
    rotK = NULL;

    nmkeys = 0;
    morphK = NULL;

    nhkeys = 0;
    hideK = NULL;

    suiv = NULL;
    Childsuiv = NULL;
    Childs = NULL;

}
//----------------------------------------------------------------------------------------------------------
ObjetU3DConv::~ObjetU3DConv() {
    Coord3DConv*     Ctmp;
    VertexU3DConv*   Vtmp;
    PolygonU3DConv*  Ftmp;
    MaterialU3DConv* Mtmp;

    //----------------- Detruit toutes les coords
    while (Ctab) {
        Ctmp = Ctab->suiv;
        delete Ctab;
        Ctab = Ctmp;
    }

    //---------------- Detruit tout les vertex
    while (Vtab) {
        Vtmp = Vtab->suiv;
        delete Vtab;
        Vtab = Vtmp;
    }

    //--------------- Detruit tout les polys
    while (Ftab) {
        Ftmp = Ftab->suiv;
        delete Ftab;
        Ftab = Ftmp;
    }

    //-------------- Detruit les materials
    while (mat) {
        Mtmp = mat->suiv;
        delete mat;
        mat = Mtmp;
    }

    //------------- Detruit les keys
    if (posK)       delete [] posK;
    if (rotK)       delete [] rotK;
    if (morphK) delete [] morphK;
    if (hideK)      delete [] hideK;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::Conv3DS(mesh3ds* m) {
    U32             a;
    Coord3DConv*     Ctmp;
    VertexU3DConv*   Vtmp;
    PolygonU3DConv*  Ftmp;
    material3ds*     Mat3ds = NULL;

    nom =  m->name;

    GetLocaMatrixFrom3DS(m->locmatrix);

    //------------------- converti les coordonnées 3D
    for (a=0; a<m->nvertices; a++)   AddCoord(&m->vertexarray[a]);


    //------------------- converti les coordonnées de texture
    if (m->ntextverts != m->nvertices)
        for (a=0; a<m->nvertices; a++)
            AddVertex((textvert3ds*)NULL);     // si pas de coord texture alors on en rajoute kan meme
    else
        for (a=0; a<m->nvertices; a++)
            AddVertex(&m->textarray[a]);


    //------------------- attache les coordonnées aux textures
    Ctmp = Ctab;
    Vtmp = Vtab;
    for (a=0; a<m->nvertices; a++) {
        Vtmp->c = Ctmp;
        Ctmp = Ctmp->suiv;
        Vtmp = Vtmp->suiv;
    }


    //------------------- converti les polys
    for (a=0; a<m->nfaces; a++)  AddFace(&m->facearray[a]);


    //------------------- converti material
    for (a=0; a<m->nmats; a++) {
        AddMat(m->matarray[a].name);

        //------------------- sauve le tiling sil y en a
        if (m->usemapinfo) {
            mat->tilex = m->map.tilex;
            mat->tiley = m->map.tiley;
        } else {
            mat->tilex = 1.0f;
            mat->tiley = 1.0f;
        }

        //------------------- assigne material a toute les faces a ki il est assigné
        for (U32 b=0; b<m->matarray[a].nfaces; b++) {
            Ftmp = GetFace(m->matarray[a].faceindex[b]);
            Ftmp->mat = mat;   // mat = adresse du dernier material entre
        }

    }

    //------------------- permet un mapping cylindrique ou spherique en rajoutant des vertex si besoin
    DoWrapping();

}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::Conv3DS(kfmesh3ds* m) {
    U32     a;
    float   pivot[3];
    float   px,py,pz;

    //--------------------------- Gere le Pivot de l'objet
    vec3_set(pivot, m->pivot.x, m->pivot.z, m->pivot.y);
    px = locaMatrix.GetXTransfoR(pivot);
    py = locaMatrix.GetYTransfoR(pivot);
    pz = locaMatrix.GetZTransfoR(pivot);
    locaMatrix.AddToPos(px, py, pz);

    //--------------------------- place l'objet comme il fo dans la hiérarchie !
    ObjetU3DConv* Parent = MondeU3DConv::ActualMonde->Get(m->parent);
    if (Parent) {    // c'est l'enfant dun autre objet
        csParent = m->parent;
        Parent->AddChild(this);     // on attache objet a son pere
        BaseObject = FALSE;
//      MondeU3DConv::ActualMonde->RemoveElement( this );   // on l'enleve donc de la liste général des objets
    }


    //--------------------------- Converti les keys d'animations

    npkeys = m->npkeys;
    nrkeys = m->nrkeys;
    nmkeys = m->nmkeys;
    nhkeys = m->nhkeys;

    if (npkeys) posK = new PosKeyFileU3D[ npkeys ];
    if (nrkeys) rotK = new RotKeyFileU3D[ nrkeys ];
    if (nmkeys) morphK = new MorphKeyFileU3D[ nmkeys ];
    if (nhkeys) hideK = new HideKeyFileU3D[ nhkeys ];

    //--------------------- Positions keys
    for (a=0; a<npkeys; a++) {
        vec3_set(posK[a].p, m->pos[a].x, m->pos[a].z, m->pos[a].y);
        KHeadereg(posK[a].k, m->pkeys[a]);
    }

    //--------------------- Rotations keys
    Mat3x4  mtmp;
    Quat    pquat, pquat1;
    Quat    qtmp;
    for (a=0; a<nrkeys; a++) {
        qtmp.SetFromAxis(-m->rot[a].x, -m->rot[a].z, -m->rot[a].y, m->rot[a].angle);
        if (a==0) {
            pquat = qtmp;
        } else {
            pquat1 = pquat * qtmp;
            if (pquat1.dot(pquat) < 0.0f)
                pquat1.neg();
            pquat = pquat1;   // calcul quaternion absolu et non pas relatif !
        }
        pquat.matrix(mtmp);

        rotK[a].q[0] = pquat[0];
        rotK[a].q[1] = pquat[1];
        rotK[a].q[2] = pquat[2];
        rotK[a].q[3] = pquat[3];

        KHeadereg(rotK[a].k, m->rkeys[a]);
    }

    //--------------------- Morph keys
    for (a=0; a<nmkeys; a++) {
        morphK[a].nom = m->morph[a].name;
        KHeadereg(morphK[a].k, m->mkeys[a]);
    }

    //-------------------- Hide keys
    for (a=0; a<nhkeys; a++)
        hideK[a].pos = m->hkeys[a].time;


    //---------------------------


    //--------------------------- trnasforme les coords dans un systeme local
    WolrdToLoca();

}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::GetLocaMatrixFrom3DS(Ufloat mat3DS[12]) {
    locaMatrix.Set(mat3DS[0], mat3DS[6], mat3DS[3],
                   mat3DS[2], mat3DS[8], mat3DS[5],
                   mat3DS[1], mat3DS[7], mat3DS[4],
                   mat3DS[9], mat3DS[11], mat3DS[10]);
}
//----------------------------------------------------------------------------------------------------------
//                                          Coords
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::AddCoord(Coord3DConv* c) {
    c->suiv     = NULL;
    c->preced   = CtabFin;
    c->num      = nbcoord;
    CtabFin     = c;
    if (c->preced)
        c->preced->suiv = c;
    if (!Ctab)
        Ctab = c;

    nbcoord++;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::AddCoord(point3ds* pt) {
    Coord3DConv* nouvo;

    nouvo = new Coord3DConv;
    TestPtr(nouvo);

    nouvo->coord[0] = pt->x;
    nouvo->coord[2] = pt->y;       // attention 3ds intervertit les
    nouvo->coord[1] = pt->z;       // Y et les Z

    AddCoord(nouvo);
}
//----------------------------------------------------------------------------------------------------------
// transform les coords dans le system local de l'objet
void ObjetU3DConv::WolrdToLoca() {
    Mat3x4      invMat;
    Coord3DConv* tmp;
    Ufloat      Vec[3];

    invMat = locaMatrix.GetInverse();

    tmp = Ctab;
    while (tmp) {
        vec3_eg(Vec, tmp->coord);

        tmp->coord[0] = invMat.GetXTransfo(Vec);
        tmp->coord[1] = invMat.GetYTransfo(Vec);
        tmp->coord[2] = invMat.GetZTransfo(Vec);

        tmp = tmp->suiv;
    }
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::WriteCoords(CArchive& ar) {
    Coord3DConv* tmp = Ctab;

    ar << nbcoord;

    while (tmp) {
        ar << tmp->coord[0];
        ar << tmp->coord[1];
        ar << tmp->coord[2];
        tmp = tmp->suiv;
    }
}
//----------------------------------------------------------------------------------------------------------
//                                          Vertex
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::AddVertex(VertexU3DConv* v) {
    v->suiv     = NULL;
    v->preced   = VtabFin;
    v->num      = nbvertices;
    VtabFin     = v;
    if (v->preced)
        v->preced->suiv = v;
    if (!Vtab)
        Vtab = v;

    nbvertices++;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::AddVertex(textvert3ds* v) {
    VertexU3DConv* nouvo;

    nouvo = new VertexU3DConv;
    TestPtr(nouvo);

    if (v) {
        nouvo->u = 1.0f - v->u;
        nouvo->v = 1.0f - v->v;
    } else {
        nouvo->u = 0.0f;
        nouvo->v = 0.0f;
    }

    AddVertex(nouvo);
}
//----------------------------------------------------------------------------------------------------------
VertexU3DConv* ObjetU3DConv::GetVertex(U16 pos) {
    VertexU3DConv* tmp = Vtab;

    while (tmp) {
        if (tmp->num == pos)
            return tmp;
        tmp = tmp->suiv;
    }

    return NULL;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::WriteVertex(CArchive& ar) {
    VertexU3DConv* tmp = Vtab;

    ar << nbvertices;

    while (tmp) {
        ar << tmp->u;
        ar << tmp->v;
        ar << tmp->c->num;
        tmp = tmp->suiv;
    }
}
//----------------------------------------------------------------------------------------------------------
//                                              Faces
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::AddFace(PolygonU3DConv* f) {
    f->suiv     = Ftab;
    f->num      = nbfaces;
    Ftab        = f;
    nbfaces++;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::AddFace(face3ds* f) {
    PolygonU3DConv* nouvo;

    nouvo = new PolygonU3DConv;
    TestPtr(nouvo);

    nouvo->flag = ((0x1&f->flag)<<2) + (0x2&f->flag) + ((0x4&f->flag)>>2);
    nouvo->wrap = (0x18&f->flag)>>3;
    nouvo->tab[0] = GetVertex(f->v1);
    nouvo->tab[1] = GetVertex(f->v2);
    nouvo->tab[2] = GetVertex(f->v3);
    nouvo->mat = NULL;

    AddFace(nouvo);
}
//----------------------------------------------------------------------------------------------------------
// retourne la face a la position pos dans la liste de face
PolygonU3DConv* ObjetU3DConv::GetFace(U16 pos) {
    PolygonU3DConv* tmp = Ftab;

    while (Ftab) {
        if (pos == tmp->num)
            return tmp;
        tmp = tmp->suiv;
    }

    return NULL;
}
//----------------------------------------------------------------------------------------------------------
// permet d'avoir mapping cylindrik comme il fo avec 3DS
void ObjetU3DConv::DoWrapping() {
    PolygonU3DConv*  tmp = Ftab;
    bool            wrapu = FALSE, wrapv = FALSE;
    VertexU3DConv*   nouvo;
    bool            wrap2 = FALSE, wrap3 = FALSE;

    while (tmp) {
        wrap2 = FALSE, wrap3 = FALSE;

        if (tmp->wrap&0x1) {
            Ufloat delta1 = tmp->tab[1]->u - tmp->tab[0]->u;
            Ufloat delta2 = tmp->tab[2]->u - tmp->tab[0]->u;
            Ufloat tilex;
            if (tmp->mat)
                tilex = tmp->mat->tilex;
            else
                tilex = 1.f;

            if (fabs(delta1) > tilex*.51f) {
                nouvo = new VertexU3DConv;
                nouvo->v = tmp->tab[1]->v;

                if (delta1< 0.0f) delta1 = tilex + delta1;
                else               delta1 = -tilex + delta1;

                nouvo->u = tmp->tab[0]->u + delta1;
                nouvo->c = tmp->tab[1]->c;
                tmp->tab[1] = nouvo;   // assigne new vertex a cette face
                AddVertex(nouvo);
            }

            if (fabs(delta2) > tilex*.51f) {
                nouvo = new VertexU3DConv;
                nouvo->v = tmp->tab[2]->v;

                if (delta2< 0.0f) delta2 = tilex + delta2;
                else               delta2 = -tilex + delta2;

                nouvo->u = tmp->tab[0]->u + delta2;
                nouvo->c = tmp->tab[2]->c;
                tmp->tab[2] = nouvo;   // assigne new vertex a cette face
                AddVertex(nouvo);
            }
        }
        if (tmp->wrap&0x2) {
            // WARNING ca bug ca fo ke je verifi !!!!!!!!!!!!!!!!
            /*          Ufloat delta1 = tmp->tab[1]->v - tmp->tab[0]->v;
                        Ufloat delta2 = tmp->tab[2]->v - tmp->tab[0]->v;
                        Ufloat tiley;
                        if( tmp->mat )
                            tiley = tmp->mat->tiley;
                        else
                            tiley = 1.f;

                        if( fabs(delta1) > tiley*.51f )
                        {
                            nouvo = new VertexU3DConv;
                            nouvo->u = tmp->tab[1]->u;

                            if( delta1< 0.0f ) delta1 = tiley + delta1;
                            else               delta1 = -tiley + delta1;

                            nouvo->v = tmp->tab[0]->v + delta1;
                            nouvo->c = tmp->tab[1]->c;
                            tmp->tab[1] = nouvo;   // assigne new vertex a cette face
                            AddVertex( nouvo );
                        }

                        if( fabs(delta2) > tiley*.51f )
                        {
                            nouvo = new VertexU3DConv;
                            nouvo->u = tmp->tab[2]->u;

                            if( delta2< 0.0f ) delta2 = tiley + delta2;
                            else               delta2 = -tiley + delta2;

                            nouvo->v = tmp->tab[0]->v + delta2;
                            nouvo->c = tmp->tab[2]->c;
                            tmp->tab[2] = nouvo;   // assigne new vertex a cette face
                            AddVertex( nouvo );
                        }*/
        }

        tmp = tmp->suiv;
    }
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::WriteFaces(CArchive& ar) {
    PolygonU3DConv* tmp = Ftab;

    ar << nbfaces;

    while (tmp) {
        ar << tmp->tab[0]->num;
        ar << tmp->tab[1]->num;
        ar << tmp->tab[2]->num;

        ar << tmp->flag;
        if (tmp->mat)   ar << tmp->mat->num;
        else            ar << (U8)255;

        tmp = tmp->suiv;
    }
}
//----------------------------------------------------------------------------------------------------------
//                                              Material
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::AddMat(MaterialU3DConv* m) {
    m->suiv = mat;
    m->num = nbmats;
    mat = m;
    nbmats++;
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::AddMat(LPCSTR nom) {
    MaterialU3DConv* nouvo;

    nouvo = new MaterialU3DConv;
    nouvo->nom =  nom;
    AddMat(nouvo);
}
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::WriteMats(CArchive& ar) {
    MaterialU3DConv* tmp = mat;

    ar << nbmats;

    while (tmp) {
        ar << tmp->nom;
        tmp = tmp->suiv;
    }
}
//----------------------------------------------------------------------------------------------------------
//                                          Hiérarchie
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::AddChild(ObjetU3DConv* child) {
    child->Childsuiv = Childs;
    Childs = child;
}
//----------------------------------------------------------------------------------------------------------
ObjetU3DConv* ObjetU3DConv::FindChild(LPCSTR nom) {
    ObjetU3DConv* tmp = Childs, *tmp1;

    while (tmp) {
        if (strcmp(tmp->GetNom(), nom)==0) {
            return tmp;
        } else {
            tmp1 = tmp->FindChild(nom);
            if (tmp1) return tmp1;
        }

        tmp = tmp->Childsuiv;
    }

    return NULL;
}
//----------------------------------------------------------------------------------------------------------
//                                          Sauvegarde
//----------------------------------------------------------------------------------------------------------
void ObjetU3DConv::Serialize(CArchive& ar) {
    U32             a;
    ObjetU3DConv*    Otmp = Childs;

    //------------------ Ecrit les données relative a l'objet
    if (ar.IsStoring()) {
        ar << nom;
        ar << csParent;

        locaMatrix.Serialize(ar);

        WriteCoords(ar);
        WriteVertex(ar);
        WriteFaces(ar);
        WriteMats(ar);


        //-------------- sauve les positions

        ar << npkeys;
        for (a=0; a<npkeys;  a++)   posK[a].Serialize(ar);

        ar << nrkeys;
        for (a=0; a<nrkeys; a++) rotK[a].Serialize(ar);

        ar << nmkeys;
        for (a=0; a<nmkeys;  a++) morphK[a].Serialize(ar);

        ar << nhkeys;
        for (a=0; a<nhkeys; a++) hideK[a].Serialize(ar);

        //--------------


        ar << CHUNK_OBJ_END;
    }

    //------------------ Sauve les fils de cet objet
    Otmp = Childs;
    while (Otmp) {
        ar << CHUNK_OBJ;
        Otmp->Serialize(ar);
        Otmp = Otmp->Childsuiv;
    }
}
//----------------------------------------------------------------------------------------------------------


#endif
