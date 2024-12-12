#include "Model.h"

Model::Model(Point3D<float> vertices[], int count_v, Point3D<int> triangles[], int count_t) {
    this->count_v = count_v;
    this->count_t = count_t;

    this->vertices = new Point3D<float>[count_v];
    for (size_t i = 0; i < count_v; ++i) {
        this->vertices[i] = vertices[i];
    }

    this->triangles = new Point3D<int>[count_t];
    for (size_t i = 0; i < count_t; ++i) {
        this->triangles[i] = triangles[i];
    }
}

Model::~Model() {
    delete[] vertices; // ������������ ������
    vertices = nullptr;

    delete[] triangles; // ������������ ������
    triangles = nullptr;
}

Model::Model(const Model& other) {
    count_v = other.count_v;
    count_t = other.count_t;
    vertices = new Point3D<float>[count_v];
    for (size_t i = 0; i < count_v; ++i) {
        vertices[i] = other.vertices[i];
    }
    triangles = new Point3D<int>[count_t];
    for (size_t i = 0; i < count_t; ++i) {
        triangles[i] = other.triangles[i];
    }
}

Model& Model::operator=(const Model& other)
{
    if (this != &other) {
        delete[] vertices;
        delete[] triangles;

        count_v = other.count_v;
        count_t = other.count_t;
        vertices = new Point3D<float>[count_v];
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
void Model::addVertex(const Point3D<float>& point) {
    addElement(vertices, count_v, point);
}

// ����� ��� ��������� ���������� ������
size_t Model::getVertexCount() const {
    return count_v;
}

// ����� ��� ������� � �������
Point3D<float>&  Model::getVertex(size_t index) {
    if (index >= count_v) {
        throw std::out_of_range("Index out of range");
    }
    return vertices[index];
}

// ����� ��� ���������� ����� �������������
void Model::addTriangls(const Point3D<int>& point) {
    addElement(triangles, count_t, point);
}

// ����� ��� ��������� ���������� �������������
size_t Model::getTrianglsCount()
{
    return count_t;
}

// ����� ��� ������� � �������������
Point3D<int>& Model::getTriangls(size_t index) {
    if (index >= count_t) {
        throw std::out_of_range("Index out of range");
    }
    return triangles[index];
}

// ����� ��� ����������� 
void Model::translate(int dx, int dy, int dz) {
    for (size_t i = 0; i < count_v; ++i) {
        vertices[i].x += dx;
        vertices[i].y += dy;
        vertices[i].z += dz;
    }
}

// ����� ��� ����������� 
void Model::translate(Vector3D<float> v) {
    for (size_t i = 0; i < count_v; i += 1) {
        vertices[i] += v;
    }
}
