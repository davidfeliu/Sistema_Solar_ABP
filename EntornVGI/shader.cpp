//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb interfície MFC, llibreries GLM i OpenGL 3.3+
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2022)
// Shader.cpp: implementación de la clase Shader
// FUNCIONS:		- Constructor (OnPaint)
//					- Control teclat (OnKeyDown)
//					- Control mouse interactiu i botons mouse 
//							(OnLButtomDown, OnRButtomDown, OnMouseMove)
//					- Control opcions de menú (On*, OnUpdate*)
//					- Control de color de fons per teclat (FONS)
//					- Transformacions Geomètriques Interactives via mouse
//

#include "stdafx.h"
#include "Shader.h"	

#include <string>

/////////////////////////////////////////////////////////////////////////////
// Construcción o destrucción de Shader
/*****************************************************************************
* loadFileShaders()
*      Loads custom shader by specifying filename (expects frag/vert pair)
*****************************************************************************/
//Shader::Shader(std::string file_Vert, std::string file_Frag)
Shader::Shader()
{
// Inicialitzar programID
	this->programID = -1;

}


Shader::~Shader()
{
	glDeleteProgram(this->programID);		// Don't leak the program.
}


/*****************************************************************************
* loadFileShaders()
*      Loads custom shader by specifying filename (expects frag/vert pair)
*****************************************************************************/
//GLuint Shader::loadFileShaders(const char*  file_Vert, const char*  file_Frag)
GLuint Shader::loadFileShaders(std::string file_Vert, std::string file_Frag)
//GLuint Shader::loadFileShaders(CString file_Vert, CString file_Frag)
{
	GLuint v, f;
	char* vs = NULL, * fs = NULL;
	int info_log_length = 0, compile_result = 0, link_result = 0;
	FILE* fd;

// Inicialitzar programID
	if (this->programID !=-1) glDeleteProgram(this->programID);		// Don't leak the program.
	this->programID = -1;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	//file_Vert = file_Vert + ".vert";
	const char* fitxer_Vert = file_Vert.c_str();
	vs = textFileRead(fitxer_Vert);
	//vs = textFileRead(filename + ".vert");

	//file_Frag = file_Frag + ".frag";
	const char* fitxer_Frag = file_Frag.c_str();
	fs = textFileRead(fitxer_Frag);

	if ((vs == NULL) || (fs == NULL)) return GL_FALSE;	// Si no s’han trobat els fitxers, retornar GL_FALSE

	const char* vv = vs;
	const char* ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);

// COMPILACIÓ VERTEX SHADER (VS)
	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

		DisplayMessageBox("Error compilacio Vertex Shader", file_Vert);
		//AfxMessageBox(_T("Error compilació Vertex Shader"));
		//fprintf(stderr, "%s \n", "Error compilacio Vertex Shader");

		// Volcar missatges error a fitxer GLSL_Error.VERT
		if ((fd = fopen("GLSL_Error.VERT", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.VERT was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.VERT was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++)
		{
			fprintf(fd, "%c", errorLog[i]);
			fprintf(stderr, "%c", errorLog[i]);
		}
		fclose(fd);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(v); // Don't leak the shader.
		return GL_FALSE;
	}
	//else DisplayMessageBox("Compilacio Vertex Shader OK", file_Vert); //AfxMessageBox(_T("Compilacio Vertex Shader OK")); //fprintf(stderr, "%s \n", "Compilacio Vertex Shader OK");

// COMPILACIÓ FRAGMENT SHADER (FS)
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(f, maxLength, &maxLength, &errorLog[0]);

		DisplayMessageBox("Error compilacio Fragment Shader", file_Frag);
		//AfxMessageBox(_T("Error compilació Fragment Shader"));
		//fprintf(stderr, "%s \n", "Error compilacio Fragment Shader");

		// Volcar missatges error a fitxer Compile_Error.FRAG
		if ((fd = fopen("GLSL_Error.FRAG", "w")) == NULL)
		{	AfxMessageBox(_T("GLSL_Error.FRAG was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.FRAG was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++)
		{	fprintf(fd, "%c", errorLog[i]);
			fprintf(stderr, "%c", errorLog[i]);
		}
		fclose(fd);

		glDeleteShader(f);	// Don't leak the shader.
		return GL_FALSE;			// Exit with failure.
	}
	//else DisplayMessageBox("Compilacio Fragment Shader OK", file_Frag); // AfxMessageBox(_T("Compilacio Fragment Shader OK"));  //fprintf(stderr, "%s \n", "Compilacio Fragment Shader OK");

// Creació del Program que inclou Vertex i Fragment Shader
	programID = glCreateProgram();
	glAttachShader(programID, v);
	glAttachShader(programID, f);

// Attribute Locations must be setup before calling glLinkProgram()
//	glBindAttribLocation(programID, 0, "in_Vertex"); // Vèrtexs
//	glBindAttribLocation(programID, 1, "in_Normal"); // Normals
//	glBindAttribLocation(programID, 2, "in_TexCoord"); // Textura
//	glBindAttribLocation(programID, 3, "in_Color"); // Color

// LINKEDICIÓ SHADER PROGRAM (programID)
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &link_result);

	// Llista error de linkedició del Shader Program
	if (link_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

		AfxMessageBox(_T("Error Linkedicio Shader Program"));
		//fprintf(stderr, "%s \n", "Error Linkedicio Shader Program");

		// Volcar missatges error a fitxer GLSL_Error.LINK
		if ((fd = fopen("GLSL_Error.LINK", "w")) == NULL)
		{	AfxMessageBox(_T("GLSL_Error.LINK was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.LINK was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
		fclose(fd);

		glDeleteShader(v);		// Don't leak the shader.
		glDeleteShader(f);		// Don't leak the shader.
		glDeleteProgram(programID);		// Don't leak the program.
		return GL_FALSE;		// Exit with failure.
	}
	//else AfxMessageBox(_T("Linkedicio Shader Program OK")); //fprintf(stderr, "%s \n", "Linkedicio Shader Program OK");

	glDeleteShader(v);		// Don't leak the shader.
	glDeleteShader(f);		// Don't leak the shader.

// Activació del Programa
	glUseProgram(programID);

// Retorna referència al Program creat
	return programID;
}


/*****************************************************************************
* loadFileShadersG()
*      Loads custom shader by specifying filename (expects geom/frag/vert trio)
*****************************************************************************/
//GLuint Shader::loadFileShaders(const char*  file_Vert, const char*  file_Frag, const char*  file_Geom = nullptr)
GLuint Shader::loadFileShadersG(std::string file_Vert, std::string file_Frag, std::string file_Geom)
{
	GLuint v, f, g;
	char* vs = NULL, * fs = NULL, * gs = NULL;
	int info_log_length = 0, compile_result = 0, link_result = 0;
	FILE* fd;

	// Inicialitzar programID
	if (this->programID != -1) glDeleteProgram(this->programID);		// Don't leak the program.
	this->programID = -1;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	//file_Vert = file_Vert + ".vert";
	const char* fitxer_Vert = file_Vert.c_str();
	vs = textFileRead(fitxer_Vert);
	//vs = textFileRead(filename + ".vert");

	//file_Frag = file_Frag + ".frag";
	const char* fitxer_Frag = file_Frag.c_str();
	fs = textFileRead(fitxer_Frag);

	if ((vs == NULL) || (fs == NULL)) return GL_FALSE;	// Si no s’han trobat els fitxers, retornar GL_FALSE

	const char* vv = vs;
	const char* ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);

	// COMPILACIÓ VERTEX SHADER (VS)
	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

		DisplayMessageBox("Error compilacio Vertex Shader", file_Vert);
		//AfxMessageBox(_T("Error compilació Vertex Shader"));
		//fprintf(stderr, "%s \n", "Error compilacio Vertex Shader");

		// Volcar missatges error a fitxer GLSL_Error.VERT
		if ((fd = fopen("GLSL_Error.VERT", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.VERT was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.VERT was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++)
		{
			fprintf(fd, "%c", errorLog[i]);
			fprintf(stderr, "%c", errorLog[i]);
		}
		fclose(fd);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(v); // Don't leak the shader.
		return GL_FALSE;
	}
//	else DisplayMessageBox("Compilacio Vertex Shader OK", file_Vert);  //AfxMessageBox(_T("Compilacio Vertex Shader OK"));  //fprintf(stderr, "%s \n", "Compilacio Vertex Shader OK");

	// COMPILACIÓ FRAGMENT SHADER (FS)
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(f, maxLength, &maxLength, &errorLog[0]);

		DisplayMessageBox("Error compilacio Fragment Shader", file_Frag);
		//AfxMessageBox(_T("Error compilació Fragment Shader"));
		//fprintf(stderr, "%s \n", "Error compilacio Fragment Shader");

		// Volcar missatges error a fitxer Compile_Error.FRAG
		if ((fd = fopen("GLSL_Error.FRAG", "w")) == NULL)
		{	AfxMessageBox(_T("GLSL_Error.FRAG was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.FRAG was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++)
		{
			fprintf(fd, "%c", errorLog[i]);
			fprintf(stderr, "%c", errorLog[i]);
		}
		fclose(fd);

		glDeleteShader(f);	// Don't leak the shader.
		return GL_FALSE;			// Exit with failure.
	}
//	else DisplayMessageBox("Compilacio Fragment Shader OK", file_Frag); //AfxMessageBox(_T("Compilacio Fragment Shader OK")); // fprintf(stderr, "%s \n", "Compilacio Fragment Shader OK");

	// GEOMETRY SHADER (si definit)
	if (file_Geom.c_str() != nullptr)
	{
		g = glCreateShader(GL_GEOMETRY_SHADER);

		const char* fitxer_Geom = file_Geom.c_str();
		vs = textFileRead(fitxer_Geom);
		const char* gg = gs;
		glShaderSource(g, 1, &gg, NULL);
		free(gs);

		// COMPILACIÓ GEOMETRY SHADER (GS)
		glCompileShader(g);
		glGetShaderiv(g, GL_COMPILE_STATUS, &compile_result);
		// Llista error de compilació
		if (compile_result == GL_FALSE) {
			GLint maxLength = 0;

			glGetShaderiv(g, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(g, maxLength, &maxLength, &errorLog[0]);

			DisplayMessageBox("Error compilacio Geometry Shader", file_Geom);
			//AfxMessageBox(_T("Error compilació Geometry Shader"));
			//fprintf(stderr, "%s \n", "Error compilacio Geometry Shader");

			// Volcar missatges error a fitxer Compile_Error.GEOM
			if ((fd = fopen("GLSL_Error.GEOM", "w")) == NULL)
			{	AfxMessageBox(_T("GLSL_Error.GEOM was not opened"));
				//fprintf(stderr, "%s \n", "GLSL_Error.GEOM was not opened");
				return GL_FALSE;
			}
			for (int i = 0; i <= maxLength; i = i++)
			{
				fprintf(fd, "%c", errorLog[i]);
				fprintf(stderr, "%c", errorLog[i]);
			}
			fclose(fd);

			glDeleteShader(g);	// Don't leak the shader.
			return GL_FALSE;	// Exit with failure.
		}
//		else DisplayMessageBox("Compilacio Geometry Shader OK", file_Geom); //AfxMessageBox(_T("Compilacio Geometry Shader OK")); //fprintf(stderr, "%s \n", "Compilacio Geometry Shader OK");
	}

	// Creació del Program que inclou Vertex i Fragment Shader
	programID = glCreateProgram();
	glAttachShader(programID, v);
	glAttachShader(programID, f);

	// Inclusió Geometry Shader, si existeix
	if (file_Geom.c_str() != nullptr) glAttachShader(programID, g);

	// Attribute Locations must be setup before calling glLinkProgram()
	//	glBindAttribLocation(programID, 0, "in_Vertex"); // Vèrtexs
	//	glBindAttribLocation(programID, 1, "in_Normal"); // Normals
	//	glBindAttribLocation(programID, 2, "in_TexCoord"); // Textura
	//	glBindAttribLocation(programID, 3, "in_Color"); // Color

	// LINKEDICIÓ SHADER PROGRAM (programID)
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &link_result);

	// Llista error de linkedició del Shader Program
	if (link_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

		AfxMessageBox(_T("Error Linkedicio Shader Program"));
		//fprintf(stderr, "%s \n", "Error Linkedicio Shader Program");

		// Volcar missatges error a fitxer GLSL_Error.LINK
		if ((fd = fopen("GLSL_Error.LINK", "w")) == NULL)
		{	AfxMessageBox(_T("GLSL_Error.LINK was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.LINK was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
		fclose(fd);

		glDeleteShader(v);		// Don't leak the shader.
		glDeleteShader(f);		// Don't leak the shader.
		glDeleteProgram(programID);		// Don't leak the program.
		return GL_FALSE;		// Exit with failure.
	}
//	else AfxMessageBox(_T("Linkedicio Shader Program OK")); //fprintf(stderr, "%s \n", "Linkedicio Shader Program OK");

	glDeleteShader(v);		// Don't leak the shader.
	glDeleteShader(f);		// Don't leak the shader.
	glDeleteShader(g);		// Don't leak the shader.

// Activació del Programa
	glUseProgram(programID);

	// Retorna referència al Program creat
	return programID;
}


// LOADING SHADERS FOR TESTING GLSL CODE
/*****************************************************************************
* loadFileShadersTest()
*      Loads custom shader by specifying filename (expects frag/vert pair) giving error message boxs for testing shaders
*****************************************************************************/
//GLuint Shader::loadFileShadersTest(const char*  file_Vert, const char*  file_Frag)
GLuint Shader::loadFileShadersTest(std::string file_Vert, std::string file_Frag)
//GLuint Shader::loadFileShadersTest(CString file_Vert, CString file_Frag)
{
	GLuint v, f;
	char* vs = NULL, * fs = NULL;
	int info_log_length = 0, compile_result = 0, link_result = 0;
	FILE* fd;

	// Inicialitzar programID
	if (this->programID != -1) glDeleteProgram(this->programID);		// Don't leak the program.
	this->programID = -1;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	//file_Vert = file_Vert + ".vert";
	const char* fitxer_Vert = file_Vert.c_str();
	vs = textFileRead(fitxer_Vert);
	//vs = textFileRead(filename + ".vert");

	//file_Frag = file_Frag + ".frag";
	const char* fitxer_Frag = file_Frag.c_str();
	fs = textFileRead(fitxer_Frag);

	if ((vs == NULL) || (fs == NULL)) return GL_FALSE;	// Si no s’han trobat els fitxers, retornar GL_FALSE

	const char* vv = vs;
	const char* ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);

	// COMPILACIÓ VERTEX SHADER (VS)
	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

		DisplayMessageBox("Error compilacio Vertex Shader", file_Vert);
		//AfxMessageBox(_T("Error compilació Vertex Shader"));
		//fprintf(stderr, "%s \n", "Error compilacio Vertex Shader");

		// Volcar missatges error a fitxer GLSL_Error.VERT
		if ((fd = fopen("GLSL_Error.VERT", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.VERT was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.VERT was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++)
		{
			fprintf(fd, "%c", errorLog[i]);
			fprintf(stderr, "%c", errorLog[i]);
		}
		fclose(fd);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(v); // Don't leak the shader.
		return GL_FALSE;
	}
	else DisplayMessageBox("Compilacio Vertex Shader OK", file_Vert); //AfxMessageBox(_T("Compilacio Vertex Shader OK")); //fprintf(stderr, "%s \n", "Compilacio Vertex Shader OK");

// COMPILACIÓ FRAGMENT SHADER (FS)
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(f, maxLength, &maxLength, &errorLog[0]);

		DisplayMessageBox("Error compilació Fragment Shader", file_Frag);
		//AfxMessageBox(_T("Error compilació Fragment Shader"));
		//fprintf(stderr, "%s \n", "Error compilacio Fragment Shader");

		// Volcar missatges error a fitxer Compile_Error.FRAG
		if ((fd = fopen("GLSL_Error.FRAG", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.FRAG was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.FRAG was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++)
		{
			fprintf(fd, "%c", errorLog[i]);
			fprintf(stderr, "%c", errorLog[i]);
		}
		fclose(fd);

		glDeleteShader(f);	// Don't leak the shader.
		return GL_FALSE;			// Exit with failure.
	}
	else DisplayMessageBox("Compilacio Fragment Shader OK", file_Frag); // AfxMessageBox(_T("Compilacio Fragment Shader OK"));  //fprintf(stderr, "%s \n", "Compilacio Fragment Shader OK");

// Creació del Program que inclou Vertex i Fragment Shader
	programID = glCreateProgram();
	glAttachShader(programID, v);
	glAttachShader(programID, f);

	// Attribute Locations must be setup before calling glLinkProgram()
	//	glBindAttribLocation(programID, 0, "in_Vertex"); // Vèrtexs
	//	glBindAttribLocation(programID, 1, "in_Normal"); // Normals
	//	glBindAttribLocation(programID, 2, "in_TexCoord"); // Textura
	//	glBindAttribLocation(programID, 3, "in_Color"); // Color

	// LINKEDICIÓ SHADER PROGRAM (programID)
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &link_result);

	// Llista error de linkedició del Shader Program
	if (link_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

		AfxMessageBox(_T("Error Linkedicio Shader Program"));
		//fprintf(stderr, "%s \n", "Error Linkedicio Shader Program");

		// Volcar missatges error a fitxer GLSL_Error.LINK
		if ((fd = fopen("GLSL_Error.LINK", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.LINK was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.LINK was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
		fclose(fd);

		glDeleteShader(v);		// Don't leak the shader.
		glDeleteShader(f);		// Don't leak the shader.
		glDeleteProgram(programID);		// Don't leak the program.
		return GL_FALSE;		// Exit with failure.
	}
	else AfxMessageBox(_T("Linkedicio Shader Program OK")); //fprintf(stderr, "%s \n", "Linkedicio Shader Program OK");

	glDeleteShader(v);		// Don't leak the shader.
	glDeleteShader(f);		// Don't leak the shader.

// Activació del Programa
	glUseProgram(programID);

	// Retorna referència al Program creat
	return programID;
}


/*****************************************************************************
* loadFileShadersGTest()
*      Loads custom shader by specifying filename (expects geom/frag/vert trio) giving error message boxs for testing shaders
*****************************************************************************/
//GLuint Shader::loadFileShaders(const char*  file_Vert, const char*  file_Frag, const char*  file_Geom = nullptr)
GLuint Shader::loadFileShadersGTest(std::string file_Vert, std::string file_Frag, std::string file_Geom)
{
	GLuint v, f, g;
	char* vs = NULL, * fs = NULL, * gs = NULL;
	int info_log_length = 0, compile_result = 0, link_result = 0;
	FILE* fd;

	// Inicialitzar programID
	if (this->programID != -1) glDeleteProgram(this->programID);		// Don't leak the program.
	this->programID = -1;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	//file_Vert = file_Vert + ".vert";
	const char* fitxer_Vert = file_Vert.c_str();
	vs = textFileRead(fitxer_Vert);
	//vs = textFileRead(filename + ".vert");

	//file_Frag = file_Frag + ".frag";
	const char* fitxer_Frag = file_Frag.c_str();
	fs = textFileRead(fitxer_Frag);

	if ((vs == NULL) || (fs == NULL)) return GL_FALSE;	// Si no s’han trobat els fitxers, retornar GL_FALSE

	const char* vv = vs;
	const char* ff = fs;

	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);

	// COMPILACIÓ VERTEX SHADER (VS)
	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetShaderiv(v, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(v, maxLength, &maxLength, &errorLog[0]);

		DisplayMessageBox("Error compilacio Vertex Shader", file_Vert);
		//AfxMessageBox(_T("Error compilació Vertex Shader"));
		//fprintf(stderr, "%s \n", "Error compilacio Vertex Shader");

		// Volcar missatges error a fitxer GLSL_Error.VERT
		if ((fd = fopen("GLSL_Error.VERT", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.VERT was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.VERT was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++)
		{
			fprintf(fd, "%c", errorLog[i]);
			fprintf(stderr, "%c", errorLog[i]);
		}
		fclose(fd);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(v); // Don't leak the shader.
		return GL_FALSE;
	}
	else DisplayMessageBox("Compilacio Vertex Shader OK", file_Vert);  //AfxMessageBox(_T("Compilacio Vertex Shader OK"));  //fprintf(stderr, "%s \n", "Compilacio Vertex Shader OK");

	// COMPILACIÓ FRAGMENT SHADER (FS)
	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compile_result);
	// Llista error de compilació
	if (compile_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetShaderiv(f, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(f, maxLength, &maxLength, &errorLog[0]);

		DisplayMessageBox("Error compilacio Fragment Shader", file_Frag);
		//AfxMessageBox(_T("Error compilació Fragment Shader"));
		//fprintf(stderr, "%s \n", "Error compilacio Fragment Shader");

		// Volcar missatges error a fitxer Compile_Error.FRAG
		if ((fd = fopen("GLSL_Error.FRAG", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.FRAG was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.FRAG was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++)
		{
			fprintf(fd, "%c", errorLog[i]);
			fprintf(stderr, "%c", errorLog[i]);
		}
		fclose(fd);

		glDeleteShader(f);	// Don't leak the shader.
		return GL_FALSE;			// Exit with failure.
	}
	else DisplayMessageBox("Compilacio Fragment Shader OK", file_Frag); //AfxMessageBox(_T("Compilacio Fragment Shader OK")); // fprintf(stderr, "%s \n", "Compilacio Fragment Shader OK");

	// GEOMETRY SHADER (si definit)
	if (file_Geom.c_str() != nullptr)
	{
		g = glCreateShader(GL_GEOMETRY_SHADER);

		const char* fitxer_Geom = file_Geom.c_str();
		vs = textFileRead(fitxer_Geom);
		const char* gg = gs;
		glShaderSource(g, 1, &gg, NULL);
		free(gs);

		// COMPILACIÓ GEOMETRY SHADER (GS)
		glCompileShader(g);
		glGetShaderiv(g, GL_COMPILE_STATUS, &compile_result);
		// Llista error de compilació
		if (compile_result == GL_FALSE) {
			GLint maxLength = 0;

			glGetShaderiv(g, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(g, maxLength, &maxLength, &errorLog[0]);

			DisplayMessageBox("Error compilacio Geometry Shader", file_Geom);
			//AfxMessageBox(_T("Error compilació Geometry Shader"));
			//fprintf(stderr, "%s \n", "Error compilacio Geometry Shader");

			// Volcar missatges error a fitxer Compile_Error.GEOM
			if ((fd = fopen("GLSL_Error.GEOM", "w")) == NULL)
			{
				AfxMessageBox(_T("GLSL_Error.GEOM was not opened"));
				//fprintf(stderr, "%s \n", "GLSL_Error.GEOM was not opened");
				return GL_FALSE;
			}
			for (int i = 0; i <= maxLength; i = i++)
			{
				fprintf(fd, "%c", errorLog[i]);
				fprintf(stderr, "%c", errorLog[i]);
			}
			fclose(fd);

			glDeleteShader(g);	// Don't leak the shader.
			return GL_FALSE;	// Exit with failure.
		}
		else DisplayMessageBox("Compilacio Geometry Shader OK", file_Geom); //AfxMessageBox(_T("Compilacio Geometry Shader OK")); //fprintf(stderr, "%s \n", "Compilacio Geometry Shader OK");
	}

	// Creació del Program que inclou Vertex i Fragment Shader
	programID = glCreateProgram();
	glAttachShader(programID, v);
	glAttachShader(programID, f);

	// Inclusió Geometry Shader, si existeix
	if (file_Geom.c_str() != nullptr) glAttachShader(programID, g);

	// Attribute Locations must be setup before calling glLinkProgram()
	//	glBindAttribLocation(programID, 0, "in_Vertex"); // Vèrtexs
	//	glBindAttribLocation(programID, 1, "in_Normal"); // Normals
	//	glBindAttribLocation(programID, 2, "in_TexCoord"); // Textura
	//	glBindAttribLocation(programID, 3, "in_Color"); // Color

	// LINKEDICIÓ SHADER PROGRAM (programID)
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &link_result);

	// Llista error de linkedició del Shader Program
	if (link_result == GL_FALSE) {
		GLint maxLength = 0;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

		AfxMessageBox(_T("Error Linkedicio Shader Program"));
		//fprintf(stderr, "%s \n", "Error Linkedicio Shader Program");

		// Volcar missatges error a fitxer GLSL_Error.LINK
		if ((fd = fopen("GLSL_Error.LINK", "w")) == NULL)
		{
			AfxMessageBox(_T("GLSL_Error.LINK was not opened"));
			//fprintf(stderr, "%s \n", "GLSL_Error.LINK was not opened");
			return GL_FALSE;
		}
		for (int i = 0; i <= maxLength; i = i++) fprintf(fd, "%c", errorLog[i]);
		fclose(fd);

		glDeleteShader(v);		// Don't leak the shader.
		glDeleteShader(f);		// Don't leak the shader.
		glDeleteProgram(programID);		// Don't leak the program.
		return GL_FALSE;		// Exit with failure.
	}
	else AfxMessageBox(_T("Linkedicio Shader Program OK")); //fprintf(stderr, "%s \n", "Linkedicio Shader Program OK");

	glDeleteShader(v);		// Don't leak the shader.
	glDeleteShader(f);		// Don't leak the shader.
	glDeleteShader(g);		// Don't leak the shader.

// Activació del Programa
	glUseProgram(programID);

	// Retorna referència al Program creat
	return programID;
}





int Shader::DisplayMessageBox(std::string mess, std::string fileShader)
{
	// Conversió std::string --> LPCWSTR
	std::wstring stemp = std::wstring(mess.begin(), mess.end());
	//LPCWSTR msg = stemp.c_str();

	std::wstring stemp2 = std::wstring(fileShader.begin(), fileShader.end());
	//LPCWSTR msg = stemp.c_str();

	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)stemp.c_str(),
		(LPCWSTR)stemp2.c_str(),
		MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
	);

	switch (msgboxID)
	{
	case IDCANCEL:
		// TODO: add code
		break;
	case IDTRYAGAIN:
		// TODO: add code
		break;
	case IDCONTINUE:
		// TODO: add code
		break;
	}

	return msgboxID;
}


char* Shader::textFileRead(const char* fn) {
	FILE* fp;
	char* content = NULL;
	int count = 0;
	if (fn != NULL) {
		fp = fopen(fn, "rt");
		if (fp != NULL) {
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			if (count > 0) {
				content = (char*)malloc(sizeof(char) * (count + 1));
				count = (int) fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
		else {
			AfxMessageBox(_T("Vertex or Fragment Program not found"));
			//printf("%s \n", "Vertex or Fragment Program not found");
			return NULL;
		}
	}
	return content;
}

/*****************************************************************************
* releaseAllShaders()
*      Unload shaders, return to OpenGL fixed function.
*****************************************************************************/
void Shader::releaseAllShaders()
{
	glUseProgram(0);
}


/*****************************************************************************
* getProgramID()
*      Get programID. If -1, error in create programID
*****************************************************************************/
GLuint Shader::getProgramID()
{
	return this->programID;
}


/*****************************************************************************
* Use()
*      Activate shades in programID
*****************************************************************************/
void Shader::use()
{
	glUseProgram(this->programID);
}


/*****************************************************************************
* set*()
*      Set Uniform Shader Variables
*****************************************************************************/
// Pas de valors simples (bool, int, float o double)
void Shader::setBool(const std::string &name, bool value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value)
{
	glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Shader::setDouble(const std::string &name, double value)
{
	glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

// Pas de vectors de 3 posicions (bool, int, float o double)
void Shader::setBool3(const std::string &name, bool value1, bool value2, bool value3)
{
	glUniform3i(glGetUniformLocation(this->programID, name.c_str()), (int)value1, (int)value2, (int)value3);
}

void Shader::setInt3(const std::string &name, int value1, int value2, int value3)
{
	glUniform3i(glGetUniformLocation(this->programID, name.c_str()), value1, value2, value3);
}

void Shader::setFloat3(const std::string &name, float value1, float value2, float value3)
{
	glUniform3f(glGetUniformLocation(this->programID, name.c_str()), value1, value2, value3);
}

void Shader::setDouble3(const std::string &name, double value1, double value2, double value3)
{
	glUniform3d(glGetUniformLocation(this->programID, name.c_str()), value1, value2, value3);
}

// Pas de vectors de 4 posicions (float o double)
void Shader::setFloat4(const std::string &name, float value1, float value2, float value3, float value4)
{
	glUniform4f(glGetUniformLocation(this->programID, name.c_str()), value1, value2, value3, value4);
}

void Shader::setDouble4(const std::string &name, double value1, double value2, double value3, double value4)
{
	glUniform4d(glGetUniformLocation(this->programID, name.c_str()), value1, value2, value3, value4);
}

// Pas de Matrius 3x3 i 4x4
void Shader::setMatrix3fv(const std::string& name, glm::mat3 trans)
{
	glUniformMatrix3fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}

void Shader::setMatrix4fv(const std::string &name, glm::mat4 trans)
{
	glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}

/*****************************************************************************
* Use()
*      Activate shades in programID
*****************************************************************************/
void Shader::DeleteProgram()
{
	glDeleteProgram(this->programID);	// Don't leak the program.
}
