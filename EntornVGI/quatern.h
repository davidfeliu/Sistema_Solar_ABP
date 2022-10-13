//******** PRACTICA VISUALITZACIÓ 3D (Facultat de Ciències / Escola d'Enginyeria - UAB)
//******** Entorn bàsic VS2017 MULTIFINESTRA amb OpenGL, interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2019)
// quatern.h : interface de quatern.cpp
//

#ifndef QUATERN
#define QUATERN


// -------------- V3D-QUATERNIONS: Número de frames de l'animació de cossos rígids per QUATERNIONS.
const int NFRAMES_Q = 25;
const int MAX_KEYFRAMES_Q = 3;	// Nombre màxim de keyframes

// -------- Estructura Quaternio representat com (w,[x,y,z])
//	where:  w       - component escalar
//			x, y, z - component vector
struct GL_Quat 
{ 	GLdouble w;
	GLdouble x;
	GLdouble y;
	GLdouble z;
};

// Funcions de tractament de quaternions

void EixAngleToQuat(double X, double Y, double Z, double degree, GL_Quat &q);
//GL_Quat EixAngleToQuat(double X, double Y, double Z, double degree);
void QuatToEixAngle(GL_Quat quat, GLdouble &x, GLdouble &y, GLdouble &z, GLdouble &degree);
void QuatSetValue(GLdouble angle, GLdouble x, GLdouble y, GLdouble z, GL_Quat &quat);
void QuatToMatrix(GL_Quat q,GLdouble* pMatrix);

void EulerToQuat(GLdouble roll,GLdouble pitch,GLdouble yaw,GL_Quat &quat);
void MatrixToQuat(double pMatrix, GL_Quat &quat);

// Interpolacions per Quaternions
void QuatLerp(GL_Quat from, GL_Quat to, GLdouble t, GL_Quat &res);   // Linial
void QuatSlerp(GL_Quat from, GL_Quat to, GLdouble t,GL_Quat &res);			// Esfèrica

// ALGEBRA DE QUATERNIONS
void QuatNormalize(GL_Quat &quat);
void QuatScaleAngle(GL_Quat & quat, GLdouble scale);
void QuatInverse(GL_Quat q1, GL_Quat &quat);
void QuatSetFromAx(GLdouble x1,GLdouble y1, GLdouble z1, 
						 GLdouble x2,GLdouble y2, GLdouble z2, GL_Quat &quat);
void QuatMul(GL_Quat q1, GL_Quat q2, GL_Quat &res);
void QuatAdd(GL_Quat q1, GL_Quat q2, GL_Quat &res);
void QuatSub(GL_Quat q1, GL_Quat q2, GL_Quat &res);
void QuatDiv(GL_Quat q1, GL_Quat q2, GL_Quat &res);
void QuatCopy(GL_Quat q1, GL_Quat &q2);
void QuatSquare(GL_Quat q1, GL_Quat &res);
void QuatSqrt(GL_Quat q1, GL_Quat &res);
GLdouble QuatDot(GL_Quat q1, GL_Quat q2);
GLdouble QuatLength(GL_Quat q1);
void QuatNegate(GL_Quat q1, GL_Quat &q2);
void QuatExp(GL_Quat q1, GL_Quat &q2);
void QuatLog(GL_Quat q1, GL_Quat &q2);
void QuatLnDif(GL_Quat q1, GL_Quat q2, GL_Quat &res);

#endif