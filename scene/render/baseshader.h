#ifndef BASESHADER_H
#define BASESHADER_H

#include <vector>
#include "geometry/geometry.hpp"
#include "color/color.h"
#include "scene/render/shadowcube.h"

using namespace GeometrySpace;

class BaseShader
{
public:
    BaseShader() {}
    virtual int vertex(std::vector<Vector4D<double>> &result, const std::vector<Vector3D<double>> &triangle, const Vector3D<double> &normal) = 0;
    virtual void geometry(const std::vector<Vector4D<double>> &triangle) = 0;
    virtual Color fragment(const Vector3D<double> &barycentric, const ShadowCube &shadowCube) const = 0;
    virtual ~BaseShader() {}
};

#endif // BASESHADER_H
