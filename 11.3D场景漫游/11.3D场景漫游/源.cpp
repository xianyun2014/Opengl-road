#include <gl/glut.h>
#include <gl/glaux.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glaux.lib")

#include <stdio.h>
#include <math.h>

UINT g_bmp[1];//贴图编号

const float MAP = 50; //地图边长一半长度
const int BOX_NUM = 110;//随机箱子的个数
float angle = 90.0f, g_eye[3] = { 0.0f, 1.8f, -10.0f }, g_look[3] = { 0.0f, 1.5f, -9.0f }; //初始视点信息

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

void Draw_Ground()
{
	glBegin(GL_LINES);
	for (int i = -MAP; i <= MAP; i += 5)
	{
		glVertex3i(i, 0, -MAP);
		glVertex3i(i, 0, MAP);
	}
	for (int k = -MAP; k <= MAP; k += 5)
	{
		glVertex3i(-MAP, 0, k);
		glVertex3i(MAP, 0, k);
	}
	glEnd();
}

float randf()
{
	return -MAP + rand() % (int)(2 * MAP);
}
struct
{
	float x, y, z;
	float len;
}BOX[BOX_NUM];
void Init_Rdm_Box()
{
	for (int i = 0; i < BOX_NUM; ++i)
	{
		BOX[i].len = rand() % 3 + 1;
		BOX[i].x = randf();
		BOX[i].z = randf();
		BOX[i].y = BOX[i].len; //确保在网格之上
	}
}
void Draw_Rdm_Box()
{
	float x = 0, y = 0, z = 0;
	glPushMatrix();
	for (int i = 0; i < BOX_NUM; ++i)
	{
		glLoadIdentity();
		gluLookAt(g_eye[0], g_eye[1], g_eye[2], g_look[0], g_look[1], g_look[2], 0, 1, 0);
		glTranslatef(BOX[i].x, BOX[i].y, BOX[i].z);
		Box(BOX[i].len, BOX[i].len, BOX[i].len);
	}
	glPopMatrix();
}
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();	
	gluLookAt(g_eye[0], g_eye[1], g_eye[2], g_look[0], g_look[1], g_look[2], 0, 1, 0);
	glPushMatrix();
	Draw_Rdm_Box(); //绘制方块
	Draw_Ground(); //绘制地面
	
	glPopMatrix();
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
}

void keyfunc(unsigned char ch, int x, int y)
{
	float speed = 0.8f;
	if (ch == 'd')
	{
		angle += speed;
	}
	if (ch == 'a')
	{
		angle -= speed;
	}
	float rad_xz = float(3.14159 * angle / 180.0);
	if (ch == 'w')
	{
		g_eye[2] += sin(rad_xz) * speed;
		g_eye[0] += cos(rad_xz) * speed;
	}
	if (ch == 's')
	{
		g_eye[2] -= sin(rad_xz) * speed;
		g_eye[0] -= cos(rad_xz) * speed;
	}
	if (ch == 'q')
	{
		g_look[1] -= 0.1f;
	}
	if (ch == 'e')
	{
		g_look[1] += 0.1f;
	}
	g_look[0] = g_eye[0] + cos(rad_xz);
	g_look[2] = g_eye[2] + sin(rad_xz);
	printf("(%f,%f,%f),(%f,%f,%f)\n", g_eye[0], g_eye[1], g_eye[2], g_look[0], g_look[1], g_look[2]);
}
void init()
{
	LoadTexture("i:\\4.bmp", g_bmp[0]);
	glEnable(GL_SMOOTH);
	Init_Rdm_Box();
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