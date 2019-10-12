#include "App.h"

App::App()
	:
	wnd(500, 500, "OK HERE WE GO!") // window creation
{}

int App::Start() // message loop
{
	while (true)
	{
		if (const auto something = Window::ProcessMessage())
		{
			return *something;
		}
		doFrame();
	}
}

void App::doFrame()
{
	//things
}