

#include "HW1b.h"

#include <iostream>


HW1b::HW1b(QWidget *parent)
	: HW(parent)
{
	m_subdivide = 0;
	m_aspectCheck = true;
	maxW = 1;
	maxH = 1;
	//m_changeColor = true;
	m_theta = 0;

}




void
HW1b::initializeGL()
{

	glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
	glColor3f(1.0, 1.0, 1.0);		// set foreground color

	initPoint();
	
}

void HW1b::initPoint(){
	vec2 p2[] = { vec2(-0.9, -0.4), vec2(0.0, 0.75), vec2(0.9, -0.4) };
	divide_triangle(p2[0], p2[1], p2[2], m_subdivide);
}


void
HW1b::resizeGL(int w, int h)
{
	m_winW = w;
	m_winH = h;
	ar = (float) m_winH / m_winW;

	

	if (m_aspectCheck == true){
		if (ar<1.0){
			maxH = 1.0;
			maxW = 1.0 / ar;

		}
		else{
			maxH = 1.0*ar;
			maxW = 1.0;
		}

	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-maxW, maxW, -maxH, maxH, -1.0, 1.0);
}




void
HW1b::paintGL()
{
	int i, w2, h2;
	w2 = m_winW;
	h2 = m_winH;


	glClear(GL_COLOR_BUFFER_BIT);


	
		

		glBegin(GL_TRIANGLES);

		for (i = 0; i < m_point.size(); i++){

			glColor3f(m_color[i][0], m_color[i][1], m_color[i][2]);
			glVertex2f(m_point[i][0], m_point[i][1]);
		
		}

		
		
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


void HW1b::divide_triangle(vec2 a, vec2 b, vec2 c, int k){

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
void HW1b::triangle(vec2 a, vec2 b, vec2 c){

	m_point.push_back(twisting(a));
	m_point.push_back(twisting(b));
	m_point.push_back(twisting(c));

	

	m_color.push_back(vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
	m_color.push_back(vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
	m_color.push_back(vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));

	


	

}

vec2 HW1b::twisting(vec2 point){
	
	float d;
	
	if (m_checkBoxTwist->isChecked())
		d = sqrt(point[0] * point[0] + point[1] * point[1]);
	else
		d = 1.0;
	float sinTheta = sin(d*m_theta);
	float cosTheta = cos(d*m_theta);
	
	return vec2(point[0] * cosTheta - point[1] * sinTheta, point[0] * sinTheta + point[1] * cosTheta);
}

void HW1b::changeTheta(int angle){

	
	m_sliderTheta->blockSignals(true);
	m_sliderTheta->setValue(angle);
	m_sliderTheta->blockSignals(false);



	m_spinBoxTheta->blockSignals(true);
	m_spinBoxTheta->setValue(angle);
	m_spinBoxTheta->blockSignals(false);




	m_theta = angle * (M_PI / 180);
	m_point.clear();

	initPoint();
	updateGL();
	
}

void HW1b::setToTwist(int state){

	m_checkBoxTwist->setChecked(state);

	m_point.clear();
	initPoint();
	updateGL();
}

void HW1b::changeDividing(int value){

	m_subdivide = value;
	m_sliderDividing->blockSignals(true);
	m_sliderDividing->setValue(value);
	m_sliderDividing->blockSignals(false);

	

	m_spinBoxDividing->blockSignals(true);
	m_spinBoxDividing->setValue(value);
	m_spinBoxDividing->blockSignals(false);

	m_point.clear();
	m_color.clear();

	initPoint();
	updateGL();

}

