//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb interfície MFC, llibreries GLM i OpenGL 3.3+
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2022)
// Obj3DS_VBO.cpp: implementation of the Obj_3DS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Obj3DS.h"
#include <stack>
//#include <il\ilut.h>		// Header File For OpenIL

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
// Constructor de la classe
//
Obj_3DS::Obj_3DS()
{
		
	g_3DModel.numOfObjects=0;
	g_3DModel.numOfMaterials=0;

	for (int i=0;i<MAX_TEXTURES;i++)
		g_Texture[i]=-1;

	fet3DS=false;

}

//
// Funció d'inicialització de la classe Obj_3DS
//
void Obj_3DS::Inicialitzar()
{
	g_3DModel.numOfObjects=0;
	g_3DModel.numOfMaterials=0;

	for (int i=0;i<MAX_TEXTURES;i++)
		g_Texture[i]=-1;
	
	fet3DS=false;
}

//
// Funció d'alliberament de memória de la classe Obj_3DS
//

void Obj_3DS::EliminarMemoria()
{
	g_3DModel.pMaterials.clear();
	fitxerstextures.clear();

	for(int i = 0; i < g_3DModel.numOfObjects; i++)
	{
// Alliberem les cares, normals, vertex i coordenades de textures
		delete [] g_3DModel.pObject[i].pFaces;
		delete [] g_3DModel.pObject[i].pNormals;
		delete [] g_3DModel.pObject[i].pVerts;
		delete [] g_3DModel.pObject[i].pTexVerts;
	}
	
	g_3DModel.numOfObjects=0;
	g_3DModel.numOfMaterials=0;

	for (int i=0;i<MAX_TEXTURES;i++){
		if (g_Texture[i]!=-1){
			const unsigned int *a=&g_Texture[i];
			glDeleteTextures(1,a);
		}
	}
	for (int i=0;i<MAX_TEXTURES;i++)
		g_Texture[i]=-1;
	fet3DS=false;

}

//
// Destructor
//

Obj_3DS::~Obj_3DS()
{
	EliminarMemoria();
}

//
// Carrega un fitxer en format 3DS
//
//bool Obj_3DS::Carregar3DS(CString nomfitxer)
bool Obj_3DS::Carregar3DS(char *nomfitxer)
{
//	strcpy(titol,nom);
	CLoad3DS g_Load3ds;
	char szBasePath[255];	  // Path were all paths in the OBJ start
	char szLibraryFile[255];

// Primer necessitem carregar el fitxer 3DS. Li passem l'adreça de la
// nostre estructura t3DModel i el nom del fitxer. 
	if (!g_Load3ds.Import3DS(&g_3DModel, nomfitxer))			// Load our .3DS file into our model structure
		return false;

	// Conversió variable w_char --> char *
	//char *nomf = reinterpret_cast<char *> (nomfitxer.GetBuffer(3));

// Get base path
    strcpy_s(szBasePath, nomfitxer); // Copiar path sencer amb nom de fitxer.
//	strcpy_s(szBasePath, nomf); // Copiar path sencer amb nom de fitxer.
	MakePath(szBasePath);			 // Extreure path (szBasePath)

// Per a tots els materials
	for(int i = 0; i < g_3DModel.numOfMaterials; i++)
	{
		// Comprovem si hi ha un fitxer per carregar per aquest material
		if(strlen(g_3DModel.pMaterials[i].strFile) > 0)
		{

// Afegir base path al fitxer de textura
			strcpy_s(szLibraryFile, szBasePath);						// Copiar el path a szLibraryFile
			strcat_s(szLibraryFile, g_3DModel.pMaterials[i].strFile);	// Afegir al path el nom del fitxer textura
			strcpy_s(g_3DModel.pMaterials[i].strFile, szLibraryFile);	// Copiar a l'estructura el path + nom del fitxer textura

// Utilitzem el nom de la textura per carregar el bitmap
// i ens retorna el seu ID de textura que l'afegim al array de ID's de  textures
			//g_Texture[i]=CreateTexture(g_3DModel.pMaterials[i].strFile);
			wchar_t *nomf = reinterpret_cast <wchar_t *> (g_3DModel.pMaterials[i].strFile);
//			g_Texture[i] = ilutGLLoadImage(g_3DModel.pMaterials[i].strFile);	// Load The Images
			g_Texture[i] = ilutGLLoadImage(nomf);	// Load The Images

/*
			char *nomf = reinterpret_cast <char *> (g_3DModel.pMaterials[i].strFile);
			// Textures SOIL
			g_Texture[i] = SOIL_load_OGL_texture
				(	nomf,
					SOIL_LOAD_AUTO,
					SOIL_CREATE_NEW_ID,
					SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
					);
*/
			struct nomfitxer a;
			a.num=i;
            strcpy_s(a.nom,g_3DModel.pMaterials[i].strFile);
			fitxerstextures.push_back(a);
			if (g_Texture[i]==NULL)return false;

		}
		// Establim el ID de textura per aquest material
		g_3DModel.pMaterials[i].texureId = i;
	}
	
	fet3DS=true;
	return true;
}

//
// Funció que dibuixa l'pbjecte 3DS. Si el parametre es false crea una llista nova.
//
CVAO Obj_3DS::Dibuixa3DS(bool actualitza,int prim_Id)
{
	
// VAO
	GLuint vaoId = 0;		GLuint vboId = 0;

	CVAO objVAO;

	objVAO.vaoId = 0;	objVAO.vboId = 0;	objVAO.nVertexs = 0;

	std::vector <double> vertices, colors, normals, textures;		// Definició vectors dinàmics per a vertexs i colors 
	vertices.resize(0);		colors.resize(0);	normals.resize(0);		textures.resize(0);// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	GLfloat cColor[4];
	glGetFloatv(GL_CURRENT_COLOR, cColor);

	//if (actualitza){glCallList(obj);}
	//else { //glNewList(obj, GL_COMPILE);				
		// Procesem cada objecte del nostre model
		for(int i = 0; i < g_3DModel.numOfObjects; i++)
		{
			// Ens assegurem que son objectes vàlids
			if(g_3DModel.pObject.size() <= 0) break;

			// Agafem l'objecte actual
			t3DObject *pObject = &g_3DModel.pObject[i];
				
			// Check to see if this object has a texture map, if so bind the texture to it.
			// Si l'objecte te textura, n'hi fiquem una
			if (pObject->bHasTexture){

				// Activem el texture mapping i desactivarem el color
				glEnable(GL_TEXTURE_2D);

				// Resetejem el color (blanc)
//				glColor3ub(255, 255, 255);

				// Vinculem la textura al objecte per el seu ID de material
				glBindTexture(GL_TEXTURE_2D, g_Texture[pObject->materialID]);

			} else {

				// Desactivem el texture mapping i activarem el color
				glDisable(GL_TEXTURE_2D);

				// Resetejem el color (blanc)
//				glColor3ub(255, 255, 255);
			}

/*			// Dibuixarem triangles
			if (ilumin == PUNTS) glBegin(GL_POINTS);
				else glBegin(GL_TRIANGLES);	
*/				

				// Per a totes les cares del objecte
				for(int j = 0; j < pObject->numOfFaces; j++)
				{
					// Per a cada vertex del triangle
					for(int whichVertex = 0; whichVertex < 3; whichVertex++)
					{
						// Agafem l'index per a cada vertex de la cara
						unsigned int index = pObject->pFaces[j].vertIndex[whichVertex];
				
						// Li passem a OpenGL  la normal per aquest vertex
						//glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
						normals.push_back(pObject->pNormals[index].x);
						normals.push_back(pObject->pNormals[index].y);
						normals.push_back(pObject->pNormals[index].z);

						// Si te textura, li passem a OpenGL la textura per aquest vertex
						if(pObject->bHasTexture) {
							// Ens asegurem que hi ha un UVW map aplicat a l'objecte o sinó aquest no te les coordinades de textura
							if(pObject->pTexVerts) {
								//glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
								textures.push_back(pObject->pTexVerts[index].x);
								textures.push_back(pObject->pTexVerts[index].y);
								}
								else {	textures.push_back(0.0);
										textures.push_back(0.0);
									}
							colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
						} else {
							textures.push_back(0.0);	textures.push_back(0.0); // Vector Textures

							// Ens assegurem que hi ha un color/material vàlid assignat al objecte
							// Sempre hem d'assignar com a mínim un material/color a un objecte
							// pero en el cas de nomès comprovar el tamany de la llista de materials
							// si el tamany és al menys 1, i el ID de material és diferent de -1
							// llavors tenim un material vàlid
							if(g_3DModel.pMaterials.size() && pObject->materialID >= 0) 
							{
								//  Agafem el color que te l'objecte
								BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;

								// Assignem el color que hem agafat a l'objecte
								//glColor3ub(pColor[0], pColor[1], pColor[2]);
								colors.push_back(pColor[0]);	colors.push_back(pColor[1]);	colors.push_back(pColor[2]);	colors.push_back(1.0);
							}
							else {
								colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);  // Vector Colors
							}
						}

						// Passem el vertex actual del objecte
						//glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
						vertices.push_back(pObject->pVerts[index].x);
						vertices.push_back(pObject->pVerts[index].y);
						vertices.push_back(pObject->pVerts[index].z);
					}
				}

			// Final del dibuix
			//glEnd();
// Dibuixarem triangles
			//glBegin(GL_TRIANGLES);
/* ---------------------- - VBO
			std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
			if (nv != 0) {
				draw_GL_TRIANGLES_VAO(vertices, normals, colors, textures);
				vertices.resize(0);		colors.resize(0);	normals.resize(0);		textures.resize(0);// Reinicialitzar vectors
				}
*/
//		}
		//glEndList();
	}

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
	objVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	objVAO.nVertexs = (int)nv / 3;
	Set_VAOList(prim_Id, objVAO);

	//Set_vaoId(prim_Id, vaoId);
	//Set_vboId(prim_Id, vboId);
	//int nvert = (int)nv / 3;
	//Set_nVertexs(prim_Id, nvert);

	return objVAO;
}


void Obj_3DS::MakePath(char szFileAndPath[])
{
	////////////////////////////////////////////////////////////////////////
	// Rips the filenames out of a path and adds a slash (if needed)
	////////////////////////////////////////////////////////////////////////
	
	// Get string length
	int iNumChars = (int)strlen(szFileAndPath);

	// Loop from the last to the first char
	for (int iCurChar=iNumChars-1; iCurChar>=0; iCurChar--)
	{
		// If the current char is a slash / backslash
		if (szFileAndPath[iCurChar] == char('\\') ||
			szFileAndPath[iCurChar] == char('/'))
		{
			// Terminate the the string behind the slash / backslash
			szFileAndPath[iCurChar + 1] = char('\0');
			return;
		}
	}

	// No slash there, set string length to zero
	szFileAndPath[0] = char('\0');
}