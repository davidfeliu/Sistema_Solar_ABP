//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb interfície MFC, llibreries GLM i OpenGL 3.3+
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2022)
// glut_geometry.cpp : Codi font de les primitives gràfiques freeglut i GLU adaptades a VS2022.
// FUNCIONS:		- Primitives sòlides (glutSolid<primitiva>)
//					- Primitives filferros (glutWire<primitiva>)
//					- Primitives punts (glutPoint<primitiva>)
//					- Interficie de primitives (glet<primitiva>)
//					- Primitives sòlides (gluCilindre, gluDisc, GluParcialDisc, GluEsfera)
//	  Versió 2.8:	- Adaptació primitives glut a VAO amb tres funcions separedes:
//						1) Càrrega geometria VAO a placa gràfica --> CVAO cubeVAO = loadglutSolidCube_VAO(1.0);
//																 --> Set_VAOList(GLUT_CUBE, cubeVAO);
//						2) Dibuix VAO tantes vegades com es vulgui --> draw_TriVAO_Object(GLUT_CUBE);
//						3) Eliminació VAO --> deleteVAOList(GLUT_CUBE)
//

/*
 * glut_geometry.cpp
 *
 * Codi adaptat de Freeglut geometry rendering methods.
 *
 * Copyright (c) 2017 Nosferatu Software. All Rights Reserved.
 * Adaptat per Enric Martí Gòdia, <enric.marti@uab.cat>
 * Creation date: Dimarts 9 Maig 2017
 * Update date: Dimarts 28 Novembre 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * PAWEL W. OLSZTA BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "stdafx.h"
#include "material.h"
#include "glut_geometry.h"

// VAOList: Vector d'identificadors de Vertex Buffer Objects per a primitives glut_geometry.
								// Cada posició del vector correspon a una primitiva:
								// 0: CUBE_SKYBOX, 1: GLUT_CUBE, 2:GLUT_CUBE_RGB, 3:GLUT_SPHERE, 4: GLUT_CONE, 
								// 5: GLUT_CYLINDER, 6: GLUT_TORUS, 7: GLUT_DODECAHEDRON, 8: GLUT_OCTAHEDRON, 
								// 9: GLUT_TETRAEDRON, 10: GLUT_ICOSAHEDRON, 11: GLUT_RHOMBICDODECAHEDRON,  
								// 12: GLUT_SIERPINSKISPONGE, 13: GLU_TEAPOT, 14: GLU_CYLINDER, 15: GLU_DISK, 
								// 16: GLU_PARTIALDISK, 17: GLU_SPHERE, 18: CRV_POLYLINE, 19: CRV_LEMNISCATA2D, 
								// 20: CRV_LEMNISCATA3D, 21: CRV_BSPLINE, 22: CRV_BEZIER, 23:SUP_BSPLINE, 24: SUP_BEZIER, 
								// 25: GLUT_LINES, 26: GLUT_TRIANGLES, 27: GLU_CILINDRE_SENCER, 28: GLUT_SQUARE, 
								// 29: MAR_FRACTAL_VAO, 30: O_FRACTAL_VAO,
								// 31: FIT_3DS, 32: FIT_3DS2, 33: FIT_OBJ, 34:FIT_OBJ2, 35:GLUT_USER1, 
								// 36: GLUT_USER2, 37: GLUT_USER3, 38: GLUT_USER4, 39: GLUT_USER5, 40: GLUT_USER6.
								// 41: GLUT_USER7, 42: GLUT_USER8, 43: GLUT_USER9, 44: GLUT_USER10.
CVAO VAOList[MAX_SIZE_VAOID];

// Color de primitiva
GLdouble cColor[4] = { 0.0, 0.0, 0.0, 1.0 };

/*
 * TODO BEFORE THE STABLE RELEASE:
 *
 * Following functions have been contributed by Andreas Umbach, updated by Enric Martí to VAO.
 *
 *      glutSolidCube()		    -- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 *											 EBO functions separated (load*_EBO(), draw_TriEBO_Object(GL*), deleteVAOList(GL*)
 *      glutSolidCubeRGB()		-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 * *										 EBO functions separated (load*_EBO(), draw_TriEBO_Object(GL*), deleteVAOList(GL*)
 *
 *  The Following functions have been updated by Nigel Stewart, based
 *  on FreeGLUT 2.0.0 implementations, updated by Enric Martí to VAO:
 *
 *      glutSolidSphere()       -- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 * *										 EBO functions separated (load*_EBO(), draw_TriEBO_Object(GL*), deleteVAOList(GL*)
 *      glutSolidCone()			-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 * *										 EBO functions separated (load*_EBO(), draw_TriEBO_Object(GL*), deleteVAOList(GL*)
 *      glutSolidCylinder()		-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 * *										 EBO functions separated (load*_EBO(), draw_TriEBO_Object(GL*), deleteVAOList(GL*)
 *
 * Those functions have been implemented by John Fay, updated by Enric Martí to VAO..
 *
 *      glutSolidTorus()				-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 *      glutSolidDodecahedron()			-- looks OK
 *      glutSolidOctahedron()			-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 *      glutSolidTetrahedron()			-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 *      glutSolidIcosahedron()			-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 *      glutSolidRhombicDodecahedron()  -- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 *      glutSolidSierpinskiSponge()		-- looks OK
 *      glutSolidTeapot()				-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 *
 *  The Following functions have been updated by Enric Martí, based
 *  on GLU 9.0.0 implementations (include texture coordinates) to VAO and EBO:
 *
 *      gluCylinder()			-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 * *										 EBO functions separated (load*_EBO(), draw_TriEBO_Object(GL*), deleteVAOList(GL*)
 *      gluDisk()				-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 * *										 EBO functions separated (load*_EBO(), draw_TriEBO_Object(GL*), deleteVAOList(GL*)
 *      gluPartialDisk()		-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 * *										 EBO functions separated (load*_EBO(), draw_TriEBO_Object(GL*), deleteVAOList(GL*)
 *      gluSphere()				-- looks OK, VAO functions separated (load*_VAO(), draw_TriVAO_Object(GL*), deleteVAOList(GL*)
 * *										 EBO functions separated (load*_EBO(), draw_TriEBO_Object(GL*), deleteVAOList(GL*)
 *      draw_Lemniscata2D()		-- looks OK, VAO functions separated (load*_VAO(), draw*(), deleteVAOList(GL*)
 *      draw_Lemniscata3D()		-- looks OK, VAO functions separated (load*_VAO(), draw*(), deleteVAOList(GL*)
 *      draw_BSpline_Curve()	-- looks OK, VAO functions separated (load*_VAO(), draw*(), deleteVAOList(GL*)
 *      draw_Bezier_Curve()		-- looks OK, VAO functions separated (load*_VAO(), draw*(), deleteVAOList(GL*)
 */


// -- INTERFACE FUNCTIONS -------------------------------------------------- 

//--------------- SETS (o PUTS)
void SetColor4d(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)
{
	cColor[0] = red; cColor[1] = green; cColor[2] = blue; cColor[3] = alpha; 
}

void Set_VAOList(GLint k, CVAO auxVAO)
{
	VAOList[k].vaoId = auxVAO.vaoId;
	VAOList[k].vboId = auxVAO.vboId;
	VAOList[k].eboId = auxVAO.eboId;
	VAOList[k].nVertexs = auxVAO.nVertexs;
	VAOList[k].nIndices = auxVAO.nIndices;
}

void Set_vaoId(GLuint k, GLuint vaoId)
{
	VAOList[k].vaoId = vaoId;
}

void Set_vboId(GLuint k, GLuint vboId)
{
	VAOList[k].vboId = vboId;
}

void Set_eboId(GLuint k, GLuint eboId)
{
	VAOList[k].eboId = eboId;
}

void Set_nVertexs(GLuint k, GLint nvertexs)
{
	VAOList[k].nVertexs = nvertexs;
}

//------------------- GETS
CVAO Get_VAOList(GLint k)
{	CVAO auxVAO; 

	auxVAO.vaoId = VAOList[k].vaoId;
	auxVAO.vboId = VAOList[k].vboId;
	auxVAO.eboId = VAOList[k].eboId;
	auxVAO.nVertexs = VAOList[k].nVertexs;
	auxVAO.nIndices = VAOList[k].nIndices;
	
	return auxVAO;
}

GLuint Get_VAOId(GLint k)
{
	return VAOList[k].vaoId;
}


//--------------- INITS i DELETES
void initVAOList()
{	int i;

	for (i = 0; i < MAX_SIZE_VAOID; i++)
	{	VAOList[i].vaoId = 0;
		VAOList[i].vboId = 0;
		VAOList[i].eboId = 0;
		VAOList[i].nVertexs = 0;
		VAOList[i].nIndices = 0;
	}

}

void netejaVAOList()
{	int i;
	for (i = 1; i < MAX_SIZE_VAOID; i++)
	{	if (VAOList[i].vaoId != 0) deleteVAOList(i);
	}
}

void deleteVAOList(GLint k)
{
	//GLuint vaoId = VAOList[k].vaoId;
	//GLuint vboId = VAOList[k].vboId;

	if (VAOList[k].vaoId != 0)
	{
		glBindVertexArray(VAOList[k].vaoId); //glBindVertexArray(vaoId);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);

		// It is good idea to release VBOs with ID 0 after use.
		// Once bound with 0, all pointers in gl*Pointer() behave as real
		// pointer, so, normal vertex array operations are re-activated
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &VAOList[k].vboId);// glDeleteBuffers(1, &vboId);

		// Delete EBO
		glDeleteBuffers(1, &VAOList[k].eboId);// glDeleteBuffers(1, &eboId);

		// Unbind and delete VAO
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VAOList[k].vaoId);	// glDeleteVertexArrays(1, &vaoId);

		VAOList[k].vaoId = 0;
		VAOList[k].vboId = 0;
		VAOList[k].eboId = 0;
		VAOList[k].nVertexs = 0;
		VAOList[k].nIndices = 0;
	}
}


//--------------- DRAWS (DIBUIX DE VAO's i EBO's
void draw_TriVAO_Object(GLint k)
{
// Recuperar identificadors VAO i nvertexs dels vector VAOList pr a dibuixar VAO
	if (VAOList[k].vaoId != 0) {
		glBindVertexArray(VAOList[k].vaoId);
		//glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glDrawArrays(GL_TRIANGLES, 0, VAOList[k].nVertexs);
		glBindVertexArray(0);
	}
}

void draw_LinVAO_Object(GLint k)
{
// Recuperar identificador VAO a vector VAOId
	if (VAOList[k].vaoId != 0) {
		glBindVertexArray(VAOList[k].vaoId);
		glDrawArrays(GL_LINES, 0, VAOList[k].nVertexs);
		glBindVertexArray(0);
	}
}

void draw_TriEBO_Object(GLint k)
{
	// Recuperar identificadors VAO i nvertexs dels vector VAOList pr a dibuixar VAO
	if (VAOList[k].vaoId != 0) {
		glBindVertexArray(VAOList[k].vaoId);
		//glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAOList[k].eboId);
		//glDrawElements(GL_TRIANGLES, VAOList[k].nVertexs, GL_UNSIGNED_INT, nullptr);
		glDrawElements(GL_TRIANGLES, VAOList[k].nIndices, GL_UNSIGNED_INT, nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void draw_LinEBO_Object(GLint k)
{
	// Recuperar identificador VAO a vector VAOId
	if (VAOList[k].vaoId != 0) {
		glBindVertexArray(VAOList[k].vaoId);
		//glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAOList[k].eboId);
		glDrawElements(GL_LINES, VAOList[k].nIndices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}


// ----------CUBE ----------------------------------------------------------------

// Draws a solid cube. Code contributed by Andreas Umbach <marvin@dataway.ch>
void glutSolidCube(GLdouble dSize)
{
	Set_VAOList(GLUT_CUBE, loadglutSolidCube_EBO(dSize)); //glutSolidCube_VAO(dSize);
	draw_TriEBO_Object(GLUT_CUBE); //draw_TriVAO_Object(GLUT_CUBE); //drawSolidCube();
	deleteVAOList(GLUT_CUBE);
}

// Load a solid cube as a VAO. Code contributed by Andreas Umbach <marvin@dataway.ch> and Enric Marti <enric.marti@uab.cat>
CVAO loadglutSolidCube_VAO(GLdouble dSize)
{
	GLdouble size = dSize * 0.5f;
	CVAO auxVAO;
	auxVAO.vaoId = 0;	auxVAO.vboId = 0;	auxVAO.nVertexs = 0;

// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// Obtenir color actual definit en OpenGL amb glColor();
//	GLdouble cColor[4];
//	glGetDoublev(GL_CURRENT_COLOR, cColor);

// Vertex coords array for glDrawArrays() =====================================
// A cube has 6 sides and each side has 1 square, therefore, a cube consists
// of 24 vertices (6 sides * 1 square * 4 vertices = 24 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 72 floats (24 * 3 = 72). The size of each float is 4 bytes (72 * 4 = 288 bytes)
	//GLdouble vertices[] = { 
	std::vector<double> vertices = {
		 size,  size,  size, -size,  size,  size, -size, -size,  size,      // v0-v1-v2 (front)
		-size, -size,  size,  size, -size,  size,  size,  size,  size,      // v2-v3-v0
		 size,  size,  size,  size, -size,  size,  size, -size, -size,      // v0-v3-v4 (right)
		 size, -size, -size,  size,  size, -size,  size,  size,  size,      // v4-v5-v0
		 size,  size,  size,  size,  size, -size, -size,  size, -size,      // v0-v5-v6 (top)
		-size,  size, -size, -size,  size,  size,  size,  size,  size,      // v6-v1-v0
		-size,  size,  size, -size,  size, -size, -size, -size, -size,      // v1-v6-v7 (left)
		-size, -size, -size, -size, -size,  size, -size,  size,  size,      // v7-v2-v1
		-size, -size, -size,  size, -size, -size,  size, -size,  size,      // v7-v4-v3 (bottom)
		 size, -size,  size, -size, -size,  size, -size, -size, -size,      // v3-v2-v7
		 size, -size, -size, -size, -size, -size, -size,  size, -size,      // v4-v7-v6 (back)
		-size,  size, -size,  size,  size, -size,  size, -size, -size };	// v6-v5-v4

// Normal array
	//GLdouble normals[] = {
	std::vector<double> normals = {
		0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,   // v0-v1-v2 (front)
		0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,   // v2-v3-v0
		1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,   // v0-v3-v4 (right)
		1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,   // v4-v5-v0
		0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,   // v0-v5-v6 (top)
		0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,   // v6-v1-v0
	   -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,   // v1-v6-v7 (left)
	   -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,   // v7-v2-v1
		0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,   // v7-v4-v3 (bottom)
		0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,   // v3-v2-v7
		0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,   // v4-v7-v6 (back)
		0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0 }; // v6-v5-v4

// Color array
	//GLdouble colors[] = {
	std::vector<double> colors = {
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v0-v1-v2 (front)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v2-v3-v0 
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v0-v3-v4 (right)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v4-v5-v0
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v0-v5-v6 (top)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v6-v1-v0
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v1-v6-v7 (left)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v7-v2-v1
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v7-v4-v3 (bottom)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v3-v2-v7
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],   // v4-v7-v6 (back)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3] }; // v6-v5-v4

// Texture Array
	//GLdouble textures[] = {
	std::vector<double> textures = {
		1.0, 1.0, 1.0, 0.0, 0.0, 0.0,      // v0-v1-v2 (front)
		0.0, 0.0, 0.0, 1.0, 1.0, 1.0,      // v2-v3-v0
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0,      // v0-v3-v4 (right)
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0,      // v4-v5-v0
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0,      // v0-v5-v6 (top)
		1.0, 0.0, 1.0, 1.0, 0.0, 1.0,      // v6-v1-v0
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0,      // v1-v6-v7 (left)
		1.0, 0.0, 1.0, 1.0, 0.0, 1.0,      // v7-v2-v1
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0,      // v7-v4-v3 (bottom)
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0,      // v3-v2-v7
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0,      // v4-v7-v6 (back)
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0 };    // v6-v5-v4

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	auxVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//auxVAO.nVertexs = 36;

	return auxVAO;
}


// Load a solid cube as a EBO. Code contributed by Andreas Umbach <marvin@dataway.ch> and Enric Marti <enric.marti@uab.cat>
CVAO loadglutSolidCube_EBO(GLdouble dSize)
{
	GLdouble size = dSize * 0.5f;
	CVAO auxVAO;
	auxVAO.vaoId = 0;	auxVAO.vboId = 0;	auxVAO.eboId = 0; auxVAO.nVertexs = 0;	auxVAO.nIndices = 0;

// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// Obtenir color actual definit en OpenGL amb glColor();
//	GLdouble cColor[4];
//	glGetDoublev(GL_CURRENT_COLOR, cColor);

// Vertex coords array for glDrawArrays() =====================================
// A cube has 6 sides and each side has 1 square, therefore, a cube consists
// of 24 vertices (6 sides * 1 square * 4 vertices = 24 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 72 floats (24 * 3 = 72). The size of each float is 4 bytes (72 * 4 = 288 bytes)
	//GLdouble vertices[] = { 
	std::vector<double> vertices = {															//				Y-Axis - Z-Axis
		 size,  size,  size, -size,  size,  size, -size, -size,  size,  size, -size,  size,		// v0-v1-v2-v3 (front) - (top)
		 size,  size,  size,  size, -size,  size,  size, -size, -size,  size,  size, -size,		// v0-v3-v4-v5 (right) - (front)
		 size,  size,  size,  size,  size, -size, -size,  size, -size,  -size,  size,  size,	// v0-v5-v6-v1 (top) - (right)
		-size,  size,  size, -size,  size, -size, -size, -size, -size,  -size, -size,  size,	// v1-v6-v7-v2 (left) - (back)
		-size, -size, -size,  size, -size, -size,  size, -size,  size,  -size, -size,  size,	// v7-v4-v3-v2 (bottom) - (left)
		 size, -size, -size, -size, -size, -size, -size,  size, -size,  size,  size, -size };	// v4-v7-v6-v5 (back) - (bottom)

// Normal array
	//GLdouble normals[] = {
	std::vector<double> normals = {													//				Y-Axis - Z-Axis
		0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,		// v0-v1-v2-v3 (front) - (top)
		1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,		// v0-v3-v4-v5 (right) - (front)
		0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,		// v0-v5-v6-v1 (top) - (right)
	   -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,		// v1-v6-v7-v2 (left) - (back)
		0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,		// v7-v4-v3-v2 (bottom) - (left)
		0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0 };	// v4-v7-v6-v5 (back) - (bottom)

// Color array
	//GLdouble colors[] = {
	std::vector<double> colors = {
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], // v0-v1-v2-v3 (front)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],	// v0-v3-v4-v5 (right)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],	// v0-v5-v6-v1 (top)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],	// v1-v6-v7-v2 (left)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],	// v7-v4-v3-v2 (bottom)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3] };	// v4-v7-v6-v5 (back)

/* Texture Array Y Axis
	//GLdouble textures[] = {
	std::vector<double> textures = {
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,     // v0-v1-v2-v3 (front)
		1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,     // v0-v3-v4-v5 (right)
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,     // v0-v5-v6-v1 (top)
		0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0,     // v1-v6-v7-v2 (left)
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,     // v7-v4-v3-v2 (bottom)
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 };   // v4-v7-v6-v5 (back)

// Texture Array 3D Texture (paret)
//GLdouble textures[] = {
	std::vector<double> textures = {
		1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,     // v0-v1-v2-v3 (top)
		1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0,     // v0-v3-v4-v5 (front)
		1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0,     // v0-v5-v6-v1 (right)
		1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,		// v1-v6-v7-v2 (back)
		0.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0,     // v7-v4-v3-v2 (left)
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0 };   // v4-v7-v6-v5 (bottom)
*/

// Texture Array Z-Axis
//GLdouble textures[] = {
	std::vector<double> textures = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,     // v0-v1-v2-v3 (top)
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,     // v0-v3-v4-v5 (front)
		1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,     // v0-v5-v6-v1 (right)
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0,		// v1-v6-v7-v2 (back)
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,     // v7-v4-v3-v2 (left)
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0};    // v4-v7-v6-v5 (bottom)

// Indices Array
	//GLuint indices[] = {
	std::vector<uint> indices = {
		0, 1, 2, 2, 3, 0,			// v0-v1-v2-v3 (front)
		4, 5, 6, 6, 7, 4,			// v0-v3-v4-v5 (right)
		8, 9, 10, 10, 11, 8,		// v0-v5-v6-v1 (top)
		12, 13, 14, 14, 15, 12,		// v1-v6-v7-v2 (left)
		16, 17, 18, 18, 19, 16,		// v7-v4-v3-v2 (bottom)
		20, 21, 22, 22, 23, 20 };	// v4-v7-v6-v5 (back)

// Creació d'un VAO, VBO i EBO carregant-hi la geometria. Guardar identificadors VAO, VBO, EBO, vertex i indexs a struct CVAO.
	auxVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);

	return auxVAO;
}
// ---------- FI CUBE ------------------------------------------------------------

// ---------- CUBE RGB------------------------------------------------------------

// Draws a solid cube with RGB color model. Code contributed by Andreas Umbach <marvin@dataway.ch>
void glutSolidCubeRGB(GLdouble dSize)
{
	Set_VAOList(GLUT_CUBE_RGB, loadglutSolidCubeRGB_EBO(dSize)); //glutSolidCube_VAO(dSize);
	draw_TriEBO_Object(GLUT_CUBE_RGB); //draw_TriVAO_Object(GLUT_CUBE_RGB); //drawSolidCube();
	deleteVAOList(GLUT_CUBE_RGB);
}

// Draws a solid cube with RGB color model. Code contributed by Andreas Umbach <marvin@dataway.ch> and Enric Marti <enric.marti@uab.cat>
CVAO loadglutSolidCubeRGB_VAO(GLdouble dSize)
{
	GLdouble size = dSize * 0.5f;
	CVAO auxVAO;
	auxVAO.vaoId = 0;	auxVAO.vboId = 0;	auxVAO.nVertexs = 0;


// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// vertex coords array for glDrawArrays() =====================================
// A cube has 6 sides and each side has 2 triangles, therefore, a cube consists
// of 36 vertices (6 sides * 2 tris * 3 vertices = 36 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 108 floats (36 * 3 = 108).
	//GLdouble vertices[] = {
	std::vector<double> vertices = { 
		 size,  size,  size, -size,  size,  size, -size, -size,  size,      // v0-v1-v2 (front)
		-size, -size,  size,  size, -size,  size,  size,  size,  size,      // v2-v3-v0
		 size,  size,  size,  size, -size,  size,  size, -size, -size,      // v0-v3-v4 (right)
		 size, -size, -size,  size,  size, -size,  size,  size,  size,      // v4-v5-v0
		 size,  size,  size,  size,  size, -size, -size,  size, -size,      // v0-v5-v6 (top)
		-size,  size, -size, -size,  size,  size,  size,  size,  size,      // v6-v1-v0
		-size,  size,  size, -size,  size, -size, -size, -size, -size,      // v1-v6-v7 (left)
		-size, -size, -size, -size, -size,  size, -size,  size,  size,      // v7-v2-v1
		-size, -size, -size,  size, -size, -size,  size, -size,  size,      // v7-v4-v3 (bottom)
		 size, -size,  size, -size, -size,  size, -size, -size, -size,      // v3-v2-v7
		 size, -size, -size, -size, -size, -size, -size,  size, -size,      // v4-v7-v6 (back)
		-size,  size, -size,  size,  size, -size,  size, -size, -size };	// v6-v5-v4

// Normal array
	//GLdouble normals[] = {
	std::vector<double> normals = {  
		0,  0,  1,  0,  0,  1,  0,  0,  1,   // v0-v1-v2 (front)
		0,  0,  1,  0,  0,  1,  0,  0,  1,   // v2-v3-v0
		1,  0,  0,  1,  0,  0,  1,  0,  0,   // v0-v3-v4 (right)
		1,  0,  0,  1,  0,  0,  1,  0,  0,   // v4-v5-v0
		0,  1,  0,  0,  1,  0,  0,  1,  0,   // v0-v5-v6 (top)
		0,  1,  0,  0,  1,  0,  0,  1,  0,   // v6-v1-v0
	   -1,  0,  0, -1,  0,  0, -1,  0,  0,   // v1-v6-v7 (left)
	   -1,  0,  0, -1,  0,  0, -1,  0,  0,   // v7-v2-v1
		0, -1,  0,  0, -1,  0,  0, -1,  0,   // v7-v4-v3 (bottom)
		0, -1,  0,  0, -1,  0,  0, -1,  0,   // v3-v2-v7
		0,  0, -1,  0,  0, -1,  0,  0, -1,   // v4-v7-v6 (back)
		0,  0, -1,  0,  0, -1,  0,  0, -1 }; // v6-v5-v4

// Color array
	//GLdouble colors[] = {
	std::vector<double> colors= { 
		1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1,   // v0-v1-v2 (front)
		0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1,   // v2-v3-v0
		1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1,   // v0-v3-v4 (right)
		1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1,   // v4-v5-v0
		1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1,   // v0-v5-v6 (top)
		0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1,   // v6-v1-v0
		0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1,   // v1-v6-v7 (left)
		0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1,   // v7-v2-v1
		0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1,   // v7-v4-v3 (bottom)
		1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1,   // v3-v2-v7
		1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1,   // v4-v7-v6 (back)
		0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1 }; // v6-v5-v4

// Texture Array
	//GLdouble textures[] = {
	std::vector<double> textures = { 
		1, 1, 1, 0, 0, 0,      // v0-v1-v2 (front)
		0, 0, 0, 1, 1, 1,      // v2-v3-v0
		1, 1, 0, 1, 0, 0,      // v0-v3-v4 (right)
		0, 0, 1, 0, 1, 1,      // v4-v5-v0
		0, 1, 0, 0, 1, 0,      // v0-v5-v6 (top)
		1, 0, 1, 1, 0, 1,      // v6-v1-v0
		0, 1, 0, 0, 1, 0,      // v1-v6-v7 (left)
		1, 0, 1, 1, 0, 1,      // v7-v2-v1
		0, 0, 1, 0, 1, 1,      // v7-v4-v3 (bottom)
		1, 1, 0, 1, 0, 0,      // v3-v2-v7
		0, 0, 1, 0, 1, 1,      // v4-v7-v6 (back)
		1, 1, 0, 1, 0, 0 };    // v6-v5-v4

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	auxVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//auxVAO.nVertexs = 36;

	return auxVAO;
}


//Draws a solid cube with RGB color model with EBO. Code contributed by Andreas Umbach <marvin@dataway.ch> and Enric Marti <enric.marti@uab.cat>
CVAO loadglutSolidCubeRGB_EBO(GLdouble dSize)
{
	GLdouble size = dSize * 0.5f;
	CVAO auxVAO;
	auxVAO.vaoId = 0;	auxVAO.vboId = 0;	auxVAO.nVertexs = 0;


// cube ///////////////////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

// vertex coords array for glDrawElements() =====================================
// A cube has 6 sides and each side has 2 triangles, therefore, a cube consists
// of 24 vertices (6 sides * 4 vertices = 24 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 108 floats (24 * 3 = 72).
	//GLdouble vertices[] = {
	std::vector<double> vertices = {															//				Y-Axis - Z-Axis
		 size,  size,  size, -size,  size,  size, -size, -size,  size,  size, -size,  size,		// v0-v1-v2-v3 (front) - (top)
		 size,  size,  size,  size, -size,  size,  size, -size, -size,  size,  size, -size,		// v0-v3-v4-v5 (right) - (front)
		 size,  size,  size,  size,  size, -size, -size,  size, -size,  -size,  size,  size,	// v0-v5-v6-v1 (top) - (right)
		-size,  size,  size, -size,  size, -size, -size, -size, -size,  -size, -size,  size,	// v1-v6-v7-v2 (left) - (back)
		-size, -size, -size,  size, -size, -size,  size, -size,  size,  -size, -size,  size,	// v7-v4-v3-v2 (bottom) - (left)
		 size, -size, -size, -size, -size, -size, -size,  size, -size,  size,  size, -size };	// v4-v7-v6-v5 (back) - (bottom)

// Normal array
	//GLdouble normals[] = {
	std::vector<double> normals = {													//				Y-Axis - Z-Axis
		0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,		// v0-v1-v2-v3 (front) - (top)
		1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,		// v0-v3-v4-v5 (right) - (front)
		0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,		// v0-v5-v6-v1 (top) - (right)
	   -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0,		// v1-v6-v7-v2 (left) - (back)
		0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,		// v7-v4-v3-v2 (bottom) - (left)
		0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0,  0.0,  0.0, -1.0 };	// v4-v7-v6-v5 (back) - (bottom)

// Color array
	//GLdouble colors[] = {
	std::vector<double> colors = {
		1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1,		// v0-v1-v2-v3 (front) - (top)
		1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1,		// v0-v3-v4-v5 (right) - (front)
		1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1,		// v0-v5-v6-v1 (top) - (right)
		0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1,		// v1-v6-v7-v2 (left) - (back)
		0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1,		// v7-v4-v3-v2 (bottom) - (left)
		1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, };	// v4-v7-v6-v5 (back) - (bottom)

// Texture Array Z-Axis
//GLdouble textures[] = {
	std::vector<double> textures = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,     // v0-v1-v2-v3 (top)
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,     // v0-v3-v4-v5 (front)
		1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0,     // v0-v5-v6-v1 (right)
		0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0,		// v1-v6-v7-v2 (back)
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,     // v7-v4-v3-v2 (left)
		1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0 };    // v4-v7-v6-v5 (bottom)

// Indices Array
	//GLuint indices[] = {
	std::vector<uint> indices = {
		0, 1, 2, 2, 3, 0,			// v0-v1-v2-v3 (top)
		4, 5, 6, 6, 7, 4,			// v0-v3-v4-v5 (front)
		8, 9, 10, 10, 11, 8,		// v0-v5-v6-v1 (right)
		12, 13, 14, 14, 15, 12,		// v1-v6-v7-v2 (back)
		16, 17, 18, 18, 19, 16,		// v7-v4-v3-v2 (left)
		20, 21, 22, 22, 23, 20 };	// v4-v7-v6-v5 (bottom)

// Creació d'un VAO, VBO i EBO carregant-hi la geometria. Guardar identificadors VAO, VBO, EBO, vertex i indexs a struct CVAO.
	auxVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures,indices);
	//auxVAO.nVertexs = 36;

	return auxVAO;
}
// ---------- FI CUBE RGB---------------------------------------------------------

// ---------- CUBE SKYBOX --------------------------------------------------------
// Draws a Cube Skybox. Code based in done by Andreas Umbach <marvin@dataway.ch> and Enric Marti <enric.marti@uab.cat>
void CubeSkybox(GLdouble dSize)
{
	Set_VAOList(CUBE_SKYBOX, loadCubeSkybox_VAO());
	drawCubeSkybox();
	deleteVAOList(CUBE_SKYBOX);
}

//Draws a solid cube. Code contributed by Andreas Umbach <marvin@dataway.ch> and Enric Marti <enric.marti@uab.cat>
CVAO loadCubeSkybox_VAO()
{
	GLfloat skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

// skybox VAO
	CVAO cubeSkyVAO;	
	cubeSkyVAO.vaoId = 0;	cubeSkyVAO.vboId = 0;	cubeSkyVAO.eboId = 0;	cubeSkyVAO.nVertexs = 0; cubeSkyVAO.nIndices = 0;

	glGenVertexArrays(1, &cubeSkyVAO.vaoId); //glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &cubeSkyVAO.vboId); //glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(cubeSkyVAO.vaoId); // glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeSkyVAO.vboId); // glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	cubeSkyVAO.nVertexs = 36;
	return cubeSkyVAO;
}

void drawCubeSkybox()
{
// Recuperar identificadors VAO, VBO dels vector VAOList
	if (VAOList[CUBE_SKYBOX].vaoId != 0) 
		{	glBindVertexArray(VAOList[CUBE_SKYBOX].vaoId);
			glDrawArrays(GL_TRIANGLES, 0, VAOList[CUBE_SKYBOX].nVertexs);
			glBindVertexArray(0);
		}
}
// ---------- FI CUBE SKYBOX -----------------------------------------------------

// ---------- SPHERE -------------------------------------------------------------
/*
 * Compute lookup table of cos and sin values forming a cirle
 *
 * Notes:
 *    It is the responsibility of the caller to free these tables
 *    The size of the table is (n+1) to form a connected loop
 *    The last entry is exactly the same as the first
 *    The sign of n can be flipped to get the reverse loop
 */

 // Draws a solid sphere
void glutSolidSphere(GLdouble radius, GLint slices, GLint stacks)
{
	//Set_VAOList(GLUT_SPHERE, loadglutSolidSphere_VAO(radius, slices, stacks));
	Set_VAOList(GLUT_SPHERE, loadglutSolidSphere_EBO(radius, slices, stacks));
	draw_TriEBO_Object(GLUT_SPHERE); //draw_TriVAO_Object(GLUT_SPHERE); //drawSolidSphere(nvertexs);
	deleteVAOList(GLUT_SPHERE);
}

// Draws a solid sphere in VAO
CVAO loadglutSolidSphere_VAO(GLdouble radius, GLint slices, GLint stacks)
{
    int i,j;

/* Adjust z and radius as stacks are drawn. */
    double z0,z1;
    double r0,r1;

/* Pre-computed circle */
    double *sint1,*cost1;
    double *sint2,*cost2;

// VAO
	CVAO sphereVAO;
	sphereVAO.vaoId = 0;	sphereVAO.vboId = 0;	sphereVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

    fghCircleTable(&sint1,&cost1,-slices);
    fghCircleTable(&sint2,&cost2,stacks*2);

/* The top stack is covered with a triangle fan */
    z0 = 1.0;
    z1 = cost2[(stacks>0)?1:0];
    r0 = 0.0;
    r1 = sint2[(stacks>0)?1:0];

    //glBegin(GL_TRIANGLE_FAN);
    //glNormal3d(0,0,1);
    //glVertex3d(0,0,radius);
//	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
//	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
//	vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(radius);		// Vector Vertices
//	textures.push_back(0.0);		textures.push_back(radius);										// Vector Textures

    for (j=slices-1; j>=0; j--)
        {   
			// Vertex 0
			//glNormal3d(0,0,1);
			//glVertex3d(0,0,radius);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(radius);		// Vector Vertices
			textures.push_back(0.0);		textures.push_back(radius);										// Vector Textures
			
			// vertex j+1
			//glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
            //glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
			colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost1[j+1] * r1);			normals.push_back(sint1[j+1] * r1);			normals.push_back(z1);	// Vector Normals
			vertices.push_back(cost1[j+1] * r1*radius);	vertices.push_back(sint1[j+1] * r1*radius);	vertices.push_back(z1*radius);	// Vector Vertices
			textures.push_back(-(cost1[j+1] * r1*radius) + (sint1[j+1] * r1*radius));	textures.push_back(-(cost1[j+1] * r1*radius) + z1*radius);// Vector Textures

			// vertex j
			//glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
			//glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
			colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost1[j] * r1);			normals.push_back(sint1[j] * r1);			normals.push_back(z1);	// Vector Normals
			vertices.push_back(cost1[j] * r1 * radius);	vertices.push_back(sint1[j] * r1 * radius);	vertices.push_back(z1 * radius);	// Vector Vertices
			textures.push_back(-(cost1[j] * r1 * radius) + (sint1[j] * r1 * radius));	textures.push_back(-(cost1[j] * r1 * radius) + z1 * radius);// Vector Textures

        }
    //glEnd();

/* Cover each stack with a quad strip, except the top and bottom stacks */
    for( i=1; i<stacks-1; i++ )
    {
        z0 = z1; z1 = cost2[i+1];
        r0 = r1; r1 = sint2[i+1];
       // glBegin(GL_QUAD_STRIP);
            for(j=0; j<=slices-1; j++)
            {   
				// Vertex 0
				//glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
                //glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
				colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
				normals.push_back(cost1[j] * r1);			normals.push_back(sint1[j] * r1);			normals.push_back(z1);			// Vector Normals
				textures.push_back(-(cost1[j] * r1*radius) + (sint1[j] * r1*radius));	textures.push_back(-(cost1[j] * r1*radius) + z1*radius);// Vector Textures
				vertices.push_back(cost1[j] * r1*radius);	vertices.push_back(sint1[j] * r1*radius);	vertices.push_back(z1*radius);	// Vector Vertices
				
				// Vèrtex 1
				//glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
                //glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
				colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(cost1[j] * r0);			normals.push_back(sint1[j] * r0);			normals.push_back(z0);			// Vector Normals
				textures.push_back(-(cost1[j] * r0*radius) + (sint1[j] * r0*radius));	textures.push_back(-(cost1[j] * r0*radius) + z0*radius);// Vector Textures
				vertices.push_back(cost1[j] * r0*radius);	vertices.push_back(sint1[j] * r0*radius);	vertices.push_back(z0*radius);	// Vector Vertices

				// Vèrtex 3
				//glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
				//glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
				colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(cost1[j + 1] * r0);			normals.push_back(sint1[j + 1] * r0);			normals.push_back(z0);			// Vector Normals
				textures.push_back(-(cost1[j + 1] * r0 * radius) + (sint1[j+1] * r0 * radius));	textures.push_back(-(cost1[j + 1] * r0 * radius) + z0 * radius);// Vector Textures
				vertices.push_back(cost1[j + 1] * r0 * radius);	vertices.push_back(sint1[j + 1] * r0 * radius);	vertices.push_back(z0 * radius);	// Vector Vertices
				
				// Vèrtex 3
				//glNormal3d(cost1[j+1]*r0,        sint1[j+1]*r0,        z0       );
				//glVertex3d(cost1[j+1]*r0*radius, sint1[j+1]*r0*radius, z0*radius);
				colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(cost1[j + 1] * r0);			normals.push_back(sint1[j + 1] * r0);			normals.push_back(z0);			// Vector Normals
				textures.push_back(-(cost1[j + 1] * r0 * radius) + (sint1[j] * r0 * radius));	textures.push_back(-(cost1[j + 1] * r0 * radius) + z0 * radius);// Vector Textures
				vertices.push_back(cost1[j + 1] * r0 * radius);	vertices.push_back(sint1[j + 1] * r0 * radius);	vertices.push_back(z0 * radius);	// Vector Vertices

				// Vertex 2
				//glNormal3d(cost1[j+1]*r1,        sint1[j+1]*r1,        z1       );
				//glVertex3d(cost1[j+1]*r1*radius, sint1[j+1]*r1*radius, z1*radius);
				colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
				normals.push_back(cost1[j+1] * r1);			normals.push_back(sint1[j+1] * r1);			normals.push_back(z1);			// Vector Normals
				textures.push_back(-(cost1[j+1] * r1 * radius) + (sint1[j] * r1 * radius));	textures.push_back(-(cost1[j+1] * r1 * radius) + z1 * radius);// Vector Textures
				vertices.push_back(cost1[j+1] * r1 * radius);	vertices.push_back(sint1[j+1] * r1 * radius);	vertices.push_back(z1 * radius);	// Vector Vertices

				// Vertex 0
				//glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
				//glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
				colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
				normals.push_back(cost1[j] * r1);			normals.push_back(sint1[j] * r1);			normals.push_back(z1);			// Vector Normals
				textures.push_back(-(cost1[j] * r1 * radius) + (sint1[j] * r1 * radius));	textures.push_back(-(cost1[j] * r1 * radius) + z1 * radius);// Vector Textures
				vertices.push_back(cost1[j] * r1 * radius);	vertices.push_back(sint1[j] * r1 * radius);	vertices.push_back(z1 * radius);	// Vector Vertices

            }
        //glEnd();
    }

/* The bottom stack is covered with a triangle fan */
    z0 = z1;
    r0 = r1;

    //glBegin(GL_TRIANGLE_FAN);
    //    glNormal3d(0,0,-1);
    //    glVertex3d(0,0,-radius);
	//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
	//normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
	//textures.push_back(0.0);		textures.push_back(-radius);									// Vector Textures
	//vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(-radius);	// Vector Vertices

    for (j=0; j<=slices-1; j++)
        {   
			// Vèrtex 0
			//glNormal3d(0,0,-1);
			//glVertex3d(0,0,-radius);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
			textures.push_back(0.0);		textures.push_back(-radius);									// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(-radius);	// Vector Vertices

			// Vèrtex j 
			//glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
            //glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost1[j] * r0);	normals.push_back(sint1[j] * r0);	normals.push_back(z0);	// Vector Normals
			textures.push_back(-(cost1[j] * r0*radius) + (sint1[j] * r0*radius));	textures.push_back(-(cost1[j] * r0*radius) + z0*radius);// Vector Textures
			vertices.push_back(cost1[j] * r0*radius);	vertices.push_back(sint1[j] * r0*radius);	vertices.push_back(z0*radius);	// Vector Vertices

			// Vèrtex j+1 
			//glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
			//glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost1[j+1] * r0);	normals.push_back(sint1[j+1] * r0);	normals.push_back(z0);	// Vector Normals
			textures.push_back(-(cost1[j+1] * r0 * radius) + (sint1[j+1] * r0 * radius));	textures.push_back(-(cost1[j+1] * r0 * radius) + z0 * radius);// Vector Textures
			vertices.push_back(cost1[j+1] * r0* radius);	vertices.push_back(sint1[j+1] * r0* radius);	vertices.push_back(z0* radius);	// Vector Vertices
        }
    //glEnd();

// ----------------------- VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
	sphereVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//sphereVAO.nVertexs = (int) nv / 3;

// Release sin and cos tables
    free(sint1);
    free(cost1);
    free(sint2);
    free(cost2);

	return sphereVAO;
}


// Draws a solid sphere in EBO
CVAO loadglutSolidSphere_EBO(GLdouble radius, GLint slices, GLint stacks)
{
	int i, j;

/* Adjust z and radius as stacks are drawn. */
	double z0, z1;
	double r0, r1;

/* Pre-computed circle */
	double* sint1, * cost1;
	double* sint2, * cost2;

	GLint index = 0;	// Apuntador a vèrtexs per a indices.

// VAO
	CVAO sphereVAO;
	sphereVAO.vaoId = 0;	sphereVAO.vboId = 0;	sphereVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

	fghCircleTable(&sint1, &cost1, -slices);
	fghCircleTable(&sint2, &cost2, stacks * 2);

	/* The top stack is covered with a triangle fan */
	z0 = 1.0;
	z1 = cost2[(stacks > 0) ? 1 : 0];
	r0 = 0.0;
	r1 = sint2[(stacks > 0) ? 1 : 0];

	//glBegin(GL_TRIANGLE_FAN);
	//glNormal3d(0,0,1);
	
	// Vertex 0
	//glVertex3d(0,0,radius);
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
	vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(radius);		// Vector Vertices
	textures.push_back(0.0);		textures.push_back(radius);										// Vector Textures

	index = index + 1;

	for (j = slices - 1; j >= 0; j--)
	{
		// Vertex 0
		//glNormal3d(0,0,1);
		//glVertex3d(0,0,radius);
		//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
		//normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
		//vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(radius);		// Vector Vertices
		//textures.push_back(0.0);		textures.push_back(radius);										// Vector Textures

		// vertex j+1
		//glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
		//glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
		colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(cost1[j + 1] * r1);			normals.push_back(sint1[j + 1] * r1);			normals.push_back(z1);	// Vector Normals
		vertices.push_back(cost1[j + 1] * r1 * radius);	vertices.push_back(sint1[j + 1] * r1 * radius);	vertices.push_back(z1 * radius);	// Vector Vertices
		textures.push_back(-(cost1[j + 1] * r1 * radius) + (sint1[j + 1] * r1 * radius));	textures.push_back(-(cost1[j + 1] * r1 * radius) + z1 * radius);// Vector Textures

		// vertex j
		//glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
		//glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
		colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(cost1[j] * r1);			normals.push_back(sint1[j] * r1);			normals.push_back(z1);	// Vector Normals
		vertices.push_back(cost1[j] * r1 * radius);	vertices.push_back(sint1[j] * r1 * radius);	vertices.push_back(z1 * radius);	// Vector Vertices
		textures.push_back(-(cost1[j] * r1 * radius) + (sint1[j] * r1 * radius));	textures.push_back(-(cost1[j] * r1 * radius) + z1 * radius);// Vector Textures

		// Vector indices CARA i GL_TRIANGLE_FAN
		indices.push_back(0);		indices.push_back(index);		indices.push_back(index + 1);		// v0-vi-vi+1
		index = index + 2;

	}
	//glEnd();

/* Cover each stack with a quad strip, except the top and bottom stacks */
	for (i = 1; i < stacks - 1; i++)
	{
		z0 = z1; z1 = cost2[i + 1];
		r0 = r1; r1 = sint2[i + 1];
		// glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= slices - 1; j++)
		{
			// Vertex 0
			//glNormal3d(cost1[j]*r1,        sint1[j]*r1,        z1       );
			//glVertex3d(cost1[j]*r1*radius, sint1[j]*r1*radius, z1*radius);
			colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(cost1[j] * r1);			normals.push_back(sint1[j] * r1);			normals.push_back(z1);			// Vector Normals
			textures.push_back(-(cost1[j] * r1 * radius) + (sint1[j] * r1 * radius));	textures.push_back(-(cost1[j] * r1 * radius) + z1 * radius);// Vector Textures
			vertices.push_back(cost1[j] * r1 * radius);	vertices.push_back(sint1[j] * r1 * radius);	vertices.push_back(z1 * radius);	// Vector Vertices

			// Vèrtex 1
			//glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
			//glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
			colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost1[j] * r0);			normals.push_back(sint1[j] * r0);			normals.push_back(z0);			// Vector Normals
			textures.push_back(-(cost1[j] * r0 * radius) + (sint1[j] * r0 * radius));	textures.push_back(-(cost1[j] * r0 * radius) + z0 * radius);// Vector Textures
			vertices.push_back(cost1[j] * r0 * radius);	vertices.push_back(sint1[j] * r0 * radius);	vertices.push_back(z0 * radius);	// Vector Vertices

			// Vertex 2
			//glNormal3d(cost1[j+1]*r1,        sint1[j+1]*r1,        z1       );
			//glVertex3d(cost1[j+1]*r1*radius, sint1[j+1]*r1*radius, z1*radius);
			colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(cost1[j + 1] * r1);			normals.push_back(sint1[j + 1] * r1);			normals.push_back(z1);			// Vector Normals
			textures.push_back(-(cost1[j + 1] * r1 * radius) + (sint1[j] * r1 * radius));	textures.push_back(-(cost1[j + 1] * r1 * radius) + z1 * radius);// Vector Textures
			vertices.push_back(cost1[j + 1] * r1 * radius);	vertices.push_back(sint1[j + 1] * r1 * radius);	vertices.push_back(z1 * radius);	// Vector Vertices

			// Vèrtex 3
			//glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
			//glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
			colors.push_back(cColor[0]);				colors.push_back(cColor[1]);				colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost1[j + 1] * r0);			normals.push_back(sint1[j + 1] * r0);			normals.push_back(z0);			// Vector Normals
			textures.push_back(-(cost1[j + 1] * r0 * radius) + (sint1[j + 1] * r0 * radius));	textures.push_back(-(cost1[j + 1] * r0 * radius) + z0 * radius);// Vector Textures
			vertices.push_back(cost1[j + 1] * r0 * radius);	vertices.push_back(sint1[j + 1] * r0 * radius);	vertices.push_back(z0 * radius);	// Vector Vertices

			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 3);	// v0-v1-v3
			indices.push_back(index + 3);	indices.push_back(index + 2);		indices.push_back(index);		// v3-v2-v0
			index = index + 4;
		}
		//glEnd();
	}

	/* The bottom stack is covered with a triangle fan */
	z0 = z1;
	r0 = r1;

	//glBegin(GL_TRIANGLE_FAN);
	//    glNormal3d(0,0,-1);
	// Vèrtex 0.
	//    glVertex3d(0,0,-radius);
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
	textures.push_back(0.0);		textures.push_back(-radius);									// Vector Textures
	vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(-radius);	// Vector Vertices

	GLuint index_inferior = index;
	index = index + 1;

	for (j = 0; j <= slices - 1; j++)
	{
		// Vèrtex 0
		//glNormal3d(0,0,-1);
		//glVertex3d(0,0,-radius);
		//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		//normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
		//textures.push_back(0.0);		textures.push_back(-radius);									// Vector Textures
		//vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(-radius);	// Vector Vertices

		// Vèrtex j 
		//glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
		//glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
		colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(cost1[j] * r0);	normals.push_back(sint1[j] * r0);	normals.push_back(z0);	// Vector Normals
		textures.push_back(-(cost1[j] * r0 * radius) + (sint1[j] * r0 * radius));	textures.push_back(-(cost1[j] * r0 * radius) + z0 * radius);// Vector Textures
		vertices.push_back(cost1[j] * r0 * radius);	vertices.push_back(sint1[j] * r0 * radius);	vertices.push_back(z0 * radius);	// Vector Vertices

		// Vèrtex j+1 
		//glNormal3d(cost1[j]*r0,        sint1[j]*r0,        z0       );
		//glVertex3d(cost1[j]*r0*radius, sint1[j]*r0*radius, z0*radius);
		colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(cost1[j + 1] * r0);	normals.push_back(sint1[j + 1] * r0);	normals.push_back(z0);	// Vector Normals
		textures.push_back(-(cost1[j + 1] * r0 * radius) + (sint1[j + 1] * r0 * radius));	textures.push_back(-(cost1[j + 1] * r0 * radius) + z0 * radius);// Vector Textures
		vertices.push_back(cost1[j + 1] * r0 * radius);	vertices.push_back(sint1[j + 1] * r0 * radius);	vertices.push_back(z0 * radius);	// Vector Vertices
	
		// Vector indices CARA i GL_TRIANGLE_FAN
		indices.push_back(index_inferior);		indices.push_back(index);		indices.push_back(index + 1);		// v0-vi-vi+1
		index = index + 2;
	}
	//glEnd();

// ----------------------- VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

 // Creació d'un VAO, VBO i EBO amb càrrega de la geometria. Guardar identificadors VAO, VBO i EBO a struct CVAO.
	sphereVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	//sphereVAO.nVertexs = (int)nv / 3;

	// Release sin and cos tables
	free(sint1);
	free(cost1);
	free(sint2);
	free(cost2);

	return sphereVAO;
}

void fghCircleTable(double **sint,double **cost,const int n)
{
    int i;

/* Table size, the sign of n flips the circle direction */
    const int size = abs(n);

/* Determine the angle between samples */
    const double angle = 2*PI/(double)( ( n == 0 ) ? 1 : n );

/* Allocate memory for n samples, plus duplicate of first entry at the end */
    *sint = (double *) calloc(sizeof(double), size+1);
    *cost = (double *) calloc(sizeof(double), size+1);

/* Bail out if memory allocation fails, fgError never returns */
    if (!(*sint) || !(*cost))
    {   free(*sint);
        free(*cost);
		//AfxMessageBox(_T("Failed to allocate memory in fghCircleTable"));
    }

/* Compute cos and sin around the circle */
    (*sint)[0] = 0.0;
    (*cost)[0] = 1.0;

    for (i=1; i<size; i++)
    {
        (*sint)[i] = sin(angle*i);
        (*cost)[i] = cos(angle*i);
    }

/* Last sample is duplicate of the first */
    (*sint)[size] = (*sint)[0];
    (*cost)[size] = (*cost)[0];
}
// ---------- FI SPHERE ------------------------------------------------------------

// ---------- CONE -----------------------------------------------------------------

// Draws a solid cone
void glutSolidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
	//Set_VAOList(GLUT_CONE, loadglutSolidCone_VAO(base, height, slices, stacks));
	Set_VAOList(GLUT_CONE, loadglutSolidCone_EBO(base, height, slices, stacks));
	draw_TriEBO_Object(GLUT_CONE); //draw_TriVAO_Object(GLUT_CONE);
	deleteVAOList(GLUT_CONE);
}

// Draws a solid cone
CVAO loadglutSolidCone_VAO(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
    int i,j;

// Step in z and radius as stacks are drawn.
    double z0,z1;
    double r0,r1;

    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );
    const double rStep = base / ( ( stacks > 0 ) ? stacks : 1 );

// Scaling factors for vertex normals
    const double cosn = ( height / sqrt ( height * height + base * base ));
    const double sinn = ( base   / sqrt ( height * height + base * base ));

// Pre-computed circle 
    double *sint,*cost;

// VAO
	CVAO conusVAO;			
	conusVAO.vaoId = 0;	conusVAO.vboId = 0;	conusVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	vertices.resize(0);		normals.resize(0);		colors.resize(0);		textures.resize(0);			// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

    fghCircleTable(&sint,&cost,-slices);

// Cover the circular base with a triangle fan...
    z0 = 0.0;
    z1 = zStep;

    r0 = base;
    r1 = r0 - rStep;

    //glBegin(GL_TRIANGLE_FAN);
	//glNormal3d(0.0,0.0,-1.0);
    //glVertex3d(0.0,0.0, z0 );
	//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]); // Vector Colors
	//normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
	//textures.push_back(0.0);		textures.push_back(z0);											// Vector Textures
	//vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(z0);			// Vector Vertices
        for (j=0; j<=slices-1; j++)
			{	
				// Vèrtex 0
				//glNormal3d(0.0,0.0,-1.0);
				//glVertex3d(0.0,0.0, z0 );
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
				textures.push_back(0.0);		textures.push_back(z0);											// Vector Textures
				vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(z0);			// Vector Vertices
				
				//Vèrtex 1 (j)
				//glVertex3d(cost[j]*r0, sint[j]*r0, z0);
				colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
				normals.push_back(0.0);				normals.push_back(0.0);				normals.push_back(-1.0);	// Vector Normals
				textures.push_back(-(cost[j] * r0) + (sint[j] * r0));					textures.push_back(-(cost[j] * r0) + z0);	// Vector Textures
				vertices.push_back(cost[j] * r0);	vertices.push_back(sint[j] * r0);	vertices.push_back(z0);		// Vector Vertices

				//Vèrtex 2 (j+1)
				//glVertex3d(cost[j+1]*r0, sint[j+1]*r0, z0);
				colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
				normals.push_back(0.0);				normals.push_back(0.0);				normals.push_back(-1.0);	// Vector Normals
				textures.push_back(-(cost[j+1] * r0) + (sint[j+1] * r0));				textures.push_back(-(cost[j+1] * r0) + z0);	// Vector Textures
				vertices.push_back(cost[j+1] * r0);	vertices.push_back(sint[j+1] * r0);	vertices.push_back(z0);		// Vector Vertices
			}
    //glEnd();

/* Cover each stack with a quad strip, except the top stack */
    for( i=0; i<stacks; i++ )
    {	//glBegin(GL_QUAD_STRIP);
            for(j=0; j<=slices-1; j++)
            {   
				// Vèrtex 0
				//glNormal3d(cost[j]*cosn, sint[j]*cosn, sinn);
                //glVertex3d(cost[j]*r0,   sint[j]*r0,   z0  );
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(cost[j] * cosn);	normals.push_back(sint[j] * cosn);	normals.push_back(sinn);// Vector Normals
				textures.push_back(-(cost[j] * r0) + (sint[j] * r0));			textures.push_back(-(cost[j] * r0) + z0);	// Vector Textures
				vertices.push_back(cost[j] * r0);	vertices.push_back(sint[j] * r0);	vertices.push_back(z0);	// Vector Vertices

				// Vèrtex 1
                //glVertex3d(cost[j]*r1,   sint[j]*r1,   z1  );
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(cost[j] * cosn);	normals.push_back(sint[j] * cosn);	normals.push_back(sinn);// Vector Normals
				textures.push_back(-(cost[j] * r1) + (sint[j] * r1));			textures.push_back(-(cost[j] * r1) + z1);	// Vector Textures
				vertices.push_back(cost[j] * r1);	vertices.push_back(sint[j] * r1);	vertices.push_back(z1);	// Vector Vertices

				// Vèrtex 3
				//glVertex3d(cost[j]*r1,   sint[j]*r1,   z1  );
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(cost[j + 1] * cosn);	normals.push_back(sint[j+1] * cosn);	normals.push_back(sinn);// Vector Normals
				textures.push_back(-(cost[j + 1] * r1) + (sint[j+1] * r1));			textures.push_back(-(cost[j + 1] * r1) + z1);	// Vector Textures
				vertices.push_back(cost[j + 1] * r1);	vertices.push_back(sint[j + 1] * r1);	vertices.push_back(z1);	// Vector Vertices

				// Vèrtex 3
				//glVertex3d(cost[j]*r1,   sint[j]*r1,   z1  );
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(cost[j + 1] * cosn);	normals.push_back(sint[j+1] * cosn);	normals.push_back(sinn);// Vector Normals
				textures.push_back(-(cost[j + 1] * r1) + (sint[j+1] * r1));			textures.push_back(-(cost[j + 1] * r1) + z1);	// Vector Textures
				vertices.push_back(cost[j + 1] * r1);	vertices.push_back(sint[j + 1] * r1);	vertices.push_back(z1);	// Vector Vertices

				// Vèrtex 2
				//glNormal3d(cost[j+1]*cosn, sint[j+1]*cosn, sinn);
				//glVertex3d(cost[j+1]*r0,   sint[j+1]*r0,   z0  );
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(cost[j+1] * cosn);	normals.push_back(sint[j+1] * cosn);	normals.push_back(sinn);// Vector Normals
				textures.push_back(-(cost[j+1] * r0) + (sint[j] * r0));			textures.push_back(-(cost[j+1] * r0) + z0);	// Vector Textures
				vertices.push_back(cost[j+1] * r0);	vertices.push_back(sint[j+1] * r0);	vertices.push_back(z0);	// Vector Vertices

				// Vèrtex 0
				//glNormal3d(cost[j]*cosn, sint[j]*cosn, sinn);
				//glVertex3d(cost[j]*r0,   sint[j]*r0,   z0  );
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
				normals.push_back(cost[j] * cosn);	normals.push_back(sint[j] * cosn);	normals.push_back(sinn);// Vector Normals
				textures.push_back(-(cost[j] * r0) + (sint[j] * r0));			textures.push_back(-(cost[j] * r0) + z0);	// Vector Textures
				vertices.push_back(cost[j] * r0);	vertices.push_back(sint[j] * r0);	vertices.push_back(z0);	// Vector Vertices
            }
            z0 = z1; z1 += zStep;
            r0 = r1; r1 -= rStep;
        //glEnd();
    }
	
/* The top stack is covered with individual triangles */
    //glBegin(GL_TRIANGLES);
    //  glNormal3d(cost[0]*sinn, sint[0]*sinn, cosn);

        for (j=0; j<slices; j++)
        {   //glVertex3d(cost[j+0]*r0,   sint[j+0]*r0,   z0    );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(cost[0] * sinn);	normals.push_back(sint[0] * sinn);	normals.push_back(cosn);	// Vector Normals
			vertices.push_back(cost[j + 0] * r0);	vertices.push_back(sint[j + 0] * r0);	vertices.push_back(z0);	// Vector Vertices
			textures.push_back(-(cost[j + 0] * r0) + (sint[j + 0] * r0));			textures.push_back(-(cost[j + 0] * r0) + z0);	// Vector Textures
            //glVertex3d(0,              0,              height);

			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost[0] * sinn);	normals.push_back(sint[0] * sinn);	normals.push_back(cosn);	// Vector Normals
			textures.push_back(0.0);			textures.push_back(height);										// Vector Textures
			vertices.push_back(0.0);			vertices.push_back(0.0);			vertices.push_back(height);	// Vector Vertices

            //glNormal3d(cost[j+1]*sinn, sint[j+1]*sinn, cosn  );
            //glVertex3d(cost[j+1]*r0,   sint[j+1]*r0,   z0    );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost[j + 1] * sinn);	normals.push_back(sint[j + 1] * sinn);	normals.push_back(cosn);// Vector Normals
			textures.push_back(-(cost[j + 1] * r0) + (sint[j + 1] * r0));		textures.push_back(-(cost[j + 1] * r0) + z0);	// Vector Textures
			vertices.push_back(cost[j + 1] * r0);	vertices.push_back(sint[j + 1] * r0);	vertices.push_back(z0);	// Vector Vertices

        }
    //glEnd();

// ----------------------- VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	conusVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//conusVAO.nVertexs = int (nv / 3);

// Release sin and cos tables
    free(sint);
    free(cost);

	return conusVAO;
}

// Draws a solid cone
CVAO loadglutSolidCone_EBO(GLdouble base, GLdouble height, GLint slices, GLint stacks)
{
	int i, j;

// Step in z and radius as stacks are drawn.
	double z0, z1;
	double r0, r1;

	const double zStep = height / ((stacks > 0) ? stacks : 1);
	const double rStep = base / ((stacks > 0) ? stacks : 1);

// Scaling factors for vertex normals
	const double cosn = (height / sqrt(height * height + base * base));
	const double sinn = (base / sqrt(height * height + base * base));

// Pre-computed circle 
	double* sint, * cost;

	GLint index = 0;	// Apuntador a vèrtexs per a indices.

// VAO
	CVAO conusVAO;
	conusVAO.vaoId = 0;	conusVAO.vboId = 0;	conusVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	vertices.resize(0);		normals.resize(0);		colors.resize(0);		textures.resize(0);			// Reinicialitzar vectors
	std::vector <uint> indices;									// Definició vector dinàmic d'índexs per a EBO
	indices.resize(0);											// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	fghCircleTable(&sint, &cost, -slices);

	// Cover the circular base with a triangle fan...
	z0 = 0.0;
	z1 = zStep;

	r0 = base;
	r1 = r0 - rStep;

	//glBegin(GL_TRIANGLE_FAN);
	//glNormal3d(0.0,0.0,-1.0);
	// Vèrtex 0
	//glVertex3d(0.0,0.0, z0 );
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
	textures.push_back(0.0);		textures.push_back(z0);											// Vector Textures
	vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(z0);			// Vector Vertices

	index = index + 1;

	for (j = 0; j <= slices - 1; j++)
	{
		// Vèrtex 0
		//glNormal3d(0.0,0.0,-1.0);
		//glVertex3d(0.0,0.0, z0 );
		//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]); // Vector Colors
		//normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
		//textures.push_back(0.0);		textures.push_back(z0);											// Vector Textures
		//vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(z0);			// Vector Vertices

		//Vèrtex 1 (j)
		//glVertex3d(cost[j]*r0, sint[j]*r0, z0);
		colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
		normals.push_back(0.0);				normals.push_back(0.0);				normals.push_back(-1.0);	// Vector Normals
		textures.push_back(-(cost[j] * r0) + (sint[j] * r0));					textures.push_back(-(cost[j] * r0) + z0);	// Vector Textures
		vertices.push_back(cost[j] * r0);	vertices.push_back(sint[j] * r0);	vertices.push_back(z0);		// Vector Vertices

		//Vèrtex 2 (j+1)
		//glVertex3d(cost[j+1]*r0, sint[j+1]*r0, z0);
		colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
		normals.push_back(0.0);				normals.push_back(0.0);				normals.push_back(-1.0);	// Vector Normals
		textures.push_back(-(cost[j + 1] * r0) + (sint[j + 1] * r0));				textures.push_back(-(cost[j + 1] * r0) + z0);	// Vector Textures
		vertices.push_back(cost[j + 1] * r0);	vertices.push_back(sint[j + 1] * r0);	vertices.push_back(z0);		// Vector Vertices
	
		// Vector indices CARA i GL_TRIANGLE_FAN
		indices.push_back(0);		indices.push_back(index);		indices.push_back(index + 1);		// v0-vi-vi+1
		index = index + 2;	
	}
	//glEnd();

/* Cover each stack with a quad strip, except the top stack */
	for (i = 0; i < stacks; i++)
	{	//glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= slices - 1; j++)
		{
			// Vèrtex 0
			//glNormal3d(cost[j]*cosn, sint[j]*cosn, sinn);
			//glVertex3d(cost[j]*r0,   sint[j]*r0,   z0  );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost[j] * cosn);	normals.push_back(sint[j] * cosn);	normals.push_back(sinn);// Vector Normals
			textures.push_back(-(cost[j] * r0) + (sint[j] * r0));			textures.push_back(-(cost[j] * r0) + z0);	// Vector Textures
			vertices.push_back(cost[j] * r0);	vertices.push_back(sint[j] * r0);	vertices.push_back(z0);	// Vector Vertices

			// Vèrtex 1
			//glVertex3d(cost[j]*r1,   sint[j]*r1,   z1  );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost[j] * cosn);	normals.push_back(sint[j] * cosn);	normals.push_back(sinn);// Vector Normals
			textures.push_back(-(cost[j] * r1) + (sint[j] * r1));			textures.push_back(-(cost[j] * r1) + z1);	// Vector Textures
			vertices.push_back(cost[j] * r1);	vertices.push_back(sint[j] * r1);	vertices.push_back(z1);	// Vector Vertices

			// Vèrtex 2
			//glNormal3d(cost[j+1]*cosn, sint[j+1]*cosn, sinn);
			//glVertex3d(cost[j+1]*r0,   sint[j+1]*r0,   z0  );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost[j + 1] * cosn);	normals.push_back(sint[j + 1] * cosn);	normals.push_back(sinn);// Vector Normals
			textures.push_back(-(cost[j + 1] * r0) + (sint[j] * r0));			textures.push_back(-(cost[j + 1] * r0) + z0);	// Vector Textures
			vertices.push_back(cost[j + 1] * r0);	vertices.push_back(sint[j + 1] * r0);	vertices.push_back(z0);	// Vector Vertices

			// Vèrtex 3
			//glVertex3d(cost[j]*r1,   sint[j]*r1,   z1  );
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(cost[j + 1] * cosn);	normals.push_back(sint[j + 1] * cosn);	normals.push_back(sinn);// Vector Normals
			textures.push_back(-(cost[j + 1] * r1) + (sint[j + 1] * r1));			textures.push_back(-(cost[j + 1] * r1) + z1);	// Vector Textures
			vertices.push_back(cost[j + 1] * r1);	vertices.push_back(sint[j + 1] * r1);	vertices.push_back(z1);	// Vector Vertices

			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 3);	// v0,v1,v3
			indices.push_back(index + 3);	indices.push_back(index + 2);		indices.push_back(index);		// v3,v2,v0
			index = index + 4;
		}
		z0 = z1; z1 += zStep;
		r0 = r1; r1 -= rStep;
		//glEnd();
	}

	/* The top stack is covered with individual triangles */
		//glBegin(GL_TRIANGLES);
		//  glNormal3d(cost[0]*sinn, sint[0]*sinn, cosn);

	for (j = 0; j < slices; j++)
	{   //glVertex3d(cost[j+0]*r0,   sint[j+0]*r0,   z0    );
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
		normals.push_back(cost[0] * sinn);	normals.push_back(sint[0] * sinn);	normals.push_back(cosn);	// Vector Normals
		vertices.push_back(cost[j + 0] * r0);	vertices.push_back(sint[j + 0] * r0);	vertices.push_back(z0);	// Vector Vertices
		textures.push_back(-(cost[j + 0] * r0) + (sint[j + 0] * r0));			textures.push_back(-(cost[j + 0] * r0) + z0);	// Vector Textures
		//glVertex3d(0,              0,              height);

		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(cost[0] * sinn);	normals.push_back(sint[0] * sinn);	normals.push_back(cosn);	// Vector Normals
		textures.push_back(0.0);			textures.push_back(height);										// Vector Textures
		vertices.push_back(0.0);			vertices.push_back(0.0);			vertices.push_back(height);	// Vector Vertices

		//glNormal3d(cost[j+1]*sinn, sint[j+1]*sinn, cosn  );
		//glVertex3d(cost[j+1]*r0,   sint[j+1]*r0,   z0    );
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(cost[j + 1] * sinn);	normals.push_back(sint[j + 1] * sinn);	normals.push_back(cosn);// Vector Normals
		textures.push_back(-(cost[j + 1] * r0) + (sint[j + 1] * r0));		textures.push_back(-(cost[j + 1] * r0) + z0);	// Vector Textures
		vertices.push_back(cost[j + 1] * r0);	vertices.push_back(sint[j + 1] * r0);	vertices.push_back(z0);	// Vector Vertices

		// Vector indices CARA i GL_TRIANGLE
		indices.push_back(index);		indices.push_back(index+1);		indices.push_back(index + 2);		// vi,vi+1,vi+2
		index = index + 2;
	}
	//glEnd();

// ----------------------- VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

 // Creació d'un VAO, VBO i EBO amb càrrega de la geometria. Guardar identificadors VAO, VBO i EBO a struct CVAO.
	conusVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	//conusVAO.nVertexs = int(nv / 3);

// Release sin and cos tables
	free(sint);
	free(cost);

	return conusVAO;
}
// ---------- FI CONE ------------------------------------------------------------


// ---------- CYLINDER -----------------------------------------------------------

// Draws a solid cylinder
void glutSolidCylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks)
{
	//Set_VAOList(GLUT_CYLINDER, loadglutSolidCylinder_VAO(radius, height, slices, stacks));
	Set_VAOList(GLUT_CYLINDER, loadglutSolidCylinder_EBO(radius, height, slices, stacks));
	draw_TriEBO_Object(GLUT_CYLINDER); //draw_TriVAO_Object(GLUT_CYLINDER);
	deleteVAOList(GLUT_CYLINDER);
}

// Draws a solid cylinder in VAO
CVAO loadglutSolidCylinder_VAO(GLdouble radius, GLdouble height, GLint slices, GLint stacks)
{
    int i,j;

/* Step in z and radius as stacks are drawn. */
    double z0,z1;
    const double zStep = height / ( ( stacks > 0 ) ? stacks : 1 );

/* Pre-computed circle */
    double *sint,*cost;

// VAO
	CVAO cilindreVAO;		
	cilindreVAO.vaoId = 0;		cilindreVAO.vboId = 0;		cilindreVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

//    FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidCylinder" );

//---- DEFINICIÓ COORDENADES TEXTURA
	//Activa_Coordenades_Textura();

    fghCircleTable(&sint,&cost,-slices);

/* Cover the base and top */

    //glBegin(GL_TRIANGLE_FAN);
    //  glNormal3d(0.0, 0.0, -1.0 );
    //  glVertex3d(0.0, 0.0,  0.0 );
	//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]) // Vector Colors
	//normals.push_back(0.0);		normals.push_back(0.0);		normals.push_back(-1.0);				// Vector Normals
	//textures.push_back(0.0);	textures.push_back(0.0);											// Vector Textures
	//vertices.push_back(0.0);	vertices.push_back(0.0);	vertices.push_back(0.0);				// Vector Vertices

	for (j = 0; j <= slices; j++)
		{	
			// Vèrtex 0 (0)
			//  glNormal3d(0.0, 0.0, -1.0 );
			//  glVertex3d(0.0, 0.0,  0.0 );
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);		normals.push_back(0.0);		normals.push_back(-1.0);				// Vector Normals
			textures.push_back(0.0);	textures.push_back(0.0);											// Vector Textures
			vertices.push_back(0.0);	vertices.push_back(0.0);	vertices.push_back(0.0);				// Vector Vertices
		
			// Vèrtex 1 (j)
			//glVertex3d(cost[j]*radius, sint[j]*radius, 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);		// Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);					// Vector Normals
			textures.push_back(-(cost[j] * radius) + (sint[j] * radius)); textures.push_back(-(cost[j] * radius));		// Vector Textures
			vertices.push_back(cost[j] * radius);	vertices.push_back(sint[j] * radius);	vertices.push_back(0.0);	// Vector Vertices

			// Vèrtex 2 (j+1)
			//glVertex3d(cost[j+1]*radius, sint[j+1]*radius, 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);		// Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);					// Vector Normals
			textures.push_back(-(cost[j+1] * radius) + (sint[j+1] * radius)); textures.push_back(-(cost[j+1] * radius));		// Vector Textures
			vertices.push_back(cost[j+1] * radius);	vertices.push_back(sint[j+1] * radius);	vertices.push_back(0.0);	// Vector Vertices

		}
    //glEnd();

    //glBegin(GL_TRIANGLE_FAN);
    //  glNormal3d(0.0, 0.0, 1.0);
    //  glVertex3d(0.0, 0.0, height);
	//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3])// Vector Colors
	//normals.push_back(0.0);		normals.push_back(0.0);		normals.push_back(1.0);				// Vector Normals
	//textures.push_back(0.0);	textures.push_back(0.0);										// Vector Textures
	//vertices.push_back(0.0);	vertices.push_back(0.0);	vertices.push_back(height);			// Vector Vertices


	//for (j = slices; j >= 0; j--)
	for (j = slices; j >= 1; j--)
		{	
			// Vèrtex 0 (0)
			//  glNormal3d(0.0, 0.0, 1.0);
			//  glVertex3d(0.0, 0.0, height);
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]);	// Vector Colors
			normals.push_back(0.0);		normals.push_back(0.0);		normals.push_back(1.0);				// Vector Normals
			textures.push_back(0.0);	textures.push_back(0.0);										// Vector Textures
			vertices.push_back(0.0);	vertices.push_back(0.0);	vertices.push_back(height);			// Vector Vertices
			
			// Vèrtex 1 (j)
			//glVertex3d(cost[j]*radius, sint[j]*radius, height);
			colors.push_back(cColor[0]); colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(0.0);		 normals.push_back(0.0);	normals.push_back(-1.0);	// Vector Normals
			textures.push_back(-(cost[j] * radius) + (sint[j] * radius)); textures.push_back(-(cost[j] * radius) + height);// Vector Textures
			vertices.push_back(cost[j] * radius);	vertices.push_back(sint[j] * radius);	vertices.push_back(height);	// Vector Vertices

			// Vèrtex 2 (j-1)
			//glVertex3d(cost[j-1]*radius, sint[j-1]*radius, height);
			colors.push_back(cColor[0]);			colors.push_back(cColor[1]);			colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(0.0);					normals.push_back(0.0);					normals.push_back(-1.0);	// Vector Normals
			textures.push_back(-(cost[j-1] * radius) + (sint[j-1] * radius)); textures.push_back(-(cost[j-1] * radius) + height);// Vector Textures
			vertices.push_back(cost[j-1] * radius);	vertices.push_back(sint[j-1] * radius);	vertices.push_back(height);	// Vector Vertices
		}
    //glEnd();

/* Do the stacks */
    z0 = 0.0;
    z1 = zStep;

    for (i=1; i<=stacks; i++)
    {   if (i==stacks) z1 = height;
	    //glBegin(GL_QUAD_STRIP);
            for (j=0; j<=slices-1; j++ )
            {   
				// Vèrtex 0
				//glNormal3d(cost[j],        sint[j],        0.0);
                //glVertex3d(cost[j]*radius, sint[j]*radius, z0);
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
				normals.push_back(cost[j]);		normals.push_back(sint[j]);		normals.push_back(0.0);		// Vector Normals
				textures.push_back(-(cost[j] * radius) + (sint[j] * radius)); textures.push_back(-(cost[j] * radius) + z0);	// Vector Textures
				vertices.push_back(cost[j] * radius);	vertices.push_back(sint[j] * radius);	vertices.push_back(z0);		// Vector Vertices
                
				// Vèrtex 1
				//glVertex3d(cost[j]*radius, sint[j]*radius, z1);
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
				normals.push_back(cost[j]);		normals.push_back(sint[j]);		normals.push_back(0.0);		// Vector Normals
				textures.push_back(-(cost[j] * radius) + (sint[j] * radius)); textures.push_back(-(cost[j] * radius) + z1);	// Vector Textures
				vertices.push_back(cost[j] * radius);	vertices.push_back(sint[j] * radius);	vertices.push_back(z1);		// Vector Vertices

				// Vèrtex 3
				//glVertex3d(cost[j+1]*radius, sint[j+1]*radius, z1);
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
				normals.push_back(cost[j+1]);	normals.push_back(sint[j+1]);	normals.push_back(0.0);		// Vector Normals
				textures.push_back(-(cost[j+1] * radius) + (sint[j+1] * radius)); textures.push_back(-(cost[j+1] * radius) + z1);	// Vector Textures
				vertices.push_back(cost[j+1] * radius);	vertices.push_back(sint[j+1] * radius);	vertices.push_back(z1);		// Vector Vertices

				// Vèrtex 3
				//glVertex3d(cost[j+1]*radius, sint[j+1]*radius, z1);
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
				normals.push_back(cost[j + 1]);	normals.push_back(sint[j + 1]);	normals.push_back(0.0);		// Vector Normals
				textures.push_back(-(cost[j + 1] * radius) + (sint[j + 1] * radius)); textures.push_back(-(cost[j + 1] * radius) + z1);	// Vector Textures
				vertices.push_back(cost[j + 1] * radius);	vertices.push_back(sint[j + 1] * radius);	vertices.push_back(z1);		// Vector Vertices

				// Vèrtex 2
				//glVertex3d(cost[j+1]*radius, sint[j+1]*radius, z0);
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
				normals.push_back(cost[j+1]);	normals.push_back(sint[j]);		normals.push_back(0.0);		// Vector Normals
				textures.push_back(-(cost[j+1] * radius) + (sint[j+1] * radius)); textures.push_back(-(cost[j+1] * radius) + z0);	// Vector Textures
				vertices.push_back(cost[j+1] * radius);	vertices.push_back(sint[j+1] * radius);	vertices.push_back(z0);		// Vector Vertices

				// Vèrtex 0
				//glNormal3d(cost[j],        sint[j],        0.0);
				//glVertex3d(cost[j]*radius, sint[j]*radius, z0);
				colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
				normals.push_back(cost[j]);		normals.push_back(sint[j]);		normals.push_back(0.0);		// Vector Normals
				textures.push_back(-(cost[j] * radius) + (sint[j] * radius)); textures.push_back(-(cost[j] * radius) + z0);	// Vector Textures
				vertices.push_back(cost[j] * radius);	vertices.push_back(sint[j] * radius);	vertices.push_back(z0);		// Vector Vertices
            }
        //glEnd();
        z0 = z1; z1 += zStep;
    }

// ------------------------ VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	//std::vector <int>::size_type ni = vertices.size();	// Tamany del vector indices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	cilindreVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//cilindreVAO.nVertexs = ni;

/* Release sVAOin and cos tables */
    free(sint);
    free(cost);

	return cilindreVAO;
}


// Draws a solid cylinder in EBO

CVAO loadglutSolidCylinder_EBO(GLdouble radius, GLdouble height, GLint slices, GLint stacks)
{
	int i, j;

// Step in z and radius as stacks are drawn.
	double z0, z1;
	const double zStep = height / ((stacks > 0) ? stacks : 1);

// Pre-computed circle 
	double* sint, * cost;

	GLuint index = 0;	// Apuntador a vèrtexs per a indices.

// VAO
	CVAO cilindreVAO;
	cilindreVAO.vaoId = 0;		cilindreVAO.vboId = 0;		cilindreVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures
	std::vector <uint> indices;									// Definició vector dinàmic d'índexs per a EBO
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

//    FREEGLUT_EXIT_IF_NOT_INITIALISED ( "glutSolidCylinder" );

//---- DEFINICIÓ COORDENADES TEXTURA
	//Activa_Coordenades_Textura();

	fghCircleTable(&sint, &cost, -slices);

/* Cover the base and top */

	//glBegin(GL_TRIANGLE_FAN);
	//  glNormal3d(0.0, 0.0, -1.0 );

	// Vèrtex 0 (0)
	//  glVertex3d(0.0, 0.0,  0.0 );
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.0);		normals.push_back(-1.0);				// Vector Normals
	textures.push_back(0.0);	textures.push_back(0.0);											// Vector Textures
	vertices.push_back(0.0);	vertices.push_back(0.0);	vertices.push_back(0.0);				// Vector Vertices

	index = index + 1;

	for (j = 0; j <= slices; j++)
	{
		// Vèrtex 0 (0)
		//  glNormal3d(0.0, 0.0, -1.0 );
		//  glVertex3d(0.0, 0.0,  0.0 );
		//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		//normals.push_back(0.0);		normals.push_back(0.0);		normals.push_back(-1.0);				// Vector Normals
		//textures.push_back(0.0);	textures.push_back(0.0);											// Vector Textures
		//vertices.push_back(0.0);	vertices.push_back(0.0);	vertices.push_back(0.0);				// Vector Vertices

		// Vèrtex 1 (j)
		//glVertex3d(cost[j]*radius, sint[j]*radius, 0.0);
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);		// Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);					// Vector Normals
		textures.push_back(-(cost[j] * radius) + (sint[j] * radius)); textures.push_back(-(cost[j] * radius));		// Vector Textures
		vertices.push_back(cost[j] * radius);	vertices.push_back(sint[j] * radius);	vertices.push_back(0.0);	// Vector Vertices

		// Vèrtex 2 (j+1)
		//glVertex3d(cost[j+1]*radius, sint[j+1]*radius, 0.0);
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);		// Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);					// Vector Normals
		textures.push_back(-(cost[j + 1] * radius) + (sint[j + 1] * radius)); textures.push_back(-(cost[j + 1] * radius));		// Vector Textures
		vertices.push_back(cost[j + 1] * radius);	vertices.push_back(sint[j + 1] * radius);	vertices.push_back(0.0);	// Vector Vertices

		// Vector indices CARA i GL_TRIANGLE_FAN
		indices.push_back(0);		indices.push_back(index);		indices.push_back(index + 1);		// v0,vi,vi+1
		index = index + 2;

	}
	//glEnd();

	//glBegin(GL_TRIANGLE_FAN);
	//  glNormal3d(0.0, 0.0, 1.0);

	// Vèrtex 0 (0)
	//  glVertex3d(0.0, 0.0, height);
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.0);		normals.push_back(1.0);				// Vector Normals
	textures.push_back(0.0);	textures.push_back(0.0);										// Vector Textures
	vertices.push_back(0.0);	vertices.push_back(0.0);	vertices.push_back(height);			// Vector Vertices

	GLuint index_inferior = index;
	index = index + 1;

	//for (j = slices; j >= 0; j--)
	for (j = slices; j >= 1; j--)
	{
		// Vèrtex 0 (0)
		//  glNormal3d(0.0, 0.0, 1.0);
		//  glVertex3d(0.0, 0.0, height);
		//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]);	// Vector Colors
		//normals.push_back(0.0);		normals.push_back(0.0);		normals.push_back(1.0);				// Vector Normals
		//textures.push_back(0.0);	textures.push_back(0.0);										// Vector Textures
		//vertices.push_back(0.0);	vertices.push_back(0.0);	vertices.push_back(height);			// Vector Vertices

		// Vèrtex 1 (j)
		//glVertex3d(cost[j]*radius, sint[j]*radius, height);
		colors.push_back(cColor[0]); colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		normals.push_back(0.0);		 normals.push_back(0.0);	normals.push_back(-1.0);	// Vector Normals
		textures.push_back(-(cost[j] * radius) + (sint[j] * radius)); textures.push_back(-(cost[j] * radius) + height);// Vector Textures
		vertices.push_back(cost[j] * radius);	vertices.push_back(sint[j] * radius);	vertices.push_back(height);	// Vector Vertices

		// Vèrtex 2 (j-1)
		//glVertex3d(cost[j-1]*radius, sint[j-1]*radius, height);
		colors.push_back(cColor[0]);			colors.push_back(cColor[1]);			colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
		normals.push_back(0.0);					normals.push_back(0.0);					normals.push_back(-1.0);	// Vector Normals
		textures.push_back(-(cost[j - 1] * radius) + (sint[j - 1] * radius)); textures.push_back(-(cost[j - 1] * radius) + height);// Vector Textures
		vertices.push_back(cost[j - 1] * radius);	vertices.push_back(sint[j - 1] * radius);	vertices.push_back(height);	// Vector Vertices
	
		// Vector indices CARA i GL_TRIANGLE_FAN
		indices.push_back(index_inferior);		indices.push_back(index);		indices.push_back(index + 1);		// v0,vi,vi+1
		index = index + 2;
	
	}
	//glEnd();

/* Do the stacks */
	z0 = 0.0;
	z1 = zStep;

	for (i = 1; i <= stacks; i++)
	{
		if (i == stacks) z1 = height;
		//glBegin(GL_QUAD_STRIP);
		for (j = 0; j <= slices - 1; j++)
		{
			// Vèrtex 0
			//glNormal3d(cost[j],        sint[j],        0.0);
			//glVertex3d(cost[j]*radius, sint[j]*radius, z0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(cost[j]);		normals.push_back(sint[j]);		normals.push_back(0.0);		// Vector Normals
			textures.push_back(-(cost[j] * radius) + (sint[j] * radius)); textures.push_back(-(cost[j] * radius) + z0);	// Vector Textures
			vertices.push_back(cost[j] * radius);	vertices.push_back(sint[j] * radius);	vertices.push_back(z0);		// Vector Vertices

			// Vèrtex 1
			//glVertex3d(cost[j]*radius, sint[j]*radius, z1);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
			normals.push_back(cost[j]);		normals.push_back(sint[j]);		normals.push_back(0.0);		// Vector Normals
			textures.push_back(-(cost[j] * radius) + (sint[j] * radius)); textures.push_back(-(cost[j] * radius) + z1);	// Vector Textures
			vertices.push_back(cost[j] * radius);	vertices.push_back(sint[j] * radius);	vertices.push_back(z1);		// Vector Vertices

			// Vèrtex 2
			//glVertex3d(cost[j+1]*radius, sint[j+1]*radius, z0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]); colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(cost[j + 1]);	normals.push_back(sint[j]);		normals.push_back(0.0);		// Vector Normals
			textures.push_back(-(cost[j + 1] * radius) + (sint[j + 1] * radius)); textures.push_back(-(cost[j + 1] * radius) + z0);	// Vector Textures
			vertices.push_back(cost[j + 1] * radius);	vertices.push_back(sint[j + 1] * radius);	vertices.push_back(z0);		// Vector Vertices

			// Vèrtex 3
			//glVertex3d(cost[j+1]*radius, sint[j+1]*radius, z1);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);	// Vector Colors
			normals.push_back(cost[j + 1]);	normals.push_back(sint[j + 1]);	normals.push_back(0.0);		// Vector Normals
			textures.push_back(-(cost[j + 1] * radius) + (sint[j + 1] * radius)); textures.push_back(-(cost[j + 1] * radius) + z1);	// Vector Textures
			vertices.push_back(cost[j + 1] * radius);	vertices.push_back(sint[j + 1] * radius);	vertices.push_back(z1);		// Vector Vertices

			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 3);	// v0,v1,v3
			indices.push_back(index + 3);	indices.push_back(index + 2);		indices.push_back(index);		// v3,v2,v0
			index = index + 4;

		}
		//glEnd();
		z0 = z1; z1 += zStep;
	}

// ------------------------ VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

 // Creació d'un VAO, VBO i EBO amb càrrega de la geometria. Guardar identificadors VAO, VBO i EBO a struct CVAO.
	cilindreVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	//cilindreVAO.nVertexs = (int)nv / 3;

//  Release sVAOin and cos tables 
	free(sint);
	free(cost);

	return cilindreVAO;
}
// ---------- FI CYLINDER --------------------------------------------------------


// ---------- TORUS --------------------------------------------------------------

// Draws a solid torus
void glutSolidTorus(GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings)
{
	//Set_VAOList(GLUT_TORUS, loadglutSolidTorus_VAO(dInnerRadius, dOuterRadius, nSides, nRings));
	Set_VAOList(GLUT_TORUS, loadglutSolidTorus_EBO(dInnerRadius, dOuterRadius, nSides, nRings));
	draw_TriEBO_Object(GLUT_TORUS); //draw_TriVAO_Object(GLUT_TORUS); // drawSolidTorus(nvertexs);
	deleteVAOList(GLUT_TORUS);
}

// Draws a solid torus in VAO
CVAO loadglutSolidTorus_VAO(GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings)
{
  double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
  double *vertex, *normal;
  int    i, j;
  double spsi, cpsi, sphi, cphi ;
  //float modul = 0.0;
  CPunt3D val;

// VAO
  CVAO torusVAO;
  torusVAO.vaoId = 0;	torusVAO.vboId = 0;		torusVAO.nVertexs = 0;

  std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
  //vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
  vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
  //GLdouble cColor[4];
  //glGetDoublev(GL_CURRENT_COLOR, cColor);

//---- DEFINICIÓ COORDENADES TEXTURA
  //Activa_Coordenades_Textura();

  if ( nSides < 1 ) nSides = 1;
  if ( nRings < 1 ) nRings = 1;

/* Increment the number of sides and rings to allow for one more point than surface */
  nSides ++ ;
  nRings ++ ;

/* Allocate the vertices array */
  vertex = (double *)calloc( sizeof(double), 3.0 * nSides * nRings );
  normal = (double *)calloc( sizeof(double), 3.0 * nSides * nRings );

  dpsi =  2.0 * PI / (double)(nRings - 1.0) ;
  dphi = -2.0 * PI / (double)(nSides - 1.0) ;
  psi  = 0.0;

  for( j=0; j<nRings; j++ )
  { cpsi = cos ( psi ) ;
    spsi = sin ( psi ) ;
    phi = 0.0;

    for( i=0; i<nSides; i++ )
    { int offset = 3 * ( j * nSides + i ) ;
      cphi = cos ( phi ) ;
      sphi = sin ( phi ) ;
      *(vertex + offset + 0) = cpsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 1) = spsi * ( oradius + cphi * iradius ) ;
      *(vertex + offset + 2) =                    sphi * iradius  ;
      *(normal + offset + 0) = cpsi * cphi ;
      *(normal + offset + 1) = spsi * cphi ;
      *(normal + offset + 2) =        sphi ;
      phi += dphi;
    }
    psi += dpsi;
  }

  //glBegin( GL_QUADS );
    for( i=0; i<nSides-1; i++ )
    { for( j=0; j<nRings-1; j++ )
      { int offset = 3 * ( j * nSides + i ) ;

		// Vèrtex 0
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		val.x = *(vertex + offset + 0);	val.y = *(vertex + offset + 1);	val.z = *(vertex + offset + 2);
		//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
		textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
		//glNormal3dv(normal + offset);
		normals.push_back(*(normal + offset + 0));	normals.push_back(*(normal + offset + 1));	normals.push_back(*(normal + offset + 2)); // Vector Normals
		//glVertex3dv(vertex + offset);
		vertices.push_back(*(vertex + offset + 0));	vertices.push_back(*(vertex + offset + 1)); vertices.push_back(*(vertex + offset + 2)); // Vector Vertices

		// Vèrtex 1
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		val.x = *(vertex + offset + 3);	val.y = *((vertex + offset + 3) + 1);	val.z = *((vertex + offset + 3) + 2);
		//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
		textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
		//glNormal3dv(normal + offset + 3);
		normals.push_back(*(normal + offset + 3)); normals.push_back(*((normal + offset + 3) + 1));	normals.push_back(*((normal + offset + 3) + 2)); // Vector Normals
		//glVertex3dv(vertex + offset + 3);
		vertices.push_back(*(vertex + offset + 3));	vertices.push_back(*((vertex + offset + 3) + 1));	vertices.push_back(*((vertex + offset + 3) + 2)); // Vector Vertices
		
		// Vèrtex 2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		val.x = *((vertex + offset + 3 * nSides + 3) + 0);	val.y = *((vertex + offset + 3 * nSides + 3) + 1);	val.z = *((vertex + offset + 3 * nSides + 3) + 2);
		//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
		textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
		//glNormal3dv(normal + offset + 3 * nSides + 3);
		normals.push_back(*((normal + offset + 3 * nSides + 3) + 0));	normals.push_back(*((normal + offset + 3 * nSides + 3) + 1)); normals.push_back(*((normal + offset + 3 * nSides + 3) + 2));	// Vector Normals
		//glVertex3dv(vertex + offset + 3 * nSides + 3);
		vertices.push_back(*((vertex + offset + 3 * nSides + 3) + 0));	vertices.push_back(*((vertex + offset + 3 * nSides + 3) + 1));	vertices.push_back(*((vertex + offset + 3 * nSides + 3) + 2)); // Vector Vertices

		// Vèrtex 2
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		val.x = *((vertex + offset + 3 * nSides + 3) + 0);	val.y = *((vertex + offset + 3 * nSides + 3) + 1);	val.z = *((vertex + offset + 3 * nSides + 3) + 2);
		//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
		textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
		//glNormal3dv(normal + offset + 3 * nSides + 3);
		normals.push_back(*((normal + offset + 3 * nSides + 3) + 0));	normals.push_back(*((normal + offset + 3 * nSides + 3) + 1)); normals.push_back(*((normal + offset + 3 * nSides + 3) + 2));	// Vector Normals
		//glVertex3dv(vertex + offset + 3 * nSides + 3);
		vertices.push_back(*((vertex + offset + 3 * nSides + 3) + 0));	vertices.push_back(*((vertex + offset + 3 * nSides + 3) + 1));	vertices.push_back(*((vertex + offset + 3 * nSides + 3) + 2)); // Vector Vertices

		// Vèrtex 3
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		val.x = *((vertex + offset + 3 * nSides) + 0);	val.y = *((vertex + offset + 3 * nSides) + 1);	val.z = *((vertex + offset + 3 * nSides) + 2);
		//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
		textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
		//glNormal3dv(normal + offset + 3 * nSides);
		normals.push_back(*((normal + offset + 3 * nSides) + 0));	normals.push_back(*((normal + offset + 3 * nSides) + 1)); normals.push_back(*((normal + offset + 3 * nSides) + 2));	// Vector Normals
		//glVertex3dv(vertex + offset + 3 * nSides);
		vertices.push_back(*((vertex + offset + 3 * nSides) + 0));	vertices.push_back(*((vertex + offset + 3 * nSides) + 1));	vertices.push_back(*((vertex + offset + 3 * nSides) + 2)); // Vector Vertices

		// Vèrtex 0
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		val.x = *(vertex + offset + 0);	val.y = *(vertex + offset + 1);	val.z = *(vertex + offset + 2);
		//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
		textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
		//glNormal3dv(normal + offset);
		normals.push_back(*(normal + offset + 0));	normals.push_back(*(normal + offset + 1));	normals.push_back(*(normal + offset + 2)); // Vector Normals
		//glVertex3dv(vertex + offset);
		vertices.push_back(*(vertex + offset + 0));	vertices.push_back(*(vertex + offset + 1)); vertices.push_back(*(vertex + offset + 2)); // Vector Vertices
      }
    }
  //glEnd();

// ------------------------ VAO
  std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
  torusVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
  torusVAO.nVertexs = (int)nv / 3;

  free ( vertex ) ;
  free ( normal ) ;

//---- DEFINICIÓ COORDENADES TEXTURA
  //Desactiva_Coordenades_Textura();
  return torusVAO;
}


// Draws a solid torus in EBO
CVAO loadglutSolidTorus_EBO(GLdouble dInnerRadius, GLdouble dOuterRadius, GLint nSides, GLint nRings)
{
	double  iradius = dInnerRadius, oradius = dOuterRadius, phi, psi, dpsi, dphi;
	double* vertex, * normal;
	int    i, j;
	double spsi, cpsi, sphi, cphi;
	//float modul = 0.0;
	CPunt3D val;

	GLuint index = 0;	// Apuntador a vèrtexs per a indices.

// VAO
	CVAO torusVAO;
	torusVAO.vaoId = 0;	torusVAO.vboId = 0;		torusVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;									// Definició vector dinàmic d'índexs per a EBO
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

  // Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

  //---- DEFINICIÓ COORDENADES TEXTURA
	//Activa_Coordenades_Textura();

	if (nSides < 1) nSides = 1;
	if (nRings < 1) nRings = 1;

// Increment the number of sides and rings to allow for one more point than surface 
	nSides++;
	nRings++;

// Allocate the vertices array 
	vertex = (double*)calloc(sizeof(double), 3.0 * nSides * nRings);
	normal = (double*)calloc(sizeof(double), 3.0 * nSides * nRings);

	dpsi = 2.0 * PI / (double)(nRings - 1.0);
	dphi = -2.0 * PI / (double)(nSides - 1.0);
	psi = 0.0;

	for (j = 0; j < nRings; j++)
	{
		cpsi = cos(psi);
		spsi = sin(psi);
		phi = 0.0;

		for (i = 0; i < nSides; i++)
		{
			int offset = 3 * (j * nSides + i);
			cphi = cos(phi);
			sphi = sin(phi);
			*(vertex + offset + 0) = cpsi * (oradius + cphi * iradius);
			*(vertex + offset + 1) = spsi * (oradius + cphi * iradius);
			*(vertex + offset + 2) = sphi * iradius;
			*(normal + offset + 0) = cpsi * cphi;
			*(normal + offset + 1) = spsi * cphi;
			*(normal + offset + 2) = sphi;
			phi += dphi;
		}
		psi += dpsi;
	}

	//glBegin( GL_QUADS );
	for (i = 0; i < nSides - 1; i++)
	{
		for (j = 0; j < nRings - 1; j++)
		{
			int offset = 3 * (j * nSides + i);

			// Vèrtex 0
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			val.x = *(vertex + offset + 0);	val.y = *(vertex + offset + 1);	val.z = *(vertex + offset + 2);
			//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
			textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
			//glNormal3dv(normal + offset);
			normals.push_back(*(normal + offset + 0));	normals.push_back(*(normal + offset + 1));	normals.push_back(*(normal + offset + 2)); // Vector Normals
			//glVertex3dv(vertex + offset);
			vertices.push_back(*(vertex + offset + 0));	vertices.push_back(*(vertex + offset + 1)); vertices.push_back(*(vertex + offset + 2)); // Vector Vertices

			// Vèrtex 1
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			val.x = *(vertex + offset + 3);	val.y = *((vertex + offset + 3) + 1);	val.z = *((vertex + offset + 3) + 2);
			//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
			textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
			//glNormal3dv(normal + offset + 3);
			normals.push_back(*(normal + offset + 3)); normals.push_back(*((normal + offset + 3) + 1));	normals.push_back(*((normal + offset + 3) + 2)); // Vector Normals
			//glVertex3dv(vertex + offset + 3);
			vertices.push_back(*(vertex + offset + 3));	vertices.push_back(*((vertex + offset + 3) + 1));	vertices.push_back(*((vertex + offset + 3) + 2)); // Vector Vertices

			// Vèrtex 2
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			val.x = *((vertex + offset + 3 * nSides + 3) + 0);	val.y = *((vertex + offset + 3 * nSides + 3) + 1);	val.z = *((vertex + offset + 3 * nSides + 3) + 2);
			//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
			textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
			//glNormal3dv(normal + offset + 3 * nSides + 3);
			normals.push_back(*((normal + offset + 3 * nSides + 3) + 0));	normals.push_back(*((normal + offset + 3 * nSides + 3) + 1)); normals.push_back(*((normal + offset + 3 * nSides + 3) + 2));	// Vector Normals
			//glVertex3dv(vertex + offset + 3 * nSides + 3);
			vertices.push_back(*((vertex + offset + 3 * nSides + 3) + 0));	vertices.push_back(*((vertex + offset + 3 * nSides + 3) + 1));	vertices.push_back(*((vertex + offset + 3 * nSides + 3) + 2)); // Vector Vertices

			// Vèrtex 3
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			val.x = *((vertex + offset + 3 * nSides) + 0);	val.y = *((vertex + offset + 3 * nSides) + 1);	val.z = *((vertex + offset + 3 * nSides) + 2);
			//modul = sqrt(val.x * val.x + val.y * val.y + val.z * val.z);	val.x = val.x / modul;	val.y = val.y / modul;	val.z = val.z / modul;
			textures.push_back(-val.x + val.y); textures.push_back(-val.x + val.z);							// Vector Textures
			//glNormal3dv(normal + offset + 3 * nSides);
			normals.push_back(*((normal + offset + 3 * nSides) + 0));	normals.push_back(*((normal + offset + 3 * nSides) + 1)); normals.push_back(*((normal + offset + 3 * nSides) + 2));	// Vector Normals
			//glVertex3dv(vertex + offset + 3 * nSides);
			vertices.push_back(*((vertex + offset + 3 * nSides) + 0));	vertices.push_back(*((vertex + offset + 3 * nSides) + 1));	vertices.push_back(*((vertex + offset + 3 * nSides) + 2)); // Vector Vertices

			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 2);	// v0,v1,v2
			indices.push_back(index + 2);	indices.push_back(index + 3);		indices.push_back(index);		// v2,v3,v0
			index = index + 4;
		}
	}
	//glEnd();

  // ------------------------ VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

  // Creació d'un VAO, VBO i EBO amb càrrega de la geometria. Guardar identificadors VAO, VBO i EBO a struct CVAO.
	torusVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	//torusVAO.nVertexs = (int)nv / 3;

	free(vertex);
	free(normal);

//---- DEFINICIÓ COORDENADES TEXTURA
	//Desactiva_Coordenades_Textura();
	return torusVAO;
}
// ---------- FI TORUS -----------------------------------------------------------


// ---------- DODECAHEDRON -------------------------------------------------------

// Draws a solid dodecahedron
void glutSolidDodecahedron(void)
{
	Set_VAOList(GLUT_DODECAHEDRON, loadglutSolidDodecahedron_VAO());
	draw_TriVAO_Object(GLUT_DODECAHEDRON); // drawSolidDodecahedron();
	deleteVAOList(GLUT_DODECAHEDRON);
}

/*
 * Draws a solid dodecahedron in VAO
 */
CVAO loadglutSolidDodecahedron_VAO( void )
{
// VAO
	CVAO dodeVAO;
	dodeVAO.vaoId = 0;		dodeVAO.vboId = 0;		dodeVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

//---- DEFINICIÓ COORDENADES TEXTURA
	//Activa_Coordenades_Textura();

  /* Magic Numbers:  It is possible to create a dodecahedron by attaching two pentagons to each face of
   * of a cube.  The coordinates of the points are:
   *   (+-x,0, z); (+-1, 1, 1); (0, z, x )
   * where x = (-1 + sqrt(5))/2, z = (1 + sqrt(5))/2 or
   *       x = 0.61803398875 and z = 1.61803398875.
   */
  //glBegin ( GL_POLYGON ) ; // CARA 1
  //  glNormal3d (  0.0,  0.525731112119,  0.850650808354 ) ; glVertex3d (  0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
  //glEnd () ;

// VAO CARA 1
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  textures.push_back(1.61803398875); textures.push_back(0.61803398875);									// Vector Textures
  vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);		// Vector Vertices

  // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			// Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  textures.push_back(2.0); textures.push_back(2.0);														// Vector Textures
  vertices.push_back(-1.0);	vertices.push_back(1.0);	vertices.push_back(1.0);						// Vector Vertices

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  textures.push_back(0.61803398875); textures.push_back(0.61803398875 + 1.61803398875);					// Vector Textures
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices
  
   // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  textures.push_back(0.61803398875); textures.push_back(0.61803398875 + 1.61803398875);					// Vector Textures
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 + 1.61803398875);				// Vector Textures
  vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices
 
  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  textures.push_back(0.0); textures.push_back(0.0);														// Vector textures
  vertices.push_back(1.0);	vertices.push_back(1.0);			vertices.push_back(1.0);				// Vector Vertices

   // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  textures.push_back(1.61803398875); textures.push_back(0.61803398875);									// Vector Textures
  vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);		// Vector Vertices

   // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  textures.push_back(0.61803398875); textures.push_back(0.61803398875 + 1.61803398875);					// Vector Textures
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices

   // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  textures.push_back(0.0); textures.push_back(0.0);														// Vector textures
  vertices.push_back(1.0);	vertices.push_back(1.0);			vertices.push_back(1.0);				// Vector Vertices

/*
  glBegin ( GL_POLYGON ) ; // CARA 2
    glNormal3d (  0.0,  0.525731112119, -0.850650808354 ) ; glVertex3d (  0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
  glEnd () ;
*/

  //vertices.resize(0);		normals.resize(0);		colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
// VAO CARA 2
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);		// Vector Vertices
  textures.push_back(1.61803398875); textures.push_back(-0.61803398875);								// Vector Textures

  //Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(1.0);	vertices.push_back(1.0);			vertices.push_back(-1.0);				// Vector Vertices
  textures.push_back(0.0);	textures.push_back(-2.0);													// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 - 1.61803398875);				// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 - 1.61803398875);				// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(0.61803398875); textures.push_back(0.61803398875 - 1.61803398875);					// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(-1.0);	vertices.push_back(1.0);			vertices.push_back(-1.0);				// Vector Vertices
  textures.push_back(2.0);	textures.push_back(0.0);													// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);		// Vector Vertices
  textures.push_back(1.61803398875); textures.push_back(-0.61803398875);								// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 - 1.61803398875);				// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(-1.0);	vertices.push_back(1.0);			vertices.push_back(-1.0);				// Vector Vertices
  textures.push_back(2.0);	textures.push_back(0.0);													// Vector textures

/*
  glBegin ( GL_POLYGON ) ;	// CARA 3
    glNormal3d (  0.0, -0.525731112119,  0.850650808354 ) ; glVertex3d (  0.0, -1.61803398875,  0.61803398875 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
  glEnd () ;
*/
// VAO CARA 3
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);		// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(0.61803398875);								// Vector Textures

  // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  vertices.push_back(1.0);	vertices.push_back(-1.0);	vertices.push_back(1.0);						// Vector Vertices
  textures.push_back(-2.0);	textures.push_back(0.0);													// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 + 1.61803398875);				// Vector Textures

   // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 + 1.61803398875);				// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices
  textures.push_back(0.61803398875); textures.push_back(0.61803398875 + 1.61803398875);					// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  vertices.push_back(-1.0);	vertices.push_back(-1.0);			vertices.push_back(1.0);				// Vector Vertices
  textures.push_back(0.0);	textures.push_back(2.0);													// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);		// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(0.61803398875);								// Vector Textures

   // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 + 1.61803398875);				// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
  vertices.push_back(-1.0);	vertices.push_back(-1.0);			vertices.push_back(1.0);				// Vector Vertices
  textures.push_back(0.0);	textures.push_back(2.0);													// Vector textures

/*
  glBegin ( GL_POLYGON ) ;	// CARA 4
    glNormal3d (  0.0, -0.525731112119, -0.850650808354 ) ; glVertex3d (  0.0, -1.61803398875, -0.61803398875 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
  glEnd () ;
*/
// VAO CARA 4
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);		// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(-0.61803398875);								// Vector Textures

  // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(-1.0);	vertices.push_back(-1.0);	vertices.push_back(-1.0);						// Vector Vertices
  textures.push_back(0.0);	textures.push_back(0.0);													// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(0.61803398875); textures.push_back(0.61803398875 - 1.61803398875);					// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(0.61803398875); textures.push_back(0.61803398875 - 1.61803398875);					// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 - 1.61803398875);				// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(1.0);	vertices.push_back(-1.0);			vertices.push_back(-1.0);				// Vector Vertices
  textures.push_back(-2.0);	textures.push_back(-2.0);													// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);		// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(-0.61803398875);								// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(0.61803398875); textures.push_back(0.61803398875 - 1.61803398875);					// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(1.0);	vertices.push_back(-1.0);			vertices.push_back(-1.0);				// Vector Vertices
  textures.push_back(-2.0);	textures.push_back(-2.0);													// Vector textures

/*
  glBegin ( GL_POLYGON ) ;	// CARA 5
    glNormal3d (  0.850650808354,  0.0,  0.525731112119 ) ; glVertex3d (  0.61803398875,  0.0,  1.61803398875 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
  glEnd () ;
*/
// VAO CARA 5
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0); vertices.push_back(1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875);	textures.push_back(-0.61803398875 + 1.61803398875);			// Vector Textures

  // Vèrtex 1 
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	 normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(-1.0); vertices.push_back(1.0);				// Vector Vertices
  textures.push_back(-2.0);			textures.push_back(0.0);										// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875); vertices.push_back(0.0);// Vector Vertices
  textures.push_back(-1.61803398875 - 0.61803398875);  textures.push_back(-1.61803398875);			// Vector Textures

   // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875); vertices.push_back(0.0);// Vector Vertices
  textures.push_back(-1.61803398875 - 0.61803398875);  textures.push_back(-1.61803398875);			// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875); vertices.push_back(0.0);	// Vector Vertices
  textures.push_back(-1.61803398875 + 0.61803398875); textures.push_back(-1.61803398875);			// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(1.0); vertices.push_back(1.0);				// Vector Vertices
  textures.push_back(0.0);			textures.push_back(0.0);										// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0); vertices.push_back(1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875);	textures.push_back(-0.61803398875 + 1.61803398875);			// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875); vertices.push_back(0.0);// Vector Vertices
  textures.push_back(-1.61803398875 - 0.61803398875);  textures.push_back(-1.61803398875);			// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(1.0); vertices.push_back(1.0);				// Vector Vertices
  textures.push_back(0.0);			textures.push_back(0.0);										// Vector textures

 /*
  glBegin ( GL_POLYGON ) ;	// CARA 6
    glNormal3d ( -0.850650808354,  0.0,  0.525731112119 ) ; glVertex3d ( -0.61803398875,  0.0,  1.61803398875 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
  glEnd () ;
 */
// VAO CARA 6
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0); vertices.push_back(1.61803398875);	// Vector Vertices
  textures.push_back(0.61803398875);	textures.push_back(0.61803398875 + 1.61803398875);			// Vector Textures

  // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(1.0); vertices.push_back(1.0);				// Vector Vertices
  textures.push_back(2.0);			textures.push_back(2.0);										// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875); vertices.push_back(0.0);	// Vector Vertices
  textures.push_back(1.61803398875 + 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875); vertices.push_back(0.0);	// Vector Vertices
  textures.push_back(1.61803398875 + 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875); vertices.push_back(0.0);// Vector Vertices
  textures.push_back(1.61803398875 - 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(-1.0); vertices.push_back(1.0);				// Vector Vertices
  textures.push_back(0.0);			textures.push_back(2.0);										// Vector textures

   // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0); vertices.push_back(1.61803398875);	// Vector Vertices
  textures.push_back(0.61803398875);	textures.push_back(0.61803398875 + 1.61803398875);			// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875); vertices.push_back(0.0);	// Vector Vertices
  textures.push_back(1.61803398875 + 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(-1.0); vertices.push_back(1.0);				// Vector Vertices
  textures.push_back(0.0);			textures.push_back(2.0);										// Vector textures

 /*
  glBegin ( GL_POLYGON ) ;	// CARA 7
    glNormal3d (  0.850650808354,  0.0, -0.525731112119 ) ; glVertex3d (  0.61803398875,  0.0, -1.61803398875 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
  glEnd () ;
 */
  //vertices.resize(0);		normals.resize(0);		colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
 // VAO CARA 7
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0); vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 - 1.61803398875);				// Vector Textures

  // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(1.0); vertices.push_back(-1.0);					// Vector Vertices
  textures.push_back(0.0); textures.push_back(-2.0);													// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);	// Vector Vertices
  textures.push_back(-1.61803398875 + 0.61803398875); textures.push_back(-1.61803398875);				// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);	// Vector Vertices
  textures.push_back(-1.61803398875 + 0.61803398875); textures.push_back(-1.61803398875);				// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	// Vector Vertices
  textures.push_back(-1.61803398875 - 0.61803398875); textures.push_back(-1.61803398875);				// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(-1.0); vertices.push_back(-1.0);					// Vector Vertices
  textures.push_back(-2.0); textures.push_back(-2.0);													// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
  vertices.push_back(0.61803398875);	vertices.push_back(0.0); vertices.push_back(-1.61803398875);	// Vector Vertices
  textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 - 1.61803398875);				// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);	// Vector Vertices
  textures.push_back(-1.61803398875 + 0.61803398875); textures.push_back(-1.61803398875);				// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(-1.0); vertices.push_back(-1.0);					// Vector Vertices
  textures.push_back(-2.0); textures.push_back(-2.0);													// Vector textures

/*
  glBegin ( GL_POLYGON ) ;	// CARA 8
    glNormal3d ( -0.850650808354,  0.0, -0.525731112119 ) ; glVertex3d ( -0.61803398875,  0.0, -1.61803398875 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
  glEnd () ;
*/
// VAO CARA 8
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0); vertices.push_back(-1.61803398875);// Vector Vertices
  textures.push_back(0.61803398875);  textures.push_back(0.61803398875 - 1.61803398875);			// Vector Textures

  // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
  vertices.push_back(-1.0);	vertices.push_back(-1.0);	vertices.push_back(-1.0);					// Vector Vertices
  textures.push_back(0.0); textures.push_back(0.0);													// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(1.61803398875 - 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(1.61803398875 - 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(1.61803398875 + 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);// Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(1.0); vertices.push_back(-1.0);				// Vector Vertices
  textures.push_back(2.0);			textures.push_back(0.0);										// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
  vertices.push_back(-0.61803398875);	vertices.push_back(0.0); vertices.push_back(-1.61803398875);// Vector Vertices
  textures.push_back(0.61803398875);  textures.push_back(0.61803398875 - 1.61803398875);			// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(1.61803398875 - 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);// Vector Colors
  normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(1.0); vertices.push_back(-1.0);				// Vector Vertices
  textures.push_back(2.0);			textures.push_back(0.0);										// Vector textures

/*
  glBegin ( GL_POLYGON ) ;	// CARA 9
    glNormal3d (  0.525731112119,  0.850650808354,  0.0 ) ; glVertex3d (  1.61803398875,  0.61803398875,  0.0 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
  glEnd () ;
*/
  //vertices.resize(0);		normals.resize(0);		colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
// VAO CARA 9
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(-1.61803398875 + 0.61803398875); textures.push_back(-1.61803398875);			// Vector Textures

  // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(1.0);			vertices.push_back(-1.0);	// Vector Vertices
  textures.push_back(0.0);			textures.push_back(-2.0);										// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0); vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
  textures.push_back(1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0); vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
  textures.push_back(1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
  textures.push_back(1.61803398875);	textures.push_back(0.61803398875);							// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(1.0);			vertices.push_back(1.0);	// Vector Vertices
  textures.push_back(0.0);			textures.push_back(0.0);										// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(-1.61803398875 + 0.61803398875); textures.push_back(-1.61803398875);			// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0); vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
  textures.push_back(1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

   // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(-0.850650808354);	// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(1.0);			vertices.push_back(1.0);	// Vector Vertices
  textures.push_back(0.0);			textures.push_back(0.0);										// Vector textures

/*
  glBegin ( GL_POLYGON ) ;	// CARA 10
    glNormal3d (  0.525731112119, -0.850650808354,  0.0 ) ; 
	glVertex3d (  1.61803398875, -0.61803398875,  0.0 ) ; 
	glVertex3d (  1.0, -1.0,  1.0 ) ; 
	glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ; 
	glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ; 
	glVertex3d (  1.0, -1.0, -1.0 ) ;
  glEnd () ;
*/
// VAO CARA 10
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(-1.61803398875 - 0.61803398875); textures.push_back(-1.61803398875);			// Vector Textures

  // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(-1.0);	vertices.push_back(1.0);			// Vector Vertices
  textures.push_back(-2.0);			textures.push_back(0.0);										// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(0.61803398875);							// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(0.61803398875);							// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(-1.0);			vertices.push_back(-1.0);	// Vector Vertices
  textures.push_back(-2.0);			textures.push_back(-2.0);										// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(-1.61803398875 - 0.61803398875); textures.push_back(-1.61803398875);			// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(0.61803398875);							// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(1.0);			vertices.push_back(-1.0);			vertices.push_back(-1.0);	// Vector Vertices
  textures.push_back(-2.0);			textures.push_back(-2.0);										// Vector textures

/*
  glBegin ( GL_POLYGON ) ;	// CARA 11
    glNormal3d ( -0.525731112119,  0.850650808354,  0.0 ) ; 
	glVertex3d ( -1.61803398875,  0.61803398875,  0.0 ) ; 
	glVertex3d ( -1.0,  1.0,  1.0 ) ; 
	glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ; 
	glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ; 
	glVertex3d ( -1.0,  1.0, -1.0 ) ;
  glEnd () ;
*/
// VAO CARA 11
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(1.61803398875 + 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(1.0);			vertices.push_back(1.0);	// Vector Vertices
  textures.push_back(2.0);			textures.push_back(2.0);										// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
  textures.push_back(1.61803398875); textures.push_back(0.61803398875);								// Vector Textures

   // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
  textures.push_back(1.61803398875); textures.push_back(0.61803398875);								// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
  textures.push_back(1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(1.0);			vertices.push_back(-1.0);	// Vector Vertices
  textures.push_back(2.0);			textures.push_back(0.0);										// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(1.61803398875 + 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

   // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
  textures.push_back(1.61803398875); textures.push_back(0.61803398875);								// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(1.0);			vertices.push_back(-1.0);	// Vector Vertices
  textures.push_back(2.0);			textures.push_back(0.0);										// Vector textures
  
/*
  glBegin ( GL_POLYGON ) ;	// CARA 12
    glNormal3d ( -0.525731112119, -0.850650808354,  0.0 ) ; 
	glVertex3d ( -1.61803398875, -0.61803398875,  0.0 ) ; 
	glVertex3d ( -1.0, -1.0, -1.0 ) ; 
	glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ; 
	glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ; 
	glVertex3d ( -1.0, -1.0,  1.0 ) ;
  glEnd () ;
*/
// VAO CARA 12
  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(1.61803398875 - 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

 // Vèrtex 1
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(-1.0);			vertices.push_back(-1.0);	// Vector Vertices
  textures.push_back(0.0);			textures.push_back(0.0);										// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.00);// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.00);// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

  // Vèrtex 3
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(0.61803398875);							// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(-1.0);			vertices.push_back(1.0);	// Vector Vertices
  textures.push_back(0.0);			textures.push_back(2.0);										// Vector textures

  // Vèrtex 0
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
  textures.push_back(1.61803398875 - 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

  // Vèrtex 2
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.00);// Vector Normals
  vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
  textures.push_back(-1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

  // Vèrtex 4
  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
  normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
  vertices.push_back(-1.0);			vertices.push_back(-1.0);			vertices.push_back(1.0);	// Vector Vertices
  textures.push_back(0.0);			textures.push_back(2.0);										// Vector textures

// ------------------------ VAO
  std::vector <int>::size_type nv = vertices.size();		// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
  dodeVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
  dodeVAO.nVertexs = 108;

  return dodeVAO;
}


// Draws a solid dodecahedron in EBO
CVAO loadglutSolidDodecahedron_EBO(void)
{
	GLint index = 0; // Nombre de vertexs acumulats que defineix index al vertexs en vector indices per a EBO.

// VAO
	CVAO dodeVAO;
	dodeVAO.vaoId = 0;		dodeVAO.vboId = 0;		dodeVAO.eboId = 0;	dodeVAO.nVertexs = 0;	dodeVAO.nIndices = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;				// Definició vector dinàmics per a indices a EBO. 
																//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

//---- DEFINICIÓ COORDENADES TEXTURA
	//Activa_Coordenades_Textura();

  /* Magic Numbers:  It is possible to create a dodecahedron by attaching two pentagons to each face of
   * of a cube.  The coordinates of the points are:
   *   (+-x,0, z); (+-1, 1, 1); (0, z, x )
   * where x = (-1 + sqrt(5))/2, z = (1 + sqrt(5))/2 or
   *       x = 0.61803398875 and z = 1.61803398875.
   */
   //glBegin ( GL_POLYGON ) ; // CARA 1
   //  glNormal3d (  0.0,  0.525731112119,  0.850650808354 ) ; glVertex3d (  0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
   //glEnd () ;


 // VAO CARA 1
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]);// Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);		// Vector Normals
	textures.push_back(1.61803398875); textures.push_back(0.61803398875);									// Vector Textures
	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);		// Vector Vertices

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			// Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
	textures.push_back(2.0); textures.push_back(2.0);														// Vector Textures
	vertices.push_back(-1.0);	vertices.push_back(1.0);	vertices.push_back(1.0);						// Vector Vertices

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
	textures.push_back(0.61803398875); textures.push_back(0.61803398875 + 1.61803398875);					// Vector Textures
	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
	textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 + 1.61803398875);				// Vector Textures
	vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
	textures.push_back(0.0); textures.push_back(0.0);														// Vector textures
	vertices.push_back(1.0);	vertices.push_back(1.0);			vertices.push_back(1.0);				// Vector Vertices

// Vector indices CARA 1
	indices.push_back(index);		indices.push_back(index+1);		indices.push_back(index+2);		// v0-v1-v2
	indices.push_back(index+2);		indices.push_back(index+3);		indices.push_back(index+4);		// v2-v3-v4
	indices.push_back(index);		indices.push_back(index+2);		indices.push_back(index+4);		// v0-v2-v4
	index = index + 5;

  /*
	glBegin ( GL_POLYGON ) ; // CARA 2
	  glNormal3d (  0.0,  0.525731112119, -0.850650808354 ) ; glVertex3d (  0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
	glEnd () ;
  */

  //vertices.resize(0);		normals.resize(0);		colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
// VAO CARA 2
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);		// Vector Vertices
	textures.push_back(1.61803398875); textures.push_back(-0.61803398875);								// Vector Textures

//Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(1.0);	vertices.push_back(1.0);			vertices.push_back(-1.0);				// Vector Vertices
	textures.push_back(0.0);	textures.push_back(-2.0);													// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
	textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 - 1.61803398875);				// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
	textures.push_back(0.61803398875); textures.push_back(0.61803398875 - 1.61803398875);					// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(-1.0);	vertices.push_back(1.0);			vertices.push_back(-1.0);				// Vector Vertices
	textures.push_back(2.0);	textures.push_back(0.0);													// Vector textures

// Vector indices CARA 2
	indices.push_back(index);		indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);		indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	index = index + 5;

  /*
	glBegin ( GL_POLYGON ) ;	// CARA 3
	  glNormal3d (  0.0, -0.525731112119,  0.850650808354 ) ; glVertex3d (  0.0, -1.61803398875,  0.61803398875 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d (  0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d ( -0.61803398875, 0.0,  1.61803398875 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
	glEnd () ;
  */
  // VAO CARA 3
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);		// Vector Vertices
	textures.push_back(-1.61803398875); textures.push_back(0.61803398875);								// Vector Textures

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
	vertices.push_back(1.0);	vertices.push_back(-1.0);	vertices.push_back(1.0);						// Vector Vertices
	textures.push_back(-2.0);	textures.push_back(0.0);													// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
	vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices
	textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 + 1.61803398875);				// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	// Vector Vertices
	textures.push_back(0.61803398875); textures.push_back(0.61803398875 + 1.61803398875);					// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	// Vector Normals
	vertices.push_back(-1.0);	vertices.push_back(-1.0);			vertices.push_back(1.0);				// Vector Vertices
	textures.push_back(0.0);	textures.push_back(2.0);													// Vector textures

// Vector indices CARA 3
	indices.push_back(index);		indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);		indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	index = index + 5;

  /*
	glBegin ( GL_POLYGON ) ;	// CARA 4
	  glNormal3d (  0.0, -0.525731112119, -0.850650808354 ) ; glVertex3d (  0.0, -1.61803398875, -0.61803398875 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( -0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d (  0.61803398875, 0.0, -1.61803398875 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
	glEnd () ;
  */
  // VAO CARA 4
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);		// Vector Vertices
	textures.push_back(-1.61803398875); textures.push_back(-0.61803398875);								// Vector Textures

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(-1.0);	vertices.push_back(-1.0);	vertices.push_back(-1.0);						// Vector Vertices
	textures.push_back(0.0);	textures.push_back(0.0);													// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
	textures.push_back(0.61803398875); textures.push_back(0.61803398875 - 1.61803398875);					// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(0.61803398875);	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	// Vector Vertices
	textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 - 1.61803398875);				// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.0);		normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(1.0);	vertices.push_back(-1.0);			vertices.push_back(-1.0);				// Vector Vertices
	textures.push_back(-2.0);	textures.push_back(-2.0);													// Vector textures

// Vector indices CARA 4
	indices.push_back(index);		indices.push_back(index + 1);	indices.push_back(index + 2);		// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);		// v2-v3-v4
	indices.push_back(index);		indices.push_back(index + 2);	indices.push_back(index + 4);		// v0-v2-v4
	index = index + 5;

  /*
	glBegin ( GL_POLYGON ) ;	// CARA 5
	  glNormal3d (  0.850650808354,  0.0,  0.525731112119 ) ; glVertex3d (  0.61803398875,  0.0,  1.61803398875 ) ; glVertex3d (  1.0, -1.0,  1.0 ) ; glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
	glEnd () ;
  */
  // VAO CARA 5
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
	vertices.push_back(0.61803398875);	vertices.push_back(0.0); vertices.push_back(1.61803398875);	// Vector Vertices
	textures.push_back(-0.61803398875);	textures.push_back(-0.61803398875 + 1.61803398875);			// Vector Textures

// Vèrtex 1 
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	 normals.push_back(0.525731112119);	// Vector Normals
	vertices.push_back(1.0);			vertices.push_back(-1.0); vertices.push_back(1.0);				// Vector Vertices
	textures.push_back(-2.0);			textures.push_back(0.0);										// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
	vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875); vertices.push_back(0.0);// Vector Vertices
	textures.push_back(-1.61803398875 - 0.61803398875);  textures.push_back(-1.61803398875);			// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875); vertices.push_back(0.0);	// Vector Vertices
	textures.push_back(-1.61803398875 + 0.61803398875); textures.push_back(-1.61803398875);			// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
	vertices.push_back(1.0);			vertices.push_back(1.0); vertices.push_back(1.0);				// Vector Vertices
	textures.push_back(0.0);			textures.push_back(0.0);										// Vector textures

// Vector indices CARA 5
	indices.push_back(index);	indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);	indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	index = index + 5;

   /*
	glBegin ( GL_POLYGON ) ;	// CARA 6
	  glNormal3d ( -0.850650808354,  0.0,  0.525731112119 ) ; glVertex3d ( -0.61803398875,  0.0,  1.61803398875 ) ; glVertex3d ( -1.0,  1.0,  1.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d ( -1.0, -1.0,  1.0 ) ;
	glEnd () ;
   */
// VAO CARA 6
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
	vertices.push_back(-0.61803398875);	vertices.push_back(0.0); vertices.push_back(1.61803398875);	// Vector Vertices
	textures.push_back(0.61803398875);	textures.push_back(0.61803398875 + 1.61803398875);			// Vector Textures

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);	// Vector Normals
	vertices.push_back(-1.0);			vertices.push_back(1.0); vertices.push_back(1.0);				// Vector Vertices
	textures.push_back(2.0);			textures.push_back(2.0);										// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);		// Vector Normals
	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875); vertices.push_back(0.0);		// Vector Vertices
	textures.push_back(1.61803398875 + 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);		// Vector Normals
	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875); vertices.push_back(0.0);	// Vector Vertices
	textures.push_back(1.61803398875 - 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(0.525731112119);		// Vector Normals
	vertices.push_back(-1.0);			vertices.push_back(-1.0); vertices.push_back(1.0);				// Vector Vertices
	textures.push_back(0.0);			textures.push_back(2.0);										// Vector textures

// Vector indices CARA 6
	indices.push_back(index);	indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);	indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	index = index + 5;

   /*
	glBegin ( GL_POLYGON ) ;	// CARA 7
	  glNormal3d (  0.850650808354,  0.0, -0.525731112119 ) ; glVertex3d (  0.61803398875,  0.0, -1.61803398875 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d (  1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d (  1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d (  1.0, -1.0, -1.0 ) ;
	glEnd () ;
   */
   //vertices.resize(0);		normals.resize(0);		colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
 // VAO CARA 7
 // Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
	vertices.push_back(0.61803398875);	vertices.push_back(0.0); vertices.push_back(-1.61803398875);	// Vector Vertices
	textures.push_back(-0.61803398875); textures.push_back(-0.61803398875 - 1.61803398875);				// Vector Textures

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
	vertices.push_back(1.0);			vertices.push_back(1.0); vertices.push_back(-1.0);					// Vector Vertices
	textures.push_back(0.0); textures.push_back(-2.0);													// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);	// Vector Vertices
	textures.push_back(-1.61803398875 + 0.61803398875); textures.push_back(-1.61803398875);				// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
	vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);	// Vector Vertices
	textures.push_back(-1.61803398875 - 0.61803398875); textures.push_back(-1.61803398875);				// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);			colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.850650808354);		// Vector Normals
	vertices.push_back(1.0);			vertices.push_back(-1.0); vertices.push_back(-1.0);					// Vector Vertices
	textures.push_back(-2.0); textures.push_back(-2.0);													// Vector textures

// Vector indices CARA 7
	indices.push_back(index);	indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);	indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	index = index + 5;

  /*
	glBegin ( GL_POLYGON ) ;	// CARA 8
	  glNormal3d ( -0.850650808354,  0.0, -0.525731112119 ) ; glVertex3d ( -0.61803398875,  0.0, -1.61803398875 ) ; glVertex3d ( -1.0, -1.0, -1.0 ) ; glVertex3d ( -1.61803398875, -0.61803398875, 0.0 ) ; glVertex3d ( -1.61803398875,  0.61803398875, 0.0 ) ; glVertex3d ( -1.0,  1.0, -1.0 ) ;
	glEnd () ;
  */
  // VAO CARA 8
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
	vertices.push_back(-0.61803398875);	vertices.push_back(0.0); vertices.push_back(-1.61803398875);// Vector Vertices
	textures.push_back(0.61803398875);  textures.push_back(0.61803398875 - 1.61803398875);			// Vector Textures

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
	vertices.push_back(-1.0);	vertices.push_back(-1.0);	vertices.push_back(-1.0);					// Vector Vertices
	textures.push_back(0.0); textures.push_back(0.0);													// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
	textures.push_back(1.61803398875 - 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);// Vector Vertices
	textures.push_back(1.61803398875 + 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);// Vector Colors
	normals.push_back(-0.850650808354);	normals.push_back(0.0);	normals.push_back(-0.525731112119);	// Vector Normals
	vertices.push_back(-1.0);			vertices.push_back(1.0); vertices.push_back(-1.0);				// Vector Vertices
	textures.push_back(2.0);			textures.push_back(0.0);										// Vector textures

// Vector indices CARA 8
	indices.push_back(index);	indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);	indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	index = index + 5;

  /*
	glBegin ( GL_POLYGON ) ;	// CARA 9
	  glNormal3d (  0.525731112119,  0.850650808354,  0.0 ) ; glVertex3d (  1.61803398875,  0.61803398875,  0.0 ) ; glVertex3d (  1.0,  1.0, -1.0 ) ; glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ; glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ; glVertex3d (  1.0,  1.0,  1.0 ) ;
	glEnd () ;
  */
  //vertices.resize(0);		normals.resize(0);		colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
// VAO CARA 9
 // Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);// Vector Vertices
	textures.push_back(-1.61803398875 + 0.61803398875); textures.push_back(-1.61803398875);			// Vector Textures

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(1.0);			vertices.push_back(1.0);			vertices.push_back(-1.0);	// Vector Vertices
	textures.push_back(0.0);			textures.push_back(-2.0);										// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(0.0); vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
	textures.push_back(1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
	textures.push_back(1.61803398875);	textures.push_back(0.61803398875);							// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(-0.850650808354);	// Vector Normals
	vertices.push_back(1.0);			vertices.push_back(1.0);			vertices.push_back(1.0);	// Vector Vertices
	textures.push_back(0.0);			textures.push_back(0.0);										// Vector textures

// Vector indices CARA 9
	indices.push_back(index);		indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);		indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	index = index + 5;

  /*
	glBegin ( GL_POLYGON ) ;	// CARA 10
	  glNormal3d (  0.525731112119, -0.850650808354,  0.0 ) ;
	  glVertex3d (  1.61803398875, -0.61803398875,  0.0 ) ;
	  glVertex3d (  1.0, -1.0,  1.0 ) ;
	  glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ;
	  glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ;
	  glVertex3d (  1.0, -1.0, -1.0 ) ;
	glEnd () ;
  */
  // VAO CARA 10
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
	textures.push_back(-1.61803398875 - 0.61803398875); textures.push_back(-1.61803398875);			// Vector Textures

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);		// Vector Normals
	vertices.push_back(1.0);			vertices.push_back(-1.0);	vertices.push_back(1.0);			// Vector Vertices
	textures.push_back(-2.0);			textures.push_back(0.0);										// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);		// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
	textures.push_back(-1.61803398875); textures.push_back(0.61803398875);								// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);		// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
	textures.push_back(-1.61803398875); textures.push_back(-0.61803398875);								// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(1.0);			vertices.push_back(-1.0);			vertices.push_back(-1.0);	// Vector Vertices
	textures.push_back(-2.0);			textures.push_back(-2.0);										// Vector textures

// Vector indices CARA 10
	indices.push_back(index);		indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);		indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	index = index + 5;

  /*
	glBegin ( GL_POLYGON ) ;	// CARA 11
	  glNormal3d ( -0.525731112119,  0.850650808354,  0.0 ) ;
	  glVertex3d ( -1.61803398875,  0.61803398875,  0.0 ) ;
	  glVertex3d ( -1.0,  1.0,  1.0 ) ;
	  glVertex3d ( 0.0,  1.61803398875,  0.61803398875 ) ;
	  glVertex3d ( 0.0,  1.61803398875, -0.61803398875 ) ;
	  glVertex3d ( -1.0,  1.0, -1.0 ) ;
	glEnd () ;
  */
// VAO CARA 11
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	vertices.push_back(0.0);// Vector Vertices
	textures.push_back(1.61803398875 + 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(-1.0);			vertices.push_back(1.0);			vertices.push_back(1.0);	// Vector Vertices
	textures.push_back(2.0);			textures.push_back(2.0);										// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
	textures.push_back(1.61803398875); textures.push_back(0.61803398875);								// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
	textures.push_back(1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(-1.0);			vertices.push_back(1.0);			vertices.push_back(-1.0);	// Vector Vertices
	textures.push_back(2.0);			textures.push_back(0.0);										// Vector textures

// Vector indices CARA 11
	indices.push_back(index);	indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);	indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	index = index + 5;

  /*
	glBegin ( GL_POLYGON ) ;	// CARA 12
	  glNormal3d ( -0.525731112119, -0.850650808354,  0.0 ) ;
	  glVertex3d ( -1.61803398875, -0.61803398875,  0.0 ) ;
	  glVertex3d ( -1.0, -1.0, -1.0 ) ;
	  glVertex3d ( 0.0, -1.61803398875, -0.61803398875 ) ;
	  glVertex3d ( 0.0, -1.61803398875,  0.61803398875 ) ;
	  glVertex3d ( -1.0, -1.0,  1.0 ) ;
	glEnd () ;
  */
// VAO CARA 12
// Vèrtex 0
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	vertices.push_back(0.0);// Vector Vertices
	textures.push_back(1.61803398875 - 0.61803398875); textures.push_back(1.61803398875);				// Vector Textures

// Vèrtex 1
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(-1.0);			vertices.push_back(-1.0);			vertices.push_back(-1.0);	// Vector Vertices
	textures.push_back(0.0);			textures.push_back(0.0);										// Vector Textures

// Vèrtex 2
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.00);// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(-0.61803398875);	// Vector Vertices
	textures.push_back(-1.61803398875); textures.push_back(-0.61803398875);							// Vector Textures

// Vèrtex 3
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(0.0);	vertices.push_back(-1.61803398875);	vertices.push_back(0.61803398875);	// Vector Vertices
	textures.push_back(-1.61803398875); textures.push_back(0.61803398875);							// Vector Textures

// Vèrtex 4
	colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
	normals.push_back(-0.525731112119);	normals.push_back(-0.850650808354);	normals.push_back(0.0);	// Vector Normals
	vertices.push_back(-1.0);			vertices.push_back(-1.0);			vertices.push_back(1.0);	// Vector Vertices
	textures.push_back(0.0);			textures.push_back(2.0);										// Vector textures

// Vector indices CARA 12
	indices.push_back(index);	indices.push_back(index + 1);	indices.push_back(index + 2);	// v0-v1-v2
	indices.push_back(index + 2);	indices.push_back(index + 3);	indices.push_back(index + 4);	// v2-v3-v4
	indices.push_back(index);	indices.push_back(index + 2);	indices.push_back(index + 4);	// v0-v2-v4
	//index = index + 5;

// ------------------------ VAO
	//std::vector <int>::size_type nv = vertices.size();		// Tamany del vector vertices en elements.

 // Creació d'un VAO, VBO i EBO carregant de la geometria. Guardar identificadors VAO, VBO, EBO, número de vertex i indices a struct CVAO.
	dodeVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures,indices);

	return dodeVAO;
}
// ---------- FI DODECAHEDRON-----------------------------------------------------


// ---------- OCTAHEDRON ---------------------------------------------------------

// Draws a solid octahedron
void glutSolidOctahedron(void)
{
	Set_VAOList(GLUT_OCTAHEDRON, loadglutSolidOctahedron_VAO());
	draw_TriVAO_Object(GLUT_OCTAHEDRON); // drawSolidOctahedron();
	deleteVAOList(GLUT_OCTAHEDRON);
}


// Draws a solid octahedron as VAO
CVAO loadglutSolidOctahedron_VAO(void)
{
// VAO
	CVAO octaVAO;	
	octaVAO.vaoId = 0;	octaVAO.vboId = 0;		octaVAO.nVertexs = 0;

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

#define RADIUS    1.0f
// vertex coords array for glDrawArrays() =====================================
// A octahedron has 8 sides and each side has 1 triangle, therefore, a octahedron consists
// of 24 vertices (8 sides * 1 triangle * 3 vertices = 24 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 72 floats (24 * 3 = 72).
	//GLdouble vertices[] = {
	std::vector<double> vertices = { 
		 RADIUS, 0.0, 0.0, 0.0,	 RADIUS,   0.0,   0.0,   0.0,   RADIUS,		// v0-v1-v2 (top +++)
		 RADIUS, 0.0, 0.0, 0.0,		0.0, -RADIUS, 0.0,  RADIUS,	  0.0, 		// v0-v4-v1 (top ++-)
		 RADIUS, 0.0, 0.0, 0.0,		0.0,  RADIUS, 0.0, -RADIUS,   0.0,		// v0-v2-v5 (top +-+)
		 RADIUS, 0.0, 0.0, 0.0, -RADIUS,	0.0,  0.0,   0.0,  -RADIUS,     // v0-v5-v4 (top +--)
		-RADIUS, 0.0, 0.0, 0.0,		0.0,  RADIUS, 0.0,  RADIUS,   0.0,      // v7-v2-v1 (bottom -++)
		-RADIUS, 0.0, 0.0, 0.0,  RADIUS,	0.0,  0.0,   0.0,  -RADIUS,     // v7-v1-v4 (bottom -+-)
		-RADIUS, 0.0, 0.0, 0.0, -RADIUS,	0.0,  0.0,	 0.0,   RADIUS,     // v7-v5-v2 (bottom --+)
		-RADIUS, 0.0, 0.0, 0.0,		0.0, -RADIUS, 0.0, -RADIUS,   0.0    }; // v7-v4-v5 (bottom ---)

// Normal array
	//GLdouble normals[] = { 
	std::vector<double> normals = {
		 0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,    // v0-v1-v2 (top +++)
		 0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,	// v0-v4-v1 (top ++-)
		 0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,	// v0-v2-v5 (top +-+)
		 0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189, 	// v0-v5-v4 (top +--)
		-0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, 	// v7-v2-v1 (bottom -++)
		-0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, 	// v7-v1-v4 (bottom -+-)
		-0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189,    // v7-v5-v2 (bottom --+)
		-0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189 };	// v7-v4-v5 (bottom ---)

// Color array
	//GLdouble colors[] = { 
	std::vector<double> colors = {
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v0-v1-v2 (top +++)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v0-v4-v1 (top ++-)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v0-v2-v5 (top +-+)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v0-v5-v4 (top +--)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v7-v2-v1 (bottom -++)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v7-v1-v4 (bottom -+-)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v7-v5-v2 (bottom --+)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3] }; // v7-v4-v5 (bottom ---)

// Texture Array
	//GLdouble textures[] = { 
	std::vector<double> textures = {
		-RADIUS, -RADIUS,  RADIUS,	 0.0,     0.0,    RADIUS,	// v0-v1-v2 (top +++)
		-RADIUS, -RADIUS,   0.0,   -RADIUS,  RADIUS,    0.0,	// v0-v4-v1 (top ++-)
		-RADIUS, -RADIUS,   0.0,    RADIUS, -RADIUS,    0.0,	// v0-v2-v5 (top + -+)
		-RADIUS, -RADIUS, -RADIUS,   0.0,     0.0,   -RADIUS,	// v0-v5-v4 (top +--)
		 RADIUS,  RADIUS,   0.0,    RADIUS,  RADIUS,    0.0,	// v7-v2-v1 (bottom -++)
		 RADIUS,  RADIUS,  RADIUS,   0.0,     0.0,   -RADIUS,	// v7-v1-v4 (bottom -+-)
		 RADIUS,  RADIUS, -RADIUS,   0.0,     0.0,    RADIUS,	// v7-v5-v2 (bottom --+)
		 RADIUS,  RADIUS, 0.0,     -RADIUS, -RADIUS,    0.0 };	// v7-v4-v5 (bottom ---)
#undef RADIUS

/*
  glBegin( GL_TRIANGLES );
    glNormal3d( 0.577350269189, 0.577350269189, 0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, RADIUS, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS );
    glNormal3d( 0.577350269189, 0.577350269189,-0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS ); glVertex3d( 0.0, RADIUS, 0.0 );
    glNormal3d( 0.577350269189,-0.577350269189, 0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS ); glVertex3d( 0.0,-RADIUS, 0.0 );
    glNormal3d( 0.577350269189,-0.577350269189,-0.577350269189); glVertex3d( RADIUS, 0.0, 0.0 ); glVertex3d( 0.0,-RADIUS, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS );
    glNormal3d(-0.577350269189, 0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS ); glVertex3d( 0.0, RADIUS, 0.0 );
    glNormal3d(-0.577350269189, 0.577350269189,-0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, RADIUS, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS );
    glNormal3d(-0.577350269189,-0.577350269189, 0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0,-RADIUS, 0.0 ); glVertex3d( 0.0, 0.0, RADIUS );
    glNormal3d(-0.577350269189,-0.577350269189,-0.577350269189); glVertex3d(-RADIUS, 0.0, 0.0 ); glVertex3d( 0.0, 0.0,-RADIUS ); glVertex3d( 0.0,-RADIUS, 0.0 );
  glEnd();
*/

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	octaVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//octaVAO.nVertexs = 24;

	return octaVAO;
}


// Draws a solid octahedron as EBO
CVAO loadglutSolidOctahedron_EBO(void)
{
	// VAO
	CVAO octaVAO;
	octaVAO.vaoId = 0;	octaVAO.vboId = 0;		octaVAO.nVertexs = 0;

	// Obtenir color actual definit en OpenGL amb glColor();
		//GLdouble cColor[4];
		//glGetDoublev(GL_CURRENT_COLOR, cColor);

#define RADIUS    1.0f
// vertex coords array for glDrawArrays() =====================================
// A octahedron has 8 sides and each side has 1 triangle, therefore, a octahedron consists
// of 24 vertices (8 sides * 1 triangle * 3 vertices = 24 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 72 floats (24 * 3 = 72).
	//GLdouble vertices[] = {
	std::vector<double> vertices = {
		 RADIUS, 0.0, 0.0, 0.0,	 RADIUS,   0.0,   0.0,   0.0,   RADIUS,		// v0-v1-v2 (top +++)
		 RADIUS, 0.0, 0.0, 0.0,		0.0, -RADIUS, 0.0,  RADIUS,	  0.0, 		// v0-v4-v1 (top ++-)
		 RADIUS, 0.0, 0.0, 0.0,		0.0,  RADIUS, 0.0, -RADIUS,   0.0,		// v0-v2-v5 (top +-+)
		 RADIUS, 0.0, 0.0, 0.0, -RADIUS,	0.0,  0.0,   0.0,  -RADIUS,     // v0-v5-v4 (top +--)
		-RADIUS, 0.0, 0.0, 0.0,		0.0,  RADIUS, 0.0,  RADIUS,   0.0,      // v7-v2-v1 (bottom -++)
		-RADIUS, 0.0, 0.0, 0.0,  RADIUS,	0.0,  0.0,   0.0,  -RADIUS,     // v7-v1-v4 (bottom -+-)
		-RADIUS, 0.0, 0.0, 0.0, -RADIUS,	0.0,  0.0,	 0.0,   RADIUS,     // v7-v5-v2 (bottom --+)
		-RADIUS, 0.0, 0.0, 0.0,		0.0, -RADIUS, 0.0, -RADIUS,   0.0 };	// v7-v4-v5 (bottom ---)

// Normal array
	//GLdouble normals[] = { 
	std::vector<double> normals = {
		 0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,  0.577350269189,    // v0-v1-v2 (top +++)
		 0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,	// v0-v4-v1 (top ++-)
		 0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,	// v0-v2-v5 (top +-+)
		 0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189, 	// v0-v5-v4 (top +--)
		-0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, -0.577350269189,  0.577350269189,  0.577350269189, 	// v7-v2-v1 (bottom -++)
		-0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, 	// v7-v1-v4 (bottom -+-)
		-0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189, -0.577350269189, -0.577350269189,  0.577350269189,    // v7-v5-v2 (bottom --+)
		-0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189, -0.577350269189 };	// v7-v4-v5 (bottom ---)

// Color array
	//GLdouble colors[] = { 
	std::vector<double> colors = {
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v0-v1-v2 (top +++)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v0-v4-v1 (top ++-)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v0-v2-v5 (top +-+)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v0-v5-v4 (top +--)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v7-v2-v1 (bottom -++)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v7-v1-v4 (bottom -+-)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],  // v7-v5-v2 (bottom --+)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3] }; // v7-v4-v5 (bottom ---)

// Texture Array
	//GLdouble textures[] = { 
	std::vector<double> textures = {
		-RADIUS, -RADIUS,  RADIUS,	 0.0,     0.0,    RADIUS,	// v0-v1-v2 (top +++)
		-RADIUS, -RADIUS,   0.0,   -RADIUS,  RADIUS,    0.0,	// v0-v4-v1 (top ++-)
		-RADIUS, -RADIUS,   0.0,    RADIUS, -RADIUS,    0.0,	// v0-v2-v5 (top + -+)
		-RADIUS, -RADIUS, -RADIUS,   0.0,     0.0,   -RADIUS,	// v0-v5-v4 (top +--)
		 RADIUS,  RADIUS,   0.0,    RADIUS,  RADIUS,    0.0,	// v7-v2-v1 (bottom -++)
		 RADIUS,  RADIUS,  RADIUS,   0.0,     0.0,   -RADIUS,	// v7-v1-v4 (bottom -+-)
		 RADIUS,  RADIUS, -RADIUS,   0.0,     0.0,    RADIUS,	// v7-v5-v2 (bottom --+)
		 RADIUS,  RADIUS, 0.0,     -RADIUS, -RADIUS,    0.0 };	// v7-v4-v5 (bottom ---)

// Indices Array
	//GLdouble textures[] = { 
	std::vector<uint> indices = {
		0, 1, 2,	// v0-v1-v2 (top +++)
		3, 4, 5,	// v0-v4-v1 (top ++-)
		6, 7, 8,	// v0-v2-v5 (top + -+)
		9, 10, 11,	// v0-v5-v4 (top +--)
		12, 13, 14,	// v7-v2-v1 (bottom -++)
		15, 16, 17,	// v7-v1-v4 (bottom -+-)
		18, 19, 20,	// v7-v5-v2 (bottom --+)
		21, 22, 23 };	// v7-v4-v5 (bottom ---)
#undef RADIUS

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	octaVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures,indices);
	//octaVAO.nVertexs = 24;

	return octaVAO;
}
// ---------- FI OCTAHEDRON-------------------------------------------------------


// ---------- TETRAHEDRON --------------------------------------------------------

// Draws a solid tetrahedron
void glutSolidTetrahedron(void)
{
	Set_VAOList(GLUT_TETRAHEDRON, loadglutSolidTetrahedron_VAO());
	draw_TriVAO_Object(GLUT_TETRAHEDRON);
	deleteVAOList(GLUT_TETRAHEDRON);
}

// Draws a solid tetrahedron in VAO
CVAO loadglutSolidTetrahedron_VAO( void )
{
// VAO
	CVAO tetraVAO;		
	tetraVAO.vaoId = 0;		tetraVAO.vboId = 0;		tetraVAO.nVertexs = 0;

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

/*
  glBegin( GL_TRIANGLES ) ;
	glNormal3d ( -tet_r[0][0], -tet_r[0][1], -tet_r[0][2] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[2] ) ;
	glNormal3d ( -tet_r[1][0], -tet_r[1][1], -tet_r[1][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[2] ) ; glVertex3dv ( tet_r[3] ) ;
	glNormal3d ( -tet_r[2][0], -tet_r[2][1], -tet_r[2][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[3] ) ; glVertex3dv ( tet_r[1] ) ;
	glNormal3d ( -tet_r[3][0], -tet_r[3][1], -tet_r[3][2] ) ; glVertex3dv ( tet_r[0] ) ; glVertex3dv ( tet_r[1] ) ; glVertex3dv ( tet_r[2] ) ;
  glEnd() ;
*/

// vertex coords array for glDrawArrays() =====================================
// A tetrahedron has 4 sides and each side has 1 triangle, therefore, a tetraahedron consists
// of 12 vertices (4 sides * 1 triangle * 3 vertices = 12 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 36 floats (12 * 3 = 36).
	//GLdouble vertices[] = { 
	std::vector<double> vertices = {
		tet_r[1][0], tet_r[1][1], tet_r[1][2], tet_r[3][0], tet_r[3][1], tet_r[3][2], tet_r[2][0], tet_r[2][1], tet_r[2][2],   // v1-v3-v2 (front)
		tet_r[0][0], tet_r[0][1], tet_r[0][2], tet_r[2][0], tet_r[2][1], tet_r[2][2], tet_r[3][0], tet_r[3][1], tet_r[3][2],   // v0-v2-v3 (bottom)
		tet_r[0][0], tet_r[0][1], tet_r[0][2], tet_r[3][0], tet_r[3][1], tet_r[3][2], tet_r[1][0], tet_r[1][1], tet_r[1][2],   // v0-v3-v1 (bottom)
		tet_r[0][0], tet_r[0][1], tet_r[0][2], tet_r[1][0], tet_r[1][1], tet_r[1][2], tet_r[2][0], tet_r[2][1], tet_r[2][2] }; // v0-v1-v2 (back)


// Normal array
	//GLdouble normals[] = { 
	std::vector<double>  normals = {
		-tet_r[0][0], -tet_r[0][1], -tet_r[0][2], -tet_r[0][0], -tet_r[0][1], -tet_r[0][2], -tet_r[0][0], -tet_r[0][1], -tet_r[0][2],   // v1-v3-v2 (front)
		 tet_r[1][0], -tet_r[1][1], -tet_r[1][2], tet_r[1][0], -tet_r[1][1], -tet_r[1][2], tet_r[1][0], -tet_r[1][1], -tet_r[1][2], 		// v0-v2-v3 (bottom)
		-tet_r[2][0], -tet_r[2][1], -tet_r[2][2], -tet_r[2][0], -tet_r[2][1], -tet_r[2][2], -tet_r[2][0], -tet_r[2][1], -tet_r[2][2],	// v0-v3-v1 (bottom)
		-tet_r[3][0], -tet_r[3][1], -tet_r[3][2], -tet_r[3][0], -tet_r[3][1], -tet_r[3][2], -tet_r[3][0], -tet_r[3][1], -tet_r[3][2] };	// v0-v1-v2 (back)

// Color array
	//GLdouble colors[] = { 
	std::vector<double> colors = {
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],		// v1-v3-v2 (front)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],		// v0-v2-v3 (bottom)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],		// v0-v3-v1 (bottom)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3] };	// v0-v1-v2 (back)

// Texture Array
	//GLdouble textures[] = {
	std::vector<double>  textures = {
		-tet_r[1][0] + tet_r[1][1], -tet_r[1][0] + tet_r[1][2], -tet_r[3][0] + tet_r[3][1], -tet_r[3][0] + tet_r[3][2], -tet_r[2][0] + tet_r[2][1], -tet_r[2][0] + tet_r[2][2],    // v1-v3-v2 (front)
		-tet_r[0][0] + tet_r[0][1], -tet_r[0][0] + tet_r[0][2], -tet_r[2][0] + tet_r[2][1], -tet_r[2][0] + tet_r[2][2], -tet_r[3][0] + tet_r[3][1], -tet_r[3][0] + tet_r[3][2],   // v0-v2-v3 (right)
		-tet_r[0][0] + tet_r[0][1], -tet_r[0][0] + tet_r[0][2], -tet_r[3][0] + tet_r[3][1], -tet_r[3][0] + tet_r[3][2], -tet_r[1][0] + tet_r[1][1], -tet_r[1][0] + tet_r[1][2],    // v0-v3-v1 (top)
		-tet_r[0][0] + tet_r[0][1], -tet_r[0][0] + tet_r[0][2], -tet_r[1][0] + tet_r[1][1], -tet_r[1][0] + tet_r[1][2], -tet_r[2][0] + tet_r[2][1], -tet_r[2][0] + tet_r[2][2] };  // v0-v1-v2 (back)

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	tetraVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//tetraVAO.nVertexs = 12;

	return tetraVAO;
}


// Draws a solid tetrahedron in EBO
CVAO loadglutSolidTetrahedron_EBO(void)
{
	// VAO
	CVAO tetraVAO;
	tetraVAO.vaoId = 0;		tetraVAO.vboId = 0;		tetraVAO.nVertexs = 0;

	// Obtenir color actual definit en OpenGL amb glColor();
		//GLfloat cColor[4];
		//glGetFloatv(GL_CURRENT_COLOR, cColor);

// vertex coords array for glDrawArrays() =====================================
// A tetrahedron has 4 sides and each side has 1 triangle, therefore, a tetraahedron consists
// of 12 vertices (4 sides * 1 triangle * 3 vertices = 12 vertices). And, each
// vertex is 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 36 floats (12 * 3 = 36).
	//GLdouble vertices[] = { 
	std::vector<double> vertices = {
		tet_r[1][0], tet_r[1][1], tet_r[1][2], tet_r[3][0], tet_r[3][1], tet_r[3][2], tet_r[2][0], tet_r[2][1], tet_r[2][2],   // v1-v3-v2 (front)
		tet_r[0][0], tet_r[0][1], tet_r[0][2], tet_r[2][0], tet_r[2][1], tet_r[2][2], tet_r[3][0], tet_r[3][1], tet_r[3][2],   // v0-v2-v3 (bottom)
		tet_r[0][0], tet_r[0][1], tet_r[0][2], tet_r[3][0], tet_r[3][1], tet_r[3][2], tet_r[1][0], tet_r[1][1], tet_r[1][2],   // v0-v3-v1 (bottom)
		tet_r[0][0], tet_r[0][1], tet_r[0][2], tet_r[1][0], tet_r[1][1], tet_r[1][2], tet_r[2][0], tet_r[2][1], tet_r[2][2] }; // v0-v1-v2 (back)


// Normal array
	//GLdouble normals[] = { 
	std::vector<double>  normals = {
		-tet_r[0][0], -tet_r[0][1], -tet_r[0][2], -tet_r[0][0], -tet_r[0][1], -tet_r[0][2], -tet_r[0][0], -tet_r[0][1], -tet_r[0][2],   // v1-v3-v2 (front)
		 tet_r[1][0], -tet_r[1][1], -tet_r[1][2], tet_r[1][0], -tet_r[1][1], -tet_r[1][2], tet_r[1][0], -tet_r[1][1], -tet_r[1][2], 		// v0-v2-v3 (bottom)
		-tet_r[2][0], -tet_r[2][1], -tet_r[2][2], -tet_r[2][0], -tet_r[2][1], -tet_r[2][2], -tet_r[2][0], -tet_r[2][1], -tet_r[2][2],	// v0-v3-v1 (bottom)
		-tet_r[3][0], -tet_r[3][1], -tet_r[3][2], -tet_r[3][0], -tet_r[3][1], -tet_r[3][2], -tet_r[3][0], -tet_r[3][1], -tet_r[3][2] };	// v0-v1-v2 (back)

// Color array
	//GLdouble colors[] = { 
	std::vector<double> colors = {
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],		// v1-v3-v2 (front)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],		// v0-v2-v3 (bottom)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],		// v0-v3-v1 (bottom)
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3] };	// v0-v1-v2 (back)

// Texture Array
	//GLdouble textures[] = {
	std::vector<double>  textures = {
		-tet_r[1][0] + tet_r[1][1], -tet_r[1][0] + tet_r[1][2], -tet_r[3][0] + tet_r[3][1], -tet_r[3][0] + tet_r[3][2], -tet_r[2][0] + tet_r[2][1], -tet_r[2][0] + tet_r[2][2],    // v1-v3-v2 (front)
		-tet_r[0][0] + tet_r[0][1], -tet_r[0][0] + tet_r[0][2], -tet_r[2][0] + tet_r[2][1], -tet_r[2][0] + tet_r[2][2], -tet_r[3][0] + tet_r[3][1], -tet_r[3][0] + tet_r[3][2],   // v0-v2-v3 (right)
		-tet_r[0][0] + tet_r[0][1], -tet_r[0][0] + tet_r[0][2], -tet_r[3][0] + tet_r[3][1], -tet_r[3][0] + tet_r[3][2], -tet_r[1][0] + tet_r[1][1], -tet_r[1][0] + tet_r[1][2],    // v0-v3-v1 (top)
		-tet_r[0][0] + tet_r[0][1], -tet_r[0][0] + tet_r[0][2], -tet_r[1][0] + tet_r[1][1], -tet_r[1][0] + tet_r[1][2], -tet_r[2][0] + tet_r[2][1], -tet_r[2][0] + tet_r[2][2] };  // v0-v1-v2 (back)

// indices Array
	//GLuint indices[] = {
	std::vector<uint>  indices = {
		0, 1, 2,		// v1-v3-v2 (front)
		3, 4, 5,		// v0-v2-v3 (right)
		6, 7, 8,		// v0-v3-v1 (top)
		9, 10, 11 };	// v0-v1-v2 (back)

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	tetraVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	//tetraVAO.nVertexs = 12;

	return tetraVAO;
}
// ---------- FI TETRAHEDRON -----------------------------------------------------


// ---------- ICOSAHEDRON --------------------------------------------------------

// Draws a solid Icosahedron
void glutSolidIcosahedron(void)
{
	Set_VAOList(GLUT_ICOSAHEDRON, loadglutSolidIcosahedron_VAO());
	draw_TriVAO_Object(GLUT_ICOSAHEDRON); // drawSolidIcosahedron();
	deleteVAOList(GLUT_ICOSAHEDRON);
}


// Draws a solid Icosahedron in VAO
CVAO loadglutSolidIcosahedron_VAO( void )
{
  int i ;
  double normal[3];

 // VAO
  CVAO icosVAO;
  icosVAO.vaoId = 0;		icosVAO.vboId = 0;		icosVAO.nVertexs = 0;

  std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
  //vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
  vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors

 // Obtenir color actual definit en OpenGL amb glColor();
  //GLdouble cColor[4];
  //glGetDoublev(GL_CURRENT_COLOR, cColor);

//---- DEFINICIÓ COORDENADES TEXTURA
//  Activa_Coordenades_Textura();

  //glBegin ( GL_TRIANGLES ) ;
    for ( i = 0; i < 20; i++ )
	{ //double normal[3];
      normal[0] = ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) - ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) ;
      normal[1] = ( icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) - ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2] ) ;
      normal[2] = ( icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0] ) * ( icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1] ) - ( icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1] ) * ( icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0] ) ;
      //glNormal3dv ( normal );
      //glVertex3dv ( icos_r[icos_v[i][0]] );
	  colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
	  normals.push_back(normal[0]);					normals.push_back(normal[1]);					normals.push_back(normal[2]);						// Vector Normals
	  vertices.push_back(icos_r[icos_v[i][0]][0]);	vertices.push_back(icos_r[icos_v[i][0]][1]);	vertices.push_back(icos_r[icos_v[i][0]][2]);		// Vector Vertices
	  textures.push_back(-icos_r[icos_v[i][0]][0] + icos_r[icos_v[i][0]][1]); textures.push_back(-icos_r[icos_v[i][0]][0] + icos_r[icos_v[i][0]][2]);	// Vector Textures

      //glVertex3dv ( icos_r[icos_v[i][1]] );
	  colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
	  normals.push_back(normal[0]);					normals.push_back(normal[1]);					normals.push_back(normal[2]);						// Vector Normals
	  vertices.push_back(icos_r[icos_v[i][1]][0]);	vertices.push_back(icos_r[icos_v[i][1]][1]);	vertices.push_back(icos_r[icos_v[i][1]][2]);		// Vector Vertices
	  textures.push_back(-icos_r[icos_v[i][1]][0] + icos_r[icos_v[i][1]][1]); textures.push_back(-icos_r[icos_v[i][1]][0] + icos_r[icos_v[i][1]][2]);	// Vector Textures
	  
	  //glVertex3dv ( icos_r[icos_v[i][2]] );
	  colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
	  normals.push_back(normal[0]);					normals.push_back(normal[1]);					normals.push_back(normal[2]);						// Vector Normals
	  vertices.push_back(icos_r[icos_v[i][2]][0]);	vertices.push_back(icos_r[icos_v[i][2]][1]);	vertices.push_back(icos_r[icos_v[i][2]][2]);		// Vector Vertices
	  textures.push_back(-icos_r[icos_v[i][2]][0] + icos_r[icos_v[i][2]][1]); textures.push_back(-icos_r[icos_v[i][2]][0] + icos_r[icos_v[i][2]][2]);	// Vector Textures
    }
  //glEnd () ;

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	icosVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	icosVAO.nVertexs = 60;

//---- DEFINICIÓ COORDENADES TEXTURA
  //Desactiva_Coordenades_Textura();
	return icosVAO;
}


// Draws a solid Icosahedron in EBO
CVAO loadglutSolidIcosahedron_EBO(void)
{
	int i;
	double normal[3];

	GLuint index = 0;	// Apuntador a vèrtexs per a indices.

// VAO
	CVAO icosVAO;
	icosVAO.vaoId = 0;		icosVAO.vboId = 0;		icosVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;									// Definició vector dinàmic d'índexs per a EBO
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

  //---- DEFINICIÓ COORDENADES TEXTURA
  //  Activa_Coordenades_Textura();

	//glBegin ( GL_TRIANGLES ) ;
	for (i = 0; i < 20; i++)
	{ //double normal[3];
		normal[0] = (icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1]) * (icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2]) - (icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2]) * (icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1]);
		normal[1] = (icos_r[icos_v[i][1]][2] - icos_r[icos_v[i][0]][2]) * (icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0]) - (icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0]) * (icos_r[icos_v[i][2]][2] - icos_r[icos_v[i][0]][2]);
		normal[2] = (icos_r[icos_v[i][1]][0] - icos_r[icos_v[i][0]][0]) * (icos_r[icos_v[i][2]][1] - icos_r[icos_v[i][0]][1]) - (icos_r[icos_v[i][1]][1] - icos_r[icos_v[i][0]][1]) * (icos_r[icos_v[i][2]][0] - icos_r[icos_v[i][0]][0]);
		//glNormal3dv ( normal );
		//glVertex3dv ( icos_r[icos_v[i][0]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(normal[0]);					normals.push_back(normal[1]);					normals.push_back(normal[2]);						// Vector Normals
		vertices.push_back(icos_r[icos_v[i][0]][0]);	vertices.push_back(icos_r[icos_v[i][0]][1]);	vertices.push_back(icos_r[icos_v[i][0]][2]);		// Vector Vertices
		textures.push_back(-icos_r[icos_v[i][0]][0] + icos_r[icos_v[i][0]][1]); textures.push_back(-icos_r[icos_v[i][0]][0] + icos_r[icos_v[i][0]][2]);	// Vector Textures

		//glVertex3dv ( icos_r[icos_v[i][1]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(normal[0]);					normals.push_back(normal[1]);					normals.push_back(normal[2]);						// Vector Normals
		vertices.push_back(icos_r[icos_v[i][1]][0]);	vertices.push_back(icos_r[icos_v[i][1]][1]);	vertices.push_back(icos_r[icos_v[i][1]][2]);		// Vector Vertices
		textures.push_back(-icos_r[icos_v[i][1]][0] + icos_r[icos_v[i][1]][1]); textures.push_back(-icos_r[icos_v[i][1]][0] + icos_r[icos_v[i][1]][2]);	// Vector Textures

		//glVertex3dv ( icos_r[icos_v[i][2]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(normal[0]);					normals.push_back(normal[1]);					normals.push_back(normal[2]);						// Vector Normals
		vertices.push_back(icos_r[icos_v[i][2]][0]);	vertices.push_back(icos_r[icos_v[i][2]][1]);	vertices.push_back(icos_r[icos_v[i][2]][2]);		// Vector Vertices
		textures.push_back(-icos_r[icos_v[i][2]][0] + icos_r[icos_v[i][2]][1]); textures.push_back(-icos_r[icos_v[i][2]][0] + icos_r[icos_v[i][2]][2]);	// Vector Textures
	
		// Vector indices CARA i
		indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 2);	// v0,v1,v2
		index = index + 3;
	
	}
	//glEnd () ;

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	icosVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	//icosVAO.nVertexs = 60;

//---- DEFINICIÓ COORDENADES TEXTURA
	//Desactiva_Coordenades_Textura();
	return icosVAO;
}
// ---------- FI ICOSAHEDRON------------------------------------------------------


// ---------- ROMBIC DODECAHEDRON ------------------------------------------------

// Draws a solid Rhombic Dodecahedron
void glutSolidRhombicDodecahedron(void)
{
	Set_VAOList(GLUT_RHOMBICDODECAHEDRON, loadglutSolidRhombicDodecahedron_EBO());
	draw_TriEBO_Object(GLUT_RHOMBICDODECAHEDRON); // drawSolidRhombicDodecahedron();
	deleteVAOList(GLUT_RHOMBICDODECAHEDRON);
}

/*
* Loads a solid Rhombic Dodecahedron in VAO
 */
CVAO loadglutSolidRhombicDodecahedron_VAO( void )
{
  int i ;

// VAO
  CVAO rhombicDodeVAO;
  rhombicDodeVAO.vaoId = 0;		rhombicDodeVAO.vboId = 0;	rhombicDodeVAO.nVertexs = 0;

  std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
  std::vector <uint> indices;
  //vertices.resize(0);		normals.resize(0);	colors.resize(0);	textures.resize(0);		// Reinicialitzar vectors
  vertices.clear();		normals.clear();		colors.clear();		textures.clear();		// Reinicialitzar vectors
  indices.clear();		// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
  //GLdouble cColor[4];
  //glGetDoublev(GL_CURRENT_COLOR, cColor);

  //glBegin ( GL_QUADS ) ;
    for ( i = 0; i < 12; i++ )
    {   //glNormal3dv ( rdod_n[i] ) ;
        // VERTEX 0  // glVertex3dv ( rdod_r[rdod_v[i][0]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][0]][0]);	vertices.push_back(rdod_r[rdod_v[i][0]][1]);	vertices.push_back(rdod_r[rdod_v[i][0]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][0]][0] + rdod_r[rdod_v[i][0]][1]); textures.push_back(-rdod_r[rdod_v[i][0]][0] + rdod_r[rdod_v[i][0]][2]);	// Vector Textures

		// VERTEX 1  //glVertex3dv ( rdod_r[rdod_v[i][1]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][1]][0]);	vertices.push_back(rdod_r[rdod_v[i][1]][1]);	vertices.push_back(rdod_r[rdod_v[i][1]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][1]][0] + rdod_r[rdod_v[i][1]][1]); textures.push_back(-rdod_r[rdod_v[i][1]][0] + rdod_r[rdod_v[i][1]][2]);	// Vector Textures

		// VERTEX 2  //glVertex3dv ( rdod_r[rdod_v[i][2]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][2]][0]);	vertices.push_back(rdod_r[rdod_v[i][2]][1]);	vertices.push_back(rdod_r[rdod_v[i][2]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][2]][0] + rdod_r[rdod_v[i][2]][1]); textures.push_back(-rdod_r[rdod_v[i][2]][0] + rdod_r[rdod_v[i][2]][2]);	// Vector Textures

		// VERTEX 2  //glVertex3dv ( rdod_r[rdod_v[i][2]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][2]][0]);	vertices.push_back(rdod_r[rdod_v[i][2]][1]);	vertices.push_back(rdod_r[rdod_v[i][2]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][2]][0] + rdod_r[rdod_v[i][2]][1]); textures.push_back(-rdod_r[rdod_v[i][2]][0] + rdod_r[rdod_v[i][2]][2]);	// Vector Textures

		// VERTEX 3  //glVertex3dv ( rdod_r[rdod_v[i][3]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][3]][0]);	vertices.push_back(rdod_r[rdod_v[i][3]][1]);	vertices.push_back(rdod_r[rdod_v[i][3]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][3]][0] + rdod_r[rdod_v[i][3]][1]); textures.push_back(-rdod_r[rdod_v[i][3]][0] + rdod_r[rdod_v[i][3]][2]);	// Vector Textures
    
		// VERTEX 0  //glVertex3dv ( rdod_r[rdod_v[i][0]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][0]][0]);	vertices.push_back(rdod_r[rdod_v[i][0]][1]);	vertices.push_back(rdod_r[rdod_v[i][0]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][0]][0] + rdod_r[rdod_v[i][0]][1]); textures.push_back(-rdod_r[rdod_v[i][0]][0] + rdod_r[rdod_v[i][0]][2]);	// Vector Textures	
	}
  //glEnd () ;

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	rhombicDodeVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	rhombicDodeVAO.nVertexs = (int) nv/3;

	return rhombicDodeVAO;
}


// Loads a solid Rhombic Dodecahedron in EBO
CVAO loadglutSolidRhombicDodecahedron_EBO(void)
{
	GLint i;
	GLuint index = 0;	// Apuntador a vèrtexs per a indices.

// VAO
	CVAO rhombicDodeVAO;
	rhombicDodeVAO.vaoId = 0;		rhombicDodeVAO.vboId = 0;	rhombicDodeVAO.eboId = 0;	rhombicDodeVAO.nVertexs = 0;	rhombicDodeVAO.nIndices = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures
	std::vector <uint> indices;
	//vertices.resize(0);		normals.resize(0);	colors.resize(0);	 textures.resize(0);		// Reinicialitzar vectors
	vertices.clear();		normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

  // Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

	//glBegin ( GL_QUADS ) ;
	for (i = 0; i < 12; i++)
	{   //glNormal3dv ( rdod_n[i] ) ;
		// VERTEX 0  // glVertex3dv ( rdod_r[rdod_v[i][0]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][0]][0]);	vertices.push_back(rdod_r[rdod_v[i][0]][1]);	vertices.push_back(rdod_r[rdod_v[i][0]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][0]][0] + rdod_r[rdod_v[i][0]][1]); textures.push_back(-rdod_r[rdod_v[i][0]][0] + rdod_r[rdod_v[i][0]][2]);	// Vector Textures

		// VERTEX 1  //glVertex3dv ( rdod_r[rdod_v[i][1]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][1]][0]);	vertices.push_back(rdod_r[rdod_v[i][1]][1]);	vertices.push_back(rdod_r[rdod_v[i][1]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][1]][0] + rdod_r[rdod_v[i][1]][1]); textures.push_back(-rdod_r[rdod_v[i][1]][0] + rdod_r[rdod_v[i][1]][2]);	// Vector Textures

		// VERTEX 2  //glVertex3dv ( rdod_r[rdod_v[i][2]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][2]][0]);	vertices.push_back(rdod_r[rdod_v[i][2]][1]);	vertices.push_back(rdod_r[rdod_v[i][2]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][2]][0] + rdod_r[rdod_v[i][2]][1]); textures.push_back(-rdod_r[rdod_v[i][2]][0] + rdod_r[rdod_v[i][2]][2]);	// Vector Textures

		// VERTEX 3  //glVertex3dv ( rdod_r[rdod_v[i][3]] );
		colors.push_back(cColor[0]);					colors.push_back(cColor[1]);					colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(rdod_n[i][0]);				normals.push_back(rdod_n[i][1]);				normals.push_back(rdod_n[i][2]);				// Vector Normals
		vertices.push_back(rdod_r[rdod_v[i][3]][0]);	vertices.push_back(rdod_r[rdod_v[i][3]][1]);	vertices.push_back(rdod_r[rdod_v[i][3]][2]);	// Vector Vertices
		textures.push_back(-rdod_r[rdod_v[i][3]][0] + rdod_r[rdod_v[i][3]][1]); textures.push_back(-rdod_r[rdod_v[i][3]][0] + rdod_r[rdod_v[i][3]][2]);	// Vector Textures

		// Vector indices CARA i
		indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 2);		// v0-v1-v2
		indices.push_back(index + 2);	indices.push_back(index + 3);		indices.push_back(index + 0);	// v2-v3-v0
		index = index + 4;
	}
	//glEnd () ;

  // ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO, VBO i EBO carregant la geometria. Guarda identificadors VAO, VBO, EBO i numero de vertex i indices a struct CVAO.
	rhombicDodeVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	//rhombicDodeVAO.nVertexs = (int)nv / 3;

	return rhombicDodeVAO;
}
// ---------- FI RHOMBIC DODECAHEDRON --------------------------------------------


// ---------- SIERPPINSKI SPONGE -------------------------------------------------

// Draws a solid Sierpinski Sponge
void glutSolidSierpinskiSponge(int num_levels, GLdouble offset[3], GLdouble scale)
{
	Set_VAOList(GLUT_SIERPINSKISPONGE, loadglutSolidSierpinskiSponge_VAO(num_levels, offset, scale));
	draw_TriVAO_Object(GLUT_SIERPINSKISPONGE); // drawSolidSierpinskiSponge(nvert);
	deleteVAOList(GLUT_SIERPINSKISPONGE);
}


// Loads a solid Sierpinski Sponge in VAO
CVAO loadglutSolidSierpinskiSponge_VAO(int num_levels, GLdouble offset[3], GLdouble scale)
{
	GLuint apuntador = 0;

// VAO
	CVAO SierpinskySpongeVAO;		
	SierpinskySpongeVAO.vaoId = 0;		SierpinskySpongeVAO.vboId = 0;		SierpinskySpongeVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();		normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

  // Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	glutSolidSierpinskiSpongeR(num_levels, offset, scale, vertices, normals, colors, textures, indices, apuntador);

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO en struct CVAO.
	SierpinskySpongeVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//SierpinskySpongeVAO.nVertexs = int(nv / 3);

	return SierpinskySpongeVAO;
}


// Loads a solid Sierpinski Sponge in EBO
CVAO loadglutSolidSierpinskiSponge_EBO(int num_levels, GLdouble offset[3], GLdouble scale)
{
	GLuint apuntador = 0;

	// VAO
	CVAO SierpinskySpongeVAO;
	SierpinskySpongeVAO.vaoId = 0;		SierpinskySpongeVAO.vboId = 0;		SierpinskySpongeVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);		// Reinicialitzar vectors
	vertices.clear();		normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	glutSolidSierpinskiSpongeR(num_levels, offset, scale, vertices, normals, colors, textures, indices, apuntador);

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO en struct CVAO.
	SierpinskySpongeVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	//SierpinskySpongeVAO.nVertexs = int(nv / 3);

	return SierpinskySpongeVAO;
}


// Draws a solid Sierpinski Sponge in Recursive function
void glutSolidSierpinskiSpongeR(int num_levels, GLdouble offset[3], GLdouble scale, std::vector <double>& vertices, std::vector <double>& normals,
	std::vector <double>& colors, std::vector <double>& textures, std::vector <uint>& indices, GLuint &apunt)
{
  int i, j ;

  GLuint index = apunt;	// Apuntador a vèrtexs per a indices.

  if ( num_levels == 0 )
  { //glBegin ( GL_TRIANGLES ) ;
    for ( i = 0 ; i < NUM_TETR_FACES ; i++ )
    {
      //glNormal3d ( -tet_r[i][0], -tet_r[i][1], -tet_r[i][2] ) ;
      for ( j = 0; j < 3; j++ )
      {
        double x = offset[0] + scale * tet_r[tet_i[i][j]][0] ;
        double y = offset[1] + scale * tet_r[tet_i[i][j]][1] ;
        double z = offset[2] + scale * tet_r[tet_i[i][j]][2] ;
        //glVertex3d ( x, y, z ) ;
		colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(-tet_r[i][0]);	normals.push_back(-tet_r[i][1]);	normals.push_back(-tet_r[i][2]);// Vector Normals
		vertices.push_back(x);				vertices.push_back(y);				vertices.push_back(z);			// Vector Vertices
		textures.push_back(-x + y);			textures.push_back(-x + z);											// Vector Textures
		
		// Index EBO
		indices.push_back(index);
		index = index + 1;
		apunt = index;
	  }
    }
    //glEnd ();
  }
  else if ( num_levels > 0 )
  { GLdouble local_offset[3] ;  /* Use a local variable to avoid buildup of roundoff errors */
    num_levels -- ;
    scale /= 2.0 ;
    for ( i = 0 ; i < NUM_TETR_FACES ; i++ )
    { local_offset[0] = offset[0] + scale * tet_r[i][0] ;
      local_offset[1] = offset[1] + scale * tet_r[i][1] ;
      local_offset[2] = offset[2] + scale * tet_r[i][2] ;
      glutSolidSierpinskiSpongeR ( num_levels, local_offset, scale, vertices, normals, colors, textures,indices, apunt);
    }
  }
}
// ---------- FI SIERPINSKI SPONGE------------------------------------------------

// ---------- TEAPOT -------------------------------------------------------------
//CVAO fghTeapot(GLint grid, GLdouble scale, GLenum type)
CVAO fghTeapot(bool flag_EBO)
{
	int i, numV = sizeof(strip_vertices) / 4, numI = sizeof(strip_normals) / 4, vidx, nidx;

	GLuint index = 0;	// Apuntador a vèrtexs per a indices.

// VAO
	CVAO teapotVAO;
	teapotVAO.vaoId = 0;	teapotVAO.vboId = 0;	teapotVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;
	//vertices.resize(0);	normals.resize(0);		colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors
	vertices.clear();		normals.clear();		colors.clear();		textures.clear();	// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

//	for (i = 0; i < numV - 1; i++)
	for (i = 2; i < numV - 1; i++)
	{
		// Testejar darrer vertex (i)
		vidx = strip_vertices[i],
		nidx = strip_normals[i];
		if (vidx != -1)
		{	vidx = strip_vertices[i-2],
			nidx = strip_normals[i-2];
			//glNormal3fv(Normals[nidx]);
			//glVertex3fv(Vertices[vidx]);
			colors.push_back(cColor[0]);			colors.push_back(cColor[1]);			colors.push_back(cColor[2]);			colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(Normals[nidx][0]);	normals.push_back(Normals[nidx][1]);	normals.push_back(Normals[nidx][2]);	// Vector Normals
			vertices.push_back(Vertices[vidx][0]);	vertices.push_back(Vertices[vidx][1]);	vertices.push_back(Vertices[vidx][2]);	// Vector Vertices
			textures.push_back(-Vertices[vidx][0] + Vertices[vidx][1]); textures.push_back(-Vertices[vidx][0] + Vertices[vidx][2]);	// Vector Textures

			// Segon vèrtex (i-1)
			vidx = strip_vertices[i-1],
			nidx = strip_normals[i-1];
			//glNormal3fv(Normals[nidx]);
			//glVertex3fv(Vertices[vidx]);
			colors.push_back(cColor[0]);			colors.push_back(cColor[1]);			colors.push_back(cColor[2]);			colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(Normals[nidx][0]);	normals.push_back(Normals[nidx][1]);	normals.push_back(Normals[nidx][2]);	// Vector Normals
			vertices.push_back(Vertices[vidx][0]);	vertices.push_back(Vertices[vidx][1]);	vertices.push_back(Vertices[vidx][2]);	// Vector Vertices
			textures.push_back(-Vertices[vidx][0] + Vertices[vidx][1]); textures.push_back(-Vertices[vidx][0] + Vertices[vidx][2]);	// Vector Textures
		
			// Tercer Vèrtex (i)
			vidx = strip_vertices[i],
			nidx = strip_normals[i];
			//glNormal3fv(Normals[nidx]);
			//glVertex3fv(Vertices[vidx]);
			colors.push_back(cColor[0]);			colors.push_back(cColor[1]);			colors.push_back(cColor[2]);			colors.push_back(cColor[3]);// Vector Colors
			normals.push_back(Normals[nidx][0]);	normals.push_back(Normals[nidx][1]);	normals.push_back(Normals[nidx][2]);	// Vector Normals
			vertices.push_back(Vertices[vidx][0]);	vertices.push_back(Vertices[vidx][1]);	vertices.push_back(Vertices[vidx][2]);	// Vector Vertices
			textures.push_back(-Vertices[vidx][0] + Vertices[vidx][1]); textures.push_back(-Vertices[vidx][0] + Vertices[vidx][2]);	// Vector Textures
		
			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 2);	// v0,v1,v2
			index = index + 3;
		}
		else {	i = i + 2;
			//glEnd();

			//glBegin(GL_TRIANGLE_STRIP);
		}
	}
	//glEnd();

// ----------------------- VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO en struct CVAO.
	if (flag_EBO) teapotVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures,indices);
		else teapotVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//teapotVAO.nVertexs = (int) nv / 3;

	return teapotVAO;
}


// -- TEAPOT INTERFACE FUNCTIONS --------------------------------------------------
void glutSolidTeapot()
{
	Set_VAOList(GLUT_TEAPOT, loadglutSolidTeapot_VAO());
	draw_TriVAO_Object(GLUT_TEAPOT); // drawSolidTeapot();
	deleteVAOList(GLUT_TEAPOT);
}

/*
* Renders a beautiful filled teapot as VAO...
*/
CVAO loadglutSolidTeapot_VAO()
{
// We will use the general teapot rendering code
	//CVAO teapotVAO = fghTeapot(10, size, GL_FILL);
	CVAO teapotVAO = fghTeapot(false);

	return teapotVAO;
}


/*
* Renders a beautiful filled teapot as EBO...
*/
CVAO loadglutSolidTeapot_EBO()
{
// We will use the general teapot rendering code
	//CVAO teapotVAO = fghTeapot(10, size, GL_FILL);
	CVAO teapotVAO = fghTeapot(true);

	return teapotVAO;
}
// ---------- FI TEAPOT-----------------------------------------------------------

// ---------- PRIMITIVES GLU (CYLINDER, DISC, SPHERE) ----------------------------

// Make it not a power of two to avoid cache thrashing on the chip
#define CACHE_SIZE	240
#define PI	      3.14159265358979323846

// --- CILINDRE

void gluCylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks)
{	
	Set_VAOList(GLU_CYLINDER, loadgluCylinder_EBO(baseRadius, topRadius, height, slices, stacks));
	draw_TriEBO_Object(GLU_CYLINDER); //drawgluCylinder(nvertexs);
	deleteVAOList(GLU_CYLINDER);
}


// Loads a gluCylinder in VAO
CVAO loadgluCylinder_VAO(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks)
{
	GLint i, j;
	GLfloat sinCache[CACHE_SIZE];
	GLfloat cosCache[CACHE_SIZE];
	GLfloat sinCache2[CACHE_SIZE];
	GLfloat cosCache2[CACHE_SIZE];
	GLfloat angle;
	GLfloat zLow, zHigh;
	GLfloat length;
	GLdouble deltaRadius;
	GLfloat zNormal;
	GLfloat xyNormalRatio;
	GLdouble radiusLow, radiusHigh;
	int needCache2, needCache3;

// VAO
	CVAO cilindreVAO;
	cilindreVAO.vaoId = 0;		cilindreVAO.vboId = 0;		cilindreVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;

	if (slices < 2 || stacks < 1 || baseRadius < 0.0 || topRadius < 0.0 ||
		height < 0.0)	return cilindreVAO;

// Compute length (needed for normal calculations)
	deltaRadius = baseRadius - topRadius;
	length = sqrt(deltaRadius*deltaRadius + height*height);
	if (length == 0.0) return cilindreVAO;

// Cache2 is the various normals at the vertices themselves
	needCache2 = needCache3 = 0;
	needCache2 = 1;
	zNormal = deltaRadius / length;
	xyNormalRatio = height / length;

	for (i = 0; i < slices; i++) {
		angle = 2 * PI * i / slices;
		sinCache2[i] = xyNormalRatio * sin(angle);
		cosCache2[i] = xyNormalRatio * cos(angle);
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	sinCache[slices] = sinCache[0];
	cosCache[slices] = cosCache[0];
	sinCache2[slices] = sinCache2[0];
	cosCache2[slices] = cosCache2[0];
	for (j = 0; j < stacks; j++) {
		zLow = j * height / stacks;
		zHigh = (j + 1) * height / stacks;
		radiusLow = baseRadius - deltaRadius * ((float)j / stacks);
		radiusHigh = baseRadius - deltaRadius * ((float)(j + 1) / stacks);

//		glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices-1; i++) {
			// Vèrtex 0 (i)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)j / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)j / stacks);						// Vector Textures
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], zLow);
			vertices.push_back(radiusLow * sinCache[i]);	vertices.push_back(radiusLow * cosCache[i]);	vertices.push_back(zLow); // Vector Vertexs

			// Vèrtex 1 (i)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)(j + 1) / stacks);				// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], zHigh);
			vertices.push_back(radiusHigh * sinCache[i]);	vertices.push_back(radiusHigh * cosCache[i]);	vertices.push_back(zHigh); // Vector Vertexs

			// Vèrtex 3 (i+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i + 1]);	normals.push_back(cosCache2[i + 1]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)(i + 1) / slices);	textures.push_back((float)(j + 1) / stacks);				// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], zHigh);
			vertices.push_back(radiusHigh * sinCache[i+1]);	vertices.push_back(radiusHigh * cosCache[i+1]);	vertices.push_back(zHigh); // Vector Vertexs

			// Vèrtex 3 (i+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i + 1]);	normals.push_back(cosCache2[i + 1]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)(i + 1) / slices);	textures.push_back((float)(j + 1) / stacks);				// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], zHigh);
			vertices.push_back(radiusHigh * sinCache[i+1]);	vertices.push_back(radiusHigh * cosCache[i+1]);	vertices.push_back(zHigh); // Vector Vertexs

			// Vèrtex 2 (i+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i+1]);	normals.push_back(cosCache2[i+1]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)j / stacks);
			textures.push_back(1 - (float)(i+1) / slices);	textures.push_back((float)j / stacks);						// Vector Textures
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], zLow);
			vertices.push_back(radiusLow * sinCache[i+1]);	vertices.push_back(radiusLow * cosCache[i+1]);	vertices.push_back(zLow); // Vector Vertexs

			// Vèrtex 0 (i)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)j / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)j / stacks);						// Vector Textures
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], zLow);
			vertices.push_back(radiusLow * sinCache[i]);	vertices.push_back(radiusLow * cosCache[i]);	vertices.push_back(zLow); // Vector Vertexs
		}
	   //glEnd();
	}

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
	cilindreVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);

	return cilindreVAO;
}


// Loads a gluCylinder in EBO
CVAO loadgluCylinder_EBO(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks)
{
	GLint i, j;
	GLfloat sinCache[CACHE_SIZE];
	GLfloat cosCache[CACHE_SIZE];
	GLfloat sinCache2[CACHE_SIZE];
	GLfloat cosCache2[CACHE_SIZE];
	GLfloat angle;
	GLfloat zLow, zHigh;
	GLfloat length;
	GLdouble deltaRadius;
	GLfloat zNormal;
	GLfloat xyNormalRatio;
	GLdouble radiusLow, radiusHigh;
	int needCache2, needCache3;

	GLint index = 0;	// Apuntador a vèrtexs per a indices.

// VAO
	CVAO cilindreVAO;
	cilindreVAO.vaoId = 0;		cilindreVAO.vboId = 0;	cilindreVAO.eboId = 0;	cilindreVAO.nVertexs = 0;	cilindreVAO.nIndices = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;

	if (slices < 2 || stacks < 1 || baseRadius < 0.0 || topRadius < 0.0 ||
		height < 0.0)	return cilindreVAO;

	// Compute length (needed for normal calculations)
	deltaRadius = baseRadius - topRadius;
	length = sqrt(deltaRadius * deltaRadius + height * height);
	if (length == 0.0) return cilindreVAO;

	// Cache2 is the various normals at the vertices themselves
	needCache2 = needCache3 = 0;
	needCache2 = 1;
	zNormal = deltaRadius / length;
	xyNormalRatio = height / length;

	for (i = 0; i < slices; i++) {
		angle = 2 * PI * i / slices;
		sinCache2[i] = xyNormalRatio * sin(angle);
		cosCache2[i] = xyNormalRatio * cos(angle);
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	sinCache[slices] = sinCache[0];
	cosCache[slices] = cosCache[0];
	sinCache2[slices] = sinCache2[0];
	cosCache2[slices] = cosCache2[0];
	for (j = 0; j < stacks; j++) {
		zLow = j * height / stacks;
		zHigh = (j + 1) * height / stacks;
		radiusLow = baseRadius - deltaRadius * ((float)j / stacks);
		radiusHigh = baseRadius - deltaRadius * ((float)(j + 1) / stacks);

		//glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices - 1; i++) {
			// Vèrtex 0 (i,j)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)j / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)j / stacks);						// Vector Textures
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], zLow);
			vertices.push_back(radiusLow * sinCache[i]);	vertices.push_back(radiusLow * cosCache[i]);	vertices.push_back(zLow); // Vector Vertexs

			// Vèrtex 1 (i,j+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)(j + 1) / stacks);				// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], zHigh);
			vertices.push_back(radiusHigh * sinCache[i]);	vertices.push_back(radiusHigh * cosCache[i]);	vertices.push_back(zHigh); // Vector Vertexs

			// Vèrtex 2 (i+1,j)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i + 1]);	normals.push_back(cosCache2[i + 1]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)j / stacks);
			textures.push_back(1 - (float)(i + 1) / slices);	textures.push_back((float)j / stacks);						// Vector Textures
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], zLow);
			vertices.push_back(radiusLow * sinCache[i + 1]);	vertices.push_back(radiusLow * cosCache[i + 1]);	vertices.push_back(zLow); // Vector Vertexs

			// Vèrtex 3 (i+1,j+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i + 1]);	normals.push_back(cosCache2[i + 1]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)(i + 1) / slices);	textures.push_back((float)(j + 1) / stacks);				// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], zHigh);
			vertices.push_back(radiusHigh * sinCache[i + 1]);	vertices.push_back(radiusHigh * cosCache[i + 1]);	vertices.push_back(zHigh); // Vector Vertexs

			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 3);	// v0-v1-v3
			indices.push_back(index + 3);	indices.push_back(index + 2);		indices.push_back(index);		// v3-v2-v0
			index = index + 4;
		}
		//glEnd();
	}

	// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO, VBO i EBO carregant de la geometria. Guardar identificadors VAO, VBO, EBO, numero vèrtexs i indices a struct CVAO.
	cilindreVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures,indices);

	return cilindreVAO;
}


// --- DISC
void gluDisk(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint loops)
{
	// Set_VAOList(GLU_DISK, loadgluDisk_VAO(innerRadius, outerRadius, slices, loops));
	Set_VAOList(GLU_DISK, loadgluDisk_EBO(innerRadius, outerRadius, slices, loops)); 
	draw_TriEBO_Object(GLU_DISK); //draw_TriVAO_Object(GLU_DISK); //drawgluDisk(nvertexs);
	deleteVAOList(GLU_DISK);
}

// Loads a gluDisk in VAO
CVAO loadgluDisk_VAO(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint loops)
{
	CVAO diskVAO = loadgluPartialDisk_VAO(innerRadius, outerRadius, slices, loops, 0.0, 360.0);
	return diskVAO;
}

// Loads a gluDisk in EBO
CVAO loadgluDisk_EBO(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint loops)
{
	CVAO diskVAO = loadgluPartialDisk_EBO(innerRadius, outerRadius, slices, loops, 0.0, 360.0);
	return diskVAO;
}


// --- PARTIAL DISC

void gluPartialDisk(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint loops, GLdouble startAngle, GLdouble sweepAngle)
{
	//Set_VAOList(GLU_PARTIALDISK, loadgluPartialDisk_VAO(innerRadius, outerRadius, slices, loops, startAngle, sweepAngle));
	Set_VAOList(GLU_PARTIALDISK, loadgluPartialDisk_EBO(innerRadius, outerRadius, slices, loops, startAngle, sweepAngle));
	draw_TriEBO_Object(GLU_PARTIALDISK); //draw_TriVAO_Object(GLU_PARTIALDISK); // drawgluPartialDisk(nvertexs);
	deleteVAOList(GLU_PARTIALDISK);
}

// Loads a gluPartialDisk in VAO
CVAO loadgluPartialDisk_VAO(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint loops, GLdouble startAngle, GLdouble sweepAngle)
{
	GLint i, j;
	GLdouble sinCache[CACHE_SIZE];
	GLdouble cosCache[CACHE_SIZE];
	GLfloat angle;
	GLfloat deltaRadius;
	GLfloat radiusLow, radiusHigh;
	GLdouble texLow = 0.0, texHigh = 0.0;
	GLfloat angleOffset;
	GLint slices2;
	GLint finish;

// VAO
	CVAO diskVAO;
	diskVAO.vaoId = 0;	diskVAO.vboId = 0;	diskVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;
	if (slices < 2 || loops < 1 || outerRadius <= 0.0 || innerRadius < 0.0 || innerRadius > outerRadius) return diskVAO;
	if (sweepAngle < -360.0) sweepAngle = 360.0;
	if (sweepAngle > 360.0) sweepAngle = 360.0;
	if (sweepAngle < 0) {	startAngle += sweepAngle;
							sweepAngle = -sweepAngle;
						}

	if (sweepAngle == 360.0) slices2 = slices;
		else slices2 = slices + 1;

// Compute length (needed for normal calculations)
	deltaRadius = outerRadius - innerRadius;

// Cache is the vertex locations cache
	angleOffset = startAngle / 180.0 * PI;
	for (i = 0; i <= slices; i++) {	angle = angleOffset + ((PI * sweepAngle) / 180.0) * i / slices;
									sinCache[i] = sin(angle);
									cosCache[i] = cos(angle);
									}

	if (sweepAngle == 360.0) {	sinCache[slices] = sinCache[0];
								cosCache[slices] = cosCache[0];
							}

	//glNormal3f(0.0, 0.0, 1.0);

	if (innerRadius == 0.0) 
		{ finish = loops - 1;

// Triangle strip for inner polygons
		//glBegin(GL_TRIANGLE_FAN);
		  //glTexCoord2f(0.5, 0.5);
		  //glVertex3f(0.0, 0.0, 0.0);
		  //colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		  //normals.push_back(0.0);		normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
		  //textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
		  //vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertexs

		  radiusLow = outerRadius - deltaRadius * ((float)(loops - 1) / loops);
		  texLow = radiusLow / outerRadius / 2;
		  for (i = slices-1; i >= 0; i--) {
			  // Vèrtex (0)
			  //glTexCoord2f(0.5, 0.5);
			  //glVertex3f(0.0, 0.0, 0.0);
			  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			  normals.push_back(0.0);		normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
			  vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertexs
			  
			  // Vèrtex (i+1)
			  //glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			  //glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			  normals.push_back(0.0);		normals.push_back(0.0);			normals.push_back(1.0);							// Vector Normals
			  textures.push_back(texLow * sinCache[i+1] + 0.5);				textures.push_back(texLow * cosCache[i+1] + 0.5);	// Vector Textures
			  vertices.push_back(radiusLow * sinCache[i+1]);				vertices.push_back(radiusLow * cosCache[i+1]);		vertices.push_back(0.0);		// Vector Vertexs

			  // Vèrtex (i)
			  //glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			  //glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			  colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			  normals.push_back(0.0);		normals.push_back(0.0);			normals.push_back(1.0);							// Vector Normals
			  textures.push_back(texLow * sinCache[i] + 0.5);				textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			  vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);		// Vector Vertexs

			  }
		//glEnd();

// ----------------------- VAO
		  std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
		}
	else finish = loops;

	for (j = 0; j < finish; j++) {
		radiusLow = outerRadius - deltaRadius * ((float)j / loops);
		radiusHigh = outerRadius - deltaRadius * ((float)(j + 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		texHigh = radiusHigh / outerRadius / 2;
		//vertices.resize(0);		normals.resize(0);	colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
		//glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices-1; i++) {
			
			// Vèrtex 0
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);	// Vector Vèrtexs
			
			// Vèrtex 1
			//glTexCoord2f(texHigh * sinCache[i] + 0.5, texHigh * cosCache[i] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);				colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i] + 0.5);				textures.push_back(texHigh * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i]);					vertices.push_back(radiusHigh * cosCache[i]);		vertices.push_back(0.0);// Vector Vèrtexs

			// Vèrtex 3
			//glTexCoord2f(texHigh * sinCache[i+1] + 0.5, texHigh * cosCache[i+1] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);				colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i + 1] + 0.5);				textures.push_back(texHigh * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i + 1]);					vertices.push_back(radiusHigh * cosCache[i + 1]);		vertices.push_back(0.0);// Vector Vèrtexs

			// Vèrtex 3
			//glTexCoord2f(texHigh * sinCache[i+1] + 0.5, texHigh * cosCache[i+1] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);				colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i + 1] + 0.5);				textures.push_back(texHigh * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i + 1]);					vertices.push_back(radiusHigh * cosCache[i + 1]);		vertices.push_back(0.0);// Vector Vèrtexs

			// Vèrtex 2
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i+1] + 0.5);				textures.push_back(texLow * cosCache[i+1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i+1]);					vertices.push_back(radiusLow * cosCache[i+1]);		vertices.push_back(0.0);	// Vector Vèrtexs

			// Vèrtex 0
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);	// Vector Vèrtexs
		}
		//glEnd();
	}

// ------------------------ VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO a vector VAOId i identificador VBO a vector VBOId.
	diskVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//diskVAO.nVertexs = (int)nv / 3;

	return diskVAO;
}

// Loads a gluPartialDisk in EBO
CVAO loadgluPartialDisk_EBO(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint loops, GLdouble startAngle, GLdouble sweepAngle)
{
	GLint i, j;
	GLdouble sinCache[CACHE_SIZE];
	GLdouble cosCache[CACHE_SIZE];
	GLfloat angle;
	GLfloat deltaRadius;
	GLfloat radiusLow, radiusHigh;
	GLdouble texLow = 0.0, texHigh = 0.0;
	GLfloat angleOffset;
	GLint slices2;
	GLint finish;

	GLint index = 0;	// Apuntador a vèrtexs per a indices EBO.

// VAO
	CVAO diskVAO;
	diskVAO.vaoId = 0;	diskVAO.vboId = 0;	diskVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;									// Definició vectors dinàmic d'índexs per a EBO 
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;
	if (slices < 2 || loops < 1 || outerRadius <= 0.0 || innerRadius < 0.0 || innerRadius > outerRadius) return diskVAO;
	if (sweepAngle < -360.0) sweepAngle = 360.0;
	if (sweepAngle > 360.0) sweepAngle = 360.0;
	if (sweepAngle < 0) {
		startAngle += sweepAngle;
		sweepAngle = -sweepAngle;
	}

	if (sweepAngle == 360.0) slices2 = slices;
	else slices2 = slices + 1;

	// Compute length (needed for normal calculations)
	deltaRadius = outerRadius - innerRadius;

	// Cache is the vertex locations cache
	angleOffset = startAngle / 180.0 * PI;
	for (i = 0; i <= slices; i++) {
		angle = angleOffset + ((PI * sweepAngle) / 180.0) * i / slices;
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	if (sweepAngle == 360.0) {
		sinCache[slices] = sinCache[0];
		cosCache[slices] = cosCache[0];
	}

	//glNormal3f(0.0, 0.0, 1.0);

	if (innerRadius == 0.0)
	{
		finish = loops - 1;

	// Triangle strip for inner polygons
		//glBegin(GL_TRIANGLE_FAN);
		//glTexCoord2f(0.5, 0.5);
		//glVertex3f(0.0, 0.0, 0.0);
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(0.0);		normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
		textures.push_back(0.5);	textures.push_back(0.5);										// Vector Textures
		vertices.push_back(0.0);	vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertexs

		index = index + 1;

		radiusLow = outerRadius - deltaRadius * ((float)(loops - 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		for (i = slices - 1; i >= 0; i--) {
			// Vèrtex (0)
			//glTexCoord2f(0.5, 0.5);
			//glVertex3f(0.0, 0.0, 0.0);
			//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			//textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
			//vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertexs

			// Vèrtex (i+1)
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);		normals.push_back(0.0);			normals.push_back(1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i + 1] + 0.5);				textures.push_back(texLow * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i + 1]);				vertices.push_back(radiusLow * cosCache[i + 1]);		vertices.push_back(0.0);		// Vector Vertexs

			// Vèrtex (i)
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);		normals.push_back(0.0);			normals.push_back(1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);				textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);		// Vector Vertexs

			// Vector indices CARA i GL_TRIANGLE_FAN
			indices.push_back(0);		indices.push_back(index);		indices.push_back(index +1);		// v0-vi-vi+1
			index = index + 2;

		}
		//glEnd();

// ----------------------- VAO
		std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	}
	else finish = loops;

	for (j = 0; j < finish; j++) {
		radiusLow = outerRadius - deltaRadius * ((float)j / loops);
		radiusHigh = outerRadius - deltaRadius * ((float)(j + 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		texHigh = radiusHigh / outerRadius / 2;
		//vertices.resize(0);		normals.resize(0);	colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
		//glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices - 1; i++) {

			// Vèrtex 0
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);	// Vector Vèrtexs

			// Vèrtex 1
			//glTexCoord2f(texHigh * sinCache[i] + 0.5, texHigh * cosCache[i] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);				colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i] + 0.5);				textures.push_back(texHigh * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i]);					vertices.push_back(radiusHigh * cosCache[i]);		vertices.push_back(0.0);// Vector Vèrtexs

			// Vèrtex 2
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i + 1] + 0.5);				textures.push_back(texLow * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i + 1]);					vertices.push_back(radiusLow * cosCache[i + 1]);		vertices.push_back(0.0);	// Vector Vèrtexs

			// Vèrtex 3
			//glTexCoord2f(texHigh * sinCache[i+1] + 0.5, texHigh * cosCache[i+1] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);				colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i + 1] + 0.5);				textures.push_back(texHigh * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i + 1]);					vertices.push_back(radiusHigh * cosCache[i + 1]);		vertices.push_back(0.0);// Vector Vèrtexs
		
			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 3);	// v0-v1-v3
			indices.push_back(index + 3);	indices.push_back(index + 2);		indices.push_back(index);		// v3-v2-v0
			index = index + 4;
		}
		//glEnd();
	}

// ------------------------ VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO a vector VAOId i identificador VBO a vector VBOId.
	diskVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);
	//diskVAO.nVertexs = (int)nv / 3;

	return diskVAO;
}


// --- ESFERA : Texturització compatible amb superficie planetes

void gluSphere(GLdouble radius, GLint slices, GLint stacks)
{	
	Set_VAOList(GLU_SPHERE, loadgluSphere_EBO(radius, slices, stacks));
	draw_TriEBO_Object(GLU_SPHERE); // drawgluSphere(nvertexs);
	deleteVAOList(GLU_SPHERE);
}


// Loads a gluSphere in VAO
CVAO loadgluSphere_VAO(GLdouble radius, GLint slices, GLint stacks)
{	
	GLint i, j;
	GLfloat sinCache1a[CACHE_SIZE];
	GLfloat cosCache1a[CACHE_SIZE];
	GLfloat sinCache2a[CACHE_SIZE];
	GLfloat cosCache2a[CACHE_SIZE];
	GLfloat sinCache1b[CACHE_SIZE];
	GLfloat cosCache1b[CACHE_SIZE];
	GLfloat sinCache2b[CACHE_SIZE];
	GLfloat cosCache2b[CACHE_SIZE];
	GLfloat angle;
	GLfloat zLow, zHigh;
	GLfloat sintemp1 = 0.0, sintemp2 = 0.0, sintemp3 = 0.0, sintemp4 = 0.0;
	GLfloat costemp1 = 0.0, costemp2 = 0.0, costemp3 = 0.0, costemp4 = 0.0;
	GLboolean needCache2;
	GLint start, finish;

// VAO
	CVAO auxVAO;
	auxVAO.vaoId = 0;		auxVAO.vboId = 0;	auxVAO.nVertexs = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;
	if (stacks >= CACHE_SIZE) stacks = CACHE_SIZE - 1;
	if (slices < 2 || stacks < 1 || radius < 0.0) return auxVAO;

// Cache2 is the various normals at the vertices themselves 
	needCache2 = GL_TRUE;

	for (i = 0; i < slices; i++) {
		angle = 2 * PI * i / slices;
		sinCache1a[i] = sin(angle);
		cosCache1a[i] = cos(angle);
		sinCache2a[i] = sinCache1a[i];
		cosCache2a[i] = cosCache1a[i];
	}

	for (j = 0; j <= stacks; j++) {
		angle = PI * j / stacks;
		sinCache2b[j] = sin(angle);
		cosCache2b[j] = cos(angle);
		sinCache1b[j] = radius * sin(angle);
		cosCache1b[j] = radius * cos(angle);
	}

// Make sure it comes to a point 
	sinCache1b[0] = 0.0f;
	sinCache1b[stacks] = 0.0;

	sinCache1a[slices] = sinCache1a[0];
	cosCache1a[slices] = cosCache1a[0];

	sinCache2a[slices] = sinCache2a[0];
	cosCache2a[slices] = cosCache2a[0];

//	** When texturing we need to respecify the
//	** texture coordinates of the apex for every adjacent vertex (because
//	** it isn't a constant for that point)
	start = 0;
	finish = stacks;

	for (j = start; j < finish; j++) {
		zLow = cosCache1b[j];
		zHigh = cosCache1b[j + 1];
		sintemp1 = sinCache1b[j];
		sintemp2 = sinCache1b[j + 1];
		sintemp3 = sinCache2b[j + 1];
		costemp3 = cosCache2b[j + 1];
		sintemp4 = sinCache2b[j];
		costemp4 = cosCache2b[j];

		//glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices-1; i++) {

			// Vèrtex 0
			//glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
			//glTexCoord2f(1 - (float)i / slices,	1 - (float)(j + 1) / stacks);
			//glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);	// Vector Colors
			normals.push_back(sinCache2a[i] * sintemp3);	normals.push_back(cosCache2a[i] * sintemp3);		normals.push_back(costemp3);	// Vector Normals
			textures.push_back(1 - (float)i / slices);		textures.push_back(1 - (float)(j + 1) / stacks);	// Vector Textures
			vertices.push_back(sintemp2 * sinCache1a[i]);	vertices.push_back(sintemp2 * cosCache1a[i]);		vertices.push_back(zHigh);		// Vector Vèrtexs

			// Vèrtex 1
			//glNormal3f(sinCache2a[i] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
			//glTexCoord2f(1 - (float)i / slices,	1 - (float)j / stacks);
			//glVertex3f(sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], zLow);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2a[i] * sintemp4);		normals.push_back(cosCache2a[i] * sintemp4);	normals.push_back(costemp4);// Vector Normals
			textures.push_back(1 - (float)i / slices);			textures.push_back(1 - (float)j / stacks);		// Vector Textures
			vertices.push_back(sintemp1 * sinCache1a[i]);		vertices.push_back(sintemp1 * cosCache1a[i]);	vertices.push_back(zLow);// Vector Vèrtexs

			// Vèrtex 3
			//glNormal3f(sinCache2a[i+1] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
			//glTexCoord2f(1 - (float)i+1 / slices,	1 - (float)j / stacks);
			//glVertex3f(sintemp1 * sinCache1a[i+1], sintemp1 * cosCache1a[i+1], zLow);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2a[i + 1] * sintemp4);		normals.push_back(cosCache2a[i + 1] * sintemp4);	normals.push_back(costemp4);// Vector Normals
			textures.push_back(1 - (float)(i + 1) / slices);			textures.push_back(1 - (float)j / stacks);		// Vector Textures
			vertices.push_back(sintemp1 * sinCache1a[i + 1]);		vertices.push_back(sintemp1 * cosCache1a[i + 1]);	vertices.push_back(zLow);// Vector Vèrtexs

			// Vèrtex 3
			//glNormal3f(sinCache2a[i+1] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
			//glTexCoord2f(1 - (float)i+1 / slices,	1 - (float)j / stacks);
			//glVertex3f(sintemp1 * sinCache1a[i+1], sintemp1 * cosCache1a[i+1], zLow);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2a[i+1] * sintemp4);		normals.push_back(cosCache2a[i+1] * sintemp4);	normals.push_back(costemp4);// Vector Normals
			textures.push_back(1 - (float)(i+1) / slices);			textures.push_back(1 - (float)j / stacks);		// Vector Textures
			vertices.push_back(sintemp1 * sinCache1a[i+1]);		vertices.push_back(sintemp1 * cosCache1a[i+1]);	vertices.push_back(zLow);// Vector Vèrtexs

			// Vèrtex 2
			//glNormal3f(sinCache2a[i+1] * sintemp3, cosCache2a[i+1] * sintemp3, costemp3);
			//glTexCoord2f(1 - (float)i+1 / slices,	1 - (float)(j + 1) / stacks);
			//glVertex3f(sintemp2 * sinCache1a[i+1], sintemp2 * cosCache1a[i+1], zHigh);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);	// Vector Colors
			normals.push_back(sinCache2a[i+1] * sintemp3);	normals.push_back(cosCache2a[i+1] * sintemp3);		normals.push_back(costemp3);	// Vector Normals
			textures.push_back(1 - (float)(i+1) / slices);		textures.push_back(1 - (float)(j + 1) / stacks);	// Vector Textures
			vertices.push_back(sintemp2 * sinCache1a[i+1]);	vertices.push_back(sintemp2 * cosCache1a[i+1]);		vertices.push_back(zHigh);		// Vector Vèrtexs

			// Vèrtex 0
			//glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
			//glTexCoord2f(1 - (float)i / slices,	1 - (float)(j + 1) / stacks);
			//glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);	// Vector Colors
			normals.push_back(sinCache2a[i] * sintemp3);	normals.push_back(cosCache2a[i] * sintemp3);		normals.push_back(costemp3);	// Vector Normals
			textures.push_back(1 - (float)i / slices);		textures.push_back(1 - (float)(j + 1) / stacks);	// Vector Textures
			vertices.push_back(sintemp2 * sinCache1a[i]);	vertices.push_back(sintemp2 * cosCache1a[i]);		vertices.push_back(zHigh);		// Vector Vèrtexs

		}
		//glEnd();
	}

// ----------------------- VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO en struct CVAO.
	auxVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//auxVAO.nVertexs = (int)nv / 3;

	return auxVAO;
}

// Loads a gluSphere in EBO
CVAO loadgluSphere_EBO(GLdouble radius, GLint slices, GLint stacks)
{
	GLint i, j;
	GLfloat sinCache1a[CACHE_SIZE];
	GLfloat cosCache1a[CACHE_SIZE];
	GLfloat sinCache2a[CACHE_SIZE];
	GLfloat cosCache2a[CACHE_SIZE];
	GLfloat sinCache1b[CACHE_SIZE];
	GLfloat cosCache1b[CACHE_SIZE];
	GLfloat sinCache2b[CACHE_SIZE];
	GLfloat cosCache2b[CACHE_SIZE];
	GLfloat angle;
	GLfloat zLow, zHigh;
	GLfloat sintemp1 = 0.0, sintemp2 = 0.0, sintemp3 = 0.0, sintemp4 = 0.0;
	GLfloat costemp1 = 0.0, costemp2 = 0.0, costemp3 = 0.0, costemp4 = 0.0;
	GLboolean needCache2;
	GLint start, finish;

	GLint index = 0; // Index a vèrtexs dins indices de l'EBO.

// VAO
	CVAO auxVAO;
	auxVAO.vaoId = 0;		auxVAO.vboId = 0;	auxVAO.eboId = 0;	auxVAO.nVertexs = 0;	auxVAO.nIndices = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures
	std::vector <uint> indices;
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);		// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();		// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;
	if (stacks >= CACHE_SIZE) stacks = CACHE_SIZE - 1;
	if (slices < 2 || stacks < 1 || radius < 0.0) return auxVAO;

// Cache2 is the various normals at the vertices themselves 
	needCache2 = GL_TRUE;

	for (i = 0; i < slices; i++) {
		angle = 2 * PI * i / slices;
		sinCache1a[i] = sin(angle);
		cosCache1a[i] = cos(angle);
		sinCache2a[i] = sinCache1a[i];
		cosCache2a[i] = cosCache1a[i];
	}

	for (j = 0; j <= stacks; j++) {
		angle = PI * j / stacks;
		sinCache2b[j] = sin(angle);
		cosCache2b[j] = cos(angle);
		sinCache1b[j] = radius * sin(angle);
		cosCache1b[j] = radius * cos(angle);
	}

// Make sure it comes to a point 
	sinCache1b[0] = 0.0f;
	sinCache1b[stacks] = 0.0;

	sinCache1a[slices] = sinCache1a[0];
	cosCache1a[slices] = cosCache1a[0];

	sinCache2a[slices] = sinCache2a[0];
	cosCache2a[slices] = cosCache2a[0];

//	** When texturing we need to respecify the
//	** texture coordinates of the apex for every adjacent vertex (because
//	** it isn't a constant for that point)
	start = 0;
	finish = stacks;

	for (j = start; j < finish; j++) {
		zLow = cosCache1b[j];
		zHigh = cosCache1b[j + 1];
		sintemp1 = sinCache1b[j];
		sintemp2 = sinCache1b[j + 1];
		sintemp3 = sinCache2b[j + 1];
		costemp3 = cosCache2b[j + 1];
		sintemp4 = sinCache2b[j];
		costemp4 = cosCache2b[j];

		//glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices - 1; i++) {

			// Vèrtex 0
			//glNormal3f(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
			//glTexCoord2f(1 - (float)i / slices,	1 - (float)(j + 1) / stacks);
			//glVertex3f(sintemp2 * sinCache1a[i], sintemp2 * cosCache1a[i], zHigh);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);	// Vector Colors
			normals.push_back(sinCache2a[i] * sintemp3);	normals.push_back(cosCache2a[i] * sintemp3);		normals.push_back(costemp3);	// Vector Normals
			textures.push_back(1 - (float)i / slices);		textures.push_back(1 - (float)(j + 1) / stacks);	// Vector Textures
			vertices.push_back(sintemp2 * sinCache1a[i]);	vertices.push_back(sintemp2 * cosCache1a[i]);		vertices.push_back(zHigh);		// Vector Vèrtexs

			// Vèrtex 1
			//glNormal3f(sinCache2a[i] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
			//glTexCoord2f(1 - (float)i / slices,	1 - (float)j / stacks);
			//glVertex3f(sintemp1 * sinCache1a[i], sintemp1 * cosCache1a[i], zLow);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2a[i] * sintemp4);		normals.push_back(cosCache2a[i] * sintemp4);	normals.push_back(costemp4);// Vector Normals
			textures.push_back(1 - (float)i / slices);			textures.push_back(1 - (float)j / stacks);		// Vector Textures
			vertices.push_back(sintemp1 * sinCache1a[i]);		vertices.push_back(sintemp1 * cosCache1a[i]);	vertices.push_back(zLow);// Vector Vèrtexs

			// Vèrtex 2
			//glNormal3f(sinCache2a[i+1] * sintemp3, cosCache2a[i+1] * sintemp3, costemp3);
			//glTexCoord2f(1 - (float)i+1 / slices,	1 - (float)(j + 1) / stacks);
			//glVertex3f(sintemp2 * sinCache1a[i+1], sintemp2 * cosCache1a[i+1], zHigh);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]);	// Vector Colors
			normals.push_back(sinCache2a[i + 1] * sintemp3);	normals.push_back(cosCache2a[i + 1] * sintemp3);		normals.push_back(costemp3);	// Vector Normals
			textures.push_back(1 - (float)(i + 1) / slices);		textures.push_back(1 - (float)(j + 1) / stacks);	// Vector Textures
			vertices.push_back(sintemp2 * sinCache1a[i + 1]);	vertices.push_back(sintemp2 * cosCache1a[i + 1]);		vertices.push_back(zHigh);		// Vector Vèrtexs

			// Vèrtex 3
			//glNormal3f(sinCache2a[i+1] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
			//glTexCoord2f(1 - (float)i+1 / slices,	1 - (float)j / stacks);
			//glVertex3f(sintemp1 * sinCache1a[i+1], sintemp1 * cosCache1a[i+1], zLow);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2a[i + 1] * sintemp4);		normals.push_back(cosCache2a[i + 1] * sintemp4);	normals.push_back(costemp4);// Vector Normals
			textures.push_back(1 - (float)(i + 1) / slices);			textures.push_back(1 - (float)j / stacks);		// Vector Textures
			vertices.push_back(sintemp1 * sinCache1a[i + 1]);		vertices.push_back(sintemp1 * cosCache1a[i + 1]);	vertices.push_back(zLow);// Vector Vèrtexs

			// Vector indices CARA 1
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 3);	// v0-v1-v3
			indices.push_back(index + 3);	indices.push_back(index + 2);		indices.push_back(index );		// v3-v2-v0
			index = index + 4;
		}
		//glEnd();
	}

// ----------------------- VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO, VBO i EBO carregant de la geometria. Guardar identificadors VAO, VBO, EBO, numero vèrtexs i indices en struct CVAO.
	auxVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures,indices);
	//auxVAO.nVertexs = (int)nv / 3;

	return auxVAO;
}
// ---------- FI PRIMITIVES GLU (CYLINDER, DISC, SPHERE) -------------------------


// ------------------- PRIMITIVA CILINDRE ----------------------------------------

// Dibuix d'un cilindre tancat amb tapes (inferior i superior)
void cilindre(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks)
{
	Set_VAOList(GLU_CILINDRE_SENCER, loadCilindre_VAO(baseRadius, topRadius, height, slices, stacks));
	draw_TriVAO_Object(GLU_CILINDRE_SENCER); //drawcilindre(nvertexs);
	deleteVAOList(GLU_CILINDRE_SENCER);
}

// Carrega un cilindre tancat amb tapes (inferior i superior) en VAO
CVAO loadCilindre_VAO(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks)
{
	GLint i, j;
	GLfloat sinCache[CACHE_SIZE];
	GLfloat cosCache[CACHE_SIZE];
	GLfloat sinCache2[CACHE_SIZE];
	GLfloat cosCache2[CACHE_SIZE];
	GLfloat angle;
	GLfloat zLow, zHigh;
	GLfloat length;
	GLdouble deltaRadius;
	GLfloat zNormal;
	GLfloat xyNormalRatio;
	GLdouble radiusLow, radiusHigh;
	int needCache2, needCache3;

// Variables Disk
	//GLint i, j;
	//GLdouble sinCache[CACHE_SIZE];
	//GLdouble cosCache[CACHE_SIZE];
	//GLfloat angle;
	//GLfloat deltaRadius;
	//GLfloat radiusLow, radiusHigh;
	GLdouble texLow = 0.0, texHigh = 0.0;
	GLfloat angleOffset;
	GLint slices2;
	GLint finish;
	GLdouble startAngle = 0.0;
	GLdouble sweepAngle = 360.0;
	GLdouble innerRadius = 0.0;
	GLdouble outerRadius = baseRadius;
	GLint loops = stacks;

// VAO
	CVAO auxVAO;
	auxVAO.vaoId = -0;		auxVAO.vboId = 0;	auxVAO.nVertexs = 0;
	
	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;

	if (slices < 2 || stacks < 1 || baseRadius < 0.0 || topRadius < 0.0 ||
		height < 0.0)	return auxVAO;

	// Compute length (needed for normal calculations)
	deltaRadius = baseRadius - topRadius;
	length = sqrt(deltaRadius * deltaRadius + height * height);
	if (length == 0.0) return auxVAO;

	// Cache2 is the various normals at the vertices themselves
	needCache2 = needCache3 = 0;
	needCache2 = 1;
	zNormal = deltaRadius / length;
	xyNormalRatio = height / length;

	for (i = 0; i < slices; i++) {
		angle = 2 * PI * i / slices;
		sinCache2[i] = xyNormalRatio * sin(angle);
		cosCache2[i] = xyNormalRatio * cos(angle);
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	sinCache[slices] = sinCache[0];
	cosCache[slices] = cosCache[0];
	sinCache2[slices] = sinCache2[0];
	cosCache2[slices] = cosCache2[0];


// DIBUIX LATERALS
	for (j = 0; j < stacks; j++) {
		zLow = j * height / stacks;
		zHigh = (j + 1) * height / stacks;
		radiusLow = baseRadius - deltaRadius * ((float)j / stacks);
		radiusHigh = baseRadius - deltaRadius * ((float)(j + 1) / stacks);

		//		glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices - 1; i++) {
			// Vèrtex 0 (i)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)j / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)j / stacks);						// Vector Textures
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], zLow);
			vertices.push_back(radiusLow * sinCache[i]);	vertices.push_back(radiusLow * cosCache[i]);	vertices.push_back(zLow); // Vector Vertexs

			// Vèrtex 1 (i)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)(j + 1) / stacks);				// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], zHigh);
			vertices.push_back(radiusHigh * sinCache[i]);	vertices.push_back(radiusHigh * cosCache[i]);	vertices.push_back(zHigh); // Vector Vertexs

			// Vèrtex 3 (i+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i + 1]);	normals.push_back(cosCache2[i + 1]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)(i + 1) / slices);	textures.push_back((float)(j + 1) / stacks);				// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], zHigh);
			vertices.push_back(radiusHigh * sinCache[i + 1]);	vertices.push_back(radiusHigh * cosCache[i + 1]);	vertices.push_back(zHigh); // Vector Vertexs

			// Vèrtex 3 (i+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i + 1]);	normals.push_back(cosCache2[i + 1]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)(i + 1) / slices);	textures.push_back((float)(j + 1) / stacks);				// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], zHigh);
			vertices.push_back(radiusHigh * sinCache[i + 1]);	vertices.push_back(radiusHigh * cosCache[i + 1]);	vertices.push_back(zHigh); // Vector Vertexs

			// Vèrtex 2 (i+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i + 1]);	normals.push_back(cosCache2[i + 1]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)j / stacks);
			textures.push_back(1 - (float)(i + 1) / slices);	textures.push_back((float)j / stacks);						// Vector Textures
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], zLow);
			vertices.push_back(radiusLow * sinCache[i + 1]);	vertices.push_back(radiusLow * cosCache[i + 1]);	vertices.push_back(zLow); // Vector Vertexs

			// Vèrtex 0 (i)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)j / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)j / stacks);						// Vector Textures
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], zLow);
			vertices.push_back(radiusLow * sinCache[i]);	vertices.push_back(radiusLow * cosCache[i]);	vertices.push_back(zLow); // Vector Vertexs
		}
		//glEnd();
	}


	// Inicialització variables disk
	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;
	if (slices < 2 || loops < 1 || outerRadius <= 0.0 || innerRadius < 0.0 || innerRadius > outerRadius) return auxVAO;
	if (sweepAngle < -360.0) sweepAngle = 360.0;
	if (sweepAngle > 360.0) sweepAngle = 360.0;
	if (sweepAngle < 0) {
		startAngle += sweepAngle;
		sweepAngle = -sweepAngle;
	}

	if (sweepAngle == 360.0) slices2 = slices;
	else slices2 = slices + 1;

// DISC INFERIOR (ROTAR 180 Graus el disc base pel vector normal cap a fora de l'objecte (x'=-x, y'=-y,z'=z)
	const GLdouble rotaPi = -1.0;
	//	outerRadius = baseRadius;		// Radi exterior disc correspon a radi base del cilindre.
	// Compute length (needed for normal calculations)
	deltaRadius = outerRadius - innerRadius;

	// Cache is the vertex locations cache
	angleOffset = startAngle / 180.0 * PI;
	for (i = 0; i <= slices; i++) {
		angle = angleOffset + ((PI * sweepAngle) / 180.0) * i / slices;
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	if (sweepAngle == 360.0) {
		sinCache[slices] = sinCache[0];
		cosCache[slices] = cosCache[0];
	}

	if (innerRadius == 0.0)
	{
		finish = loops - 1;
		// Triangle strip for inner polygons
		//glBegin(GL_TRIANGLE_FAN);
		  //glTexCoord2f(0.5, 0.5);
		  //glVertex3f(0.0, 0.0, 0.0);
		  //colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		  //normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
		  //textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
		  //vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertexs

		radiusLow = outerRadius - deltaRadius * ((float)(loops - 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		for (i = slices - 1; i >= 0; i--) {
			// Vèrtex (0)
			//glTexCoord2f(0.5, 0.5);
			//glVertex3f(0.0, 0.0, 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);			// Vector Normals
			textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertexs

			// Vèrtex (i+1)
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);		normals.push_back(0.0);				normals.push_back(-1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i + 1] + 0.5);				textures.push_back(texLow * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i + 1]);				vertices.push_back(radiusLow * cosCache[i + 1]);		vertices.push_back(0.0);		// Vector Vertexs

			// Vèrtex (i)
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);		normals.push_back(0.0);				normals.push_back(-1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);		// Vector Vertexs

		}
		//glEnd();

// ----------------------- VAO
		std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	}
	else finish = loops;

	for (j = 0; j < finish; j++) {
		radiusLow = outerRadius - deltaRadius * ((float)j / loops);
		radiusHigh = outerRadius - deltaRadius * ((float)(j + 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		texHigh = radiusHigh / outerRadius / 2;
		//vertices.resize(0);		normals.resize(0);	colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
		//glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices - 1; i++) {

			// Vèrtex 0
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);	// Vector Vèrtexs

			// Vèrtex 1
			//glTexCoord2f(texHigh * sinCache[i] + 0.5, texHigh * cosCache[i] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);				colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i] + 0.5);				textures.push_back(texHigh * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i]);					vertices.push_back(radiusHigh * cosCache[i]);		vertices.push_back(0.0);// Vector Vèrtexs

			// Vèrtex 3
			//glTexCoord2f(texHigh * sinCache[i+1] + 0.5, texHigh * cosCache[i+1] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);				colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i + 1] + 0.5);			textures.push_back(texHigh * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i + 1]);				vertices.push_back(radiusHigh * cosCache[i + 1]);	vertices.push_back(0.0);// Vector Vèrtexs

			// Vèrtex 3
			//glTexCoord2f(texHigh * sinCache[i+1] + 0.5, texHigh * cosCache[i+1] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);				colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i + 1] + 0.5);			textures.push_back(texHigh * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i + 1]);				vertices.push_back(radiusHigh * cosCache[i + 1]);	vertices.push_back(0.0);// Vector Vèrtexs

			// Vèrtex 2
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i + 1] + 0.5);				textures.push_back(texLow * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i + 1]);				vertices.push_back(radiusLow * cosCache[i + 1]);	vertices.push_back(0.0);	// Vector Vèrtexs

			// Vèrtex 0
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);	// Vector Vèrtexs
		}
		//glEnd();
	}
	// ---- FI DISC INFERIOR

	// DISC SUPERIOR
	outerRadius = topRadius;		// Radi exterior disc correspon a radi superior del cilindre.
// Compute length (needed for normal calculations)
	deltaRadius = outerRadius - innerRadius;

	// Cache is the vertex locations cache
	angleOffset = startAngle / 180.0 * PI;
	for (i = 0; i <= slices; i++) {
		angle = angleOffset + ((PI * sweepAngle) / 180.0) * i / slices;
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	if (sweepAngle == 360.0) {
		sinCache[slices] = sinCache[0];
		cosCache[slices] = cosCache[0];
	}

	if (innerRadius == 0.0)
	{
		finish = loops - 1;
		// Triangle strip for inner polygons
		//glBegin(GL_TRIANGLE_FAN);
		  //glTexCoord2f(0.5, 0.5);
		  //glVertex3f(0.0, 0.0, 0.0);
		  //colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		  //normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
		  //textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
		  //vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(height);		// Vector Vertexs

		radiusLow = outerRadius - deltaRadius * ((float)(loops - 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		for (i = slices - 1; i >= 0; i--) {
			// Vèrtex (0)
			//glTexCoord2f(0.5, 0.5);
			//glVertex3f(0.0, 0.0, 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
			vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(height);		// Vector Vertexs

			// Vèrtex (i+1)
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);		normals.push_back(0.0);				normals.push_back(1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i + 1] + 0.5);				textures.push_back(texLow * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i + 1]);				vertices.push_back(radiusLow * cosCache[i + 1]);	vertices.push_back(height);		// Vector Vertexs

			// Vèrtex (i)
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);	vertices.push_back(height);		// Vector Vertexs

		}
		//glEnd();

// ----------------------- VAO
		std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	}
	else finish = loops;

	for (j = 0; j < finish; j++) {
		radiusLow = outerRadius - deltaRadius * ((float)j / loops);
		radiusHigh = outerRadius - deltaRadius * ((float)(j + 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		texHigh = radiusHigh / outerRadius / 2;
		//vertices.resize(0);		normals.resize(0);	colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
		//glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices - 1; i++) {

			// Vèrtex 0
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(height);	// Vector Vèrtexs

			// Vèrtex 1
			//glTexCoord2f(texHigh * sinCache[i] + 0.5, texHigh * cosCache[i] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i] + 0.5);				textures.push_back(texHigh * cosCache[i] + 0.5);// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i]);					vertices.push_back(radiusHigh * cosCache[i]);	vertices.push_back(height);// Vector Vèrtexs

			// Vèrtex 3
			//glTexCoord2f(texHigh * sinCache[i+1] + 0.5, texHigh * cosCache[i+1] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i + 1] + 0.5);			textures.push_back(texHigh * cosCache[i + 1] + 0.5);// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i + 1]);				vertices.push_back(radiusHigh * cosCache[i + 1]);	vertices.push_back(height);// Vector Vèrtexs

			// Vèrtex 3
			//glTexCoord2f(texHigh * sinCache[i+1] + 0.5, texHigh * cosCache[i+1] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);				colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i + 1] + 0.5);				textures.push_back(texHigh * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i + 1]);					vertices.push_back(radiusHigh * cosCache[i + 1]);		vertices.push_back(height);// Vector Vèrtexs

			// Vèrtex 2
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i + 1] + 0.5);				textures.push_back(texLow * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i + 1]);					vertices.push_back(radiusLow * cosCache[i + 1]);vertices.push_back(height);	// Vector Vèrtexs

			// Vèrtex 0
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);				// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);	vertices.push_back(height);	// Vector Vèrtexs
		}
		//glEnd();
	}

// ----------------------- VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
	auxVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//auxVAO.nVertexs = (int)nv / 3;

	return auxVAO;
}

// Carrega un cilindre tancat amb tapes (inferior i superior) en EBO
CVAO loadCilindre_EBO(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks)
{
	GLint i, j;
	GLfloat sinCache[CACHE_SIZE];
	GLfloat cosCache[CACHE_SIZE];
	GLfloat sinCache2[CACHE_SIZE];
	GLfloat cosCache2[CACHE_SIZE];
	GLfloat angle;
	GLfloat zLow, zHigh;
	GLfloat length;
	GLdouble deltaRadius;
	GLfloat zNormal;
	GLfloat xyNormalRatio;
	GLdouble radiusLow, radiusHigh;
	int needCache2, needCache3;

	// Variables Disk
		//GLint i, j;
		//GLdouble sinCache[CACHE_SIZE];
		//GLdouble cosCache[CACHE_SIZE];
		//GLfloat angle;
		//GLfloat deltaRadius;
		//GLfloat radiusLow, radiusHigh;
	GLdouble texLow = 0.0, texHigh = 0.0;
	GLfloat angleOffset;
	GLint slices2;
	GLint finish;
	GLdouble startAngle = 0.0;
	GLdouble sweepAngle = 360.0;
	GLdouble innerRadius = 0.0;
	GLdouble outerRadius = baseRadius;
	GLint loops = stacks;

	GLint index = 0;		// Apuntador a vèrtexs per a indices.
	GLint index_ini = 0;

// VAO
	CVAO auxVAO;
	auxVAO.vaoId = -0;		auxVAO.vboId = 0;		auxVAO.eboId = 0;	auxVAO.nVertexs = 0;	auxVAO.nIndices = 0;

	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;
	//vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors
	vertices.clear();			normals.clear();		colors.clear();		textures.clear();			// Reinicialitzar vectors
	indices.clear();

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;

	if (slices < 2 || stacks < 1 || baseRadius < 0.0 || topRadius < 0.0 ||
		height < 0.0)	return auxVAO;

	// Compute length (needed for normal calculations)
	deltaRadius = baseRadius - topRadius;
	length = sqrt(deltaRadius * deltaRadius + height * height);
	if (length == 0.0) return auxVAO;

	// Cache2 is the various normals at the vertices themselves
	needCache2 = needCache3 = 0;
	needCache2 = 1;
	zNormal = deltaRadius / length;
	xyNormalRatio = height / length;

	for (i = 0; i < slices; i++) {
		angle = 2 * PI * i / slices;
		sinCache2[i] = xyNormalRatio * sin(angle);
		cosCache2[i] = xyNormalRatio * cos(angle);
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	sinCache[slices] = sinCache[0];
	cosCache[slices] = cosCache[0];
	sinCache2[slices] = sinCache2[0];
	cosCache2[slices] = cosCache2[0];

// DIBUIX LATERALS
	for (j = 0; j < stacks; j++) {
		zLow = j * height / stacks;
		zHigh = (j + 1) * height / stacks;
		radiusLow = baseRadius - deltaRadius * ((float)j / stacks);
		radiusHigh = baseRadius - deltaRadius * ((float)(j + 1) / stacks);

		//		glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices - 1; i++) {
			// Vèrtex 0 (i,j)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)j / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)j / stacks);						// Vector Textures
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], zLow);
			vertices.push_back(radiusLow * sinCache[i]);	vertices.push_back(radiusLow * cosCache[i]);			vertices.push_back(zLow); // Vector Vertexs

			// Vèrtex 1 (i,j+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i]);	normals.push_back(cosCache2[i]);	normals.push_back(zNormal);		// Vector Normals
			//glTexCoord2f(1 - (float)i / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)i / slices);	textures.push_back((float)(j + 1) / stacks);				// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], zHigh);
			vertices.push_back(radiusHigh * sinCache[i]);	vertices.push_back(radiusHigh * cosCache[i]);			vertices.push_back(zHigh); // Vector Vertexs

			// Vèrtex 2 (i+1,j)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			//glNormal3f(sinCache2[i], cosCache2[i], zNormal);
			normals.push_back(sinCache2[i + 1]);	normals.push_back(cosCache2[i + 1]);	normals.push_back(zNormal);	// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)j / stacks);
			textures.push_back(1 - (float)(i + 1) / slices);	textures.push_back((float)j / stacks);					// Vector Textures
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], zLow);
			vertices.push_back(radiusLow * sinCache[i + 1]);	vertices.push_back(radiusLow * cosCache[i + 1]);	vertices.push_back(zLow); // Vector Vertexs

			// Vèrtex 3 (i+1,j+1)
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);		colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(sinCache2[i + 1]);	normals.push_back(cosCache2[i + 1]);	normals.push_back(zNormal);	// Vector Normals
			//glTexCoord2f(1 - (float)(i+1) / slices, (float)(j + 1) / stacks);
			textures.push_back(1 - (float)(i + 1) / slices);	textures.push_back((float)(j + 1) / stacks);			// Vector Textures
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], zHigh);
			vertices.push_back(radiusHigh * sinCache[i + 1]);	vertices.push_back(radiusHigh * cosCache[i + 1]);		vertices.push_back(zHigh); // Vector Vertexs

			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 3);	// v0-v1-v3
			indices.push_back(index + 3);	indices.push_back(index + 2);		indices.push_back(index);		// v3-v2-v0
			index = index + 4;

		}
		//glEnd();
	}

// Inicialització variables disk
	if (slices >= CACHE_SIZE) slices = CACHE_SIZE - 1;
	if (slices < 2 || loops < 1 || outerRadius <= 0.0 || innerRadius < 0.0 || innerRadius > outerRadius) return auxVAO;
	if (sweepAngle < -360.0) sweepAngle = 360.0;
	if (sweepAngle > 360.0) sweepAngle = 360.0;
	if (sweepAngle < 0) {
		startAngle += sweepAngle;
		sweepAngle = -sweepAngle;
	}

	if (sweepAngle == 360.0) slices2 = slices;
	else slices2 = slices + 1;

// DISC INFERIOR (ROTAR 180 Graus el disc base pel vector normal cap a fora de l'objecte (x'=-x, y'=-y,z'=z)
	const GLdouble rotaPi = -1.0;
	//	outerRadius = baseRadius;		// Radi exterior disc correspon a radi base del cilindre.
	// Compute length (needed for normal calculations)
	deltaRadius = outerRadius - innerRadius;

	// Cache is the vertex locations cache
	angleOffset = startAngle / 180.0 * PI;
	for (i = 0; i <= slices; i++) {
		angle = angleOffset + ((PI * sweepAngle) / 180.0) * i / slices;
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	if (sweepAngle == 360.0) {
		sinCache[slices] = sinCache[0];
		cosCache[slices] = cosCache[0];
	}

	if (innerRadius == 0.0)
	{
		finish = loops - 1;
		// Triangle strip for inner polygons
		//glBegin(GL_TRIANGLE_FAN);
		  //glTexCoord2f(0.5, 0.5);
		  //glVertex3f(0.0, 0.0, 0.0);
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);		// Vector Normals
		textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertexs

		index_ini = index;
		index = index + 1;

		radiusLow = outerRadius - deltaRadius * ((float)(loops - 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		for (i = slices - 1; i >= 0; i--) {
			// Vèrtex (0)
			//glTexCoord2f(0.5, 0.5);
			//glVertex3f(0.0, 0.0, 0.0);
			//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);	// Vector Normals
			//textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
			//vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(0.0);		// Vector Vertexs

			// Vèrtex (i+1)
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i + 1] + 0.5);				textures.push_back(texLow * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i + 1]);				vertices.push_back(radiusLow * cosCache[i + 1]);		vertices.push_back(0.0);		// Vector Vertexs

			// Vèrtex (i)
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);						// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);		// Vector Vertexs

			// Vector indices CARA i GL_TRIANGLE_FAN
			indices.push_back(index_ini);		indices.push_back(index);		indices.push_back(index + 1);		// v0-vi-vi+1
			index = index + 2;

		}
		//glEnd();

// ----------------------- VAO
		std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	}
	else finish = loops;

	for (j = 0; j < finish; j++) {
		radiusLow = outerRadius - deltaRadius * ((float)j / loops);
		radiusHigh = outerRadius - deltaRadius * ((float)(j + 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		texHigh = radiusHigh / outerRadius / 2;
		//vertices.resize(0);		normals.resize(0);	colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
		//glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices - 1; i++) {

			// Vèrtex 0
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);						// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);		vertices.push_back(0.0);	// Vector Vèrtexs

			// Vèrtex 1
			//glTexCoord2f(texHigh * sinCache[i] + 0.5, texHigh * cosCache[i] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);						// Vector Normals
			textures.push_back(texHigh * sinCache[i] + 0.5);				textures.push_back(texHigh * cosCache[i] + 0.5);// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i]);					vertices.push_back(radiusHigh * cosCache[i]);	vertices.push_back(0.0);// Vector Vèrtexs

			// Vèrtex 2
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i + 1] + 0.5);				textures.push_back(texLow * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i + 1]);				vertices.push_back(radiusLow * cosCache[i + 1]);	vertices.push_back(0.0);	// Vector Vèrtexs


			// Vèrtex 3
			//glTexCoord2f(texHigh * sinCache[i+1] + 0.5, texHigh * cosCache[i+1] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(-1.0);							// Vector Normals
			textures.push_back(texHigh * sinCache[i + 1] + 0.5);			textures.push_back(texHigh * cosCache[i + 1] + 0.5);// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i + 1]);				vertices.push_back(radiusHigh * cosCache[i + 1]);	vertices.push_back(0.0);// Vector Vèrtexs

			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 3);	// v0-v1-v3
			indices.push_back(index + 3);	indices.push_back(index + 2);		indices.push_back(index);		// v3-v2-v0
			index = index + 4;
		
		}
		//glEnd();
	}
// ---- FI DISC INFERIOR


// DISC SUPERIOR
	outerRadius = topRadius;		// Radi exterior disc correspon a radi superior del cilindre.
// Compute length (needed for normal calculations)
	deltaRadius = outerRadius - innerRadius;

// Cache is the vertex locations cache
	angleOffset = startAngle / 180.0 * PI;
	for (i = 0; i <= slices; i++) {
		angle = angleOffset + ((PI * sweepAngle) / 180.0) * i / slices;
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	if (sweepAngle == 360.0) {
		sinCache[slices] = sinCache[0];
		cosCache[slices] = cosCache[0];
	}

	if (innerRadius == 0.0)
	{
		finish = loops - 1;
		// Triangle strip for inner polygons
		//glBegin(GL_TRIANGLE_FAN);
		  //glTexCoord2f(0.5, 0.5);
		  //glVertex3f(0.0, 0.0, 0.0);
		colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
		textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
		vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(height);		// Vector Vertexs

		index_ini = index;
		index = index + 1;

		radiusLow = outerRadius - deltaRadius * ((float)(loops - 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		for (i = slices - 1; i >= 0; i--) {
			// Vèrtex (0)
			//glTexCoord2f(0.5, 0.5);
			//glVertex3f(0.0, 0.0, 0.0);
			//colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//normals.push_back(0.0);		normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			//textures.push_back(0.5);		textures.push_back(0.5);										// Vector Textures
			//vertices.push_back(0.0);		vertices.push_back(0.0);		vertices.push_back(height);		// Vector Vertexs

			// Vèrtex (i+1)
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);		normals.push_back(0.0);				normals.push_back(1.0);								// Vector Normals
			textures.push_back(texLow * sinCache[i + 1] + 0.5);				textures.push_back(texLow * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i + 1]);				vertices.push_back(radiusLow * cosCache[i + 1]);	vertices.push_back(height);		// Vector Vertexs

			// Vèrtex (i)
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);		normals.push_back(0.0);				normals.push_back(1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);	vertices.push_back(height);		// Vector Vertexs

			// Vector indices CARA i GL_TRIANGLE_FAN
			indices.push_back(index_ini);		indices.push_back(index);		indices.push_back(index + 1);		// v0-vi-vi+1
			index = index + 2;

		}
		//glEnd();

// ----------------------- VAO
		std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	}
	else finish = loops;


	for (j = 0; j < finish; j++) {
		radiusLow = outerRadius - deltaRadius * ((float)j / loops);
		radiusHigh = outerRadius - deltaRadius * ((float)(j + 1) / loops);
		texLow = radiusLow / outerRadius / 2;
		texHigh = radiusHigh / outerRadius / 2;
		//vertices.resize(0);		normals.resize(0);	colors.resize(0);	 textures.resize(0);			// Reinicialitzar vectors
		//glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices - 1; i++) {

			// Vèrtex 0
			//glTexCoord2f(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
			//glVertex3f(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);							// Vector Normals
			textures.push_back(texLow * sinCache[i] + 0.5);					textures.push_back(texLow * cosCache[i] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow * sinCache[i]);					vertices.push_back(radiusLow * cosCache[i]);	vertices.push_back(height);	// Vector Vèrtexs

			// Vèrtex 1
			//glTexCoord2f(texHigh * sinCache[i] + 0.5, texHigh * cosCache[i] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);					colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);							// Vector Normals
			textures.push_back(texHigh * sinCache[i] + 0.5);				textures.push_back(texHigh * cosCache[i] + 0.5);// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i]);					vertices.push_back(radiusHigh * cosCache[i]);	vertices.push_back(height);// Vector Vèrtexs

			// Vèrtex 2
			//glTexCoord2f(texLow * sinCache[i+1] + 0.5, texLow * cosCache[i+1] + 0.5);
			//glVertex3f(radiusLow * sinCache[i+1], radiusLow * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);						colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);								// Vector Normals
			textures.push_back(texLow* sinCache[i + 1] + 0.5);				textures.push_back(texLow* cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusLow* sinCache[i + 1]);					vertices.push_back(radiusLow* cosCache[i + 1]);		vertices.push_back(height);	// Vector Vèrtexs

			// Vèrtex 3
			//glTexCoord2f(texHigh * sinCache[i+1] + 0.5, texHigh * cosCache[i+1] + 0.5);
			//glVertex3f(radiusHigh * sinCache[i+1], radiusHigh * cosCache[i+1], 0.0);
			colors.push_back(cColor[0]);	colors.push_back(cColor[1]);	colors.push_back(cColor[2]);							colors.push_back(cColor[3]); // Vector Colors
			normals.push_back(0.0);			normals.push_back(0.0);			normals.push_back(1.0);									// Vector Normals
			textures.push_back(texHigh * sinCache[i + 1] + 0.5);				textures.push_back(texHigh * cosCache[i + 1] + 0.5);	// Vector Textures
			vertices.push_back(radiusHigh * sinCache[i + 1]);					vertices.push_back(radiusHigh * cosCache[i + 1]);	vertices.push_back(height);// Vector Vèrtexs

			// Vector indices CARA i
			indices.push_back(index);		indices.push_back(index + 1);		indices.push_back(index + 3);	// v0-v1-v3
			indices.push_back(index + 3);	indices.push_back(index + 2);		indices.push_back(index);		// v3-v2-v0
			index = index + 4;

		}
		//glEnd();
	}
// ---- FI DISC SUPERIOR


// ----------------------- VAO
	//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
	auxVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures,indices);
	//auxVAO.nVertexs = (int)nv / 3;

	return auxVAO;
}


//----------- FI PRIMITIVA CILINDRE ----------------------------------------------

// ---------- PRIMITIVA SQUARE (QUADRAT) -----------------------------------------
void glutSolidSquare(GLdouble dSize)
{
	Set_VAOList(GLUT_SQUARE, loadglutSolidSquare_VAO(dSize)); 
	draw_TriEBO_Object(GLUT_SQUARE); //draw_TriVAO_Object(GLUT_SQUARE);
	deleteVAOList(GLUT_SQUARE);
}


CVAO loadglutSolidSquare_VAO(GLdouble dSize)
{
	GLdouble size = dSize * 0.5f;

// VAO
	CVAO auxVAO;
	auxVAO.vaoId = -0;		auxVAO.vboId = 0;	auxVAO.nVertexs = 0;

// square ///////////////////////////////////////////////////////////////////////
//
//        |  Z
//        |   
//    v2--|----v1
//   /    |    /
//  v3------v0

// Obtenir color actual definit en OpenGL amb glColor();
//	GLdouble cColor[4];
//	glGetDoublev(GL_CURRENT_COLOR, cColor);

// Vertex coords array for glDrawArrays() =====================================
// A square has 1 sides that consists of 4 vertices. And, each
// vertex has 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 72 floats (4 * 3 = 12). The size of each double is 8 bytes (12 * 8 = 96 bytes)
	//GLdouble vertices[] = { 
	std::vector<double> vertices = {
		 size,  size,   0.0, -size,  size,   0.0, -size, -size,   0.0,      // v0-v1-v2 (front)
		-size, -size,   0.0,  size, -size,   0.0,  size,  size,   0.0 };    // v2-v3-v0

// Normal array
	//GLdouble normals[] = {
	std::vector<double> normals = {
		0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,   // v0-v1-v2 (front)
		0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0 }; // v2-v3-v0

// Color array
	//GLdouble colors[] = {
	std::vector<double> colors = {
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3],		// v0-v1-v2
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3]  };	// v2-v3-v0 

// Texture Array
	//GLdouble textures[] = {
	std::vector<double> textures = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0,      // v0-v1-v2 (front)
		1.0, 1.0, 0.0, 1.0, 0.0, 0.0 };    // v2-v3-v0

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO i identificador VBO a struct CVAO.
	auxVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);

    return auxVAO;
}


CVAO loadglutSolidSquare_EBO(GLdouble dSize)
{
	GLdouble size = dSize * 0.5f;

// VAO
	CVAO auxVAO;
	auxVAO.vaoId = 0;	auxVAO.vboId = 0;	auxVAO.eboId = 0; auxVAO.nVertexs = 0;	auxVAO.nIndices = 0;

// square ///////////////////////////////////////////////////////////////////////
//
//        |  Z
//        |   
//    v2--|----v1
//   /    |    /
//  v3------v0

// Obtenir color actual definit en OpenGL amb glColor();
	//GLdouble cColor[4];
	//glGetDoublev(GL_CURRENT_COLOR, cColor);

// Vertex coords array for glDrawArrays() =====================================
// A square has 1 sides that consists of 4 vertices. And, each
// vertex has 3 components (x,y,z) of floats, therefore, the size of vertex
// array is 72 floats (4 * 3 = 12). The size of each double is 8 bytes (12 * 8 = 96 bytes)
	//GLdouble vertices[] = { 
	std::vector<double> vertices = {																//				Y-Axis - Z-Axis
		 size,  size,   0.0, -size,  size,   0.0, -size, -size,   0.0,  size, -size,   0.0  };		// v0-v1-v2-v3 (front) - (top)

// Normal array
	//GLdouble normals[] = {
	std::vector<double> normals = {													//				Y-Axis - Z-Axis
		0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0,  0.0,  0.0,  1.0 };	// v0-v1-v2-v3 (front) - (top)

// Color array
	//GLdouble colors[] = {
	std::vector<double> colors = {
		cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3], cColor[0], cColor[1], cColor[2], cColor[3] }; // v0-v1-v2-v3 (front)

// Texture Array Z-Axis
//GLdouble textures[] = {
	std::vector<double> textures = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0 };     // v0-v1-v2-v3 (top)

// Indices Array
	//GLuint indices[] = {
	std::vector<uint> indices = {
		0, 1, 2, 2, 3, 0 };			// v0-v1-v2-v3 (front)


// Creació d'un VAO, VBO i EBO carregant-hi la geometria. Guardar identificadors VAO, VBO, EBO, vertex i indexs a struct CVAO.
	auxVAO = load_TRIANGLES_EBO(vertices, normals, colors, textures, indices);

	return auxVAO;
}
// ---------- FI PRIMITIVA SQUARE (QUADRAT) --------------------------------------

//----------- DIBUIX DE CORBES i SUPERFICIES -------------------------------------

//----- CÀLCUL VECTOR NORMAL (PRODUCTE VECTORAL)

/*
// VNormal: Càlcul del producte vectorial normalitzat de dos vectors v1, v2.
//         Resultat a la variable n.
CPunt3D VNormal(CPunt3D v1, CPunt3D v2)
{
	float longitud;
	CPunt3D vn;

	vn.x = v1.y * v2.z - v2.y * v1.z;	//	n[0] = v1[1] * v2[2] - v2[1] * v1[2];
	vn.y = v1.z * v2.x - v2.z * v1.x;	//	n[1] = v1[2] * v2[0] - v2[2] * v1[0];
	vn.z = v1.x * v2.y - v1.y * v2.x;	//	n[2] = v1[0] * v2[1] - v1[1] * v2[0];

// Convertim el vector en vector unitat
	longitud = sqrt(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
	vn.x = vn.x / longitud;
	vn.y = vn.y / longitud;
	vn.z = vn.z / longitud;

	return vn;
}
*/

// Producte_Vectorial: Càlcul del producte vectorial normalitzat de dos vectors v1,v2.
//			Resultat a la variable n.
CPunt3D Prod_Vectorial(CPunt3D v1, CPunt3D v2)
{
	double longitud;
	CPunt3D vn;

	vn.x = v1.y * v2.z - v2.y * v1.z;	//	n[0] = v1[1] * v2[2] - v2[1] * v1[2];
	vn.y = v1.z * v2.x - v2.z * v1.x;	//	n[1] = v1[2] * v2[0] - v2[2] * v1[0];
	vn.z = v1.x * v2.y - v1.y * v2.x;	//	n[2] = v1[0] * v2[1] - v1[1] * v2[0];

// Convertim el vector en vector unitat (normalització)
	longitud = sqrt(vn.x * vn.x + vn.y * vn.y + vn.z * vn.z);
	vn.x = vn.x / longitud;
	vn.y = vn.y / longitud;
	vn.z = vn.z / longitud;

	return vn;
}

//------ TRIEDRE DE FRENET

// VNormalPrincipal: Calcul del Vector Normal Principal a partir dels vector Tangent (VT) i el vector BiNormal (VBN) com a 
//            producte vectorial dels dos. Normalitzat.
CPunt3D Vector_Normal_Principal(CPunt3D VBN, CPunt3D VT)
{
	CPunt3D vnp = { 0.0f, 0.0f, 0.0f, 1.0f };
	double longitut = 0;

	vnp = Prod_Vectorial(VBN, VT);

	// Convertim el vector en vector unitat (normalització)
	longitut = sqrt(vnp.x * vnp.x + vnp.y * vnp.y + vnp.z * vnp.z);
	vnp.x = vnp.x / longitut;
	vnp.y = vnp.y / longitut;
	vnp.z = vnp.z / longitut;

	// Rectificació cap a eix Z positiu
	//if (vnp.z < 0.0) vnp.z = -vnp.z;
	return vnp;
}


// dibuixa_Triedre_Frenet: Dibuix del triedre de Frenet segons: VBN B(T)-(vermell), VNP N(t)-(verd) i VT T(t)-(blau)
void dibuixa_Triedre_Frenet(GLuint sh_programID, CPunt3D vertex, CPunt3D VT, CPunt3D VNP, CPunt3D VBN)
{
	const int incr = 10;
	CColor color_objecte;
	bool sw_mat[5] = { false, true, true, false, true};

// Doblar el gruix de la linia dels eixos.
	glLineWidth(2.0);

// VAO
	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs i colors 
	//vertices.resize(0);	colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();		colors.clear();		// Reinicialitzar vectors

// Eix VBN (vermell)
//	glColor3f(1.0f, 0.0f, 0.0f);
	color_objecte.r = 1.0;	color_objecte.g = 0.0; color_objecte.b = 0.0;	color_objecte.a = 1.0;
	SeleccionaColorMaterial(sh_programID, color_objecte, sw_mat);
//	glBegin(GL_LINES);
//	glVertex3f(vertex.x, vertex.y, vertex.z);
	colors.push_back(1.0);			colors.push_back(0.0);			colors.push_back(0.0);			colors.push_back(1.0);
	vertices.push_back(vertex.x);	vertices.push_back(vertex.y);	vertices.push_back(vertex.z);

//	glVertex3f(vertex.x + incr * VBN.x, vertex.y + incr * VBN.y, vertex.z + incr * VBN.z);
	colors.push_back(1.0);			colors.push_back(0.0);			colors.push_back(0.0);			colors.push_back(1.0);
	vertices.push_back(vertex.x + incr * VBN.x);	vertices.push_back(vertex.y + incr * VBN.y);	vertices.push_back(vertex.z + incr * VBN.z);
//	glEnd();

	draw_GL_LINES_VAO(vertices, colors);
	vertices.clear();			colors.clear();		// Reinicialitzar vectors

// Eix VNP (verd)
	//	glColor3f(0.0f, 1.0f, 0.0f);
	color_objecte.r = 0.0;	color_objecte.g = 1.0; color_objecte.b = 0.0;	color_objecte.a = 1.0;
	SeleccionaColorMaterial(sh_programID, color_objecte, sw_mat);
	//	glBegin(GL_LINES);
	//	glVertex3f(vertex.x, vertex.y, vertex.z);
	colors.push_back(0.0);			colors.push_back(1.0);			colors.push_back(0.0);			colors.push_back(1.0);
	vertices.push_back(vertex.x);	vertices.push_back(vertex.y);	vertices.push_back(vertex.z);

	//	glVertex3f(vertex.x + incr * VNP.x, vertex.y + incr * VNP.y, vertex.z + incr * VNP.z);
	colors.push_back(0.0);			colors.push_back(1.0);			colors.push_back(0.0);			colors.push_back(1.0);
	vertices.push_back(vertex.x + incr * VNP.x);	vertices.push_back(vertex.y + incr * VNP.y);	vertices.push_back(vertex.z + incr * VNP.z);
	//	glEnd();

	draw_GL_LINES_VAO(vertices, colors);
	vertices.clear();			colors.clear();		// Reinicialitzar vectors

// Eix VT (blau) 
	//	glColor3f(0.0, 1.0, 1.0);
	color_objecte.r = 0.0;	color_objecte.g = 1.0; color_objecte.b = 1.0;	color_objecte.a = 1.0;
	SeleccionaColorMaterial(sh_programID, color_objecte, sw_mat);
	//	glBegin(GL_LINES);
	//	glVertex3f(vertex.x, vertex.y, vertex.z);
	colors.push_back(0.0);			colors.push_back(1.0);			colors.push_back(1.0);		colors.push_back(1.0);
	vertices.push_back(vertex.x);	vertices.push_back(vertex.y);	vertices.push_back(vertex.z);

	//	glVertex3f(vertex.x + incr * VT.x, vertex.y + incr * VT.y, vertex.z + incr * VT.z);
	colors.push_back(0.0);			colors.push_back(1.0);			colors.push_back(1.0);		colors.push_back(1.0);
	vertices.push_back(vertex.x + incr * VT.x);	vertices.push_back(vertex.y + incr * VT.y);	vertices.push_back(vertex.z + incr * VT.z);
	//	glEnd();

	draw_GL_LINES_VAO(vertices, colors);

// Restaurar el gruix de la linia dels eixos
	glLineWidth(1.0);
}


//----------  POLYLINE (Interpolació linial entre punts de control) --------------
void draw_PolyLine(CPunt3D* ctr_points, int nptsPolyLine, float pas)
{
	Set_VAOList(CRV_POLYLINE, load_PolyLine_VAO(ctr_points, nptsPolyLine, pas));
	draw_LinVAO_Object(CRV_POLYLINE); // drawCRVPolyLine(npunts);
	deleteVAOList(CRV_POLYLINE);
}


CVAO load_PolyLine_VAO(CPunt3D* ctr_points, int nptsPolyLine, float pas)
{
	CPunt3D vertexL1, vertexL2;
	CPunt3D ctr[3];		// Punts control del patch de la línia.
	int patch = 0;		// Patch actual.
	GLfloat t = 0;

// VAO
	CVAO polyVAO;		
	polyVAO.vaoId = 0;		polyVAO.vboId = 0;		polyVAO.nVertexs = 0;

	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();			colors.clear();		// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	GLfloat cColor[4];
	glGetFloatv(GL_CURRENT_COLOR, cColor);

// TRIEDRE DE FRENET
	CPunt3D VT = { 0.0, 0.0, 0.0f, 1.0f };
	CPunt3D VNP = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VBN = { 0.0f, 0.0f, 0.0f, 1.0f };

// Càrrega primers punts de control.
	for (int i = 0; i < 3; i++)
	{	ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;
	}
	//glBegin(GL_LINES);
	vertexL1 = Punt_PolyLine(t, ctr);
	t = t + pas;
	while (patch <= nptsPolyLine - 2) {
		if (t >= 1.0) {
			vertexL2 = Punt_PolyLine(1.0, ctr);
			// Colorejar cada patch
			if (patch == 0) glColor3f(1.0, 0.0, 0.0);
			else if (patch == 1) glColor3f(1.0, 0.5, 0.0);
			else if (patch == 2) glColor3f(1.0, 1.0, 0.0);
			else if (patch == 3) glColor3f(1.0, 0.0, 1.0);
			else if (patch == 4) glColor3f(1.0, 0.5, 1.0);
			else if (patch == 5) glColor3f(1.0, 1.0, 1.0);
			else if (patch == 6) glColor3f(1.0, 0.5, 0.5);
			else if (patch == 7) glColor3f(0.7, 0.5, 0.3);
			else glLineWidth(4.0);

			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);	// Vértex 1 de la línia
			vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs

			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);	// Vèrtex 2 de la línia
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs
			
			vertexL1 = vertexL2;
			t = 0;
			patch++;
			if (patch <= nptsPolyLine - 2)
			{	if (patch < nptsPolyLine - 2)
				{	for (int i = 0; i < 3; i++)
					{	ctr[i].x = ctr_points[patch + i].x;
						ctr[i].y = ctr_points[patch + i].y;
						ctr[i].z = ctr_points[patch + i].z;
					}
				}
				else {	for (int i = 0; i < 2; i++)
						{	ctr[i].x = ctr_points[patch + i].x;
							ctr[i].y = ctr_points[patch + i].y;
							ctr[i].z = ctr_points[patch + i].z;
						}
						ctr[2].x = ctr_points[0].x;
						ctr[2].y = ctr_points[0].y;
						ctr[2].z = ctr_points[0].z;
					}
			}
		}
		else if (patch <= nptsPolyLine - 2) {
			//				if (t>1.0) vertexL2 = Punt_PolyLine(1.0, ctr);
			//				else 
			vertexL2 = Punt_PolyLine(t, ctr);
			// Colorejar cada patch
			if (patch == 0) glColor3f(1.0, 0.0, 0.0);
			else if (patch == 1) glColor3f(1.0, 0.5, 0.0);
			else if (patch == 2) glColor3f(1.0, 1.0, 0.0);
			else if (patch == 3) glColor3f(1.0, 0.0, 1.0);
			else if (patch == 4) glColor3f(1.0, 0.5, 1.0);
			else if (patch == 5) glColor3f(1.0, 1.0, 1.0);
			else if (patch == 6) glColor3f(1.0, 0.5, 0.5);
			else if (patch == 7) glColor3f(0.7, 0.5, 0.3);
			else glLineWidth(4.0);

			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);	// Vértex 1 de la línia
			vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs

			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);	// Vèrtex 2 de la línia
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs

			vertexL1 = vertexL2;
			t = t + pas;
		}
	}
	//glEnd();

// ----------------------- VAO
// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
	polyVAO = load_LINES_VAO(vertices, colors);

	return polyVAO;
}


// draw_TFPolyLine: Visualització del Triedre de Frenet del PolyLine de cada vèrtex del polyLine
//					donada la seqüència de punts 
void draw_TFPolyLine(GLuint sh_programID, CPunt3D* ctr_points, int nptsPolyLine, float pas)
{
	CPunt3D vertexL1, vertexL2;
	CPunt3D ctr[3];		// Punts control del patch de la línia.
	int patch = 0;		// Patch actual.
	GLfloat t = 0;

// TRIEDRE DE FRENET
	CPunt3D VT = { 0.0, 0.0, 0.0f, 1.0f };
	CPunt3D VNP = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VBN = { 0.0f, 0.0f, 0.0f, 1.0f };

// Càrrega primers punts de control.
	for (int i = 0; i < 3; i++)
	{
		ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;
	}
	vertexL1 = Punt_PolyLine(t, ctr);
	// Càlcul i dibuix Triedre de Frenet en vertexL1	
	VT = VT_PolyLine(ctr);
	VBN = VBN_PolyLine(ctr);
	VNP = VNP_PolyLine(VBN, VT);
	dibuixa_Triedre_Frenet(sh_programID, vertexL1, VT, VNP, VBN);
	t = t + pas;
	while (patch <= nptsPolyLine - 2) {
		if (t >= 1.0)
		{	vertexL2 = Punt_PolyLine(1.0, ctr);
			// Dibuix en vertexL2 del Triedre de Frenet calculat al principi de la recta
			//dibuixa_TriedreFrenet(vertexL2, VT, VNP, VBN);
		t = 0.0;
			patch++;
			if (patch <= nptsPolyLine - 2)
			{	if (patch < nptsPolyLine - 2)
				{	for (int i = 0; i < 3; i++)
					{	ctr[i].x = ctr_points[patch + i].x;
						ctr[i].y = ctr_points[patch + i].y;
						ctr[i].z = ctr_points[patch + i].z;
					}
				}
				else {
					for (int i = 0; i < 2; i++)
						{	ctr[i].x = ctr_points[patch + i].x;
							ctr[i].y = ctr_points[patch + i].y;
						ctr[i].z = ctr_points[patch + i].z;
						}
					// Si cadena de punts Tancat (primer punt [0] igual a últim[nptsPolyLine]), agafar punt [1]
					if ((ctr[1].x == ctr[2].x) && (ctr[1].y = ctr[2].y) && (ctr[1].z == ctr[2].z))
						{	ctr[2].x = ctr_points[1].x;
							ctr[2].y = ctr_points[1].y;
						ctr[2].z = ctr_points[1].z;
						}
					// Si cadena de punts no Tancada, agafar primer punt com a últim
					else {	ctr[2].x = ctr_points[0].x;
							ctr[2].y = ctr_points[0].y;
							ctr[2].z = ctr_points[0].z;
						}
				}
			}
			vertexL1 = Punt_PolyLine(t, ctr);
			// Càlcul i dibuix Triedre de Frenet en vertexL1 (primer vèrtex del patch de línia)	
			VT = VT_PolyLine(ctr);
			VBN = VBN_PolyLine(ctr);
			VNP = VNP_PolyLine(VBN, VT);
			dibuixa_Triedre_Frenet(sh_programID, vertexL1, VT, VNP, VBN);
		}
		else if (patch <= nptsPolyLine - 2) {
			vertexL2 = Punt_PolyLine(t, ctr);
			// Dibuix en vertexL2 del Triedre de Frenet calculat al principi de la recta
			dibuixa_Triedre_Frenet(sh_programID, vertexL2, VT, VNP, VBN);
			//	vertexL1 = vertexL2;
			t = t + pas;
			}
	}
}

// Punt_Polyline: Calcul del punt del Polyline en coordenades 3D (CPunt3D) segons el 
//					paràmetre t i els punts de control (extrems) ctr del segment (patch) 
CPunt3D Punt_PolyLine(float t, CPunt3D* ctr)
{
	CPunt3D p = { (0.0f, 0.0f, 0.0f, 1.0f) };
	CPunt3D lambda;

	// Càlcul de la pendent
	lambda.x = ctr[1].x - ctr[0].x;	lambda.y = ctr[1].y - ctr[0].y;	lambda.z = ctr[1].z - ctr[0].z;

	// Càlcul de la Posició
	p.x = ctr[0].x + t * lambda.x;
	p.y = ctr[0].y + t * lambda.y;
	p.z = ctr[0].z + t * lambda.z;
	return p;
}

// D_PolyLine: Calcul de la derivada de la Polyline en coordenades 3D (CPunt3D) segons el 
//             els punts de control (extrems) v1-v0 continguts en la variable ctr del patch (v0,v1,v2) 
CPunt3D D_PolyLine(CPunt3D* ctr)
{
	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Càlcul de la Primera Derivada v1-v0
	dp.x = ctr[1].x - ctr[0].x;
	dp.y = ctr[1].y - ctr[0].y;
	dp.z = ctr[1].z - ctr[0].z;

	return dp;
}


// D2_PolyLine: Calcul de la segona derivada de la PolyLine en coordenades 3D (CPunt3D) segons el 
//              els punts de control (extrems) v2-v0 continguts en la variable ctr del patch (v0,v1,v2) 
CPunt3D D2_PolyLine(CPunt3D* ctr)
{
	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };

	// Càlcul de la Primera Derivada v2-v0
	dp.x = ctr[2].x - ctr[0].x;
	dp.y = ctr[2].y - ctr[0].y;
	dp.z = ctr[2].z - ctr[0].z;

	return dp;
}

//-- TRIEDRE DE FRENET PER A POLYLINE

// VT_PolyLine: Calcul del Vector Tangent (primera derivada) del PolyLine en coordenades 3D (CPunt3D) segons
//              els punts de control (extrems) continguts en la variable ctr del patch (v0,v1,v2). Normalitzat.
CPunt3D VT_PolyLine(CPunt3D* ctr)
{
	CPunt3D vt = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt = D_PolyLine(ctr);

	// Convertim el vector en vector unitat (normalització)
	longitut = sqrt(vt.x * vt.x + vt.y * vt.y + vt.z * vt.z);
	vt.x = vt.x / longitut;
	vt.y = vt.y / longitut;
	vt.z = vt.z / longitut;

	return vt;
}


// VBN_PolyLine: Calcul del Vector Normal Principal (segona derivada) de la PolyLine en coordenades 3D (CPunt3D) segons
//				 els punts de control (extrems) continguts en la variable ctr del patch (v0,v1,v2).. Normalitzat.
CPunt3D VBN_PolyLine(CPunt3D* ctr)
{
	CPunt3D vt1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vt2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vbn = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt1 = D_PolyLine(ctr);  // Vector Tangent v1-v0
	vt2 = D2_PolyLine(ctr); // Vector tangent V2-v0

	vbn = Prod_Vectorial(vt1, vt2);

	// Convertim el vector en vector unitat (normalització)
	//	longitut = sqrt(vbn.x * vbn.x + vbn.y * vbn.y + vbn.z * vbn.z);
	//	vbn.x = vbn.x / longitut;
	//	vbn.y = vbn.y / longitut;
	//	vbn.z = vbn.z / longitut;
	return vbn;
}


// VNP_Polyline: Càlcul del Vector Normal principal a partir dels vectors Tangent (VT) i 
//				 el Binormal (VBN) com a producte vectorial d'ambós. Normalitzat.
CPunt3D VNP_PolyLine(CPunt3D VBN, CPunt3D VT)
{
	CPunt3D vnp = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vnp = Prod_Vectorial(VBN, VT);

// We normalize the vector
	//	longitut = sqrt(vnp.x * vnp.x + vnp.y * vnp.y + vnp.z * vnp.z);
	//	vnp.x = vnp.x / longitut;
	//	vnp.y = vnp.y / longitut;
	//	vnp.z = vnp.z / longitut;
	return vnp;
}
//----------  FI POLYLINE (Interpolació linial entre punts de control) -----------


//----------- CORBA LEMNISCATA 2D ------------------------------------------------
void draw_Lemniscata2D(float escala, float pas)
{
	Set_VAOList(CRV_LEMNISCATA2D, load_Lemniscata2D_VAO(escala, pas));
	draw_LinVAO_Object(CRV_LEMNISCATA2D); //drawCRVLemniscata2D(npunts);
	deleteVAOList(CRV_LEMNISCATA2D);
}

CVAO load_Lemniscata2D_VAO(float escala, float pas)
{
	float t = 0;
	CPunt3D vertexL1, vertexL2;

// VAO
	CVAO lemniVAO;		
	lemniVAO.vaoId = 0;		lemniVAO.vboId = 0;	lemniVAO.nVertexs = 0;
	
	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();			colors.clear();		// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	if (pas == 0) return lemniVAO;
	else {
		vertexL1 = Punt_Lemniscata2D(t, escala);
		t = t + pas;
		//glBegin(GL_LINES);
		while (t <= 360) {
			vertexL2 = Punt_Lemniscata2D(t, escala);
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs

			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs

			vertexL1 = vertexL2;
			t = t + pas;
		}
		//glEnd();

// ----------------------- VAO
// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
	lemniVAO = load_LINES_VAO(vertices, colors);

	return lemniVAO;
	}
}


CVAO load_Lemniscata2D_EBO(float escala, float pas)
{
	float t = 0;
	CPunt3D vertexL1, vertexL2;
	GLint index = 0;		// Apuntador a vèrtexs per a indices.

// VAO
	CVAO lemniVAO;
	lemniVAO.vaoId = 0;		lemniVAO.vboId = 0;		lemniVAO.eboId = 0;		lemniVAO.nVertexs = 0;	lemniVAO.nIndices = 0;

	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs, normals i textures 
	std::vector <uint> indices;					// Vector dinàmic per a indexs EBO
	//vertices.resize(0);		colors.resize(0);	indices.resize(0);	// Reinicialitzar vectors
	vertices.clear();			colors.clear();		indices.clear();		// Reinicialitzar vectors

	if (pas == 0) return lemniVAO;
	else {
		// Definició del primer vèrtex de la corba.
		vertexL1 = Punt_Lemniscata2D(t, escala);
		colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs

		t = t + pas;
		//glBegin(GL_LINES);
		while (t <= 360) {
			vertexL2 = Punt_Lemniscata2D(t, escala);
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);
			//colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs
			indices.push_back(index);		// Vector indices Primer Vèrtex segment

			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs
			indices.push_back(index + 1);	// Vector indices Vèrtex segment
			index = index + 1;

			//vertexL1 = vertexL2;
			t = t + pas;
		}
		//glEnd();

// ----------------------- VAO
// Creació d'un VAO, VBO i EBO per a la càrrega de la geometria. Guardar identificador VAO, VBO, EBO nVertexs i nIndices a struct CVAO.
	lemniVAO = load_LINES_EBO(vertices, colors, indices);

	return lemniVAO;
	}
}


// draw_TFLemniscata2D: Visualització del Triedre de Frenet de la corba de lemniscata 2D donada l'escala, 
//						amb increment pas i si volem visualtzar el Triedre de Frenet 
void draw_TFLemniscata2D(GLuint sh_programID, float escala, float pas)
{
	float t = 0;
	CPunt3D vertexL1, vertexL2;
// TRIEDRE DE FRENET
	CPunt3D VT = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VNP = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VBN = { 0.0f, 0.0f, 0.0f, 1.0f };

// Càlcul i dibuix Triedre de Frenet en cada vèrtex de la corba Lemniscata3D
	t = 0;
	vertexL1 = Punt_Lemniscata2D(t, escala);
// Càlcul i dibuix Triedre de Frenet en vertexL1
	VT = VT_Lemniscata2D(t, escala);
	VBN = VBN_Lemniscata2D(t, escala);
	VNP = Vector_Normal_Principal(VBN, VT);
	dibuixa_Triedre_Frenet(sh_programID, vertexL1, VT, VNP, VBN);

	t = t + pas;
	while (t <= 360) {	vertexL2 = Punt_Lemniscata2D(t, escala);
						// Càlcul i dibuix Triedre de Frenet en vertexL2
						VT = VT_Lemniscata2D(t, escala);
						VBN = VBN_Lemniscata2D(t, escala);
						VNP = Vector_Normal_Principal(VBN, VT);
						dibuixa_Triedre_Frenet(sh_programID, vertexL2, VT, VNP, VBN);
						vertexL1 = vertexL2;
						t = t + pas;
					}
}

// Punt_Lemniscata2D: Calcul de la posició (x,y,z) segons lemniscata 2D (CPoint3D)
CPunt3D Punt_Lemniscata2D(float t, float scale)
{
	CPunt3D p = { (0, 0, 0) };
	const double a = 1.0 * scale * (0.6); // paràmetre lemniscata

	float bet = sqrtf(2.0) / 2;

	float x1 = a * cos(2 * t * PI / 180) * cos(t * PI / 180);
	float y1 = a * cos(2 * t * PI / 180) * sin(t * PI / 180);

// Càlcul de la Posició
	p.x = bet * x1 + bet * y1;
	p.y = -bet * x1 + bet * y1;
	p.z = 0;
	return p;
}

// D_CLemniscata2D: Calcul de la primera derivada de lemniscata 2D (CPoint3D)
CPunt3D D_Lemniscata2D(float t, float scale)
{
	const double a = 1.0 * scale * (0.6); // paràmetre lemniscata

	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };

	float bet = sqrtf(2.0) / 2;

	float dx1 = -2 * a * sin(2 * t * PI / 180) * cos(t * PI / 180) - a * cos(2 * t * PI / 180) * sin(t * PI / 180);
	float dy1 = -2 * a * sin(2 * t * PI / 180) * sin(t * PI / 180) + a * cos(2 * t * PI / 180) * cos(t * PI / 180);
	float dz1 = 0.0f;	// Trajectòria 1

// Càlcul de la Primera Derivada
	dp.x = bet * dx1 + bet * dy1;
	dp.y = -bet * dx1 + bet * dy1;
	dp.z = 0.0;

	return dp;
}


// D2CLemniscata3D: Calcul de la segona derivada de lemniscata 3D (CPoint3D)
CPunt3D D2_Lemniscata2D(float t, float scale)
{
	const double a = 1.0 * scale * (0.6); // paràmetre lemniscata

	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };

	float bet = sqrtf(2.0) / 2;

	float dx1 = -4 * a * cos(2 * t * PI / 180) * cos(t * PI / 180) + 2 * a * sin(2 * t * PI / 180) * sin(t * PI / 180) + 2 * a * sin(2 * t * PI / 180) * sin(t * PI / 180) - a * cos(2 * t * PI / 180) * cos(t * PI / 180);
	float dy1 = -4 * a * cos(2 * t * PI / 180) * sin(t * PI / 180) - 2 * a * sin(2 * t * PI / 180) * cos(t * PI / 180) - 2 * a * sin(2 * t * PI / 180) * cos(t * PI / 180) - a * cos(2 * t * PI / 180) * sin(t * PI / 180);
	float dz1 = 0.0;		// Trajectòria 1

// Càlcul de la Segona Derivada
	dp.x = bet * dx1 + bet * dy1;
	dp.y = -bet * dx1 + bet * dy1;
	dp.z = 0.0;

	return dp;
}


//-- TRIEDRE DE FRENET PER A CORBES LEMNISCATA 2D

// VT_Lemniscata2D: Calcul del Vector Tangent (primera derivada) de la corba Lemniscata 2D en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i l'escala scale. Normalitzat.
CPunt3D VT_Lemniscata2D(float t, float scale)
{
	CPunt3D vt = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt = D_Lemniscata2D(t, scale);

	// Convertim el vector en vector unitat (normalització)
	longitut = sqrt(vt.x * vt.x + vt.y * vt.y);
	vt.x = vt.x / longitut;
	vt.y = vt.y / longitut;
	//vt.z = 0;

	return vt;
}

// VBN_Lemniscata2D: Calcul del Vector Normal Principal (segona derivada) de la corba Lemniscata 2D en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i l'escala scale. Normalitzat.
CPunt3D VBN_Lemniscata2D(float t, float scale)
{
	CPunt3D vt1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vt2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vbn = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt1 = D_Lemniscata2D(t, scale);
	vt2 = D2_Lemniscata2D(t, scale);

	vbn = Prod_Vectorial(vt1, vt2);

// Convertim el vector en vector unitat (normalització)
	//	longitut = sqrt(vbn.x * vbn.x + vbn.y * vbn.y);
	//	vbn.x = vbn.x / longitut;
	//	vbn.y = vbn.y / longitut;
		//vnp.z = 0.0;
	return vbn;
}
//----------- FI CORBA LEMNISCATA 2D ---------------------------------------------

//----------- CORBA LEMNISCATA 3D ------------------------------------------------
void draw_Lemniscata3D(float escala, float pas)
{
	Set_VAOList(CRV_LEMNISCATA3D, load_Lemniscata3D_VAO(escala, pas));
	draw_LinVAO_Object(CRV_LEMNISCATA3D); // drawCRVLemniscata3D(npunts);
	deleteVAOList(CRV_LEMNISCATA3D);
}


// load_Lemniscata3D_VAO: Càrrega en VAO de la corba de lemniscata 3D donada l'escala, 
//             amb increment pas i si volem visualtzar el Triedre de Frenet 
CVAO load_Lemniscata3D_VAO(float escala, float pas)
{
// VAO
	CVAO lemniVAO;		
	lemniVAO.vaoId = 0;		lemniVAO.vboId = 0;	lemniVAO.nVertexs = 0;
	
	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();			colors.clear();		// Reinicialitzar vectors

	float t = 0;
	CPunt3D vertexL1, vertexL2;
	if (pas == 0) return lemniVAO;
	else {
		vertexL1 = Punt_Lemniscata3D(t, escala);
		t = t + pas;
		//glBegin(GL_LINES);
		while (t <= 360) {
			vertexL2 = Punt_Lemniscata3D(t, escala);
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs

			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs

			vertexL1 = vertexL2;
			t = t + pas;
		}
		//glEnd();

// ----------------------- VAO
		std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
		lemniVAO = load_LINES_VAO(vertices, colors);
		lemniVAO.nVertexs = int(nv / 3);
		
		return lemniVAO;
	}
}


// load_Lemniscata3D_EBO: Càrrega en VAO i EBO de la corba de lemniscata 3D donada l'escala, 
//             amb increment pas i si volem visualtzar el Triedre de Frenet 
CVAO load_Lemniscata3D_EBO(float escala, float pas)
{
	float t = 0;
	CPunt3D vertexL1, vertexL2;
	GLint index = 0;		// Apuntador a vèrtexs per a indices.

// VAO
	CVAO lemniVAO;
	lemniVAO.vaoId = 0;		lemniVAO.vboId = 0;		lemniVAO.eboId = 0;  lemniVAO.nVertexs = 0; lemniVAO.nIndices = 0;

	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs i color 
	std::vector <uint> indices;					// Vector dinàmic per a indexs EBO
	//vertices.resize(0);		colors.resize(0);	indices.resize(0);	// Reinicialitzar vectors
	vertices.clear();			colors.clear();		indices.clear();		// Reinicialitzar vectors

	if (pas == 0) return lemniVAO;
	else {
		// Definició del primer vèrtex de la corba.
		vertexL1 = Punt_Lemniscata3D(t, escala);
		colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
		vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs

		t = t + pas;
		//glBegin(GL_LINES);
		while (t <= 360) {
			vertexL2 = Punt_Lemniscata3D(t, escala);
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);
			//colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs
			indices.push_back(index);		// Vector indices Primer Vèrtex segment

			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs
			indices.push_back(index + 1);	// Vector indices Vèrtex segment
			index = index + 1;

			//vertexL1 = vertexL2;
			t = t + pas;
		}
		//glEnd();

// ----------------------- VAO
// Creació d'un VAO, VBO i EBO per a la càrrega de la geometria. Guardar identificador VAO, VBO, EBO nVertexs i nIndices a struct CVAO.
	lemniVAO = load_LINES_EBO(vertices, colors,indices);

	return lemniVAO;
	}
}

// draw_TFLemniscata3D: Visualització del Triedre de Frenet de la corba de lemniscata 3D donada l'escala, 
//             amb increment pas.
void draw_TFLemniscata3D(GLuint sh_programID, float escala, float pas)
{
	float t = 0;
	CPunt3D vertexL1, vertexL2;
	// TRIEDRE DE FRENET
	CPunt3D VT = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VNP = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VBN = { 0.0f, 0.0f, 0.0f, 1.0f };

// Càlcul i dibuix Triedre de Frenet en cada vèrtex de la corba Lemniscata3D
	t = 0;
	vertexL1 = Punt_Lemniscata3D(t, escala);
	// Càlcul i dibuix Triedre de Frenet en vertexL1
	VT = VT_Lemniscata3D(t, escala);
	VBN = VBN_Lemniscata3D(t, escala);
	VNP = Vector_Normal_Principal(VBN, VT);
	dibuixa_Triedre_Frenet(sh_programID, vertexL1, VT, VNP, VBN);

	t = t + pas;
	while (t <= 360) {
		vertexL2 = Punt_Lemniscata3D(t, escala);
		// Càlcul i dibuix Triedre de Frenet en vertexL2
		VT = VT_Lemniscata3D(t, escala);
		VBN = VBN_Lemniscata3D(t, escala);
		VNP = Vector_Normal_Principal(VBN, VT);
		dibuixa_Triedre_Frenet(sh_programID, vertexL2, VT, VNP, VBN);
		vertexL1 = vertexL2;
		t = t + pas;
		}
}

// Punt_Lemniscata3D: Calcul de la posició (x,y,z) segons lemniscata 3D (CPunt3D)
CPunt3D Punt_Lemniscata3D(float t, float scale)
{
	CPunt3D p = { (0, 0, 0) };
	const double a = 1.0*scale*(0.6); // paràmetre lemniscata

	float bet = sqrtf(2.0) / 2;
	//float aeli=0.95*512/2;
	//float beli=0.8*512/2;
	float x1 = a*cos(2 * t*PI / 180)*cos(t*PI / 180);
	float y1 = a*cos(2 * t*PI / 180)*sin(t*PI / 180);
	float z1 = a*(sin(t*PI / 180) + cos(t*PI / 180));	// Trajectòria 1
	//float z1=a*sin(t*PI/180);							// Trajectòria 2

// Càlcul de la Posició
	p.x = bet*x1 + bet*y1;
	p.y = -bet*x1 + bet*y1;

	//p.z=bet*z1+256;		// Elevació 1 (la més alta)
	//p.z=(bet/2)*z1+158;	// Elevació 2 
	p.z = (bet / 4)*z1 + 82;	// Elevació 3
	//p.z=(bet/8)*z1+43;	// Elevació 4
	return p;
}

// D_CLemniscata3D: Calcul de la primera derivada de lemniscata 3D (CPoint3D)
CPunt3D D_Lemniscata3D(float t, float scale)
{
	const double a = 1.0 * scale * (0.6); // paràmetre lemniscata

	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };

	float bet = sqrtf(2.0) / 2;
	//float x1=a*cos(2*t*pi/180)*cos(t*pi/180);
	//float y1=a*cos(2*t*pi/180)*sin(t*pi/180);
	//	float z1=a*(sin(t*pi/180)+cos(t*pi/180));

	float dx1 = -2 * a * sin(2 * t * PI / 180) * cos(t * PI / 180) - a * cos(2 * t * PI / 180) * sin(t * PI / 180);
	float dy1 = -2 * a * sin(2 * t * PI / 180) * sin(t * PI / 180) + a * cos(2 * t * PI / 180) * cos(t * PI / 180);
	float dz1 = a * (cos(t * PI / 180) - sin(t * PI / 180));	// Trajectòria 1
	//float dz1=a*cos(t*pi/180);						// Trajectòria 2

// Càlcul de la Primera Derivada
	dp.x = bet * dx1 + bet * dy1;
	dp.y = -bet * dx1 + bet * dy1;
	dp.z = bet * dz1;

	return dp;
}


// D2_CLemniscata3D: Calcul de la segona derivada de lemniscata 3D (CPoint3D)
CPunt3D D2_Lemniscata3D(float t, float scale)
{
	const double a = 1.0 * scale * (0.6); // paràmetre lemniscata

	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };

	float bet = sqrtf(2.0) / 2;
	//float x1=a*cos(2*t*pi/180)*cos(t*pi/180);
	//float y1=a*cos(2*t*pi/180)*sin(t*pi/180);
	//	float z1=a*(sin(t*pi/180)+cos(t*pi/180));

	float dx1 = -4 * a * cos(2 * t * PI / 180) * cos(t * PI / 180) + 2 * a * sin(2 * t * PI / 180) * sin(t * PI / 180) + 2 * a * sin(2 * t * PI / 180) * sin(t * PI / 180) - a * cos(2 * t * PI / 180) * cos(t * PI / 180);
	float dy1 = -4 * a * cos(2 * t * PI / 180) * sin(t * PI / 180) - 2 * a * sin(2 * t * PI / 180) * cos(t * PI / 180) - 2 * a * sin(2 * t * PI / 180) * cos(t * PI / 180) - a * cos(2 * t * PI / 180) * sin(t * PI / 180);
	float dz1 = -a * sin(t * PI / 180) - cos(t * PI / 180);	// Trajectòria PI
	//float dz1=-a*sin(t*pi/180);						// Trajectòria 2

// Càlcul de la Segona Derivada
	dp.x = bet * dx1 + bet * dy1;
	dp.y = -bet * dx1 + bet * dy1;
	dp.z = bet * dz1;

	return dp;
}


//-- TRIEDRE DE FRENET PER A CORBES LEMNISCATA 3D

// VT_Lemniscata3D: Calcul del Vector Tangent (primera derivada) de la corba lemniscata 3D en coordenades 3D (CPunt3D) segons el 
//             paràmetre i l'escala scale. Normalitzat.
CPunt3D VT_Lemniscata3D(float t, float scale)
{
	CPunt3D vt = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt = D_Lemniscata3D(t, scale);

	// Convertim el vector en vector unitat (normalització)
	longitut = sqrt(vt.x * vt.x + vt.y * vt.y + vt.z * vt.z);
	vt.x = vt.x / longitut;
	vt.y = vt.y / longitut;
	vt.z = vt.z / longitut;

	return vt;
}


// VNP_Lemniscata3D: Calcul del Vector Normal Principal (segona derivada) de la corba lemniscata 3D en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i l'escala scale. Normalitzat.
CPunt3D VBN_Lemniscata3D(float t, float scale)
{
	CPunt3D vt1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vt2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vbn = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt1 = D_Lemniscata3D(t, scale);
	vt2 = D2_Lemniscata3D(t, scale);

	vbn = Prod_Vectorial(vt1, vt2);

// Convertim el vector en vector unitat (normalització)
	//	longitut = sqrt(vbn.x * vbn.x + vbn.y * vbn.y + vbn.z * vbn.z);
	//	vbn.x = vbn.x / longitut;
	//	vbn.y = vbn.y / longitut;
	//	vbn.z = vbn.z / longitut;

	return vbn;
}
//----------- FI CORBA LEMNISCATA 3D ---------------------------------------------


//----------- CORBA B-SPLINE -----------------------------------------------------
void draw_BSpline_Curve(int nptsCorba, CPunt3D ctr_points[MAX_PATCH_CORBA], float pas)
{	
	Set_VAOList(CRV_BSPLINE, load_BSpline_Curve_VAO(nptsCorba, ctr_points, pas));
	draw_LinVAO_Object(CRV_BSPLINE); //drawCRVBSpline(npunts);
	deleteVAOList(CRV_BSPLINE);
}

// load_BSpline_Curve_VAO: Càrrega de la corba de B-Spline donada per nptsCorba punts de control definits en ctr_points, 
//							 amb increment pas com a VAO.
CVAO load_BSpline_Curve_VAO(int nptsCorba, CPunt3D ctr_points[MAX_PATCH_CORBA], float pas)
{
	CPunt3D vertexL1, vertexL2;
	CPunt3D ctr[4];		// Punts control del patch de l'spline.
	int patch = 0;			// Patch actual.
	GLfloat t = 0;

// VAO
	CVAO BSplineVAO;		
	BSplineVAO.vaoId = 0;		BSplineVAO.vboId = 0;		BSplineVAO.nVertexs = 0;

	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs, normals i textures 
	//vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();			colors.clear();		// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	//t = t - pas;
// Càrrega primers punts de control.
	for (int i = 0; i<4; i++)
	{	ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;
	}
	//glBegin(GL_LINES);
	vertexL1 = Punt_Corba_BSpline(t, ctr);
	t = t + pas;
	while (patch <= nptsCorba - 4){
		if (t > 1.0 + 2 * pas)	{
			t -= 1.0;
			patch++;
			if (patch <= nptsCorba - 4)
			{
				for (int i = 0; i < 4; i++)
				{
					ctr[i].x = ctr_points[patch + i].x;
					ctr[i].y = ctr_points[patch + i].y;
					ctr[i].z = ctr_points[patch + i].z;
				}
			}
		}
		if (patch <= nptsCorba - 4) {
			if (t>1.0) vertexL2 = Punt_Corba_BSpline(1.0, ctr);
			else vertexL2 = Punt_Corba_BSpline(t, ctr);
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);	// Vertex 1 de la línia
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs
			
			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);	// Vertex 2 de la línia
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs
			vertexL1 = vertexL2;
			t = t + pas;
		}
	}
	//glEnd();

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

	BSplineVAO = load_LINES_VAO(vertices, colors);
	BSplineVAO.nVertexs = int(nv / 3);

	return BSplineVAO;
}


// load_BSpline_Curve_EBO: Càrrega de la corba de B-Spline donada per nptsCorba punts de control definits en ctr_points, 
//							 amb increment pas com a VAO amb EBO.
CVAO load_BSpline_Curve_EBO(int nptsCorba, CPunt3D ctr_points[MAX_PATCH_CORBA], float pas)
{
	CPunt3D vertexL1, vertexL2;
	CPunt3D ctr[4];		// Punts control del patch de l'spline.
	int patch = 0;			// Patch actual.
	GLfloat t = 0;
	GLint index = 0;		// Apuntador a vèrtexs per a indices.

// VAO
	CVAO BSplineVAO;
	BSplineVAO.vaoId = 0;		BSplineVAO.vboId = 0;		BSplineVAO.eboId = 0; BSplineVAO.nVertexs = 0;	BSplineVAO.nIndices = 0;

	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs i color 
	std::vector <uint> indices;					// Vector dinàmic per a indexs EBO
	//vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();			colors.clear();		indices.clear();		// Reinicialitzar vectors

// Càrrega primers punts de control.
	for (int i = 0; i < 4; i++) {
		ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;
		}
	//glBegin(GL_LINES);
	// Definició del primer vèrtex de la corba.
	vertexL1 = Punt_Corba_BSpline(t, ctr);
	colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
	vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs

	t = t + pas;
	while (patch <= nptsCorba - 4) {
		if (t > 1.0 + 2 * pas) {
			t -= 1.0;
			patch++;
			if (patch <= nptsCorba - 4)	{
				for (int i = 0; i < 4; i++)	{
					ctr[i].x = ctr_points[patch + i].x;
					ctr[i].y = ctr_points[patch + i].y;
					ctr[i].z = ctr_points[patch + i].z;
					}
				}
		}
		if (patch <= nptsCorba - 4) {
			if (t > 1.0) vertexL2 = Punt_Corba_BSpline(1.0, ctr);
			else vertexL2 = Punt_Corba_BSpline(t, ctr);
			vertexL2 = Punt_Corba_BSpline(t, ctr);
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);	// Vertex 1 de la línia
			//colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs
			indices.push_back(index);		// Vector indices Primer Vèrtex segment

			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);	// Vertex 2 de la línia
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs
			indices.push_back(index + 1);	// Vector indices Vèrtex segment
			index = index + 1;
			//vertexL1 = vertexL2;
			t = t + pas;
		}
	}
	//glEnd();

// ----------------------- VAO
// Creació d'un VAO, VBO i EBO per a la càrrega de la geometria. Guardar identificador VAO, VBO, EBO nVertexs i nIndices a struct CVAO.
	BSplineVAO = load_LINES_EBO(vertices, colors,indices);

	return BSplineVAO;
}


// draw_TFBSpline_Curve: Visualització del Triedre de Drenet de la corba de B-Spline donada per nptsCorba punts de control definits en ctr_points, 
//             amb increment pas.
void draw_TFBSpline_Curve(GLuint sh_programID, CPunt3D* ctr_points, int nptsCorba, float pas)
{
	CPunt3D vertexL1, vertexL2;
	CPunt3D ctr[4];		// Punts control del patch de la B-Spline.
	int patch = 0;		// Patch actual.
	GLfloat t = 0;
	// TRIEDRE DE FRENET
	CPunt3D VT = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VNP = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VBN = { 0.0f, 0.0f, 0.0f, 1.0f };

	//t = t - pas;
// Càrrega primers punts de control.
	for (int i = 0; i < 4; i++)
	{
		ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;
	}

// Càlcul i dibuix Triedre de Frenet en cada vèrtex de la corba B-Spline
	vertexL1 = Punt_Corba_BSpline(t, ctr);

// Càlcul i dibuix Triedre de Frenet en vertexL1
	VT = VT_BSpline_Curve(t, ctr);
	VBN = VBN_BSpline_Curve(t, ctr);
	VNP = Vector_Normal_Principal(VBN, VT);
	dibuixa_Triedre_Frenet(sh_programID, vertexL1, VT, VNP, VBN);

	t = t + pas;
	while (patch <= nptsCorba - 4) {
		if (t >= 1.0)
			{	vertexL2 = Punt_Corba_BSpline(1.0, ctr);
				// Càlcul i dibuix Triedre de Frenet en vertexL2
				VT = VT_BSpline_Curve(t, ctr);
				VBN = VBN_BSpline_Curve(t, ctr);
				VNP = Vector_Normal_Principal(VBN, VT);
				dibuixa_Triedre_Frenet(sh_programID, vertexL2, VT, VNP, VBN);

				t = 0.0;
				patch++;
				if (patch <= nptsCorba - 4)
				{	for (int i = 0; i < 4; i++)
					{	ctr[i].x = ctr_points[patch + i].x;
						ctr[i].y = ctr_points[patch + i].y;
						ctr[i].z = ctr_points[patch + i].z;
					}
				}
			}
			else if (patch <= nptsCorba - 4) {
//				if (t>1.0) vertexL2 = Punt_Corba_BSpline(1.0, ctr);
//				else 
				vertexL2 = Punt_Corba_BSpline(t, ctr);

// Càlcul i dibuix Triedre de Frenet en vertexL2
				VT = VT_BSpline_Curve(t, ctr);
				VBN = VBN_BSpline_Curve(t, ctr);
				VNP = Vector_Normal_Principal(VBN, VT);
				dibuixa_Triedre_Frenet(sh_programID, vertexL2, VT, VNP, VBN);

				vertexL1 = vertexL2;
				t = t + pas;
			}
		}
}


// Punt_Corba_Spline: Calcul del punt del spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre i i els punts de control ctr 
CPunt3D Punt_Corba_BSpline(float t, CPunt3D * ctr)
{
	CPunt3D p = { (0, 0, 0) };
	float coef[4];
	int i, j;

// Polinomis que multipliquen els punts de control del patch
	for (i = 0; i<4; i++)
	{
		coef[i] = 0;
		for (j = 0; j<4; j++)
			coef[i] = coef[i] * t + AS[i][j];
	}

// Càlcul de la Posició
	for (i = 0; i<4; i++)
	{
		p.x += coef[i] * ctr[i].x;
		p.y += coef[i] * ctr[i].y;
		p.z += coef[i] * ctr[i].z;
	}
	return p;

}


// D_BSpline_Curve: Calcul de la derivada del B-Spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i els punts de control ctr
CPunt3D D_BSpline_Curve(float t, CPunt3D* ctr)
{
	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };
	float coef[4];
	int i, j;

// Polinomis que multipliquen els punts de control del patch
	for (i = 0; i < 4; i++)
	{
		coef[i] = 0;
		for (j = 0; j < 3; j++)
			coef[i] = coef[i] * t + (3 - j) * AS[i][j];
	}

// Càlcul de la Primera Derivada
	for (i = 0; i < 4; i++)
	{
		dp.x += coef[i] * ctr[i].x;
		dp.y += coef[i] * ctr[i].y;
		dp.z += coef[i] * ctr[i].z;
	}
	return dp;
}


// D2_BSpline_Curve: Calcul de la segona derivada del B-Spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i els punts de control ctr
CPunt3D D2_BSpline_Curve(float t, CPunt3D* ctr)
{
	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };
	float coef[4];
	int i, j;

// Polinomis que multipliquen els punts de control del patch
	for (i = 0; i < 4; i++)
	{
		coef[i] = 0;
		for (j = 0; j < 2; j++)
			coef[i] = coef[i] * t + (3 - j) * (2 - j) * AS[i][j];
	}

// Càlcul de la Segona Derivada
	for (i = 0; i < 4; i++)
	{
		dp.x += coef[i] * ctr[i].x;
		dp.y += coef[i] * ctr[i].y;
		dp.z += coef[i] * ctr[i].z;
	}
	return dp;
}

//-- TRIEDRE DE FRENET PER A CORBES SPLINE
// VT_BSpline_Curve: Calcul del Vector Tangent (primera derivada) del B-Spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i els punts de control ctr. Normalitzat.
CPunt3D VT_BSpline_Curve(float t, CPunt3D* ctr)
{
	CPunt3D vt = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt = D_BSpline_Curve(t, ctr);

	// Convertim el vector en vector unitat (normalització)
	longitut = sqrt(vt.x * vt.x + vt.y * vt.y + vt.z * vt.z);
	vt.x = vt.x / longitut;
	vt.y = vt.y / longitut;
	vt.z = vt.z / longitut;

	return vt;
}

// VBN_BSpline_Curve: Calcul del Vector BiNormal del B-Spline en coordenades 3D (CPunt3D) segons el 
//             paràmetre t i els punts de control ctr. Normalitzat.
CPunt3D VBN_BSpline_Curve(float t, CPunt3D* ctr)
{
	CPunt3D vt1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vt2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vbn = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt1 = D_BSpline_Curve(t, ctr);
	vt2 = D2_BSpline_Curve(t, ctr);

	vbn = Prod_Vectorial(vt1, vt2);

// Convertim el vector en vector unitat (normalització)
	//	longitut = sqrt(vbn.x * vbn.x + vbn.y * vbn.y + vbn.z * vbn.z);
	//	vbn.x = vbn.x / longitut;
	//	vbn.y = vbn.y / longitut;
	//	vbn.z = vbn.z / longitut;
	return vbn;
}
//----------- FI CORBA B-SPLINE --------------------------------------------------


//----------- CORBA BEZIER -------------------------------------------------------
// draw_Bezier_Curve: Visualització de la corba de Bezier donada per nptsCorba punts de control definits en ctr_points, 
//             amb increment pas, corba tancada o no i si volem visualtzar el Triedre de Frenet 
void draw_Bezier_Curve(int nptsCorba, CPunt3D ctr_points[MAX_PATCH_CORBA], float pas, bool tancat)
{
	Set_VAOList(CRV_BEZIER, load_Bezier_Curve_VAO(nptsCorba, ctr_points, pas, tancat));
	draw_LinVAO_Object(CRV_BEZIER); // drawCRVBezier(npunts);
	deleteVAOList(CRV_BEZIER);
}

// load_Bezier_Curve_VAO: Càrrega en VAO dels punts de la corba de Bezier donada per nptsCorba punts de control definits en ctr_points, 
//             amb increment pas, corba tancada o no i si volem visualtzar el Triedre de Frenet 
CVAO load_Bezier_Curve_VAO(int nptsCorba, CPunt3D ctr_points[MAX_PATCH_CORBA], float pas, bool tancat)
{
// VAO
	CVAO BezierVAO;		
	BezierVAO.vaoId = 0;	BezierVAO.vboId = 0;	BezierVAO.nVertexs = 0;
	
	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs, normals i textures 
	vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors

// Obtenir color actual definit en OpenGL amb glColor();
	//GLfloat cColor[4];
	//glGetFloatv(GL_CURRENT_COLOR, cColor);

	int i=0, npuntsCorba = nptsCorba;
	CPunt3D ctrl_points[MAX_PATCH_CORBA];
	CPunt3D vertexL1, vertexL2;
	CPunt3D ctr[4];		// Punts control del patch de l'spline.
	int patch = 0;			// Patch actual.
	GLfloat t = 0;

	if (tancat && npuntsCorba + 3 < MAX_PATCH_CORBA)
	{	for (i=0; i < nptsCorba ; i++) ctrl_points[i] = ctr_points[i];
	i = nptsCorba;
		ctrl_points[i] = ctr_points[0];
		ctrl_points[i + 1] = ctr_points[1];
		ctrl_points[i + 2] = ctr_points[2];
		npuntsCorba = nptsCorba + 3;
	}
	else { for (i = 0; i < nptsCorba; i++) ctrl_points[i] = ctr_points[i];
		}

	//t = t - pas;
// Càrrega primers punts de control.
	for (int i = 0; i<4; i++)
	{	ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;
	}
	//glBegin(GL_LINES);
	vertexL1 = Punt_Bezier_Curve(t, ctr);
	t = t + pas;
//	while (patch <= nptsCorba - 4){
	while (patch <= npuntsCorba - 4) {
		if (t > 1.0 + 2 * pas)	{
			t -= 1.0;
			//patch++;
			patch = patch + 3;
//			if ((patch < nptsCorba - 1) && (patch + 4 > nptsCorba)) patch = nptsCorba - 4;
			if ((patch < npuntsCorba - 1) && (patch + 4 > npuntsCorba)) patch = npuntsCorba - 4;
			//if (patch <= nptsCorba - 4)
			if (patch <= npuntsCorba - 4)
			{	for (int i = 0; i < 4; i++)
				{	ctr[i].x = ctrl_points[patch + i].x;	// ctr[i].x = ctr_points[patch + i].x;
					ctr[i].y = ctrl_points[patch + i].y;	// ctr[i].y = ctr_points[patch + i].y;
					ctr[i].z = ctrl_points[patch + i].z;	// ctr[i].z = ctr_points[patch + i].z;
				}
			}
		}
		//if (patch <= nptsCorba - 4) {
		if (patch <= npuntsCorba - 4) {
			if (t>1.0) vertexL2 = Punt_Bezier_Curve(1.0, ctr);
			else vertexL2 = Punt_Bezier_Curve(t, ctr);
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);	// Vértex 1 de la línia
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs
			
			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);	// Vèrtex 2 de la línia
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs
			
			vertexL1 = vertexL2;
			t = t + pas;
		}
	}
	//glEnd();

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.

// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
	BezierVAO = load_LINES_VAO(vertices, colors);
	BezierVAO.nVertexs = int(nv / 3);
	
	return BezierVAO;
}


// load_Bezier_Curve_EBO: Càrrega en VAO i EBO els punts de la corba de Bezier donada per nptsCorba punts de control definits en ctr_points, 
//             amb increment pas, corba tancada o no i si volem visualtzar el Triedre de Frenet 
CVAO load_Bezier_Curve_EBO(int nptsCorba, CPunt3D ctr_points[MAX_PATCH_CORBA], float pas, bool tancat)
{
	int i = 0, npuntsCorba = nptsCorba;
	CPunt3D ctrl_points[MAX_PATCH_CORBA];
	CPunt3D vertexL1, vertexL2;
	CPunt3D ctr[4];		// Punts control del patch de l'spline.
	int patch = 0;			// Patch actual.
	GLfloat t = 0;
	GLint index = 0;		// Apuntador a vèrtexs per a indices.

// VAO
	CVAO BezierVAO;
	BezierVAO.vaoId = 0;	BezierVAO.vboId = 0;	BezierVAO.eboId = 0; BezierVAO.nVertexs = 0; BezierVAO.nIndices = 0;

	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs i color 
	std::vector <uint> indices;					// Vector dinàmic per a indexs EBO
	vertices.clear();			colors.clear();		indices.clear();		// Reinicialitzar vectors

	if (tancat && npuntsCorba + 3 < MAX_PATCH_CORBA)
	{
		for (i = 0; i < nptsCorba; i++) ctrl_points[i] = ctr_points[i];
		i = nptsCorba;
		ctrl_points[i] = ctr_points[0];
		ctrl_points[i + 1] = ctr_points[1];
		ctrl_points[i + 2] = ctr_points[2];
		npuntsCorba = nptsCorba + 3;
	}
	else {
		for (i = 0; i < nptsCorba; i++) ctrl_points[i] = ctr_points[i];
	}

	//t = t - pas;
// Càrrega primers punts de control.
	for (int i = 0; i < 4; i++)
	{
		ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;
	}
	//glBegin(GL_LINES);

	// Definició del primer vèrtex de la corba.
	vertexL1 = Punt_Bezier_Curve(t, ctr);
	colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
	vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs

	t = t + pas;
	//	while (patch <= nptsCorba - 4){
	while (patch <= npuntsCorba - 4) {
		if (t > 1.0 + 2 * pas) {
			t -= 1.0;
			//patch++;
			patch = patch + 3;
			//			if ((patch < nptsCorba - 1) && (patch + 4 > nptsCorba)) patch = nptsCorba - 4;
			if ((patch < npuntsCorba - 1) && (patch + 4 > npuntsCorba)) patch = npuntsCorba - 4;
			//if (patch <= nptsCorba - 4)
			if (patch <= npuntsCorba - 4)
			{
				for (int i = 0; i < 4; i++)
				{
					ctr[i].x = ctrl_points[patch + i].x;	// ctr[i].x = ctr_points[patch + i].x;
					ctr[i].y = ctrl_points[patch + i].y;	// ctr[i].y = ctr_points[patch + i].y;
					ctr[i].z = ctrl_points[patch + i].z;	// ctr[i].z = ctr_points[patch + i].z;
				}
			}
		}
		//if (patch <= nptsCorba - 4) {
		if (patch <= npuntsCorba - 4) {
			if (t > 1.0) vertexL2 = Punt_Bezier_Curve(1.0, ctr);
			else vertexL2 = Punt_Bezier_Curve(t, ctr);
			//glVertex3f(vertexL1.x, vertexL1.y, vertexL1.z);	// Vértex 1 de la línia
			//colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			//vertices.push_back(vertexL1.x);		vertices.push_back(vertexL1.y);		vertices.push_back(vertexL1.z);	// Vector Vèrtexs
			indices.push_back(index);		// Vector indices Primer Vèrtex segment

			//glVertex3f(vertexL2.x, vertexL2.y, vertexL2.z);	// Vèrtex 2 de la línia
			colors.push_back(cColor[0]);		colors.push_back(cColor[1]);		colors.push_back(cColor[2]);	colors.push_back(cColor[3]); // Vector Colors
			vertices.push_back(vertexL2.x);		vertices.push_back(vertexL2.y);		vertices.push_back(vertexL2.z);	// Vector Vèrtexs
			indices.push_back(index + 1);	// Vector indices Vèrtex segment
			index = index + 1;

			//vertexL1 = vertexL2;
			t = t + pas;
		}
	}
	//glEnd();

// ----------------------- VAO
// Creació d'un VAO, VBO i EBO per a la càrrega de la geometria. Guardar identificador VAO, VBO, EBO nVertexs i nIndices a struct CVAO.
	BezierVAO = load_LINES_EBO(vertices, colors,indices);

	return BezierVAO;
}

// draw_TFBezier_Curve: Visualització del Triedre de Drenet de la corba de Bezier donada per nptsCorba punts de control definits en ctr_points, 
//             amb increment pas, corba tancada o no i si volem visualtzar el Triedre de Frenet 
void draw_TFBezier_Curve(GLuint sh_programID, CPunt3D* ctr_points, int nptsCorba, float pas, bool tancat)
{
	CPunt3D vertexL1, vertexL2;
	CPunt3D ctr[4];		// Punts control del patch de l'spline.
	int patch = 0;			// Patch actual.
	GLfloat t = 0;

// TRIEDRE DE FRENET
	CPunt3D VT = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VNP = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D VBN = { 0.0f, 0.0f, 0.0f, 1.0f };

	//t = t - pas;
// Càrrega primers punts de control.
	for (int i = 0; i < 4; i++)
	{	ctr[i].x = ctr_points[i].x;
		ctr[i].y = ctr_points[i].y;
		ctr[i].z = ctr_points[i].z;
	}

// Càlcul i dibuix Triedre de Frenet en cada vèrtex de la corba Bezier
	vertexL1 = Punt_Bezier_Curve(t, ctr);
// Càlcul i dibuix Triedre de Frenet en vertexL1	
	VT = VT_Bezier_Curve(t, ctr);
	VBN = VBN_Bezier_Curve(t, ctr);
	VNP = Vector_Normal_Principal(VBN, VT);
	dibuixa_Triedre_Frenet(sh_programID, vertexL1, VT, VNP, VBN);
	t = t + pas;
	while (patch <= nptsCorba - 4) {
		if (t >= 1.0)
		{	vertexL2 = Punt_Bezier_Curve(1.0, ctr);
			// Càlcul i dibuix Triedre de Frenet en vertexL2	
			VT = VT_Bezier_Curve(t, ctr);
			VBN = VBN_Bezier_Curve(t, ctr);
			VNP = Vector_Normal_Principal(VBN, VT);
			dibuixa_Triedre_Frenet(sh_programID, vertexL2, VT, VNP, VBN);
			vertexL1 = vertexL2;

			t = 0.0 + pas;		//t = 0.0; 
			patch = patch + 3;	//patch++;
			if ((patch < nptsCorba - 1) && (patch + 4 > nptsCorba)) patch = nptsCorba - 4;
			if (patch <= nptsCorba - 4)
			{	for (int i = 0; i < 4; i++)
				{	ctr[i].x = ctr_points[patch + i].x;
					ctr[i].y = ctr_points[patch + i].y;
					ctr[i].z = ctr_points[patch + i].z;
				}
			}
		}
		else if (patch <= nptsCorba - 4) {
			//				if (t>1.0) vertexL2 = Punt_Corba_Bezier(1.0, ctr);
			//				else 
			vertexL2 = Punt_Bezier_Curve(t, ctr);
			// Càlcul i dibuix Triedre de Frenet en vertexL2	
			VT = VT_Bezier_Curve(t, ctr);
			VBN = VBN_Bezier_Curve(t, ctr);
			VNP = Vector_Normal_Principal(VBN, VT);
			dibuixa_Triedre_Frenet(sh_programID, vertexL2, VT, VNP, VBN);
			vertexL1 = vertexL2;
			t = t + pas;
		}
	}
}


// Punt_Bezier_Curve: Calcul del punt de Bezier en coordenades 3D (CPunt3D) segons el 
//             paràmetre i i els punts de control ctr 
CPunt3D Punt_Bezier_Curve(float t, CPunt3D * ctr)
{
	CPunt3D p = { (0, 0, 0) };
	float coef[4];
	int i, j;

	// Polinomis que multipliquen els punts de control del patch
	for (i = 0; i<4; i++)
	{	coef[i] = 0;
		for (j = 0; j<4; j++)
			coef[i] = coef[i] * t + AB[i][j];
	}

	// Càlcul de la Posició
	for (i = 0; i<4; i++)
	{	p.x += coef[i] * ctr[i].x;
		p.y += coef[i] * ctr[i].y;
		p.z += coef[i] * ctr[i].z;
	}
	return p;
}

// D_Bezier_Curve: Calcul de la derivada de la corba Bezier en coordenades 3D (CPunt3D) segons el 
//             paràmetre i els punts de control ctr
CPunt3D D_Bezier_Curve(float t, CPunt3D* ctr)
{	CPunt3D dp = { 0.0f, 0.0f, 0.0f, 1.0f };
	float coef[4];
	int i, j;

// Polinomis que multipliquen els punts de control del patch
	for (i = 0; i < 4; i++)
	{	coef[i] = 0;
		for (j = 0; j < 3; j++)
			coef[i] = coef[i] * t + (3 - j) * AB[i][j];
	}

// Càlcul de la Primera Derivada
	for (i = 0; i < 4; i++)
	{
		dp.x += coef[i] * ctr[i].x;
		dp.y += coef[i] * ctr[i].y;
		dp.z += coef[i] * ctr[i].z;
	}
	return dp;
}


// D2_Bezie_Curver: Calcul de la segona derivada de la corba Bezier en coordenades 3D (CPunt3D) segons el 
//             paràmetre i els punts de control ctr
CPunt3D D2_Bezier_Curve(float t, CPunt3D* ctr)
{	CPunt3D dp = { 0, 0, 0 };
	float coef[4];
	int i, j;

// Polinomis que multipliquen els punts de control del patch
	for (i = 0; i < 4; i++)
	{	coef[i] = 0;
		for (j = 0; j < 2; j++)
			coef[i] = coef[i] * t + (3 - j) * (2 - j) * AB[i][j];
	}

// Càlcul de la Segona Derivada
	for (i = 0; i < 4; i++)
	{	dp.x += coef[i] * ctr[i].x;
		dp.y += coef[i] * ctr[i].y;
		dp.z += coef[i] * ctr[i].z;
	}
	return dp;
}

//-- TRIEDRE DE FRENET PER A CORBES BEZIER
// VT_Bezier_Curve: Calcul del Vector Tangent (primera derivada) de la corba Bezier en coordenades 3D (CPunt3D) segons el 
//             paràmetre i els punts de control ctr. Normalitzat.
CPunt3D VT_Bezier_Curve(float t, CPunt3D* ctr)
{
	CPunt3D vt = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt = D_Bezier_Curve(t, ctr);

// Convertim el vector en vector unitat (normalització)
	longitut = sqrt(vt.x * vt.x + vt.y * vt.y + vt.z * vt.z);
	vt.x = vt.x / longitut;
	vt.y = vt.y / longitut;
	vt.z = vt.z / longitut;

	return vt;
}


// VBN_Bezier_Curve: Calcul del Vector BiNormal de la corba Bezier en coordenades 3D (CPunt3D) segons el 
//             paràmetre i els punts de control ctr. Normalitzat.
CPunt3D VBN_Bezier_Curve(float t, CPunt3D* ctr)
{
	CPunt3D vt1 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vt2 = { 0.0f, 0.0f, 0.0f, 1.0f };
	CPunt3D vbn = { 0.0f, 0.0f, 0.0f, 1.0f };
	float longitut = 0;

	vt1 = D_Bezier_Curve(t, ctr);
	vt2 = D2_Bezier_Curve(t, ctr);

	vbn = Prod_Vectorial(vt1, vt2);

// Convertim el vector en vector unitat (normalització)
	//	longitut = sqrt(vbn.x * vbn.x + vbn.y * vbn.y + vbn.z * vbn.z);
	//	vbn.x = vbn.x / longitut;
	//	vbn.y = vbn.y / longitut;
	//	vbn.z = vbn.z / longitut;
	return vbn;
}
//----------- FI CORBA BEZIER ----------------------------------------------------

// ---------- ACTIVACIÓ VERTEX ARRAY DE CADA FORMA GL_* PER A VAO, VBO i EBO -----
// ------------------- (LINES, TRIANGLES) ----------------------------------------

// ---------- LINES VAO 
CVAO load_LINES_VAO(std::vector <double> vertices, std::vector <double> colors)
{
	CVAO tmpVAO;
	tmpVAO.vaoId = 0;	tmpVAO.vboId = 0;	tmpVAO.eboId = 0; tmpVAO.nVertexs = 0;	tmpVAO.nIndices = 0;

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.

// Create Vertex Array Object (VAO) for 3D Model Cube
	glGenVertexArrays(1, &tmpVAO.vaoId);

// Create vertex buffer objects for 3D Model attributes in the VAO
	glGenBuffers(1, &tmpVAO.vboId);

// Bind our Vertex Array Object as the current used object
	glBindVertexArray(tmpVAO.vaoId);

// Bind our Vertex Buffer Object as the current used object
	glBindBuffer(GL_ARRAY_BUFFER, tmpVAO.vboId);

	glBufferData(GL_ARRAY_BUFFER, (vertices.size() + colors.size()) * sizeof(double), 0, GL_STATIC_DRAW);	// Allocate data to VBO starting from 0 offest

// Position Vertex attributes
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(double), &vertices[0]);	// Copy geometry data to VBO starting from 0 offest
	glEnableVertexAttribArray(0);											// Enable attribute index 0 as being used (position)
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

// Color Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), colors.size() * sizeof(double), &colors[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(1);										// Enable attribute index 1 as being used (normals)
	glVertexAttribPointer(1, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)(vertices.size() * sizeof(float)));	// Specify that our color data is going into attribute index 0 and contains 3 double

//Unbind the registered VBO as current VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

// Dissable Attrib arrays
	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

// Unbind VAO, to prevent bugs
	glBindVertexArray(0);

	tmpVAO.nVertexs = (int)vertices.size() / 3;

	return tmpVAO;	// retorna CVAO.
}


void draw_LINES_VAO(GLint vaoList_indx)
{
	GLuint vaoId = VAOList[vaoList_indx].vaoId;
	int nvert = VAOList[vaoList_indx].nVertexs;

// Dibuixar VAO 
	if (vaoId != 0) {
		glBindVertexArray(vaoId);
		glDrawArrays(GL_LINES, 0, nvert);
		glBindVertexArray(0);
	}
}


// ---------- LINES EBO
CVAO load_LINES_EBO(std::vector <double> vertices, std::vector <double> colors, std::vector <uint> indices)
{
	CVAO tmpVAO;
	tmpVAO.vaoId = 0;	tmpVAO.vboId = 0;	tmpVAO.eboId = 0;	tmpVAO.nVertexs = 0;	tmpVAO.nIndices = 0;

// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.

// Create Vertex Array Object (VAO) for 3D Model Cube
	glGenVertexArrays(1, &tmpVAO.vaoId);

// Create vertex buffer objects for 3D Model attributes in the VAO
	glGenBuffers(1, &tmpVAO.vboId);

// Create vertex buffer objects for 3D Model EBO Elements attributes in the VAO
	glGenBuffers(1, &tmpVAO.eboId);

// Bind our Vertex Array Object as the current used object
	glBindVertexArray(tmpVAO.vaoId);

// Bind our Vertex Buffer Object as the current used object
	glBindBuffer(GL_ARRAY_BUFFER, tmpVAO.vboId);

	glBufferData(GL_ARRAY_BUFFER, (vertices.size() + colors.size()) * sizeof(double), 0, GL_STATIC_DRAW);	// Allocate data to VBO starting from 0 offest

// Position Vertex attributes
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(double), &vertices[0]);	// Copy geometry data to VBO starting from 0 offest
	glEnableVertexAttribArray(0);											// Enable attribute index 0 as being used (position)
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

// Color Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), colors.size() * sizeof(double), &colors[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(1);										// Enable attribute index 1 as being used (normals)
	glVertexAttribPointer(1, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)(vertices.size() * sizeof(float)));	// Specify that our color data is going into attribute index 0 and contains 3 double

	// Bind our Element Buffer Object as the current used object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpVAO.eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

//Unbind the registered EBO as current EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//Unbind the registered VBO as current VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

// Dissable Attrib arrays
	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);

	// Unbind VAO, to prevent bugs
	glBindVertexArray(0);

	tmpVAO.nVertexs = (int)vertices.size() / 3;
	tmpVAO.nIndices = (GLint)indices.size();

	return tmpVAO;	// retorna CVAO.
}

void draw_LINES_EBO(GLint vaoList_indx)
{
	GLuint vaoId = VAOList[vaoList_indx].vaoId;
	int nvert = VAOList[vaoList_indx].nVertexs;

	// Dibuixar VAO 
	if (VAOList[vaoList_indx].vaoId != 0) {
		glBindVertexArray(VAOList[vaoList_indx].vaoId);
		//glDrawArrays(GL_LINES, 0, nvert);

		// Activar EBO
		glBindVertexArray(VAOList[vaoList_indx].eboId);
		// Dibuixar EBO
		//glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAOList[vaoList_indx].eboId);
		glDrawElements(GL_LINES, VAOList[vaoList_indx].nVertexs, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}
}


// ---------- TRIANGLES VAO
CVAO load_TRIANGLES_VAO(std::vector <double> vertices, std::vector <double> normals, std::vector <double> colors, std::vector <double> textures)
{
// ----------------------- VAO
	CVAO tmpVAO;
	tmpVAO.vaoId = 0;	tmpVAO.vboId = 0;	tmpVAO.nVertexs = 0;

	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	//std::vector <int>::size_type nn = normals.size();	// Tamany del vector normals en elements.
	//std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.
	//std::vector <int>::size_type nt = textures.size();	// Tamany del vector textures en elements.

// Create Vertex Array Object (VAO) for 3D Model Cube
	glGenVertexArrays(1, &tmpVAO.vaoId);

// Create vertex buffer objects for 3D Model attributes in the VAO
	glGenBuffers(1, &tmpVAO.vboId);

// Bind our Vertex Array Object as the current used object
	glBindVertexArray(tmpVAO.vaoId);

// Bind our Vertex Buffer Object as the current used object
	glBindBuffer(GL_ARRAY_BUFFER, tmpVAO.vboId);

	glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size() + colors.size()) * sizeof(double), 0, GL_STATIC_DRAW);

// Position Vertex attributes
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(double), &vertices[0]);	// Copy geometry data to VBO starting from 0 offset
	glEnableVertexAttribArray(0);											// Enable attribute index 0 as being used (position)
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

// Normal Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), normals.size() * sizeof(double), &normals[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(1);											// Enable attribute index 1 as being used (normals)
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (GLvoid*)(vertices.size() * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

// Texture Coordinates Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(double), textures.size() * sizeof(double), &textures[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(2);												// Enable attribute index 2 as being used (texture coordinates)
	glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (GLvoid*)((vertices.size() + normals.size()) * sizeof(double)));						// Specify that our color data is going into attribute index 0 and contains 3 double

// Color Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size()) * sizeof(double), colors.size() * sizeof(double), &colors[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(3);												// Enable attribute index 3 as being used
	glVertexAttribPointer(3, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)((vertices.size() + normals.size() + textures.size()) * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

	// Dissable Attrib arrays
		//glDisableVertexAttribArray(0);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);
		//glDisableVertexAttribArray(3);

	// Unbind VAO, to prevent bugs
	glBindVertexArray(0);

	tmpVAO.nVertexs = (int)vertices.size() / 3;
	tmpVAO.nIndices = 0;

	return tmpVAO;	// retorna CVAO.
}

void draw_TRIANGLES_VAO(GLint vaoList_indx)
{
	//GLuint vaoId = VAOList[vaoList_indx].vaoId;
	//int nvert = VAOList[vaoList_indx].nVertexs;

// Dibuixar VAO
	if (VAOList[vaoList_indx].vaoId != 0) 
	{	glBindVertexArray(VAOList[vaoList_indx].vaoId);					//glBindVertexArray(vaoId);
		glDrawArrays(GL_TRIANGLES, 0, VAOList[vaoList_indx].nVertexs);	//glDrawArrays(GL_TRIANGLES, 0, nvert);
		glBindVertexArray(0);
	}
}


// ---------- TRIANGLES EBO
CVAO load_TRIANGLES_EBO(std::vector <double> vertices, std::vector <double> normals, std::vector <double> colors, std::vector <double> textures, 
	std::vector <uint> indices)
{
// ----------------------- VAO
	CVAO tmpVAO;
	tmpVAO.vaoId = 0;	tmpVAO.vboId = 0;	tmpVAO.eboId = 0;	tmpVAO.nVertexs = 0;	tmpVAO.nIndices = 0;

	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	//std::vector <int>::size_type nn = normals.size();	// Tamany del vector normals en elements.
	//std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.
	//std::vector <int>::size_type nt = textures.size();	// Tamany del vector textures en elements.

// Create Vertex Array Object (VAO) for 3D Model Cube
	glGenVertexArrays(1, &tmpVAO.vaoId);

// Create vertex buffer objects for 3D Model attributes in the VAO
	glGenBuffers(1, &tmpVAO.vboId);

// Create vertex buffer objects for 3D Model EBO Elements attributes in the VAO
	glGenBuffers(1, &tmpVAO.eboId);

// Bind our Vertex Array Object as the current used object
	glBindVertexArray(tmpVAO.vaoId);

// Bind our Vertex Buffer Object as the current used object
	glBindBuffer(GL_ARRAY_BUFFER, tmpVAO.vboId);

	glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size() + colors.size()) * sizeof(double), 0, GL_STATIC_DRAW);

// Position Vertex attributes
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(double), &vertices[0]);	// Copy geometry data to VBO starting from 0 offset
	glEnableVertexAttribArray(0);											// Enable attribute index 0 as being used (position)
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

// Normal Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), normals.size() * sizeof(double), &normals[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(1);											// Enable attribute index 1 as being used (normals)
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (GLvoid*)(vertices.size() * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

// Texture Coordinates Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(double), textures.size() * sizeof(double), &textures[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(2);												// Enable attribute index 2 as being used (texture coordinates)
	glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (GLvoid*)((vertices.size() + normals.size()) * sizeof(double)));						// Specify that our color data is going into attribute index 0 and contains 3 double

// Color Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size()) * sizeof(double), colors.size() * sizeof(double), &colors[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(3);												// Enable attribute index 3 as being used
	glVertexAttribPointer(3, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)((vertices.size() + normals.size() + textures.size()) * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

// Bind our Element Buffer Object as the current used object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpVAO.eboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

//Unbind the registered EBO as current EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//Unbind the registered VBO as current VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

// Dissable Attrib arrays
	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(2);
	//glDisableVertexAttribArray(3);

// Unbind VAO, to prevent bugs
	glBindVertexArray(0);

	tmpVAO.nVertexs = (GLint) vertices.size() / 3;
	tmpVAO.nIndices = (GLint)indices.size();

	return tmpVAO;	// retorna CVAO.
}

void draw_TRIANGLES_EBO(GLint vaoList_indx)
{
	//GLuint vaoId = VAOList[vaoList_indx].vaoId;
	//int nvert = VAOList[vaoList_indx].nVertexs;

// Dibuixar EBO
	if (VAOList[vaoList_indx].vaoId != 0) 
		{	// Activar VAO
			glBindVertexArray(VAOList[vaoList_indx].vaoId); //glBindVertexArray(vaoId);
			
			// Activar EBO
			glBindVertexArray(VAOList[vaoList_indx].eboId);
			// Dibuixar EBO
			//glBindBuffer(GL_ARRAY_BUFFER, vboId);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAOList[vaoList_indx].eboId);
			glDrawElements(GL_TRIANGLES, VAOList[vaoList_indx].nIndices, GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}
}

// ---------- GL_LINES, GL_TRIANGLES, GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP -------------------------
// 
// ----- ACTIVACIÓ VERTEX ARRAY I DIBUIX DE GL_LINES PER A VAO
void draw_GL_LINES_VAO(std::vector <double> vertices, std::vector <double> colors)
{
// ----------------------- VAO, VBO
	GLuint vaoId = 0, vboId = 0;
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.

// Create Vertex Array Object (VAO) for 3D Model Cube
	glGenVertexArrays(1, &vaoId);

// Create vertex buffer objects for 3D Model attributes in the VAO
	glGenBuffers(1, &vboId);

// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vaoId);

// Bind our Vertex Buffer Object as the current used object
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glBufferData(GL_ARRAY_BUFFER, (vertices.size() + colors.size()) * sizeof(double), 0, GL_STATIC_DRAW);	// Allocate data to VBO starting from 0 offest

// Position Vertex attributes
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(double), &vertices[0]);	// Copy geometry data to VBO starting from 0 offest
	glEnableVertexAttribArray(0);											// Enable attribute index 0 as being used (position)
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

// Color Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), colors.size() * sizeof(double), &colors[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(1);										// Enable attribute index 1 as being used (normals)
	glVertexAttribPointer(1, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)(vertices.size() * sizeof(float)));	// Specify that our color data is going into attribute index 0 and contains 3 double

//Unbind the registered VBO as current VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArrays(GL_LINES, 0, (int) nv /3);

// Dissable Attrib arrays
	glDisableVertexAttribArray(0);		// Disable attribute index 0 as being used (Position)
	glDisableVertexAttribArray(1);		// Disable attribute index 1 as being used (Color)

// It is good idea to release VBOs with ID 0 after use.
// Once bound with 0, all pointers in gl*Pointer() behave as real
// pointer, so, normal vertex array operations are re-activated
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboId);

// Unbind and delete VAO
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoId);
}


// ----- ACTIVACIÓ VERTEX ARRAY I DIBUIX DE GL_TRIANGLES PER A VAO
void draw_GL_TRIANGLES_VAO(std::vector <double> vertices, std::vector <double> normals, std::vector <double> colors, std::vector <double> textures)
{
// ----------------------- VAO
	GLuint vaoId = 0, vboId = 0;
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	//std::vector <int>::size_type nn = normals.size();	// Tamany del vector normals en elements.
	//std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.
	//std::vector <int>::size_type nt = textures.size();	// Tamany del vector textures en elements.

// Create Vertex Array Object (VAO) for 3D Model Cube
	glGenVertexArrays(1, &vaoId);

// Create vertex buffer objects for 3D Model attributes in the VAO
	glGenBuffers(1, &vboId);

// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vaoId);

// Bind our Vertex Buffer Object as the current used object
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size() + colors.size()) * sizeof(double), 0, GL_STATIC_DRAW);

// Position Vertex attributes
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(double), &vertices[0]);	// Copy geometry data to VBO starting from 0 offest
	glEnableVertexAttribArray(0);											// Enable attribute index 0 as being used (position)
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

// Normal Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), normals.size() * sizeof(double), &normals[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(1);											// Enable attribute index 1 as being used (normals)
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (GLvoid*)(vertices.size() * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

// Texture Coordinates Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(double), textures.size() * sizeof(double), &textures[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(2);												// Enable attribute index 2 as being used (texture coordinates)
	glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (GLvoid*)((vertices.size() + normals.size()) * sizeof(double)));						// Specify that our color data is going into attribute index 0 and contains 3 double

// Color Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size()) * sizeof(double), colors.size() * sizeof(double), &colors[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(3);												// Enable attribute index 3 as being used
	glVertexAttribPointer(3, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)((vertices.size() + normals.size() + textures.size()) * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double
	
	glDrawArrays(GL_TRIANGLES, 0, (int) nv/3);

// Dissable Attrib arrays
	glDisableVertexAttribArray(0);		// Disable attribute index 0 as being used (Position)
	glDisableVertexAttribArray(1);		// Disable attribute index 1 as being used (Normal)
	glDisableVertexAttribArray(2);		// Disable attribute index 2 as being used (Texture Coordinates)
	glDisableVertexAttribArray(3);		// Disable attribute index 3 as being used (Color)

// It is good idea to release VBOs with ID 0 after use.
// Once bound with 0, all pointers in gl*Pointer() behave as real
// pointer, so, normal vertex array operations are re-activated
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboId);

	// Unbind and delete VAO
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoId);
}

// ----- ACTIVACIÓ VERTEX ARRAY I DIBUIX DE GL_TRIANGLE_FAN PER A VAO
void draw_GL_TRIANGLE_FAN_VAO(std::vector <double> vertices, std::vector <double> normals, std::vector <double> colors, std::vector <double> textures)
{
// ----------------------- VAO, VBO
	GLuint vaoId = 0, vboId = 0;
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	//std::vector <int>::size_type nn = normals.size();	// Tamany del vector normals en elements.
	//std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.
	//std::vector <int>::size_type nt = textures.size();	// Tamany del vector textures en elements.

// Create Vertex Array Object (VAO) for 3D Model Cube
	glGenVertexArrays(1, &vaoId);

// Create vertex buffer objects for 3D Model attributes in the VAO
	glGenBuffers(1, &vboId);

// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vaoId);

// Bind our Vertex Buffer Object as the current used object
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size() + colors.size()) * sizeof(double), 0, GL_STATIC_DRAW);

// Position Vertex attributes
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(double), &vertices[0]);	// Copy geometry data to VBO starting from 0 offest
	glEnableVertexAttribArray(0);											// Enable attribute index 0 as being used (position)
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

// Normal Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), normals.size() * sizeof(double), &normals[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(1);											// Enable attribute index 1 as being used (normals)
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (GLvoid*)(vertices.size() * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

// Texture Coordinates Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(double), textures.size() * sizeof(double), &textures[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(2);												// Enable attribute index 2 as being used (texture coordinates)
	glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (GLvoid*)((vertices.size() + normals.size()) * sizeof(double)));						// Specify that our color data is going into attribute index 0 and contains 3 double

// Color Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size()) * sizeof(double), colors.size() * sizeof(double), &colors[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(3);												// Enable attribute index 3 as being used
	glVertexAttribPointer(3, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)((vertices.size() + normals.size() + textures.size()) * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

// Draw Primitive
	glDrawArrays(GL_TRIANGLE_FAN, 0, (int) nv / 3);

// Dissable Attrib arrays
	glDisableVertexAttribArray(0);		// Disable attribute index 0 as being used (Position)
	glDisableVertexAttribArray(1);		// Disable attribute index 1 as being used (Normal)
	glDisableVertexAttribArray(2);		// Disable attribute index 2 as being used (Texture Coordinates)
	glDisableVertexAttribArray(3);		// Disable attribute index 3 as being used (Color)

// It is good idea to release VBOs with ID 0 after use.
// Once bound with 0, all pointers in gl*Pointer() behave as real
// pointer, so, normal vertex array operations are re-activated
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboId);

// Unbind and delete VAO
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoId);
}


// ----- ACTIVACIÓ VERTEX ARRAY I DIBUIX DE GL_TRIANGLE_STRIP PER A VAO
void draw_GL_TRIANGLE_STRIP_VAO(std::vector <double> vertices, std::vector <double> normals, std::vector <double> colors, std::vector <double> textures)
{
// ----------------------- VAO, VBO
	GLuint vaoId = 0, vboId = 0;
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	//std::vector <int>::size_type nn = normals.size();	// Tamany del vector normals en elements.
	//std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.
	//std::vector <int>::size_type nt = textures.size();	// Tamany del vector textures en elements.

// Create Vertex Array Object (VAO) for 3D Model Cube
	glGenVertexArrays(1, &vaoId);

// Create vertex buffer objects for 3D Model attributes in the VAO
	glGenBuffers(1, &vboId);

// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vaoId);

// Bind our Vertex Buffer Object as the current used object
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size() + colors.size()) * sizeof(double), 0, GL_STATIC_DRAW);

// Position Vertex attributes
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(double), &vertices[0]);	// Copy geometry data to VBO starting from 0 offest
	glEnableVertexAttribArray(0);											// Enable attribute index 0 as being used (position)
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

// Normal Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), normals.size() * sizeof(double), &normals[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(1);											// Enable attribute index 1 as being used (normals)
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), (GLvoid*)(vertices.size() * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

// Texture Coordinates Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(double), textures.size() * sizeof(double), &textures[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(2);												// Enable attribute index 2 as being used (texture coordinates)
	glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (GLvoid*)((vertices.size() + normals.size()) * sizeof(double)));						// Specify that our color data is going into attribute index 0 and contains 3 double

// Color Vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + textures.size()) * sizeof(double), colors.size() * sizeof(double), &colors[0]);	// Copy normal data to VBO starting from 0 offest
	glEnableVertexAttribArray(3);												// Enable attribute index 3 as being used
	glVertexAttribPointer(3, 4, GL_DOUBLE, GL_FALSE, 4 * sizeof(double), (GLvoid*)((vertices.size() + normals.size() + textures.size()) * sizeof(double)));	// Specify that our color data is going into attribute index 0 and contains 3 double

// Draw Primitive
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (int) nv / 3);

// Dissable Attrib arrays
	glDisableVertexAttribArray(0);		// Disable attribute index 0 as being used (Position)
	glDisableVertexAttribArray(1);		// Disable attribute index 1 as being used (Normal)
	glDisableVertexAttribArray(2);		// Disable attribute index 2 as being used (Texture Coordinates)
	glDisableVertexAttribArray(3);		// Disable attribute index 3 as being used (Color)

// It is good idea to release VBOs with ID 0 after use.
// Once bound with 0, all pointers in gl*Pointer() behave as real
// pointer, so, normal vertex array operations are re-activated
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboId);

// Unbind and delete VAO
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoId);
}

float round3dec(float val)
{
	float res = 0.0;

	res = round(val * 1000);
	return res / 1000.0;
}
/*** END OF FILE ***/
