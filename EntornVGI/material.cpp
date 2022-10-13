//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb interfície MFC, llibreries GLM i OpenGL 3.3+
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2022)
// material.cpp : Funcions de definició de les propietats de reflectivitat dels objectes.

#include "stdafx.h"
#include "material.h"

// Tipus de materials que es seleccionen a la funció SeleccionaMaterial
MATERIAL  materials[MAX_MATERIALS] =
{
	{//Default
		{0.2f, 0.2f, 0.2f, 1.0f},	// Ambient
		{0.8f, 0.8f, 0.8f, 1.0f},	// Difusse
		{0.0f, 0.0f, 0.0f, 1.0f},	// Specular
		{0.0f, 0.0f, 0.0f, 1.0f},	// Emission
		0							// Shinniness
	},
	{//Material base
		{0, 0, 0.75f, 1},
		{0, 0, 0.75f, 1},
		{0.0f, 0.5f, 1, 1},
		{0, 0, 0, 1},
		20
	},

	{//Metall Obscur
		{0, 0, 0, 1},
		{0.5f, 0.5f, 0.5f, 1},
		{0.7f, 0.7f, 0.7f, 1},
		{0, 0, 0, 1},
		100
	},

	{//Vidre
		{0.2f, 0.65f, 0.9f, 0.5},
		{0.3f, 0.75f, 1, 0.5},
		{1, 1, 1, 1},
		{0, 0, 0, 1},
		50
	},

	{//Goma Negra
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0.3f, 0.3f, 0.3f, 1},
		{0, 0, 0, 1},
		20
	},

	{//Gespa
		{0, 0.2f, 0, 1},
		{0.1f, 0.5f, 0.2f, 1},
		{0.1f, 0.5f, 0.2f, 1},
		{0, 0, 0, 1},
		0
	},

	{//Metall Groc
		{0.3f, 0.3f, 0.1f, 1},
		{1, 1, 0.25f, 1},
		{1, 1, 0.5f, 1},
		{0, 0, 0, 1},
		128
	},

	{//Llum vermella
		{1, 0, 0, 1},
		{1, 0, 0, 1},
		{1, 0.5f, 0.5f, 1},
		{1, 0, 0, 1},
		128
	},

	{//Llum Groga
		{1.0f, 1.0f, 0, 1},
		{1, 1.0f, 0, 1},
		{1, 1.0f, 0, 1},
		{1, 0.8f, 0, 1},
		128
	},

	{//Llum verda
		{0, 1, 0, 1},
		{0, 1, 0, 1},
		{0.5f, 1, 0.5f, 1},
		{0, 1, 0, 1},
		128
	},

	{// Fitxer
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		{0, 0, 0, 1},
		0
	}

};


/******************************************************************************\
|* SeleccionaMaterial                                                         *|
|* Selecciona un dels materials del vector materials                          *|
\******************************************************************************/
void SeleccionaMaterial(GLuint sh_programID, int quin,bool sw_mater[5])
{
	//const float zeros[4]={0.0f,0.0f,0.0f,1.0f};
	MATERIAL material;

// Pas vector booleà sw_materials al Shader
	//glUniform4uiv(glGetUniformLocation(sh_programID, "sw_intensity"), 1, sw_mater);
	glUniform4i(glGetUniformLocation(sh_programID, "sw_intensity"), sw_mater[0], sw_mater[1], sw_mater[2], sw_mater[3]);
	glUniform1i(glGetUniformLocation(sh_programID, "sw_material"), sw_mater[4]);
	
	if ((quin >= 0) && (quin<MAX_MATERIALS))
		{	// Component d'emissió de llum del material
			//glColorMaterial(GL_FRONT, GL_EMISSION);
			if (sw_mater[0]) {	//glColor4fv(materials[quin].emission);
								//glMaterialfv(GL_FRONT, GL_EMISSION, materials[quin].emission);
								material.emission[0] = materials[quin].emission[0];
								material.emission[1] = materials[quin].emission[1];
								material.emission[2] = materials[quin].emission[2];
								material.emission[3] = materials[quin].emission[3];
							}
				else {	//glColor4fv(zeros);
						//glMaterialfv(GL_FRONT, GL_EMISSION, zeros);
						material.emission[0] = 0.0f;
						material.emission[1] = 0.0f;
						material.emission[2] = 0.0f;
						material.emission[3] = 1.0f;
					}
			// Pas component emission del material al Shader
			//glUniform4fv(glGetUniformLocation(sh_programID, "material.emission"), 1, material.emission);
			glUniform4f(glGetUniformLocation(sh_programID, "material.emission"), material.emission[0], material.emission[1], material.emission[2], material.emission[3]);

			// Coeficient de reflectivitat ambient del material
			//glColorMaterial(GL_FRONT, GL_AMBIENT);
			if (sw_mater[1]) {	//glColor4fv(materials[quin].ambient);
								//glMaterialfv(GL_FRONT, GL_AMBIENT, materials[quin].ambient);
								material.ambient[0] = materials[quin].ambient[0];
								material.ambient[1] = materials[quin].ambient[1];
								material.ambient[2] = materials[quin].ambient[2];
								material.ambient[3] = materials[quin].ambient[3];
							}
				else {	//glColor4fv(zeros);
						//glMaterialfv(GL_FRONT, GL_AMBIENT, materials[quin].ambient);
						material.ambient[0] = 0.0f;
						material.ambient[1] = 0.0f;
						material.ambient[2] = 0.0f;
						material.ambient[3] = 1.0f;
					}
			// Pas component ambient del material al Shader
			//glUniform4fv(glGetUniformLocation(sh_programID, "material.ambient"), 1, material.ambient);
			glUniform4f(glGetUniformLocation(sh_programID, "material.ambient"), material.ambient[0], material.ambient[1], material.ambient[2], material.ambient[3]);

			// Coeficient de reflectivitat difusa del material
			//glColorMaterial(GL_FRONT, GL_DIFFUSE);
			if (sw_mater[2]) {	//glColor4fv(materials[quin].diffuse);
								//glMaterialfv(GL_FRONT, GL_DIFFUSE, materials[quin].diffuse);	// Coef. Reflexió Difusa
								material.diffuse[0] = materials[quin].diffuse[0];
								material.diffuse[1] = materials[quin].diffuse[1];
								material.diffuse[2] = materials[quin].diffuse[2];
								material.diffuse[3] = materials[quin].diffuse[3];
							}
				else {	//glColor4fv(zeros);
						//glMaterialfv(GL_FRONT, GL_DIFFUSE, zeros);
						material.diffuse[0] = 0.0f;
						material.diffuse[1] = 0.0f;
						material.diffuse[2] = 0.0f;
						material.diffuse[3] = 1.0f;
					}
			// Pas component diffuse del material al Shader
			//glUniform4fv(glGetUniformLocation(sh_programID, "material.diffuse"), 1, material.diffuse);
			glUniform4f(glGetUniformLocation(sh_programID, "material.diffuse"), material.diffuse[0], material.diffuse[1], material.diffuse[2], material.diffuse[3]);

			// Coeficient de reflectivitat especular del material
			//glColorMaterial(GL_FRONT, GL_SPECULAR);
			if (sw_mater[3]){	//glColor4fv(materials[quin].specular);
								//glMaterialfv(GL_FRONT, GL_SPECULAR, materials[quin].specular);	// Coef. Reflexió Especular
								//glMaterialf(GL_FRONT, GL_SHININESS, materials[quin].shininess);
								material.specular[0] = materials[quin].specular[0];
								material.specular[1] = materials[quin].specular[1];
								material.specular[2] = materials[quin].specular[2];
								material.specular[3] = materials[quin].specular[3];
								material.shininess = materials[quin].shininess;
							}
				else {	//glColor4fv(zeros);
						//glMaterialfv(GL_FRONT, GL_SPECULAR, zeros);
						//glMaterialf(GL_FRONT, GL_SHININESS, 0);
						material.specular[0] = 0.0f;
						material.specular[1] = 0.0f;
						material.specular[2] = 0.0f;
						material.specular[3] = 1.0f;
						material.shininess = 0.0f;
					}
	// Pas component specular i shininess del material al Shader
		//glUniform4fv(glGetUniformLocation(programID, "material.specular"), 1, material.specular);
		glUniform4f(glGetUniformLocation(sh_programID, "material.specular"), material.specular[0], material.specular[1], material.specular[2], material.specular[3]);
		glUniform1f(glGetUniformLocation(sh_programID, "material.shininess"), material.shininess);
	}
}


/******************************************************************************\
|* SeleccionaColorMaterial                                                         *|
|* Selecciona el Color del material                                           *|
\******************************************************************************/
void SeleccionaColorMaterial(GLuint sh_programID, CColor c_obj, bool sw_mater[5])
{
	//const float zeros[4]={0.0f,0.0f,0.0f,1.0f};
	//float color[4] = { c_obj.r, c_obj.g, c_obj.b, c_obj.a };
	MATERIAL material;

//	glColor4fv(color);	//glColor3f(c_obj.r, c_obj.g, c_obj.b);
	SetColor4d(c_obj.r, c_obj.g, c_obj.b, c_obj.a);		// Definir color a primitives GLUT.

// Pas vector booleans sw_materials al Shader
	glUniform4i(glGetUniformLocation(sh_programID, "sw_intensity"), sw_mater[0], sw_mater[1], sw_mater[2], sw_mater[3]);
	glUniform1i(glGetUniformLocation(sh_programID, "sw_material"), sw_mater[4]);

// Component de reflectivitat d'emissió de llum del material
//	glColorMaterial(GL_FRONT, GL_EMISSION);
	if (sw_mater[0]) {	//glColor4fv(color);
						//glMaterialfv(GL_FRONT, GL_EMISSION, color);
						material.emission[0] = c_obj.r;	
						material.emission[1] = c_obj.g;	
						material.emission[2] = c_obj.b;	
						material.emission[3] = c_obj.a;
					}
		else {	//glColor4fv(zeros);
				//glMaterialfv(GL_FRONT, GL_EMISSION, zeros);
				material.emission[0] = 0.0f;
				material.emission[1] = 0.0f;
				material.emission[2] = 0.0f;
				material.emission[3] = 1.0f;
			}
// Pas component emission del material al Shader
	//glUniform4fv(glGetUniformLocation(sh_programID, "material.emission"), 1, material.emission);
	glUniform4f(glGetUniformLocation(sh_programID, "material.emission"), material.emission[0], material.emission[1], material.emission[2], material.emission[3]);

// Component de reflectivitat ambient del material
//	glColorMaterial(GL_FRONT, GL_AMBIENT);
	if (sw_mater[1]) {	//glColor4fv(color);
						//glMaterialfv(GL_FRONT, GL_AMBIENT, color);
						material.ambient[0] = c_obj.r;
						material.ambient[1] = c_obj.g;
						material.ambient[2] = c_obj.b;
						material.ambient[3] = c_obj.a;
					}
		else {	//glColor4fv(zeros);
				//glMaterialfv(GL_FRONT, GL_AMBIENT, zeros);
				material.ambient[0] = 0.0f;
				material.ambient[1] = 0.0f;
				material.ambient[2] = 0.0f;
				material.ambient[3] = 1.0f;
			}

// Pas component ambient del material al Shader
	//glUniform4fv(glGetUniformLocation(sh_programID, "material.ambient"), 1, material.ambient);
	glUniform4f(glGetUniformLocation(sh_programID, "material.ambient"), material.ambient[0], material.ambient[1], material.ambient[2], material.ambient[3]);

// Component de reflectivitat difusa del material
//	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	if (sw_mater[2]) {	//glColor4fv(color);
						//glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
						material.diffuse[0] = c_obj.r;
						material.diffuse[1] = c_obj.g;
						material.diffuse[2] = c_obj.b;
						material.diffuse[3] = c_obj.a;
					}
		else {	//glColor4fv(zeros);
				//glMaterialfv(GL_FRONT, GL_DIFFUSE, zeros);
				material.diffuse[0] = 0.0f;
				material.diffuse[1] = 0.0f;
				material.diffuse[2] = 0.0f;
				material.diffuse[3] = 1.0f;
			}

// Pas component diffuse del material al Shader
	//glUniform4fv(glGetUniformLocation(sh_programID, "material.diffuse"), 1, material.diffuse);
	glUniform4f(glGetUniformLocation(sh_programID, "material.diffuse"), material.diffuse[0], material.diffuse[1], material.diffuse[2], material.diffuse[3]);

// Component de reflectivitat especular del material
//	glColorMaterial(GL_FRONT, GL_SPECULAR);
	if (sw_mater[3]) {	//glColor4fv(color);
						//glMaterialfv(GL_FRONT, GL_SPECULAR, color);
						//glMaterialf(GL_FRONT, GL_SHININESS, 128);
						material.specular[0] = c_obj.r;
						material.specular[1] = c_obj.g;
						material.specular[2] = c_obj.b;
						material.specular[3] = c_obj.a;
						material.shininess = 32.0f;
					}
		else {	//glColor4fv(zeros);
				//glMaterialfv(GL_FRONT, GL_SPECULAR, zeros);
				//glMaterialf(GL_FRONT, GL_SHININESS, 0);
				material.specular[0] = 0.0f;
				material.specular[1] = 0.0f;
				material.specular[2] = 0.0f;
				material.specular[3] = 1.0f;
				material.shininess = 0.0f;
			}
// Pas component specular i shininess del material al Shader
	//glUniform4fv(glGetUniformLocation(sh_programID, "material.specular"), 1, material.specular);
	glUniform4f(glGetUniformLocation(sh_programID, "material.specular"), material.specular[0], material.specular[1], material.specular[2], material.specular[3]);
	glUniform1f(glGetUniformLocation(sh_programID, "material.shininess"), material.shininess);
}


// llegir_Material: Llegir propietats de material de d'un fitxer segons l'estructura MATERIAL
//			Retorna l'estructura MATERIAL llegida i un booleà si s'ha trobat el fitxer [TRUE] o no [FALSE].
bool llegir_Material(GLuint sh_programID, char* filename,int index)
{ 
	FILE *fd;
	char strMessage[255] = { 0 };
	GLfloat auxemis[4],auxamb[4],auxdiff[4],auxspec[4];
	GLfloat auxshin;

// Inicialitzar variable
	auxamb[0]=0.0f;			auxamb[1]=0.0f;			auxamb[2]=0.0f;			auxamb[3]=1.0f;
	auxemis[0]=0.0f;		auxemis[1]=0.0f;		auxemis[2]=0.0f;		auxemis[3]=1.0f;
	auxdiff[0]=0.0f,		auxdiff[1]=0.0f;		auxdiff[2]=0.0f;		auxdiff[3]=1.0f;
	auxspec[0]=0.0f;		auxspec[1]=0.0f;		auxspec[2]=0.0f;		auxspec[3]=1.0f;
	auxshin=0.0f;

	//if ( (fd=fopen(nomf,"rt"))==NULL)			// VS6.0
	if ( (fopen_s(&fd,filename,"rt"))!=0)		// VS2005
		{	fprintf(stderr, "ERROR: Unable to find the file: %s!", filename);
			return false;
		}

	fscanf(fd,"%f %f %f %f \n",&auxemis[0],&auxemis[1],&auxemis[2],&auxemis[3]);
	fscanf(fd,"%f %f %f %f \n",&auxamb[0],&auxamb[1],&auxamb[2],&auxamb[3]);
	fscanf(fd,"%f %f %f %f \n",&auxdiff[0],&auxdiff[1],&auxdiff[2],&auxdiff[3]);
	fscanf(fd,"%f %f %f %f \n",&auxspec[0],&auxspec[1],&auxspec[2],&auxspec[3]);
	fscanf(fd,"%f \n",&auxshin);

	fclose(fd);

	materials[index].emission[0]=auxemis[0];	materials[index].emission[1]=auxemis[1];
	materials[index].emission[2]=auxemis[2];	materials[index].emission[3]=auxemis[3];
// Pas component emission del material al Shader
	//glUniform4fv(glGetUniformLocation(sh_programID, "material.emission"), 1, materials[index].emission);
	glUniform4f(glGetUniformLocation(sh_programID, "material.emission"), materials[index].emission[0], materials[index].emission[1], materials[index].emission[2], materials[index].emission[3]);

	materials[index].ambient[0]=auxamb[0];		materials[index].ambient[1]=auxamb[1];
	materials[index].ambient[2]=auxamb[2];		materials[index].ambient[3]=auxamb[3];
// Pas component ambient del material al Shader
	//glUniform4fv(glGetUniformLocation(sh_programID, "material.ambient"), 1, materials[index].ambient);
	glUniform4f(glGetUniformLocation(sh_programID, "material.ambient"), materials[index].ambient[0], materials[index].ambient[1], materials[index].ambient[2], materials[index].ambient[3]);
	
	materials[index].diffuse[0]=auxdiff[0];		materials[index].diffuse[1]=auxdiff[1];
	materials[index].diffuse[2]=auxdiff[2];		materials[index].diffuse[3]=auxdiff[3];
	// Pas component diffuse del material al Shader
	//glUniform4fv(glGetUniformLocation(sh_programID, "material.diffuse"), 1, materials[index].diffuse);
	glUniform4f(glGetUniformLocation(sh_programID, "material.diffuse"), materials[index].diffuse[0], materials[index].diffuse[1], materials[index].diffuse[2], materials[index].diffuse[3]);

	materials[index].specular[0]=auxspec[0];	materials[index].specular[1]=auxspec[1];
	materials[index].specular[2]=auxspec[2];	materials[index].specular[3]=auxspec[3];
	materials[index].shininess=auxshin;
// Pas component specular i shininess del material al Shader
	//glUniform4fv(glGetUniformLocation(sh_programID, "material.specular"), 1, materials[index].specular);
	glUniform4f(glGetUniformLocation(sh_programID, "material.specular"), materials[index].specular[0], materials[index].specular[1], materials[index].specular[2], materials[index].specular[3]);
	glUniform1f(glGetUniformLocation(sh_programID, "material.shininess"), materials[index].shininess);

	return true;
}
