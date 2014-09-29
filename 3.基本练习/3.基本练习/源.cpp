#include <gl/glut.h>
#include <gl/GLU.h>
#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut.lib")

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBegin(GL_POINTS);//�����
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(-1.2, 0.0, 0.0);
	glEnd();
	glBegin(GL_LINES);//�����߶�
	glVertex3f(-0.9, 0.9, 0.0);
	glVertex3f(-0.7, 1.9, 0.0);
	glVertex3f(-0.8, 0.7, 0.0);
	glVertex3f(-0.8, 0.2, 0.0);
	glEnd();
	glBegin(GL_LINE_STRIP);//���պ�����
	glVertex3f(-1, 1.4, 0.0);
	glVertex3f(-0.5, 0.1, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
	glBegin(GL_TRIANGLES);//�������������
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glEnd();
	glBegin(GL_QUADS);//��������ı���
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(2.0, 1.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glEnd();
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