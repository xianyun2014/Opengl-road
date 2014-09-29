#include <gl/glut.h>
#include <gl/glaux.h>
#include <stdio.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glaux.lib")

void Box(float x, float y, float z) //������
{
	glPushMatrix();
	glScalef(x, y, z);
	glEnable(GL_TEXTURE_2D); //��ͼ��Ч
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);// ǰ
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// ��
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);// ��
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// ��
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);// ��
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// ��
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);//ȡ����ͼ
	glPopMatrix();
}
bool LoadTexture(char *filename, GLuint &texture)
{
	AUX_RGBImageRec *pImage = auxDIBImageLoadA(filename); //װ��λͼ
	if (pImage == NULL) return false;
	glGenTextures(1, &texture); //������ͼ
	glBindTexture(GL_TEXTURE_2D, texture); //��ͼ��Ч
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data); //��ͼ����
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
GLfloat spin;  //������ת

GLuint loop;
UINT g_bmp[1];//��ͼ���
void init()
{
	LoadTexture("i:\\5.bmp", g_bmp[0]);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//������ϸ��͸������
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
		glRotatef(spin, 0.0f, 0.0f, 1.0f); //��z��ת
		glColor4ub(star[loop].r, star[loop].g, star[loop].b, 255);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();
		spin += 0.01f; //���ǹ�ת
		star[loop].angle += 1.0f * loop / num; //�ı���ת�Ƕ�
		star[loop].dist -= 0.01f; //�ı����������ĵľ���

		if (star[loop].dist < 0.0f) //��������
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
	glMatrixMode(GL_PROJECTION);// ��λ��ͶӰ����
	glLoadIdentity();
	glViewport(0, 0, w, h);// �����ӿڴ�СΪ�������ڴ�С
	gluPerspective(45, ratio, 1, 1000);// ������ȷ��ͶӰ����
	glMatrixMode(GL_MODELVIEW);//����������ģ����ͼ����
}

void keyfunc(unsigned char ch, int x, int y)
{
	if (ch == 'w') //������С
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
	glutIdleFunc(renderScene); //ָ���������ʱ���ú���
	glutReshapeFunc(changeSize); //ָ��������״�仯ʱ�Ļص�����
	glutKeyboardFunc(keyfunc);//���̻ص�����
	glutMainLoop();
	return 0;
}