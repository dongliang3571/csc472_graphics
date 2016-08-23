This is a Qt project, in order to run this you have to have Qt installed!

Remember to **uncomment** following line in the HW.h if you're on Windows or Linux platform.......
```
#include <GL/glu.h>
```

<strike>HW3 might not work properly on Mac I couldn't find out why. But if you can't display image on Mac, try Linux and Windows.....</strike>


Solve the issue which image is not showing properly on Mac.....

Important Notes
===============

- `gl_ModelViewProjectionMatrix` in shader file(normally is multiplied by a Position attribute variable, eg. `gl_Position = gl_ModelViewProjectionMatrix * a.Position`) is controlled by glfunctions, eg. `glLoadIdentity()`,
	`glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)`, `glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z)` and so on, these are outside shader files, normally they reside in program methods, like
  ```
  void HW3a::resizeGL(int w, int h)
  {
	  // save window dimensions
	  m_winW = w;
	  m_winH = h;

	  // compute aspect ratio
	  float ar = (float) w / h;
	  float xmax, ymax;
	  if(ar > 1.0) {		// wide screen
		  xmax = ar;
		  ymax = 1.;
	  }
	  else {			// tall screen
		  xmax = 1.;
		  ymax = 1 / ar;
	  }

	  // set viewport to occupy full canvas
	  glViewport(0, 0, w, h);

	  // init viewing coordinates for orthographic projection
	  glLoadIdentity();                               ////////////////Control gl_ModelViewProjectionMatrix//////////// 
	  glOrtho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);   ////////////////Control gl_ModelViewProjectionMatrix//////////// 
  }
  ```


