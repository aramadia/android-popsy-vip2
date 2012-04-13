//---------------------------------------------------------------------------------------------------------------
//						+---------------------------------------------------+
//						|	Class qui gere le rendu dans les DIB etc....    |
//					    +---------------------------------------------------+
//---------------------------------------------------------------------------------------------------------------



//=================
// Defintion des Constantes


// ----> couleurs
#define TRANSP_COLOR_SKIN		000
#define GO_COLOR_SKIN			128
#define R640_COLOR_SKIN		129
#define R800_COLOR_SKIN		130
#define R1024_COLOR_SKIN		131
#define VURL_COLOR_SKIN		132
#define VTXT_COLOR_SKIN		133
#define PURL_COLOR_SKIN		134
#define QUIT_COLOR_SKIN		135
#define MONO_COLOR_SKIN		136
#define NBPP_COLOR_SKIN		137
#define NFO_COLOR_SKIN			138

	
//-----> position dans le mask
#define DEHORS_SKIN		0x00000001
#define DANS_SKIN			0x00000002
#define ID_LAUNCHBUTTON	0x00000004
#define ID_640BUTTON		0x00000005
#define ID_6408BUTTON		0x00000028
#define ID_800BUTTON		0x00000006
#define ID_1024BUTTON		0x00000007
#define ID_VURLBUTTON		0x00000008
#define ID_VTXTBUTTON		0x00000009
#define ID_PURLBUTTON		0x00000010
#define ID_QUITBUTTON		0x00000003
#define ID_MONOBUTTON		0x00000012
#define ID_STERBUTTON		0x00000026
#define ID_NBPPBUTTON32	0x00000013
#define ID_NBPPBUTTON16	0x00000025
#define ID_NFOBUTTON		0x00000011
#define ID_8001BUTTON		0x00000029

#define ID_640BUTTONA		0x00000014
#define ID_800BUTTONA		0x00000015
#define ID_1024BUTTONA		0x00000016
#define ID_VURLBUTTONA		0x00000017
#define ID_VTXTBUTTONA		0x00000018
#define ID_PURLBUTTONA		0x00000019
#define ID_QUITBUTTONA		0x00000021
#define ID_MONOBUTTONA		0x00000022
#define ID_NBPPBUTTONA		0x00000023
#define ID_NFOBUTTONA		0x00000024



//=================
// Defintion de la classe! de la Skin Principale

class DIBRender{

	static U32*	SkinGen		;	// Bitmap de la skin Generale en 32 bit
	static U8*	MaskGen		;	// Bitmap de Mask en 8 bit	( couche alpha)
	static U8*	ActionGen	;	// Bitmap de ou ya les zones d'action
	static int	xGen,yGen	;	// dimension des bitmap pour la skin generale

	static U32*	ScreenGen		;	// Mask pour écran
	static int	TailleScreen	;	// taille de l'ecran
	static int	TailleXScreen	;	// taille en X de l'ecran
	static int	TailleYScreen	;	// taille en Y de l'ecran

	static BitmapSkin*	ListeBitmap		;	// liste de toute les bitmap de la skin
	static BoutonSkin*	ListeBouton		;	// liste de tout les boutons de la skin
	static MultiAff*	ListeMultiAff	;	// liste de tout les boutons de la skin a checker
//
public:

//	static HWND m_hWnd				;
	static unsigned int	*BackGen	;	// mémoire tampon

	static void Initialise()	;
	static void Release()			;

	static void			AddBitmap( int ID, LPCSTR nomOpen, bool msk=FALSE )		;	// ajoute une bitmap dans la skin
	static BitmapSkin	*GetBitmap( int ID )									;	// retourn bitmap avec cette ID

	static void			AddBouton( int ID, int xp, int yp, int colormask, BitmapSkin *b )	;
	static BoutonSkin*	GetBouton( int ID )													;

	static void			AddMultiAff( int ID, int xp, int yp  )	;
	static MultiAff*	GetMultiAff( int ID )					;

/*
	static void		LoadBitmap24bit(int ID, int &x, int &y, unsigned int *&dest )			;
	static void		LoadBitmap8bit(int ID, int &x, int &y, char *&dest )					;
*/
	static void		LoadBitmapJpg( LPCSTR nom, int &x, int &y, U32 *&dest )					;
	static void		LoadBitmapRaw( LPCSTR nom, int x, int y, U8 *&dest )					;
//	static void		AffiSkinGen(CDC *Cdest, HWND hwnd, int xpos, int ypos )					;	// Prend le Background
	static int		OuEstTu(int x, int y )													;	// retourne l'ID de l'element
	static int		GetColorMask( int x, int y )											;	// renvoie couleur dans la mask a position x, y
//	static HRGN		MakeRegion()															;
	static void		AffSkinBmp()															;
	static void		AffInSkin( unsigned int *src, int x, int y, int width, int heigth )		;
	static void		AffInSkinMsk( unsigned int *src, int x, int y, int width, int heigth )	;
	static void		AffInSkinAdd( unsigned int *src, int x, int y, int width, int heigth )	;

	static void	GetDim(int &width, int &height)	;	// Renvoie taille que devrait faire la fenetre
	static int	GetYSize()						{return yGen;}
	static int	GetXSize()						{return xGen;}

	static void	ClearAllState()	;	// remet tout les etats de chak bouton a 0 ( non highlighté )

	static void	TestScreenOrAllocate( int x, int y )	;
	static void	GetBackGround()						;
	static void	FillBackWindow( int xpos, int ypos )	;	// remplit le fond de la fenetre

};
