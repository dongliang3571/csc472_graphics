//
//  HW2a.h
//  hw
//
//  Created by dong liang on 10/13/15.
//
//

#ifndef HW2a_h
#define HW2a_h


#include "HW.h"
#include <QGLShaderProgram>
#include <QtOpenGL>


class HW2a : public HW {
    Q_OBJECT
public:
    HW2a		(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();
    void		initVertexBuffer();		// init vertices
    void		initShaders	();		// init shaders
    
    public slots:

    
    
protected:
    void		initializeGL	();		// init GL state
    void		resizeGL	(int, int);	// resize GL widget
    void		paintGL		();		// render GL scene
    
    
private:
    int		  m_winW;			// window width
    int		  m_winH;			// window height
    QGLShaderProgram  m_program;			// GLSL program
   
    std::vector<vec2> m_points;			// vector of 2D points (XY)
    std::vector<vec3> m_colors;
    size_t		  m_numPoints;			// number of 2D points
    
    GLuint		  m_u_Color;		// shader index to transformation matrix
   
};



#endif /* HW2a_h */
