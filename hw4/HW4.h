#ifndef HW4_LAST
#define HW4_LAST



#include "HW.h"
#include <QGLShaderProgram>
#include <QtOpenGL>


#define MAXGRID 65



class HW4 : public HW {
    Q_OBJECT
public:
    HW4(QWidget *parent = 0);		// constructor
    QGroupBox*	controlPanel	();		// create control panel
    void		reset		();
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
    void        getTexCoords();


public slots:
    void        wave();
    void        begintimer();
    void        stopTimer();
    void        displayChange(int);
    void        selectMode(int);
    void        setSpeed(int value);
    void        setsize(int value);

protected:
    void		initializeGL	();		// init GL state
    void		resizeGL	(int, int);	// resize GL widget
    void		paintGL		();		// render GL scene

    void		initShader1	();		// texture mapping shader
    void		initShader2	();		// wireframe rendering shader
    void        initShader3 ();

    void        initPosition();


    void        mousePressEvent(QMouseEvent *event);
    void        mouseReleaseEvent(QMouseEvent *event);
    void        mouseMoveEvent(QMouseEvent *event);

private:
    int		  m_winW;			// window width
    int		  m_winH;			// window height
    QGLShaderProgram  m_program[3];			// GLSL programs


    std::vector<vec3> m_points;			// vector of 2D points (XY)
    std::vector<vec2> m_coords;			// vector of 2D coords (UV)

    size_t		  m_numPoints;			// number of 2D points
    QMatrix4x4	  m_ModelMatrix;		// 4x4 transformation matrix

    QMatrix4x4	  m_ViewMatrix;
    QMatrix4x4	  m_ProjectMatrix;
    QMatrix4x4	  m_NormalMatrix;

    QPushButton   *m_PushbottonGo;
    QPushButton   *m_PushbottonStop;
    QTimer        *timer;
    QComboBox     *m_comboBox;
    QComboBox     *m_comboBox_mode;
    QSlider       *m_sliderGrid;
    QSlider       *m_sliderSpeed;
    QSpinBox      *m_spinboxGrid;
    QSpinBox      *m_spinboxSpeed;
    QLabel        *m_labelSpeed;
    QLabel        *m_labelGrid;


    QImage		  m_image;			// texture image
    GLuint		  m_texture;			// shader index to texture unit
    GLuint		  m_uniform[3][16];		// uniform vars for two shaders and <16 vars


    //members from demo

    bool waving;
    bool leftButton;

    float sphi;
    float stheta;
    float sdepth;
    float zNear;
    float zFar;
    float aspect;
    float dt;
    int grid;
    int displayMode;
    int resetMode;
    int downX;
    int downY;

    float force[MAXGRID][MAXGRID];
    float veloc[MAXGRID][MAXGRID];
    float posit[MAXGRID][MAXGRID];
    float texCoords[MAXGRID][MAXGRID][2];


    // for hw4

    float shininess;
    vec3 a_color;
    vec3 lightcolor;
    vec3 ambient;
    vec3 ambientProduct;
    vec3 diffuseProduct;
    vec3 specularProduct;

    vec3 lightDirection;

    std::vector<vec3> m_normals;



};



#endif // HW4

