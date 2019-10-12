#pragma once
#include "Window.h"
class App
{
public:
	App();
	int Start();

private:
	void doFrame();

private:
	Window wnd;
};