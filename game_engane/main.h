#pragma once
#include "math/math.h"
#include "canvas/windows/canvas.h"


namespace my_fun {
    template <typename T>
    void addElement(T*& arr, int& size, T newElement) {
        // ������� ����� ������ � ����������� ������������
        T* newArr = new T[size + 1];

        // �������� ������ �������� � ����� ������
        for (int i = 0; i < size; ++i) {
            newArr[i] = arr[i];
        }

        // ��������� ����� �������
        newArr[size] = newElement;

        // ����������� ������ ������� �������
        delete[] arr;

        // ��������� �� ����� ������ � ��������� ������
        arr = newArr;
        size++; // ����������� ������
    }

    template <typename T>
    void addElement(T*& arr, size_t& size, T newElement) {
        // ������� ����� ������ � ����������� ������������
        T* newArr = new T[size + 1];

        // �������� ������ �������� � ����� ������
        for (int i = 0; i < size; ++i) {
            newArr[i] = arr[i];
        }

        // ��������� ����� �������
        newArr[size] = newElement;

        // ����������� ������ ������� �������
        delete[] arr;

        // ��������� �� ����� ������ � ��������� ������
        arr = newArr;
        size++; // ����������� ������
    }

    template <typename T>
    void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }
}

using namespace std;
using namespace math;
using namespace my_fun;
using namespace canvas;

class Model {
private:
    Point3D<int>* vertices;
    size_t count_v; // ���������� ������� ������
    Point3D<int>* triangles;
    size_t count_t; // ���������� ������� ������

public:
    Model() : vertices(new Point3D<int>[0]), count_v(0), triangles(new Point3D<int>[0]), count_t(0) {};

    Model(Point3D<int> vertices[], int count_v, Point3D<int> triangles[], int count_t) {
        this->count_v = count_v;
        this->count_t = count_t;

        this->vertices = new Point3D<int>[count_v];
        for (size_t i = 0; i < count_v; ++i) {
            this->vertices[i] = vertices[i];
        }

        this->triangles = new Point3D<int>[count_t];
        for (size_t i = 0; i < count_t; ++i) {
            this->triangles[i] = triangles[i];
        }
    }

    ~Model() {
        delete[] vertices; // ������������ ������
        vertices = nullptr;

        delete[] triangles; // ������������ ������
        triangles = nullptr;
    }

    Model(const Model& other) {
        count_v = other.count_v;
        count_t = other.count_t;
        vertices = new Point3D<int>[count_v];
        for (size_t i = 0; i < count_v; ++i) {
            vertices[i] = other.vertices[i];
        }
        triangles = new Point3D<int>[count_t];
        for (size_t i = 0; i < count_t; ++i) {
            triangles[i] = other.triangles[i];
        }
    }

    Model& operator=(const Model& other) {
        if (this != &other) {
            delete[] vertices;
            delete[] triangles;

            count_v = other.count_v;
            count_t = other.count_t;
            vertices = new Point3D<int>[count_v];
            for (size_t i = 0; i < count_v; ++i) {
                vertices[i] = other.vertices[i];
            }
            triangles = new Point3D<int>[count_t];
            for (size_t i = 0; i < count_t; ++i) {
                triangles[i] = other.triangles[i];
            }
        }
        return *this;
    }

    // ����� ��� ���������� ����� �������
    void addVertex(const Point3D<int>& point) {
        addElement(vertices, count_v, point);
    }

    // ����� ��� ��������� ���������� ������
    size_t getVertexCount() const {
        return count_v;
    }

    // ����� ��� ������� � �������
    Point3D<int>& getVertex(size_t index) {
        if (index >= count_v) {
            throw std::out_of_range("Index out of range");
        }
        return vertices[index];
    }

    // ����� ��� ���������� ����� �������������
    void addTriangls(const Point3D<int>& point) {
        addElement(triangles, count_t, point);
    }

    // ����� ��� ��������� ���������� �������������
    size_t getTrianglsCount() const {
        return count_t;
    }

    // ����� ��� ������� � �������������
    Point3D<int>& getTriangls(size_t index) {
        if (index >= count_t) {
            throw std::out_of_range("Index out of range");
        }
        return triangles[index];
    }

    void translate(int dx, int dy, int dz) {
        for (size_t i = 0; i < count_v; ++i) {
            vertices[i].x += dx;
            vertices[i].y += dy;
            vertices[i].z += dz;
        }
    }

    void translate(Vector3D<int> v) {
        for (size_t i = 0; i < count_v; i+=1) {
            vertices[i] += v;
        }
    }
};

int* interpolated(int i0, int d0, int i1, int d1, int& size) {
    size = abs(i1 - i0) + 1; // ���������� ����������������� ��������
    int* values = new int[size]; // ������� ������ ������� �������

    // ���������, ���� i0 ����� i1
    if (i0 == i1) {
        values[0] = d0; // ��������� d0 � ������
    }
    else {
        int delta = d1 - d0;
        for (int i = 0; i < size; ++i) {
            values[i] = d0 + (delta * i) / (i1 - i0); // �������� ������������
        }
    }

    return values; // ���������� ������
}

// ��������� �����
void DrawLine(Canvas canvas, Point2D<int> P0, Point2D<int> P1, rgb_color color = rgb_color(0, 0, 0)) {
    int dx = P1.x - P0.x;
    int dy = P1.y - P0.y;

    if (std::abs(dx) > abs(dy)) {
        if (P0.x > P1.x) {
            my_fun::swap(P0, P1);
        }

        int size;
        int* ys = interpolated(P0.x, P0.y, P1.x, P1.y, size);

        for (int x = P0.x; x <= P1.x; ++x) {
            int i = x - P0.x;
            canvas.PutPixel(x, ys[i], color);
        }

        delete[] ys; // ����������� ������
    }
    else {
        if (P0.y > P1.y) {
            my_fun::swap(P0, P1);
        }

        int size;
        int* xs = interpolated(P0.y, P0.x, P1.y, P1.x, size);

        for (int y = P0.y; y <= P1.y; ++y) {
            int i = y - P0.y;
            canvas.PutPixel(xs[i], y, color);
        }

        delete[] xs; // ����������� ������
    }
}

// ��������� ������� ������������ 
void DrawWireframeTringle(Canvas canvas, Point2D<int> P0, Point2D<int> P1, Point2D<int> P2, rgb_color color = rgb_color(0, 0, 0))
{
    DrawLine(canvas, P0, P1, color);
    DrawLine(canvas, P1, P2, color);
    DrawLine(canvas, P2, P0, color);

}

// ��������� �����
void DrawCircle(Canvas canvas, int centerX, int centerY, int radius, rgb_color color = rgb_color(0, 0, 0)) {
    for (int angle = 0; angle < 360; angle++) {
        int x = centerX + static_cast<int>(radius * cos(angle * 3.14159 / 180));
        int y = centerY + static_cast<int>(radius * sin(angle * 3.14159 / 180));
        canvas.PutPixel(x, y, color);  // ������ ������ ������� �����
    }
}
