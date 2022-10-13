//******** PRACTICA VISUALITZACIÓ GRÀFICA INTERACTIVA (Escola Enginyeria - UAB)
//******** Entorn bàsic VS2022 MULTIFINESTRA amb interfície MFC, llibreries GLM i OpenGL 3.3+
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2022)
// material.h : interface de material.cpp
//

#ifndef GLMATERIAL_H
#define GLMATERIAL_H

#include "stdafx.h"

//-------------- GC2: Màxim nombre de Materials
#define MAX_MATERIALS  11

//Tipus de materials
#define MAT_CAP 		 -1
#define	MAT_DEFAULT       0
#define MAT_BASE          1
#define MAT_METALL_OBSCUR 2 
#define	MAT_VIDRE		  3
#define MAT_GOMA_NEGRA    4  
#define MAT_GESPA         5
#define MAT_METALL_GROC   6
#define MAT_LLUM_VERMELLA 7
#define MAT_LLUM_GROGA    8
#define MAT_LLUM_VERDA    9
#define MAT_FITXER	     10

/* MATERIALS -> A constants.h
typedef struct
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float shininess;
} MATERIAL;


struct MATERIAL
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float emission[4];
	float shininess;
};
*/

/*
enum 
{
	MAT_CAP,
	MAT_DEFAULT,
	MAT_BASE, 
	MAT_METALL_OBSCUR, 
	MAT_VIDRE, 
	MAT_GOMA_NEGRA, 
	MAT_GESPA, 
	MAT_METALL_GROC,
	MAT_LLUM_VERMELLA,
	MAT_LLUM_GROGA,
	MAT_LLUM_VERDA
} mat;
*/


/* ------------------------------------------------------------------------- */
/*                            Funcions de material                           */
/* ------------------------------------------------------------------------- */

// Seleccionar Materials
void SeleccionaMaterial(GLuint sh_programID, int quin, bool sw_mater[5]);
void SeleccionaColorMaterial(GLuint sh_programID, CColor c_obj, bool sw_mater[5]);

bool llegir_Material(GLuint sh_programID, char * filename,int index);
#endif