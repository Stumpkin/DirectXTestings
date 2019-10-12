//used the video source ChiliTamatoNoodle "C++ 3D DirectX Tutorial [Window Creation] 2", "C++ 3D DirectX Tutorial [Message Loop / WndProc] 3"
//"C++ 3D DirectX Tutorial [Windows Messages] 4", and "C++ 3D DirectX Tutorial [WM_CHAR / Mouse] 5"

#include <Windows.h> //NEEDED for now at least. Does not use C++ functions
#include <string> // Used exclusivly in WndProc for WM_CHAR
#include <sstream> // Used exclusivly in WndProc for WM_CHAR

using namespace std;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // used for later

//the main "FUNCTION" in the window program

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//More information go and look up on what the parameters do. Go and look it up in the API or watch the videos

	/*
	notes
	This function haven't been updated ever since Windows 3.0
	HINSTANCE are pointers that hold information about the program.
	So in the first two HINSTANCE parameters set them to NULL (always the second one to NULL)
	LPSTR lpCmdLine is the command line as a single string
	nCmdShow indicator of how the window is showen when the program is started
	CALLBACk C++ use a different call convention stdcall.
	*/

	//Register window class

	WNDCLASSEX wc; // look this up
	const auto dClassName = "SomethingClass"; //to fill the class name you need a null-terminating string or an atom (use VS example) 
	wc.cbSize = sizeof(WNDCLASSEX); //The video does an array of WNDCLASSEX but I will use the VS example
	wc.style = CS_OWNDC; // the style of window for now CS_OWNDC "Allocates a unique device context for each window in the class."
	wc.lpfnWndProc = WndProc; // Ok the video uses DefWindowProc but that doesn't end the message loop in this instance So I call it from the window protocal function 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = dClassName;
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);

	//Creating window instance
	HWND hWnd = CreateWindowEx(0, dClassName, "WINDOW NAME", WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX, 500, 500, 640, 480, nullptr, nullptr, hInstance, nullptr);

	//the purple text represent what will be in the window
	//look up the format of this function
	//HWND = Window Handle
	//hWnd is the handle for the window for future operations for the window
	//why EX the is more that you can do with this type of window compared to the normal window

	//SHOW US THE GOD DAMN WINDOW!
	ShowWindow(hWnd, SW_SHOW); // shows the window but its frozen without the message loop;
	UpdateWindow(hWnd); // This is not nessary for now I just used this from the VS example

	//message loop
	//Remember All windows based programs revolves around the windows themselves and messages.
	MSG msg; //message
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}
	return (int) msg.wParam;

	//Another outside source that I used for the loop is I used the loop that the teacher gave us
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//This is your window protocal, basically what goes down in the window
	static string textTitle;
	switch (msg)
	{
	case WM_KEYDOWN: //whenever any key is pressed down
		if (wParam == 'W')
		{
			SetWindowText(hWnd, "UWU");
		}

		if (wParam == 'O')
		{
			SetWindowText(hWnd, "OWO");
		}
		break;

	case WM_KEYUP: //whenever any key is released
		if (wParam == 'Q')
		{
			SetWindowText(hWnd, "WINDOW NAME");
			textTitle = "";
		}

		else if (wParam == 'O')
		{
			SetWindowText(hWnd, "OWO");
			textTitle = "";
		}

		else if (wParam == 'U')
		{
			SetWindowText(hWnd, "UWU");
			textTitle = "";
		}
		break;

	case WM_CHAR: //whenever a character key is pressed through the TranslateMessage in the message loop
		{
		
		textTitle.push_back((char)wParam);
		SetWindowText(hWnd, textTitle.c_str());
		}
		break;

	case WM_LBUTTONDOWN: //whenever the LMB is pressed down in the window
		{
			POINTS pt = MAKEPOINTS(lParam);
			ostringstream stringoThing;
			stringoThing << "(" << pt.x << ", " << pt.y << ")";
			SetWindowText(hWnd, stringoThing.str().c_str());
			textTitle = "";
		}
		break;

	case WM_CLOSE: //when the window is closed. Linked to WM_DESTROY
		DestroyWindow(hWnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(17);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0; //ultimatly posts a message to wParam whenever the window is closed
}