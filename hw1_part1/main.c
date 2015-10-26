



#include <GL/glut.h>

/* function prototypes */
void init	(void);
void display	(void);
void reshape	(int, int);

/* user-defined abstract datatype */
typedef float	vector2f[2];

/* global variables: window dimensions and vertex list */
int		WinW, WinH;
vector2f	p[] = {{0.2,0.2}, {0.2,0.3}, {0.2,0.4},{0.2,0.5}, {0.2,0.6}, {0.2,0.7}, {0.2,0.8}, {0.3,0.8}, {0.4,0.8}, {0.5,0.8}, {0.575,0.725}, {0.6,0.65}, {0.575,0.575}, {0.5,0.5}, {0.4,0.5}, {0.3,0.5}};


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * main:
 *
 * Initialize window info, state variables, and callback functions.
 * Enter main loop and process events.
 */
int
main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); /* double-buffer; RGB    */
    glutInitWindowSize(500, 500);		/* initialize window size     */
    glutInitWindowPosition(100, 100);	/* initialize window position */
    glutCreateWindow(argv[0]);		/* set titlebar name; create  */
    init();					/* initialize state variables */
    glutDisplayFunc(display);		/* display callback function  */
    glutReshapeFunc(reshape);		/* reshape callback function  */
    glutMainLoop();				/* enter infinite event loop  */
    return 0;				/* ANSI C requires int main() */
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * init:
 *
 * Initialize state variables.
 *
 */
void
init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);	/* set background color */
    glColor3f   (1.0, 1.0, 0.0);		/* set foreground color */
    
    /* initialize viewing values; setup unit view cube */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * display:
 *
 * Display callback function.
 */
void
display(void)
{
    int	i, w2, h2;
    
    /* init w2 and h2 with dimensions of a window quadrant */
    w2 = WinW;
    h2 = WinH;
    
    glClear(GL_COLOR_BUFFER_BIT);		/* clear FB with backgd color */
    
    
    glViewport(0, 0, w2/3, h2/3);
    glBegin(GL_POINTS);
    for(i=0; i<16; i++) glVertex2fv(p[i]);
    glEnd();
    
    glViewport(w2/3, 0, w2/3, h2/3);
    glBegin(GL_LINES);
    for(i=0; i<16; i++) glVertex2fv(p[i]);
    glEnd();
    
    glViewport(w2-w2/3, 0, w2/3, h2/3);
    glBegin(GL_LINE_STRIP);
    for(i=0; i<16; i++) glVertex2fv(p[i]);
    glEnd();
    
    glLoadIdentity();    // turn P upside down
    glOrtho(0.0,1.0,1.0,0.0,-1.0,1.0);
    glViewport(0, h2/3, w2/3, h2/3);
    glBegin(GL_LINE_LOOP);
    for(i=0; i<16; i++) glVertex2fv(p[i]);
    glEnd();
    
    glLoadIdentity();
    glOrtho(0.0,1.0,0.0,1.0,-1.0,1.0); //turn P back to normal position
    glViewport(w2/3, h2/3, w2/3, h2/3);
    glBegin(GL_TRIANGLES);
    for(i=0; i<16; i++) glVertex2fv(p[i]);
    glEnd();
    
    glViewport(w2-w2/3, h2/3, w2/3, h2/3);
    glBegin(GL_TRIANGLE_STRIP);
    for(i=0; i<16; i++) glVertex2fv(p[i]);
    glEnd();
    
    glViewport(0, h2-h2/3, w2/3, h2/3);
    glBegin(GL_TRIANGLE_FAN);
    for(i=0; i<16; i++) glVertex2fv(p[i]);
    glEnd();
    
    glViewport(w2/3, h2-h2/3, w2/3, h2/3);
    glBegin(GL_QUADS);
    for(i=0; i<16; i++) glVertex2fv(p[i]);
    glEnd();
    
    glViewport(w2-w2/3, h2-h2/3, w2/3, h2/3);
    glBegin(GL_POLYGON);
    for(i=0; i<16; i++) glVertex2fv(p[i]);
    glEnd();
    
    
   	glFlush();				/* draw to (off-screen) buffer*/
    glutSwapBuffers();			/* swap buffer to see drawing */
}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * reshape:
 *
 * Reshape callback function.
 */
void
reshape(int w, int h)
{
    /* init global variables with window dimensions; for use in display() */
    WinW = w;
    WinH = h;
    
    /* initialize viewing values; setup unit view cube */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}
