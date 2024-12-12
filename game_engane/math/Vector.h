#pragma once

#include <iostream>
#include <cmath>

namespace math {
    template <typename T>
    class Point2D; // ��������������� ���������� ������ Point2D

    template <typename T>
    class Point3D; // ��������������� ���������� ������ Point3D

    // Vector3D � Vector2D

    template <typename T>
    class Vector2D {
    public:
        T x, y;

        Vector2D() : x(0), y(0) {}
        Vector2D(T x, T y) : x(x), y(y) {};

        void print_vector();//����� ������� � �������
    };

    template <typename T>
    class Vector3D : public Vector2D<T> {
    public:
        T z;
        Vector3D() : Vector2D<T>(0, 0), z(0) {};
        Vector3D(T x, T y, T z) : Vector2D<T>(x, y), z(z) {};

        void print_vector(); //����� ������� � �������

        Point3D<T> operator+(Point3D<T> p); //�������� ����� � �������

        Vector3D<T> operator+(Vector3D<T> vec); //�������� ��������

        Vector3D<T> operator*(T num); // ��������� ������� �� �����

        T length(); // ����� ��� ���������� ������(�����) �������

        T scoliar_product(Vector3D<T> vec); // ��������� ������������
        Vector3D<T> vector_product(Vector3D<T> vec); // ��������� ������������
    };
}