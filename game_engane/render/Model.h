#pragma once
#include "../math/Vector.h"
#include "../math/Point.h"
#include "../Utils.h"
#include <iostream>

using namespace my_fun;
using namespace math;

class Model {
private:
    Point3D<float>* vertices;
    size_t count_v; // ���������� ������� ������
    Point3D<int>* triangles;
    size_t count_t; // ���������� ������� ������

public:
    Model() : vertices(new Point3D<float>[0]), count_v(0), triangles(new Point3D<int>[0]), count_t(0) {};

    Model(Point3D<float> vertices[], int count_v, Point3D<int> triangles[], int count_t);

    ~Model();

    Model(const Model& other);

    Model& operator=(const Model& other);

    // ����� ��� ���������� ����� �������
    void addVertex(const Point3D<float>& point);

    // ����� ��� ��������� ���������� ������
    size_t getVertexCount() const;

    // ����� ��� ������� � �������
    Point3D<float>& getVertex(size_t index);

    // ����� ��� ���������� ����� �������������
    void addTriangls(const Point3D<int>& point);

    // ����� ��� ��������� ���������� �������������
    size_t getTrianglsCount();

    // ����� ��� ������� � �������������
    Point3D<int>& getTriangls(size_t index);

    void translate(int dx, int dy, int dz);

    void translate(Vector3D<float> v);
};

