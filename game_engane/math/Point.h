#pragma once
#include <iostream>

namespace math {
    template <typename T>
    class Vector2D; // ��������������� ���������� ������ Vector2D

    template <typename T>
    class Vector3D; // ��������������� ���������� ������ Vector3D

    // Point3D � Point2D
    template <typename T>
    class Point2D {
    public:
        T x, y;
    public:
        Point2D() : x(0), y(0) {}
        Point2D(T x, T y) : x(x), y(y) {};
        ~Point2D() = default;

        void print_position();//����� ����� � �������
    };

    template <typename T>
    class Point3D : public Point2D<T> {
    public:
        T z;
    public:
        Point3D() : Point2D<T>(0, 0), z(0) {};
        Point3D(T x, T y, T z) : Point2D<T>(x, y), z(z) {};
        ~Point3D() = default;

        // �������� += ��� ���������� �������
        Point3D<T>& operator+=(const Vector3D<T>& v) {
            this->x += v.x;
            this->y += v.y;
            this->z += v.z;
            return *this; // ���������� this
        }

        Vector3D<T> operator-(Point3D<T> p); //��������� �����

        void print_position();//����� ����� � �������
    };
}
