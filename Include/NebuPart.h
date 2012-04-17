/*-----------------------------------------------------+
 |                      NebuPart.h                      |
 |                                                      |
 |  Va gérer le champ de particule de la nebuleuse de   |
 |                      la 1ere partie                  |
 |                                                      |
 |  U2^PoPsy TeAm 2000                              |
 +-----------------------------------------------------*/

#ifndef _NEBUPART_VIP2_H
#define _NEBUPART_VIP2_H


/////////////////////////////////////////
// defintion d'une petite particule
class OneNebuParticule {

//--------------- Datas
private:

    SplineU3D*   mouvement;
    Ufloat      RVBA[4];
    Ufloat      size;


//-------------- Fonctions
public:

    OneNebuParticule();
    ~OneNebuParticule();

    void Move(Ufloat step) {
        mouvement->IncPos(step);
    }

    void    GetPos(Ufloat vec[3])  {
        mouvement->GetVal(vec);
    }
    Ufloat* GetColor()              {
        return RVBA;
    }
    Ufloat  GetSize()               {
        return size;
    }
};
/////////////////////////////////////////




/////////////////////////////////////////
// definition la classe ki va gérer les particules
class Nebuparticule {

//--------------- Datas
private:

    TextureU3D*  partTex;   // image de la particule de base
    MaterialU3D* partMat;   // material de la particule de base
    BitmapU3D*   partBmp;   // bitmpa de la particule

    UImage* Explodeima;

    OneNebuParticule*    AllPart;   // tout les particules

    Ufloat centreX,centreY;   // coord 2D du centre de la nebuleuse !

//-------------- Fonctions
private:

    void AffBigPart();   // affiche grosse particule centrale
    void AffAllPart(Ufloat step);     // affiche le champ de particules

public:

    Nebuparticule(LPCSTR nomTex, LPCSTR nomTexEx);
    ~Nebuparticule();

    void Explode(Ufloat time);
    void UpdateFrame(Ufloat step);
};
/////////////////////////////////////////




/////////////////////////////////////////
// definit un rayon ki ira sur la nebuleuse avant lexplosion
class OneNebuRayon {

//--------------- Datas
private:

    Ufloat vec[3];
    Ufloat dist;
    Ufloat taille;
    Ufloat timeToGo;
    Ufloat totalTime;

//-------------- Fonctions
public:

    OneNebuRayon();

    void Ini();
    bool Move(Ufloat step)                                         {
        timeToGo-=step;
        if (timeToGo<0.0f) return FALSE;
        else return TRUE;
    }
    void GetVal(Ufloat debut[3], Ufloat fin[3], Ufloat& intensity);     // retourne intensité du rayon
};
/////////////////////////////////////////



/////////////////////////////////////////
// classe ki va gérer tout les rayons
class NebuRayon {

//--------------- Datas
private:

    OneNebuRayon*   AllRayon;
    Ufloat          TimeToGo;
    Ufloat          TotalTime;

//-------------- Fonctions
public:

    NebuRayon(Ufloat tappear);
    ~NebuRayon();

    void UpdateFrame(Ufloat step);
};
/////////////////////////////////////////

#endif
