#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>
#include <md_types.h>
#include <md_helpers.h>
#include <iostream>

int mouse_x_old  = 0;
int mouse_y_old  = 0;
int mouse_x  = 0;
int mouse_y  = 0;
int angle_x = 0;
int angle_y = 0;
int eye_dist = 0;

void draw_molecules() {
    static auto molecules = read_molecules_from_file( "test_norm.xyz" );

    glColor3f(1.0, 0.0, 0.0);
    for ( auto& mol : molecules ) {
        glTranslatef( mol.pos.x, mol.pos.y, mol.pos.z );
        glutSolidSphere( 0.02, 10, 10);
        glTranslatef( -mol.pos.x, -mol.pos.y, -mol.pos.z );
    }
}

void draw_cube() {
    
    std::vector< glm::fvec3 > cube = {   
        glm::fvec3( 0.0, 0.0, 0.0 ),
        glm::fvec3( 1.0, 0.0, 0.0 ),
        glm::fvec3( 1.0, 0.0, 1.0 ),
        glm::fvec3( 0.0, 0.0, 1.0 ),
        glm::fvec3( 1.0, 1.0, 0.0 ),
        glm::fvec3( 1.0, 1.0, 1.0 ),
        glm::fvec3( 0.0, 1.0, 1.0 ),
        glm::fvec3( 0.0, 1.0, 0.0 ) 
    };

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
                glVertex3fv( (GLfloat *) &cube[faceIndex[face][line]]);
            }
        glEnd();
    }

    draw_molecules();

    glutSwapBuffers();
}


void draw_scene() {
    glPushMatrix();
        //glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();

        glTranslated(0.5, 0.5, 0.5);

        glTranslatef( -0.5, -0.5, -0.5 );
        glRotatef( angle_y, 1.0, 0.0, 0.0);
        glRotatef( angle_x, 0.0, 1.0, 0.0);

        glTranslated(-0.5, -0.5, -0.5);

        draw_cube();

    glPopMatrix();
}

void setMatrix(void) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void resize(int w, int h) {
  glViewport(0, 0, w, h);
  setMatrix();
}

void mouse_callback(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		if (button == 3)
			eye_dist += 0.2;
		else if (button == 4) {
			eye_dist -= 0.2;
        }
	}
    else {
        mouse_x = x;
        mouse_y = y;
        mouse_x_old = mouse_x;
        mouse_y_old = mouse_y;
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
try {
    glutInit(&argc, argv);

    glutInitWindowSize(1000, 1000);
    glutInitDisplayMode(GLUT_RGB | GLUT_STENCIL | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("Moldinam visualizer");

    glutDisplayFunc( draw_scene );
    glutReshapeFunc(resize);
    glutMouseFunc(mouse_callback);
	glutMotionFunc(mousemotion);

    glutMainLoop();
} catch ( std::exception& ex ) {
    std::cout << "exception: " << ex.what() << std::endl;
}
    return 0;
}
