#ifndef HW1A_H
#define HW1A_H

#include "HW.h"


class HW1a : public HW {
	Q_OBJECT
public:
	HW1a(QWidget *parent = 0);		
	QGroupBox*	controlPanel();		

protected:
	void		initializeGL();		
	void		resizeGL(int, int);	
	void		paintGL();		
private:
	int		 m_winW;			
	int		 m_winH;

};

#endif 