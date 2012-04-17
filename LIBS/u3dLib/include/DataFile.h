/*------------------------------------------------------+
  |                  DataFile.h                         |
  |                                                     |
  |  class ki permet de récupérer facilement se kil y   |
  |     dans un GTZ ( une surcouche koa en fait )       |
  |                                                     |
  |     U2^PoPsy TeAm 2000                              |
  +-----------------------------------------------------*/

#include <vector>

///////////////////////////////////
// definition de la classe
class DataManager {

    //GTZpack   *pack;

public:

    DataManager();
    ~DataManager();

    void LoadGTZ(CArchive& ar);     // renvoie TRUE si succes sinon FALSE ( pf on l'aurai pas deviné )
    bool LoadGTZ(LPCSTR nom);     // renvoie TRUE si succes sinon FALSE ( pf on l'aurai pas deviné )

    void SetCurrentFolder(LPCSTR nom);

    std::vector<char>* GetData(LPCSTR nom,bool destruc=FALSE);   // voir fonctions danbs pack.h (de la  GTZLib )
    std::vector<char>* GetDataFolder(LPCSTR nom,bool destruc=FALSE);    // voir fonctions danbs pack.h (de la  GTZLib )
};
///////////////////////////////////
