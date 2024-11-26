#include <iostream>
#include <string>
#include <functional>
#include <chrono>
#include <thread>
#include <cmath>
#include <windows.h>

// ����������� ������� ����� ��� ���������, �� �������� ����� ������������� ��� ������������� ����������.
class PlatformCanvas {
public:
    virtual ~PlatformCanvas() {}

    // ����� ����������� ������, ������� ����� �������������� � ������ ���������.
    virtual void Clear() = 0;  // �������� �����
    virtual void PutPixel(int x, int y, COLORREF color) = 0;  // ���������� ������� � ������������ ������� � �������� ������
    virtual void Initialize(size_t width, size_t height, const std::wstring& title) = 0;  // ������������� ���� � ��������� �����������
    virtual void ProcessEvents() = 0;  // ��������� ������� (��������, ������� ������)
    virtual bool ShouldQuit() const = 0;  // ��������, ����� �� ��������� ���������� (��������, �� ������� Escape)
    virtual void DrawText(int x, int y, const std::wstring& text, COLORREF color) = 0;  // ��������� ������ �� ������

    // ������ ��� ��������� �������� ���� (������ ���������� int).
    virtual int GetWidth() const = 0;  // ���������� ������ ����
    virtual int GetHeight() const = 0;  // ���������� ������ ����
};

// ����� ��� Windows-���������, ��������� ������ ��� ��������� �� ������ � Windows.
class WindowsCanvas : public PlatformCanvas {
public:
    WindowsCanvas() : hwnd(nullptr), hInstance(GetModuleHandle(NULL)), width(800), height(600) {}

    // ������������� ���� � ��������� ��������� � ���������
    void Initialize(size_t width, size_t height, const std::wstring& title) override {
        this->width = width;  // ��������� ���������� �������
        this->height = height;

        WNDCLASS wc = {};
        wc.lpfnWndProc = StaticWindowProc;  // ��������� ���������� ��������� ����
        wc.hInstance = hInstance;
        wc.lpszClassName = L"CanvasWindowClass";  // ��� ������ ����
        RegisterClass(&wc);  // ������������ ����� ����

        // ������� ���� � ���������� �����������
        hwnd = CreateWindowEx(
            0, L"CanvasWindowClass", title.c_str(),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,  // ����������� ����� ����
            CW_USEDEFAULT, CW_USEDEFAULT, static_cast<int>(width), static_cast<int>(height),
            NULL, NULL, hInstance, this
        );
    }

    // ������� ������
    void Clear() override {
        HDC hdc = GetDC(hwnd);  // �������� �������� ���������� ��� ���������
        RECT rect;
        GetClientRect(hwnd, &rect);  // �������� ������� ���������� ������� ����
        FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW + 1));  // ��������� ����� �����
        ReleaseDC(hwnd, hdc);  // ����������� �������� ����������
    }

    // ��������� ������� �� �����
    void PutPixel(int x, int y, COLORREF color) override {
        HDC hdc = GetDC(hwnd);  // �������� �������� ����������
        SetPixel(hdc, x, y, color);  // ������������� ������� � �������� �������
        ReleaseDC(hwnd, hdc);  // ����������� �������� ����������
    }

    // ��������� ������ �� ������
    void DrawText(int x, int y, const std::wstring& text, COLORREF color) override {
        HDC hdc = GetDC(hwnd);  // �������� �������� ����������
        SetTextColor(hdc, color);  // ������������� ���� ������
        TextOut(hdc, x, y, text.c_str(), static_cast<int>(text.length()));  // ������������ �����
        ReleaseDC(hwnd, hdc);  // ����������� �������� ����������
    }

    // ��������� ��������� � ������� ����
    void ProcessEvents() override {
        MSG msg;
        while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {  // �������� �� ������� ���������
            TranslateMessage(&msg);
            DispatchMessage(&msg);  // ��������� ���������
        }
    }

    // ��������, ����� �� ��������� ���������� (��������, �� ������� ������� Escape)
    bool ShouldQuit() const override {
        return (GetAsyncKeyState(VK_ESCAPE) & 0x8000);  // ��������� ��������� ������� Escape
    }

    // ���������� ������ ����
    int GetWidth() const override {
        return static_cast<int>(width);  // ���������� ������ ���� ��� int
    }

    // ���������� ������ ����
    int GetHeight() const override {
        return static_cast<int>(height);  // ���������� ������ ���� ��� int
    }

protected:
    HWND hwnd;  // ���������� ����
    HINSTANCE hInstance;  // ���������� ����������
    size_t width, height;  // ������ � ������ ����

    // ����������� ������� ��� ��������� ��������� ����
    static LRESULT CALLBACK StaticWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if (uMsg == WM_NCCREATE) {
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            WindowsCanvas* pCanvas = reinterpret_cast<WindowsCanvas*>(pCreate->lpCreateParams);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pCanvas);  // ��������� ��������� �� ������ Canvas
        }
        else {
            WindowsCanvas* pCanvas = reinterpret_cast<WindowsCanvas*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            if (pCanvas && uMsg == WM_DESTROY) {
                PostQuitMessage(0);  // ��������� ����
                return 0;
            }
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);  // ������������ ����������� ���������
    }
};

// �������� ��� LinuxCanvas (��������� ������ ����� � �������)
class LinuxCanvas : public PlatformCanvas {
public:
    LinuxCanvas() {}

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
        return false;
    }

    int GetWidth() const override {
        return 800;  // �������� ��� Linux
    }

    int GetHeight() const override {
        return 600;  // �������� ��� Linux
    }
};

// ����� Canvas, ������� ���������� PlatformCanvas ��� ��������� �� ������.
class Canvas {
public:
    // ����������� ��������� ������������� �����, ������� ����� �������������� ��� ���������.
    Canvas(PlatformCanvas* platformCanvas) : platformCanvas(platformCanvas), clearScreen(true) {}

    // �������� ���� ����������, ��� ���������� ���������.
    void Run(std::function<void(Canvas&)> drawFunc, int targetFPS) {
        platformCanvas->Initialize(800, 600, L"Canvas Application");  // ������������� ���� � ��������� 800x600

        auto frameDuration = std::chrono::milliseconds(1000 / targetFPS);  // ���������� ����������������� ������ �����
        auto lastTime = std::chrono::high_resolution_clock::now();
        int frameCount = 0;

        while (true) {
            platformCanvas->ProcessEvents();  // ��������� �������
            if (platformCanvas->ShouldQuit()) {
                break;  // ����� �� �����, ���� ����� ��������� ���������
            }

            if (clearScreen) {
                platformCanvas->Clear();  // ������� ������
            }

            drawFunc(*this);  // ����� ���������� ������� ��� ���������
            frameCount++;

            std::this_thread::sleep_for(frameDuration);  // ����������� FPS
        }
    }

    // ������� ������
    void Clear() {
        platformCanvas->Clear();
    }

    // ������������� ������ ������� ������ (��������, ����� �� ������� ����� ������ ���)
    void ToggleClearScreen() {
        clearScreen = !clearScreen;
    }

    // ��������� ������ ����
    int GetWidth() const {
        return platformCanvas->GetWidth();  // �������� ����� �������������� ������
    }

    // ��������� ������ ����
    int GetHeight() const {
        return platformCanvas->GetHeight();  // �������� ����� �������������� ������
    }


    // ��������� ������� �� ������
    void PutPixel(int x, int y, COLORREF color) {
        platformCanvas->PutPixel(
            GetWidth() / 2 + x,
            GetHeight() / 2 + y,
            color);
    }

private:
    PlatformCanvas* platformCanvas;  // ��������� �� ������������� ����� ��� ���������
    bool clearScreen;  // ����, �����������, ����� �� ������� ����� ��� ������ �����
};


