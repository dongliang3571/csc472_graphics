#ifndef HW1B_H
#define HW1B_H

#include "HW.h"


class HW1b : public HW {
	Q_OBJECT
public:
	HW1b(QWidget *parent = 0);
	QGroupBox*	controlPanel();

	

protected:
	void		initializeGL();
	void		resizeGL(int, int);
	void		paintGL();

private:

	void divide_triangle(QVector2D a, QVector2D b, QVector2D c, int k);
	void triangle(QVector2D a, QVector2D b, QVector2D c);

	int		 m_winW;
	int		 m_winH;
	int		 m_count=0;


	QVector2D p[1000000] = {};


	QSlider *m_sliderDividing;
	QSpinBox *m_spinBoxDividing;
	
	

};

#endif 