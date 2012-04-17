/*---------------------------------------------------------------------+
 |                          EtoileGreets.cpp                            |
 |                                                                      |
 |   les greets c'est bien design et ca permet de faire une jolie scene |
 |                          voiliiiiii                                  |
 |                                                                      |
 |  U2^PoPsy TeAm 2000                                              |
 +---------------------------------------------------------------------*/

#include "stdafx.h"


//----------------------------------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      +-------------------+
//                                      |  Class EtoilePart |
//                                      +-------------------+
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------
EtoileGreets::EtoileGreets(Ufloat timedefil) {
    TDefil = timedefil;
    Temps = 0.0f;

    /*  GreetsInter = new UImage( "Datas/meshes/Etoile/greetsI.jpg", BILINEAR_TEXTURE, 256.f, 512.f );
        GreetsInter->SetTransp( MatTranspAddAlpha );
        GreetsInter->SetUpLeftPos( 0.0f, 0.0f );
        GreetsInter->InterpoYPos( 1.f + GreetsInter->GetYSize()*.5f, -GreetsInter->GetYSize()*.5f, TDefil );

        GreetsFrench = new UImage( "Datas/meshes/Etoile/greetsF.jpg", BILINEAR_TEXTURE, 256.f, 512.f );
        GreetsFrench->SetTransp( MatTranspAddAlpha );
        GreetsFrench->SetUpLeftPos( 1.0f - GreetsFrench->GetXSize(), 0.0f );
        GreetsFrench->InterpoYPos( -GreetsFrench->GetYSize()*.5f, 1.f + GreetsFrench->GetYSize()*.5f, TDefil );*/

    /*  Cote1 = new UImage( "Datas/meshes/Etoile/cote.jpg", BILINEAR_TEXTURE|TILE_TEXTURE );
        Cote1->SetTransp( MatTranspAddAlpha );
        Cote1->SetYSize( 1.0f );
        Cote1->SetUpLeftPos( 0.0f, 0.0f );

        Cote2 = new UImage;
        (*Cote2) = (*Cote1);
    //  Cote2->SetPos( GreetsFrench->GetXpos() - GreetsFrench->GetXSize()*.5f + Cote2->GetXSize()*.5f, .5f );
        Cote2->SetPos( .75f, .5f );*/


    AddImage(0, "e_sunflower.jpg", GetUniXpos(470.f), GetUniYpos(177.f));
    AddImage(1, "f_vengeurs.jpg", GetUniXpos(492), GetUniYpos(320.f));
    AddImage(2, "f_doh.jpg", GetUniXpos(560.f), GetUniYpos(88.f));
    AddImage(3, "f_digital.jpg", GetUniXpos(248.f), GetUniYpos(440.f));

    AddImage(4, "f_ojuice.jpg", GetUniXpos(160.f), GetUniYpos(80.f));
    AddImage(5, "f_calodox.jpg", GetUniXpos(530.f), GetUniYpos(400.f));
    AddImage(6, "e_tbl.jpg", GetUniXpos(210.f), GetUniYpos(440.f));
    AddImage(7, "f_gamesquad.jpg", GetUniXpos(450.f), GetUniYpos(240.f));

    AddImage(8, "e_matrix.jpg", GetUniXpos(120.f), GetUniYpos(300.f));
    AddImage(9, "f_bomb.jpg", GetUniXpos(525.f), GetUniYpos(95.f));
    AddImage(10, "f_orion.jpg", GetUniXpos(547.f), GetUniYpos(400.f));
    AddImage(11, "f_eclipse.jpg", GetUniXpos(150.f), GetUniYpos(130.f));

    AddImage(11+1, "f_syndrome.jpg", GetUniXpos(137.f), GetUniYpos(66.f));
    AddImage(12+1, "f_mandarine.jpg", GetUniXpos(508.f), GetUniYpos(400.f));
    AddImage(13+1, "f_condense.jpg", GetUniXpos(526.f), GetUniYpos(307.f));
    AddImage(14+1, "e_purple.jpg", GetUniXpos(120.f), GetUniYpos(401.f));

    AddImage(15+1, "f_skytech.jpg", GetUniXpos(130.f), GetUniYpos(225.f));
    AddImage(16+1, "e_tpolm.jpg", GetUniXpos(186.f), GetUniYpos(450.f));
    AddImage(17+1, "f_cocoon.jpg", GetUniXpos(540.f), GetUniYpos(370.f));
    AddImage(18+1, "e_blasphemy.jpg", GetUniXpos(450.f), GetUniYpos(120.f));

    AddImage(19+1, "e_haujobb.jpg", GetUniXpos(150.f), GetUniYpos(90.f));
    AddImage(20+1, "e_komplex.jpg", GetUniXpos(500.f), GetUniYpos(75.f));
    AddImage(21+1, "f_wipe.jpg", GetUniXpos(520.f), GetUniYpos(300.f));
    AddImage(22+1, "e_exceed.jpg", GetUniXpos(200.f), GetUniYpos(360.f));
}
//----------------------------------------------------------------------------------------------------------------------------------------
EtoileGreets::~EtoileGreets() {
    /*  delete GreetsInter;
        delete GreetsFrench;*/
    /*  delete Cote2;
        delete Cote1;*/

    for (U32 a=0; a<24; a++)
        delete Greets[a];
}
//----------------------------------------------------------------------------------------------------------------------------------------
void EtoileGreets::AddImage(U32 num, LPCSTR nom, Ufloat posx, Ufloat posy) {
    char nomf[256];
    sprintf(nomf, "Datas/meshes/etoile/%s", nom);

    Greets[num] = new UImage(nomf, BILINEAR_TEXTURE|ALPHA_TEXTURE);
    Greets[num]->SetTransp(MatTranspAddAlpha);
    Greets[num]->SetPos(posx, posy);
    Greets[num]->Disable();
}
//----------------------------------------------------------------------------------------------------------------------------------------
void EtoileGreets::DisableAll() {
    for (U32 a=0; a<24; a++)
        Greets[a]->Disable();
}
//----------------------------------------------------------------------------------------------------------------------------------------
// balance un screen de greets
void EtoileGreets::GoScreen(U32 num) {
    DisableAll();

    switch (num) {
    case 0:
        Greets[0]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[0]->Enable();
        Greets[1]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[1]->Enable();
        Greets[2]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[2]->Enable();
        Greets[3]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[3]->Enable();
        break;

    case 1:
        Greets[4]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[4]->Enable();
        Greets[5]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[5]->Enable();
        Greets[6]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[6]->Enable();
        Greets[7]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[7]->Enable();
        break;

    case 2:
        Greets[8]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[8]->Enable();
        Greets[9]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[9]->Enable();
        Greets[10]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[10]->Enable();
        Greets[11]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[11]->Enable();
        break;

    case 3:
        Greets[11+1]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[11+1]->Enable();
        Greets[12+1]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[12+1]->Enable();
        Greets[13+1]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[13+1]->Enable();
        Greets[14+1]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[14+1]->Enable();
        break;

    case 4:
        Greets[15+1]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[15+1]->Enable();
        Greets[16+1]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[16+1]->Enable();
        Greets[17+1]->InterpoZoom(2.0f, 1.0f, 2.0f);
        Greets[17+1]->Enable();
        Greets[18+1]->InterpoZoom(1.0f, 2.0f, 2.0f);
        Greets[18+1]->Enable();
        break;

    case 5:
        Greets[19+1]->InterpoZoom(2.0f, 1.0f, 1.0f);
        Greets[19+1]->Enable();
        Greets[20+1]->InterpoZoom(1.0f, 2.0f, 1.0f);
        Greets[20+1]->Enable();
        Greets[21+1]->InterpoZoom(2.0f, 1.0f, 1.0f);
        Greets[21+1]->Enable();
        Greets[22+1]->InterpoZoom(1.0f, 2.0f, 1.0f);
        Greets[22+1]->Enable();
        break;
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------
void EtoileGreets::AfficheFore(Ufloat laptime) {
    Temps += laptime;

    /*  Cote1->GetMaterial()->SetTex1Pos( 0.0f, -Temps, 1.0f, 1.0f );
        Cote2->GetMaterial()->SetTex1Pos( 0.0f, -Temps+.5f, 1.0f, 1.0f );*/

    /*  Cote1->Affiche( laptime );
        Cote2->Affiche( laptime );*/

    for (U32 a=0; a<24; a++)
        Greets[a]->Affiche(laptime);
    /*  GreetsInter->Affiche( laptime );
        if( !GreetsInter->IsInterpoYPos() )
            GreetsInter->InterpoYPos( 1.f + GreetsInter->GetYSize()*.5f, -GreetsInter->GetYSize()*.5f, TDefil );

        GreetsFrench->Affiche( laptime );
        if( !GreetsFrench->IsInterpoYPos() )
            GreetsFrench->InterpoYPos( -GreetsFrench->GetYSize()*.5f, 1.f + GreetsFrench->GetYSize()*.5f, TDefil );*/
}
//----------------------------------------------------------------------------------------------------------------------------------------
