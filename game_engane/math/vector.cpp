#include "math.h"
#include <iostream>
#include <cmath>

using namespace std;
using namespace math;

// vector_2D

template <typename T>
void vector_2D<T>::print_vector() {
    // ����� ��������� �������
    cout << "x = " << this->x << endl;
    cout << "y = " << this->y << endl;
}

// vector_3D

template <typename T>
void vector_3D<T>::print_vector() {
    // ����� ��������� �������
    cout << "x = " << this->x << endl;
    cout << "y = " << this->y << endl;
    cout << "z = " << this->z << endl;
}

// �������� �������� ��������

template <typename T>
point_3D<T> vector_3D<T>::operator+(point_3D<T> p) {
    // �������� ��������� ��������
    p.x += this->x;
    p.y += this->y;
    p.z += this->z;

    return p;
}

// �������� �������� ��������

template <typename T>
vector_3D<T> vector_3D<T>::operator+(vector_3D<T> vec) {
    // �������� ��������� ��������
    return vector_3D<T>(this->x + vec.x,
        this->y + vec.y,
        this->z + vec.z);
}

// �������� ��������� ������� �� ������

template <typename T>
vector_3D<T> vector_3D<T>::operator*(T num) {
    // ��������� ��������� ������� �� ������
    return vector_3D<T>(this->x * num,
        this->y * num,
        this->z * num);
}

// ����� �������

template <typename T>
T vector_3D<T>::length() {
    // ���������� ����� �������
    return std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

// ��������� ������������ ��������

template <typename T>
T vector_3D<T>::scoliar_product(vector_3D<T> vec) {
    // ���������� ���������� ������������ ��������
    return this->x * vec.x + this->y * vec.y + this->z * vec.z;
}

// ��������� ������������ ��������

template <typename T>
vector_3D<T> vector_3D<T>::vector_product(vector_3D<T> vec) {
    // ���������� ���������� ������������ ��������
    return vector_3D<T>(this->y * vec.z - this->z * vec.y,
        this->z * vec.x - this->x * vec.z,
        this->x * vec.y - this->y * vec.x);
}
