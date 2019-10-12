#include "Window.h"
#include "resource.h" //Just for the icon
#include <sstream>

using namespace std;

//Window class definition
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)) //get the handle of the instance as a static member or else this will have to pass it through a parameter
{
	//The rest in here remains the same for registering the window. Go to "Things" on main.cpp for explanmation
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handleMSGSetup; //Used for later *NEW*
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = getInstance(); //*NEW*
	wc.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 64, 64, 0)); // If you don't have an icon image just set it to nullptr
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = getName(); //*NEW*
	wc.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, 0)); // Line 17 (wc.hIcon)
	
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, getInstance());
}

const char* Window::WindowClass::getName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::getInstance() noexcept
{
	return wndClass.hInst;
}

//Window definition

Window::Window(int width, int height, const char* name) 
{
	//calculate window size

	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
	// throw runtime_error("LuL > LUL"); Throwing errors

	//create window and window handle
	hWnd = CreateWindowEx(0, WindowClass::getName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 400, 500, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, WindowClass::getInstance(), this);

	// if window failed to create for some reason...
	if (hWnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}

	//show window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Window::~Window() //Deconstutor
{
	DestroyWindow(hWnd);
}

void Window::setTitle(const string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		throw CHWND_LAST_EXCEPT();
	}
}

LRESULT WINAPI Window::handleMSGSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//Use create parameter passed in from CreateWindow() to store window class pointer at WINAPI 
	if (msg == WM_NCCREATE)
	{
		//extract pointer to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);

		//set WINAPI-managed user data to store pointer to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));

		//set message proc to normal (not setup) handler
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handleMSGThunk));
		
		//forward message to window class handleer
		return pWnd->handleMSG(hWnd, msg, wParam, lParam);
	}
	
	//if we get a message before "WM_NCCREATE" message, it will go to the default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::handleMSGThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//retrieve pointer to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

	//forward message to window class handler
	return pWnd->handleMSG(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMSG(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	/********************Mouse Messages***************************/
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}

		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}

			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		
	}

	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}

	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}

	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}

	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
	}
	/********************End Mouse Messages************************/

	/********************Keyboard Messages*************************/
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN: // keys like Alt and fuctions buttons "F2"
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;

	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;

	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	/********************End Keyboard Messages********************/
	case WM_CLOSE:
		PostQuitMessage(17);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Windows Catcher / Exception definition

Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	Catcher(line, file),
	hr(hr)
{}


const char* Window::Exception::getType() const noexcept 
{
	return "Some Window Exception";
}

HRESULT Window::Exception::getErrorCode() const noexcept
{
	return hr;
}

string Window::Exception::getErrorString() const noexcept
{
	return translateErrorCode(hr);
}

string Window::Exception::translateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr;

	// FormatMessage gives you a descrition string for that error code from HRESULT
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr); 
	//some jankness on the 3rd last parameter bacause it supposed to take a pointer to a string. BUT I used ALLOCATE_BUFFER, which needs a pointer to a pointer

	if (nMsgLen == 0)
	{
		return "Unidentified error code :I";
	}
	string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

const char* Window::Exception::what() const noexcept
{
	ostringstream oss;
	oss << getType() << endl;
	oss << "[Error Code] " << getErrorCode() << endl;
	oss << "[Description] " << getErrorString() << endl;
	oss << getOriginString() << endl;

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
