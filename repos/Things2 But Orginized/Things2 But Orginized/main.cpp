#include "App.h"

using namespace std;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		App pog;
		return pog.Start();
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