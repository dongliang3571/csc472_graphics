

#include "HW1a.h"


HW1a::HW1a(QWidget *parent)
	: HW(parent)
{}




void
HW1a::initializeGL()
{

	glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
	glColor3f(1.0, 1.0, 0.0);		// set foreground color
}




void
HW1a::resizeGL(int w, int h)
{
	m_winW = w;
	m_winH = h;

	glViewport(0, 0, w, h);

	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}




void
HW1a::paintGL()
{
	int i, w2, h2;
	w2 = m_winW;
	h2 = m_winH;

	QVector2D p[] = { 
		QVector2D(0.2, 0.2), QVector2D(0.2, 0.3), QVector2D(0.2, 0.4), QVector2D(0.2, 0.5), 
		QVector2D(0.2, 0.6), QVector2D(0.2, 0.7), QVector2D(0.2, 0.8), QVector2D(0.3, 0.8), 
		QVector2D(0.4, 0.8), QVector2D(0.5, 0.8), QVector2D(0.575, 0.725),QVector2D( 0.6, 0.65 ), 
		QVector2D( 0.575, 0.575 ), QVector2D( 0.5, 0.5 ), QVector2D( 0.4, 0.5 ), QVector2D( 0.3, 0.5 )};


	
	glClear(GL_COLOR_BUFFER_BIT);

	
	glViewport(0, 0, w2 / 3, h2 / 3);
	glBegin(GL_POINTS);
	for (i = 0; i<16; i++) glVertex2f(p[i].x(),p[i].y());
	glEnd();

	glViewport(w2 / 3, 0, w2 / 3, h2 / 3);
	glBegin(GL_LINES);
	for (i = 0; i<16; i++) glVertex2f(p[i].x(), p[i].y());
	glEnd();

	glViewport(w2 - w2 / 3, 0, w2 / 3, h2 / 3);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i<16; i++) glVertex2f(p[i].x(), p[i].y());
	glEnd();

	glLoadIdentity();    // turn P upside down
	glOrtho(0.0, 1.0, 1.0, 0.0, -1.0, 1.0);
	glViewport(0, h2 / 3, w2 / 3, h2 / 3);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i<16; i++) glVertex2f(p[i].x(), p[i].y());
	glEnd();

	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0); //turn P back to normal position
	glViewport(w2 / 3, h2 / 3, w2 / 3, h2 / 3);
	glBegin(GL_TRIANGLES);
	for (i = 0; i<16; i++) glVertex2f(p[i].x(), p[i].y());
	glEnd();

	glViewport(w2 - w2 / 3, h2 / 3, w2 / 3, h2 / 3);
	glBegin(GL_TRIANGLE_STRIP);
	for (i = 0; i<16; i++) glVertex2f(p[i].x(), p[i].y());
	glEnd();

	glViewport(0, h2 - h2 / 3, w2 / 3, h2 / 3);
	glBegin(GL_TRIANGLE_FAN);
	for (i = 0; i<16; i++) glVertex2f(p[i].x(), p[i].y());
	glEnd();

	glViewport(w2 / 3, h2 - h2 / 3, w2 / 3, h2 / 3);
	glBegin(GL_QUADS);
	for (i = 0; i<16; i++) glVertex2f(p[i].x(), p[i].y());
	glEnd();

	glViewport(w2 - w2 / 3, h2 - h2 / 3, w2 / 3, h2 / 3);
	glBegin(GL_POLYGON);
	for (i = 0; i<16; i++) glVertex2f(p[i].x(), p[i].y());
	glEnd();
	glFlush();
}


QGroupBox*
HW1a::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 1a");

	return(groupBox);
}
