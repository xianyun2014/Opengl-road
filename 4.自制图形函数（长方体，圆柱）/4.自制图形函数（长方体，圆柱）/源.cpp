#include <gl/glut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut.lib")


#include <math.h>
void Cube()
{
	glBegin(GL_QUAD_STRIP);//���͹�����
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glBegin(GL_QUAD_STRIP);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();
}
void Circle()
{
	glBegin(GL_TRIANGLE_FAN);//����������������δ�
	glVertex3f(0.0f, 0.0f, 0.0f);
	int i = 0;
	for (i = 0; i <= 390; i += 15)
	{
		float p = i * 3.14 / 180;
		glVertex3f(sin(p), cos(p), 0.0f);
	}
	glEnd();
}
void Cylinder()
{
	glBegin(GL_QUAD_STRIP);//��������ı��δ�
	int i = 0;
	for (i = 0; i <= 390; i += 15)
	{
		float p = i * 3.14 / 180;
		glVertex3f(sin(p), cos(p), 1.0f);
		glVertex3f(sin(p), cos(p), 0.0f);
	}
	glEnd();
	Circle();
	glTranslatef(0, 0, 1);
	Circle();
}
void Cone()
{
	glBegin(GL_QUAD_STRIP);//��������ı��δ�
	int i = 0;
	for (i = 0; i <= 390; i += 15)
	{
		float p = i * 3.14 / 180;
		glVertex3f(0, 0, 1.0f);
		glVertex3f(sin(p), cos(p), 0.0f);
	}
	glEnd();
	Circle();
}
void renderScene(void)
{
	static float i = 0;
	i += 0.1;
	if (i > 360)
		i = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
	glColor3f(0, 1, 0);
	glTranslatef(-2, 2.0, -12);
	glRotatef(i, 1, 1, 1);
	Cylinder();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslatef(2, 2, -12);
	glRotatef(i, 1, 1, 1);
	Circle();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslatef(-2, -2, -12);
	glRotatef(i, 1, 1, 1);
	Cube();
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(2, -2, -12);
	glRotatef(i, 1, 1, 1);
	Cone();
	glPopMatrix();
	glutSwapBuffers();
}
void changeSize(int w, int h) {

	// ��ֹ�������߶�Ϊ0
	// (��������ô��ڿ��Ϊ0).
	if (h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// ��λ��ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// �����ӿڴ�СΪ�������ڴ�С
	glViewport(0, 0, w, h);

	// ������ȷ��ͶӰ����
	gluPerspective(45, ratio, 1, 1000);
	//����������ģ����ͼ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);//���ù۲��
}
int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Hello OpenGL");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene); //ָ���������ʱ���ú���
	glutReshapeFunc(changeSize); //ָ��������״�仯ʱ�Ļص�����
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}