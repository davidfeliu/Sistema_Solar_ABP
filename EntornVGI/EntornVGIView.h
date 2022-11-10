//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb interfície MFC, llibreries GLM i OpenGL 3.3+
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2022)
// EntornVGIView.h : interface of the CEntornVGI class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MainFrm.h"

// Entorn VGI: Constants de l'aplicació entorn VGI
#include "constants.h"

// Entorn VGI: Includes shaders GLSL
#include <gl/glew.h>
#include <gl/wglew.h>

// Include all GLM core / GLSL features
#include <glm/glm.hpp>		// perspective, translate, rotate
// Include all GLM extensions
#include <glm/ext.hpp>

#include "shader.h"

// Entorn VGI: OBJECTE 3DS. Include per la definició de l'objecte Obj_3DS
#include "Obj3DS.h"	

// Entorn VGI: OBJECTE OBJ. Include per la definició de l'objecte Obj_OBJ
#include "objLoader.h"	

class CEntornVGIView : public CView
{
protected: // Crear sólo a partir de serialización
	CEntornVGIView();
	DECLARE_DYNCREATE(CEntornVGIView)

// Atributos
public:
	CEntornVGIDoc* GetDocument() const;

//-------------- Entorn VGI: Variables globals de CPractivaView
	HGLRC	 m_hrc;		// OpenGL Rendering Context 

// Entorn V3D: Variables de control per Menú Càmera: Esfèrica, Navega, Mòbil, Zoom, Satelit, Polars... 
	char camera;	// Variable que controla el tipus de càmera segons valors definits en constants.h
	bool mobil;		// Opció canvi de Punt de Vista interactiu (mobil) [0:NO,1:SI]
	bool zzoom;		// Opció Zoom interactiu (zoom) [0:NO,1:SI]
	bool zzoomO;	// Opció Zoom en Projecció Ortogràfica adaptant Volum Visualització [0:NO,1:SI]
	bool satelit;	// Opció de navegació animada al volt de l'objecte centrat definint increment per mouse (SATÈLIT)

// Entorn VGI: Variables de control de l'opció Càmera->Navega?
	GLdouble n[3];			// Punt cap on mira.
	CPunt3D opvN;			// Definició Orígen Punt de Vista (en coordenades món)
	double angleZ;			// angle de rotació de la trajectòria.
	glm::mat4 ViewMatrix;	// Matriu de Càmera a passar al shader

// Entorn VGI: Variables de control de l'opció Càmera->Geode?
	CEsfe3D OPV_G;			// Paràmetres camera en coord. esfèriques (R,alfa,beta) per a Vista Geode

// Entorn VGI: Variables de control per Menú Vista: fullsecreen, pan, dibuixar eixos i grid. 
	bool fullscreen;	// Opció booleana per Pantalla Completal [0:NO,1:SI]
	bool pan;			// Opcio Pan. Desplaçament del centre de l'esfera de Vista [0:NO,1:SI]
	bool eixos;			// Opció per dibuixar els eixos X,Y,Z (Eixos?) [0:NO,1:SI]
	Shader shaderEixos;	// Ientificador pel shader d'eixos.
	GLuint eixos_programID;
	GLuint eixos_Id;	// Identificador del VBO per a dibuixar els eixos.
	bool sw_grid;		// Opció per activar comandes teclat per moure grids [TRUE] o no [FALSE]
	CMask3D grid;		// Opció per a dibuixar grids (.x: grid XY, .y:grid XZ, .z: grid YZ, .w:grid XYZ)
	CPunt3D hgrid;		// Alçada de cada grid (.x: grid XY, .y:grid XZ, .z: grid YZ, .w:grid XYZ)

// Entorn VGI: Variables de l'opció Vista->Pan
	double fact_pan;	// Factor de desplaçament de la càmara (opció pan).
	CPunt3D tr_cpv;		// Vector de Traslació del Centre del Punt de Vista actiu dins opció pan.
	CPunt3D tr_cpvF;	// Vector de Traslació del Centre del Punt de Vista (fixat amb INSERT dins opció pan) 
						//       i actiu fora l'opció pan.

// Entorn VGI: Variables de control per les opcions de menú Projecció, Objecte
	char projeccio;	// Menú Projecció: Tipus de projeccio
	glm::mat4 ProjectionMatrix;// Matriu de Projecció a passar al shader
	char objecte;	// Menú Objecte: Tipus d'Objecte
	SSolar SS;


// Entorn VGI: Variables de control Skybox Cube
	bool SkyBoxCube;			// Booleana que controla si es visualitza Skybox [TRUE] o no [FALSE].
	Shader shader_SkyBoxC;		// Shader Skybox Cub
	GLuint skC_programID;		// Identificador program Skybox Cube
	CVAO skC_VAOID;				// Identificador VAO List (vaoId, vboId, nVertexs) per a Skybox Cube
	GLuint cubemapTexture;		// Identificador textura cubemap.
	Shader shaderSkyBox;		// Shader SkyBox

// Entorn VGI: Variables de control del menú Transforma
	bool transf;	// Booleana per activar Transformacions (true) o no (false).
	bool trasl;		// Booleana d'activació de la Traslació (true) o no (false).
	bool rota;		// Booleana d'activació de la Rotació (true) o no (false).
	bool escal;		// Booleana d'activació de l'Escalatge (true) o no (false).
	double fact_Tras, fact_Rota;	// Increments de Traslació i Rotació.
	INSTANCIA TG;	// Estructura que conté TG del menú Transforma actiu dins qualsevol opció de Transforma 
					//      (Traslació Rotació i Escalatge).
	INSTANCIA TGF;	// Estructura que conté TG del menú Transforma fixat amb INSERT dins les opcions de Transforma
					//     i actiu fora l'opció Transforma.
	bool transX;	// Opció Mobil Eix X?: TG interactives per l'eix X via mouse [F:NO,T:SI].
	bool transY;	// Opció Mobil Eix Y?: TG interactives per l'eix Y via mouse [F:NO,T:SI].
	bool transZ;	// Opció Mobil Eix Z?: TG interactives per l'eix Z via mouse [F:NO,T:SI].
	glm::mat4 GTMatrix; // Matriu de Transformacions Gemètriques a passar al shader

// Entorn VGI: Variables de control per les opcions de menú Ocultacions
	bool front_faces;// Menú ocultacions: Determina si les cares visibles són les frontals [true-defecte] o les back [false] pel Test de Visibilitat.
	bool test_vis;  // Menú ocultacions: Activa [true] o desactiva [false] Test Visibilitat.
	bool oculta;    // Menú ocultacions: Activa [true] o desactiva [false] Ocultacions (Z-buffer).
	bool back_line; // Menú ocultacions: Activa [true] o desactiva [false] dibuix cares back com a línies.

// Entorn VGI: Variables de control del menú Iluminació	
	bool ifixe;         // Iluminació fixe independent del PV (ifixe=1) 
						// o depenent (mobil) respecte el PV (casc miner) (ifixe=0)
	bool ilum2sides;	// Iluminació de les cares FRONT i BACK [TRUE] o només les FRONT [FALSE]
	char ilumina;		// Tipus d'il.luminació [PUNTS 'P',FILFERROS 'w', PLANA 'f',GOURAUD 'g', PHONG 'p']
	bool sw_material[4];// Variable que controla els coeficients de reflectivitat del material [TRUE] o no [FALSE]:
						// [0]: emission, [1]: ambient, [2]: difusa, [3] especular.
	bool sw_material_old[4]; // Variable que guarda els valors de sw_material mentre no hi ha reflectivitat de material (refl_material=false).
	bool textura;       // Control de textures desactivades [0:NO] o activades [1:SI]
	char t_textura;		// Tipus de textures (predefinides o per fitxer)
	bool textura_map;	// Mapping de textura modulat amb la llum [TRUE] o calcat [FALSE]
	GLint texturesID[NUM_MAX_TEXTURES];	// Vector d'identificadors de textura de l'entorn. Si no hi ha textura activa, agafa el valor -1.
										// 0: Textura general, assignada a la lectura de fitxer.
										// 1-6: Textures assignables
										// 7: Textura pel Fractal
										// 8-9: Lliures
	bool tFlag_invert_Y; // Booleana que activa la inversió coordenada textura t (o Y) a 1.0-cty segons llibreria SOIL (TRUE) o no (FALSE).

// Entorn VGI: Variables de control del menú Llums
	bool llum_ambient;		// Booleana que controla la llum ambient (SI/NO).
	LLUM llumGL[NUM_MAX_LLUMS];		// Vector de llums d'OpenGL
	bool sw_llambient;		// Booleana que controla modus de configurar el color de la llum ambient [TRUE] o no [FALSE]
	CColor col_llambient;	// Color llum ambient.

// Entorn VGI: Variables de control del menú Shaders
	char shader_menu;			// Variable que defineix el shader actiu
	GLuint shader_programID;	// Shader Program que conté el Vertex i Fragment program.
	Shader shaderLighting;		// Shader que implementa els codis GLSL d'il.luminació
	//GLuint flat_programID, gouraud_programID, phong_programID;	// Shaders programs de les iluminacions plana, de Gouraud i de Phong.

// Entorn VGI: Variables butons de mouse 
	CPoint m_PosEAvall, m_PosDAvall; // Coordenades del cursor quan el boto esquerre(E) o dret(D) del 
									 //    mouse ha estat clicat.
	bool m_ButoEAvall, m_ButoDAvall; //TRUE si el boto esquerre(E) o dret(D) del mouse esta clicat.
	CEsfe3D m_EsfeEAvall;			 // Coordenades Esfèriques del PV (OPV) quan el boto esquerre(E) o dret(D) del 
	//										mouse ha estat clicat.
	CEsfe3D m_EsfeIncEAvall;		 // Increment de desplaçament en coordenades Esfèriques del PV (OPV).

// Entorn VGI: Variables que controlen paràmetres visualització: Mides finestra Windows i PV
	int w, h;				// Mides de la finestra Windows (w-amplada,h-alçada)
	int w_old, h_old;		// Mides de la finestra Windows (w-amplada,h-alçada) per restaurar Finestra des de fullscreen
	CEsfe3D OPV;			// Paràmetres camera en coord. esfèriques (R,alfa,beta)
	char Vis_Polar;			// Variable que controla orientació dels eixos en Visualització Interactiva (POLARZ,POLARY,POLARX)

// Entorn VGI: Color de fons i de l'objecte
	bool fonsR, fonsG, fonsB;	// Booleanes per controlar variació de color per teclat.
	CColor c_fons;			// Intensitat de color de fons.
	bool sw_color;			// Booleana que controla el modus de configurar color de l'objecte per teclat [TRUE] o no [FALSE]
	CColor col_obj;			// Color de l'objecte simple.

// Entorn VGI: Objecte 3DS:
	Obj_3DS* Ob3DS;			// Variable d'objecte 3D Studio (*.3DS)
	CVAO vao_3DS;			// Identificador CVAO per a objecte 3DS
//	int nvert_3DS;			// Nombre de vèrtexs de l'objecte 3DS

// Entorn VGI: Objecte OBJ:
	COBJModel* ObOBJ;		// Variable d'objecte format OBJ (*.OBJ)
	CVAO vao_OBJ;			// Identificador VAO per a objecte OBJ
//	int nvert_OBJ;			// Nombre de vèrtexs de l'objecte OBJ

// Entorn VGI: OBJECTE --> Corbes Bezier i BSpline
	int npts_T;							// Número de punts de control de la corba en el Vector corbaSpline (<=MAX_PATH_SPLINE)
	CPunt3D PC_t[MAX_PATCH_CORBA];		// Vector que enmagatzema Punts de Control Corba Spline
	GLdouble pas_Corba;					// Increment del paràmetre t per al dibuix de les corbes.
	GLdouble pas_CS;						// Increment del paràmetre t per al dibuix de corbes i superficies.
	bool sw_Punts_Control;				// Booleana que activa o desactiva la visualització dels punts de control de la corba o de la superficie

// Entorn VGI. TRIEDRE DE FRENET / DARBOUX: VT: vector Tangent, VNP: Vector Normal Principal, VBN: vector BiNormal
	bool dibuixa_TriedreFrenet;			// Booleana que controla dibuix de Triedre de Frenet per a cada punt de la Corba [TRUE-dibuixa, FALSE-no dibuixa]
	CPunt3D VT, VNP, VBN;				// TRIEDRE de FRENET: VT: Vector Tangent, VNP: Vector Normal Principal, VBN: Vector BiNormal.

// Entorn VGI: Variables del Timer
	double t;		// Paràmetre t pel Timer.
	bool anima;		// Booleana que controla si l'animació és activa (TRUE) o no (FALSE)
					//    dins la funció de control del rellotge OnTimer.

// Entorn VGI: Variables de l'objecte FRACTAL
	char t_fractal;		// Tipus de fractal.
	char soroll;		// Menú Fractals: Tipus de soroll
	int pas, pas_ini;	// Resolució del fractal inicial (pas_ini) i de visualització (pas).
	bool sw_il;			// Booleana que controla si cal compilar el fractal (sw_il=1) o no (sw_il=0)
	bool palcolFractal;	// Booleana que activa coloració del fractal segons paleta de colors [TRUE] o no [FALSE].

// Entorn VGI: Altres variables
	GLdouble mida;	// Factor d'escala per calcular Volum de Visualització de l'objecte que encaixi.
	CString nom;	// Nom de fitxer.
	CString buffer; // Buffer que magatzema string caracters corresponent a variables double a printar en Status Bar (funció Barra_Estat).
//-------------- Entorn VGI: Fi De Variables globals de CEntornVGIView

// Operaciones
public:

// Reemplazos
public:
	virtual void OnDraw(CDC* pDC);  // Reemplazado para dibujar esta vista
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Funcions de càrrega i activació de shaders
	void InitAPI();
	void GetGLVersion(int* major, int* minor);
	void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
		const GLchar* message, const void* userParam);
	void OnInitialUpdate();

// Implementación
public:
	virtual ~CEntornVGIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void CEntornVGIView::configura_Escena();
	void CEntornVGIView::dibuixa_Escena();
	void CEntornVGIView::Barra_Estat();
	void CEntornVGIView::double2CString(double varf);
	int CEntornVGIView::Log2(int num);							// Log2: Càlcul del log base 2 de num
	char * CEntornVGIView::CString2Char(CString entrada);		// Conversió string CString --> char *
	void CEntornVGIView::Refl_MaterialOff();					// Desactivar Reflexió de Material
	void CEntornVGIView::Refl_MaterialOn();						// Activar Reflexió de Material
	int CEntornVGIView::llegir_ptsC(char *nomf);				// Lectura Punts de Control Corba (B-spline o Bezier)
	bool CEntornVGIView::llegir_FontLlum(char* nomf);			// Lectura Paràmetres Font de Llum
	std::string CEntornVGIView::CString2String(const CString& cString); // Conversió CString --> std::string

// Entorn VGI: Funcions de càrrega i activació de shaders
	GLuint CEntornVGIView::initializeShaders(CString filename);
	GLuint CEntornVGIView::loadFileShaders(CString file_Vert, CString file_Frag);
	char *CEntornVGIView::textFileRead(char *fn);
	void CEntornVGIView::releaseAllShaders();

// // Entorn VGI: Funcions de tractament de teclat en diferents modus
	void CEntornVGIView::Teclat_ColorObjecte(UINT nChar, UINT nRepCnt);
	void CEntornVGIView::Teclat_ColorFons(UINT nChar, UINT nRepCnt);
	void CEntornVGIView::Teclat_Navega(UINT nChar, UINT nRepCnt);
	void CEntornVGIView::Teclat_Pan(UINT nChar, UINT nRepCnt);
	void CEntornVGIView::Teclat_TransEscala(UINT nChar, UINT nRepCnt);
	void CEntornVGIView::Teclat_TransRota(UINT nChar, UINT nRepCnt);
	void CEntornVGIView::Teclat_TransTraslada(UINT nChar, UINT nRepCnt);
	void CEntornVGIView::Teclat_Grid(UINT nChar, UINT nRepCnt);
	void CEntornVGIView::Teclat_PasCorbes(UINT nChar, UINT nRepCnt);

	CMFCStatusBar& GetStatusBar() const
	{
		return ((CMainFrame*)AfxGetMainWnd())->GetStatusBar();
	}

// Variables Full Screen
protected:
	CWnd *saveParent;
	CMenu *ContextMenu;

private:

//Rendering Context and Device Context Pointers
	HGLRC     m_hRC;
	CDC*      m_pDC;

// Funciones de asignación de mensajes generadas
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnArxiuObrirFractal();
	afx_msg void OnArxiuObrirFitxerObj();
	afx_msg void OnArxiuObrirFitxer3ds();
	afx_msg void OnVistaMobil();
	afx_msg void OnUpdateVistaMobil(CCmdUI *pCmdUI);
	afx_msg void OnVistaZoom();
	afx_msg void OnUpdateVistaZoom(CCmdUI *pCmdUI);
	afx_msg void OnVistaPolarsX();
	afx_msg void OnUpdateVistaPolarsX(CCmdUI *pCmdUI);
	afx_msg void OnVistaPolarsY();
	afx_msg void OnUpdateVistaPolarsY(CCmdUI *pCmdUI);
	afx_msg void OnVistaPolarsZ();
	afx_msg void OnUpdateVistaPolarsZ(CCmdUI *pCmdUI);
	afx_msg void OnVistaPan();
	afx_msg void OnUpdateVistaPan(CCmdUI *pCmdUI);
	afx_msg void OnVistaOrigenpan();
	afx_msg void OnVistaNavega();
	afx_msg void OnUpdateVistaNavega(CCmdUI *pCmdUI);
	afx_msg void OnVistaOrigennavega();
	afx_msg void OnVistaEixos();
	afx_msg void OnUpdateVistaEixos(CCmdUI *pCmdUI);
	afx_msg void OnProjeccioPerspectiva();
	afx_msg void OnUpdateProjeccioPerspectiva(CCmdUI *pCmdUI);
	afx_msg void OnObjecteCub();
	afx_msg void OnUpdateObjecteCub(CCmdUI *pCmdUI);
	afx_msg void OnObjecteEsfera();
	afx_msg void OnUpdateObjecteEsfera(CCmdUI *pCmdUI);
	afx_msg void OnObjecteTetera();
	afx_msg void OnUpdateObjecteTetera(CCmdUI *pCmdUI);
	afx_msg void OnTransformaTraslacio();
	afx_msg void OnUpdateTransformaTraslacio(CCmdUI *pCmdUI);
	afx_msg void OnTransformaOrigentraslacio();
	afx_msg void OnTransformaRotacio();
	afx_msg void OnUpdateTransformaRotacio(CCmdUI *pCmdUI);
	afx_msg void OnTransformaOrigenrotacio();
	afx_msg void OnTransformaEscalat();
	afx_msg void OnUpdateTransformaEscalat(CCmdUI *pCmdUI);
	afx_msg void OnTransformaOrigenescalat();
	afx_msg void OnTransformaMobilx();
	afx_msg void OnUpdateTransformaMobilx(CCmdUI *pCmdUI);
	afx_msg void OnTransformaMobily();
	afx_msg void OnUpdateTransformaMobily(CCmdUI *pCmdUI);
	afx_msg void OnTransformaMobilz();
	afx_msg void OnUpdateTransformaMobilz(CCmdUI *pCmdUI);
	afx_msg void OnOcultacionsTestvis();
	afx_msg void OnUpdateOcultacionsTestvis(CCmdUI *pCmdUI);
	afx_msg void OnOcultacionsZbuffer();
	afx_msg void OnUpdateOcultacionsZbuffer(CCmdUI *pCmdUI);
	afx_msg void OnOcultacionsBackline();
	afx_msg void OnUpdateOcultacionsBackline(CCmdUI *pCmdUI);
	afx_msg void OnIluminacioLlumfixe();
	afx_msg void OnUpdateIluminacioLlumfixe(CCmdUI *pCmdUI);
	afx_msg void OnIluminacioFilferros();
	afx_msg void OnUpdateIluminacioFilferros(CCmdUI *pCmdUI);
	afx_msg void OnIluminacioPlana();
	afx_msg void OnUpdateIluminacioPlana(CCmdUI *pCmdUI);
	afx_msg void OnIluminacioGouraud();
	afx_msg void OnUpdateIluminacioGouraud(CCmdUI *pCmdUI);
	afx_msg void OnMaterialEmissio();
	afx_msg void OnUpdateMaterialEmissio(CCmdUI *pCmdUI);
	afx_msg void OnMaterialAmbient();
	afx_msg void OnUpdateMaterialAmbient(CCmdUI *pCmdUI);
	afx_msg void OnMaterialDifusa();
	afx_msg void OnUpdateMaterialDifusa(CCmdUI *pCmdUI);
	afx_msg void OnMaterialEspecular();
	afx_msg void OnUpdateMaterialEspecular(CCmdUI *pCmdUI);
	afx_msg void OnIluminacioTextures();
	afx_msg void OnUpdateIluminacioTextures(CCmdUI *pCmdUI);
	afx_msg void OnIluminacioTexturaFitxerimatge();
	afx_msg void OnUpdateIluminacioTexturaFitxerimatge(CCmdUI *pCmdUI);
	afx_msg void OnLlumsLlumambient();
	afx_msg void OnUpdateLlumsLlumambient(CCmdUI *pCmdUI);
	afx_msg void OnLlumsLlum0();
	afx_msg void OnUpdateLlumsLlum0(CCmdUI *pCmdUI);
//	afx_msg void OnShadersSense();
//	afx_msg void OnUpdateShadersSense(CCmdUI *pCmdUI);
//	afx_msg void OnShadersGouraud();
//	afx_msg void OnUpdateShadersGouraud(CCmdUI *pCmdUI);
//	afx_msg void OnShadersPhong();
//	afx_msg void OnUpdateShadersPhong(CCmdUI *pCmdUI);
	afx_msg void OnVistaSatelit();
	afx_msg void OnUpdateVistaSatelit(CCmdUI *pCmdUI);
	afx_msg void OnVistaFullscreen();
	afx_msg void OnUpdateVistaFullscreen(CCmdUI *pCmdUI);
//	afx_msg void OnObjecteCubRGB();
//	afx_msg void OnUpdateObjecteCubRGB(CCmdUI *pCmdUI);
	afx_msg void OnObjecteTie();
	afx_msg void OnUpdateObjecteTie(CCmdUI *pCmdUI);
	afx_msg void OnIluminacioPunts();
	afx_msg void OnUpdateIluminacioPunts(CCmdUI *pCmdUI);
	afx_msg void OnShaderLoadFiles();
	afx_msg void OnUpdateShaderLoadFiles(CCmdUI *pCmdUI);
	afx_msg void OnOcultacionsFrontFaces();
	afx_msg void OnUpdateOcultacionsFrontFaces(CCmdUI *pCmdUI);
	afx_msg void OnLlumsLlum1();
	afx_msg void OnUpdateLlumsLlum1(CCmdUI *pCmdUI);
	afx_msg void OnLlumsLlum2();
	afx_msg void OnUpdateLlumsLlum2(CCmdUI *pCmdUI);
	afx_msg void OnLlumsLlum3();
	afx_msg void OnUpdateLlumsLlum3(CCmdUI *pCmdUI);
	afx_msg void OnLlumsLlum4();
	afx_msg void OnUpdateLlumsLlum4(CCmdUI *pCmdUI);
	afx_msg void OnObjeteCorbaBezier();
	afx_msg void OnUpdateObjeteCorbaBezier(CCmdUI *pCmdUI);
	afx_msg void OnObjecteCorbaLemniscata();
	afx_msg void OnUpdateObjecteCorbaLemniscata(CCmdUI *pCmdUI);
	afx_msg void OnObjecteCorbaBSpline();
	afx_msg void OnUpdateObjecteCorbaBSpline(CCmdUI *pCmdUI);
	afx_msg void OnObjectePuntsControl();
	afx_msg void OnUpdateObjectePuntsControl(CCmdUI *pCmdUI);
	afx_msg void OnVistaGridXY();
	afx_msg void OnUpdateVistaGridXY(CCmdUI *pCmdUI);
	afx_msg void OnVistaGridXZ();
	afx_msg void OnUpdateVistaGridXZ(CCmdUI *pCmdUI);
	afx_msg void OnVistaGridYZ();
	afx_msg void OnUpdateVistaGridYZ(CCmdUI *pCmdUI);
	afx_msg void OnVistaGridXYZ();
	afx_msg void OnUpdateVistaGridXYZ(CCmdUI *pCmdUI);
	afx_msg void OnIluminacio2Sides();
	afx_msg void OnUpdateIluminacio2Sides(CCmdUI *pCmdUI);
//	afx_msg void OnObjecteTexteBITMAP();
//	afx_msg void OnUpdateObjecteTexteBitmap(CCmdUI* pCmdUI);
//	afx_msg void OnObjecteTexteSTROKE();
//	afx_msg void OnUpdateObjecteTexteSTROKE(CCmdUI* pCmdUI);
	afx_msg void OnObjecteMatriuPrimitives();
	afx_msg void OnUpdateObjecteMatriuPrimitives(CCmdUI* pCmdUI);
	afx_msg void OnObjecteMatriuPrimitivesVBO();
	afx_msg void OnUpdateObjecteMatriuPrimitivesVBO(CCmdUI* pCmdUI);
	afx_msg void OnVistaSkyBox();
	afx_msg void OnUpdateVistaSkyBox(CCmdUI* pCmdUI);
	afx_msg void OnIluminacioPhong();
	afx_msg void OnUpdateIluminacioPhong(CCmdUI* pCmdUI);
	afx_msg void OnObjecteArc();
	afx_msg void OnUpdateObjecteArc(CCmdUI* pCmdUI);
	afx_msg void OnCameraEsferica();
	afx_msg void OnUpdateCameraEsferica(CCmdUI* pCmdUI);
	afx_msg void OnVistaZoomOrto();
	afx_msg void OnUpdateVistaZoomOrto(CCmdUI* pCmdUI);
	afx_msg void OnCameraGeode();
	afx_msg void OnUpdateCameraGeode(CCmdUI* pCmdUI);
	afx_msg void OnCameraOrigenGeode();
	afx_msg void OnCorbesTriedreFrenet();
	afx_msg void OnUpdateCorbesTriedreFrenet(CCmdUI* pCmdUI);
	afx_msg void OnObjecteCubRGB();
	afx_msg void OnUpdateObjecteCubRGB(CCmdUI* pCmdUI);
	afx_msg void OnIluminacioTexturaFlagInvertY();
	afx_msg void OnUpdateIluminacioTexturaFlagInvertY(CCmdUI* pCmdUI);
	afx_msg void OnMaterialReflmaterial();
	afx_msg void OnUpdateMaterialReflMaterial(CCmdUI* pCmdUI);
	afx_msg void OnArxiuObrirFitxerFontLlum();
	afx_msg void OnObjecteCap();
	afx_msg void OnUpdateObjecteCap(CCmdUI* pCmdUI);
	afx_msg void OnObjecteSol();
	afx_msg void OnUpdateObjecteSol(CCmdUI* pCmdUI);

	afx_msg void initSS();
	afx_msg void OnObjecteSistemasolar();
	afx_msg void OnUpdateObjecteSistemasolar(CCmdUI* pCmdUI);
	afx_msg void OnSistemasolarRota();
	afx_msg void OnUpdateSistemasolarRota(CCmdUI* pCmdUI);
	afx_msg void OnSistemasolarDibuixa();
	afx_msg void OnUpdateSistemasolarDibuixa(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // Versión de depuración en EntornVGIView.cpp
inline CEntornVGIDoc* CEntornVGIView::GetDocument() const
   { return reinterpret_cast<CEntornVGIDoc*>(m_pDocument); }
#endif

