#include <gl/glut.h>
#include <gl/glaux.h>
#include <stdio.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glaux.lib")

UINT g_bmp[1];//贴图编号

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //环境光参数，半亮白色
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //漫射光参数，
GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //光源位置

void Box(float x, float y, float z) //长方体
{
	glPushMatrix();
	glScalef(x, y, z);
	glEnable(GL_TEXTURE_2D); //贴图有效
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);// 前
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// 后
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);// 上
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// 下
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);// 左
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// 右
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);//取消贴图
	glPopMatrix();
}
bool LoadTexture(char *filename, GLuint &texture)
{
	AUX_RGBImageRec *pImage = auxDIBImageLoadA(filename); //装入位图
	if (pImage == NULL) return false;
	glGenTextures(1, &texture); //生成贴图
	glBindTexture(GL_TEXTURE_2D, g_bmp[0]); //贴图生效
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data); //贴图数据
	return true;
}
void init()
{
	LoadTexture("i:\\4.bmp", g_bmp[0]);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); //设置环境光
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); //设置漫射光
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); //设置光源位置
	glEnable(GL_LIGHT1); //启用一号光源
	glEnable(GL_LIGHTING); //开光
}

float s = 1, r = 0; //放大缩小的倍数和旋转参数
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();									// Reset The View
	glTranslatef(0.0f, 0.0f, -5);
	glRotatef(r, 1.0f, 1.0f, 0.0f);
	glScalef(s, s, s);

	Box(1, 1, 1);

	r += 0.01;//旋转
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
int light = 1, bp = 0;
void keyfunc(unsigned char ch, int x, int y)
{
	if (ch == 'w') //发大缩小
		s += 0.1;
	else if (ch == 's')
		s -= 0.1;
	if (ch == 'd')//d键开关灯
	{
		if (light)
		{
			light = 0;
			glDisable(GL_LIGHTING);
			printf("off\n");
		}
		else
		{
			light = 1;
			glEnable(GL_LIGHTING);
			printf("on\n");
		}
	}
}
int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 320);
	glutCreateWindow("Hello OpenGL");
	init();
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene); //指定程序空闲时调用函数
	glutReshapeFunc(changeSize); //指定窗口形状变化时的回调函数
	glEnable(GL_DEPTH_TEST);
	glutKeyboardFunc(keyfunc);//键盘回调函数
	glutMainLoop();
	return 0;
}