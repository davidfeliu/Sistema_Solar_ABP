//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb interfície MFC, llibreries GLM i OpenGL 3.3+
//******** Enric Martí (Setembre 2022)
// shader.h : interface of the Shader class
//
/////////////////////////////////////////////////////////////////////////////


#ifndef SHADER_H
#define SHADER_H

// Entorn VGI: Constants i tipus de l'aplicació entorn VGI
#include "stdafx.h"

/*
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
*/

class Shader
{

// Atributos
public:
//-------------- Entorn VGI: Variables globals de la classe Shader
	unsigned int programID;	// the program ID
//-------------- Entorn VGI: Fi De Variables globals de la classe Shader 

//-------------- Entorn VGI: Funcions (mètodes) de de la classe Shader 
// Constructor reads and builds the Vertex Shader (VS) and Fragment Shader (FS) into a Program (programID)
//	Shader(std::string file_Vert, std::string file_Frag);
	Shader();

// Destructor
	~Shader();

// Funcions Auxiliars
	GLuint loadFileShaders(std::string file_Vert, std::string file_Frag);
	GLuint loadFileShadersG(std::string file_Vert, std::string file_Frag, std::string file_Geom);
	// For testing
	GLuint loadFileShadersTest(std::string file_Vert, std::string file_Frag);
	GLuint loadFileShadersGTest(std::string file_Vert, std::string file_Frag, std::string file_Geom);

	int Shader::DisplayMessageBox(std::string mess, std::string fileShader);
	char* textFileRead(const char* fn);
	void releaseAllShaders();

// Get programID
	GLuint getProgramID();

// Use/activate the shader
	void use();

// Utility uniform functions
	// Pas de valors simples (bool, int, float o double)
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
	void setDouble(const std::string &name, double value);

	// Pas de vectors de 3 posicions (bool, int, float o double)
	void setBool3(const std::string &name, bool value1, bool value2, bool value3);
	void setInt3(const std::string &name, int value1, int value2, int value3);
	void setFloat3(const std::string &name, float value1, float value2, float value3);
	void setDouble3(const std::string &name, double value1, double value2, double value3);

	// Pas de vectors de 4 posicions (float o double)
	void setFloat4(const std::string &name, float value1, float value2, float value3, float value4);
	void setDouble4(const std::string &name, double value1, double value2, double value3, double value4);
	
	// Pas de Matrius 3x3 i 4x4
	void setMatrix3fv(const std::string &name, glm::mat3 trans);
	void setMatrix4fv(const std::string &name, glm::mat4 trans);

	void DeleteProgram();

//-------------- Entorn VGI: Fi Funcions (mètodes) de de la classe Shader 
};
#endif
