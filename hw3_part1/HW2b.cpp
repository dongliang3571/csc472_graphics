//
//  HW2b.cpp
//  hw
//
//  Created by dong liang on 10/15/15.
//
//


#include "HW2b.h"

HW2b::HW2b(QWidget *parent): HW(parent)
{
    m_subnumber = 0;
    m_isTwist = 0;
    m_theta = 0.0;
    m_colorChange =true;
    vertices[0]=vec2(-0.9, -0.4);
    vertices[1]=vec2(0.0, 0.75);
    vertices[2]=vec2(0.9, -0.4);
    
}


void HW2b::initializeGL(){
    
    initializeGLFunctions();
    
    // init vertex and fragment shaders
    initShaders();
    
    // initialize vertex buffer and write positions to vertex shader
    initVertexBuffer();
    
    // init state variables
    glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
//    glColor3f   (1.0, 1.0, 0.0);
    
}

void HW2b::initShaders(){
    // compile vertex shader
    if(!m_program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader2b.glsl")) {
        QMessageBox::critical(0, "Error", "Vertex shader error", QMessageBox::Ok);
        QApplication::quit();
    }
    
    // compile fragment shader
    if(!m_program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader2b.glsl")) {
        QMessageBox::critical(0, "Error", "Fragment shader error",QMessageBox::Ok);
        QApplication::quit();
    }
    
    //     link shader pipeline
    if(!m_program.link()) {
        QMessageBox::critical(0, "Error", "Could not link shader", QMessageBox::Ok);
        QApplication::quit();
    }
    
    // bind the glsl progam
    glUseProgram(m_program.programId());
    
    glBindAttribLocation(m_program.programId(), ATTRIB_VERTEX, "a_Position");
    
    glBindAttribLocation(m_program.programId(), ATTRIB_COLOR,  "a_Color");

    m_u_isTwist = glGetUniformLocation(m_program.programId(), "u_Twist");
    if((int) m_u_isTwist < 0) {
        qDebug() << "Failed to get the storage location of u_Twist";
        exit(-1);
    }
    glUniform1i(m_u_isTwist,m_isTwist);
    
    m_u_angle = glGetUniformLocation(m_program.programId(), "u_angle");
    if((int) m_u_angle < 0) {
        qDebug() << "Failed to get the storage location of u_angle";
        exit(-1);
    }
    glUniform1i(m_u_angle,m_theta);


    m_u_ModelMatrix = glGetUniformLocation(m_program.programId(), "u_ModelMatrix");
    if((int) m_u_ModelMatrix < 0) {
        qDebug() << "Failed to get the storage location of u_ModelMatrix";
        exit(-1);
    }
    
    m_ModelMatrix.setToIdentity();
    glUniformMatrix4fv(m_u_ModelMatrix, 1, GL_FALSE, m_ModelMatrix.constData());
    
}

void HW2b::initVertexBuffer(){
    
    // set flag for creating buffers (1st time only)
    static bool flag = 1;
    
    // verify that we have valid vertex/color buffers
    static GLuint vertexBuffer = -1;
    static GLuint colorBuffer  = -1;
    if(flag) {	// create vertex and color buffers
        glGenBuffers(1, &vertexBuffer);
        glGenBuffers(1, &colorBuffer );
        flag = 0;	// reset flag
    }
    
    // init geometry data
    
    divide_triangle(vertices[0], vertices[1], vertices[2], m_subnumber);
    
    
    m_numPoints = m_points.size();		// save number of vertices
    
    // bind the buffer to the GPU; all future drawing calls gets data from this buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // copy the vertices from CPU to GPU
    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_points[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, false, 0, NULL);
    
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    // assign the buffer object to the attribute vertex variable
    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);

    glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);

    glEnableVertexAttribArray(ATTRIB_COLOR);
    //    // clear vertex vectors because they have already been copied into GPU
    m_points.clear();
//    m_colors.clear();
    
    
    
}

void HW2b::paintGL()
{
    int w2, h2;
    w2 = m_winW;
    h2 = m_winH;
    
    glClear(GL_COLOR_BUFFER_BIT);
    
//    glViewport(0, 0, w2/3 , h2/3 );
    glDrawArrays(GL_TRIANGLES, 0, m_numPoints);
    
}

void HW2b::resizeGL(int w, int h)
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

void HW2b::divide_triangle(vec2 a, vec2 b, vec2 c, int k){

    if (k>0) {
        
        float midab1 = (a.x() + b.x()) / 2.0;
        float midab2 = (a.y() + b.y()) / 2.0;
        
        float midac1 = (a.x() + c.x()) / 2.0;
        float midac2 = (a.y() + c.y()) / 2.0;
        
        float midbc1 = (b.x() + c.x()) / 2.0;
        float midbc2 = (b.y() + c.y()) / 2.0;
        
        vec2 tema;
        tema.setX(midab1);
        tema.setY(midab2);
        
        vec2 temb;
        temb.setX(midac1);
        temb.setY(midac2);
        
        vec2 temc;
        temc.setX(midbc1);
        temc.setY(midbc2);
        
        
        divide_triangle(a, tema, temb, k - 1);
        divide_triangle(c, temb, temc, k - 1);
        divide_triangle(b, temc, tema, k - 1);
        divide_triangle(tema, temb, temc, k-1);
    }
    else triangle(a, b, c);
    
}
void HW2b::triangle(vec2 a, vec2 b, vec2 c){
    
    m_points.push_back(a);
    m_points.push_back(b);
    m_points.push_back(c);
    
    
    if (m_colorChange==true) {
        
    
    m_colors.push_back(vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
    m_colors.push_back(m_colors[m_colors.size()-1]);
    m_colors.push_back(m_colors[m_colors.size()-1]);
    }
    
    
}



QGroupBox* HW2b::controlPanel()
{
    // init group box
    QGroupBox *groupBox = new QGroupBox("Homework 2b");
    
    groupBox->setMinimumWidth(300);
    
    m_sliderDividing = new QSlider(Qt::Horizontal);
    m_sliderDividing->setRange(0, 8);
    m_sliderDividing->setValue(0);
    
    // create spinBox
    m_spinBoxDividing = new QSpinBox;
    m_spinBoxDividing->setRange(0, 8);
    m_spinBoxDividing->setValue(0);
    
    m_sliderTheta = new QSlider(Qt::Horizontal);
    m_sliderTheta->setRange(0, 360);
    m_sliderTheta->setValue(0);
    
    m_spinBoxTheta = new QSpinBox;
    m_spinBoxTheta->setRange(0, 360);
    m_spinBoxTheta->setValue(0);
    
    QGridLayout *layout = new QGridLayout;
    QLabel *label = new QLabel("Subdivide");
    QLabel *label2 = new QLabel("Theta");
    
    m_checkBoxTwist = new QCheckBox("Twist");
    
    layout->addWidget(label,0,0,1,1);
    layout->addWidget(m_sliderDividing,0,1,1,3);
    layout->addWidget(m_spinBoxDividing,0,4);
    
    layout->addWidget(label2,2,0,3,1);
    layout->addWidget(m_sliderTheta, 2, 1, 3, 3);
    layout->addWidget(m_spinBoxTheta,3,4);
    
    layout->addWidget(m_checkBoxTwist, 4, 0);
    
    connect(m_sliderDividing, SIGNAL(valueChanged(int)), this, SLOT(changeDividing(int)));
    connect(m_spinBoxDividing, SIGNAL(valueChanged(int)), this, SLOT(changeDividing(int)));
    
    connect(m_sliderTheta, SIGNAL(valueChanged(int)),this, SLOT(changeTheta(int)));
    connect(m_spinBoxTheta, SIGNAL(valueChanged(int)), this, SLOT(changeTheta(int)));
    
    connect(m_checkBoxTwist, SIGNAL(stateChanged(int)), this, SLOT(setToTwist(int)));
    
    groupBox->setLayout(layout);
    
    
    
    return(groupBox);
}


void HW2b::changeDividing(int value){
    
    m_colors.clear();
    m_subnumber = value;
    m_sliderDividing->blockSignals(true);
    m_sliderDividing->setValue(value);
    m_sliderDividing->blockSignals(false);
    
    
    
    m_spinBoxDividing->blockSignals(true);
    m_spinBoxDividing->setValue(value);
    m_spinBoxDividing->blockSignals(false);

    initVertexBuffer();
    
    updateGL();
    
}

void HW2b::changeTheta(int angle){
    
    m_sliderTheta->blockSignals(true);
    m_sliderTheta->setValue(angle);
    m_sliderTheta->blockSignals(false);
    
    
    
    m_spinBoxTheta->blockSignals(true);
    m_spinBoxTheta->setValue(angle);
    m_spinBoxTheta->blockSignals(false);
    
    m_theta = angle * (M_PI / 180);
    
    if (m_checkBoxTwist->isChecked()){
        glUniform1i(m_u_isTwist,m_isTwist);
        glUniform1f(m_u_angle, m_theta);
        updateGL();
    }
    
    else{
    
        // update model's rotation matrix
        
        m_ModelMatrix.setToIdentity();
        m_ModelMatrix.rotate(angle, QVector3D(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(m_u_ModelMatrix, 1, GL_FALSE, m_ModelMatrix.constData());
        
        updateGL();
    }
    
}

void HW2b::setToTwist(int state){
    
//    m_checkBoxTwist->setChecked(state);
    if (state==Qt::Checked) {
        m_isTwist=1;
    }
    else{
        m_isTwist=0;
    }
    
    glUniform1i(m_u_isTwist,m_isTwist);
    glUniform1f(m_u_angle, m_theta);
    
    updateGL();
    
    
}


void HW2b::reset()
{
    // reset parameters
    m_subnumber = 0;
    m_isTwist = 0;
    m_theta = 0.0;
    
    
    m_sliderTheta->blockSignals(true);
    m_sliderTheta->setValue(0);
    m_sliderTheta->blockSignals(false);
    
    m_sliderDividing->blockSignals(true);
    m_sliderDividing->setValue(0);
    m_sliderDividing->blockSignals(false);
    
    m_spinBoxTheta->blockSignals(true);
    m_spinBoxTheta->setValue(0);
    m_spinBoxTheta->blockSignals(false);
    
    m_spinBoxDividing->blockSignals(true);
    m_spinBoxDividing->setValue(0);
    m_spinBoxDividing->blockSignals(false);
    
    // set "Unchecked" to checkbox
    m_checkBoxTwist->setCheckState(Qt::Unchecked);
    
    // if geometry will change, call initVertexBuffer() by uncommenting next line:
//    initVertexBuffer();
    
    // reset 4x4 transformation matrix for model
    m_ModelMatrix.setToIdentity();
    
    // copy data to vertex shader
    glUniformMatrix4fv(m_u_ModelMatrix, 1, GL_FALSE, m_ModelMatrix.constData());

    
    // draw
    updateGL();
}



