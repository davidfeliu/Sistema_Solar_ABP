//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb interfície MFC, llibreries GLM i OpenGL 3.3+
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2022)
// escena.cpp : Aqui es on ha d'anar el codi de les funcions que 
//              dibuixin les escenes.
//
//    Versió 2.0:	- Objectes Cub, Esfera, Tetera (primitives libreria GLUT)
//
//	  Versió 2.2:	- Objectes Cub, Esfera, Tetera definides en fitxer font glut_geometry amb altres primitives GLUT
//
//	  Versió 2.5:	- Objectes cubRGB i Tie (nau Star Wars fet per alumnes)
//

#include "stdafx.h"
#include "material.h"
#include "visualitzacio.h"
#include "escena.h"
const int IMAGEMAX = 2048;

void sistemaSolar(GLint shaderId, glm::mat4 MatriuVista, glm::mat4 MatriuTG, bool sw_mat[4], SSolar SS)
{
	//structs para editar los objetos
	Planeta sol = SS.Sol;
	Planeta mercurio = SS.Mercurio;
	Planeta venus = SS.Venus;
	Planeta tierra = SS.Tierra;
	Planeta luna = SS.Luna;
	Planeta marte = SS.Marte;
	Planeta jupiter = SS.Jupiter;
	Planeta saturno = SS.Saturno;
	Planeta anilloSaturno = SS.AnilloSaturno;
	Planeta urano = SS.Urano;
	Planeta neptuno = SS.Neptuno;
	Planeta orbita = SS.Orbita;
	Planeta asteroides = SS.Asteroides;
	Planeta Kuiper = SS.Kuiper;

	//variables y cosas para crear y editar los objetos
	glm::mat4 TransMatrix(1.0), NormalMatrix(1.0), ModelMatrix(1.0);
	CColor col_object;

	col_object.r = 1.0;	col_object.g = 1.0;		col_object.b = 1.0;	 col_object.a = 1.0;
	SeleccionaColorMaterial(shaderId, col_object, sw_mat);
	

	//sol
	SetTextureParameters(sol.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);

	ModelMatrix = glm::scale(TransMatrix, vec3(sol.sx, sol.sy, sol.sz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	//mercurio
	SetTextureParameters(mercurio.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);

	TransMatrix = glm::translate(MatriuTG, vec3(mercurio.tx, mercurio.ty, mercurio.tz));
	ModelMatrix = glm::scale(TransMatrix, vec3(mercurio.sx, mercurio.sy, mercurio.sz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);

	SetTextureParameters(orbita.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);

	ModelMatrix = glm::scale(MatriuTG, vec3(mercurio.radi * 0.97855, mercurio.radi, 1.0));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_LINES);
	glBindTexture(GL_TEXTURE_2D, 0);


	//venus
	SetTextureParameters(venus.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);

	TransMatrix = glm::translate(MatriuTG, vec3(venus.tx, venus.ty, venus.tz));
	ModelMatrix = glm::scale(TransMatrix, vec3(venus.sx, venus.sy, venus.sz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	SetTextureParameters(orbita.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);
	ModelMatrix = glm::scale(MatriuTG, vec3(venus.radi * 0.99997, venus.radi, 1.0001));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_LINES);
	glBindTexture(GL_TEXTURE_2D, 0);

	//tierra
	SetTextureParameters(tierra.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);

	TransMatrix = glm::translate(MatriuTG, vec3(tierra.tx, tierra.ty, tierra.tz));
	TransMatrix = glm::scale(TransMatrix, vec3(tierra.sx, tierra.sy, tierra.sz));
	ModelMatrix = glm::rotate(TransMatrix, radians(180.f), vec3(tierra.rx, tierra.ry, tierra.rz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);

	SetTextureParameters(orbita.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);
	ModelMatrix = glm::scale(MatriuTG, vec3(tierra.radi * 0.99986, tierra.radi, 1.0));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_LINES);
	glBindTexture(GL_TEXTURE_2D, 0);

	//luna
	SetTextureParameters(luna.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0); // Activació del bloc 1 de textura.

	TransMatrix = glm::translate(MatriuTG, vec3(luna.tx, luna.ty, luna.tz));
	ModelMatrix = glm::scale(TransMatrix, vec3(luna.sx, luna.sy, luna.sz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);


	//marte
	SetTextureParameters(marte.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0); // Activació del bloc 1 de textura.

	TransMatrix = glm::translate(MatriuTG, vec3(marte.tx, marte.ty, marte.tz));
	ModelMatrix = glm::scale(TransMatrix, vec3(marte.sx, marte.sy, marte.sz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);

	SetTextureParameters(orbita.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);
	ModelMatrix = glm::scale(MatriuTG, vec3(marte.radi * 0.99566, marte.radi, 1.0));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_LINES);
	glBindTexture(GL_TEXTURE_2D, 0);

	//jupiter
	SetTextureParameters(jupiter.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0); // Activació del bloc 1 de textura.

	TransMatrix = glm::translate(MatriuTG, vec3(jupiter.tx, jupiter.ty, jupiter.tz));
	ModelMatrix = glm::scale(TransMatrix, vec3(jupiter.sx, jupiter.sy, jupiter.sz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);

	SetTextureParameters(orbita.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);
	ModelMatrix = glm::scale(MatriuTG, vec3(jupiter.radi * 0.99880, jupiter.radi, 1.0));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_LINES);
	glBindTexture(GL_TEXTURE_2D, 0);

	//saturno
	SetTextureParameters(saturno.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0); // Activació del bloc 1 de textura.

	TransMatrix = glm::translate(MatriuTG, vec3(saturno.tx, saturno.ty, saturno.tz));
	ModelMatrix = glm::scale(TransMatrix, vec3(saturno.sx, saturno.sy, saturno.sz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);

	SetTextureParameters(orbita.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);
	ModelMatrix = glm::scale(MatriuTG, vec3(saturno.radi * 0.99840, saturno.radi, 1.0));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_LINES);
	glBindTexture(GL_TEXTURE_2D, 0);

	//anilloSaturno, rotado  ->>> probar torus
	SetTextureParameters(anilloSaturno.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0); // Activació del bloc 1 de textura.

	TransMatrix = glm::translate(MatriuTG, vec3(anilloSaturno.tx, anilloSaturno.ty, anilloSaturno.tz));
	TransMatrix = glm::scale(TransMatrix, vec3(anilloSaturno.sx*2, anilloSaturno.sy*3, anilloSaturno.sz));
	ModelMatrix = glm::rotate(TransMatrix, radians(30.f), vec3(anilloSaturno.rx, anilloSaturno.ry, anilloSaturno.rz));
	// Pas ModelView Matrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_DISK);
	glBindTexture(GL_TEXTURE_2D, 0);


	//urano
	SetTextureParameters(urano.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0); // Activació del bloc 1 de textura.

	TransMatrix = glm::translate(MatriuTG, vec3(urano.tx, urano.ty, urano.tz));
	ModelMatrix = glm::scale(TransMatrix, vec3(urano.sx, urano.sy, urano.sz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);

	SetTextureParameters(orbita.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);
	ModelMatrix = glm::scale(MatriuTG, vec3(urano.radi * 0.99892, urano.radi, 1.0));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_LINES);
	glBindTexture(GL_TEXTURE_2D, 0);

	//neptuno
	SetTextureParameters(neptuno.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0); // Activació del bloc 1 de textura.

	TransMatrix = glm::translate(MatriuTG, vec3(neptuno.tx, neptuno.ty, neptuno.tz));
	ModelMatrix = glm::scale(TransMatrix, vec3(neptuno.sx, neptuno.sy, neptuno.sz));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE);
	glBindTexture(GL_TEXTURE_2D, 0);

	SetTextureParameters(orbita.textura, true, true, true, false);
	glActiveTexture(GL_TEXTURE0);
	ModelMatrix = glm::scale(MatriuTG, vec3(neptuno.radi * 0.99995 - 0.0001, neptuno.radi, 1.0));
	// Pas ModelView Matrix a shade
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_LINES);
	glBindTexture(GL_TEXTURE_2D, 0);

	//asteroides
	for (int i = 0; i < 9; i++)
	{
		float radianes;
		if (i % 2 == 0)
			radianes = 0;
		else if (i % 3 == 0)
			radianes = 2.5;
		else
			radianes = 4;

		for (int j = 0; j < (50 + i * 4); j++)
		{
			SetTextureParameters(asteroides.textura, true, true, true, false);
			glActiveTexture(GL_TEXTURE0); // Activació del bloc 1 de textura.

			TransMatrix = glm::translate(MatriuTG, vec3(float(sin(radianes)) * (asteroides.radi + i * 2.5), float(cos(radianes)) * (asteroides.radi + i * 2.5), asteroides.tz));

			if (j == 25 && i == 4)
				ModelMatrix = glm::scale(TransMatrix, vec3(asteroides.sx * 7, asteroides.sy * 7, asteroides.sz * 7));
			else if (j % 5 == 0)
				ModelMatrix = glm::scale(TransMatrix, vec3(asteroides.sx * 1.5, asteroides.sy * 1.5, asteroides.sz * 1.5));
			else if (j % 2 == 0)
				ModelMatrix = glm::scale(TransMatrix, vec3(asteroides.sx, asteroides.sy, asteroides.sz));
			else if (j % 3 == 0)
				ModelMatrix = glm::scale(TransMatrix, vec3(asteroides.sx * 0.7, asteroides.sy * 0.7, asteroides.sz * 0.7));
			else if (j % 7 == 0 && i % 2 == 0)
				ModelMatrix = glm::scale(TransMatrix, vec3(asteroides.sx * 2, asteroides.sy, asteroides.sz));
			else
				ModelMatrix = glm::scale(TransMatrix, vec3(asteroides.sx, asteroides.sy, asteroides.sz));

			// Pas ModelView Matrix a shade
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
			NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
			// Pas NormalMatrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
			draw_TriEBO_Object(GLUT_DODECAHEDRON);
			glBindTexture(GL_TEXTURE_2D, 0);

			radianes = radianes + 3;
			if (radianes > 360)
				radianes = 0;
		}
	}

	//cinturon de Kuiper
	for (int i = 0; i < 25; i++)
	{
		float radianes;
		if (i % 2 == 0)
			radianes = 0;
		else if (i % 3 == 0)
			radianes = 2.5;
		else
			radianes = 4;

		for (int j = 0; j < (110 + i * 2); j++)
		{
			SetTextureParameters(Kuiper.textura, true, true, true, false);
			glActiveTexture(GL_TEXTURE0); // Activació del bloc 1 de textura.

			TransMatrix = glm::translate(MatriuTG, vec3(float(sin(radianes)) * (Kuiper.radi + i * 2.5), float(cos(radianes)) * (Kuiper.radi + i * 2.5), Kuiper.tz));

			if (j == 25 && i == 4)
				ModelMatrix = glm::scale(TransMatrix, vec3(Kuiper.sx * 7, Kuiper.sy * 7, Kuiper.sz * 7));
			else if (j % 5 == 0)
				ModelMatrix = glm::scale(TransMatrix, vec3(Kuiper.sx * 1.5, Kuiper.sy * 1.5, Kuiper.sz * 1.5));
			else if (j % 2 == 0)
				ModelMatrix = glm::scale(TransMatrix, vec3(Kuiper.sx, Kuiper.sy, Kuiper.sz));
			else if (j % 3 == 0)
				ModelMatrix = glm::scale(TransMatrix, vec3(Kuiper.sx * 0.7, Kuiper.sy * 0.7, Kuiper.sz * 0.7));
			else if (j % 7 == 0 && i % 2 == 0)
				ModelMatrix = glm::scale(TransMatrix, vec3(Kuiper.sx * 2, Kuiper.sy, Kuiper.sz));
			else
				ModelMatrix = glm::scale(TransMatrix, vec3(Kuiper.sx, Kuiper.sy, Kuiper.sz));

			// Pas ModelView Matrix a shade
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
			NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
			// Pas NormalMatrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
			draw_TriEBO_Object(GLUT_DODECAHEDRON);
			glBindTexture(GL_TEXTURE_2D, 0);

			radianes = radianes + 1.2;
			if (radianes > 360)
				radianes = 0;
		}
	}
}

// Dibuixa Eixos Coordenades Món i Reixes, activant un shader propi.
void dibuixa_Eixos(GLuint ax_programID, bool eix, GLuint axis_Id, CMask3D reixa, CPunt3D hreixa, 
	glm::mat4 MatriuProjeccio, glm::mat4 MatriuVista)
{
// Visualització Eixos Coordenades Mòn
	glUseProgram(ax_programID);

// Pas Matrius Projecció i Vista Vista a shader
	glUniformMatrix4fv(glGetUniformLocation(ax_programID, "projectionMatrix"), 1, GL_FALSE, &MatriuProjeccio[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(ax_programID, "viewMatrix"), 1, GL_FALSE, &MatriuVista[0][0]);

// Attribute Locations must be setup before calling glLinkProgram()
	glBindAttribLocation(ax_programID, 0, "in_Vertex"); // Vèrtexs
	glBindAttribLocation(ax_programID, 1, "in_Color");	// Color

//  Dibuix dels eixos
	if (eix) draw_Eixos(axis_Id);

// Dibuixa el grid actiu
	if ((reixa.x) || (reixa.y) || (reixa.z) || (reixa.w)) draw_Grid(reixa, hreixa);
}

// Dibuixa Skybox en forma de Cub, activant un shader propi.
void dibuixa_Skybox(GLuint sk_programID, GLuint cmTexture, char eix_Polar, glm::mat4 MatriuProjeccio, glm::mat4 MatriuVista)
{
	glm::mat4 ModelMatrix(1.0);

	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

// Activació shader per a cub skybox
	glUseProgram(sk_programID);

// Pas Matrius Projecció i Vista a shader
	glUniformMatrix4fv(glGetUniformLocation(sk_programID, "projectionMatrix"), 1, GL_FALSE, &MatriuProjeccio[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(sk_programID, "viewMatrix"), 1, GL_FALSE, &MatriuVista[0][0]);

// Rotar skyBox per a orientar sobre eix superior Z o X en Vista Esfèrica (POLARX, POLARY, POLARZ)
	if (eix_Polar == POLARZ) ModelMatrix = glm::rotate(ModelMatrix, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
	else if (eix_Polar == POLARX) ModelMatrix = glm::rotate(ModelMatrix, radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));

// Escalar Cub Skybox a 5000 per encabir objectes escena a l'interior
	ModelMatrix = glm::scale(ModelMatrix, vec3(5000.0f, 5000.0f, 5000.0f));		//glScaled(5000.0, 5000.0, 5000.0);
	glUniformMatrix4fv(glGetUniformLocation(sk_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);

// Activar textura cubemaps del Skybox per encabir objectes escena
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cmTexture);

// Attribute Locations must be setup before calling glLinkProgram()
	glBindAttribLocation(sk_programID, 0, "in_Vertex"); // Vèrtexs

//  Dibuix del Skybox
	drawCubeSkybox();

	glDepthFunc(GL_LESS); // set depth function back to default
}


// dibuixa_EscenaGL: Dibuix de l'escena amb comandes GL
void dibuixa_EscenaGL(GLuint sh_programID, bool eix, GLuint axis_Id, CMask3D reixa, CPunt3D hreixa, char objecte, 
			CColor col_object, bool sw_mat[5],
			bool textur, GLint texturID[NUM_MAX_TEXTURES], bool textur_map, bool flagInvertY,
			int nptsU, CPunt3D PC_u[MAX_PATCH_CORBA], GLfloat pasCS, bool sw_PC, bool dib_TFrenet,
			GLuint vaoList_3DS, GLuint vaoList_OBJ,
			glm::mat4 MatriuVista, glm::mat4 MatriuTG, SSolar SS)
{
	float altfar = 0;
	GLint npunts = 0, nvertexs = 0;
	int i, j, k;
	GLdouble tras[3]; //Sierpinski Sponge
	CColor color_vermell, color_Mar;
	bool sw_material[5];
	
// Matrius de Transformació
	glm::mat4 NormalMatrix(1.0), ModelMatrix(1.0), TransMatrix(1.0), ScaleMatrix(1.0), RotMatrix(1.0);

// VAO
	CVAO objectVAO;
	objectVAO.vaoId = 0;	objectVAO.vboId = 0;	objectVAO.nVertexs = 0;

	tras[0] = 0.0;	tras[1] = 0.0; tras[2] = 0.0;
	color_vermell.r = 1.0;	color_vermell.g = 0.0; color_vermell.b = 0.0; color_vermell.a = 1.0;
	sw_material[0] = false;	sw_material[1] = true; sw_material[2] = true; sw_material[3] = false;	sw_material[4] = true;

// Shader Visualització Objectes
	glUseProgram(sh_programID);

// Parametrització i activació/desactivació de textures
	if (texturID[0] != -1) SetTextureParameters(texturID[0], true, true, textur_map, false);
	if (textur) {	glUniform1i(glGetUniformLocation(sh_programID, "textur"), GL_TRUE); //glEnable(GL_TEXTURE_2D);
					glUniform1i(glGetUniformLocation(sh_programID, "modulate"), GL_TRUE); //glEnable(GL_MODULATE);
				}
		else {	glUniform1i(glGetUniformLocation(sh_programID, "textur"), GL_FALSE); //glDisable(GL_TEXTURE_2D);
				glUniform1i(glGetUniformLocation(sh_programID, "modulate"), GL_FALSE); //glDisable(GL_MODULATE);
			}
	glUniform1i(glGetUniformLocation(sh_programID, "flag_invert_y"), flagInvertY);

// Attribute Locations must be setup before calling glLinkProgram()
	glBindAttribLocation(sh_programID, 0, "in_Vertex");		// Vèrtexs
	glBindAttribLocation(sh_programID, 1, "in_Normal");		// Normals
	glBindAttribLocation(sh_programID, 2, "in_TexCoord");	// Textura
	glBindAttribLocation(sh_programID, 3, "in_Color");		// Color

// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	SeleccionaColorMaterial(sh_programID, col_object, sw_mat);

	switch (objecte)
	{

// Arc
	case SISTEMASOLAR:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material pel color de l'objecte.
		SeleccionaColorMaterial(sh_programID, col_object, sw_mat);

		sistemaSolar(sh_programID, MatriuVista, MatriuTG, sw_mat, SS);
		break;

	case ARC:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material pel color de l'objecte.
		SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
		arc(sh_programID, MatriuVista, MatriuTG, sw_mat);

		// Dibuix geometria Mar
		color_Mar.r = 0.5;	color_Mar.g = 0.4; color_Mar.b = 0.9; color_Mar.a = 1.0;
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material pel color de l'objecte.
		SeleccionaColorMaterial(sh_programID, color_Mar, sw_mat);
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		// Pas NormalMatrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_TriVAO_Object(MAR_FRACTAL_VAO);
		break;

// Dibuix de l'objecte TIE (Nau enemiga Star Wars)
	case TIE:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material pel color de l'objecte.
		SeleccionaColorMaterial(sh_programID, col_object, sw_mat);

		tie(sh_programID, MatriuVista, MatriuTG, sw_mat);
		break;

// Dibuix de l'objecte 3DS
	case OBJ3DS:
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		// Pas NormalMatrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		// Dibuix de l'objecte 3DS amb textures 
		draw_TRIANGLES_VAO(vaoList_3DS);		// Dibuixar VAO a pantalla
		break;

// Dibuix de l'objecte OBJ
	case OBJOBJ:
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		// Pas NormalMatrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		// Objecte OBJ: Dibuix de l'objecte OBJ amb textures
		draw_TRIANGLES_VAO(vaoList_OBJ);		// Dibuixar VAO a pantalla
		break;

// Corba Bezier
	case C_BEZIER:
		// Dibuixar Punts de Control
		if (sw_PC)
		{	// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material pel color vermell.
			SeleccionaColorMaterial(sh_programID, color_vermell, sw_material);

			for (int i = 0; i < nptsU; i++)
			{	//glPushMatrix();
				  //glTranslatef(PC_u[i].x, PC_u[i].y, PC_u[i].z);
				  ModelMatrix = glm::translate(MatriuTG, vec3(PC_u[i].x, PC_u[i].y, PC_u[i].z));
				  // Pas ModelViewMatrix a shader
				  glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
				  // Pas NormalMatrix a shader
				  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
				  glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
				  draw_TriEBO_Object(GLU_SPHERE); //drawgluSphere(npunts);	//gluSphere(5.0, 20, 20);
				//glPopMatrix();
			}

		}
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material pel color de l'objecte.
		SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
		ModelMatrix = MatriuTG;
		// Pas ModelViewMatrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		// Pas NormalMatrix a shader
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_LinEBO_Object(CRV_BEZIER); //draw_LinVAO_Object(CRV_BEZIER); //draw_Bezier_Curve(nptsU, PC_u, pasCS, false);
		if (dib_TFrenet) draw_TFBezier_Curve(sh_programID, PC_u, nptsU, pasCS, false); // Dibuixar Triedre de Frenet
		break;

// Corba Lemniscata
	case C_LEMNISCATA:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material pel color de l'objecte.
		SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
		ModelMatrix = MatriuTG;
		// Pas ModelViewMatrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		// Pas NormalMatrix a shader
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_LinEBO_Object(CRV_LEMNISCATA3D); // draw_LinVAO_Object(CRV_LEMNISCATA3D); //draw_Lemniscata3D(800, pasCS*20.0);
		if (dib_TFrenet) draw_TFLemniscata3D(sh_programID, 800, pasCS * 20.0);
		break;

// Corba Spline
	case C_BSPLINE:
		// Dibuixar Punts de Control
		if (sw_PC)
		{	// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material pel color vermell.
			SeleccionaColorMaterial(sh_programID, color_vermell, sw_material);
			//objectVAO = loadgluSphere_VAO(5.0, 20, 20);
			for (int i = 0; i < nptsU; i++)
			{	//glPushMatrix();
				  //glTranslatef(PC_u[i].x, PC_u[i].y, PC_u[i].z);
				ModelMatrix = glm::translate(MatriuTG, vec3(PC_u[i].x, PC_u[i].y, PC_u[i].z));
				// Pas ModelViewMatrix a shader
				glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
				// Pas NormalMatrix a shader
				NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
				glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
				draw_TriEBO_Object(GLU_SPHERE); // drawgluSphere(npunts);	//gluSphere(5.0, 20, 20);
				//glPopMatrix();
			}

		}
		SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
		ModelMatrix = MatriuTG;
		// Pas ModelViewMatrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		// Pas NormalMatrix a shader
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_LinEBO_Object(CRV_BSPLINE); //draw_LinVAO_Object(CRV_BSPLINE); //draw_BSpline_Curve(nptsU, PC_u, pasCS);
		if (dib_TFrenet) draw_TFBSpline_Curve(sh_programID, PC_u, nptsU, pasCS); // Dibuixar Triedre de Frenet
		break;

		// Matriu de Primitives SENSE pre-compilació prèvia en VBO (precompilació, draw i alliberació VBO en una funció)
	case MATRIUP:
		SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
				for (k = 0; k < 10; k++)
				{	//glPushMatrix();
					  //glTranslated(i * 15.0, j * 15.0, k * 15.0);
					  //glScaled(5.0, 5.0, 5.0);
					TransMatrix = glm::translate(MatriuTG, vec3(i * 15.0f, j * 15.0f, k * 15.0));
					ModelMatrix = glm::scale(TransMatrix, vec3(5.0f, 5.0f, 5.0f));
					// Pas ModelView Matrix a shader
					glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
					// Pas NormalMatrix a shader
					NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
					glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
					glutSolidCube(1.0);
					//glutSolidTorus(1.0,2.0,20,20);
					//glutSolidOctahedron();
					//glutSolidTetrahedron();
					//glutSolidIcosahedron();
					//glutSolidRhombicDodecahedron();
					//glutSolidSierpinskiSponge(3,tras,1.0);
					//glutSolidTeapot(1.0);
					//gluCylinder(0.5,0.5,1.0,20,20);
					//gluSphere(1.0, 20, 20);
					//glPopMatrix();
				}
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
			{	//glPushMatrix();
				  //glTranslated(i * 15.0, j * 15.0, 160.0);
				  //glTranslated(i * 15.0, j * 15.0, 160.0);
				ModelMatrix = glm::translate(MatriuTG, vec3(i * 15.0f, j * 15.0f, 160.0f));
				// Pas ModelView Matrix a shader
				glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
				// Pas NormalMatrix a shader
				NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
				glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
				  //glScaled(5.0, 5.0, 5.0);
				  //glutSolidCube(1.0);
				glutSolidTorus(2.0,3.0,20,20);
				//glutSolidOctahedron();
				//glutSolidTetrahedron();
				//glutSolidIcosahedron();
				//glutSolidRhombicDodecahedron();
				//glutSolidSierpinskiSponge(3,tras,1.0);
				//glutSolidTeapot(1.0);
				//gluCylinder(0.5,0.5,1.0,20,20);
				//gluSphere(1.0, 20, 20);
				//glPopMatrix();
			}
// Dibuix una esfera
		//glPushMatrix();
		  //glTranslated(200.0, 200.0, 200.0);
		  //glScaled(5.0, 5.0, 5.0);
		TransMatrix = glm::translate(MatriuTG, vec3(200.0f, 200.0f, 200.0f));
		ModelMatrix = glm::scale(TransMatrix, vec3(5.0f, 5.0f, 5.0f));
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		// Pas NormalMatrix a shader
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		gluSphere(1.0, 20, 20);
		//glPopMatrix();
		break;

// Matriu de Primitives AMB pre-compilació prèvia en VBO 
//	(separació en 3 funcions: *_VBO() per precompilació, draw*() per dibuix i deleteVBO() per alliberar VBO)
	case MATRIUP_VAO:
		SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
		//glutSolidCube_VAO(1.0);		// Càrrega de la geometria del Cub al VAO

		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
				for (k = 0; k < 10; k++)
				{	//glPushMatrix();
					  //glTranslated(i * 15.0, j * 15.0, k * 15.0);
					  //glScaled(5.0, 5.0, 5.0);
					  TransMatrix = glm::translate(MatriuTG, vec3(i * 15.0f, j * 15.0f, k * 15.0f));
					  ModelMatrix = glm::scale(TransMatrix, vec3(5.0f, 5.0f, 5.0f));
					  // Pas ModelView Matrix a shader
					  glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
					  // Pas NormalMatrix a shader
					  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
					  glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
					  
					  draw_TriEBO_Object(GLUT_CUBE); //drawSolidCube();	// Dibuix del cub dins VBO.

				}
		//deleteVAO(GLUT_CUBE);	// Eliminació del VAO cub.

		//nvertexs = glutSolidTorus_VAO(2.0, 3.0, 20, 20);	// Càrrega de la geometria del Torus al VAO
		for (i = 0; i < 10; i++)
			for (j = 0; j < 10; j++)
			{	//glPushMatrix();
				  //glTranslated(i * 15.0, j * 15.0, 160.0);
				  //glTranslated(i * 15.0, j * 15.0, 160.0);
				ModelMatrix = glm::translate(MatriuTG, vec3(i * 15.0f, j * 15.0f, 160.0f));
				// Pas ModelView Matrix a shader
				glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
				// Pas NormalMatrix a shader
				NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
				glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
				draw_TriEBO_Object(GLUT_TORUS); //drawSolidTorus(nvertexs);
				//glPopMatrix();

			}
		//deleteVAO(GLUT_CUBE);		// Eliminació del VAO cub.
		//deleteVAO(GLUT_TORUS);    // Eliminació del VAO Torus.
// Dibuix una esfera
		//glPushMatrix();
		  //glTranslated(200.0, 200.0, 200.0);
		  //glScaled(5.0, 5.0, 5.0);
		  TransMatrix = glm::translate(MatriuTG, vec3(200.0f, 200.0f, 200.0f));
		  ModelMatrix = glm::scale(TransMatrix, vec3(5.0f, 5.0f, 5.0f));
		  // Pas ModelView Matrix a shader
		  glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		  // Pas NormalMatrix a shader
		  glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		  draw_TriEBO_Object(GLU_SPHERE); //gluSphere(1.0, 20, 20);
		//glPopMatrix();
		break;

// Dibuix de la resta d'objectes
	default:
		// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
		dibuixa(sh_programID, objecte,MatriuVista, MatriuTG);
		break;
	}

// Enviar les comandes gràfiques a pantalla
//	glFlush();
}


// dibuixa: Funció que dibuixa objectes simples de la llibreria GLUT segons obj
void dibuixa(GLuint sh_programID, char obj, glm::mat4 MatriuVista, glm::mat4 MatriuTG)
{
//	std::string String;
//	const char* string;
	glm::mat4 NormalMatrix(1.0), ModelMatrix(1.0); // TransMatrix(1.0); // ScaleMatrix(1.0), RotMatrix(1.0);
	GLdouble tras[3]; //Sierpinski Sponge
	
	tras[0] = 0.0;	tras[1] = 0.0; tras[2] = 0.0;

	switch(obj)
	{

// Cub
	case CUB:
		//glPushMatrix();
		  //glScaled(5.0,5.0,5.0);
		ModelMatrix = glm::scale(MatriuTG, vec3(5.0f, 5.0f, 5.0f));
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		// Pas NormalMatrix a shader
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_TriEBO_Object(GLUT_CUBE);	//draw_TriVAO_Object(GLUT_CUBE);  //glutSolidCube(1.0);
		//glPopMatrix();

		break;

	case CUB_RGB:
		//glPushMatrix();
		  //glScaled(5.0,5.0,5.0);
		ModelMatrix = glm::scale(MatriuTG, vec3(5.0f, 5.0f, 5.0f));
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		// Pas NormalMatrix a shader
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_TriEBO_Object(GLUT_CUBE_RGB);	//draw_TriVAO_Object(GLUT_CUBE_RGB); // glutSolidCubeRGB(1.0);
		//glPopMatrix();
		break;

// Esfera
	case ESFERA:	
		//glPushMatrix();
		  //glScaled(5.0,5.0,5.0);
		ModelMatrix = glm::scale(MatriuTG, vec3(5.0f, 5.0f, 5.0f));
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		// Pas NormalMatrix a shader
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_TriEBO_Object(GLU_SPHERE); //draw_TriVAO_Object(GLU_SPHERE); //gluSphere(1.0, 40, 40);
		//glPopMatrix();
		break;

// Tetera
	case TETERA:
		//glPushMatrix();
		  //glScaled(5.0,5.0,5.0);
		  ModelMatrix = MatriuTG;
		  // Pas ModelView Matrix a shader
		  glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		  // Pas NormalMatrix a shader
		  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		  glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		  draw_TriVAO_Object(GLUT_TEAPOT); // glutSolidTeapot(1.0);
		//glPopMatrix();
		break;
	}
}

// OBJECTE ARC
void arc(GLuint sh_programID, glm::mat4 MatriuVista, glm::mat4 MatriuTG, bool sw_mat[5])
{	CColor col_object;
	glm::mat4 NormalMatrix(1.0), ModelMatrix(1.0);

// Definir VAO del Cub
	//glutSolidCube_VAO(1.0);

// Pota Esquerra
	//glPushMatrix();
	  //glTranslated(0.0, 0.0, 2.5);
	  //glScaled(1.0, 1.0, 5.0);
	ModelMatrix = glm::translate(MatriuTG, vec3(0.0f, 0.0f, 2.5f));
	ModelMatrix = glm::scale(ModelMatrix, vec3(1.0f, 1.0f, 5.0f));
	// Pas ModelView Matrix a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	// Pas NormalMatrix a shader
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_CUBE); // drawSolidCube();	//  glutSolidCube(1.0);
	//glPopMatrix();

// Pota Dreta
	//glPushMatrix();
	//glTranslated(0.0, 5.0, 2.5);
	//glScaled(1.0, 1.0, 5.0);
	ModelMatrix = glm::translate(MatriuTG, vec3(0.0f, 5.0f, 2.5f));
	ModelMatrix = glm::scale(ModelMatrix, vec3(1.0f, 1.0f, 5.0f));
	// Pas ModelView Matrix a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	// Pas NormalMatrix a shader
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_CUBE); //drawSolidCube();	//  glutSolidCube(1.0);
	//glPopMatrix();

// Travesser
	//glPushMatrix();
	  //glTranslated(0.0, 2.5, 5.5);
	  //glRotated(90.0, 1.0, 0.0, 0.0);
	  //glScaled(1.0, 1.0, 6.0);
	ModelMatrix = glm::translate(MatriuTG, vec3(0.0f, 2.5f, 5.5f));
	ModelMatrix = glm::rotate(ModelMatrix, radians(90.f),vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, vec3(1.0f, 1.0f, 6.0f));
	// Pas ModelView Matrix a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	// Pas NormalMatrix a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLUT_CUBE);  //drawSolidCube();	//  glutSolidCube(1.0);
	//glPopMatrix();

// Eliminar VAO del Cub
	//deleteVAO(GLUT_CUBE);

// Esfera
	col_object.r = 0.0;		col_object.g = 1.0;		col_object.b = 1.0;		col_object.a = 1.0;	// Color blau clar
	SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
	//glPushMatrix();
	  //glTranslated(0.0, 5.0, 6.5);
    ModelMatrix = glm::translate(MatriuTG, vec3(0.0f, 5.0f, 6.5f));
	// Pas ModelView Matrix a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	// Pas NormalMatrix a shader
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriEBO_Object(GLU_SPHERE); //gluSphere(0.5, 20, 20);
	//glPopMatrix();

// Tetera
	col_object.r = 1.0;		col_object.g = 1.0;		col_object.b = 0.0;		col_object.a = 1.0;	// Color groc
	SeleccionaColorMaterial(sh_programID, col_object, sw_mat);
	//glPushMatrix();
	  //glTranslated(0.0, 0.0, 6.0);
	  //glRotated(-90.0, 0.0, 0.0, 1.0);
	  //glRotated(90.0, 1.0, 0.0, 0.0);
	  //glScaled(0.25, 0.25, 0.25);
	ModelMatrix = glm::translate(MatriuTG, vec3(0.0f, 0.0f, 6.0f));
	ModelMatrix = glm::rotate(ModelMatrix, radians(-90.f), vec3(0.0f, 0.0f, 1.0f));
	ModelMatrix = glm::rotate(ModelMatrix, radians(90.f), vec3(1.0f, 0.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, vec3(0.25f, 0.25f, 0.25f));
	// Pas ModelView Matrix a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	// Pas NormalMatrix a shader
	NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	draw_TriVAO_Object(GLUT_TEAPOT); //glutSolidTeapot(1.0);
	//glPopMatrix();
}


// Mar amb ondulacions
CVAO loadSea_VAO(CColor colorM)
{
	int i, j, step, it1, it2;
	double angle, delay, Nx;
	glm::mat4 ModelMatrix(1.0), NormalMatrix(1.0);

// VAO
	GLuint vaoId = 0; GLuint vboId = 0;
	CVAO seaVAO;
	seaVAO.vaoId = 0;	seaVAO.vboId = 0;	seaVAO.nVertexs = 0;
	std::vector <double> vertices, normals, colors, textures;	// Definició vectors dinàmics per a vertexs, normals i textures 
	vertices.resize(0);		normals.resize(0);		colors.resize(0);	textures.resize(0);			// Reinicialitzar vectors

// Aigua amb ondulacions simulades a partir de normals sinusoidals
	step = 10; //step = 5;
	delay = 0;
	it1 = 0;

//   SeleccionaMaterial(MAT_DEFAULT);	
	float h = 2 * PI*step / 500;
	for (j = -250; j<250 - step; j = j + step)
	{	delay = 1.0*h*it1;

		it2 = 0;
		//glColor3f(0.5f, 0.4f, 0.9f);
		for (i = -250; i<250 - step; i = i + step)
		{	//glBegin(GL_POLYGON);
			  angle = 1.0*it2*h * 15;
			  Nx = -cos(angle);
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i, j, 0);			// Vèrtex P1
			  colors.push_back(colorM.r);		colors.push_back(colorM.g);		colors.push_back(colorM.b);		colors.push_back(colorM.a);  // Vector Colors
			  normals.push_back(Nx);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(0.0);			textures.push_back(0.0);										// Vector Textures
			  vertices.push_back(i);			vertices.push_back(j);			vertices.push_back(0.0);		// Vector Vertices

			  angle = 1.0*(it2 + 1.0)*h * 15;
			  Nx = -cos(angle);
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i + step, j, 0);		// Vèrtex P2
			  colors.push_back(colorM.r);		colors.push_back(colorM.g);		colors.push_back(colorM.b);		colors.push_back(colorM.a);  // Vector Colors
			  normals.push_back(Nx);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(1.0);			textures.push_back(0.0);										// Vector Textures
			  vertices.push_back(i+step);		vertices.push_back(j);			vertices.push_back(0.0);		// Vector Vertices

			  angle = 1.0*(it2 + 1.0)*h * 15;
			  Nx = -cos(angle);
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i + step, j + step, 0);// Vèrtex P3
			  colors.push_back(colorM.r);		colors.push_back(colorM.g);		colors.push_back(colorM.b);		colors.push_back(colorM.a);  // Vector Colors
			  normals.push_back(Nx);			normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(1.0);			textures.push_back(1.0);										// Vector Textures
			  vertices.push_back(i+step);		vertices.push_back(j+step);		vertices.push_back(0.0);		// Vector Vertices

			//glEnd();
// ----------------------- VAO
			//std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
			//draw_GL_TRIANGLES_VAO(vertices, normals, colors, textures);
			//vertices.resize(0);	normals.resize(0);	colors.resize(0);	textures.resize(0);	// Reinicialitzar vectors

			//glBegin(GL_POLYGON);
			  angle = 1.0*it2*h * 15;
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i, j, 0);			// Vèrtex P1
			  colors.push_back(colorM.r);		colors.push_back(colorM.g);		colors.push_back(colorM.b);		colors.push_back(colorM.a);  // Vector Colors
			  normals.push_back(-cos(angle));	normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(1.0);			textures.push_back(1.0);										// Vector Textures
			  vertices.push_back(i);			vertices.push_back(j);			vertices.push_back(0.0);		// Vector Vertices

			  angle = 1.0*(it2 + 1)*h * 15;
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i + step, j + step, 0);// Vèrtex P2
			  colors.push_back(colorM.r);		colors.push_back(colorM.g);		colors.push_back(colorM.b);		colors.push_back(colorM.a);  // Vector Colors
			  normals.push_back(-cos(angle));	normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(1.0);			textures.push_back(1.0);										// Vector Textures
			  vertices.push_back(i + step);		vertices.push_back(j + step);	vertices.push_back(0.0);		// Vector Vertices

			  angle = 1.0*it2*h * 15;
			  //glNormal3f(-cos(angle), 0, 1);
			  //glVertex3f(i, j + step, 0);		// Vèrtex P3
			  colors.push_back(colorM.r);		colors.push_back(colorM.g);		colors.push_back(colorM.b);		colors.push_back(colorM.a);  // Vector Colors
			  normals.push_back(-cos(angle));	normals.push_back(0.0);			normals.push_back(1.0);			// Vector Normals
			  textures.push_back(0.0);			textures.push_back(1.0);										// Vector Textures
			  vertices.push_back(i);			vertices.push_back(j + step);		vertices.push_back(0.0);	// Vector Vertices

			//glEnd();

			it2++;
		}
		it1++;
	}
// ----------------------- VAO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	//draw_GL_TRIANGLES_VAO(vertices, normals, colors, textures);
	// 
// Creació d'un VAO i un VBO i càrrega de la geometria. Guardar identificador VAO identificador VBO a struct CVAO.
	seaVAO = load_TRIANGLES_VAO(vertices, normals, colors, textures);
	//seaVAO.vaoId = vaoId;
	//seaVAO.vboId = vboId;

	seaVAO.nVertexs = (int)nv / 3;
	return seaVAO;
}


// OBJECTE TIE: FETS PER ALUMNES -----------------------------------------------------------------

// Objecte TIE

void tie(GLint shaderId, glm::mat4 MatriuVista, glm::mat4 MatriuTG, bool sw_mat[4])
{	Motor(shaderId,MatriuVista,MatriuTG, sw_mat);
	Alas(shaderId, MatriuVista, MatriuTG, sw_mat);
	Canon(shaderId, MatriuVista, MatriuTG, sw_mat);
	Cuerpo(shaderId, MatriuVista, MatriuTG, sw_mat);
	Cabina(shaderId, MatriuVista, MatriuTG, sw_mat);
};

void Alas(GLint shaderId, glm::mat4 MatriuVista, glm::mat4 MatriuTG, bool sw_mat[4])
{
// Matrius Transformació
	glm::mat4 TransMatrix(1.0), ModelMatrix(1.0), NormalMatrix(1.0);
	CColor col_object;
	
	draw_TriEBO_Object(GLU_DISK); //int nvertD = gluDisk_VAO(0.0f, 5.0f, 6, 1);

// Ala 1 Parte central
//Parte Exterior
	//glPushMatrix();
	  //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	  col_object.r = 1.0;	col_object.g = 1.0;		col_object.b = 1.0;	 col_object.a = 1.0;
	  SeleccionaColorMaterial(shaderId, col_object, sw_mat);
	  //glTranslatef(25.25f, 0.0f, 0.0f);
	  //glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScaled(10.0f, 10.0f, 5.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(25.25f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix,radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(10.0f, 10.0f, 5.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  
	  draw_TriEBO_Object(GLU_CYLINDER);	//gluCylinder(5.0f, 5.0f, 0.5f, 6, 1);
	//glPopMatrix();

	//Tapas
	//glPushMatrix();
//	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  //glTranslatef(25.25f, 0.0f, 0.0f);
	  //glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScaled(10.0f, 10.0f, 5.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(25.25f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(10.0f, 10.0f, 5.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  //gluDisk(0.0f, 5.0f, 6, 1);
	  draw_TriEBO_Object(GLU_DISK); // drawgluDisk(nvertD);
	//glPopMatrix();

	//glPushMatrix();
//	  glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  //glTranslatef(27.75f, 0.0f, 0.0f);
	  //glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScaled(10.0f, 10.0f, 5.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(27.75f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(10.0f, 10.0f, 5.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  //gluDisk(0.0f, 5.0f, 6, 1);
	  draw_TriEBO_Object(GLU_DISK); // drawgluDisk(nvertD);
	//glPopMatrix();

//Parte Interior
	//glPushMatrix();
	  //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  SetColor4d(0.0, 0.0, 0.0, 1.0);
// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	  col_object.r = 0.0;	col_object.g = 0.0;		col_object.b = 0.0;	 col_object.a = 1.0;
	  SeleccionaColorMaterial(shaderId, col_object, sw_mat);
	  //glTranslated(25.25f, 0.0f, 0.0f);
	  //glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScaled(2.0f, 2.0f, 2.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(25.25f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(2.0f, 2.0f, 2.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_USER1); // gluCylinder(5.0f, 5.0f, 2.0f, 6, 1);
	//glPopMatrix();

//Tapas
	//glPushMatrix();
	  //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  //glTranslatef(25.25f, 0.0f, 0.0f);
	  //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScalef(2.0f, 2.0f, 2.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(25.75f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(2.0f, 2.0f, 2.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  //gluDisk(0.0f, 5.0f, 6, 1);
	  draw_TriEBO_Object(GLU_DISK); //drawgluDisk(nvertD);
	//glPopMatrix();

	//glPushMatrix();
	  //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  //glTranslatef(29.25f, 0.0f, 0.0f);
	  //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScalef(2.0f, 2.0f, 2.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(29.25f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(2.0f, 2.0f, 2.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  //gluDisk(0.0f, 5.0f, 6, 1);
	  draw_TriEBO_Object(GLU_DISK); //drawgluDisk(nvertD);
	//glPopMatrix();

//Decoracion Exterior
	int j;
	//glPushMatrix();
	  //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  //glTranslatef(26.5f, 0.0f, 0.0f);
	  //glScalef(15.75f, 13.75f, 13.75f);
	  //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(26.5f, 0.0f, 0.0f));
	  TransMatrix = glm::scale(TransMatrix, vec3(15.75f, 13.75f, 13.75f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(30.f), vec3(0.0f, 0.0f, 1.0f));
	  for (j = 0; j<3; j = j + 1)
		{	//glRotatef(60, 0, 0, 1);
			TransMatrix = glm::rotate(TransMatrix, radians(60.f), vec3(0.0f, 0.0f, 1.0f));
			//glPushMatrix();
			   //glScalef(12.7f * 0.583f, 0.1f, 0.3f);
				ModelMatrix = glm::scale(TransMatrix, vec3(12.7f * 0.583f, 0.1f, 0.3f));
				// Pas ModelView Matrix a shader
				glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
				NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
				// Pas NormalMatrix a shader
				glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
				draw_TriEBO_Object(GLUT_CUBE); //glutSolidCube(1.0f);
			//glPopMatrix();
		}
	//glPopMatrix();

	int u;
	//glPushMatrix();
	//glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	TransMatrix = MatriuTG;
	for (u = 0; u < 6; u = u + 1)
	{	//glRotated(60, 1.0, 0.0, 0.0);
		TransMatrix = glm::rotate(TransMatrix, radians(60.f), vec3(1.0f, 0.0f, 0.0f));
		//glPushMatrix();
			//glTranslatef(26.5f, 0.0f, 43.25f);
			//glRotatef(90, 0.0, 0.0, 1.0);
			//glScalef(10 * 5.155f, 5.0f, 3.0f);
			ModelMatrix = glm::translate(TransMatrix, vec3(26.5f, 0.0f, 43.25f));
			ModelMatrix = glm::rotate(ModelMatrix, radians(90.f), vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix = glm::scale(ModelMatrix, vec3(10 * 5.155f, 5.0f, 3.0f));
			// Pas ModelView Matrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
			NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
			// Pas NormalMatrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
			draw_TriEBO_Object(GLUT_CUBE); //glutSolidCube(1.0f);
		//glPopMatrix();
	}
	//glPopMatrix();


//Ala 2 Parte central 
//Parte Exterior
	//glPushMatrix();
	  //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  SetColor4d(1.0, 1.0, 1.0, 1.0);
// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	  col_object.r = 1.0;	col_object.g = 1.0;		col_object.b = 1.0;	 col_object.a = 1.0;
	  SeleccionaColorMaterial(shaderId, col_object, sw_mat);
	  //glTranslated(-27.75f, 0.0f, 0.0f);
	  //glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScaled(10.0, 10.0, 5.0);
	  TransMatrix = glm::translate(MatriuTG, vec3(-27.75f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(10.0f, 10.0f, 5.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLU_CYLINDER); //gluCylinder(5.0f, 5.0f, 0.5f, 6, 1);
	//glPopMatrix();

//Tapas
	//glPushMatrix();
	  //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  //glTranslatef(-27.75f, 0.0f, 0.0f);
	  //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScalef(10.0f, 10.0f, 5.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(-27.75f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(10.0f, 10.0f, 5.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  //gluDisk(0.0f, 5.0f, 6, 1);
	  draw_TriEBO_Object(GLU_DISK); //drawgluDisk(nvertD);
	//glPopMatrix();

	//glPushMatrix();
	  //glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	  //glTranslatef(-25.25f, 0.0f, 0.0f);
	  //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScalef(10.0f, 10.0f, 5.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(-25.25f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(10.0f, 10.0f, 5.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  //gluDisk(0.0f, 5.0f, 6, 1);
	  draw_TriEBO_Object(GLU_DISK); //drawgluDisk(nvertD);
	//glPopMatrix();

//Parte Interior
	//glPushMatrix();
	  //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  // Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	  col_object.r = 0.0;	col_object.g = 0.0;		col_object.b = 0.0;	 col_object.a = 1.0;
	  SeleccionaColorMaterial(shaderId, col_object, sw_mat);
	  //glTranslatef(-29.25f, 0.0f, 0.0f);
	  //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScalef(2.0f, 2.0f, 2.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(-29.25f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(2.0f, 2.0f, 2.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_USER1); // gluCylinder(5.0f, 5.0f, 2.0f, 6, 1);
	//glPopMatrix();

//Tapas
	//glPushMatrix();
	  //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
 	  //glTranslatef(-29.25f, 0.0f, 0.0f);
	  //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScalef(2.0f, 2.0f, 2.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(-29.25f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(2.0f, 2.0f, 2.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  //gluDisk(0.0f, 5.0f, 6, 1);
	  draw_TriEBO_Object(GLU_DISK);  //drawgluDisk(nvertD);
	//glPopMatrix();

	//glPushMatrix();
	  //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  //SetColor4d(0.0, 0.0, 0.0, 0.0);
	  //glTranslatef(-25.25f, 0.0f, 0.0f);
	  //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScalef(2.0f, 2.0f, 2.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(-25.25f, 0.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(2.0f, 2.0f, 2.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  //gluDisk(0.0f, 5.0f, 6, 1);
	  draw_TriEBO_Object(GLU_DISK); //drawgluDisk(nvertD);
	//glPopMatrix();

//Decoracion Exterior
	int w;
	//glPushMatrix();
	  //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  //SetColor4d(0.0, 0.0, 0.0, 0.0);
	  //glTranslatef(-26.5f, 0.0f, 0.0f);
	  //glScalef(15.75f, 13.75f, 13.75f);
	  //glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	  //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(-26.5f, 0.0f, 0.0f));
	  TransMatrix = glm::scale(TransMatrix, vec3(15.75f, 13.75f, 13.75f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(30.f), vec3(0.0f, 0.0f, 1.0f));
	  for (w = 0; w<3; w = w + 1)
	  {	//glRotatef(60.0f, 0.0f, 0.0f, 1.0f);
		TransMatrix = glm::rotate(TransMatrix, radians(60.f), vec3(0.0f, 0.0f, 1.0f));
		//glPushMatrix();
			//glScalef(12.7f * 0.583f, 0.1f, 0.3f);
			ModelMatrix = glm::scale(TransMatrix, vec3(12.7f * 0.583f, 0.1f, 0.3f));
			// Pas ModelView Matrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
			NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
			// Pas NormalMatrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
			draw_TriEBO_Object(GLUT_CUBE); //glutSolidCube(1.0f);
		//glPopMatrix();
	}
	//glPopMatrix();

	int h;
	//glPushMatrix();
	  //glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
	  //SetColor4d(0.0, 0.0, 0.0, 0.0);
	  TransMatrix = MatriuTG;
	  for (h = 0; h < 6; h = h + 1)
	  {	//glRotated(60, 1.0, 0.0, 0.0);
		TransMatrix = glm::rotate(TransMatrix, radians(60.f), vec3(1.0f, 0.0f, 0.0f));
		//glPushMatrix();
			//glTranslatef(-26.5f, 0.0f, 43.25f);
			//glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			//glScalef(10 * 5.155f, 5.0f, 3.0f);
			ModelMatrix = glm::translate(TransMatrix, vec3(-26.5f, 0.0f, 43.25f));
			ModelMatrix = glm::rotate(ModelMatrix, radians(90.f), vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix = glm::scale(ModelMatrix, vec3(10 * 5.155f, 5.0f, 3.0f));
			// Pas ModelView Matrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
			NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
			// Pas NormalMatrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
			draw_TriEBO_Object(GLUT_CUBE); //glutSolidCube(1.0f);
		//glPopMatrix();
	  }
	//glPopMatrix();
	  //deleteVAO(GLU_DISK);
};


void Motor(GLint shaderId, glm::mat4 MatriuVista, glm::mat4 MatriuTG, bool sw_mat[4])
{
// Matrius  
	glm::mat4 TransMatrix(1.0), ModelMatrix(1.0), NormalMatrix(1.0);
	CColor col_object;

//Helices Motor
	int j;
	//glPushMatrix();
	  //glColor4f(0.58f, 0.58f, 0.58f, 0.0f);
// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	  col_object.r = 0.58;	col_object.g = 0.58;	col_object.b = 0.58;	 col_object.a = 1.0;
	  SeleccionaColorMaterial(shaderId, col_object, sw_mat);
	  //glTranslatef(0.0f, -18.0f, 0.0f);
	  //glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  //glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  //glRotatef(45, 0, 0, 1);
	  TransMatrix = glm::translate(MatriuTG, vec3(0.0f, -18.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(-90.f), vec3(0.0f, 0.0f, 1.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(45.f), vec3(0.0f, 0.0f, 1.0f));
	  for (j = 0; j<6; j = j + 1)
	  {	//glRotatef(30, 0, 0, 1);
		 TransMatrix = glm::rotate(TransMatrix, radians(30.f), vec3(0.0f, 0.0f, 1.0f));
		//glPushMatrix();
			//glScalef(12 * 0.583f, 0.1f, 0.3f);
			 ModelMatrix = glm::scale(TransMatrix, vec3(12 * 0.583f, 0.1f, 0.3f));
			// Pas ModelView Matrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
			NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
			// Pas NormalMatrix a shader
			glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
			draw_TriEBO_Object(GLUT_CUBE); //glutSolidCube(1.0f);
		//glPopMatrix();
	  }
	//glPopMatrix();

//Soporte Motor
	//glPushMatrix();
	  //glColor4f(0.28f, 0.28f, 0.28f, 0.0f);
	  // Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	  col_object.r = 0.28;	col_object.g = 0.28;	col_object.b = 0.28;	 col_object.a = 1.0;
	  SeleccionaColorMaterial(shaderId, col_object, sw_mat);
	  //glTranslatef(0.0f, -18.0f, 0.0f);
	  //glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  //glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  TransMatrix = glm::translate(MatriuTG, vec3(0.0f, -18.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(-90.f), vec3(0.0f, 0.0f, 1.0f));
	  ModelMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_TORUS); //glutSolidTorus(1.0, 5.0, 20, 20);
	//glPopMatrix();
};

void Canon(GLint shaderId, glm::mat4 MatriuVista, glm::mat4 MatriuTG, bool sw_mat[4])
{
// Matrius Transformació
	glm::mat4 TransMatrix(1.0), ModelMatrix(1.0), NormalMatrix(1.0);
	CColor col_object;

//Cañones

// Salida cañon 1
	//glPushMatrix();
		//glColor4d(0.28, 0.28, 0.28, 0.0);
// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		col_object.r = 0.28;	col_object.g = 0.28;	col_object.b = 0.28;	 col_object.a = 1.0;
		SeleccionaColorMaterial(shaderId, col_object, sw_mat);
		//glTranslated(5.0, 8.0, -10.0);
		//glRotated(-90.0, 1.0, 0.0, 0.0);
		TransMatrix = glm::translate(MatriuTG, vec3(5.0f, 8.0f, -10.0f));
		ModelMatrix = glm::rotate(TransMatrix, radians(-90.f), vec3(1.0f, 0.0f, 0.0f));
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		// Pas NormalMatrix a shader
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_TriEBO_Object(GLUT_USER2); //gluCylinder(1.0f, 0.5f, 5.0f, 60, 1);
	//glPopMatrix();

// Salida cañon 2
	//glPushMatrix();
	  //glColor4d(0.28, 0.28, 0.28, 0.0);
	  //glTranslated(-5.0, 8.0, -10.0);
	  //glRotated(-90.0, 1.0, 0.0, 0.0);
	  TransMatrix = glm::translate(MatriuTG, vec3(-5.0f, 8.0f, -10.0f));
	  ModelMatrix = glm::rotate(TransMatrix, radians(-90.f), vec3(1.0f, 0.0f, 0.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_USER2); // gluCylinder(1.0f, 0.5f, 5.0f, 60, 1);
	//glPopMatrix();

// Cañon 1
	//glPushMatrix();
		//glColor4d(0.58, 0.58, 0.58, 0.0);
// Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
		col_object.r = 0.58;	col_object.g = 0.58;		col_object.b = 0.58;	 col_object.a = 1.0;
		SeleccionaColorMaterial(shaderId, col_object, sw_mat);
		//glTranslated(5.0, 10.0, -10.0);
		//glRotated(-90.0, 1.0, 0.0, 0.0);
		TransMatrix = glm::translate(MatriuTG, vec3(5.0f, 10.0f, -10.0f));
		ModelMatrix = glm::rotate(TransMatrix, radians(-90.f), vec3(1.0f, 0.0f, 0.0f));
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		// Pas NormalMatrix a shader
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_TriEBO_Object(GLUT_USER3); // gluCylinder(0.35f, 0.35f, 5.0f, 80, 1);
	//glPopMatrix();

//Cañon 2
	//glPushMatrix();
		//glColor4d(0.58, 0.58, 0.58, 1.0);
 		//glTranslated(-5.0, 10.0, -10.0);
		//glRotated(-90.0, 1.0, 0.0, 0.0);
		TransMatrix = glm::translate(MatriuTG, vec3(-5.0f, 10.0f, -10.0f));
		ModelMatrix = glm::rotate(TransMatrix, radians(-90.f), vec3(1.0f, 0.0f, 0.0f));
		// Pas ModelView Matrix a shader
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
		NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
		// Pas NormalMatrix a shader
		glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
		draw_TriEBO_Object(GLUT_USER3); // gluCylinder(0.35f, 0.35f, 5.0f, 80, 1);
	//glPopMatrix();
}

void Cuerpo(GLint shaderId, glm::mat4 MatriuVista, glm::mat4 MatriuTG, bool sw_mat[4])
{
// Matrius Transformació
	glm::mat4 TransMatrix(1.0), ModelMatrix(1.0), NormalMatrix(1.0);
	CColor col_object;

//Sujeccion de las Alas

//Lado2
	//glPushMatrix();
	  //glColor4d(0.16, 0.16, 0.16, 1.0);
	  // Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	  col_object.r = 0.16;	col_object.g = 0.16;	col_object.b = 0.16;	 col_object.a = 1.0;
	  SeleccionaColorMaterial(shaderId, col_object, sw_mat);
	  //glRotated(-90.0, 0.0, 1.0, 0.0);
	  //glRotated(90.0, 0.0, 0.0, 1.0);
	  //glScaled(2.0, 2.0, 2.5);
	  TransMatrix = glm::rotate(MatriuTG, radians(-90.f), vec3(0.0f, 1.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 0.0f, 1.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(2.0f, 2.0f, 2.5f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_USER4); // gluCylinder(4.0f, 2.0f, 10.25f, 40, 1);
	//glPopMatrix();

//X2
	//glPushMatrix();
	  //glColor4d(0.16, 0.16, 0.16, 1.0);
	  //glTranslated(-25.0, 0.0, 0.0);
	  //glRotated(90.0, 0.0, 1.0, 0.0);
	  TransMatrix = glm::translate(MatriuTG, vec3(-25.0f, 0.0f, 0.0f));
	  ModelMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_TORUS); // glutSolidTorus(1.0, 5.0, 20, 20);
	//glPopMatrix();

//Lado 1
	//glPushMatrix();
	  //glColor4f(0.16f, 0.16f, 0.16f, 1.0f);
	  //SetColor4d(0.16, 0.16, 0.16, 1.0);
	  //glRotated(90.0, 0.0, 1.0, 0.0);
	  //glRotated(90.0, 0.0, 0.0, 1.0);
	  //glScaled(2.0, 2.0, 2.5);
	  TransMatrix = glm::rotate(MatriuTG, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 0.0f, 1.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(2.0f, 2.0f, 2.5f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_USER4); // gluCylinder(4.0f, 2.0f, 10.25f, 40, 1);
	//glPopMatrix();

//X1
	//glPushMatrix();
	  //glColor4d(0.16, 0.16, 0.16, 1.0);
	  //SetColor4d(0.16, 0.16, 0.16, 1.0);
	  //glTranslated(25.25, 0.0, 0.0);
	  //glRotated(90.0, 0.0, 1.0, 0.0);
	  TransMatrix = glm::translate(MatriuTG, vec3(25.25f, 0.0f, 0.0f));
	  ModelMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_TORUS); //glutSolidTorus(1.0, 5.0, 20, 20);
	//glPopMatrix();

//Cuerpo
	//glPushMatrix();
	  //glColor4d(0.16, 0.16, 0.16, 1.0);
	  //SetColor4d(0.16, 0.16, 0.16, 1.0);
	  //glScaled(1.75f, 1.75f, 1.5f);
	  ModelMatrix = glm::scale(MatriuTG, vec3(1.75f, 1.75f, 1.5f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLU_SPHERE); //gluSphere(10.0f, 80, 80);
	//glPopMatrix();
}

void Cabina(GLint shaderId, glm::mat4 MatriuVista, glm::mat4 MatriuTG, bool sw_mat[4])
{
// Matrius Transformació
	glm::mat4 TransMatrix(1.0), ModelMatrix(1.0), NormalMatrix(1.0);
	CColor col_object;

	glEnable(GL_BLEND);
// Tapa Cabina
	//glPushMatrix();
	  //glColor4d(1.0, 0.058, 0.058, 0.5);
	  // Definició propietats de reflexió (emissió, ambient, difusa, especular) del material.
	  col_object.r = 1.0;	col_object.g = 0.058;	col_object.b = 0.058;	 col_object.a = 0.5;
	  SeleccionaColorMaterial(shaderId, col_object, sw_mat);
	  //glTranslated(0.0, 19.45, 0.0);
	  //glRotated(-90.0, 0.0, 0.0, 1.0);
	  //glRotated(90.0, 0.0, 1.0, 0.0);
	  //glScaled(1.75, 1.75, 1.75);
	  TransMatrix = glm::translate(MatriuTG, vec3(0.0f, 19.45f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(-90.f), vec3(0.0f, 0.0f, 1.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(1.75f, 1.75f, 1.75f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_USER6); //gluDisk(0.0f, 1.5f, 8, 1);
	//glPopMatrix();

// Cristal Cabina
	//glPushMatrix();
	  //glColor4d(1.0, 0.058, 0.058, 0.5);
	  //glTranslated(0.0f, 19.45f, 0.0f);
	  //glRotated(-90.0f, 0.0f, 0.0f, 1.0f);
	  //glRotated(90.0f, 0.0f, 1.0f, 0.0f);
	  //glScaled(1.75f, 1.75f, 1.75f);
	  TransMatrix = glm::translate(MatriuTG, vec3(0.0f, 19.45f, 0.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(-90.f), vec3(0.0f, 0.0f, 1.0f));
	  TransMatrix = glm::rotate(TransMatrix, radians(90.f), vec3(0.0f, 1.0f, 0.0f));
	  ModelMatrix = glm::scale(TransMatrix, vec3(1.75f, 1.75f, 1.75f));
	  // Pas ModelView Matrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	  NormalMatrix = transpose(inverse(MatriuVista * ModelMatrix));
	  // Pas NormalMatrix a shader
	  glUniformMatrix4fv(glGetUniformLocation(shaderId, "normalMatrix"), 1, GL_FALSE, &NormalMatrix[0][0]);
	  draw_TriEBO_Object(GLUT_USER5); //gluCylinder(1.5f, 4.5f, 2.0f, 8, 1);
	//glPopMatrix();

	glDisable(GL_BLEND);
}
// FI OBJECTE TIE: FETS PER ALUMNES -----------------------------------------------------------------
