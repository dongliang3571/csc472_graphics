//
//  HW2a.cpp
//  hw
//
//  Created by dong liang on 10/13/15.
//
//

#include "HW2a.h"

HW2a::HW2a(QWidget *parent): HW(parent)
{
    
}

void HW2a::initializeGL(){
    
    initializeGLFunctions();
    
    // init vertex and fragment shaders
    initShaders();
    
    // initialize vertex buffer and write positions to vertex shader
    initVertexBuffer();
    
    // init state variables
    glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
    glColor3f   (1.0, 1.0, 0.0);
    
}


void HW2a::initShaders(){
    // compile vertex shader
    if(!m_program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader2a.glsl")) {
        QMessageBox::critical(0, "Error", "Vertex shader error", QMessageBox::Ok);
        QApplication::quit();
    }
    
    // compile fragment shader
    if(!m_program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader2a.glsl")) {
        QMessageBox::critical(0, "Error", "Fragment shader error",QMessageBox::Ok);
        QApplication::quit();
    }
    
    // link shader pipeline
    if(!m_program.link()) {
        QMessageBox::critical(0, "Error", "Could not link shader", QMessageBox::Ok);
        QApplication::quit();
    }
    
    // bind the glsl progam
    glUseProgram(m_program.programId());
    
    glBindAttribLocation(m_program.programId(), ATTRIB_VERTEX, "a_Position");
    
    
    // get storage location of u_ModelMatrix in vertex shader
    m_u_Color = glGetUniformLocation(m_program.programId(), "u_Color");
    if((int) m_u_Color < 0) {
        qDebug() << "Failed to get the storage location of u_ModelMatrix";
        exit(-1);
    }
    
    glUniform4f(m_u_Color, 1.0f, 1.0f, 0.0f,1.0f);
    
    
}

void HW2a::initVertexBuffer(){
    
    // set flag for creating buffers (1st time only)
    static bool flag = 1;
    
    // verify that we have valid vertex/color buffers
    static GLuint vertexBuffer = -1;

    if(flag) {	// create vertex and color buffers
        glGenBuffers(1, &vertexBuffer);
        flag = 0;	// reset flag
    }
    
    // init geometry data
    const vec2 vertices[] = {
    
        vec2(0.2, 0.2), vec2(0.2, 0.3), vec2(0.2, 0.4), vec2(0.2, 0.5),
        vec2(0.2, 0.6), vec2(0.2, 0.7), vec2(0.2, 0.8), vec2(0.3, 0.8),
        vec2(0.4, 0.8), vec2(0.5, 0.8), vec2(0.575, 0.725),vec2( 0.6, 0.65 ),
        vec2( 0.575, 0.575 ), vec2( 0.5, 0.5 ), vec2( 0.4, 0.5 ), vec2( 0.3, 0.5 )
    };
    
    // store vertex positions and colors in m_points and m_colors, respectively
    for(int i=0; i<16; i++) {
        m_points.push_back(vertices[i]);
    }
    m_numPoints = m_points.size();		// save number of vertices
    
    // bind the buffer to the GPU; all future drawing calls gets data from this buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    // copy the vertices from CPU to GPU
    glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_points[0], GL_STATIC_DRAW);
    
    // enable the assignment of attribute vertex variable
    glEnableVertexAttribArray(ATTRIB_VERTEX);
    
    // assign the buffer object to the attribute vertex variable
    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, false, 0, NULL);
    
    
    
    // clear vertex vectors because they have already been copied into GPU
    m_points.clear();

    
    
}


void
HW2a::resizeGL(int w, int h)
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

void HW2a::paintGL()
{
    // clear canvas with background color
    glClear(GL_COLOR_BUFFER_BIT);
    
    // draw triangles
    glDrawArrays(GL_LINE_LOOP, 0, 16);
}


QGroupBox* HW2a::controlPanel()
{
    // init group box
    QGroupBox *groupBox = new QGroupBox("Homework 2a");
    
    
    return(groupBox);
}
