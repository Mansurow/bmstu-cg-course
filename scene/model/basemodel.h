#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <vector>
#include "geometry/geometry.hpp"
#include "color/color.h"

struct Material
{
    Color diffuse_;
    Color specular_;
    int shininess_;
};

struct ModelAttributes
{
    double lengthTop;
    double lengthBot;
    double height;
    int nVerts;
};

using namespace GeometrySpace;

class BaseModel
{
public:
    BaseModel();
    virtual ~BaseModel();
    inline int countVertices() const { return vertices_.size(); }
    inline int countTriangles() const { return triangles_.size() / 3; }
    inline Vector3D<double> getTranslation() const { return translation_; }
    inline Matrix<double> getRotation() const { return rotation_; }
    inline void setTranslation(const Vector3D<double> &translation) { translation_ = translation; }
    inline void setRotation(const Matrix<double> &rotation) { rotation_ = rotation; }
    inline void setMaterial(const Material &material) { material_ = material; }
    void getTriangle(std::vector<Vector3D<double>> &triangle, const int &num) const;
    inline Vector3D<double> getNormal(const int &index) const { return normals_[index]; }
    inline Material getMaterial() const { return material_; }
    virtual void changeVerticesCount(const int &verts) = 0;
    virtual void changeTopLength(const double &length) = 0;
    virtual void changeBotLength(const double &length) = 0;
    virtual void changeHeight(const double &height) = 0;
protected:
    virtual void computeVertices(const ModelAttributes &attributes) = 0;
    virtual void computeTriangles(const int &verts) = 0;
    virtual void computeNormals(const int &verts) = 0;
    virtual ModelAttributes getAttributes() const = 0 ;
    std::vector<Vector3D<double>> vertices_;
    std::vector<int> triangles_;
    std::vector<Vector3D<double>> normals_;
    Matrix<double> rotation_;
    Vector3D<double> translation_;
    Material material_;
};

#endif // BASEMODEL_H
