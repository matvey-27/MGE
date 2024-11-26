#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <cmath>
#ifdef _WIN32
#include <windows.h>
#endif

// ����������� ������� ����� ��� ���������
class PlatformCanvas {
public:
    virtual ~PlatformCanvas() {}
    virtual void Clear() = 0;
    virtual void PutPixel(int x, int y, COLORREF color) = 0;
    virtual void Initialize(size_t width, size_t height, const std::wstring& title) = 0; // ���������� size_t
    virtual void ProcessEvents() = 0; // ��������� �������
    virtual bool ShouldQuit() const = 0; // �������� �� ����������
    virtual void DrawText(int x, int y, const std::wstring& text, COLORREF color) = 0; // ����� ����� ��� ��������� ������
};

// ����� ��� Windows
class WindowsCanvas : public PlatformCanvas {
public:
    WindowsCanvas() : hwnd(nullptr), hInstance(GetModuleHandle(NULL)) {}

    void Initialize(size_t width, size_t height, const std::wstring& title) override {
        WNDCLASS wc = {};
        wc.lpfnWndProc = StaticWindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"CanvasWindowClass";
        RegisterClass(&wc);

        hwnd = CreateWindowEx(
            0, L"CanvasWindowClass", title.c_str(),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(width), static_cast<int>(height), // ���������� � int
            NULL, NULL, hInstance, this
        );
    }

    void Clear() override {
        HDC hdc = GetDC(hwnd);
        RECT rect;
        GetClientRect(hwnd, &rect);
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));
        ReleaseDC(hwnd, hdc);
    }

    void PutPixel(int x, int y, COLORREF color) override {
        HDC hdc = GetDC(hwnd);
        SetPixel(hdc, x, y, color);
        ReleaseDC(hwnd, hdc);
    }

    void DrawText(int x, int y, const std::wstring& text, COLORREF color) override {
        HDC hdc = GetDC(hwnd);
        SetTextColor(hdc, color);
        TextOut(hdc, x, y, text.c_str(), static_cast<int>(text.length()));
        ReleaseDC(hwnd, hdc);
    }

    void ProcessEvents() override {
        MSG msg;
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) { // ����������: �������� hwnd
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    bool ShouldQuit() const override {
        return (GetAsyncKeyState(VK_ESCAPE) & 0x8000);
    }

protected:
    HWND hwnd;
    HINSTANCE hInstance;

    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            WindowsCanvas* pCanvas = reinterpret_cast<WindowsCanvas*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pCanvas);
        }
        else {
            WindowsCanvas* pCanvas = reinterpret_cast<WindowsCanvas*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if (pCanvas && uMsg == WM_DESTROY) {
                PostQuitMessage(0);
                return 0;
            }
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
};

// �������� ��� LinuxCanvas
class LinuxCanvas : public PlatformCanvas {
public:
    LinuxCanvas() {
        // ����� ������ ���� ������������� ��� Linux
    }

    void Initialize(size_t width, size_t height, const std::wstring& title) override {
        std::wcout << L"LinuxCanvas: ������������� ���� " << title << L" (" << width << L"x" << height << L")" << std::endl;
    }

    void Clear() override {
        std::wcout << L"LinuxCanvas: ������� ������" << std::endl;
    }

    void PutPixel(int x, int y, COLORREF color) override {
        std::wcout << L"LinuxCanvas: ��������� ������� (" << x << L", " << y << L") ����: " << color << std::endl;
    }

    void DrawText(int x, int y, const std::wstring& text, COLORREF color) override {
        std::wcout << L"LinuxCanvas: ��������� ������ '" << text << L"' � ������� (" << x << L", " << y << L") ����: " << color << std::endl;
    }

    void ProcessEvents() override {
        // ����� ������ ���� ��������� ������� ��� Linux
    }

    bool ShouldQuit() const override {
        return false; // ��������
    }
};

// ����� Canvas, ������� ���������� PlatformCanvas
class Canvas {
public:
    Canvas(PlatformCanvas* platformCanvas) : platformCanvas(platformCanvas), clearScreen(true) {}

    void Run(std::function<void(Canvas&)> drawFunc, int targetFPS) {
        platformCanvas->Initialize(800, 600, L"Canvas Application"); // �������� �� L""

        auto frameDuration = std::chrono::milliseconds(1000 / targetFPS);
        auto lastTime = std::chrono::high_resolution_clock::now();
        int frameCount = 0;

        while (true) {
            platformCanvas->ProcessEvents();
            if (platformCanvas->ShouldQuit()) {
                break; // ����� �� �����
            }

            if (clearScreen) {
                platformCanvas->Clear();
            }

            drawFunc(*this);
            frameCount++;

            // ���������� � ����������� FPS
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = currentTime - lastTime;
            if (elapsed.count() >= 1.0) {
                std::wstring fpsText = L"FPS: " + std::to_wstring(frameCount);
                platformCanvas->DrawText(10, 10, fpsText, RGB(255, 0, 0)); // ����������� FPS � ������� ����� ����
                frameCount = 0;
                lastTime = currentTime;
            }

            std::this_thread::sleep_for(frameDuration); // ����������� FPS
        }
    }

    void Clear() {
        platformCanvas->Clear();
    }

    void PutPixel(int x, int y, COLORREF color) {
        platformCanvas->PutPixel(x, y, color);
    }

    void DrawCircle(int centerX, int centerY, int radius, COLORREF color) {
        for (int angle = 0; angle < 360; angle++) {
            int x = centerX + static_cast<int>(radius * cos(angle * 3.14159 / 180));
            int y = centerY + static_cast<int>(radius * sin(angle * 3.14159 / 180));
            PutPixel(x, y, color);
        }
    }

    void ToggleClearScreen() {
        clearScreen = !clearScreen; // ������������ ����� ������� ������
    }

private:
    PlatformCanvas* platformCanvas; // ��������� �� ������������� �����
    bool clearScreen; // ���� ��� ���������� �������� ������
};
