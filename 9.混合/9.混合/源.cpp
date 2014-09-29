#include <gl/glut.h>
#include <gl/glaux.h>
#include <stdio.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut.lib")
#pragma comment(lib, "glaux.lib")

UINT g_bmp[1];//��ͼ���

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //�����������������ɫ
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //����������
GLfloat LightPosition[] = { 1.5f, 0.0f, -2.0f, 1.0f }; //��Դλ��

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
	AUX_RGBImageRec *pImage = NULL;
	pImage = auxDIBImageLoadA(filename); //װ��λͼ
	if (pImage == NULL)
		return false;
	glGenTextures(1, &texture); //������ͼ
	glBindTexture(GL_TEXTURE_2D, g_bmp[0]); //��ͼ��Ч
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, pImage->sizeX, pImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, pImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //��Сʱ�����˲�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); //����ʱ�����˲�
	gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->sizeX, pImage->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pImage->data); //��ͼ����
	free(pImage->data);//�ͷ�λͼ�ڴ�
	free(pImage);
	return true;
}
void init()
{
	//g_text = gluNewQuadric(); //������ͼ����
	LoadTexture("i:\\4.bmp", g_bmp[0]);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient); //���û�����
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse); //���������
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition); //���ù�Դλ��
	glEnable(GL_LIGHT1); //����һ�Ź�Դ
	glEnable(GL_LIGHTING); //����
	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);//��ɫ0.5 alphaֵ
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);//��Ϻ���
	glEnable(GL_BLEND);//����͸����ע�ⲻҪ������Ȳ���,����Ҫ��glEnable(GL_DEPTH_TEST)
}

float s = 1, r = 0;
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5);
	glRotatef(r, 1.0f, 1.0f, 0.0f);
	glScalef(s, s, s);

	Box(1, 1, 1);

	r += 0.01f;
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
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0f, 1.0f, 0.0f);//���ù۲��
}
int light = 1;
void keyfunc(unsigned char ch, int x, int y)
{
	if (ch == 'w') //������С
		s += 0.1f;
	else if (ch == 's')
		s -= 0.1f;
	if (ch == 'd')//d�����ص�
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
	glutIdleFunc(renderScene); //ָ���������ʱ���ú���
	glutReshapeFunc(changeSize); //ָ��������״�仯ʱ�Ļص�����
	glutKeyboardFunc(keyfunc);//���̻ص�����
	glutMainLoop();
	return 0;
}