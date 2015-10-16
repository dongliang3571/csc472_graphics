#ifndef HW1B_H
#define HW1B_H

#include "HW.h"

typedef QVector2D vec2;
typedef QVector3D vec3;

class HW1b : public HW {
	Q_OBJECT
public:
	HW1b(QWidget *parent = 0);
	QGroupBox*	controlPanel();


public slots:

	void changeDividing(int value); //Slider calls his function to trigger dividing
	void changeTheta(int angle);
	void setToTwist(int state);


	

protected:
	void		initializeGL();
	void		resizeGL(int, int);
	void		paintGL();


private:

	void divide_triangle(vec2 a, vec2 b, vec2 c, int k);
	void triangle(vec2 a, vec2 b, vec2 c);
	vec2 twisting(vec2 point);
	void initPoint();
	

	int		 m_winW;
	int		 m_winH;
	float      ar;
	float		 maxW;
	float		 maxH;

	int m_subdivide;
	//bool m_changeColor;

	bool m_aspectCheck;
	float m_theta;
	
	

	std::vector<QVector2D> m_point;
	std::vector<QVector3D> m_color;

	


	QSlider *m_sliderDividing;
	QSpinBox *m_spinBoxDividing;
	
	QSlider *m_sliderTheta;
	QSpinBox *m_spinBoxTheta;

	QCheckBox *m_checkBoxTwist;
	

	

};

#endif 