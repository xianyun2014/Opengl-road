#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#pragma comment( lib, "opengl32.lib")	// OpenGL32连接库
#pragma comment( lib, "glu32.lib")		// GLu32连接库

HGLRC hRc = NULL;
HDC   hDC = NULL;
HWND  hWnd = NULL;
HINSTANCE hInstance;

bool keys[256];
bool active = true;
bool fullscreen = true;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLvoid ResizeGLScene(GLsizei width, GLsizei height)  //重置opengl窗口大小
{
	if (height == 0) //防止除0
		height = 1;
	glViewport(0, 0, width, height); //重置当前视口

	glMatrixMode(GL_PROJECTION); //指明接下来的两行影响投影矩阵（该矩阵负责为场景添加透视）
	glLoadIdentity(); //将所选矩阵状态恢复成原始

	//设置视口大小，45度视角，0.1f ，100.0f是在场景中所能绘制深度的起点和终点
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW); //任何新的变换将会影响模型观察矩阵
	glLoadIdentity();
}

int InitGL(GLvoid)
{
	glShadeModel(GL_SMOOTH); //启动阴影平滑

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //清除屏幕的颜色，黑色

	glClearDepth(1.0f); //设置深度缓存
	glEnable(GL_DEPTH_TEST); //启用深度测试
	glDepthFunc(GL_LEQUAL); //所作深度测试的类型

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //进行最好的透视修正

	return true;
}

int DrawGLScene(GLvoid)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除屏幕和深度缓存
	glLoadIdentity();

	return true;
}

GLvoid KillGLWindow(GLvoid) //正常销毁窗口
{
	if (fullscreen) //为了防止桌面崩溃，先切换回窗口模式
	{
		ChangeDisplaySettings(NULL, 0); //回到原始桌面，第二个参数0表示用系统的分辨率、色彩深度等恢复原始窗口
		ShowCursor(true); //鼠标指针可见
	}
	if (hRc)  //是否拥有opengl渲染描述表
	{
		if (!wglMakeCurrent(NULL, NULL)) //是否能释放DC和RC描述表
		{
			MessageBox(NULL, TEXT("提放DC或RC失败"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRc))//是否能删除rc
		{
			MessageBox(NULL, TEXT("释放渲染描述表失败"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		}
		hRc = NULL;
	}
	if (hDC && !ReleaseDC(hWnd, hDC)) //是否能释放dc
	{
		MessageBox(NULL, TEXT("释放设备环境失败"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}
	if (hWnd && !DestroyWindow(hWnd)) //释放窗口
	{
		MessageBox(NULL, TEXT("释放窗口失败"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}
	if (!UnregisterClass(TEXT("OpenGL"), hInstance)) //注销窗口类
	{
		MessageBox(NULL, TEXT("注销窗口类失败"), TEXT("提示"), MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}

BOOL CreateGLWindow(TCHAR* Title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint PixelFormat; //保存查找匹配的结果

	RECT rect;
	rect.left = 0;
	rect.right = width;
	rect.top = 0;
	rect.bottom = height;

	fullscreen = fullscreenflag;

	WNDCLASS wc;
	DWORD dwExStyle;  //扩展窗口风格
	DWORD dwStyle;   //窗口风格

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

	if (fullscreen) //设置全屏模式
	{
		DEVMODE dm; //设备模式
		memset(&dm, 0, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmPelsWidth = width; //屏幕宽度
		dm.dmPelsHeight = height; //屏幕高度
		dm.dmBitsPerPel = bits;  //色彩深度
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			if (MessageBox(NULL, TEXT("全屏失败!\n 使用窗口模式吗?"), TEXT("错误"), MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = false;
			}
			else
			{
				return false;
			}
		}
	}

	if (fullscreen) //全屏模式
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
	AdjustWindowRectEx(&rect, dwStyle, false, dwExStyle); //根据类型调整窗口达到真正要求的大小
	
	if (!(hWnd = CreateWindowEx(dwExStyle, TEXT("OpenGL"), Title,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		NULL, NULL, GetModuleHandle(NULL), NULL)))
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("窗口创建失败"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	static PIXELFORMATDESCRIPTOR pfd =  //pfd告诉窗口我们所希望的东东，即窗口使用的像素格式
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,									//版本号
		PFD_DRAW_TO_WINDOW |				//格式支持窗口
		PFD_SUPPORT_OPENGL |				//格式必须支持OpenGL
		PFD_DOUBLEBUFFER,					//必须支持双缓冲
		PFD_TYPE_RGBA,						//申请 RGBA格式
		bits,								//选定色彩深度
		0, 0, 0, 0, 0, 0,					//忽略的色彩位
		0,									//无alpha缓存
		0,									//忽略shift bit
		0,									//无累加缓存
		0, 0, 0, 0,							//忽略聚集位
		16,									//16位 Z-缓存 （深度缓存）
		0,									//无蒙版缓存
		0,									//无辅助缓存
		PFD_MAIN_PLANE,						//主绘图层
		0,									//Reserved
		0, 0, 0								//忽略层遮罩
	};

	if (!(hDC = GetDC(hWnd)))
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("无法获得设备环境"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))		// Windows 找到相应的象素格式了吗?
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("不能设置像素格式"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// 能够设置象素格式么?
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("不能设置像素格式"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
	if (!(hRc = wglCreateContext(hDC)))					// 能否取得着色描述表?
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL, TEXT("不能创建OpenGL渲染描述表"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}
	if (!wglMakeCurrent(hDC, hRc))						// 尝试激活着色描述表
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL, TEXT("不能激活当前的OpenGL渲然描述表"), TEXT("错误"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// 显示窗口
	SetForegroundWindow(hWnd);						// 略略提高优先级
	SetFocus(hWnd);								// 设置键盘的焦点至此窗口
	ResizeGLScene(width, height);						// 设置透视 GL 屏幕

	if (!InitGL())								// 初始化新建的GL窗口
	{
		KillGLWindow();							// 重置显示区
		MessageBox(NULL, TEXT("Initialization Failed."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		return FALSE;							// 返回 FALSE
	}

	return true;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)  //消息循环
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		if (!HIWORD(wParam))  //检测最小化状态
			active = true;
		else
			active = false;   //最小化时不再激活

		return 0;
	case WM_SYSCOMMAND:						// 系统中断命令
		switch (wParam)						// 检查系统调用
		{
		case SC_SCREENSAVE:				// 屏保要运行?
		case SC_MONITORPOWER:			// 显示器要进入节电模式?
			return 0;					// 阻止发生
		}

		break;							// 退出
	case WM_CLOSE:						// 收到Close消息
		PostQuitMessage(0);				// 发出退出消息

		return 0;						// 返回
	case WM_KEYDOWN:					// 有键按下么?
		keys[wParam] = TRUE;			// 如果是，设为TRUE

		return 0;						// 返回
	case WM_KEYUP:						// 有键放开么?
		keys[wParam] = FALSE;			// 如果是，设为FALSE

		return 0;						// 返回
	case WM_SIZE:						// 调整OpenGL窗口大小
		ResizeGLScene(LOWORD(lParam), HIWORD(lParam));		// LoWord=Width,HiWord=Height

		return 0;						// 返回
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);// 向 DefWindowProc传递所有未处理的消息。
}

int WINAPI WinMain(HINSTANCE	hInstance,				// 当前窗口实例
	HINSTANCE	hPrevInstance,				// 前一个窗口实例
	LPSTR		lpCmdLine,				// 命令行参数
	int		nCmdShow)				// 窗口显示状态
{

	MSG	msg;								// Windowsx消息结构
	BOOL	done = FALSE;							// 用来退出循环的Bool 变量

	// 提示用户选择运行模式
	if (MessageBox(NULL, TEXT("你想在全屏模式下运行么？"), TEXT("设置全屏模式"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		fullscreen = FALSE;						// FALSE为窗口模式

	// 创建OpenGL窗口
	if (!CreateGLWindow(TEXT("OpenGL程序框架"), 800, 600, 16, fullscreen))
		return 0;							// 失败退出

	while (!done)							// 保持循环直到 done=TRUE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))		// 有消息在等待吗?
		{
			if (msg.message == WM_QUIT)			// 收到退出消息?
			{
				done = TRUE;					// 是，则done=TRUE
			}
			else								// 不是，处理窗口消息
			{
				TranslateMessage(&msg);			// 翻译消息
				DispatchMessage(&msg);			// 发送消息
			}
		}
		else								// 如果没有消息
		{
			// 绘制场景。监视ESC键和来自DrawGLScene()的退出消息
			if (active)						// 程序激活的么?
			{
				if (keys[VK_ESCAPE])		// ESC 是否按下
				{
					done = TRUE;			// ESC 发出退出信号
				}
				else						// 不是退出的时候，刷新屏幕
				{
					DrawGLScene();			// 绘制场景
					SwapBuffers(hDC);		// 交换缓存 (双缓存)
				}
			}

			if (keys[VK_F1])				// F1键按下了么
			{
				keys[VK_F1] = FALSE;				// 若是，使对应的Key数组中的值为 FALSE

				KillGLWindow();					// 销毁当前的窗口

				fullscreen = !fullscreen;				// 切换 全屏 / 窗口 模式
				// 重建 OpenGL 窗口
				if (!CreateGLWindow(TEXT("OpenGL 程序框架"), 800, 600, 16, fullscreen))
					return 0;				// 如果窗口未能创建，程序退出
			}
		}
	}

	// 关闭程序
	KillGLWindow();								// 销毁窗口
	return (msg.wParam);							// 退出程序
}