//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb interfície MFC, llibreries GLM i OpenGL 3.3+
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2022)
// visualitzacio.cpp : Funcions de visualització i pantalla
// FUNCIONS:	- Iluminació (Iluminacio)
//				- Visualització Ortogràfica (ProjeccioOrto i Ortografica)
//				- Visualització Perspectiva amb coord. esfèriques (ProjeccioPerspectiva(),Vista_Esferica())
//              - Visualització Perspectiva amb navegació per tecles cursor (ProjeccioPerspectiva(),Vista_Navega())
//				- Dibuixar els eixos (deixos)
//				- Incorporar plans de Grid2D (XY, XZ, YZ) i un Grid 3D (grid*())
//				- Dibuixar fons: blanc (FonsW) i negre (Fons)
//				- Carregar imatges de formats compatibles amb DevIL/OpenIL com a textura (loadIMA())
//				- Inicialitzar imatges textura per l'objecte Truck (Init_Textures())
//

#include "stdafx.h"

// Entorn V3D. QUATERNIONS: Include per a la definició del tipus GL_QUAT i crida a les funcions de quatern.
#include "quatern.h"

#include "visualitzacio.h"
#include "escena.h"

// Iluminació: Configurar iluminació de l'escena
void Iluminacio(GLint sh_programID, char ilumin, bool ifix, bool ilu2sides, bool ll_amb, LLUM* lumin, char obj, bool frnt_fcs,
	bool bc_lin, int step)
{
	//bool ll_amb=true;
	GLdouble angv, angh;

	// Configuració de la font de llum LIGHT0
	GLfloat position[] = { 0.0,0.0,200.0,1.0 };
	GLfloat especular[] = { 0.0,0.0,0.0,1.0 };
	GLfloat ambientg[] = { .5,.5,.5,1.0 };

// Definició de llum ambient segons booleana ll_amb
//	if (ll_amb) glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientg);
//		else glLightModelfv(GL_LIGHT_MODEL_AMBIENT,especular);
	if (ll_amb) glUniform4f(glGetUniformLocation(sh_programID, "LightModelAmbient"), ambientg[0], ambientg[1], ambientg[2], ambientg[3]);
	else glUniform4f(glGetUniformLocation(sh_programID, "LightModelAmbient"), especular[0], especular[1], especular[2], especular[3]);

// Passar llum fixe a Coordenades Món o llum lligada a càmera
	glUniform1i(glGetUniformLocation(sh_programID, "fixedLight"), ifix);

// Llum #0 - (+Z)
// Conversió angles graus -> radians
	angv = lumin[0].posicio.alfa * PI / 180;
	angh = lumin[0].posicio.beta * PI / 180;

// Conversió Coord. esfèriques -> Coord. cartesianes per a la posició de la llum
	position[0] = lumin[0].posicio.R * cos(angh) * cos(angv);
	position[1] = lumin[0].posicio.R * sin(angh) * cos(angv);
	position[2] = lumin[0].posicio.R * sin(angv);
	position[3]=1.0;
	//	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[0].position"), position[0], position[1], position[2], position[3]);

	//	glLightfv(GL_LIGHT0, GL_DIFFUSE, lumin[0].difusa);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[0].diffuse"), lumin[0].difusa[0], lumin[0].difusa[1], lumin[0].difusa[2], lumin[0].difusa[3]);
	//	glLightfv(GL_LIGHT0, GL_SPECULAR, lumin[0].especular);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[0].specular"), lumin[0].especular[0], lumin[0].especular[1], lumin[0].especular[2], lumin[0].especular[3]);

	// Coeficients factor atenuació f_att=1/(ad2+bd+c)
	//	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lumin[0].atenuacio.c);
	//	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, lumin[0].atenuacio.b);
	//	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lumin[0].atenuacio.a);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[0].attenuation"), lumin[0].atenuacio.a, lumin[0].atenuacio.b, lumin[0].atenuacio.c);

	//	if (lumin[0].restringida) {	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lumin[0].spotdirection);
	//								glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, lumin[0].spotcutoff);
	//								glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, lumin[0].spotexponent);
	//							}
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[0].restricted"), lumin[0].restringida);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[0].spotDirection"), lumin[0].spotdirection[0], lumin[0].spotdirection[1], lumin[0].spotdirection[2]);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[0].spotCosCutoff"), lumin[0].spotcoscutoff);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[0].spotExponent"), lumin[0].spotexponent);
	//	glEnable(GL_LIGHT0);	//	else glDisable(GL_LIGHT0);
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[0].sw_light"), lumin[0].encesa);

// Llum #1 (+X)
// Conversió angles graus -> radians
	angv = lumin[1].posicio.alfa*PI / 180;
	angh = lumin[1].posicio.beta*PI / 180;

// Conversió Coord. esfèriques -> Coord. cartesianes per a la posició de la llum
	position[0] = lumin[1].posicio.R*cos(angh)*cos(angv);
	position[1] = lumin[1].posicio.R*sin(angh)*cos(angv);
	position[2] = lumin[1].posicio.R*sin(angv);
	position[3] = 1.0;
	//glLightfv(GL_LIGHT1, GL_POSITION, position);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[1].position"), position[0], position[1], position[2], position[3]);

//	glLightfv(GL_LIGHT1, GL_DIFFUSE, lumin[1].difusa);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[1].diffuse"), lumin[0].difusa[1], lumin[1].difusa[1], lumin[1].difusa[2], lumin[1].difusa[3]);
//	glLightfv(GL_LIGHT1, GL_SPECULAR, lumin[1].especular);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[1].specular"), lumin[1].especular[0], lumin[1].especular[1], lumin[1].especular[2], lumin[1].especular[3]);

// Coeficients factor atenuació f_att=1/(ad2+bd+c)
//	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, lumin[1].atenuacio.c);
//	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, lumin[1].atenuacio.b);
//	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, lumin[1].atenuacio.a);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[1].attenuation"), lumin[1].atenuacio.a, lumin[1].atenuacio.b, lumin[1].atenuacio.c);

	//	if (lumin[1].restringida) {	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lumin[1].spotdirection);
	//								glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, lumin[1].spotcutoff);
	//								glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, lumin[1].spotexponent);
	//							}
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[1].restricted"), lumin[1].restringida);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[1].spotDirection"), lumin[1].spotdirection[0], lumin[1].spotdirection[1], lumin[1].spotdirection[2]);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[1].spotCosCutoff"), lumin[1].spotcoscutoff);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[1].SpotExponent"), lumin[1].spotexponent);

	//	glEnable(GL_LIGHT1);	//	glDisable(GL_LIGHT1);
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[1].sw_light"), lumin[1].encesa);

// Llum #2 (+Y)
// Conversió angles graus -> radians
	angv = lumin[2].posicio.alfa*PI / 180;
	angh = lumin[2].posicio.beta*PI / 180;
				
// Conversió Coord. esfèriques -> Coord. cartesianes
	position[0] = lumin[2].posicio.R*cos(angh)*cos(angv);
	position[1] = lumin[2].posicio.R*sin(angh)*cos(angv);
	position[2] = lumin[2].posicio.R*sin(angv);
	position[3] = 1.0;
	//	glLightfv(GL_LIGHT2, GL_POSITION, position);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[2].position"), position[0], position[1], position[2], position[3]);

	//	glLightfv(GL_LIGHT2, GL_DIFFUSE, lumin[2].difusa);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[2].diffuse"), lumin[2].difusa[0], lumin[2].difusa[1], lumin[2].difusa[2], lumin[2].difusa[3]);
	//	glLightfv(GL_LIGHT2, GL_SPECULAR, lumin[2].especular);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[2].specular"), lumin[2].especular[0], lumin[2].especular[1], lumin[2].especular[2], lumin[2].especular[3]);

// Coeficients factor atenuació f_att=1/(ad2+bd+c)
	//	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, lumin[2].atenuacio.c);
	//	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, lumin[2].atenuacio.b);
	//	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, lumin[2].atenuacio.a);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[2].attenuation"), lumin[2].atenuacio.a, lumin[2].atenuacio.b, lumin[2].atenuacio.c);

	//	if (lumin[2].restringida) {	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lumin[2].spotdirection);
	//								glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lumin[2].spotcutoff);
	//								glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, lumin[2].spotexponent);
	//							}
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[2].restricted"), lumin[2].restringida);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[2].spotDirection"), lumin[2].spotdirection[0], lumin[2].spotdirection[1], lumin[2].spotdirection[2]);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[2].spotCosCutoff"), lumin[2].spotcoscutoff);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[2].spotExponent"), lumin[2].spotexponent);

	//	glEnable(GL_LIGHT2);	//	glDisable(GL_LIGHT2);
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[2].sw_light"), lumin[2].encesa);

// Llum #3 (Z=Y=X)
// Conversió angles graus -> radians
	angv = lumin[3].posicio.alfa*PI / 180;
	angh = lumin[3].posicio.beta*PI / 180;

// Conversió Coord. esfèriques -> Coord. cartesianes
	position[0] = lumin[3].posicio.R*cos(angh)*cos(angv);
	position[1] = lumin[3].posicio.R*sin(angh)*cos(angv);
	position[2] = lumin[3].posicio.R*sin(angv);
	position[3] = 1.0;
	//glLightfv(GL_LIGHT3, GL_POSITION, position);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[3].position"), position[0], position[1], position[2], position[3]);

	//	glLightfv(GL_LIGHT3, GL_DIFFUSE, lumin[3].difusa);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[3].diffuse"), lumin[3].difusa[0], lumin[3].difusa[1], lumin[3].difusa[2], lumin[3].difusa[3]);
	//	glLightfv(GL_LIGHT3, GL_SPECULAR, lumin[3].especular);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[3].specular"), lumin[3].especular[0], lumin[3].especular[1], lumin[3].especular[2], lumin[3].especular[3]);

// Coeficients factor atenuació f_att=1/(ad2+bd+c)
	//	glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, lumin[3].atenuacio.c);
	//	glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, lumin[3].atenuacio.b);
	//	glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, lumin[3].atenuacio.a);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[3].attenuation"), lumin[3].atenuacio.a, lumin[3].atenuacio.b, lumin[3].atenuacio.c);

	//	if (lumin[3].restringida) {	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lumin[3].spotdirection);
	//								glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, lumin[3].spotcutoff);
	//								glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, lumin[3].spotexponent);
	//							}
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[3].restricted"), lumin[3].restringida);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[3].spotDirection"), lumin[3].spotdirection[0], lumin[3].spotdirection[1], lumin[3].spotdirection[2]);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[3].spotCosCutoff"), lumin[3].spotcoscutoff);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[3].spotExponent"), lumin[3].spotexponent);

	//	glEnable(GL_LIGHT3);	// glDisable(GL_LIGHT3);
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[3].sw_light"), lumin[3].encesa);

// Llum #4 (-Z)
// Conversió angles graus -> radians
	angv = lumin[4].posicio.alfa*PI / 180;
	angh = lumin[4].posicio.beta*PI / 180;
				
// Conversió Coord. esfèriques -> Coord. cartesianes
	position[0] = lumin[4].posicio.R*cos(angh)*cos(angv);
	position[1] = lumin[4].posicio.R*sin(angh)*cos(angv);
	position[2] = lumin[4].posicio.R*sin(angv);
	position[3] = 1.0;
	//glLightfv(GL_LIGHT4, GL_POSITION, position);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[4].position"), position[0], position[1], position[2], position[3]);

	//	glLightfv(GL_LIGHT4, GL_DIFFUSE, lumin[4].difusa);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[4].diffuse"), lumin[4].difusa[0], lumin[4].difusa[1], lumin[4].difusa[2], lumin[4].difusa[3]);
	//	glLightfv(GL_LIGHT4, GL_SPECULAR, lumin[4].especular);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[4].specular"), lumin[4].especular[0], lumin[4].especular[1], lumin[4].especular[2], lumin[4].especular[3]);

// Coeficients factor atenuació f_att=1/(ad2+bd+c)
	//	glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, lumin[4].atenuacio.c);
	//	glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, lumin[4].atenuacio.b);
	//	glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, lumin[4].atenuacio.a);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[4].attenuation"), lumin[4].atenuacio.a, lumin[4].atenuacio.b, lumin[4].atenuacio.c);

	//	if (lumin[4].restringida) {	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lumin[4].spotdirection);
	//								glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, lumin[4].spotcutoff);
	//								glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, lumin[4].spotexponent);
	//							}
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[4].restricted"), lumin[4].restringida);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[4].spotDirection"), lumin[4].spotdirection[0], lumin[4].spotdirection[1], lumin[4].spotdirection[2]);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[4].spotCosCutoff"), lumin[4].spotcoscutoff);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[4].spotExponent"), lumin[4].spotexponent);

	//	glEnable(GL_LIGHT4);	//	else glDisable(GL_LIGHT4);
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[4].sw_light"), lumin[4].encesa);

// Llum #5
// Conversió angles graus -> radians
	angv = lumin[5].posicio.alfa * PI / 180;
	angh = lumin[5].posicio.beta * PI / 180;

// Conversió Coord. esfèriques -> Coord. cartesianes
	position[0] = lumin[5].posicio.R * cos(angh) * cos(angv);
	position[1] = lumin[5].posicio.R * sin(angh) * cos(angv);
	position[2] = lumin[5].posicio.R * sin(angv);
	position[3] = 1.0;
	//glLightfv(GL_LIGHT5, GL_POSITION, position);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[5].position"), position[0], position[1], position[2], position[3]);

	//	glLightfv(GL_LIGHT5, GL_DIFFUSE, lumin[5].difusa);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[5].diffuse"), lumin[5].difusa[0], lumin[5].difusa[1], lumin[5].difusa[2], lumin[5].difusa[3]);
	//	glLightfv(GL_LIGHT5, GL_SPECULAR, lumin[5].especular);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[5].specular"), lumin[5].especular[0], lumin[5].especular[1], lumin[5].especular[2], lumin[5].especular[3]);

// Coeficients factor atenuació f_att=1/(ad2+bd+c)
	//	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, lumin[5].atenuacio.c);
	//	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, lumin[5].atenuacio.b);
	//	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, lumin[5].atenuacio.a);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[5].attenuation"), lumin[5].atenuacio.a, lumin[5].atenuacio.b, lumin[5].atenuacio.c);

	//	if (lumin[5].restringida) {	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, lumin[5].spotdirection);
	//								glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, lumin[5].spotcoscutoff);
	//								glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, lumin[5].spotexponent);
	//							}
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[5].restricted"), lumin[5].restringida);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[5].spotDirection"), lumin[5].spotdirection[0], lumin[5].spotdirection[1], lumin[5].spotdirection[2]);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[5].spotCosCutoff"), lumin[5].spotcoscutoff);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[5].spotExponent"), lumin[5].spotexponent);
//	else glDisable(GL_LIGHT5);
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[5].sw_light"), lumin[5].encesa);

// Llum #6 
// Conversió angles graus -> radians
	angv = lumin[6].posicio.alfa * PI / 180;
	angh = lumin[6].posicio.beta * PI / 180;

// Conversió Coord. esfèriques -> Coord. cartesianes
	position[0] = lumin[6].posicio.R * cos(angh) * cos(angv);
	position[1] = lumin[6].posicio.R * sin(angh) * cos(angv);
	position[2] = lumin[6].posicio.R * sin(angv);
	position[3] = 1.0;
	//	glLightfv(GL_LIGHT6, GL_POSITION, position);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[6].position"), position[0], position[1], position[2], position[3]);

	//	glLightfv(GL_LIGHT6, GL_DIFFUSE, lumin[6].difusa);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[6].diffuse"), lumin[6].difusa[0], lumin[6].difusa[1], lumin[6].difusa[2], lumin[6].difusa[3]);
	//	glLightfv(GL_LIGHT6, GL_SPECULAR, lumin[6].especular);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[6].specular"), lumin[6].especular[0], lumin[6].especular[1], lumin[6].especular[2], lumin[6].especular[3]);

// Coeficients factor atenuació f_att=1/(ad2+bd+c)
	//	glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, lumin[6].atenuacio.c);
	//	glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, lumin[6].atenuacio.b);
	//	glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, lumin[6].atenuacio.a);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[6].attenuation"), lumin[6].atenuacio.a, lumin[6].atenuacio.b, lumin[6].atenuacio.c);

	//	if (lumin[6].restringida) {	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, lumin[6].spotdirection);
	//								glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, lumin[6].cutoff);
	//								glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, lumin[6].exponent);
	//							}
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[6].restricted"), lumin[6].restringida);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[6].spotDirection"), lumin[6].spotdirection[0], lumin[6].spotdirection[1], lumin[6].spotdirection[2]);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[6].spotCosCutoff"), lumin[6].spotcoscutoff);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[6].spotExponent"), lumin[6].spotexponent);
	
	// glEnable(GL_LIGHT6)	//	glDisable(GL_LIGHT6);
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[6].sw_light"), lumin[6].encesa);

// Llum #7 
// Conversió angles graus -> radians
	angv = lumin[7].posicio.alfa * PI / 180;
	angh = lumin[7].posicio.beta * PI / 180;

// Conversió Coord. esfèriques -> Coord. cartesianes
	position[0] = lumin[7].posicio.R * cos(angh) * cos(angv);
	position[1] = lumin[7].posicio.R * sin(angh) * cos(angv);
	position[2] = lumin[7].posicio.R * sin(angv);
	position[3] = 1.0;
	//glLightfv(GL_LIGHT7, GL_POSITION, position);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[7].position"), position[0], position[1], position[2], position[3]);

	//	glLightfv(GL_LIGHT7, GL_DIFFUSE, lumin[7].difusa);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[7].diffuse"), lumin[7].difusa[0], lumin[7].difusa[1], lumin[7].difusa[2], lumin[7].difusa[3]);
	//	glLightfv(GL_LIGHT7, GL_SPECULAR, lumin[7].especular);
	glUniform4f(glGetUniformLocation(sh_programID, "LightSource[7].specular"), lumin[7].especular[0], lumin[7].especular[1], lumin[7].especular[2], lumin[7].especular[3]);

// Coeficients factor atenuació f_att=1/(ad2+bd+c)
	//	glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, lumin[7].atenuacio.c);
	//	glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, lumin[7].atenuacio.b);
	//	glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, lumin[7].atenuacio.a);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[7].attenuation"), lumin[7].atenuacio.a, lumin[7].atenuacio.b, lumin[7].atenuacio.c);

	//	if (lumin[5].restringida) {	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, lumin[7].spotdirection);
	//								glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, lumin[7].cutoff);
	//								glLightf(GL_LIGHt7, GL_SPOT_EXPONENT, lumin[7].exponent);
	//							}
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[7].restricted"), lumin[7].restringida);
	glUniform3f(glGetUniformLocation(sh_programID, "LightSource[7].spotDirection"), lumin[7].spotdirection[0], lumin[7].spotdirection[1], lumin[7].spotdirection[2]);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[7].spotCosCutoff"), lumin[7].spotcoscutoff);
	glUniform1f(glGetUniformLocation(sh_programID, "LightSource[7].spotExponent"), lumin[7].spotexponent);

	//	glEnable(GL_LIGHT7);	//	glDisable(GL_LIGHT7);
	glUniform1i(glGetUniformLocation(sh_programID, "LightSource[7].sw_light"), lumin[7].encesa);

	//glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

// Selecció del model d'iluminació segons variable ilumin
	switch(ilumin)
	{
	case PUNTS:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;

	case FILFERROS:
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		break;

    case PLANA:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
// PRÀCTICA 3: Crida a la funció pel càlcul dels vectors normals normals a les cares si l'objecte és un fractal
   
// Il.luminació per cares planes
		glShadeModel(GL_FLAT); 
		break;

	case GOURAUD:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
// PRÀCTICA 3: Crida a la funció pel càlcul dels vectors normals normals a les cares si l'objecte és un fractal

// Il.luminació suau 
        glShadeModel(GL_SMOOTH); 
		break;

	case PHONG:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// Il.luminació suau 
		glShadeModel(GL_SMOOTH);
		break;
	}
}


// -------- Entorn VGI: ORTOGRÀFICA (Funcions Projeccio_Orto i Vista_Ortografica)

// Projeccio_Orto: Definició Viewport i glOrtho 
// ---- Entorn VGI: ATENCIÓ!!. CAL DEFINIR PARÀMETRES DE LA FUNCIÓ
glm::mat4 Projeccio_Orto()
{   
	glm::dmat4 MatriuProjeccio(1.0);
// ---- Entorn VGI: ATENCIÓ!!. ESPECIFICACIO DELS PARÀMETRES DE PROJECCIÓ ORTOGRÀFICA
//			        QUE ES CARREGUEN A LA MATRIU DE PROJECCIÓ GL_PROJECTION
	return MatriuProjeccio;
}

// Vista_Ortogràfica: Ilumina i dibuixa l'escena. Crida a la funció gluLookAt segons la variable prj 
//				(planta, alçat, perfil o axonometrica).
glm::mat4 Vista_Ortografica(GLuint sh_programID, int prj,GLdouble Raux,CColor col_fons,CColor col_object,char objecte,GLdouble mida,int step,
				bool frnt_fcs, bool oculta, bool testv, bool bck_ln, 
				char iluminacio, bool llum_amb, LLUM* lumi, bool ifix, bool il2sides, 
				bool eix, CMask3D reixa, CPunt3D hreixa)
{
	glm::mat4 MatriuVista(1.0);

// Iluminacio movent-se amb la camara (abans gluLookAt)
	if (!ifix) Iluminacio(sh_programID, iluminacio, ifix, il2sides, llum_amb, lumi, objecte, frnt_fcs, bck_ln, step);

// Implementació de planta,alçat,perfil i isomètrica 
// ---- Entorn VGI: ATENCIÓ!!. ESPECIFICACIO DEL PUNT DE VISTA
//								Cal definir el punt de vista (gluLookAt) en funció del
//								tipus de projecció definit a la variable prj.

// Pas Matriu a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "viewMatrix"), 1, GL_FALSE, &MatriuVista[0][0]);

// Neteja dels buffers de color i profunditat
	Fons(col_fons);

// Iluminacio fixe respecte la camara (després gluLookAt)
	if (ifix) Iluminacio(sh_programID, iluminacio, ifix, il2sides, llum_amb, lumi, objecte, frnt_fcs, bck_ln, step);

// Test de Visibilitat
	if (testv) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

// Ocultacions (Z-buffer)
	if (oculta) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

// Dibuix de les cares back com a línies en Il.luminacio PLANA i SUAU
	if (bck_ln) glPolygonMode(GL_BACK, GL_LINE);

// Dibuix dels eixos
	//if (eix) glCallList(EIXOS);

// Dibuixa el grid actiu
	//if ((reixa.x) || (reixa.y) || (reixa.z) || (reixa.w)) draw_Grid(reixa, hreixa);
	
	return MatriuVista;
}

// -------- Entorn VGI: PERSPECTIVA (Funcions Projeccio_Perspectiva i Vista_Esferica)

// Projeccio_Perspectiva: Definició Viewport i gluPerspective
glm::mat4 Projeccio_Perspectiva(GLuint sh_programID, int minx,int miny,GLsizei w,GLsizei h,double zoom)
{
	glm::mat4 MatriuProjeccio(1.0);

// Definició Viewport
	glViewport(minx,miny,w,h);
	if (h==0) h=1;

// Activació i inicialització matriu PROJECTION
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();

// PROJECCIO PERSPECTIVA.Definim volum de visualització adaptant-lo 
//	 a les mides actuals de la finestra windows	

// Amb glm::Perspective
	//if (w>=h) 	gluPerspective(60.0,1.0*w/h,p_near,p_far+zoom);
		//else gluPerspective(60.0*h/w,1.0*w/h,p_near,p_far+zoom);
	if (w >= h) MatriuProjeccio = glm::perspective(glm::radians(60.0), 1.0 * w / h, p_near, p_far);
	else MatriuProjeccio = glm::perspective(glm::radians(60.0), 1.0 * w / h, p_near, p_far);

// Pas Matriu a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "projectionMatrix"), 1, GL_FALSE, &MatriuProjeccio[0][0]);

// Activació matriu MODELVIEW (tancar matriu PROJECTION)
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	return MatriuProjeccio;
}

// Vista_Esferica: Definició gluLookAt amb possibilitat de moure el punt de vista interactivament amb el ratolí, 
//					ilumina i dibuixa l'escena
glm::mat4 Vista_Esferica(GLuint sh_programID,CEsfe3D opv,char VPol,bool pant,CPunt3D tr,CPunt3D trF,
				 CColor col_fons,CColor col_object,char objecte,double mida,int step, 
				 bool frnt_fcs, bool oculta, bool testv, bool bck_ln, 
				 char iluminacio, bool llum_amb, LLUM* lumi, bool ifix, bool il2sides,
				 bool eix, CMask3D reixa, CPunt3D hreixa)
{    
	GLdouble cam[3],up[3];
	glm::mat4 MatriuVista(1.0);
	// Matrius Traslació
	glm::mat4 TransMatrix(1.0);

// Conversió angles radians -> graus
   	opv.alfa=opv.alfa*PI/180;
    opv.beta=opv.beta*PI/180;

	if(opv.R<1.0) opv.R=1.0;

// Neteja dels buffers de color i profunditat
	Fons(col_fons);

// Posició càmera i vector cap amunt
	if (VPol==POLARZ) { cam[0]=opv.R*cos(opv.beta)*cos(opv.alfa);
						cam[1]=opv.R*sin(opv.beta)*cos(opv.alfa);
						cam[2]=opv.R*sin(opv.alfa);		
						up[0]=-cos(opv.beta)*sin(opv.alfa);
						up[1]=-sin(opv.beta)*sin(opv.alfa);
						up[2]=cos(opv.alfa);	}
		else if (VPol==POLARY) {	cam[0]=opv.R*sin(opv.beta)*cos(opv.alfa);
									cam[1]=opv.R*sin(opv.alfa);
									cam[2]=opv.R*cos(opv.beta)*cos(opv.alfa);		
									up[0]=-sin(opv.beta)*sin(opv.alfa);
									up[1]=cos(opv.alfa);
									up[2]=-cos(opv.beta)*sin(opv.alfa);		}
			else {	cam[0]=opv.R*sin(opv.alfa);
					cam[1]=opv.R*cos(opv.beta)*cos(opv.alfa);
					cam[2]=opv.R*sin(opv.beta)*cos(opv.alfa);		
					up[0]=cos(opv.alfa);
					up[1]=-cos(opv.beta)*sin(opv.alfa);
					up[2]=-sin(opv.beta)*sin(opv.alfa);		}

// Iluminacio movent-se amb la camara (abans glLookAt)
	if (!ifix) Iluminacio(sh_programID,iluminacio, ifix, il2sides, llum_amb, lumi, objecte, frnt_fcs, bck_ln, step);

// Opció pan: desplaçament del Centre de l'esfera (pant=1)
//	if (pant) glTranslatef(tr.x,tr.y,tr.z);
//	glTranslatef(trF.x,trF.y,trF.z);	// Traslació fixada amb la INSERT dins l'opció pan
	if (pant) TransMatrix = glm::translate(TransMatrix, vec3(tr.x, tr.y, tr.z));	//glTranslatef(tr.x, tr.y, tr.z);
	TransMatrix = glm::translate(TransMatrix, vec3(trF.x, trF.y, trF.z));	// Traslació fixada amb la INSERT dins l'opció pan

// Especificació del punt de vista
   //gluLookAt(cam[0],cam[1],cam[2],0.,0.,0.,up[0],up[1],up[2]);
   MatriuVista = glm::lookAt(
	   glm::vec3(cam[0], cam[1], cam[2]), // Camera is here
	   glm::vec3(0, 0, 0), // and looks here
	   glm::vec3(up[0], up[1], up[2])  // Head is up (set to 0,-1,0 to look upside-down)
	   );

   MatriuVista = TransMatrix * MatriuVista;

// Pas Matriu Vista a shader
   glUniformMatrix4fv(glGetUniformLocation(sh_programID, "viewMatrix"), 1, GL_FALSE, &MatriuVista[0][0]);

// Iluminacio fixe respecte la camara (després glLookAt)
   if (ifix) Iluminacio(sh_programID,iluminacio, ifix, il2sides, llum_amb, lumi, objecte, frnt_fcs, bck_ln, step);

// Test de Visibilitat
	if (testv) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

// Ocultacions (Z-buffer)
	if (oculta) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

// Dibuix de les cares back com a línies en Il.luminacio PLANA i SUAU (DEPRECAT per a GL 3.3)
	//if (bck_ln) glPolygonMode(GL_BACK, GL_LINE);

//  Dibuix dels eixos
	//if (eix) glCallList(EIXOS);

// Dibuixa el grid actiu
	//if ((reixa.x) || (reixa.y) || (reixa.z) || (reixa.w)) draw_Grid(reixa, hreixa);

	return MatriuVista;
}


//Vista_Navega: Definició gluLookAt directament per paràmetre, sense esfèriques.
//              amb possibilitat de moure el punt de vista interactivament amb les
//				tecles de cursor per poder navegar.
glm::mat4 Vista_Navega(GLuint sh_programID, CPunt3D pv,bool pvb,GLdouble n[3],GLdouble v[3],bool pant,CPunt3D tr,CPunt3D trF,
				  CColor col_fons,CColor col_object,char objecte,bool color, int step,
				  bool frnt_fcs, bool oculta, bool testv, bool bck_ln, 
				  char iluminacio, bool llum_amb, LLUM* lumi, bool ifix, bool il2sides,
				  bool eix, CMask3D reixa, CPunt3D hreixa)
{   double altfar=0;
	glm::mat4 MatriuVista(1.0);

// Neteja dels buffers de color i profunditat
	Fons(col_fons);

// Iluminacio movent-se amb la camara (abans glLookAt)
	if (!ifix) Iluminacio(sh_programID,iluminacio, ifix, il2sides, llum_amb, lumi, objecte, frnt_fcs, bck_ln, step);

// Opció pan: desplaçament del Centre de l'esfera (pant=true)
	if (pant) glTranslatef(tr.x,tr.y,tr.z);
	glTranslatef(trF.x,trF.y,trF.z);	// Traslació fixada amb la INSERT dins l'opció pan

// Especificació del punt de vista
	//gluLookAt(pv.x,pv.y,pv.z,n[0],n[1],n[2],v[0],v[1],v[2]);
	MatriuVista = glm::lookAt(
		glm::vec3(pv.x, pv.y, pv.z), // Camera is here
		glm::vec3(n[0], n[1], n[2]), // and looks here
		glm::vec3(v[0], v[1], v[2])  // Head is up (set to 0,-1,0 to look upside-down)
		);

// Pas Matriu a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "viewMatrix"), 1, GL_FALSE, &MatriuVista[0][0]);


// Iluminacio fixe respecte la camara (després glLookAt)
	if (ifix) Iluminacio(sh_programID,iluminacio, ifix, il2sides, llum_amb, lumi, objecte, frnt_fcs, bck_ln, step);

// Test de Visibilitat
	if (testv) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);

// Ocultacions (Z-buffer)
	if (oculta) glEnable(GL_DEPTH_TEST);
		else glDisable(GL_DEPTH_TEST);

// Dibuix de les cares back com a línies en Il.luminacio PLANA i SUAU
	//if (bck_ln) glPolygonMode(GL_BACK, GL_LINE);

// Dibuix dels eixos
	//if (eix) glCallList(EIXOS);

// Dibuixa el grid actiu
	//if ((reixa.x) || (reixa.y) || (reixa.z) || (reixa.w)) draw_Grid(reixa, hreixa);

	return MatriuVista;
}


//Vista_Geode: Definició gluLookAt a partir de coordenades esfèriques, però situant la càmera a l'origen (0,0,0)
//              i colocant el punt on mira cap el punt (R,alfa,beta) amb la possibilitat de moure aquest darrer punt
//				interactivament amb les	tecles de cursor per a veure una cúpula.
glm::mat4 Vista_Geode(GLuint sh_programID, CEsfe3D opv, char VPol, bool pant, CPunt3D tr, CPunt3D trF,
	CColor col_fons, CColor col_object, char objecte, double mida, int step,
	bool frnt_fcs, bool oculta, bool testv, bool bck_ln,
	char iluminacio, bool llum_amb, LLUM* lumi, bool ifix, bool il2sides,
	bool eix, CMask3D reixa, CPunt3D hreixa)
{
	GLdouble cam[3], camN[3], up[3];
	glm::mat4 MatriuVista(1.0);
	// Matrius Traslació
	glm::mat4 TransMatrix(1.0);

	// Conversió angles radians -> graus
	opv.alfa = opv.alfa * PI / 180;
	opv.beta = opv.beta * PI / 180;

	if (opv.R < 1.0) opv.R = 1.0;

// Neteja dels buffers de color i profunditat
	Fons(col_fons);

// Posició càmera i vector cap amunt
	// Posició Càmera
	cam[0] = opv.R * cos(opv.beta) * cos(opv.alfa);
	cam[1] = opv.R * sin(opv.beta) * cos(opv.alfa);
	cam[2] = opv.R * sin(opv.alfa);
	// Vector camp on mira (cap a (R,alfa,beta)
	camN[0] = cos(opv.beta) * cos(opv.alfa);
	camN[1] = sin(opv.beta) * cos(opv.alfa);
	camN[2] = sin(opv.alfa);
	// Vector tangent (diferencial respecte alfa)
	up[0] = -cos(opv.beta) * sin(opv.alfa);
	up[1] = -sin(opv.beta) * sin(opv.alfa);
	up[2] = cos(opv.alfa);

// Iluminacio movent-se amb la camara (abans gluLookAt)
	if (!ifix) Iluminacio(sh_programID, iluminacio, ifix, il2sides, llum_amb, lumi, objecte, frnt_fcs, bck_ln, step);

// Opció pan: desplaçament del Centre de l'esfera (pant=1)
//	if (pant) glTranslatef(tr.x,tr.y,tr.z);
//	glTranslatef(trF.x,trF.y,trF.z);	// Traslació fixada amb la INSERT dins l'opció pan
	if (pant) TransMatrix = glm::translate(TransMatrix, vec3(tr.x, tr.y, tr.z));	//glTranslatef(tr.x, tr.y, tr.z);
	TransMatrix = glm::translate(TransMatrix, vec3(trF.x, trF.y, trF.z));	// Traslació fixada amb la INSERT dins l'opció pan

// Especificació del punt de vista
   //gluLookAt(cam[0],cam[1],cam[2],0.,0.,0.,up[0],up[1],up[2]);
	MatriuVista = glm::lookAt(
		glm::vec3(cam[0], cam[1], cam[2]), // Camera is here
		glm::vec3(cam[0] + camN[0], cam[1] + camN[1], cam[2] + camN[2]), // and looks here
		glm::vec3(up[0], up[1], up[2])  // Head is up (set to 0,-1,0 to look upside-down)
	);

	MatriuVista = TransMatrix * MatriuVista;

// Pas Matriu Vista a shader
	glUniformMatrix4fv(glGetUniformLocation(sh_programID, "viewMatrix"), 1, GL_FALSE, &MatriuVista[0][0]);

// Iluminacio fixe respecte la camara (després gluLookAt)
	if (ifix) Iluminacio(sh_programID, iluminacio, ifix, il2sides, llum_amb, lumi, objecte, frnt_fcs, bck_ln, step);

// Test de Visibilitat
	if (testv) glEnable(GL_CULL_FACE);
	else glDisable(GL_CULL_FACE);

// Ocultacions (Z-buffer)
	if (oculta) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);

	return MatriuVista;
}

// instancia: Aplica Transformacions Geometriques d'instanciació, segons els parametres 
//            definits a la persiana Transformacions
glm::mat4 instancia(bool TR, INSTANCIA tg, INSTANCIA tgF)
{
	glm::mat4 MatriuTG(1.0), MatriuT(1.0);

// Transformacions geometriques sobre objecte (Traslacio, Rotacions i Escalatge) amb conversió angles Euler.
	if (TR) MatriuTG = glm::translate(MatriuTG, glm::vec3(tg.VTras.x, tg.VTras.y, tg.VTras.z));
	MatriuTG = glm::translate(MatriuTG, glm::vec3(tgF.VTras.x, tgF.VTras.y, tgF.VTras.z));	// Traslacio fixada amb la INSERT dins l'opcio Transformacions

// Convertir angles Euler tg i tgF en Matrius
	if (TR) {	MatriuT = glm::eulerAngleX(tg.VRota.x * PI / 180.f); // Passar angle en radians
				MatriuTG = MatriuTG * MatriuT;
			}
	MatriuT = glm::eulerAngleX(tgF.VRota.x * PI / 180.f); // Passar angle en radians
	MatriuTG = MatriuTG * MatriuT;
	if (TR) {	MatriuT = glm::eulerAngleY(tg.VRota.y * PI / 180.f); // Passar angle en radians
				MatriuTG = MatriuTG * MatriuT;
			}
	MatriuT = glm::eulerAngleY(tgF.VRota.y * PI / 180.f); // Passar angle en radians
	MatriuTG = MatriuTG * MatriuT;
	if (TR) {	MatriuT = glm::eulerAngleZ(tg.VRota.z * PI / 180.f); // Passar angle en radians
				MatriuTG = MatriuTG * MatriuT;
			}
	MatriuT = glm::eulerAngleX(tgF.VRota.z * PI / 180.f); // Passar angle en radians
	MatriuTG = MatriuTG * MatriuT;

	if (TR) MatriuTG = glm::scale(MatriuTG, vec3(tg.VScal.x, tg.VScal.y, tg.VScal.z));
	MatriuTG = glm::scale(MatriuTG, vec3(tgF.VScal.x, tgF.VScal.y, tgF.VScal.z));	// Escalatge fixada amb la INSERT dins l'opcio Transformacions

// Transformacions geometriques sobre objecte (Traslacio, Rotacions i Escalatge) amb Quaternions
	if (TR) MatriuTG = glm::translate(MatriuTG, glm::vec3(tg.VTras.x, tg.VTras.y, tg.VTras.z));
	MatriuTG = glm::translate(MatriuTG, glm::vec3(tgF.VTras.x, tgF.VTras.y, tgF.VTras.z));	// Traslacio fixada amb la INSERT dins l'opcio Transformacions

	return MatriuTG;
}


// deixos: Creació del VBO per al dibuix dels eixos coordenats
GLuint deixos()
{
	GLuint vaoId = 0;	// VAO Index
	GLuint vboId = 0;	// VBO Index

// vertex coords array for glDrawArrays() =====================================
// The World Coordinates Axes (WCA) are 3 lines, one over X axis (red), one over Y axis (green), one over Z axis(blue),
// the WCA cube consists of 6 vertices (3 axes * 4 vertices = 6 vertices). And, each
// vertex is 3 components (x,y,z) of doubles, therefore, the size of vertex
// array is 18 doubles (6 * 3 = 18). The size of each double is 4 bytes (72 * 4 = 288 bytes)
	GLfloat vertices[] = { 0.0f, 0.0f, 0.0f,  300.0f,   0.0f,   0.0f,  // X-Axis
						   0.0f, 0.0f, 0.0f,    0.0f, 300.0f,   0.0f,	 // Y-Axis
						   0.0f, 0.0f, 0.0f,    0.0f,   0.0f, 300.0f}; // Z-Axis

	GLfloat colors[] = { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // Red color (X-Axis)
						 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,   // Green color (Y-Axis)
						 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f }; // Blue color (Z-Axis)

// Doblar el gruix de la linia dels eixos.
	//glLineWidth(2.0);

// Create Vertex Array Object (VAO) for 3D Model Cube
	glGenVertexArrays(1, &vaoId);

// Create vertex buffer objects for 3D Model attributes in the VAO
	glGenBuffers(1, &vboId);

// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vaoId);

// Bind first VBO as being active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vboId);							// Activar VBO vèrtexs

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), 0, GL_STATIC_DRAW);	// Allocate data to VBO starting from 0 offest

// Position Vertex attributes
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), &vertices);	// Copy geometry data to VBO starting from 0 offest
	glEnableVertexAttribArray(0);										// Enable attribute index 0 as being used (position)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);	// Specify that our coordinate data is going into attribute index 0 and contains 3 double

// Normal vertex Attributes
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), &colors); // Copy color data to VBO starting from 0 offest
	glEnableVertexAttribArray(1);										// Enable attribute index 1 as being used (normals)
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)sizeof(vertices));	// Specify that our color data is going into attribute index 0 and contains 3 double

//Unbind the registered VBO as current VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

// Unbind VAO, to prevent bugs
	glBindVertexArray(0);

//	glDrawArrays(GL_LINES, 0, 6);

/*
// Eix X (vermell)
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_LINES);
	 glVertex3f(0.0,0.0,0.0);
	 glVertex3f(300.0,0.0,0.0);
	glEnd();

// Eix Y (verd)
    glColor3f(0.0,1.0,0.0);
	glBegin(GL_LINES);
	 glVertex3f(0.0,0.0,0.0);
	 glVertex3f(0.0,300.0,0.0);
	glEnd();

// Eix Z (blau) 
	glColor3f(0.0,1.0,1.0);
	glBegin(GL_LINES);
	 glVertex3f(0.0,0.0,0.0);
	 glVertex3f(0.0,0.0,300.0);
	glEnd();
*/

/* Eliminar VBO
	glDisableClientState(GL_VERTEX_ARRAY);  // Disable vertex arrays
	glDisableClientState(GL_COLOR_ARRAY);

// It is good idea to release VBOs with ID 0 after use.
// Once bound with 0, all pointers in gl*Pointer() behave as real
// pointer, so, normal vertex array operations are re-activated
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vboId);
*/
// Restaurar el gruix de la linia dels eixos
	//glLineWidth(1.0);
	return vaoId;
}

// draw_Eixos: Dibuix del VBO dels eixos coordenats
void draw_Eixos(GLuint vaoId)
{
// Doblar el gruix de la linia dels eixos.
//	glEnable(GL_LINE_WIDTH);
//	glLineWidth(2.0);

	glBindVertexArray(vaoId);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);

// Restaurar el gruix de la linia dels eixos
//	glLineWidth(1.0);
//	glDisable(GL_LINE_WIDTH);
}


// ------------------- GRID (gridXY, gridYZ, gridXZ, gridXYZ) -------------------------

void draw_Grid(CMask3D quadricula, CPunt3D hquadricula)
{
// Switch on transparency to draw the grid over the scene
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

// Grids drawing
	if (quadricula.w) gridXYZ(GRID_SIZE);
	else  {	if (quadricula.x) gridXY(GRID_SIZE, hquadricula.z);
			if (quadricula.y) gridXZ(GRID_SIZE, hquadricula.y);
			if (quadricula.z) gridYZ(GRID_SIZE, hquadricula.x);
			}

// Switch off transparency
	glDisable(GL_BLEND);
}


// gridXY: Construeix un grid XY de tamany size amb pas 1 situat una alçada h de l'eiz Z.
void gridXY(int size, GLdouble h)
{
	int i;
	//CColor col;
	//bool ref_mat;
	//bool sw_mat[4];

// VBO
	GLuint vaoId = 0, vboId = 0;
	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs i colors 
	//vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();		colors.clear();		// Reinicialitzar vectors

	//ref_mat = false;
	//sw_mat[0] = false;	sw_mat[1] = true;	sw_mat[2] = true;	sw_mat[3] = false;

// Eix X
	//glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	//col.r = 1.0f;	col.g = 0.0f;	col.b = 0.0f;	col.a = 0.5f;
	//SeleccionaColor(ref_mat, sw_mat, col);
	for (i = -size; i < size + 1; i++)
		{	//glBegin(GL_LINES);
			  colors.push_back(1.0);		colors.push_back(0.0);			colors.push_back(0.0);	colors.push_back(1.0);
			  //glVertex3f(i, -size, h);	// glVertex3f(i, -0.5f, 0.0f);
			  vertices.push_back(-i);		vertices.push_back(-size);		vertices.push_back(h);
			  
			  colors.push_back(1.0);			colors.push_back(0.0);		colors.push_back(0.0);	colors.push_back(1.0);
			  //glVertex3f(i, +size, h);	// glVertex3f(i, +0.5f, 0.0f);
			  vertices.push_back(-i);		vertices.push_back(+size);		vertices.push_back(h);

			//glEnd();
		}

// Eix Y
	//glColor4f(0.0f, 1.0, 0.0f, 0.5f);
	//col.r = 0.0f;	col.g = 1.0f;	col.b = 0.0f;	col.a = 0.5f;
	//SeleccionaColor(ref_mat, sw_mat, col);
	for (i = -size; i < size + 1; i++)
		{	//glBegin(GL_LINES);
			  colors.push_back(0.0);		colors.push_back(1.0);		colors.push_back(0.0);	colors.push_back(1.0);
			  //glVertex3f(-size, i, h);	// glVertex3f(-0.5f, i, 0.0f);
			  vertices.push_back(-size);	vertices.push_back(i);		vertices.push_back(h);

			  colors.push_back(0.0);		colors.push_back(1.0);		colors.push_back(0.0);	colors.push_back(1.0);
			  //glVertex3f(+size, i, h);	// glVertex3f(+0.5f, i, 0.0f);
			  vertices.push_back(+size);	vertices.push_back(i);		vertices.push_back(h);
			//glEnd();
		}

// ----------------------- VBO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.

	draw_GL_LINES_VAO(vertices, colors);
}

// gridXZ: Construeix un grid YZ de tamany size amb pas 1 situat una alçada h de l'eiz Z.
void gridXZ(int size, GLdouble h)
{
	int i;
	//CColor col;
	//bool ref_mat;
	//bool sw_mat[4];

// VBO
	//GLuint vaoId = 0, vboId = 0;
	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs i colors 
	//vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();		colors.clear();		// Reinicialitzar vectors

	//ref_mat = false;
	//sw_mat[0] = false;	sw_mat[1] = true;	sw_mat[2] = true;	sw_mat[3] = false;

// Eix X
	//glColor4f(1.0f, 0.0, 0.0f, 0.5f);
	//col.r = 1.0f;	col.g = 0.0f;	col.b = 0.0f;	col.a = 0.5f;
	//SeleccionaColor(ref_mat, sw_mat, col);
	for (i = -size; i < size + 1; i++)
		{	//glBegin(GL_LINES);
			  colors.push_back(1.0);		colors.push_back(0.0);	colors.push_back(0.0);	colors.push_back(1.0);
			  // glVertex3f(i, h, -size);
			  vertices.push_back(i);		vertices.push_back(h);	vertices.push_back(-size);

			  colors.push_back(1.0);		colors.push_back(0.0);	colors.push_back(0.0);	colors.push_back(1.0);
			  //glVertex3f(i, h, +size); 
			  vertices.push_back(i);	vertices.push_back(h);		vertices.push_back(+size);
			//glEnd();
		}

// Eix Z
	//glColor4f(0.0f, 1.0, 1.0f, 0.5f);
	//col.r = 0.0f;	col.g = 1.0f;	col.b = 1.0f;	col.a = 0.5f;
	//SeleccionaColor(ref_mat, sw_mat, col);
	for (i = -size; i < size + 1; i++)
		{	//glBegin(GL_LINES);
			  colors.push_back(0.0);		colors.push_back(1.0);	colors.push_back(1.0); colors.push_back(1.0);
			  //glVertex3f(-size, h, i);
			  vertices.push_back(-size);	vertices.push_back(h);	vertices.push_back(i);

			  colors.push_back(0.0);		colors.push_back(1.0);	colors.push_back(1.0);	colors.push_back(1.0);
			  //glVertex3f(size, h, i);
			  vertices.push_back(size);	vertices.push_back(h);		vertices.push_back(i);
			//glEnd();
	}

// ----------------------- VBO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.

	draw_GL_LINES_VAO(vertices, colors);
}


// gridYZ: Construeix un grid YZ de tamany size amb pas 1 situat una alçada h de l'eiz Z.
void gridYZ(int size, GLdouble h)
{
	int i;
	//CColor col;
	//bool ref_mat;
	//bool sw_mat[4];

// VAO, VBO
	//GLuint vaoId = 0, vboId = 0;
	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs i colors 
	//vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();		colors.clear();		// Reinicialitzar vectors

	//ref_mat = false;
	//sw_mat[0] = false;	sw_mat[1] = true;	sw_mat[2] = true;	sw_mat[3] = false;

// Eix Y
	//glColor4f(0.0f, 1.0, 0.0f, 0.5f);
	//col.r = 0.0f;	col.g = 1.0f;	col.b = 0.0f;	col.a = 0.5f;
	//SeleccionaColor(ref_mat, sw_mat, col);
	for (i = -size; i < size + 1; i++)
		{	//glBegin(GL_LINES);
		      colors.push_back(0.0);		colors.push_back(1.0);			colors.push_back(0.0);	colors.push_back(1.0);
			  //glVertex3f(h, i, -size);	// glVertex3f(0.0f, i, -0.5f);
			  vertices.push_back(h);		vertices.push_back(i);			vertices.push_back(-size);
			  
			  colors.push_back(0.0);		colors.push_back(1.0);			colors.push_back(0.0);	colors.push_back(1.0);
			  //glVertex3f(h, i, +size);	// glVertex3f(0.0f, i, 0.5f);
			  vertices.push_back(h);		vertices.push_back(i);			vertices.push_back(+size);

			//glEnd();
		}
/*
// Eix Z
	//glColor4f(0.0f, 1.0, 1.0f, 0.5f);
	//col.r = 0.0f;	col.g = 1.0f;	col.b = 1.0f;	col.a = 0.5f;
	//SeleccionaColor(ref_mat, sw_mat, col);
	for (i = -size; i < size + 1; i++)
		{	//glBegin(GL_LINES);
			  colors.push_back(0.0);		colors.push_back(1.0);			colors.push_back(1.0);	colors.push_back(1.0);
			  //glVertex3f(h, -size, i);	// glVertex3f(0.0, -0.5f, i);
			  vertices.push_back(h);		vertices.push_back(-size);		vertices.push_back(i);
			  
			  colors.push_back(0.0);		colors.push_back(1.0);			colors.push_back(1.0);	colors.push_back(1.0);
			  //glVertex3f(h, +size, i);	// glVertex3f(0.0f, 0.5f, i);
			  vertices.push_back(h);		vertices.push_back(+size);		vertices.push_back(i);
			//glEnd();
	}
*/
// ----------------------- VBO
	std::vector <int>::size_type nv = vertices.size();	// Tamany del vector vertices en elements.
	std::vector <int>::size_type nc = colors.size();	// Tamany del vector colors en elements.

	draw_GL_LINES_VAO(vertices, colors);
}


// gridXYZ: Construeix un grid 3D de tamany size amb pas 1.
void gridXYZ(int size)
{
	int i, j;
	//CColor col;
	//bool ref_mat;
	//bool sw_mat[4];

// VAO, VBO
	GLuint vaoId = 0, vboId = 0;
	std::vector <double> vertices, colors;		// Definició vectors dinàmics per a vertexs i colors 
	//vertices.resize(0);		colors.resize(0);	// Reinicialitzar vectors
	vertices.clear();		colors.clear();		// Reinicialitzar vectors

	//ref_mat = false;
	//sw_mat[0] = false;	sw_mat[1] = true;	sw_mat[2] = true;	sw_mat[3] = false;

	for (j = -size; j < size; j++)
	{
// Eix X
		//glColor4f(1.0f, 0.0, 0.0f, 0.5f);
		//col.r = 1.0f;	col.g = 0.0f;	col.b = 0.0f;	col.a = 0.5f;
		//SeleccionaColor(ref_mat, sw_mat, col);
		for (i = -size; i <= size; i++)
			{	//glBegin(GL_LINES);
				  colors.push_back(1.0);		colors.push_back(0.0);			colors.push_back(0.0);	colors.push_back(1.0);
				  //glVertex3f(i, -size, j);	// glVertex3f(i, -0.5f, z);
				  vertices.push_back(i);		vertices.push_back(-size);		vertices.push_back(j);

				  colors.push_back(1.0);		colors.push_back(0.0);			colors.push_back(0.0);	colors.push_back(1.0);
				  //glVertex3f(i, +size, j);	// glVertex3f(i, +0.5f, z);
				  vertices.push_back(i);		vertices.push_back(+size);		vertices.push_back(j);
				//glEnd();
			}

// Eix Y
		//glColor4f(0.0f, 1.0, 0.0f, 0.5f);
		//col.r = 0.0f;	col.g = 1.0f;	col.b = 0.0f;	col.a = 0.5f;
		//SeleccionaColor(ref_mat, sw_mat, col);
		for (i = -size; i <= size; i++)
			{	//glBegin(GL_LINES);
				  colors.push_back(0.0);		colors.push_back(1.0);		colors.push_back(0.0);	colors.push_back(1.0);
				  //glVertex3f(-size, i, j);	// glVertex3f(-0.5f, i, z);
				  vertices.push_back(-size);	vertices.push_back(i);		vertices.push_back(j);

				  colors.push_back(0.0);		colors.push_back(1.0);		colors.push_back(0.0);	colors.push_back(1.0);
				  //glVertex3f(+size, i, j);	// glVertex3f(+0.5f, i, z);
				  vertices.push_back(+size);	vertices.push_back(i);		vertices.push_back(j);
				//glEnd();
			}
	}

	draw_GL_LINES_VAO(vertices, colors);
}



// Fons: Dibuixa el fons variable sefons el color int_fons
void Fons(CColor int_fons)
{
	glClearColor(int_fons.r,int_fons.g,int_fons.b,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();
}

// FonsN: Dibuixa el fons negre
void FonsN()
{
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glFlush();
}

// FonsB: Dibuixa el fons blanc
void FonsB()
{
	glClearColor(1.f,1.f,1.f,1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.f,0.f,0.f,1.f);
	glClear(GL_DEPTH_BUFFER_BIT);
   glFlush();
}

// TEXTURES------------------------------------------------------
// loadIMA_SOIL: This function receives as input the image filename and an 
// integer identifier (0 for the first texture) and creates an OpenGL 
// texture which is stored in the global array 'textures'
// It uses SOIL library functions to load the image
// Paràmetres:
//		- filename: Fitxer que conté la imatge de qualsevol format:
//					BMP, JPG, TIFF, TGA, GIF, i d'altres suportats per SOIL
//		- Retorna: Identificador dins la taula textures on volem
//                assignar la imatge
GLint loadIMA_SOIL(const char * filename)
{	FILE *file = NULL;
	int errno;
	GLuint textureID = 0;

// Open the image file for reading
//  file=fopen(nomf,"r");					// Funció Visual Studio 6.0
	errno = fopen_s(&file, filename, "rb");		// Funció Visual 2010

// If the file is empty (or non existent) print an error and return false
// if (file == NULL)
	if (errno != 0)
	{	//	printf("Could not open file '%s'.\n",filename) ;
		return false;
	}

// Close the image file
	fclose(file);

/*
// SOIL_load_OGL_texture: Funció que llegeix la imatge del fitxer filename
//				si és compatible amb els formats SOIL (BMP,JPG,GIF,TIF,TGA,etc.)
//				i defineix la imatge com a textura OpenGL retornant l'identificador 
//				de textura OpenGL.
	textureID = SOIL_load_OGL_texture
	(filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_DDS_LOAD_DIRECT | SOIL_FLAG_INVERT_Y
	);
*/

//Generate texture ID and load texture data 
	glGenTextures(1, &textureID);
	int width, height;
	unsigned char* image = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGB);
// Assign texture to ID
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

// If execution arrives here it means that all went well. Return true
	return textureID;
}


// TEXTURES------------------------------------------------------
// loadIMA_ILUT: This function receives as input the image filename and an 
// integer identifier (0 for the first texture) and creates an OpenGL 
// texture which is stored in the global array 'textures'
// It uses Devil library functions to load the image
// Paràmetres:
//		- filename: Fitxer que conté la imatge de qualsevol format:
//					BMP, JPG, TIFF, TGA, GIF, i d'altres suportats per OpenIL
//		- Retorna: Identificador dins la taula textures on volem
//                assignar la imatge
GLint loadIMA_ILUT(const char * filename)
{
	FILE *file = NULL;
	int errno;
	char szFilePath[255];
	//wchar_t* nomf = NULL;
	GLint auxID;

// Open the image file for reading
//  file=fopen(nomf,"r");					// Funció Visual Studio 6.0
	errno = fopen_s(&file, filename, "rb");		// Funció Visual 2010

// If the file is empty (or non existent) print an error and return false
// if (file == NULL)
	if (errno != 0)
	{
	//	printf("Could not open file '%s'.\n",filename) ;
		return -1;
	}

// Close the image file
	fclose(file);

// strcpy_s: Conversió variable char* --> char[255]
	strcpy_s(szFilePath, filename); // Copiar path sencer amb nom de fitxer.

// reinterpret_cast: Funció de conversió d'una variable char * -> wchar_t *
	wchar_t *nomf = reinterpret_cast <wchar_t *> (szFilePath);
	//int wchars_num = MultiByteToWideChar(CP_UTF8, 0, szFilePath, -1, NULL, 0);
	//wchar_t* nomf = new wchar_t[wchars_num];
	//MultiByteToWideChar(CP_UTF8, 0, szFilePath, -1, nomf, wchars_num);

// ilutGLLoadImage: Funció que llegeix la imatge del fitxer filename
//				si és compatible amb els formats DevIL/OpenIL (BMP,JPG,GIF,TIF,TGA,etc.)
//				i defineix la imatge com a textura OpenGL retornant l'identificador 
//				de textura OpenGL.
//	textures[texID] = ilutGLLoadImage(nomf);
	auxID = ilutGLLoadImage(nomf);

// If execution arrives here it means that all went well. Return true
	return auxID;
}



// ----------------- CUBEMAP TEXTURE
// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height; // nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		unsigned char* data = SOIL_load_image(faces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			//stbi_image_free(data);
			SOIL_free_image_data(data);
		}
		else
		{	//std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			fprintf(stderr, "Cubemap texture failed to load at path: %s \n", faces[i].c_str());
			//stbi_image_free(data);
			SOIL_free_image_data(data);
			return 0;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}



// SetTextureParameters: Funció que configura paràmetres de l'objecte textura IDText 
// Paràmetres:
//		- IDText: Identificador de l'objecte textura a configurar
//		- extensT: Extén la imatge textura fora dels límits normalitzats (0,1) [TRUE] o no [FALSE]
//		- filterT: Aplica filtre linieal a la imatge textura [TRUE] o pren píxel més proper [FALSE]
//		- blendT: Aplica la funció de blending tenint en compte luminància [TRUE] o només la intensitat de la imatge textura [FALSE]
//		- genMipmpT: Implementa mipmaps sobre la imatge textura [TRUE] o no [FALSE]
void SetTextureParameters(GLint IDText, bool extensT, bool filterT, bool blendT, bool genMipmpT)
{
	glBindTexture(GL_TEXTURE_2D, IDText);

// Funció de Blending de textura
	//if (blendT) glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//else glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

// Extensió de textures
	if (extensT) {	// Configuració textura per a que es repereixi més enllà de coordenades (0,0)-(1,1)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}

// Filtratge de textures
	if (filterT) {	// Filtratge de textures (GL_LINEAR)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		if (genMipmpT) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	else {	// Filtratge de textures (GL_NEAREST)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (genMipmpT) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

// Activació de MipMaps de la imatge textura
	if (genMipmpT) {	// Activació MIPMAP en textures
		glGenerateMipmap(GL_TEXTURE_2D);
	}

// Activació i inicialització matriu GL_TEXTURE
//	glMatrixMode(GL_TEXTURE);
//	glLoadIdentity();
//	glRotatef(45.0f, 0.0f, 0.0f, 1.0f); // Rotació 45 graus de la imatge textura

// Activació i inicialització matriu GL_PROJECTION
//	glMatrixMode(GL_PROJECTION);
}

