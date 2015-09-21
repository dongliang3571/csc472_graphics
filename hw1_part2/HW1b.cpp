

#include "HW1b.h"




HW1b::HW1b(QWidget *parent)
	: HW(parent)
{}




void
HW1b::initializeGL()
{

	glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
	glColor3f(1.0, 1.0, 1.0);		// set foreground color


	
	QVector2D p2[] = { QVector2D(0.1, 0.2), QVector2D(0.5, 0.893), QVector2D(0.9, 0.2) };



	divide_triangle(p2[0], p2[1], p2[2], 9);
	
}




void
HW1b::resizeGL(int w, int h)
{
	m_winW = w;
	m_winH = h;

	glViewport(0, 0, w, h);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}




void
HW1b::paintGL()
{
	int i, w2, h2;
	w2 = m_winW;
	h2 = m_winH;


	//vector2f a ={0.1, 0.2};
	//vector2f c ={0.5, 0.893};
	//vector2f b ={0.9,0.2};
	



	glClear(GL_COLOR_BUFFER_BIT);


	glViewport(0, 0, w2, h2);
	glBegin(GL_TRIANGLES);
	for (i = 0; i<m_count; i++) glVertex2f(p[i].x(), p[i].y());
	glEnd();
	glFlush();



}


QGroupBox*
HW1b::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 1b");
	groupBox->setMinimumWidth(300);

	m_sliderDividing = new QSlider(Qt::Horizontal);
	m_sliderDividing->setRange(0, 360);
	m_sliderDividing->setValue(0);

	// create spinBox
	m_spinBoxDividing = new QSpinBox;
	m_spinBoxDividing->setRange(0, 360);
	m_spinBoxDividing->setValue(0);
	
	QGridLayout *layout = new QGridLayout;
	QLabel *label = new QLabel("Subdivide");

	layout->addWidget(label,0,0,0,1);
	layout->addWidget(m_sliderDividing,0,1,0,2);
	layout->addWidget(m_spinBoxDividing,0,3);
	
	
	

	groupBox->setLayout(layout);

	return(groupBox);
}


void HW1b::divide_triangle(QVector2D a, QVector2D b, QVector2D c, int k){

	if (k>0) {

		float midab1 = (a.x() + b.x()) / 2.0;
		float midab2 = (a.y() + b.y()) / 2.0;

		float midac1 = (a.x() + c.x()) / 2.0;
		float midac2 = (a.y() + c.y()) / 2.0;

		float midbc1 = (b.x() + c.x()) / 2.0;
		float midbc2 = (b.y() + c.y()) / 2.0;

		QVector2D tema;
		tema.setX(midab1);
		tema.setY(midab2);
		QVector2D temb;
		temb.setX(midac1);
		temb.setY(midac2);
		QVector2D temc;
		temc.setX(midbc1);
		temc.setY(midbc2);


		divide_triangle(a, tema, temb, k - 1);
		divide_triangle(c, temb, temc, k - 1);
		divide_triangle(b, temc, tema, k - 1);
	}
	else triangle(a, b, c);



}
void HW1b::triangle(QVector2D a, QVector2D b, QVector2D c){

	p[m_count].setX(a.x());
	p[m_count].setY(a.y());
	m_count++;

	p[m_count].setX(b.x());
	p[m_count].setY(b.y());
	m_count++;

	p[m_count].setX(c.x());
	p[m_count].setY(c.y());
	m_count++;


}
