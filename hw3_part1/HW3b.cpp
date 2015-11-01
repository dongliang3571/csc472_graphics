
// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// HW3b.cpp - HW3b class
//
// Written by: George Wolberg, 2015
// ===============================================================

#include "HW3b.h"

enum {
    MV,
    VIEWMATRIX,
    PROJECTMATRIX,
    SAMPLER
};


enum {WIREFRAME, HIDDENLINE, FLATSHADED, SMOOTHSHADED, TEXTURED};
enum {FULLSCREEN, FACENORMALS, ANTIALIAS, ENVMAP};
enum {WEAK, NORMAL, STRONG};
enum {SMALL, MEDIUM, LARGE, XLARGE};
enum {CURRENT, FLAT, SPIKE, DIAGONALWALL, SIDEWALL, HOLE,
    MIDDLEBLOCK, DIAGONALBLOCK, CORNERBLOCK, HILL, HILLFOUR};



#define SQRTOFTWOINV 1.0 / 1.414213562

unsigned int TexId1, TexId2;


int downX, downY;
bool middleButton = false;
GLfloat lightPosition[] = { 0.0, 0.0, 1.0, 1.0};
int displayMenu, otherMenu, speedMenu, sizeMenu, resetMenu, mainMenu;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::HW3b:
//
// HW3b constructor.
//
HW3b::HW3b(QWidget *parent)
    : HW(parent)
{
    waving = true;
    editing = false;
    drawFaceNorms = false;
    antialias = false;
    envMap = false;
    grid = 17;
    dt=0.008;
    displayMode = TEXTURED;
    resetMode = MIDDLEBLOCK;
    sphi=90.0;stheta=45.0;
    sdepth = 5.0/4.0 * (grid+3);
    zNear=15.0; zFar=100.0;
    aspect = 5.0/4.0;
    timer = new QTimer(this);
    leftButton = false;
    
    connect(timer, SIGNAL(timeout()), this, SLOT(wave()));

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW3b::controlPanel()
{
    // init group box
    QGroupBox *groupBox = new QGroupBox("Homework 3a");


    groupBox->setMinimumWidth(300);
//
//    m_sliderSubdiv = new QSlider(Qt::Horizontal);
//    m_sliderSubdiv->setRange(0, 8);
//    m_sliderSubdiv->setValue(0);
//
//    // create spinBox
//    m_spinBoxSubdiv = new QSpinBox;
//    m_spinBoxSubdiv->setRange(0, 8);
//    m_spinBoxSubdiv->setValue(0);
//
//    m_sliderTheta = new QSlider(Qt::Horizontal);
//    m_sliderTheta->setRange(0, 360);
//    m_sliderTheta->setValue(0);
//
//    m_spinBoxTheta = new QSpinBox;
//    m_spinBoxTheta->setRange(0, 360);
//    m_spinBoxTheta->setValue(0);
//
    QGridLayout *layout = new QGridLayout;
//    QLabel *label = new QLabel("Subdivide");
//    QLabel *label2 = new QLabel("Theta");
//
//    m_checkBoxTwist = new QCheckBox("Twist");
//    m_checkBoxWire = new QCheckBox("Wire");
//
//    layout->addWidget(label,0,0,1,1);
//    layout->addWidget(m_sliderSubdiv,0,1,1,3);
//    layout->addWidget(m_spinBoxSubdiv,0,4);
//
//    layout->addWidget(label2,2,0,3,1);
//    layout->addWidget(m_sliderTheta, 2, 1, 3, 3);
//    layout->addWidget(m_spinBoxTheta,3,4);
//
//    layout->addWidget(m_checkBoxTwist, 5,0,2,2);
//    layout->addWidget(m_checkBoxWire,5,2,2,2);
//
//    connect(m_sliderSubdiv, SIGNAL(valueChanged(int)), this, SLOT(changeSubdiv(int)));
//    connect(m_spinBoxSubdiv, SIGNAL(valueChanged(int)), this, SLOT(changeSubdiv(int)));
//
//    connect(m_sliderTheta, SIGNAL(valueChanged(int)),this, SLOT(changeTheta(int)));
//    connect(m_spinBoxTheta, SIGNAL(valueChanged(int)), this, SLOT(changeTheta(int)));
//
//    connect(m_checkBoxTwist, SIGNAL(stateChanged(int)), this, SLOT(changeTwist(int)));
//    connect(m_checkBoxWire, SIGNAL(stateChanged(int)), this, SLOT(changeWire(int)));
//
    
    m_PushbottonGo = new QPushButton("go");
    layout->addWidget(m_PushbottonGo);
    connect(m_PushbottonGo, SIGNAL(clicked()), this, SLOT(begintimer()));
    groupBox->setLayout(layout);
    

    return(groupBox);
}

void HW3b::begintimer(){
    timer->start(0);
}

void HW3b::wave()
{
    
    getforce();
    getvelocity();
    getposition();
    initVertexBuffer();
    updateGL();
    
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW3b::initializeGL()
{
    // initialize GL function resolution for current context
    initializeGLFunctions();

    // init texture
    initTexture();

    // init vertex and fragment shaders
    initShaders();
    initPoints();
    // initialize vertex buffer and write positions to vertex shader
    initVertexBuffer();

    // init state variables
    glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
    glColor3f   (1.0, 1.0, 0.0);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initTexture:
//
// Initialize texture.
//
void
HW3b::initTexture()
{
    // read image from file
    m_image.load(QString(":/envmap.ppm"));

    // convert jpg to GL formatted image
    QImage qImage = QGLWidget::convertToGLFormat(m_image);

    // init vars
    int w = qImage.width ();
    int h = qImage.height();

    // generate texture name and bind it
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, qImage.bits());

//    loading second image
    m_image.load(QString(":/texmap.ppm"));

//     convert jpg to GL formatted image
    qImage = QGLWidget::convertToGLFormat(m_image);

    // init vars
    w = qImage.width ();
    h = qImage.height();

    // generate texture name and bind it
    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, qImage.bits());

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initShaders:
//
// Initialize vertex and fragment shaders.
//
void
HW3b::initShaders()
{
    initShader1();
//    initShader2();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initShader1:
//
// Initialize vertex and fragment shaders for texture mapping.
//
void
HW3b::initShader1()
{
    // compile vertex shader
    if(!m_program[0].addShaderFromSourceFile(QGLShader::Vertex, ":/vshader3b1.glsl")) {
        QMessageBox::critical(0, "Error", "Vertex shader error", QMessageBox::Ok);
        QApplication::quit();
    }

    // compile fragment shader
    if(!m_program[0].addShaderFromSourceFile(QGLShader::Fragment, ":/fshader3b1.glsl")) {
        QMessageBox::critical(0, "Error", "Fragment shader error",QMessageBox::Ok);
        QApplication::quit();
    }

    
    glBindAttribLocation(m_program[0].programId(), ATTRIB_VERTEX, "a_Position");
    

    
    glBindAttribLocation(m_program[0].programId(), ATTRIB_TEXTURE_POSITION, "a_TexCoord");

    // link shader pipeline; attribute bindings go into effect at this point
    if(!m_program[0].link()) {
        QMessageBox::critical(0, "Error", "Could not link shader", QMessageBox::Ok);
        QApplication::quit();
    }

    // get storage location of u_ModelMatrix in vertex shader
    m_uniform[0][MV] = glGetUniformLocation(m_program[0].programId(), "u_ModelMatrix");
    if((int) m_uniform[0][MV] < 0) {
        qDebug() << "Failed to get the storage location of u_ModelMatrix";
        exit(-1);
    }

    // get storage location of u_Theta in vertex shader
//    m_uniform[0][VIEWMATRIX] = glGetUniformLocation(m_program[0].programId(), "u_ViewMatrix");
//    if((int) m_uniform[0][VIEWMATRIX] < 0) {
//        qDebug() << "Failed to get the storage location of u_ViewMatrix";
//        exit(-1);
//    }


//    m_uniform[0][PROJECTMATRIX] = glGetUniformLocation(m_program[0].programId(), "u_ProjectMatrix");
//    if((int) m_uniform[0][PROJECTMATRIX] < 0) {
//        qDebug() << "Failed to get the storage location of u_ProjectMatrix";
//        exit(-1);
//    }

    // get storage location of u_Sampler in fragment shader
    m_uniform[0][SAMPLER] = glGetUniformLocation(m_program[0].programId(), "u_Sampler");
    if((int) m_uniform[0][SAMPLER] < 0) {
        qDebug() << "Failed to get the storage location of u_Sampler";
        exit(-1);
    }

    // bind the glsl program
    glUseProgram(m_program[0].programId());

    initPosition();
    
    
    glUniform1i(m_uniform[0][SAMPLER], 1);
}

void HW3b::initPosition(){
    
    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(0.0,0.0,-sdepth);
    //
    m_ModelMatrix.rotate(-stheta, 1.0, 0.0, 0.0);
    m_ModelMatrix.rotate(sphi, 0.0, 0.0, 1.0);
    m_ModelMatrix.translate(-(float)((grid+1)/2-1), -(float)((grid+1)/2-1), 0.0);
    
    m_ViewMatrix.setToIdentity();
    //    m_ViewMatrix.lookAt(vec3(0,0,sdepth), vec3(grid/2,grid/2,0.0), vec3(0.0,1.0,0.0));
//    glUniformMatrix4fv(m_uniform[0][VIEWMATRIX], 1, GL_FALSE, m_ViewMatrix.constData());
    
    m_ProjectMatrix.setToIdentity();
    m_ProjectMatrix.perspective(65.0, aspect, 0, zFar);
    
    m_ModelMatrix = m_ViewMatrix * m_ModelMatrix;
    m_ModelMatrix = m_ProjectMatrix * m_ModelMatrix;
//    glUniformMatrix4fv(m_uniform[0][PROJECTMATRIX], 1, GL_FALSE, m_ProjectMatrix.constData());
    glUniformMatrix4fv(m_uniform[0][MV], 1, GL_FALSE, m_ModelMatrix.constData());
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initShader2:

// Initialize vertex and fragment shaders for wireframe rendering.

//void
//HW3b::initShader2()
//{
//    // compile vertex shader
//    if(!m_program[1].addShaderFromSourceFile(QGLShader::Vertex, ":/vshader3a2.glsl")) {
//        QMessageBox::critical(0, "Error", "Vertex shader error", QMessageBox::Ok);
//        QApplication::quit();
//    }
//
//    // compile fragment shader
//    if(!m_program[1].addShaderFromSourceFile(QGLShader::Fragment, ":/fshader3a2.glsl")) {
//        QMessageBox::critical(0, "Error", "Fragment shader error",QMessageBox::Ok);
//        QApplication::quit();
//    }
//
//    // bind the attribute variable in the glsl program with a generic vertex attribute index;
//    // values provided via ATTRIB_VERTEX will modify the value of "a_position")
//    glBindAttribLocation(m_program[1].programId(), ATTRIB_VERTEX, "a_Position");
//
//    // bind the attribute variable in the glsl program with a generic vertex attribute index;
//    // values provided via ATTRIB_TEXTURE_POSITION will modify the value of "a_TexCoord")
//    glBindAttribLocation(m_program[1].programId(), ATTRIB_TEXTURE_POSITION, "a_TexCoord");
//
//    // link shader pipeline; attribute bindings go into effect at this point
//    if(!m_program[1].link()) {
//        QMessageBox::critical(0, "Error", "Could not link shader", QMessageBox::Ok);
//        QApplication::quit();
//    }
//
//    // get storage location of u_ModelMatrix in vertex shader
//    m_uniform[1][MV] = glGetUniformLocation(m_program[1].programId(), "u_ModelMatrix");
//    if((int) m_uniform[1][MV] < 0) {
//        qDebug() << "Failed to get the storage location of u_ModelMatrix";
//        exit(-1);
//    }
//
//    // get storage location of u_Theta in vertex shader
//    m_uniform[1][THETA] = glGetUniformLocation(m_program[1].programId(), "u_Theta");
//    if((int) m_uniform[1][THETA] < 0) {
//        qDebug() << "Failed to get the storage location of u_Theta";
//        exit(-1);
//    }
//
//    // get storage location of u_Twist in vertex shader
//    m_uniform[1][TWIST] = glGetUniformLocation(m_program[1].programId(), "u_Twist");
//    if((int) m_uniform[1][TWIST] < 0) {
//        qDebug() << "Failed to get the storage location of u_Twist";
//        exit(-1);
//    }
//
//    // bind the glsl program
//    glUseProgram(m_program[1].programId());
//
//    // init model matrix; pass it to vertex shader along with theta and twist flag
//    m_ModelMatrix.setToIdentity();
//    glUniformMatrix4fv(m_uniform[1][MV], 1, GL_FALSE, m_ModelMatrix.constData());
//    glUniform1f(m_uniform[1][THETA], m_theta);
//    glUniform1i(m_uniform[1][TWIST], m_twist);
//}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::initVertexBuffer:
//
// Initialize vertex buffer.
//
void
HW3b::initVertexBuffer()
{
    
    int i,j;
    
//    for(i=0; i<grid; ++i){
//        for (j=0; j<grid; ++j)
//            m_points.push_back(vec3((float)i,(float)j,posit[i][j]));
//    
//    }
//    for(i=0; i<grid; ++i){
//        for (j=0; j<grid; ++j)
//            m_points.push_back(vec3((float)j,(float)i,posit[i][j]));
//    }
    
    for (i=0; i<grid-1; i++) {
        for (j=0; j<grid-1; j++){
            m_points.push_back(vec3((float)i,(float)j,posit[i][j]));
            m_points.push_back(vec3((float)(i),(float)(j+1),posit[i][j+1]));
//            m_points.push_back(vec3((float)(i+1),(float)(j+1),posit[i+1][j+1]));
            m_points.push_back(vec3((float)(i+1),(float)(j),posit[i+1][j]));
            
            m_points.push_back(vec3((float)(i),(float)(j+1),posit[i][j+1]));
            m_points.push_back(vec3((float)(i+1),(float)(j+1),posit[i+1][j+1]));
            m_points.push_back(vec3((float)(i+1),(float)(j),posit[i+1][j]));
        }
    }
    
//    qDebug() <<m_points[5].z()<< "Failed to get the storage location of u_ProjectMatrix";
//    exit(-1);
    
    m_numPoints = m_points.size();	// save number of vertices

    static GLuint vertexBuffer = -1;
    glGenBuffers(1, &vertexBuffer);
    // bind the buffer to the GPU; all future drawing calls gets data from this buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // copy the vertices from CPU to GPU
    
    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_points[0], GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, false, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_VERTEX);

    getTexCoords();
//    for (i=0; i<grid*grid*2; ++i) {
//        m_coords.push_back(vec2());
//    }
    
//    for(i=0; i<grid; ++i){
//        for (j=0; j<grid; ++j)
//            m_coords.push_back(vec2(texCoords[i][j][0],texCoords[i][j][1]));
//        
//    }
//    for(i=0; i<grid; ++i){
//        for (j=0; j<grid; ++j)
//            m_points.push_back(vec3((float)i,(float)j,posit[i][j]));
//    }
    
    for (i=0; i<grid-1; i++) {
        for (j=0; j<grid-1; j++){
            m_coords.push_back(vec2(texCoords[i][j][0],texCoords[i][j][1]));
            m_coords.push_back(vec2(texCoords[i][j+1][0],texCoords[i][j+1][1]));
//            m_coords.push_back(vec2(texCoords[i+1][j+1][0],texCoords[i+1][j+1][1]));
            m_coords.push_back(vec2(texCoords[i+1][j][0],texCoords[i+1][j][1]));
            
            m_coords.push_back(vec2(texCoords[i][j+1][0],texCoords[i][j+1][1]));
            m_coords.push_back(vec2(texCoords[i+1][j+1][0],texCoords[i+1][j+1][1]));
            m_coords.push_back(vec2(texCoords[i+1][j][0],texCoords[i+1][j][1]));
            
        }
    }
    //Create texture coordinates buffer
    static GLuint texCoordBuffer = -1;
    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_coords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_TEXTURE_POSITION, 2, GL_FLOAT, false, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_TEXTURE_POSITION);

    m_points.clear();
    m_coords.clear();

}




void
HW3b::resizeGL(int w, int h)
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
    glLoadIdentity();
    glOrtho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::paintGL:
//
// Update GL scene.
//
void
HW3b::paintGL()
{
    // clear canvas with background color
    glClear(GL_COLOR_BUFFER_BIT);

    // draw texture mapped triangles
    switch (displayMode) {
            
        case TEXTURED:
            glUseProgram(m_program[0].programId());
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei) m_numPoints);
            
            break;
            
//        case WIREFRAME:
//            glUseProgram(m_program[0].programId());		// bind the glsl progam
//            for ( int i=0; i<=(int)m_numPoints-grid; i=i+grid) {
//                glDrawArrays(GL_LINE_STRIP, i, (GLsizei) grid);
//            }
//            break;
        
//        case HIDDENLINE:   drawHiddenLine();	break;
//        case FLATSHADED:   drawFlatShaded();	break;
//        case SMOOTHSHADED: drawSmoothShaded();	break;
        
    }
    
    
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW3b::reset:
//
// Reset parameters.
//
void
HW3b::initPoints()
{
    for(int i=0; i<grid; i++) {
        for(int j=0; j<grid; j++) {
            force[i][j]=0.0;
            veloc[i][j]=0.0;
            
            switch(resetMode) {
                case FLAT:
                    posit[i][j] = 0.0;
                    break;
                case SPIKE:
                    posit[i][j]= (i==j && i == grid/2) ? grid*1.5 : 0.0;
                    break;
                case HOLE:
                    posit[i][j]= (!((i > grid/3 && j > grid/3)&&(i < grid*2/3 && j < grid*2/3))) ? grid/4 : 0.0;
                    break;
                case DIAGONALWALL:
                    posit[i][j]= (((grid-i)-j<3) && ((grid-i)-j>0)) ? grid/6 : 0.0;
                    break;
                case SIDEWALL:
                    posit[i][j]= (i==1) ? grid/4 : 0.0;
                    break;
                case DIAGONALBLOCK:
                    posit[i][j]= ((grid-i)-j<3) ? grid/6 : 0.0;
                    break;
                case MIDDLEBLOCK:
                    posit[i][j]= ((i > grid/3 && j > grid/3)&&(i < grid*2/3 && j < grid*2/3)) ? grid/4 : 0.0;
                    break;
                case CORNERBLOCK:
                    posit[i][j]= ((i > grid*3/4 && j > grid*3/4)) ? grid/4 : 0.0;
                    break;
                case HILL:
                    posit[i][j]=
                    (sin(M_PI * ((float)i/(float)grid)) +
                     sin(M_PI * ((float)j/(float)grid)))* grid/6.0;
                    break;
                case HILLFOUR:
                    posit[i][j]=
                    (sin(M_PI*2 * ((float)i/(float)grid)) +
                     sin(M_PI*2 * ((float)j/(float)grid)))* grid/6.0;
                    break;
            }
            if (i==0||j==0||i==grid-1||j==grid-1) posit[i][j]=0.0;
        }
    }

    // draw
//    updateGL();
}

void HW3b::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        leftButton = true;
        downX = event->x();
        downY = event->y();

    }
}

void HW3b::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        leftButton = false;
    }
}

void HW3b::mouseMoveEvent(QMouseEvent *event)
{
    if (leftButton==true) {
        
        sphi += (float)(event->x() - downX) / 100.0;
        stheta += (float)(downY - event->y()) / 100.0;
        initPosition();
        updateGL();
        
        
    }
}

void HW3b::getforce()
{
    float d;

    for(int i=0;i<grid;i++)
        for(int j=0;j<grid;j++)
            force[i][j]=0.0;

    for(int i=2;i<grid-2;i++) {
        for(int j=2;j<grid-2;j++) {
            d = posit[i][j]-posit[i][j-1];
            force[i][ j ] -= d;
            force[i][j-1] += d;

            d = posit[i][j]-posit[i-1][j];
            force[i][ j ] -= d;
            force[i-1][j] += d;

            d = (posit[i][j]-posit[i][j+1]);
            force[i][ j ] -= d ;
            force[i][j+1] += d;

            d = (posit[i][j]-posit[i+1][j]);
            force[ i ][j] -= d ;
            force[i+1][j] += d;

            d = (posit[i][j]-posit[i+1][j+1])*SQRTOFTWOINV;
            force[ i ][j] -= d ;
            force[i+1][j+1] += d;

            d = (posit[i][j]-posit[i-1][j-1])*SQRTOFTWOINV;
            force[ i ][ j ] -= d ;
            force[i-1][j-1] += d;

            d = (posit[i][j]-posit[i+1][j-1])*SQRTOFTWOINV;
            force[ i ][ j ] -= d ;
            force[i+1][j-1] += d;

            d = (posit[i][j]-posit[i-1][j+1])*SQRTOFTWOINV;
            force[ i ][ j ] -= d ;
            force[i-1][j+1] += d;
        }
    }
}

void HW3b::getvelocity()
{
    for(int i=0;i<grid;i++)
        for(int j=0;j<grid;j++)
            veloc[i][j]+=force[i][j] * dt;
}

void HW3b::getposition()
{
    for(int i=0;i<grid;i++)
        for(int j=0;j<grid;j++)
            posit[i][j]+=veloc[i][j];
}


void HW3b::copy(float vec0[3], float vec1[3])
{
    vec0[0] = vec1[0];
    vec0[1] = vec1[1];
    vec0[2] = vec1[2];
}

void HW3b::sub(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[0] - vec2[0];
    vec0[1] = vec1[1] - vec2[1];
    vec0[2] = vec1[2] - vec2[2];
}

void HW3b::add(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[0] + vec2[0];
    vec0[1] = vec1[1] + vec2[1];
    vec0[2] = vec1[2] + vec2[2];
}

void HW3b::scalDiv(float vec[3], float c)
{
    vec[0] /= c; vec[1] /= c; vec[2] /= c;
}

void HW3b::cross(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    vec0[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    vec0[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

void HW3b::norm(float vec[3])
{
    float c = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    scalDiv(vec, c);
}

void HW3b::set(float vec[3], float x, float y, float z)
{
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}


// face normals - for flat shading
void HW3b::getFaceNorms()
{
    float vec0[3], vec1[3], vec2[3], norm0[3], norm1[3];
    float geom0[3], geom1[3], geom2[3], geom3[3];
    for(int i=0; i < grid-1; i++) {
        for(int j = 0; j < grid-1; j++) {
            // get vectors from geometry points
            geom0[0] = i;   geom0[1] = j;   geom0[2] = posit[ i ][ j ];
            geom1[0] = i;   geom1[1] = j+1; geom1[2] = posit[ i ][j+1];
            geom2[0] = i+1; geom2[1] = j;   geom2[2] = posit[i+1][ j ];
            geom3[0] = i+1; geom3[1] = j+1; geom3[2] = posit[i+1][j+1];

            sub( vec0, geom1, geom0 );
            sub( vec1, geom1, geom2 );
            sub( vec2, geom1, geom3 );

            // get triangle face normals from vectors & normalize them
            cross( norm0, vec0, vec1 );
            norm ( norm0 );

            cross( norm1, vec1, vec2 );
            norm ( norm1 );

            copy( faceNorms[0][i][j], norm0 );
            copy( faceNorms[1][i][j], norm1 );
        }
    }
}

// vertex normals - average of face normals for smooth shading
void HW3b::getVertNorms()
{
    float avg[3];
    for(int i=0; i < grid; i++) {
        for(int j=0; j < grid; j++) {
            // for each vertex, average normals from all faces sharing vertex;
            // check each quadrant in turn
            set(avg, 0.0, 0.0, 0.0);

            // right & above
            if (j < grid-1 && i < grid-1)
                add( avg, avg, faceNorms[0][i][j] );

            // right & below
            if (j < grid-1 && i > 0) {
                add( avg, avg, faceNorms[0][i-1][j] );
                add( avg, avg, faceNorms[1][i-1][j] );
            }

            // left & above
            if (j > 0 && i < grid-1) {
                add( avg, avg, faceNorms[0][i][j-1] );
                add( avg, avg, faceNorms[1][i][j-1] );
            }

            // left & below
            if (j > 0 && i > 0)
                add( avg, avg, faceNorms[1][i-1][j-1] );

            // normalize
            norm( avg );
            copy( vertNorms[i][j], avg );
        }
    }
}


void HW3b::getFaceNormSegs()
{
    float center0[3], center1[3], normSeg0[3], normSeg1[3];
    float geom0[3], geom1[3], geom2[3], geom3[3];
    for(int i=0; i < grid-1; i++) {
        for(int j=0; j < grid-1; j++) {
            geom0[0] = i;   geom0[1] = j;   geom0[2] = posit[ i ][j];
            geom1[0] = i;   geom1[1] = j+1; geom1[2] = posit[ i ][j+1];
            geom2[0] = i+1; geom2[1] = j;   geom2[2] = posit[i+1][j];
            geom3[0] = i+1; geom3[1] = j+1; geom3[2] = posit[i+1][j+1];

            // find center of triangle face by averaging three vertices
            add( center0, geom2, geom0 );
            add( center0, center0, geom1 );
            scalDiv( center0, 3.0 );

            add( center1, geom2, geom1 );
            add( center1, center1, geom3 );
            scalDiv( center1, 3.0 );

            // translate normal to center of triangle face to get normal segment
            add( normSeg0, center0, faceNorms[0][i][j] );
            add( normSeg1, center1, faceNorms[1][i][j] );

            copy( faceNormSegs[0][0][i][j], center0 );
            copy( faceNormSegs[1][0][i][j], center1 );

            copy( faceNormSegs[0][1][i][j], normSeg0 );
            copy( faceNormSegs[1][1][i][j], normSeg1 );
        }
    }
}

void HW3b::getTexCoords()
{
    for(int i=0; i < grid; i++) {
        for(int j=0; j < grid; j++) {
            texCoords[i][j][0] = (float)j / (float)(grid-1);
            texCoords[i][j][1] = (float)i / (float)(grid-1);
        }
    }
}

//void setSize(int value)
//{
//    int prevGrid = grid;
//    switch(value) {
//        case SMALL : grid = MAXGRID/4; break;
//        case MEDIUM: grid = MAXGRID/2; break;
//        case LARGE : grid = MAXGRID/1.5; break;
//        case XLARGE: grid = MAXGRID; break;
//    }

//    if (prevGrid > grid) reset(resetMode);

//    zNear= grid/10.0;
//    zFar= grid*3.0;
//    sdepth = 5.0/4.0 * grid;
//    getTexCoords();
//    glutPostRedisplay();
//}

//void setSpeed(int value)
//{
//    switch(value) {
//        case WEAK  : dt = 0.001; break;
//        case NORMAL: dt = 0.004; break;
//        case STRONG: dt = 0.008; break;
//    }
//}
