#include "Utils.h"

template <typename T>
void my_fun::addElement(T*& arr, int& size, T newElement) {
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
void my_fun::addElement(T*& arr, size_t& size, T newElement) {
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
void my_fun::swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}