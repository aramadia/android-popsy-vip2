/*-----------------------------------------------------+
 |                       material.cpp                   |
 |                                                      |
 |   et op une chtite définition de material pour les   |
 |                  polygones 3D                        |
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
//                                          MaterialU3D
//----------------------------------------------------------------------------------------------------------
MaterialU3D::MaterialU3D(const char* n) {
    nom = strdup(n);

    vec4_set(RVBA, 1.f, 1.f, 1.f, 1.f);

    //------------------- Infos textures
    Tex1    = NULL;
    Xstart1 = 0.0f;
    Ystart1 = 0.0f;
    Xdelta1 = 1.0f;
    Ydelta1 = 1.0f;
    Transparence1   = MatTranspRien;

    Tex2    = NULL;
    Xstart2 = 0.0f;
    Ystart2 = 0.0f;
    Xdelta2 = 1.0f;
    Ydelta2 = 1.0f;
    Transparence2   = MatTranspRien;

    Side2 = FALSE;
    Shading = MatShadingRien;
    SpecularShading = TRUE;   // !!!!!!!!!!!!!! TMP doit etre FALSE par defaut !!!!!!!!!!

    MatPipe =  NULL;
}
//----------------------------------------------------------------------------------------------------------
MaterialU3D::~MaterialU3D() {
    if (Tex1)   TextureManager::SubUse(Tex1);
    if (Tex2)   TextureManager::SubUse(Tex2);
}
//----------------------------------------------------------------------------------------------------------
MaterialU3D& MaterialU3D::operator=(MaterialU3D& mat) {
    nom = mat.nom;

    vec4_eg(RVBA, mat.RVBA);

    //------------------- Infos textures
    SetTexture1(mat.Tex1);
    Xstart1 = mat.Xstart1;
    Ystart1 = mat.Ystart1;
    Xdelta1 = mat.Xdelta1;
    Ydelta1 = mat.Ydelta1;
    Transparence1   = mat.Transparence1;

    SetTexture2(mat.Tex2);
    Xstart2 = mat.Xstart1;
    Ystart2 = mat.Ystart2;
    Xdelta2 = mat.Xdelta1;
    Ydelta2 = mat.Ydelta2;
    Transparence2   = mat.Transparence2;

    Side2 = mat.Side2;
    Shading = mat.Shading;
    SpecularShading = mat.SpecularShading;

    MatPipe =  mat.MatPipe;

    return *this;
}
//----------------------------------------------------------------------------------------------------------
// remet a jour MatPipe !!!!!!!! ATTENTION doit etre fait apres une MODIF de texture ou de shading !!!
void MaterialU3D::Confirm() {
    /*  if( MatPipe ) U3D3Monde3D->GetActualPipe()->Release( (MatPipeline*)MatPipe );
        MatPipe = (void*)U3D3Monde3D->GetActualPipe()->GetOrCreate( this );*/
    if (MatPipe) U3D3Pipeline->Release((MatPipeline*)MatPipe);
    MatPipe = (void*)U3D3Pipeline->GetOrCreate(this);
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetTransp(U32 tr) {
    Transparence1 = tr;
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetTranspTex2(U32 tr) {
    Transparence2 = tr;
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetTexture1(TextureU3D* tex) {
    if (Tex1 != tex)
        if (Tex1)   TextureManager::SubUse(Tex1);

    Tex1 = tex;

    if (Tex1)   TextureManager::AddUse(Tex1);
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetTexture2(TextureU3D* tex) {
    if (Tex2 != tex)
        if (Tex2)   TextureManager::SubUse(Tex2);

    Tex2 = tex;

    if (Tex2)   TextureManager::AddUse(Tex2);
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetTextures(TextureU3D* tex1, TextureU3D* tex2) {
    SetTexture1(tex1);
    SetTexture2(tex2);
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetColorFromTXM(U32 r, U32 v, U32 b, U32 a) {
    vec4_set(RVBA, (float)r/255.f, (float)v/255.f, (float)b/255.f, (float)a/255.f);
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetTex1PosFromTXM(Ufloat u, Ufloat v) {
    if (Tex1) {
        Xstart1 = (float)u / (Tex1->Width()-1);
        Ystart1 = (float)v / (Tex1->Height()-1);
    }
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetTex2PosFromTXM(Ufloat u, Ufloat v) {
    if (Tex2) {
        Xstart2 = (float)u / (Tex2->Width()-1);
        Ystart2 = (float)v / (Tex2->Height()-1);
    }
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetTex1DeltaFromTXM(Ufloat u, Ufloat v) {
    if (Tex1) {
        Xdelta1 = (float)u / (Tex1->Width()-1);
        Ydelta1 = (float)v / (Tex1->Height()-1);
    }
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::SetTex2DeltaFromTXM(Ufloat u, Ufloat v) {
    if (Tex2) {
        Xdelta2 = (float)u / (Tex2->Width()-1);
        Ydelta2 = (float)v / (Tex2->Height()-1);
    }
}
//----------------------------------------------------------------------------------------------------------
bool MaterialU3D::Cmp(LPCSTR n) {
    if (strcmp((LPCSTR)nom,n)==0)
        return TRUE;
    return FALSE;
}
//----------------------------------------------------------------------------------------------------------
void MaterialU3D::Serialize(CArchive& ar) {
    char tmp[256];
    char tmp1[256];
    U32     iTex=0;

    if (ar.IsStoring()) {
        // ecrit nom material
        sprintf(tmp, "Material %s \15\n", nom);
        ar.WriteString(tmp);

        // ecrit couleur Solid
        sprintf(tmp, "	Couleur Solid R %d V %d B %d A %d\15\n", (
                    U32)(RVBA[0]*255.f), (U32)(RVBA[1]*255.f),(U32)(RVBA[2]*255.f), (U32)(RVBA[3]*255.f));
        ar.WriteString(tmp);

        // ecrit si 2side
        if (Side2)
            ar.WriteString("	2side\15\n");

        // ecrit le shading
        switch (Shading) {
        case MatShadingRien :
            ar.WriteString("	Shading Rien\15\n");
            break;
        case MatShadingFlat :
            ar.WriteString("	Shading Flat\15\n");
            break;
        case MatShadingGouraud :
            ar.WriteString("	Shading Gouraud\15\n");
            break;
        case MatShadingPhong :
            ar.WriteString("	Shading Phong\15\n");
            break;
        }

        // ecrit transparence
        switch (Transparence1) {
        case MatTranspAdd :
            ar.WriteString("	Transp Add\15\n");
            break;
        case MatTranspMul :
            ar.WriteString("	Transp Mul\15\n");
            break;
        case MatTranspAddAlpha :
            ar.WriteString("	Transp AAdd\15\n");
            break;
        case MatTranspMulAlpha :
            ar.WriteString("	Transp AMul\15\n");
            break;
        case MatTranspAlpha :
            ar.WriteString("	Transp Alpha\15\n");
            break;
        case MatTranspTrou :
            ar.WriteString("	Transp Trou\15\n");
            break;
        }

        // ecrit les textures
        if (Tex1) {
            iTex++;
            sprintf(tmp, "	Tex1 %s\15\n", TextureManager::GetNom(Tex1));
            ar.WriteString(tmp);
            sprintf(tmp, "	PosTex1 U %2.3f V %2.3f\15\n", Xstart1*(Tex1->Width()-1), Ystart1*(Tex1->Height()-1));
            ar.WriteString(tmp);
            sprintf(tmp, "	DeltaTex1 U %2.3f V %2.3f\15\n", Xdelta1*(Tex1->Width()-1), Ydelta1*(Tex1->Height()-1));
            ar.WriteString(tmp);
        }
        if (Tex2) {
            iTex++;
            sprintf(tmp, "	Tex%d %s\15\n", iTex, TextureManager::GetNom(Tex2));
            ar.WriteString(tmp);
            sprintf(tmp, "	PosTex%d U %2.3f V %2.3f\15\n", iTex, Xstart2*(Tex2->Width()-1), Ystart2*(Tex2->Height()-1));
            ar.WriteString(tmp);
            sprintf(tmp, "	DeltaTex%d U %2.3f V %2.3f\15\n", iTex, Xdelta2*(Tex2->Width()-1), Ydelta2*(Tex2->Height()-1));
            ar.WriteString(tmp);
        }
        if (iTex==2) {
            switch (Transparence2) {
            case MatTranspAdd :
                ar.WriteString("	TexTransp2 Add\15\n");
                break;
            case MatTranspMul :
                ar.WriteString("	TexTransp2 Mul\15\n");
                break;
            case MatTranspAddAlpha :
                ar.WriteString("	TexTransp2 AAdd\15\n");
                break;
            case MatTranspMulAlpha :
                ar.WriteString("	TexTransp2 AMul\15\n");
                break;
            case MatTranspAlpha :
                ar.WriteString("	TexTransp2 Alpha\15\n");
                break;
            }
        }



        // enfin saute une ligne
        ar.WriteString("\15\n");
    }
}
//----------------------------------------------------------------------------------------------------------
