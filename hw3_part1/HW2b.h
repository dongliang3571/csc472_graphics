//
//  HW2b.h
//  hw
//
//  Created by dong liang on 10/15/15.
//
//

#ifndef HW2b_h
#define HW2b_h



#include "HW.h"
#include <QGLShaderProgram>
#include <QtOpenGL>


class HW2b : public HW {
    Q_OBJECT
public:
    HW2b		(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();
    void		initVertexBuffer();		// init vertices
    void		initShaders	();		// init shaders
    void        divide_triangle(vec2, vec2, vec2, int);
    void        triangle(vec2, vec2, vec2);
    void        reset();
    
public slots:
    void changeDividing(int value); //Slider calls his function to trigger dividing
    void changeTheta(int angle);
    void setToTwist(int state);
    
    
protected:
    void		initializeGL	();		// init GL state
    void		resizeGL	(int, int);	// resize GL widget
    void		paintGL		();		// render GL scene
    
    
private:
    int		  m_winW;			// window width
    int		  m_winH;			// window height
    QGLShaderProgram  m_program;			// GLSL program
    QMatrix4x4	  m_ModelMatrix;
    GLuint		  m_u_ModelMatrix;		// shader index to transformation matrix
    GLuint        m_u_isTwist;
    GLuint        m_u_angle;
    
    std::vector<vec2> m_points;			// vector of 2D points (XY)
    std::vector<vec3> m_colors;
    size_t		  m_numPoints;			// number of 2D points
    
    vec2 vertices[3];
    
    //    GLuint		  m_u_Color;		// shader index to transformation matrix
    
    int m_subnumber;
    int m_isTwist;
    float m_theta;
    bool m_colorChange;
    
    
    
    QSlider *m_sliderDividing;
    QSpinBox *m_spinBoxDividing;
    
    QSlider *m_sliderTheta;
    QSpinBox *m_spinBoxTheta;
    
    QCheckBox *m_checkBoxTwist;
    
    
};


#endif /* HW2b_h */
