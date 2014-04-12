#include <GL/glut.h>

int mouse_x_old  = 0;
int mouse_y_old  = 0;
int mouse_x  = 0;
int mouse_y  = 0;
int angle_x = 0;
int angle_y = 0;
int eye_dist = 0;

void draw_cube() {
    
    static GLfloat cube[8][3] =
    { { 0.0, 0.0, 0.0 },
      { 1.0, 0.0, 0.0 },
      { 1.0, 0.0, 1.0 },
      { 0.0, 0.0, 1.0 },
      { 1.0, 1.0, 0.0 },
      { 1.0, 1.0, 1.0 },
      { 0.0, 1.0, 1.0 },
      { 0.0, 1.0, 0.0 } };

    static int faceIndex[6][4] =
    { { 0, 1, 2, 3 },
      { 1, 4, 5, 2 },
      { 4, 7, 6, 5 },
      { 7, 0, 3, 6 },
      { 3, 2, 5, 6 },
      { 7, 4, 1, 0 } };

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 0.0);

    for ( unsigned int face = 0; face < 6; face++ ) {
        glBegin(GL_LINE_LOOP);
            for ( unsigned int line = 0; line < 4; line++  )  {
                glVertex3fv((GLfloat *) cube[faceIndex[face][line]]);
            }
        glEnd();
    }

    glutSwapBuffers();
}

void draw_scene() {
    glPushMatrix();
        glRotatef( angle_y, 1.0, 0.0, 0.0);
        glRotatef( angle_x, 0.0, 1.0, 0.0);
        draw_cube();
    glPopMatrix();
}

void
setMatrix(void)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void
resize(int w, int h)
{
  glViewport(0, 0, w, h);
  setMatrix();
}

void mouse_callback(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		if (button == 3)
			eye_dist +=1;
		else if (button == 4) {
			eye_dist -=1;
        }
	}
}

void mousemotion(int x, int y) {
	mouse_x_old = mouse_x;
	mouse_y_old = mouse_y;
    mouse_x = x;
    mouse_y = y;
    
    angle_x += mouse_x_old - mouse_x;
    angle_y += mouse_y_old - mouse_y;

	glutPostRedisplay();
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(1000, 1000);
    glutInitDisplayMode(GLUT_RGB | GLUT_STENCIL | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Moldinam visualizer");

    glutDisplayFunc( draw_scene );
    glutReshapeFunc(resize);
    glutMouseFunc(mouse_callback);
	glutMotionFunc(mousemotion);

    glutMainLoop();

    return 0;
}
