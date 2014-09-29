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
	glBegin(GL_POINTS);//多组点
	glVertex3f(-1.0, 0.0, 0.0);
	glVertex3f(-1.2, 0.0, 0.0);
	glEnd();
	glBegin(GL_LINES);//多组线段
	glVertex3f(-0.9, 0.9, 0.0);
	glVertex3f(-0.7, 1.9, 0.0);
	glVertex3f(-0.8, 0.7, 0.0);
	glVertex3f(-0.8, 0.2, 0.0);
	glEnd();
	glBegin(GL_LINE_STRIP);//不闭合折线
	glVertex3f(-1, 1.4, 0.0);
	glVertex3f(-0.5, 0.1, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();
	glBegin(GL_TRIANGLES);//多组填充三角形
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.5, 0.0, 0.0);
	glVertex3f(0.0, 0.5, 0.0);
	glEnd();
	glBegin(GL_QUADS);//多组填充四边形
	glVertex3f(1.0, 1.0, 0.0);
	glVertex3f(2.0, 1.0, 0.0);
	glVertex3f(2.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glEnd();
	glutSwapBuffers();
}
void changeSize(int w, int h) {

	// 防止除数即高度为0
	// (你可以设置窗口宽度为0).
	if (h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// 单位化投影矩阵。
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 设置视口大小为整个窗口大小
	glViewport(0, 0, w, h);

	// 设置正确的投影矩阵
	gluPerspective(45, ratio, 1, 1000);
	//下面是设置模型视图矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);//设置观测点
}
int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Hello OpenGL");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene); //指定程序空闲时调用函数
	glutReshapeFunc(changeSize); //指定窗口形状变化时的回调函数
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}