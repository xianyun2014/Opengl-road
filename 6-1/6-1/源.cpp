#include <gl/glut.h>
#include <gl/glaux.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glaux.lib")

#include <stdio.h>
float s = 1;
void renderScene(void)
{
	static float i = 0;
	i += 0.01f;
	if (i > 360) i = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glTranslatef(0, 0, -10);
	glRotatef(i, 1, 1, 1);glScalef(s, s, s);
	auxWireTeapot(2);
	glutSwapBuffers();
}

void changeSize(int w, int h)
{
	if (h == 0) h = 1;
	float ratio = 1.0* w / h;
	glMatrixMode(GL_PROJECTION);// 单位化投影矩阵。
	glLoadIdentity();
	
	glViewport(0, 0, w, h);// 设置视口大小为整个窗口大小
	gluPerspective(45, ratio, 1, 1000);// 设置正确的投影矩阵
	glMatrixMode(GL_MODELVIEW);//下面是设置模型视图矩阵
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);//设置观测点
}
void keyfunc(unsigned char ch, int x, int y)
{
	if (ch == 'w')
		s += 0.1;
	else if (ch == 's')
		s -= 0.1;
	printf("%f\n", s);
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
	glutKeyboardFunc(keyfunc);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}