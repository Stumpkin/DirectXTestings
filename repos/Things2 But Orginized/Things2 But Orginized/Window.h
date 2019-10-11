#pragma once
#include "OhWindows.h"
#include "Catcher.h"

using namespace std;

class Window
{
public:
	class Exception : public Catcher
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* getType() const noexcept override;
		static string translateErrorCode(HRESULT hr) noexcept;
		HRESULT getErrorCode() const noexcept;
		string getErrorString() const noexcept;

	private:
		HRESULT hr;
	};

private:
	class WindowClass
	{
	public:
		static const char* getName() noexcept;
		static HINSTANCE getInstance() noexcept;

	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;

		static constexpr const char* wndClassName = "This is my window I guess";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

private:
	static LRESULT CALLBACK handleMSGSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK handleMSGThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT handleMSG(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
	int width;
	int height;
	HWND hWnd;
};

// error exception macros
#define CHWND_EXCEPT(hr) Window::Exception(__LINE__, __FILE__, hr)
#define CHWND_LAST_EXCEPT() Window::Exception(__LINE__, __FILE__, GetLastError())