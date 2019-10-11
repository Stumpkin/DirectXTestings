#include "Window.h"

using namespace std;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		Window wnd(800, 300, "PLEASE");
		//Window wnd2(500, 500, "THONK");
		MSG msg;

		while (GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		return (int)msg.wParam;
	}

	catch (const Catcher& e)
	{
		MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
	}

	catch (const exception & e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	catch (...)
	{
		MessageBox(nullptr, "No details", "UNKNOWN BLACK EXCEPTION!", MB_OK | MB_ICONEXCLAMATION);
	}

	return -1;
}