//
// glrnRecordNormals.cpp - Version 1.0, 4/16/2017.
//  Display recorded normals for OpenGL surface3 normal debugging
//  Developed for Math 155A, Winter 2017
//  Author: Sam Buss
//  All rights reserved.
//  May be used freely for academic purposes.
//  Software is as-is and without any warranties.
//

#include <math.h>			// For math routines (such as sqrt & trig).
#include <stdio.h>
#include <vector>
#include <GL/glut.h>
#include "glrnRenderNormals.h"

// ****************************
// Global variables
// ****************************

float glrnNormalDrawLength = 0.3f;
float glrnNormalDrawColor[3] = { 0.5f, 1.0f, 0.5f };
float glrnCurrentModelview[16];				// Effectively 3x4 (by assumption)
float glrnCurrentInverseTranspose[16];		// Only 3x3 part is relevant
float glrnCurrentNormal[3];
std::vector<float> glrnStoredValues;

// *****************************************************
// Templates for local functions. For internal use only.
// *****************************************************
void glrnToWorldCoordinates(const float *fromVec, float *toVec, const float* matrix44);
void glrnToWorldCoordsNormal(const float *fromVec, float *toVec);

// *****************************************************
// Main glrnRecordNormals routines.
// *****************************************************

// Set the length at which the normal vectors will be drawn
//   Use length equal to zero to disable drawing normals
void glrnSetDrawLength(float length)
{
	glrnNormalDrawLength = length;
}

// Set the color which the normal vectors will be drawn
//   Use length equal to zero to disable drawing normals
void glrnSetDrawColor(float r, float g, float b)
{
	glrnNormalDrawColor[0] = r;
	glrnNormalDrawColor[1] = g;
	glrnNormalDrawColor[2] = b;
}


// Call this routine *BEFORE* calling glBegin where you plan to record normals
// Function: Saves the current modelview matrix.
void glrnBegin()
{
	glGetFloatv(GL_MODELVIEW_MATRIX, glrnCurrentModelview);
	if (glrnCurrentModelview[3] != 0.0f || glrnCurrentModelview[7] != 0.0f || glrnCurrentModelview[11] != 0.0f) {
		throw std::exception("ModelView matrix holds non-affine transformation");
	}
	 
	// Compute the transposed inverse of the 3x3 portion
	const float *m = glrnCurrentModelview;
	float minor00 = (m[5] * m[10] - m[6] * m[9]);
	float minor10 = -(m[4] * m[10] - m[6] * m[8]);
	float minor20 = (m[4] * m[9] - m[5] * m[8]);
	float minor01 = -(m[1] * m[10] - m[2] * m[9]);
	float minor11 = (m[0] * m[10] - m[2] * m[8]);
	float minor21 = -(m[0] * m[9] - m[1] * m[8]);
	float minor02 = (m[1] * m[6] - m[2] * m[5]);
	float minor12 = -(m[0] * m[6] - m[2] * m[4]);
	float minor22 = (m[0] * m[5] - m[1] * m[4]);
	float determinant = m[0] * minor00  + m[1] * minor10 + m[2] * minor20;
	if (determinant == 0.0f) {
		throw std::exception("ModelView matrix is not full rank");
	}
	float detInv = 1.0f / determinant;

	float* t = glrnCurrentInverseTranspose;
	t[0] = minor00*detInv;
	t[1] = minor10*detInv;
	t[2] = minor20*detInv;
	t[4] = minor01*detInv;
	t[5] = minor11*detInv;
	t[6] = minor21*detInv;
	t[8] = minor02*detInv;
	t[9] = minor12*detInv;
	t[10] = minor22*detInv;
	t[3] = t[7] = t[11] = t[12] = t[13] = t[14] = 0.0f;
	t[15] = 1.0f;
}

// Call this routine every time you call glNormal..(), with exactly the
//    same parameters.  
// The parameter is the current normal.
// Function: Records the current normal.
void glrnNormal3f(float x, float y, float z)
{
	glrnCurrentNormal[0] = x;
	glrnCurrentNormal[1] = y;
	glrnCurrentNormal[2] = z;
	if (glrnCurrentNormal[0]==0.0f && glrnCurrentNormal[1] == 0.0f && glrnCurrentNormal[2] == 0.0f) {
		throw std::exception("Normal vector is zero");
	}
}

// Call this routine every time you call glVertex..(), with exactly the
//    same parameters.  
// The parameter is the current vertex position.
// Records the vertex in world coordinates, for later rendering.
void glrnVertex3f(float x, float y, float z)
{
	if (glrnNormalDrawLength <= 0.0f) {
		return;
	}
	const float localPt[3] = { x, y, z };
	float worldVertex1[6];						// Store the current vertex in first three entries
	glrnToWorldCoordinates(localPt, worldVertex1, glrnCurrentModelview);
	float* worldVertex2 = worldVertex1 + 3;		// Store the other end of the normal here
	float normalWorld[3];
	glrnToWorldCoordinates(glrnCurrentNormal, normalWorld, glrnCurrentInverseTranspose);
	float normSq = 0.0f;
	for (int i = 0; i < 3; i++) {
		normSq += normalWorld[i] * normalWorld[i];
	}
	if (normSq <= 0.0f) {
		throw std::exception("Normal vector is zero");
	}
	float scaleFactor = glrnNormalDrawLength / sqrtf(normSq);
	for (int i = 0; i < 3; i++) {
		worldVertex2[i] = worldVertex1[i] + scaleFactor*normalWorld[i];
	}
	for (int i = 0; i < 6; i++) {
		glrnStoredValues.push_back(worldVertex1[i]);
	}
}

// Call this routine to render the normals.
void glrnRenderNormals() {

	// Save current color, save enable flag for lights
	GLboolean lightingEnabled = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);
	float savedColor[4];
	glGetFloatv(GL_CURRENT_COLOR, savedColor);

	// Draw the normals
	glColor3fv(glrnNormalDrawColor);
	glMatrixMode(GL_MODELVIEW);		// Just in case!
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);
	auto i = glrnStoredValues.cbegin();
	for (; i != glrnStoredValues.cend(); i += 3) {
		glVertex3f(*i, *(i+1), *(i+2));
	}
	glEnd();
	glPopMatrix();
	glrnInit();

	// Restore current color, restore enabling of lights
	if (lightingEnabled) {
		glEnable(GL_LIGHTING);
	}
	glColor4fv(savedColor);
}



// Call this routine to reinitialize without drawing normals,
//   and start a new pass (not necessary if glrnRenderNormals()
//   has been called.
void glrnInit()
{
	glrnStoredValues.clear();
}

// *****************************************************
// Local functions. For internal use only.
// *****************************************************

// glrnToWorldCoordinates 
//   - transforms a vector by current Modelview matrix
//   - the two arguments are 3-vectors
void glrnToWorldCoordinates(const float *fromVec, float *toVec, const float* matrix44)
{
	const float* mPtr = matrix44;
	float toVecHomogeneous[4];
	float* tPtr = toVecHomogeneous;
	for (int i = 0; i < 4; i++, tPtr++, mPtr++) {
		const float* fPtr = fromVec;
		const float* rowPtr = mPtr;
		*tPtr = 0.0f;
		for (int j = 0; j < 3; j++, fPtr++, rowPtr += 4) {
			*tPtr += (*fPtr)*(*rowPtr);
		}
		*tPtr += *rowPtr;		// Translation amount (last column of M)
	}
	//assert(toVecHomogeneous[3] != 0.0f);
	float scale = 1.0f / toVecHomogeneous[3];
	for (int i = 0; i < 3; i++) {
		*(toVec+i) = toVecHomogeneous[i] * scale;
	}
}


