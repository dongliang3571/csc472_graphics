TEMPLATE = app
TARGET = hw
QT += widgets opengl
OBJECTS_DIR = ./obj
MOC_DIR     = ./moc
#RESOURCES   = hw.qrc

win32-msvc2013 {
	LIBS += -lopengl32 -lglu32
	QMAKE_CXXFLAGS += /MP /Zi
}


macx{
        QMAKE_MAC_SDK = macosx10.11
	QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
}

unix:!macx {
	CONFIG += C++11
}

# Input
HEADERS += MainWindow.h \
	   HW0a.h HW0b.h HW1a.h HW1b.h \
    HW2a.h \
    HW.h
SOURCES += main.cpp MainWindow.cpp \
	   HW0a.cpp HW0b.cpp HW1a.cpp HW1b.cpp \
    HW2a.cpp \
    HW.cpp

RESOURCES += \
    resources.qrc
