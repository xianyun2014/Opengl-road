#include <GL\glut.h>
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

void Render()
{
	static GLfloat rt = 73.0f;
	glPushMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glRotatef(rt, 1.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5, -0.5, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glEnd();

	glPopMatrix();
	glutSwapBuffers();
	rt += 0.1;
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(200, 20);
	glutInitWindowSize(600, 400);
	glutCreateWindow("opengl");
	glutFullScreen();
	glutDisplayFunc(Render);
	glutIdleFunc(Render);

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}