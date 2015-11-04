#ifndef HW3B
#define HW3B

#include "HW.h"
#include <QGLShaderProgram>
#include <QtOpenGL>


// ----------------------------------------------------------------------
// standard include files
//
#define MAXGRID 63



class HW3b : public HW {
    Q_OBJECT
public:
    HW3b		(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    
    void		initVertexBuffer();		// init vertices
    void		initShaders	();		// init shaders
    void		initTexture	();		// init texture image
    void        initPoints();
    //functions from demo
    void        getforce();
    void        getvelocity();
    void        getposition();
    void        copy(float vec0[3], float vec1[3]);
    void        sub(float vec0[3], float vec1[3], float vec2[3]);
    void        add(float vec0[3], float vec1[3], float vec2[3]);
    void        scalDiv(float vec[3], float c);
    void        cross(float vec0[3], float vec1[3], float vec2[3]);
    void        norm(float vec[3]);
    void        set(float vec[3], float x, float y, float z);
    void        getFaceNorms();
    void        getVertNorms();
    void        getFaceNormSegs();
    void        getTexCoords();

public slots:
    void        wave();
    void        begintimer();
    
protected:
    void		initializeGL	();		// init GL state
    void		resizeGL	(int, int);	// resize GL widget
    void		paintGL		();		// render GL scene

    void		initShader1	();		// texture mapping shader
    void		initShader2	();		// wireframe rendering shader
    
    void        initPosition();
   
    
    void        mousePressEvent(QMouseEvent *event);
    void        mouseReleaseEvent(QMouseEvent *event);
    void        mouseMoveEvent(QMouseEvent *event);

private:
    int		  m_winW;			// window width
    int		  m_winH;			// window height
    QGLShaderProgram  m_program[2];			// GLSL programs

    
    std::vector<vec3> m_points;			// vector of 2D points (XY)
    std::vector<vec2> m_coords;			// vector of 2D coords (UV)
    
    size_t		  m_numPoints;			// number of 2D points
    QMatrix4x4	  m_ModelMatrix;		// 4x4 transformation matrix
    
    QMatrix4x4	  m_ViewMatrix;
    QMatrix4x4	  m_ProjectMatrix;
    
    QPushButton   *m_PushbottonGo;
    QTimer        *timer;
    
    
    QImage		  m_image;			// texture image
    GLuint		  m_texture;			// shader index to texture unit
    GLuint		  m_uniform[2][16];		// uniform vars for two shaders and <16 vars


    //members from demo

    bool waving;
    bool editing;
    bool drawFaceNorms;
    bool antialias;
    bool envMap;
    bool m_wire;
    
    // viewing
    float sphi;
    float stheta;
    float sdepth;
    float zNear;
    float zFar;
    float aspect;
    float dt;
    int grid;
    
    bool leftButton;
    
    
    int displayMode;
    int resetMode;
    
    float force[MAXGRID][MAXGRID];
    float veloc[MAXGRID][MAXGRID];
    float posit[MAXGRID][MAXGRID];
    float vertNorms[MAXGRID][MAXGRID][3];
    float faceNorms[2][MAXGRID][MAXGRID][3];
    float faceNormSegs[2][2][MAXGRID][MAXGRID][3];
    float texCoords[MAXGRID][MAXGRID][2];

};




#endif // HW3B

