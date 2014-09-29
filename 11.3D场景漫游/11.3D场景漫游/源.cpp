#include <gl/glut.h>
#include <gl/glaux.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glaux.lib")

#include <stdio.h>
#include <math.h>

UINT g_bmp[1];//��ͼ���

const float MAP = 50; //��ͼ�߳�һ�볤��
const int BOX_NUM = 110;//������ӵĸ���
float angle = 90.0f, g_eye[3] = { 0.0f, 1.8f, -10.0f }, g_look[3] = { 0.0f, 1.5f, -9.0f }; //��ʼ�ӵ���Ϣ

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
	glBindTexture(GL_TEXTURE_2D, g_bmp[0]); //��ͼ��Ч
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data); //��ͼ����
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
		BOX[i].y = BOX[i].len; //ȷ��������֮��
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
	Draw_Rdm_Box(); //���Ʒ���
	Draw_Ground(); //���Ƶ���
	
	glPopMatrix();
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
	glutIdleFunc(renderScene); //ָ���������ʱ���ú���
	glutReshapeFunc(changeSize); //ָ��������״�仯ʱ�Ļص�����
	glEnable(GL_DEPTH_TEST);
	glutKeyboardFunc(keyfunc);//���̻ص�����
	glutMainLoop();
	return 0;
}