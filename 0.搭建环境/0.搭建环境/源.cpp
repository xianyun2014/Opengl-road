#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#pragma comment( lib, "opengl32.lib")	// OpenGL32���ӿ�
#pragma comment( lib, "glu32.lib")		// GLu32���ӿ�

HGLRC hRc = NULL;
HDC   hDC = NULL;
HWND  hWnd = NULL;
HINSTANCE hInstance;

bool keys[256];
bool active = true;
bool fullscreen = true;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLvoid ResizeGLScene(GLsizei width, GLsizei height)  //����opengl���ڴ�С
{
	if (height == 0) //��ֹ��0
		height = 1;
	glViewport(0, 0, width, height); //���õ�ǰ�ӿ�

	glMatrixMode(GL_PROJECTION); //ָ��������������Ӱ��ͶӰ���󣨸þ�����Ϊ�������͸�ӣ�
	glLoadIdentity(); //����ѡ����״̬�ָ���ԭʼ

	//�����ӿڴ�С��45���ӽǣ�0.1f ��100.0f���ڳ��������ܻ�����ȵ������յ�
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW); //�κ��µı任����Ӱ��ģ�͹۲����
	glLoadIdentity();
}

int InitGL(GLvoid)
{
	glShadeModel(GL_SMOOTH); //������Ӱƽ��

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //�����Ļ����ɫ����ɫ

	glClearDepth(1.0f); //������Ȼ���
	glEnable(GL_DEPTH_TEST); //������Ȳ���
	glDepthFunc(GL_LEQUAL); //������Ȳ��Ե�����

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //������õ�͸������

	return true;
}

int DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����Ļ����Ȼ���
	glLoadIdentity();

	return true;
}

GLvoid KillGLWindow(GLvoid) //�������ٴ���
{
	if (fullscreen) //Ϊ�˷�ֹ������������л��ش���ģʽ
	{
		ChangeDisplaySettings(NULL, 0); //�ص�ԭʼ���棬�ڶ�������0��ʾ��ϵͳ�ķֱ��ʡ�ɫ����ȵȻָ�ԭʼ����
		ShowCursor(true); //���ָ��ɼ�
	}
	if (hRc)  //�Ƿ�ӵ��opengl��Ⱦ������
	{
		if (!wglMakeCurrent(NULL, NULL)) //�Ƿ����ͷ�DC��RC������
		{
			MessageBox(NULL, TEXT("���DC��RCʧ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRc))//�Ƿ���ɾ��rc
		{
			MessageBox(NULL, TEXT("�ͷ���Ⱦ������ʧ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		}
		hRc = NULL;
	}
	if (hDC && !ReleaseDC(hWnd, hDC)) //�Ƿ����ͷ�dc
	{
		MessageBox(NULL, TEXT("�ͷ��豸����ʧ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}
	if (hWnd && !DestroyWindow(hWnd)) //�ͷŴ���
	{
		MessageBox(NULL, TEXT("�ͷŴ���ʧ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}
	if (!UnregisterClass(TEXT("OpenGL"), hInstance)) //ע��������
	{
		MessageBox(NULL, TEXT("ע��������ʧ��"), TEXT("��ʾ"), MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}

BOOL CreateGLWindow(TCHAR* Title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint PixelFormat; //�������ƥ��Ľ��

	RECT rect;
	rect.left = 0;
	rect.right = width;
	rect.top = 0;
	rect.bottom = height;

	fullscreen = fullscreenflag;

	WNDCLASS wc;
	DWORD dwExStyle;  //��չ���ڷ��
	DWORD dwStyle;   //���ڷ��

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("OpenGL");
	wc.hInstance = GetModuleHandle(NULL);

	if (!RegisterClass(&wc))
	{ 
		MessageBox(NULL, TEXT("register failed"), TEXT("wrong"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (fullscreen) //����ȫ��ģʽ
	{
		DEVMODE dm; //�豸ģʽ
		memset(&dm, 0, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = width; //��Ļ���
		dm.dmPelsHeight = height; //��Ļ�߶�
		dm.dmBitsPerPel = bits;  //ɫ�����
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL, TEXT("ȫ��ʧ��!\n ʹ�ô���ģʽ��?"), TEXT("����"), MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = false;
			}
			else
			{
				return false;
			}
		}
	}

	if (fullscreen) //ȫ��ģʽ
	{
		dwExStyle = WS_EX_APPWINDOW;
		dwStyle = WS_POPUP;
		ShowCursor(false);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}
	AdjustWindowRectEx(&rect, dwStyle, false, dwExStyle); //�������͵������ڴﵽ����Ҫ��Ĵ�С
	
	if (!(hWnd = CreateWindowEx(dwExStyle, TEXT("OpenGL"), Title,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, GetModuleHandle(NULL), NULL)))
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("���ڴ���ʧ��"), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	static PIXELFORMATDESCRIPTOR pfd =  //pfd���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,									//�汾��
		PFD_DRAW_TO_WINDOW |				//��ʽ֧�ִ���
		PFD_SUPPORT_OPENGL |				//��ʽ����֧��OpenGL
		PFD_DOUBLEBUFFER,					//����֧��˫����
		PFD_TYPE_RGBA,						//���� RGBA��ʽ
		bits,								//ѡ��ɫ�����
		0, 0, 0, 0, 0, 0,					//���Ե�ɫ��λ
		0,									//��alpha����
		0,									//����shift bit
		0,									//���ۼӻ���
		0, 0, 0, 0,							//���Ծۼ�λ
		16,									//16λ Z-���� ����Ȼ��棩
		0,									//���ɰ滺��
		0,									//�޸�������
		PFD_MAIN_PLANE,						//����ͼ��
		0,									//Reserved
		0, 0, 0								//���Բ�����
	};

	if (!(hDC = GetDC(hWnd)))
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("�޷�����豸����"), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))		// Windows �ҵ���Ӧ�����ظ�ʽ����?
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("�����������ظ�ʽ"), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// �ܹ��������ظ�ʽô?
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("�����������ظ�ʽ"), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	if (!(hRc = wglCreateContext(hDC)))					// �ܷ�ȡ����ɫ������?
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, TEXT("���ܴ���OpenGL��Ⱦ������"), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}
	if (!wglMakeCurrent(hDC, hRc))						// ���Լ�����ɫ������
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, TEXT("���ܼ��ǰ��OpenGL��Ȼ������"), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// ��ʾ����
	SetForegroundWindow(hWnd);						// ����������ȼ�
	SetFocus(hWnd);								// ���ü��̵Ľ������˴���
	ResizeGLScene(width, height);						// ����͸�� GL ��Ļ

	if (!InitGL())								// ��ʼ���½���GL����
	{
		KillGLWindow();							// ������ʾ��
		MessageBox(NULL, TEXT("Initialization Failed."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// ���� FALSE
	}

	return true;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)  //��Ϣѭ��
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (!HIWORD(wParam))  //�����С��״̬
			active = true;
		else
			active = false;   //��С��ʱ���ټ���

		return 0;
	case WM_SYSCOMMAND:						// ϵͳ�ж�����
		switch (wParam)						// ���ϵͳ����
		{
		case SC_SCREENSAVE:				// ����Ҫ����?
		case SC_MONITORPOWER:			// ��ʾ��Ҫ����ڵ�ģʽ?
			return 0;					// ��ֹ����
		}

		break;							// �˳�
	case WM_CLOSE:						// �յ�Close��Ϣ
		PostQuitMessage(0);				// �����˳���Ϣ

		return 0;						// ����
	case WM_KEYDOWN:					// �м�����ô?
		keys[wParam] = TRUE;			// ����ǣ���ΪTRUE

		return 0;						// ����
	case WM_KEYUP:						// �м��ſ�ô?
		keys[wParam] = FALSE;			// ����ǣ���ΪFALSE

		return 0;						// ����
	case WM_SIZE:						// ����OpenGL���ڴ�С
		ResizeGLScene(LOWORD(lParam), HIWORD(lParam));		// LoWord=Width,HiWord=Height

		return 0;						// ����
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);// �� DefWindowProc��������δ�������Ϣ��
}

int WINAPI WinMain(HINSTANCE	hInstance,				// ��ǰ����ʵ��
	HINSTANCE	hPrevInstance,				// ǰһ������ʵ��
	LPSTR		lpCmdLine,				// �����в���
	int		nCmdShow)				// ������ʾ״̬
{

	MSG	msg;								// Windowsx��Ϣ�ṹ
	BOOL	done = FALSE;							// �����˳�ѭ����Bool ����

	// ��ʾ�û�ѡ������ģʽ
	if (MessageBox(NULL, TEXT("������ȫ��ģʽ������ô��"), TEXT("����ȫ��ģʽ"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		fullscreen = FALSE;						// FALSEΪ����ģʽ

	// ����OpenGL����
	if (!CreateGLWindow(TEXT("OpenGL������"), 800, 600, 16, fullscreen))
		return 0;							// ʧ���˳�

	while (!done)							// ����ѭ��ֱ�� done=TRUE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))		// ����Ϣ�ڵȴ���?
		{
			if (msg.message == WM_QUIT)			// �յ��˳���Ϣ?
			{
				done = TRUE;					// �ǣ���done=TRUE
			}
			else								// ���ǣ���������Ϣ
			{
				TranslateMessage(&msg);			// ������Ϣ
				DispatchMessage(&msg);			// ������Ϣ
			}
		}
		else								// ���û����Ϣ
		{
			// ���Ƴ���������ESC��������DrawGLScene()���˳���Ϣ
			if (active)						// ���򼤻��ô?
			{
				if (keys[VK_ESCAPE])		// ESC �Ƿ���
				{
					done = TRUE;			// ESC �����˳��ź�
				}
				else						// �����˳���ʱ��ˢ����Ļ
				{
					DrawGLScene();			// ���Ƴ���
					SwapBuffers(hDC);		// �������� (˫����)
				}
			}

			if (keys[VK_F1])				// F1��������ô
			{
				keys[VK_F1] = FALSE;				// ���ǣ�ʹ��Ӧ��Key�����е�ֵΪ FALSE

				KillGLWindow();					// ���ٵ�ǰ�Ĵ���

				fullscreen = !fullscreen;				// �л� ȫ�� / ���� ģʽ
				// �ؽ� OpenGL ����
				if (!CreateGLWindow(TEXT("OpenGL ������"), 800, 600, 16, fullscreen))
					return 0;				// �������δ�ܴ����������˳�
			}
		}
	}

	// �رճ���
	KillGLWindow();								// ���ٴ���
	return (msg.wParam);							// �˳�����
}