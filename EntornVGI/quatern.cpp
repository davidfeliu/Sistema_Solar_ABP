//******** PRACTICA VISUALITZACIÓ 3D (Facultat de Ciències / Escola d'Enginyeria - UAB)
//******** Entorn bàsic VS2017 MULTIFINESTRA amb OpenGL, interfície MFC i Status Bar
//******** Ferran Poveda, Marc Vivet, Carme Julià, Débora Gil, Enric Martí (Setembre 2019)
// quatern.cpp : Funcions bàsiques de manipulació i interpolació de
//					 quaternions. 
// EixAngle2Quat i Quat2Matrix són funcions adaptades de Ben Humphrey (Digiben) 
//	GameTutorials (www.gametutorials.com).



#include "stdafx.h"
#include "quatern.h"

//#include "main.h"										// This includes our header file

#define DELTA 1e-6     // error tolerance

/* -------- Estructura Quaternio representat com (w,[x,y,z])
//	where:  w       - component escalar
//			x, y, z - component vector
struct GL_Quat { 
	GLdouble w:
	GLdouble x;
	GLdouble y;
	GLdouble z;
}*/

// EixAngle2Quat: This function takes an angle and an axis of rotation, then converts
//				  it to a quaternion.  An example of an axis and angle is what we 
//				  pass into glRotatef().  
//				  That is an axis angle rotation.  It is assumed an angle in 
//				  degrees is being passed in.  
//				  Instead of using glRotatef(), we can now handle the rotations our self.

//				  The equations for axis angle to quaternions are such:
//						w = cos( theta / 2 )
//						x = X * sin( theta / 2 )
//						y = Y * sin( theta / 2 )
//						z = Z * sin( theta / 2 )
void EixAngleToQuat(double X, double Y, double Z, double degree, GL_Quat &q) 
//GL_Quat EixAngle2Quat(double X, double Y, double Z, double degree) 
{ 

	GL_Quat auxq;
//	GL_Quat qPi;

//	qPi.w=1.0;	qPi.x=0.0;	qPi.y=0.0;	qPi.z=0.0;				// Quaternió Identitat
//	if ((Y==0) && (Z==0)) {qPi.w=0.0;	qPi.x=1.0;}				// Quaternió Rotació pi graus (1,0,0)
//		else if ((X==0) && (Z==0)) {qPi.w=0.0;	qPi.y=1.0;}		// Quaternió Rotació pi graus (0,1,0)
//				if ((X==0) && (Z==0)) {qPi.w=0.0;	qPi.z=1.0;}	// Quaternió Rotació pi graus (0,0,1)
				
// First we want to convert the degrees to radians 
//		since the angle is assumed to be in radians
	double angle = double((degree / 180.0f) * pi);

// Here we calculate the sin( theta / 2) once for optimization
	double result = (double)sin( angle / 2.0f );
		
// Calcualte the w value by cos( theta / 2 )
	auxq.w = cos( angle / 2.0f ); if (q.w<DELTA) q.w=0;

// Calculate the x, y and z of the quaternion
	auxq.x = GLdouble(X * result);
	auxq.y = GLdouble(Y * result);
	auxq.z = GLdouble(Z * result);

	q=auxq;
//	if (angle>pi) 	QuatMul(auxq,qPi,q);

//	EixAngle2Quat.w =q.w;
//	EixAngle2Quat.x =q.x;	
//	EixAngle2Quat.y =q.y;	
//	EixAngle2Quat.z =q.z;

}

/*SDOC***********************************************************************

  Name:		gluQuatGetValue_EXT

  Action:   Disassembles quaternion to an axis and an angle

  Params:   GL_QUAT* (quaternion), GLdouble* (x, y, z - axis), GLdouble (angle)

  Returns:  nothing

  Comments: NOTE: vector has been split into x, y, z so that you do not have
			to change your vector library (i.e. greater portability)

			NOTE2: angle is in RADIANS

***********************************************************************EDOC*/
// QuatToEixAngle: Disassembles quaternion to an axis and an angle
//			NOTE: vector has been split into x, y, z so that you do not have
//					to change your vector library (i.e. greater portability)
//			NOTE2: angle is in DEGREES
void QuatToEixAngle(GL_Quat quat, GLdouble &x, GLdouble &y, GLdouble &z, GLdouble &degree)
{
    GLdouble	len,dgr_rad;
    GLdouble tx, ty, tz;

	// cache variables
	tx = quat.x;
	ty = quat.y;
	tz = quat.z;

	len = sqrt(tx * tx + ty * ty + tz * tz);

    if (len > DELTA) 
	{
//		*x = tx * (1.0f / len);
		x = tx * (1.0f / len);	if (x<DELTA) x=0;
		y = ty * (1.0f / len);	if (y<DELTA) y=0;
		z = tz * (1.0f / len);	if (z<DELTA) z=0;
	    dgr_rad = (GLdouble)(2.0 * acos(quat.w)); // Angle en radians
		degree= GLdouble((dgr_rad * 180.0f) / pi); // Angle en graus
		while (degree>=360)	degree=degree-360;
		while (degree<0)	degree=degree+360;
    }

    else {
		x = 0.0;
		y = 0.0;
		z = 1.0;
	    degree = 0.0;
    }
}


//QuatSetValue: Assembles quaternion from an axis and an angle
//			NOTE: vector has been split into x, y, z so that you do not have
//				o change your vector library (i.e. greater portability)
//			NOTE2: angle has to be in RADIANS
void QuatSetValue(GLdouble angle, GLdouble x, GLdouble y, GLdouble z, GL_Quat &quat)
{
	GLdouble temp, dist;

// normalize
	temp = x*x + y*y + z*z;

    dist = (GLdouble)(1.0 / sqrt(temp));

    x *= dist;
    y *= dist;
    z *= dist;

	quat.x = x;
	quat.y = y;
	quat.z = z;

	quat.w = (GLdouble)cos(angle / 2.0f);
	
}



// Quat2Matrix: This function is a necessity when it comes to doing almost anything
//					with quaternions.  Since we are working with OpenGL, which uses 
//					a 4x4 homogeneous matrix, we need to have a way to take our 
//					quaternion and convert it to a rotation matrix to modify the 
//					current model view matrix.
//				We pass in a 4x4 matrix, which is a 1D array of 16 doubles.  
//				This is how OpenGL allows us to pass in a matrix to glMultMatrixf(), 
//					so we use a single dimensioned array.
//				After about 300 trees murdered and 20 packs of chalk depleted, the
//					mathematicians came up with these equations for a quaternion to 
//					matrix converion:
//
//     ¦        2     2												 ¦
//     ¦ 1 - (2y  + 2z )   2xy + 2zw         2xz - 2yw			0	 ¦
//     ¦															 ¦
//     ¦                          2     2							 ¦
// M = ¦ 2xy - 2zw         1 - (2x  + 2z )   2zy + 2xw			0	 ¦
//     ¦															 ¦
//     ¦                                            2     2			 ¦
//     ¦ 2xz + 2yw         2yz - 2xw         1 - (2x  + 2y )	0	 ¦
//     ¦															 ¦
//     ¦															 ¦
//     ¦ 0				   0				 0					1	 |													 ¦
//     ¦															 ¦
// 
// This is of course a 4x4 matrix.  Notice that a rotational matrix can just
//	be a 3x3 matrix, but since OpenGL uses a 4x4 matrix, we need to conform to the man.
//	Remember that the identity matrix of a 4x4 matrix has a diagonal of 1's, where
//	the rest of the indices are 0.  That is where we get the 0's lining the sides, and
//	the 1 at the bottom-right corner.  Since OpenGL matrices are row by column, we fill
//	in our matrix accordingly below.
void QuatToMatrix(GL_Quat q,GLdouble* pMatrix)
{
// Make sure the matrix has allocated memory to store the rotation data
	if(!pMatrix) return;
	
// First row
	pMatrix[ 0] = 1.0f - 2.0f * ( q.y * q.y + q.z * q.z ); 
	pMatrix[ 1] = 2.0f * (q.x * q.y + q.z * q.w);
	pMatrix[ 2] = 2.0f * (q.x * q.z - q.y * q.w);
	pMatrix[ 3] = 0.0f;  

// Second row
	pMatrix[ 4] = 2.0f * ( q.x * q.y - q.z * q.w );  
	pMatrix[ 5] = 1.0f - 2.0f * ( q.x * q.x + q.z * q.z ); 
	pMatrix[ 6] = 2.0f * (q.z * q.y + q.x * q.w );  
	pMatrix[ 7] = 0.0f;  

// Third row
	pMatrix[ 8] = 2.0f * ( q.x * q.z + q.y * q.w );
	pMatrix[ 9] = 2.0f * ( q.y * q.z - q.x * q.w );
	pMatrix[10] = 1.0f - 2.0f * ( q.x * q.x + q.y * q.y );  
	pMatrix[11] = 0.0f;  

// Fourth row
	pMatrix[12] = 0;  
	pMatrix[13] = 0;  
	pMatrix[14] = 0;  
	pMatrix[15] = 1.0f;



// Now pMatrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
}


//EulerToQuat: Converts representation of a rotation from Euler angles to
//				quaternion representation
//	PARÀMETRES:		roll  - rotation around X axis
//					pitch - rotation around Y axis
//					yaw   - rotation around Z axis
//			rotations are performed in the following order:
//					yaw -> pitch -> roll
//			Qfinal = Qyaw Qpitch Qroll
void EulerToQuat(GLdouble roll,GLdouble pitch,GLdouble yaw,GL_Quat &quat)
{

	GL_Quat qX,qY,qZ,qT;
/*	
	GLdouble cr, cp, cy, sr, sp, sy, cpcy, spsy;

	cr = cos(roll/2);
	cp = cos(pitch/2);
	cy = cos(yaw/2);

	sr = sin(roll/2);
	sp = sin(pitch/2);
	sy = sin(yaw/2);
	
	cpcy = cp * cy;
	spsy = sp * sy;

	quat.w = cr * cpcy + sr * spsy;
	quat.x = sr * cpcy - cr * spsy;
	quat.y = cr * sp * cy + sr * cp * sy;
	quat.z = cr * cp * sy - sr * sp * cy;
*/

	EixAngleToQuat(1,0,0,roll,qX);
	EixAngleToQuat(0,1,0,pitch,qY);
	EixAngleToQuat(0,0,1,yaw,qZ);
	QuatMul(qX,qY,qT);
	QuatMul(qT,qZ,quat);

}


// MatToQuat: Converts matrix representation of a rotation to a quaternion
//			    representation
//			  Remember matrix (in OGL) is represented in COLUMN major form
void MatrixToQuat(double *pMatrix, GL_Quat &quat)
{

  GLdouble  tr, s;
  GLdouble  q[4];
  GLint    i, j, k;

  double m[4][4];
  int nxt[3] = {1, 2, 0};

  m[0][0] = pMatrix[0];
  m[0][1] = pMatrix[1];
  m[0][2] = pMatrix[2];
  m[0][3] = pMatrix[3];
 
  m[1][0] = pMatrix[4];
  m[1][1] = pMatrix[5];
  m[1][2] = pMatrix[6];
  m[1][3] = pMatrix[7];

  m[2][0] = pMatrix[8];
  m[2][1] = pMatrix[9];
  m[2][2] = pMatrix[10];
  m[2][3] = pMatrix[11];

  m[3][0] = pMatrix[12];
  m[3][1] = pMatrix[13];
  m[3][2] = pMatrix[14];
  m[3][3] = pMatrix[15];

  tr = m[0][0] + m[1][1] + m[2][2];

// check the diagonal

  if (tr > 0.0) 
  {
    s = sqrt (tr + 1.0);

    quat.w = s / 2.0;
    
	s = 0.5 / s;

    quat.x = (m[1][2] - m[2][1]) * s;
    quat.y = (m[2][0] - m[0][2]) * s;
    quat.z = (m[0][1] - m[1][0]) * s;

  } else {		
	  
// diagonal is negative
    
	  i = 0;

      if (m[1][1] > m[0][0]) i = 1;
	  if (m[2][2] > m[i][i]) i = 2;
    
	  j = nxt[i];
      k = nxt[j];

      s = sqrt ((m[i][i] - (m[j][j] + m[k][k])) + 1.0);
      
	  q[i] = s * 0.5;

      if (s != 0.0) s = 0.5 / s;

	  q[3] = (m[j][k] - m[k][j]) * s;
      q[j] = (m[i][j] + m[j][i]) * s;
      q[k] = (m[i][k] + m[k][i]) * s;

	  quat.x = q[0];
	  quat.y = q[1];
	  quat.z = q[2];
	  quat.w = q[3];
  }

}


//----------------- INTERPOLACIÓ PER QUATERNIONS (linial i esfèrica)  ------


// QuaSLerp: Smoothly (spherically, shortest path on a quaternion sphere) 
//				interpolates between two UNIT quaternion positions
//				Most of this code is optimized for speed and not for readability
//				As t goes from 0 to 1, qt goes from p to q.
//				slerp(p,q,t) = (p*sin((1-t)*omega) + q*sin(t*omega)) / sin(omega)

//    PARÀMETRES:	- from, to: Quaternions inicial i final
//					- t: Paràmetre (0,1)
//					- res: Quaternió interpolat
//		 
void QuatSlerp(GL_Quat from, GL_Quat to, GLdouble t,GL_Quat &res)
{
        GLdouble           to1[4];
        GLdouble          omega, cosom, sinom;
        GLdouble          scale0, scale1;

        // calc cosine
        cosom = from.x * to.x + from.y * to.y + from.z * to.z
			       + from.w * to.w;

// adjust signs (if necessary)
        if ( cosom < 0.0 )
		{
			cosom = -cosom;

			to1[0] = - to.x;
			to1[1] = - to.y;
			to1[2] = - to.z;
			to1[3] = - to.w;

        } else  {

			to1[0] = to.x;
			to1[1] = to.y;
			to1[2] = to.z;
			to1[3] = to.w;

        }

// calculate coefficients

        if ( (1.0 - cosom) > DELTA ) 
		{
                // standard case (slerp)
                omega = acos(cosom);
                sinom = sin(omega);
                scale0 = sin((1.0 - t) * omega) / sinom;
                scale1 = sin(t * omega) / sinom;

        } else {        
			    // "from" and "to" quaternions are very close 
			    //  ... so we can do a linear interpolation

                scale0 = 1.0 - t;
                scale1 = t;
        }

		// calculate final values
		res.x = scale0 * from.x + scale1 * to1[0];
		res.y = scale0 * from.y + scale1 * to1[1];
		res.z = scale0 * from.z + scale1 * to1[2];
		res.w = scale0 * from.w + scale1 * to1[3];

}


// QuatLerp: Linearly interpolates between two quaternion positions.
//				Ràpid però no tant suau com Slerp.
//    PARÀMETRES:	- from, to: Quaternions inicial i final
//					- t: Paràmetre (0,1)
//					- res: Quaternió interpolat
//		 
void QuatLerp(GL_Quat from, GL_Quat to, GLdouble t, GL_Quat &res)
{
        GLdouble           to1[4];
        GLdouble          cosom;
        GLdouble          scale0, scale1;

        // calc cosine
        cosom = from.x * to.x + from.y * to.y + from.z * to.z
			       + from.w * to.w;

// adjust signs (if necessary)
        if ( cosom < 0.0 )
		{
			to1[0] = - to.x;
			to1[1] = - to.y;
			to1[2] = - to.z;
			to1[3] = - to.w;

        } else  {

			to1[0] = to.x;
			to1[1] = to.y;
			to1[2] = to.z;
			to1[3] = to.w;

        }

 
// interpolate linearly
        scale0 = 1.0 - t;
        scale1 = t;
 
		// calculate final values
		res.x = scale0 * from.x + scale1 * to1[0];
		res.y = scale0 * from.y + scale1 * to1[1];
		res.z = scale0 * from.z + scale1 * to1[2];
		res.w = scale0 * from.w + scale1 * to1[3];

}


//----------------- ALGEBRA DE QUATERNIONS  ------------

//QuatNormalize: Normalizes quaternion (i.e. w^2 + x^2 + y^2 + z^2 = 1)
void QuatNormalize(GL_Quat &quat)
{
    GLdouble	dist, square;

	square = quat.x * quat.x + quat.y * quat.y + quat.z * quat.z
		+ quat.w * quat.w;
	
	if (square > 0.0)
		dist = (GLdouble)(1.0 / sqrt(square));
	else dist = 1;

    quat.x *= dist;
    quat.y *= dist;
    quat.z *= dist;
    quat.w *= dist;
}


// QuatScaleAngle: Scales the rotation angle of a quaternion
void QuatScaleAngle(GL_Quat & quat, GLdouble scale)
{
    GLdouble x, y, z;	// axis
    GLdouble angle;		// and angle

	QuatToEixAngle(quat, x, y, z, angle);

    EixAngleToQuat((angle * scale),x, y, z, quat);
}



// QuatInverse: Inverts quaternion's rotation ( q^(-1) )
void QuatInverse(GL_Quat q1, GL_Quat &quat)
{
	GLdouble norm, invNorm;

	norm = q1.x * q1.x + q1.y * q1.y + q1.z * q1.z
		               + q1.w * q1.w;
	
	invNorm = (GLdouble) (1.0 / norm);
	
	quat.x = -q1.x * invNorm;
    quat.y = -q1.y * invNorm;
    quat.z = -q1.z * invNorm;
    quat.w =  q1.w * invNorm;

}


//QuatSetFromAx: Constructs quaternion to rotate from one direction vector to 
//				another
//	PARÀMETRES: GLdouble (x1, y1, z1 - from vector), 
//				GLdouble (x2, y2, z2 - to vector), GL_QUAT* (resulting quaternion)
//	COMENTARIS: Two vectors have to be UNIT vectors (so make sure you normalize
//					them before calling this function
//				Some parts are heavily optimized so readability is not so great :(
void QuatSetFromAx(GLdouble x1,GLdouble y1, GLdouble z1, 
						 GLdouble x2,GLdouble y2, GLdouble z2, GL_Quat &quat)

{
    GLdouble tx, ty, tz, temp, dist;

    GLdouble	cost, len, ss;

	// get dot product of two vectors
    cost = x1 * x2 + y1 * y2 + z1 * z2;

    // check if parallel
    if (cost > 0.99999f) {
	quat.x = quat.y = quat.z = 0.0f;
	quat.w = 1.0f;
	return;
    }
    else if (cost < -0.99999f) {		// check if opposite

// check if we can use cross product of from vector with [1, 0, 0]
	tx = 0.0;
	ty = x1;
	tz = -y1;

	len = sqrt(ty*ty + tz*tz);

	if (len < DELTA)
	{
		// nope! we need cross product of from vector with [0, 1, 0]
		tx = -z1;
		ty = 0.0;
		tz = x1;
	}

// normalize
	temp = tx*tx + ty*ty + tz*tz;

    dist = (GLdouble)(1.0 / sqrt(temp));

    tx *= dist;
    ty *= dist;
    tz *= dist;
	
	quat.x = tx;
	quat.y = ty;
	quat.z = tz;
	quat.w = 0.0;

	return;
    }

// ... else we can just cross two vectors

	tx = y1 * z2 - z1 * y2;
	ty = z1 * x2 - x1 * z2;
	tz = x1 * y2 - y1 * x2;

	temp = tx*tx + ty*ty + tz*tz;

    dist = (GLdouble)(1.0 / sqrt(temp));

    tx *= dist;
    ty *= dist;
    tz *= dist;


// we have to use half-angle formulae (sin^2 t = ( 1 - cos (2t) ) /2)
	
	ss = (double)sqrt(0.5f * (1.0f - cost));

	tx *= ss;
	ty *= ss;
    tz *= ss;

// scale the axis to get the normalized quaternion
    quat.x = tx;
    quat.y = ty;
    quat.z = tz;

// cos^2 t = ( 1 + cos (2t) ) / 2
// w part is cosine of half the rotation angle
    quat.w = (double)sqrt(0.5f * (1.0f + cost));

}


// QuatMul: Multiplies two quaternions
//    PARÀMETRES:	q1,q2: First quaternion
//					res: Destination quaternion (res=q1 * q2)
void QuatMul(GL_Quat q1, GL_Quat q2, GL_Quat &res)
{

	res.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y; //if (res.x<DELTA) res.x=0;
	res.y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z; //if (res.y<DELTA) res.y=0;
	res.z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x; //if (res.z<DELTA) res.z=0;
	res.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z; //if (res.w<DELTA) res.w=0;

	// make sure the resulting quaternion is a unit quat.
	QuatNormalize(res);

}


// QuatAdd: Add two quaternions
//    PARÀMETRES:	q1,q2: First quaternion
//					res: Destination quaternion (res=q1 + q2)
void QuatAdd(GL_Quat q1, GL_Quat q2, GL_Quat &res)
{
	res.x = q1.x + q2.x;
	res.y = q1.y + q2.y;
	res.z = q1.z + q2.z;
	res.w = q1.w + q2.w;

// make sure the resulting quaternion is a unit quat.
	QuatNormalize(res);
}


// QuatSub: Subtracts two quaternions
//    PARÀMETRES:	q1,q2: First quaternion
//					res: Destination quaternion (res=q1 - q2)
void QuatSub(GL_Quat q1, GL_Quat q2, GL_Quat &res)
{
	res.x = q1.x - q2.x;
	res.y = q1.y - q2.y;
	res.z = q1.z - q2.z;
	res.w = q1.w - q2.w;

	// make sure the resulting quaternion is a unit quat.
	QuatNormalize(res);
}


// QuatDiv: Divide two quaternions
//    PARÀMETRES:	q1,q2: First quaternion
//					res: Destination quaternion
void QuatDiv(GL_Quat q1, GL_Quat q2, GL_Quat &res)
{
	GL_Quat q, r, s;

	QuatCopy(q2, q);

// invert vector
    q.x = -q.x;
    q.y = -q.y;
    q.z = -q.z;
	
	QuatMul(q1,q,r);
	QuatMul(q,q,s);

	res.x = r.x / s.w;
	res.y = r.y / s.w;
	res.z = r.z / s.w;
	res.w = r.w / s.w;

}


// QuatCopy: Copies q1 to q2.
void QuatCopy(GL_Quat q1, GL_Quat &q2)
{
	q2.x = q1.x;
	q2.y = q1.y;
	q2.z = q1.z;
	q2.w = q1.w;
}


// QuatSquare: Square quaternion
//    PARÀMETRES:	q1: First quaternion
//					res: Destination quaternion
void QuatSquare(GL_Quat q1, GL_Quat &res)
{
	GLdouble  tt;


	tt = 2 * q1.w;
	res.x = tt * q1.x;
	res.y = tt * q1.y;
	res.z = tt * q1.z;
	res.w = (q1.w * q1.w - q1.x * q1.x - q1.y * q1.y - q1.z * q1.z);
}


// QuatSqrt: Find square root of a quaternion
//    PARÀMETRES:	q1: First quaternion
//					res: Destination quaternion
void QuatSqrt(GL_Quat q1, GL_Quat &res)
{
	GLdouble  length, m, r1, r2;
	GL_Quat r;

	length = sqrt (q1.w * q1.w + q1.x * q1.x + q1.y * q1.y);
	if (length != 0.0) 
		length = 1.0 / length; 
	else length = 1.0;

	r.x = q1.x * length;
	r.y = q1.z * length;
	r.z = 0.0f;
	r.w = q1.w * length;

	m = 1.0 / sqrt (r.w * r.w + r.x * r.x);
	r1 = sqrt ((1.0 + r.y) * 0.5);
	r2 = sqrt ((1.0 - r.y) * 0.5);

	res.x = sqrt (length) * r2 * r.x * m;
	res.y = sqrt (length) * r1;
	res.z = q1.z;
	res.w = sqrt (length) * r1 * r.w * m;

}


// QuatDot: Funció que retorna el producte escalar de dos quaternions unitaris
//    PARÀMETRES:	q1: First quaternion
//					q2: Destination quaternion
//    COMETARIS: Quaternion has to be normalized (i.e. it's a unit quaternion)
GLdouble QuatDot(GL_Quat q1, GL_Quat q2)
{
  return (GLdouble)(q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z);
}


// QuatLength: Calculates the length of a quaternion
GLdouble QuatLength(GL_Quat q1)
{
  return sqrt (q1.w * q1.w + q1.x * q1.x + q1.y * q1.y + q1.z * q1.z);
}


// QuatNegate: egates vector part of a quaternion
//    PARÀMETRES:	q1: Source quaternion
//					q2: Destination quaternion
void QuatNegate(GL_Quat q1, GL_Quat &q2)
{
	QuatCopy(q1, q2);

	QuatNormalize(q2);
	q2.x = -q2.x;
	q2.y = -q2.y;
	q2.z = -q2.z;
}


// QuatExp: Calculates exponent of a quaternion
//    PARÀMETRES:	q1: Source quaternion
//					q2: Destination quaternion
void QuatExp(GL_Quat q1, GL_Quat &q2)
{
	GLdouble  len1, len2;

	len1 = (GLdouble) sqrt (q1.x * q1.x + q1.y * q1.y + q1.z * q1.z);
	if (len1 > 0.0) 
		len2 = (GLdouble)sin(len1) / len1; 
	else 
		len2 = 1.0;

	q2.x = q1.x * len2;
	q2.y = q1.y * len2;
	q2.z = q1.z * len2;
	q2.w = cos (len1);
}


// QuatLog: Calculates natural logarithm of a quaternion
//    PARÀMETRES:	q1: Source quaternion
//					q2: Destination quaternion
void QuatLog(GL_Quat q1, GL_Quat &q2)
{
	GLdouble  length;

	length = sqrt (q1.x * q1.x + q1.y * q1.y + q1.z * q1.z);

//make sure we do not divide by 0
	if (q1.w != 0.0) 
		length = atan (length / q1.w); 
	else length = (GLdouble)pi/2;

	q2.w = 0.0f;
	q2.x = q1.x * length;
	q2.y = q1.y * length;
	q2.z = q1.z * length;
}


// QuatLnDif: Computes the "natural log difference" of two quaternions,
//				q1 and q2 as  ln(qinv(q1)*q2)
//    PARÀMETRES:	q1,q2: Source quaternions
//					res: Destination quaternion
void QuatLnDif(GL_Quat q1, GL_Quat q2, GL_Quat &res)
{

	GL_Quat inv, dif;
	GLdouble  len, len1, s;

	QuatInverse(q1,inv);
	QuatMul(inv, q2, dif);
	len = sqrt (dif.x*dif.x + dif.y*dif.y + dif.z*dif.z);
	s = QuatDot (q1, q2);
	if (s != 0.0) len1 = atan (len / s); else len1 = pi/2;
	if (len != 0.0) len1 /= len;
	res.w = 0.0;
	res.x = dif.x * len1;
	res.y = dif.y * len1;
	res.z = dif.z * len1;
//	QuatCopy(temp, out);
}
