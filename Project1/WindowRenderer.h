#pragma once
#include <Windows.h>
#include "Points.h"

class WindowRenderer
{
private:
	HWND hwnd;                 // Дескриптор вікна
	const Points& scene;   // Посилання на клас із точками (константне)

    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
    {
        WindowRenderer* pThis = nullptr;

        if (uMsg == WM_NCCREATE) 
        {
            // При створенні вікна дістаємо вказівник на об'єкт класу (this), який ми передали у CreateWindowEx
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            pThis = reinterpret_cast<WindowRenderer*>(pCreate->lpCreateParams);
            // Зберігаємо його всередині структури вікна Windows
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        }
        else 
        {
            // Для всіх наступних повідомлень дістаємо збережений вказівник на наш клас
            pThis = reinterpret_cast<WindowRenderer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        }

        if (pThis) 
        {
            // Перенаправляємо обробку в звичайний (не статический) метод класу
            return pThis->WindowProc(hwnd, uMsg, wParam, lParam);
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) 
        {
        case WM_PAINT: 
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Отримуємо точки з нашого класу даних
            const auto& points = scene.getPoints();

            // Малюємо кожну точку на екрані
            for (const auto& pt : points) 
            {
                SetRoughPoint(hdc, pt);
                //SetPixel(hdc, pt.x, pt.y, RGB(pt.col.R, pt.col.G, pt.col.B));
            }

            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void SetRoughPoint(HDC hdc, Point point)
    {
        SetPixel(hdc, point.x, point.y, RGB(point.col.R, point.col.G, point.col.B));
        SetPixel(hdc, point.x-1, point.y, RGB(point.col.R, point.col.G, point.col.B));
        SetPixel(hdc, point.x+1, point.y, RGB(point.col.R, point.col.G, point.col.B));
        SetPixel(hdc, point.x, point.y-1, RGB(point.col.R, point.col.G, point.col.B));
        SetPixel(hdc, point.x, point.y+1, RGB(point.col.R, point.col.G, point.col.B));
    }
public:
	WindowRenderer(const Points& dots) : 
		hwnd(nullptr), 
		scene(dots) 
	{}

    bool Init(const wchar_t* title, int width, int height) 
    {
        HINSTANCE hInstance = GetModuleHandle(NULL);
        const wchar_t CLASS_NAME[] = L"RendererWindowClass";

        WNDCLASS wc = {};
        wc.lpfnWndProc = StaticWindowProc; // Вказуємо наш статичний метод
        wc.hInstance = hInstance;
        wc.lpszClassName = CLASS_NAME;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);

        RegisterClass(&wc);

        // Створюємо вікно і передаємо `this` (вказівник на поточний об'єкт класу) як останній параметр
        hwnd = CreateWindowEx(
            0, CLASS_NAME, title,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            NULL, NULL, hInstance, this
        );

        return hwnd != nullptr;
    }

    void Run() 
    {
        if (!hwnd) return;

        ShowWindow(hwnd, SW_SHOW);

        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0) > 0) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
};