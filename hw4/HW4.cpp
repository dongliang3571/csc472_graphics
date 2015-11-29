


#include "HW4.h"

enum {MVP, VIEWMATRIX, PROJECTMATRIX, SAMPLER};
enum {TEXTURED, WIREFRAME, TEX_WIRE};
enum {FLAT, SPIKE, DIAGONALWALL, SIDEWALL, HOLE,
    MIDDLEBLOCK, DIAGONALBLOCK, CORNERBLOCK, HILL, HILLFOUR};
#define SQRTOFTWOINV 1.0 / 1.414213562



HW4::HW4(QWidget *parent)
    : HW(parent)
{
    waving = false;
    grid = 17;
    dt=0.006;
    displayMode = TEX_WIRE;
    resetMode = HILLFOUR;
    sphi=45.0;stheta=45.0;
    sdepth = 1.5;
    zNear=0.1; zFar=5.0;
    aspect = 5.0/4.0;
    timer = new QTimer(this);
    leftButton = false;

    connect(timer, SIGNAL(timeout()), this, SLOT(wave()));

}



QGroupBox*
HW4::controlPanel()
{
    // init group box
    QGroupBox *groupBox = new QGroupBox("Homework 3a");

    groupBox->setMinimumWidth(300);
    QGridLayout *layout = new QGridLayout;
    m_comboBox = new QComboBox();
    m_comboBox->addItem("Texture");
    m_comboBox->addItem("Wireframe");
    m_comboBox->addItem("Tex+Wire");
    m_comboBox->setCurrentIndex(TEX_WIRE);


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
    m_sliderGrid->setValue(16);

    m_spinboxGrid = new QSpinBox;
    m_spinboxGrid->setRange(2,64);
    m_spinboxGrid->setValue(16);


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
    glColor3f   (1.0, 1.0, 0.0);		// set foreground color
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

void HW4::initPosition(){

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
    glUseProgram(m_program[1].programId());
    glUniformMatrix4fv(m_uniform[1][MVP], 1, GL_FALSE, m_ModelMatrix.constData());
    glUseProgram(m_program[0].programId());
    glUniformMatrix4fv(m_uniform[0][MVP], 1, GL_FALSE, m_ModelMatrix.constData());
}



void
HW4::initVertexBuffer()
{

    int i,j;

    for (i=0; i<grid-1; i++) {
        for (j=0; j<grid-1; j++){
            m_points.push_back(vec3(i/((float)grid-1)/1.0,j/((float)grid-1)/1.0,posit[i][j]/((float)grid-1)/1.0));
            m_points.push_back(vec3(i/((float)grid-1)/1.0,(j+1)/((float)grid-1)/1.0,posit[i][j+1]/((float)grid-1)/1.0));
            m_points.push_back(vec3((i+1)/((float)grid-1)/1.0,(j+1)/((float)grid-1)/1.0,posit[i+1][j+1]/((float)grid-1)/1.0));
            m_points.push_back(vec3((i+1)/((float)grid-1)/1.0,j/((float)grid-1)/1.0,posit[i+1][j]/((float)grid-1)/1.0));
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

    for (i=0; i<grid-1; i++) {
        for (j=0; j<grid-1; j++){
            m_coords.push_back(vec2(texCoords[i][j][0],texCoords[i][j][1]));
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
    glVertexAttribPointer(ATTRIB_TEXTURE_POSITION, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(ATTRIB_TEXTURE_POSITION);

    m_points.clear();
    m_coords.clear();

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
//            if (i==0||j==0||i==grid-1||j==grid-1) posit[i][j]=0.0;
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
