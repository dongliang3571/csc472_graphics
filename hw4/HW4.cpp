


#include "HW4.h"

enum {MVP, VIEWMATRIX, PROJECTMATRIX, SAMPLER, AMBIENT, DIFFUSE, SPECULAR, LIGHT, SHININESS, NOR, NORMALMATRIX, LIGHTCOLOR};
enum {TEXTURED, WIREFRAME, TEX_WIRE, FLAT_SHADING, SMOOTH_SHADING, SMOOTH_SHADING_TEX};
enum {FLAT, SPIKE, DIAGONALWALL, SIDEWALL, HOLE,
    MIDDLEBLOCK, DIAGONALBLOCK, CORNERBLOCK, HILL, HILLFOUR};
#define SQRTOFTWOINV 1.0 / 1.414213562



HW4::HW4(QWidget *parent)
    : HW(parent)
{
    waving = false;
    grid = 17;
    dt=0.006;
    displayMode = FLAT_SHADING;
    resetMode = HILLFOUR;
    sphi=0.0;stheta=0.0;
    sdepth = 1.5;
    zNear=0.1; zFar=5.0;
    aspect = 5.0/4.0;
    timer = new QTimer(this);
    leftButton = false;

    connect(timer, SIGNAL(timeout()), this, SLOT(wave()));


    //hw4

    shininess = 10.0;
    a_color = vec3(0.8f, 0.2f, 0.8f);
    ambient = vec3(0.3f, 0.3f, 0.3f);
    lightcolor = vec3(1.0, 1.0, 1.0);
    lightDirection = vec3(0.0, 0.0, 1.0);
}



QGroupBox*
HW4::controlPanel()
{
    // init group box
    QGroupBox *groupBox = new QGroupBox("Homework 4");

    groupBox->setMinimumWidth(300);
    QGridLayout *layout = new QGridLayout;
    m_comboBox = new QComboBox();
    m_comboBox->addItem("Texture");
    m_comboBox->addItem("Wireframe");
    m_comboBox->addItem("Tex+Wire");
    m_comboBox->addItem("Flat_Shading");
    m_comboBox->addItem("Smooth_Shading");
    m_comboBox->addItem("Smooth_Shading_Tex");
    m_comboBox->setCurrentIndex(FLAT_SHADING);


    m_comboBox_mode = new QComboBox();
    m_comboBox_mode->addItem("FLAT");
    m_comboBox_mode->addItem("SPIKE");
    m_comboBox_mode->addItem("DIAGONALWALL");
    m_comboBox_mode->addItem("SIDEWALL");
    m_comboBox_mode->addItem("HOLE");
    m_comboBox_mode->addItem("MIDDLEBLOCK");
    m_comboBox_mode->addItem("DIAGONALBLOCK");
    m_comboBox_mode->addItem("CORNERBLOCK");
    m_comboBox_mode->addItem("HILL");
    m_comboBox_mode->addItem("HILLFOUR");
    m_comboBox_mode->setCurrentIndex(HILLFOUR);



    m_PushbottonGo = new QPushButton("go");
    m_PushbottonStop = new QPushButton("Stop");
    layout->addWidget(m_PushbottonGo,0,0,1,2);
    layout->addWidget(m_PushbottonStop,1,0,1,2);
    layout->addWidget(m_comboBox,2,0,1,2);
    layout->addWidget(m_comboBox_mode,3,0,1,2);

    m_sliderGrid = new QSlider(Qt::Horizontal);
    m_sliderGrid->setRange(2, 64);
    m_sliderGrid->setValue(17);

    m_spinboxGrid = new QSpinBox;
    m_spinboxGrid->setRange(2,64);
    m_spinboxGrid->setValue(17);


    m_sliderSpeed = new QSlider(Qt::Horizontal);
    m_sliderSpeed->setRange(1, 20);
    m_sliderSpeed->setValue(6);


    m_spinboxSpeed = new QSpinBox;
    m_spinboxSpeed->setRange(1,20);
    m_spinboxSpeed->setValue(6);


    m_labelGrid = new QLabel("Grid");
    m_labelSpeed = new QLabel("Speed");

    layout->addWidget(m_labelGrid,4,0,1,1);
    layout->addWidget(m_sliderGrid,4,1,1,1);
    layout->addWidget(m_spinboxGrid,4,3,1,1);

    layout->addWidget(m_labelSpeed,5,0,1,1);
    layout->addWidget(m_sliderSpeed,5,1,1,1);
    layout->addWidget(m_spinboxSpeed,5,3,1,1);


    connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(displayChange(int)));
    connect(m_comboBox_mode, SIGNAL(currentIndexChanged(int)), this, SLOT(selectMode(int)));
    connect(m_PushbottonGo, SIGNAL(clicked()), this, SLOT(begintimer()));
    connect(m_PushbottonStop, SIGNAL(clicked()), this, SLOT(stopTimer()));

    connect(m_sliderGrid, SIGNAL(valueChanged(int)), this, SLOT(setsize(int)));
    connect(m_spinboxGrid, SIGNAL(valueChanged(int)), this, SLOT(setsize(int)));
    connect(m_sliderSpeed, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));
//    connect(m_spinboxSpeed, SIGNAL(valueChanged(int)), this, SLOT(setSpeed(int)));


    groupBox->setLayout(layout);


    return(groupBox);
}

void HW4::begintimer(){

    waving = true;
    if (waving == true) {
        timer->start(0);

    }
    else{
        timer->stop();
    }
}

void HW4::stopTimer(){
    waving = false;
    timer->stop();
}

void HW4::wave()
{
    getforce();
    getvelocity();
    getposition();
    initVertexBuffer();
    updateGL();
}

void HW4::displayChange(int index){


    stopTimer();
//    if(index==FLAT_SHADING){
//        static GLuint normalBuffer = -1;
//        glGenBuffers(1, &normalBuffer);
//        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
//        glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_normals[0], GL_STATIC_DRAW);
//        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//        glEnableVertexAttribArray(ATTRIB_NORMAL);
//        m_normals.clear();
//    }
//    if(index==SMOOTH_SHADING){
//        static GLuint vector_normalBuffer = -1;
//        glGenBuffers(1, &vector_normalBuffer);
//        glBindBuffer(GL_ARRAY_BUFFER, vector_normalBuffer);
//        glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_average_normals[0], GL_STATIC_DRAW);
//        glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//        glEnableVertexAttribArray(ATTRIB_NORMAL);

//        m_average_normals.clear();
//    }
    displayMode = index;

    updateGL();

}

void HW4::selectMode(int index){
    stopTimer();
    resetMode = index;
    initPoints();
    initVertexBuffer();
    updateGL();

}

void HW4::setsize(int value)
{
    timer->stop();
    m_sliderGrid->setValue(value);
    m_spinboxGrid->setValue(value);
    grid = value+1;
//    sdepth = 5.0/4.0 * (grid+3);
//    sdepth = sdepth+0.6;
    initPoints();
    initPosition();
    initVertexBuffer();
    updateGL();
}

void HW4::setSpeed(int value)
{
    m_spinboxSpeed->setValue(value);
    m_sliderSpeed->setValue(value);
    timer->stop();
    if (waving == true) {
        dt =(float)value/1000.0;
        timer->start(0);

    }
    else{
        waving=false;
        dt =(float)value/1000.0;
    }

}

void HW4::initializeGL()
{
    // initialize GL function resolution for current context
    initializeGLFunctions();
    initTexture();
    initShaders();
    initPoints();
    initVertexBuffer();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
    glColor3f(0.8, 0.2, 0.8);		// set foreground color
}

void HW4::initTexture()
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
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
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glActiveTexture(GL_TEXTURE1);

    // set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, qImage.bits());

}

void
HW4::initShaders()
{

    initShader1();
    initShader2();
    initShader3();
    initShader4();
    initShader5();
}

void HW4::initShader1()
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
    m_uniform[0][MVP] = glGetUniformLocation(m_program[0].programId(), "u_MVPMatrix");
    if((int) m_uniform[0][MVP] < 0) {
        qDebug() << "Failed to get the storage location of u_MVPMatrix";
        exit(-1);
    }

    // get storage location of u_Sampler in fragment shader

    m_uniform[0][SAMPLER] = glGetUniformLocation(m_program[0].programId(), "u_Sampler");
    if((int) m_uniform[0][SAMPLER] < 0) {
        qDebug() << "Failed to get the storage location of u_Sampler";
        exit(-1);
    }

    // bind the glsl program
    glUseProgram(m_program[0].programId());

    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(0.0,0.0,-sdepth);
    //
    m_ModelMatrix.rotate(-stheta, 1.0, 0.0, 0.0);
    m_ModelMatrix.rotate(sphi, 0.0, 0.0, 1.0);
    m_ModelMatrix.translate(-0.5, -0.5, 0.0);

    m_ViewMatrix.setToIdentity();
    m_ProjectMatrix.setToIdentity();
    m_ProjectMatrix.perspective(64.0, aspect, zNear, zFar);

    m_ModelMatrix = m_ViewMatrix * m_ModelMatrix;
    m_ModelMatrix = m_ProjectMatrix * m_ModelMatrix;


    glUniformMatrix4fv(m_uniform[0][MVP], 1, GL_FALSE, m_ModelMatrix.constData());
    glUniform1i(m_uniform[0][SAMPLER], 1);
}



void
HW4::initShader2()
{
    // compile vertex shader
    if(!m_program[1].addShaderFromSourceFile(QGLShader::Vertex, ":/vshader3b2.glsl")) {
        QMessageBox::critical(0, "Error", "Vertex shader error", QMessageBox::Ok);
        QApplication::quit();
    }

    // compile fragment shader
    if(!m_program[1].addShaderFromSourceFile(QGLShader::Fragment, ":/fshader3b2.glsl")) {
        QMessageBox::critical(0, "Error", "Fragment shader error",QMessageBox::Ok);
        QApplication::quit();
    }

    glBindAttribLocation(m_program[1].programId(), ATTRIB_VERTEX, "a_Position");

    if(!m_program[1].link()) {
        QMessageBox::critical(0, "Error", "Could not link shader", QMessageBox::Ok);
        QApplication::quit();
    }

    // get storage location of u_ModelMatrix in vertex shader
    m_uniform[1][MVP] = glGetUniformLocation(m_program[1].programId(), "u_MVPMatrix");
    if((int) m_uniform[1][MVP] < 0) {
        qDebug() << "Failed to get the storage location of u_MVPMatrix";
        exit(-1);
    }



    // bind the glsl program
    glUseProgram(m_program[1].programId());

    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(0.0,0.0,-sdepth);
    //
    m_ModelMatrix.rotate(-stheta, 1.0, 0.0, 0.0);
    m_ModelMatrix.rotate(sphi, 0.0, 0.0, 1.0);
    m_ModelMatrix.translate(-0.5, -0.5, 0.0);

    m_ViewMatrix.setToIdentity();
    m_ProjectMatrix.setToIdentity();
    m_ProjectMatrix.perspective(64.0, aspect, zNear, zFar);

    m_ModelMatrix = m_ViewMatrix * m_ModelMatrix;
    m_ModelMatrix = m_ProjectMatrix * m_ModelMatrix;


    glUniformMatrix4fv(m_uniform[1][MVP], 1, GL_FALSE, m_ModelMatrix.constData());

}

void HW4::initShader3()
{
    // compile vertex shader
    if(!m_program[2].addShaderFromSourceFile(QGLShader::Vertex, ":/vshader4_per_vertex.glsl")) {
        QMessageBox::critical(0, "Error", "Vertex shader error", QMessageBox::Ok);
        QApplication::quit();
    }

    // compile fragment shader
    if(!m_program[2].addShaderFromSourceFile(QGLShader::Fragment, ":/fshader4_per_vertex.glsl")) {
        QMessageBox::critical(0, "Error", "Fragment shader error",QMessageBox::Ok);
        QApplication::quit();
    }

    glBindAttribLocation(m_program[2].programId(), ATTRIB_VERTEX, "a_Position");
    glBindAttribLocation(m_program[2].programId(), ATTRIB_NORMAL, "vNormal");

    if(!m_program[2].link()) {
        QMessageBox::critical(0, "Error", "Could not link shader", QMessageBox::Ok);
        QApplication::quit();
    }

    // get storage location of u_ModelMatrix in vertex shader
    m_uniform[2][MVP] = glGetUniformLocation(m_program[2].programId(), "u_MVPMatrix");
    if((int) m_uniform[2][MVP] < 0) {
        qDebug() << "Failed to get the storage location of u_MVPMatrix";
        exit(-1);
    }


    m_uniform[2][VIEWMATRIX] = glGetUniformLocation(m_program[2].programId(), "ModelView");
    if((int) m_uniform[2][VIEWMATRIX] < 0) {
        qDebug() << "Failed to get the storage location of ModelView";
        exit(-1);
    }


    m_uniform[2][AMBIENT] = glGetUniformLocation(m_program[2].programId(), "AmbientProduct");
    if((int) m_uniform[2][AMBIENT] < 0) {
        qDebug() << "Failed to get the storage location of AmbientProduct";
        exit(-1);
    }

    m_uniform[2][DIFFUSE] = glGetUniformLocation(m_program[2].programId(), "DiffuseProduct");
    if((int) m_uniform[2][DIFFUSE] < 0) {
        qDebug() << "Failed to get the storage location of DiffuseProduct";
        exit(-1);
    }

    m_uniform[2][SPECULAR] = glGetUniformLocation(m_program[2].programId(), "SpecularProduct");
    if((int) m_uniform[2][SPECULAR] < 0) {
        qDebug() << "Failed to get the storage location of SpecularProduct";
        exit(-1);
    }

    m_uniform[2][LIGHT] = glGetUniformLocation(m_program[2].programId(), "LightPosition");
    if((int) m_uniform[2][LIGHT] < 0) {
        qDebug() << "Failed to get the storage location of lightPosition";
        exit(-1);
    }

    m_uniform[2][SHININESS] = glGetUniformLocation(m_program[2].programId(), "Shininess");
    if((int) m_uniform[2][SHININESS] < 0) {
        qDebug() << "Failed to get the storage location of Shininess";
        exit(-1);
    }

//    m_uniform[2][NOR] = glGetUniformLocation(m_program[2].programId(), "vNormal");
//    if((int) m_uniform[2][NOR] < 0) {
//        qDebug() << "Failed to get the storage location of AmbienProduct";
//        exit(-1);
//    }

    m_uniform[2][NORMALMATRIX] = glGetUniformLocation(m_program[2].programId(), "NormalMatrix");
    if((int) m_uniform[2][NORMALMATRIX] < 0) {
        qDebug() << "Failed to get the storage location of NormalMatrix";
        exit(-1);
    }


    // bind the glsl program
    glUseProgram(m_program[2].programId());

    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(0.0,0.0,-sdepth);
    //
    m_ModelMatrix.rotate(-stheta, 1.0, 0.0, 0.0);
    m_ModelMatrix.rotate(sphi, 0.0, 0.0, 1.0);
    m_ModelMatrix.translate(-0.5, -0.5, 0.0);

    glUniformMatrix4fv(m_uniform[2][VIEWMATRIX], 1, GL_FALSE, m_ModelMatrix.constData());
    //normal matrix
    QMatrix4x4 tem_matrix = m_ModelMatrix;
    m_NormalMatrix = tem_matrix.inverted();
    glUniformMatrix4fv(m_uniform[2][NORMALMATRIX], 1, GL_TRUE, m_NormalMatrix.constData());
    m_ViewMatrix.setToIdentity();
    m_ProjectMatrix.setToIdentity();
    m_ProjectMatrix.perspective(64.0, aspect, zNear, zFar);

    m_ModelMatrix = m_ViewMatrix * m_ModelMatrix;
    m_ModelMatrix = m_ProjectMatrix * m_ModelMatrix;


    glUniformMatrix4fv(m_uniform[2][MVP], 1, GL_FALSE, m_ModelMatrix.constData());

    //ambient
    ambientProduct = ambient * a_color;
    glUniform3f(m_uniform[2][AMBIENT], ambientProduct.x(), ambientProduct.y(), ambientProduct.z());

    //diffuse
    diffuseProduct = lightcolor * a_color;
    glUniform3f(m_uniform[2][DIFFUSE], diffuseProduct.x(), diffuseProduct.y(), diffuseProduct.z());

    specularProduct = lightcolor * a_color;
    glUniform3f(m_uniform[2][SPECULAR], specularProduct.x(), specularProduct.y(), specularProduct.z());

    //light direction
    glUniform3f(m_uniform[2][LIGHT], lightDirection.x(), lightDirection.y(),lightDirection.z());


    //Shininess
    glUniform1f(m_uniform[2][SHININESS],shininess);

//    glUniform4f(m_uniform[2][NOR], 0.0, 0.0 ,1.0, 0.0);





}


void HW4::initShader4()
{
    // compile vertex shader
    if(!m_program[3].addShaderFromSourceFile(QGLShader::Vertex, ":/vshader4_per_fragment.glsl")) {
        QMessageBox::critical(0, "Error", "Vertex shader error", QMessageBox::Ok);
        QApplication::quit();
    }

    // compile fragment shader
    if(!m_program[3].addShaderFromSourceFile(QGLShader::Fragment, ":/fshader4_per_fragment.glsl")) {
        QMessageBox::critical(0, "Error", "Fragment shader error",QMessageBox::Ok);
        QApplication::quit();
    }

    glBindAttribLocation(m_program[3].programId(), ATTRIB_VERTEX, "a_Position");
    glBindAttribLocation(m_program[3].programId(), ATTRIB_VECTOR_NORMAL, "vNormal");
    glBindAttribLocation(m_program[3].programId(), ATTRIB_COLOR, "a_Color");


    if(!m_program[3].link()) {
        QMessageBox::critical(0, "Error", "Could not link shader", QMessageBox::Ok);
        QApplication::quit();
    }

    // get storage location of u_ModelMatrix in vertex shader
    m_uniform[3][MVP] = glGetUniformLocation(m_program[3].programId(), "u_MVPMatrix");
    if((int) m_uniform[3][MVP] < 0) {
        qDebug() << "Failed to get the storage location of u_MVPMatrix";
        exit(-1);
    }


    m_uniform[3][VIEWMATRIX] = glGetUniformLocation(m_program[3].programId(), "ModelView");
    if((int) m_uniform[3][VIEWMATRIX] < 0) {
        qDebug() << "Failed to get the storage location of ModelView";
        exit(-1);
    }


    m_uniform[3][AMBIENT] = glGetUniformLocation(m_program[3].programId(), "AmbientLight");
    if((int) m_uniform[3][AMBIENT] < 0) {
        qDebug() << "Failed to get the storage location of AmbientProduct";
        exit(-1);
    }

//    m_uniform[3][DIFFUSE] = glGetUniformLocation(m_program[3].programId(), "DiffuseProduct");
//    if((int) m_uniform[3][DIFFUSE] < 0) {
//        qDebug() << "Failed to get the storage location of DiffuseProduct";
//        exit(-1);
//    }

//    m_uniform[3][SPECULAR] = glGetUniformLocation(m_program[3].programId(), "SpecularProduct");
//    if((int) m_uniform[3][SPECULAR] < 0) {
//        qDebug() << "Failed to get the storage location of SpecularProduct";
//        exit(-1);
//    }

    m_uniform[3][LIGHT] = glGetUniformLocation(m_program[3].programId(), "LightPosition");
    if((int) m_uniform[3][LIGHT] < 0) {
        qDebug() << "Failed to get the storage location of lightPosition";
        exit(-1);
    }

    m_uniform[3][SHININESS] = glGetUniformLocation(m_program[3].programId(), "Shininess");
    if((int) m_uniform[3][SHININESS] < 0) {
        qDebug() << "Failed to get the storage location of Shininess";
        exit(-1);
    }

//    m_uniform[3][NOR] = glGetUniformLocation(m_program[3].programId(), "vNormal");
//    if((int) m_uniform[3][NOR] < 0) {
//        qDebug() << "Failed to get the storage location of AmbienProduct";
//        exit(-1);
//    }

    m_uniform[3][NORMALMATRIX] = glGetUniformLocation(m_program[3].programId(), "NormalMatrix");
    if((int) m_uniform[3][NORMALMATRIX] < 0) {
        qDebug() << "Failed to get the storage location of NormalMatrix";
        exit(-1);
    }

    m_uniform[3][LIGHTCOLOR] = glGetUniformLocation(m_program[3].programId(), "u_LightColor");
    if((int) m_uniform[3][LIGHTCOLOR] < 0) {
        qDebug() << "Failed to get the storage location of NormalMatrix";
        exit(-1);
    }


    // bind the glsl program
    glUseProgram(m_program[3].programId());

    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(0.0,0.0,-sdepth);
    //
    m_ModelMatrix.rotate(-stheta, 1.0, 0.0, 0.0);
    m_ModelMatrix.rotate(sphi, 0.0, 0.0, 1.0);
    m_ModelMatrix.translate(-0.5, -0.5, 0.0);

    glUniformMatrix4fv(m_uniform[3][VIEWMATRIX], 1, GL_FALSE, m_ModelMatrix.constData());
    //normal matrix
    QMatrix4x4 tem_matrix = m_ModelMatrix;
    m_NormalMatrix = tem_matrix.inverted();
    glUniformMatrix4fv(m_uniform[3][NORMALMATRIX], 1, GL_TRUE, m_NormalMatrix.constData());
    m_ViewMatrix.setToIdentity();
    m_ProjectMatrix.setToIdentity();
    m_ProjectMatrix.perspective(64.0, aspect, zNear, zFar);

    m_ModelMatrix = m_ViewMatrix * m_ModelMatrix;
    m_ModelMatrix = m_ProjectMatrix * m_ModelMatrix;


    glUniformMatrix4fv(m_uniform[3][MVP], 1, GL_FALSE, m_ModelMatrix.constData());

    //ambient

    glUniform3f(m_uniform[3][AMBIENT], ambient.x(), ambient.y(), ambient.z());

    //diffuse
//    diffuseProduct = lightcolor * a_color;
//    glUniform3f(m_uniform[3][DIFFUSE], diffuseProduct.x(), diffuseProduct.y(), diffuseProduct.z());

//    specularProduct = lightcolor * a_color;
//    glUniform3f(m_uniform[3][SPECULAR], specularProduct.x(), specularProduct.y(), specularProduct.z());

    //light direction
    glUniform3f(m_uniform[3][LIGHT], lightDirection.x(), lightDirection.y(),lightDirection.z());


    //Shininess
    glUniform1f(m_uniform[3][SHININESS],shininess);

    glUniform3f(m_uniform[3][LIGHTCOLOR], lightcolor.x(), lightcolor.y(), lightcolor.z());





}


void HW4::initShader5()
{
    // compile vertex shader
    if(!m_program[4].addShaderFromSourceFile(QGLShader::Vertex, ":/vshader4_tex_fragment.glsl")) {
        QMessageBox::critical(0, "Error", "Vertex shader error", QMessageBox::Ok);
        QApplication::quit();
    }

    // compile fragment shader
    if(!m_program[4].addShaderFromSourceFile(QGLShader::Fragment, ":/fshader4_tex_fragment.glsl")) {
        QMessageBox::critical(0, "Error", "Fragment shader error",QMessageBox::Ok);
        QApplication::quit();
    }

    glBindAttribLocation(m_program[4].programId(), ATTRIB_VERTEX, "a_Position");
    glBindAttribLocation(m_program[4].programId(), ATTRIB_VECTOR_NORMAL, "vNormal");
//    glBindAttribLocation(m_program[4].programId(), ATTRIB_COLOR, "a_Color");
    glBindAttribLocation(m_program[4].programId(), ATTRIB_TEXTURE_POSITION, "a_TexCoord");

    if(!m_program[4].link()) {
        QMessageBox::critical(0, "Error", "Could not link shader", QMessageBox::Ok);
        QApplication::quit();
    }

    // get storage location of u_ModelMatrix in vertex shader
    m_uniform[4][MVP] = glGetUniformLocation(m_program[4].programId(), "u_MVPMatrix");
    if((int) m_uniform[4][MVP] < 0) {
        qDebug() << "Failed to get the storage location of u_MVPMatrix";
        exit(-1);
    }


    m_uniform[4][VIEWMATRIX] = glGetUniformLocation(m_program[4].programId(), "ModelView");
    if((int) m_uniform[4][VIEWMATRIX] < 0) {
        qDebug() << "Failed to get the storage location of ModelView";
        exit(-1);
    }


    m_uniform[4][AMBIENT] = glGetUniformLocation(m_program[4].programId(), "AmbientLight");
    if((int) m_uniform[4][AMBIENT] < 0) {
        qDebug() << "Failed to get the storage location of AmbientProduct";
        exit(-1);
    }

//    m_uniform[4][DIFFUSE] = glGetUniformLocation(m_program[4].programId(), "DiffuseProduct");
//    if((int) m_uniform[4][DIFFUSE] < 0) {
//        qDebug() << "Failed to get the storage location of DiffuseProduct";
//        exit(-1);
//    }

//    m_uniform[4][SPECULAR] = glGetUniformLocation(m_program[4].programId(), "SpecularProduct");
//    if((int) m_uniform[4][SPECULAR] < 0) {
//        qDebug() << "Failed to get the storage location of SpecularProduct";
//        exit(-1);
//    }

    m_uniform[4][LIGHT] = glGetUniformLocation(m_program[4].programId(), "LightPosition");
    if((int) m_uniform[4][LIGHT] < 0) {
        qDebug() << "Failed to get the storage location of lightPosition";
        exit(-1);
    }

    m_uniform[4][SHININESS] = glGetUniformLocation(m_program[4].programId(), "Shininess");
    if((int) m_uniform[4][SHININESS] < 0) {
        qDebug() << "Failed to get the storage location of Shininess";
        exit(-1);
    }

//    m_uniform[4][NOR] = glGetUniformLocation(m_program[4].programId(), "vNormal");
//    if((int) m_uniform[4][NOR] < 0) {
//        qDebug() << "Failed to get the storage location of AmbienProduct";
//        exit(-1);
//    }

    m_uniform[4][NORMALMATRIX] = glGetUniformLocation(m_program[4].programId(), "NormalMatrix");
    if((int) m_uniform[4][NORMALMATRIX] < 0) {
        qDebug() << "Failed to get the storage location of NormalMatrix";
        exit(-1);
    }

    m_uniform[4][LIGHTCOLOR] = glGetUniformLocation(m_program[4].programId(), "u_LightColor");
    if((int) m_uniform[4][LIGHTCOLOR] < 0) {
        qDebug() << "Failed to get the storage location of NormalMatrix";
        exit(-1);
    }

    m_uniform[4][SAMPLER] = glGetUniformLocation(m_program[4].programId(), "u_Sampler");
    if((int) m_uniform[4][SAMPLER] < 0) {
        qDebug() << "Failed to get the storage location of u_Sampler";
        exit(-1);
    }


    // bind the glsl program
    glUseProgram(m_program[4].programId());

    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(0.0,0.0,-sdepth);
    //
    m_ModelMatrix.rotate(-stheta, 1.0, 0.0, 0.0);
    m_ModelMatrix.rotate(sphi, 0.0, 0.0, 1.0);
    m_ModelMatrix.translate(-0.5, -0.5, 0.0);

    glUniformMatrix4fv(m_uniform[4][VIEWMATRIX], 1, GL_FALSE, m_ModelMatrix.constData());
    //normal matrix
    QMatrix4x4 tem_matrix = m_ModelMatrix;
    m_NormalMatrix = tem_matrix.inverted();
    glUniformMatrix4fv(m_uniform[4][NORMALMATRIX], 1, GL_TRUE, m_NormalMatrix.constData());
    m_ViewMatrix.setToIdentity();
    m_ProjectMatrix.setToIdentity();
    m_ProjectMatrix.perspective(64.0, aspect, zNear, zFar);

    m_ModelMatrix = m_ViewMatrix * m_ModelMatrix;
    m_ModelMatrix = m_ProjectMatrix * m_ModelMatrix;


    glUniformMatrix4fv(m_uniform[4][MVP], 1, GL_FALSE, m_ModelMatrix.constData());

    //ambient

    glUniform3f(m_uniform[4][AMBIENT], ambient.x(), ambient.y(), ambient.z());

    //diffuse
//    diffuseProduct = lightcolor * a_color;
//    glUniform3f(m_uniform[4][DIFFUSE], diffuseProduct.x(), diffuseProduct.y(), diffuseProduct.z());

//    specularProduct = lightcolor * a_color;
//    glUniform3f(m_uniform[4][SPECULAR], specularProduct.x(), specularProduct.y(), specularProduct.z());

    //light direction
    glUniform3f(m_uniform[4][LIGHT], lightDirection.x(), lightDirection.y(),lightDirection.z());


    //Shininess
    glUniform1f(m_uniform[4][SHININESS],shininess);

    glUniform3f(m_uniform[4][LIGHTCOLOR], lightcolor.x(), lightcolor.y(), lightcolor.z());

    glUniform1i(m_uniform[4][SAMPLER], 1);

}


void HW4::initPosition(){

    m_ModelMatrix.setToIdentity();
    m_ModelMatrix.translate(0.0,0.0,-sdepth);
    //
    m_ModelMatrix.rotate(-stheta, 1.0, 0.0, 0.0);
    m_ModelMatrix.rotate(sphi, 0.0, 0.0, 1.0);
    m_ModelMatrix.translate(-0.5, -0.5, 0.0);

    glUseProgram(m_program[2].programId());
    QMatrix4x4 tem_matrix = m_ModelMatrix;
    m_NormalMatrix = tem_matrix.inverted();
    glUniformMatrix4fv(m_uniform[2][NORMALMATRIX], 1, GL_TRUE, m_NormalMatrix.constData());

    glUseProgram(m_program[3].programId());
    tem_matrix = m_ModelMatrix;
    m_NormalMatrix = tem_matrix.inverted();
    glUniformMatrix4fv(m_uniform[3][NORMALMATRIX], 1, GL_TRUE, m_NormalMatrix.constData());

    glUseProgram(m_program[4].programId());
    tem_matrix = m_ModelMatrix;
    m_NormalMatrix = tem_matrix.inverted();
    glUniformMatrix4fv(m_uniform[4][NORMALMATRIX], 1, GL_TRUE, m_NormalMatrix.constData());


    m_ViewMatrix.setToIdentity();
    m_ProjectMatrix.setToIdentity();
    m_ProjectMatrix.perspective(64.0, aspect, zNear, zFar);

    m_ModelMatrix = m_ViewMatrix * m_ModelMatrix;
    m_ModelMatrix = m_ProjectMatrix * m_ModelMatrix;


    glUniformMatrix4fv(m_uniform[4][MVP], 1, GL_FALSE, m_ModelMatrix.constData());
    glUseProgram(m_program[3].programId());
    glUniformMatrix4fv(m_uniform[3][MVP], 1, GL_FALSE, m_ModelMatrix.constData());
    glUseProgram(m_program[2].programId());
    glUniformMatrix4fv(m_uniform[2][MVP], 1, GL_FALSE, m_ModelMatrix.constData());
    glUseProgram(m_program[1].programId());
    glUniformMatrix4fv(m_uniform[1][MVP], 1, GL_FALSE, m_ModelMatrix.constData());
    glUseProgram(m_program[0].programId());
    glUniformMatrix4fv(m_uniform[0][MVP], 1, GL_FALSE, m_ModelMatrix.constData());

}



void
HW4::initVertexBuffer()
{

    int i,j;

//    for (i=0; i<grid-1; i++) {
//        for (j=0; j<grid-1; j++){
//            m_points.push_back(vec3(i/((float)grid-1)/1.0,j/((float)grid-1)/1.0,posit[i][j]/((float)grid-1)/1.0));
//            m_points.push_back(vec3(i/((float)grid-1)/1.0,(j+1)/((float)grid-1)/1.0,posit[i][j+1]/((float)grid-1)/1.0));
//            m_points.push_back(vec3((i+1)/((float)grid-1)/1.0,(j+1)/((float)grid-1)/1.0,posit[i+1][j+1]/((float)grid-1)/1.0));
//            m_points.push_back(vec3((i+1)/((float)grid-1)/1.0,j/((float)grid-1)/1.0,posit[i+1][j]/((float)grid-1)/1.0));
//        }
//    }

    for (i=0; i<grid-1; i++) {
        for (j=0; j<grid-1; j++){
            m_points.push_back(vec3(i/((float)grid-1)/1.0,(j+1)/((float)grid-1)/1.0,posit[i][j+1]/((float)grid-1)/1.0));
            m_points.push_back(vec3(i/((float)grid-1)/1.0,(j)/((float)grid-1)/1.0,posit[i][j]/((float)grid-1)/1.0));
            m_points.push_back(vec3((i+1)/((float)grid-1)/1.0,j/((float)grid-1)/1.0,posit[i+1][j]/((float)grid-1)/1.0));
            m_points.push_back(vec3(i/((float)grid-1)/1.0,(j+1)/((float)grid-1)/1.0,posit[i][j+1]/((float)grid-1)/1.0));
            m_points.push_back(vec3((i+1)/((float)grid-1)/1.0,j/((float)grid-1)/1.0,posit[i+1][j]/((float)grid-1)/1.0));
            m_points.push_back(vec3((i+1)/((float)grid-1)/1.0,(j+1)/((float)grid-1)/1.0,posit[i+1][j+1]/((float)grid-1)/1.0));
        }
    }

    m_numPoints = m_points.size();	// save number of vertices

    static GLuint vertexBuffer = -1;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_points[0], GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_VERTEX);

    getTexCoords();

//    for (i=0; i<grid-1; i++) {
//        for (j=0; j<grid-1; j++){
//            m_coords.push_back(vec2(texCoords[i][j][0],texCoords[i][j][1]));
//            m_coords.push_back(vec2(texCoords[i][j+1][0],texCoords[i][j+1][1]));
//            m_coords.push_back(vec2(texCoords[i+1][j+1][0],texCoords[i+1][j+1][1]));
//            m_coords.push_back(vec2(texCoords[i+1][j][0],texCoords[i+1][j][1]));


//        }
//    }

    for (i=0; i<grid-1; i++) {
        for (j=0; j<grid-1; j++){
            m_coords.push_back(vec2(texCoords[i][j+1][0],texCoords[i][j+1][1]));
            m_coords.push_back(vec2(texCoords[i][j][0],texCoords[i][j][1]));
            m_coords.push_back(vec2(texCoords[i+1][j][0],texCoords[i+1][j][1]));
            m_coords.push_back(vec2(texCoords[i][j+1][0],texCoords[i][j+1][1]));
            m_coords.push_back(vec2(texCoords[i+1][j][0],texCoords[i+1][j][1]));
            m_coords.push_back(vec2(texCoords[i+1][j+1][0],texCoords[i+1][j+1][1]));


        }
    }

    //Create texture coordinates buffer
    static GLuint texCoordBuffer = -1;
    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_coords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_TEXTURE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_TEXTURE_POSITION);



    getFaceNorms();

    for (i=0; i<grid-1; i++) {
        for (j=0; j<grid-1; j++){
            m_normals.push_back(vec3(faceNorms[0][i][j][0],faceNorms[0][i][j][1],faceNorms[0][i][j][2]));
            m_normals.push_back(vec3(faceNorms[0][i][j][0],faceNorms[0][i][j][1],faceNorms[0][i][j][2]));
            m_normals.push_back(vec3(faceNorms[0][i][j][0],faceNorms[0][i][j][1],faceNorms[0][i][j][2]));
            m_normals.push_back(vec3(faceNorms[1][i][j][0],faceNorms[1][i][j][1],faceNorms[1][i][j][2]));
            m_normals.push_back(vec3(faceNorms[1][i][j][0],faceNorms[1][i][j][1],faceNorms[1][i][j][2]));
            m_normals.push_back(vec3(faceNorms[1][i][j][0],faceNorms[1][i][j][1],faceNorms[1][i][j][2]));
        }
    }

    static GLuint normalBuffer = -1;
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_NORMAL);



    getVertNorms();


    for (i=0; i<grid-1; i++) {
        for (j=0; j<grid-1; j++){
            m_average_normals.push_back(vec3(vertNorms[i][j+1][0],vertNorms[i][j+1][1],vertNorms[i][j+1][2]));
            m_average_normals.push_back(vec3(vertNorms[i][j][0],vertNorms[i][j][1],vertNorms[i][j][2]));
            m_average_normals.push_back(vec3(vertNorms[i+1][j][0],vertNorms[i+1][j][1],vertNorms[i+1][j][2]));
            m_average_normals.push_back(vec3(vertNorms[i][j+1][0],vertNorms[i][j+1][1],vertNorms[i][j+1][2]));
            m_average_normals.push_back(vec3(vertNorms[i+1][j][0],vertNorms[i+1][j][1],vertNorms[i+1][j][2]));
            m_average_normals.push_back(vec3(vertNorms[i+1][j+1][0],vertNorms[i+1][j+1][1],vertNorms[i+1][j+1][2]));

        }
    }

    static GLuint vector_normalBuffer = -1;
    glGenBuffers(1, &vector_normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vector_normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_average_normals[0], GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_VECTOR_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_VECTOR_NORMAL);


    for (i=0; i<grid-1; i++) {
        for (j=0; j<grid-1; j++){
           m_colors.push_back(a_color);
           m_colors.push_back(a_color);
           m_colors.push_back(a_color);
           m_colors.push_back(a_color);
           m_colors.push_back(a_color);
           m_colors.push_back(a_color);

        }
    }

    static GLuint color_buffer = -1;
    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), &m_colors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_COLOR);


    m_points.clear();
    m_coords.clear();
    m_normals.clear();
    m_average_normals.clear();
    m_colors.clear();

}




void
HW4::resizeGL(int w, int h)
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

void HW4::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1, 1);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    switch (displayMode) {


        case TEXTURED:
            glUseProgram(m_program[0].programId());
            glDrawArrays(GL_QUADS, 0, (GLsizei) m_numPoints);
            break;
        case WIREFRAME:
            glUseProgram(m_program[1].programId());		// bind the glsl progam
            glLineWidth(2.0f);
            for(int i=0; i<(int)m_numPoints; i+=4)
                glDrawArrays(GL_LINE_LOOP, i, (GLsizei) 4);
            break;
        case TEX_WIRE:
            glUseProgram(m_program[0].programId());
            glDrawArrays(GL_QUADS, 0, (GLsizei) m_numPoints);

            glUseProgram(m_program[1].programId());		// bind the glsl progam
            glLineWidth(2.0f);
            for(int i=0; i<(int)m_numPoints; i+=4)
                glDrawArrays(GL_LINE_LOOP, i, (GLsizei) 4);
            break;
         case FLAT_SHADING:
            glUseProgram(m_program[2].programId());		// bind the glsl progam
//            glLineWidth(2.0f);
//            for(int i=0; i<(int)m_numPoints; i+=3)
                glDrawArrays(GL_TRIANGLES, 0, (GLsizei) m_numPoints);
            break;
         case SMOOTH_SHADING:
            glUseProgram(m_program[3].programId());		// bind the glsl progam
//            glLineWidth(2.0f);
//            for(int i=0; i<(int)m_numPoints; i+=3)
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei) m_numPoints);
            break;
         case SMOOTH_SHADING_TEX:
            glUseProgram(m_program[4].programId());		// bind the glsl progam
//            glLineWidth(2.0f);
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei) m_numPoints);
            break;
    }
}

void HW4::initPoints()
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

}

void HW4::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        leftButton = true;
        downX = event->x();
        downY = event->y();

    }
}

void HW4::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        leftButton = false;
    }
}

void HW4::mouseMoveEvent(QMouseEvent *event)
{
    if (leftButton==true) {

        sphi += (float)(event->x() - downX) / 4.0;
        stheta += (float)(downY - event->y()) / 4.0;

        downX = event->x();
        downY = event->y();
        initPosition();
        updateGL();


    }
}

void HW4::getforce()
{
    float d;

    for(int i=0;i<grid;i++)
        for(int j=0;j<grid;j++)
            force[i][j]=0.0;

    for(int i=1;i<grid-1;i++) {
        for(int j=1;j<grid-1;j++) {
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

void HW4::getvelocity()
{
    for(int i=0;i<grid;i++)
        for(int j=0;j<grid;j++)
            veloc[i][j]+=force[i][j] * dt;
}

void HW4::getposition()
{
    for(int i=0;i<grid;i++)
        for(int j=0;j<grid;j++)
            posit[i][j]+=veloc[i][j];
}


void HW4::copy(float vec0[3], float vec1[3])
{
    vec0[0] = vec1[0];
    vec0[1] = vec1[1];
    vec0[2] = vec1[2];
}

void HW4::sub(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[0] - vec2[0];
    vec0[1] = vec1[1] - vec2[1];
    vec0[2] = vec1[2] - vec2[2];
}

void HW4::add(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[0] + vec2[0];
    vec0[1] = vec1[1] + vec2[1];
    vec0[2] = vec1[2] + vec2[2];
}

void HW4::scalDiv(float vec[3], float c)
{
    vec[0] /= c; vec[1] /= c; vec[2] /= c;
}

void HW4::cross(float vec0[3], float vec1[3], float vec2[3])
{
    vec0[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
    vec0[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
    vec0[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
}

void HW4::norm(float vec[3])
{
    float c = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    scalDiv(vec, c);
}

void HW4::set(float vec[3], float x, float y, float z)
{
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}



void HW4::getTexCoords()
{
    for(int i=0; i < grid; i++) {
        for(int j=0; j < grid; j++) {
            texCoords[i][j][0] = (float)j / (float)(grid-1);
            texCoords[i][j][1] = (float)i / (float)(grid-1);
        }
    }
}

void HW4::getFaceNorms()
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

void HW4::getVertNorms()
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

void HW4::reset (){

    timer->stop();
    waving = false;
    grid = 17;
    dt=0.006;
//    displayMode = TEX_WIRE;
//    resetMode = HILLFOUR;
    sphi=45.0;stheta=45.0;
    sdepth = 1.5;
    m_comboBox->setCurrentIndex(displayMode);
    m_comboBox_mode->setCurrentIndex(resetMode);
    m_spinboxSpeed->setValue(6);
    m_sliderSpeed->setValue(6);
    m_spinboxGrid->setValue(16);
    m_sliderGrid->setValue(16);
    initPosition();
    initPoints();
    initVertexBuffer();


    updateGL();
}
