#include "stdafx.h"
#include "application.h"
#include "base/memory/singleton.h"
#include "base/message_loop.h"
#include "base/win/wrapped_window_proc.h"

#include "rootnode.h"
#include "windownode.h"

#include "cubenode.h"

#include "xmlconstructer.h"
#include "renderengine.h"
#include "node_factory.h"

const wchar_t* kWindowClass = L"CubeTestWnd";

Application* __instance_;

Application::Application(void)
	: root_node_(new RootNode)
{
	__instance_ = this;
}


Application* Application::GetInstance()
{
	return __instance_;
}


void Application::Run()
{
	base::MessageLoop::current()->Run();
}

void Application::Init(base::MessageLoopForUI* loop)
{
	main_message_loop_ = loop;

	WNDCLASSEX window_class;
	base::win::InitializeWindowClass(
		kWindowClass,
		&base::win::WrappedWindowProc<Application::WndProc>,
		CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW,
		0,
		0,
		LoadCursor(NULL, IDC_ARROW),
		reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)),
		NULL,
		NULL,//class_info.icon,
		NULL,//class_info.icon,
		&window_class);

	ATOM atom = RegisterClassEx(&window_class);

	NodeClassFactory::GetInstance()->Init();
}

LRESULT CALLBACK Application::WndProc( HWND window, UINT message, WPARAM w_param, LPARAM l_param )
{
	if (message == WM_NCCREATE) {
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(l_param);
		Application* app = reinterpret_cast<Application*>(cs->lpCreateParams);
		SetWindowLongPtr(window, GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(app));

		
		return TRUE;
	}

	Application *app = reinterpret_cast<Application*>(GetWindowLongPtr(window, GWLP_USERDATA));

	LRESULT result = 0;
	if (app){
		return app->HandleMessage(window, message, w_param, l_param);
	}

	return DefWindowProc(window, message, w_param, l_param);
}

LRESULT Application::HandleMessage(HWND hwnd, UINT message, WPARAM w_param, LPARAM l_param )
{
	return root_node_->HandleMessage(hwnd, message, w_param, l_param);
}

HWND Application::CreateHWND()
{
	HWND hwnd = (CreateWindowEx(WS_EX_WINDOWEDGE, kWindowClass, NULL,
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPED,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, NULL, (LPVOID)GetInstance()));

	DWORD dwStyle = ::GetWindowLong(hwnd, GWL_STYLE);
	::SetWindowLong(hwnd, GWL_STYLE, (dwStyle & ~WS_CAPTION));

	return hwnd;
}

void Application::LoadFromFile( const std::wstring& file )
{
	//���������ļ�
	XmlConstructer constructer(root_node_.get());
	constructer.Load(file);
	
	//�����л�������Ϣ
	root_node_->CheckAndApplyProperties();

	root_node_->ShowMainWindow();

}


