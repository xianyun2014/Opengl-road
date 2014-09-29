#include <gl/glut.h>
#include <gl/glaux.h>
#include <stdio.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glaux.lib")

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
	glBindTexture(GL_TEXTURE_2D, texture); //贴图生效
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data); //贴图数据
	return true;
}

float s = 90, r = 0;
bool flash, key_r;
const int num = 30;
typedef struct
{
	int r, g, b;
	GLfloat dist;
	GLfloat angle;
}Stars;
Stars star[num];
GLfloat spin;  //星星自转

GLuint loop;
UINT g_bmp[1];//贴图编号
void init()
{
	LoadTexture("i:\\5.bmp", g_bmp[0]);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//真正精细的透视修正
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);
	for (loop = 0; loop < num; ++loop)
	{
		star[loop].angle = 0.0f;
		star[loop].dist = 1.0f * loop / num * 4.0f;
		star[loop].r = rand() % 256;
		star[loop].g = rand() % 256;
		star[loop].b = rand() % 256;
	}
}


void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, g_bmp[0]);
	glEnable(GL_TEXTURE_2D);

	for (loop = 0; loop < num; ++loop)
	{
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, -15);
		glRotatef(s, 1.0f, 0.0f, 0.0f);
		glRotatef(star[loop].angle, 0.0f, 1.0f, 0.0f);
		glTranslatef(star[loop].dist, 0.0f, 0.0f);
		glRotatef(-star[loop].angle, 0.0f, 1.0f, 0.0f);
		glRotatef(-s, 1.0f, 0.0f, 0.0f);
		if (flash)
		{
			glColor4ub(star[num - loop - 1].r, star[num - loop - 1].g, star[num - loop - 1].b, 255);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
		}
		glRotatef(spin, 0.0f, 0.0f, 1.0f); //绕z公转
		glColor4ub(star[loop].r, star[loop].g, star[loop].b, 255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
		spin += 0.01f; //星星公转
		star[loop].angle += 1.0f * loop / num; //改变自转角度
		star[loop].dist -= 0.01f; //改变星星离中心的距离

		if (star[loop].dist < 0.0f) //到中心了
		{
			star[loop].dist += 4.0f;
			star[loop].r = rand() % 256;
			star[loop].g = rand() % 256;
			star[loop].b = rand() % 256;
		}
	}
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
}

void keyfunc(unsigned char ch, int x, int y)
{
	if (ch == 'w') //发大缩小
		s += 3;
	else if (ch == 's')
		s -= 3;

	if (ch == 'r')
	{
		key_r = 1;
		flash = !flash;
	}
	else
		key_r = 0;
}
int main(int argc, char * argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Hello OpenGL");
	init();
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene); //指定程序空闲时调用函数
	glutReshapeFunc(changeSize); //指定窗口形状变化时的回调函数
	glutKeyboardFunc(keyfunc);//键盘回调函数
	glutMainLoop();
	return 0;
}