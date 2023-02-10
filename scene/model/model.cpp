#include "model.h"
#include "model.h"

Cube::Cube()
{
}

Cube::Cube(const ModelAttributes &attributes, const Material &material)
{
    computeVertices(attributes);
    computeTriangles(attributes.nVerts);
    computeNormals(attributes.nVerts);
    material_ = material;
}

void Cube::changeVerticesCount(const int &nVerts)
{
    ModelAttributes attributes;
    attributes.lengthBot = vertices_[0].x();
    attributes.lengthTop = vertices_[countVertices() / 2].x();
    attributes.height = -vertices_[0].y() * 2;
    attributes.nVerts = nVerts;
    vertices_.clear();
    triangles_.clear();
    normals_.clear();
    computeVertices(attributes);
    computeTriangles(nVerts);
    computeNormals(nVerts);
}

void Cube::changeTopLength(const double &length)
{
    int verts = countVertices();
    double d_angle = 4 * M_PI / verts;
    double angle = 0;
    for (int i = verts / 2; i < verts; i++)
    {
        vertices_[i].setX(length * cos(angle));
        vertices_[i].setZ(length * sin(angle));
        angle -= d_angle;
    }
    normals_.clear();
    computeNormals(countVertices()/2);
}

void Cube::changeBotLength(const double &length)
{
    int verts = countVertices() / 2;
    double d_angle = 2 * M_PI / verts;
    double angle = 0;
    for (int i = 0; i < verts; i++)
    {
        vertices_[i].setX(length * cos(angle));
        vertices_[i].setZ(length * sin(angle));
        angle += d_angle;
    }
    normals_.clear();
    computeNormals(countVertices()/2);
}

void Cube::changeHeight(const double &height)
{
    int verts = countVertices() / 2;
    double half_height = height / 2;
    for (int i = 0; i < verts; i++)
    {
        vertices_[i].setY(-half_height);
        vertices_[i + verts].setY(half_height);
    }
    normals_.clear();
    computeNormals(countVertices()/2);
}

ModelAttributes Cube::getAttributes() const
{
    int verts = countVertices() / 2;
    ModelAttributes attributes;
    attributes.lengthBot = vertices_[0].x();
    attributes.lengthTop = vertices_[verts].x();
    attributes.height = vertices_[verts].y() * 2;
    attributes.nVerts = verts;
    return attributes;
}

void Cube::computeVertices(const ModelAttributes &attributes)
{
    double d_angle = 2 * M_PI / attributes.nVerts;
    double angle = 0;
    double half_height = attributes.height / 2;
    for (int i = 0; i < attributes.nVerts; i++)
    {
        vertices_.emplace_back(attributes.lengthBot * cos(angle), -half_height, attributes.lengthBot * sin(angle));
        angle += d_angle;
    }
    angle = 0;
    for (int i = 0; i < attributes.nVerts; i++)
    {
        vertices_.emplace_back(attributes.lengthTop * cos(angle), half_height, attributes.lengthTop * sin(angle));
        angle -= d_angle;
    }
}

void Cube::computeTriangles(const int &nVerts)
{
    for (int i = 0; i < nVerts - 2; i++)
    {
        triangles_.push_back(0);
        triangles_.push_back(i + 1);
        triangles_.push_back(i + 2);
        triangles_.push_back(nVerts);
        triangles_.push_back(nVerts + i + 1);
        triangles_.push_back(nVerts + i + 2);
    }
    for (int i = 0; i < nVerts; i++)
    {
        int first = nVerts + i;
        int second = nVerts - i;
        int third = nVerts - i - 1;
        int fourth = nVerts + i + 1;
        second = second == nVerts ? 0 : second;
        fourth = fourth == nVerts * 2 ? nVerts : fourth;
        triangles_.push_back(first);
        triangles_.push_back(second);
        triangles_.push_back(third);
        triangles_.push_back(first);
        triangles_.push_back(third);
        triangles_.push_back(fourth);
    }
}

void Cube::computeNormals(const int &nVerts)
{
    Vector3D<double> normal;
    for (int i = 0; i < nVerts - 2; i++)
    {
        normal = Vector3D<double>(vertices_[i + 1] - vertices_[0]) ^ Vector3D<double>(vertices_[i + 2] - vertices_[i + 1]);
        normal.normalize();
        normals_.push_back(normal);
        normal = Vector3D<double>(vertices_[nVerts + i + 1] - vertices_[nVerts]) ^ Vector3D<double>(vertices_[nVerts + i + 2] - vertices_[nVerts + i + 1]);
        normal.normalize();
        normals_.push_back(normal);
    }
    for (int i = 0; i < nVerts; i++)
    {
        int first = nVerts + i;
        int second = nVerts - i;
        int third = nVerts - i - 1;
        int fourth = nVerts + i + 1;
        second = second == nVerts ? 0 : second;
        fourth = fourth == nVerts * 2 ? nVerts : fourth;
        normal = Vector3D<double>(vertices_[second] - vertices_[first]) ^ Vector3D<double>(vertices_[third] - vertices_[second]);
        normal.normalize();
        normals_.push_back(normal);
        normal = Vector3D<double>(vertices_[third] - vertices_[first]) ^ Vector3D<double>(vertices_[fourth] - vertices_[third]);
        normal.normalize();
        normals_.push_back(normal);
    }
}
