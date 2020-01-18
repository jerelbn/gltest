README / INSTRUCTIONS FOR USE
-----------------------------
glrnRenderNormals

Files:
glrnRenderNormals.cpp
glrnRenderNormals.h

Draws normals in an OpenGL scene with calls duplicating calls to glNormal3f and glVertex3f.
Intended for use in debugging normals in an OpenGL scene.

Usage. Do the following:

0. Include the two glrnRenderNormals files in your project. Or create a static library to hold glrnRenderNormals object file.

1. Include #include "glrnRenderNormals.h" in your code.

2. Before you start drawing, e.g. *before* a call to glBegin(), call 

       glrnBegin().
	   
   Repeat this whenever the ModelView matrix may have changed.
   
3. Everywhere you give commands

       glNormal3f(a,b,c);
	   glVertex3f(c,d,e);
	   
   add the two lines:
   
       glrnNormal3f(a,b,c);
	   glrnVertex3f(c,d,e);

   Two "glrn..." commands can be before or after the two "gl..." commands, but they must be given in the indicated order.
   
4. Call glrnRenderNormals() once the entire scene is drawn (after any final glEnd() command).

5. Change the normals' length by calling
   
       glrnNormalDrawLength(float len);
	   
   Initially, the draw length is 0.3.  Setting the length to 0.0 turns off the drawing of normals.
   
========================================================================
Main routines in the code are:

glrnNormalDrawlength(float len);
	Changes the length of normals a drawn, see 5. above.
	
glrnNormal3f(float x, float y, float z);
	Records the normal before a call to glrnVertex3f();
	
glrnVertex3f(float x, float y, float z);
	Saves information about how to draw the normal at the vertex for later rendering.
	
glrnRenderNormals();
	Renders the saved normals.
	
glrnInit();
	Clears all saved normals. (Called by glrnRenderNormals().)

============================
Author: Sam Buss
Code freely available. As is, no warranty of fitness or correctness.
